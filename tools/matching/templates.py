#!/usr/bin/env python3
"""Zero-token template tier: lift small branch-free ARM/Thumb functions to C and verify.

A symbolic executor walks a straight-line function (no local branches; ARM predicated
`movCC` pairs after a `cmp` become boolean/ternary values) and emits C:
  * params r0-r3 (pointer when used as a base, else s32), loads/stores of any width
    through `*(T *)((u8 *)base + off)`, literal-pool constants and symbol addresses
    (relocs.txt names), ALU ops incl. shifted operands, mla/mul/rsb/mvn/bic;
  * calls (bl/blx/ARM `b` tail calls) with arity inferred from argument registers
    written since the previous call, outgoing stack arguments (`str rX,[sp,#k]`),
    call results bound to temps;
  * loads/results that stay live across a store or call are bound to temps first.
Shapes covered: return const/arg, field get/set (incl. globals), two-arg arithmetic,
bool compare (ARM predicated), wrappers / tail calls, call sequences, store blocks.
Every function gets a few variants (param count, call arity, void vs value return,
temp binding) and each is verified with match.py; only passing files are kept in
src/auto/.

Idea from tangosdev/sm64ds-decomp tools/swarm.py + tools/sweep.py (MIT License,
Copyright (c) 2026 Tango): a free rule tier verified by the byte oracle. The rules
there are per-shape regexes over ARM; this is a generic lifter covering ARM and Thumb.

    python tools/matching/templates.py [--max 0x80] [--module ov000] [--dry-run] [-j 2]
    python tools/matching/templates.py --show func_ov000_022f18f4   # print variants
"""
from __future__ import annotations

import argparse
import itertools
import re
import sys
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

import capstone
from capstone import arm as A

sys.path.insert(0, str(Path(__file__).resolve().parent))
from accel_common import ROOT, Corpus, is_branch, pool_words, run_match  # noqa: E402

_MDD = {}


def mdd(thumb):
    if thumb not in _MDD:
        m = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_THUMB if thumb else capstone.CS_MODE_ARM)
        m.detail = True
        _MDD[thumb] = m
    return _MDD[thumb]


class Bail(Exception):
    pass


VARIANT_BAILS = {"missing stack arg", "no return value"}


REGN = {}


def rname(md, r):
    return md.reg_name(r)


LOADT = {"ldr": "u32", "ldrh": "u16", "ldrsh": "s16", "ldrb": "u8", "ldrsb": "s8"}
STORET = {"str": "u32", "strh": "u16", "strb": "u8"}
COND_C = {"eq": "==", "ne": "!=", "lt": "<", "ge": ">=", "gt": ">", "le": "<=",
          "lo": "<u", "cc": "<u", "hs": ">=u", "cs": ">=u", "hi": ">u", "ls": "<=u"}
NEG = {"eq": "ne", "ne": "eq", "lt": "ge", "ge": "lt", "gt": "le", "le": "gt",
       "lo": "hs", "cc": "cs", "hs": "lo", "cs": "cc", "hi": "ls", "ls": "hi"}
CCNAME = {A.ARM_CC_EQ: "eq", A.ARM_CC_NE: "ne", A.ARM_CC_HS: "hs", A.ARM_CC_LO: "lo",
          A.ARM_CC_HI: "hi", A.ARM_CC_LS: "ls", A.ARM_CC_GE: "ge", A.ARM_CC_LT: "lt",
          A.ARM_CC_GT: "gt", A.ARM_CC_LE: "le", A.ARM_CC_MI: "mi", A.ARM_CC_PL: "pl"}


# ----------------------------------------------------------------------------- values
class V:
    """Symbolic value. kind: param/const/sym/expr/addr. `load` marks memory dependence."""

    def __init__(self, kind, text="", val=None, base=None, off=0, load=False, ptr=False, deps=()):
        self.kind, self.text, self.val, self.base, self.off = kind, text, val, base, off
        self.load, self.ptr = load, ptr
        self.deps = set(deps)

    def c(self, as_ptr=False) -> str:
        if self.kind == "const":
            v = self.val & 0xFFFFFFFF
            if v >= 0x80000000 and v > 0xFFFF0000:
                return str(v - (1 << 32))
            return hex(v) if v > 9 else str(v)
        if self.kind == "addr":
            b = self.base.c(True)
            if self.off == 0:
                return b
            return f"((u8 *){b} + {hex(self.off) if self.off > 0 else '-' + hex(-self.off)})"
        return self.text


def const(v):
    return V("const", val=v & 0xFFFFFFFF)


def addr_of(v: V, off: int) -> V:
    if v.kind == "addr":
        return V("addr", base=v.base, off=v.off + off, load=v.load, deps=v.deps)
    if v.kind == "const":
        return const(v.val + off)
    return V("addr", base=v, off=off, load=v.load, deps=v.deps)


def expr(text, *vs):
    return V("expr", text=text, load=any(v.load for v in vs), deps=set().union(*[v.deps for v in vs]) if vs else ())


def num(v: V) -> str:
    """value as an integer expression"""
    if v.kind == "addr":
        return f"(s32){v.c()}"
    if v.kind == "sym" or v.ptr:
        return f"(s32){v.c()}"
    return v.c()


# ----------------------------------------------------------------------------- lifter
class Block:
    def __init__(self, end, saved, kind="if"):
        self.end, self.saved, self.kind = end, saved, kind
        self.returned = False
        self.then_regs = None      # register state at the end of the then-arm (if/else)


class Lifter:
    def __init__(self, c: Corpus, f, nparams: int, arity: dict, ret: str, bind_all: bool):
        self.c, self.f = c, f
        self.md = mdd(f.thumb)
        self.np, self.arity, self.ret, self.bind_all = nparams, arity, ret, bind_all
        self.regs: dict[str, V] = {}
        self.params = [V("param", text=f"p{i}") for i in range(4)]
        for i in range(nparams):
            self.regs[f"r{i}"] = self.params[i]
        self.stmts: list[str] = []
        self.depth = 1
        self.externs: dict[str, str] = {}
        self.ntemp = 0
        self.stack_args: dict[int, V] = {}
        self.flags = None          # (lhs V, rhs V) the flags compare
        self.pool = pool_words(f)
        self.blocks: list[Block] = []

    # helpers
    def emit(self, s):
        self.stmts.append("    " * self.depth + s)

    def temp(self, v: V, ctype="s32") -> V:
        n = f"t{self.ntemp}"
        self.ntemp += 1
        self.emit(f"{ctype} {n} = {v.c()};")
        t = V("expr", text=n, ptr=ctype.endswith("*"))
        t.temp = True
        return t

    def reg(self, r) -> V:
        n = canon(rname(self.md, r))
        if n not in self.regs:
            raise Bail(f"read undefined {n}")
        return self.regs[n]

    def setr(self, r, v):
        self.regs[canon(rname(self.md, r))] = v

    def operand(self, op) -> V:
        if op.type == A.ARM_OP_IMM:
            v = const(op.imm)
        elif op.type == A.ARM_OP_REG:
            v = self.reg(op.reg)
        else:
            raise Bail("operand")
        st, sv = op.shift.type, op.shift.value
        if st:
            if st in (A.ARM_SFT_LSL, A.ARM_SFT_LSR, A.ARM_SFT_ASR):
                return self.shift(v, {A.ARM_SFT_LSL: "lsl", A.ARM_SFT_LSR: "lsr", A.ARM_SFT_ASR: "asr"}[st], const(sv))
            if st in (A.ARM_SFT_LSL_REG, A.ARM_SFT_LSR_REG, A.ARM_SFT_ASR_REG):
                k = {A.ARM_SFT_LSL_REG: "lsl", A.ARM_SFT_LSR_REG: "lsr", A.ARM_SFT_ASR_REG: "asr"}[st]
                return self.shift(v, k, self.reg(sv))
            raise Bail("shift kind")
        return v

    def shift(self, v, kind, amt: V):
        if kind == "lsl":
            if v.kind == "const" and amt.kind == "const":
                return const(v.val << amt.val)
            return expr(f"({num(v)} << {amt.c()})", v, amt)
        if kind == "lsr":
            return expr(f"((u32){num(v)} >> {amt.c()})", v, amt)
        return expr(f"((s32){num(v)} >> {amt.c()})", v, amt)

    def mem(self, op) -> V:
        m = op.mem
        if m.base == A.ARM_REG_PC:
            raise Bail("pc mem")
        if m.base == A.ARM_REG_SP:
            raise Bail("sp mem")
        b = self.reg(m.base)
        if m.index:
            idx = self.reg(m.index)
            st, sv = op.shift.type, op.shift.value
            if st == A.ARM_SFT_LSL:
                idx = self.shift(idx, "lsl", const(sv))
            elif st:
                raise Bail("index shift")
            if getattr(op, "subtracted", False):
                raise Bail("sub index")
            return expr(f"((u8 *){b.c(True)} + {num(idx)})", b, idx)
        if b.kind == "param":
            b.ptr = True
        return addr_of(b, m.disp)

    def lval(self, ctype, a: V) -> str:
        if a.kind == "addr" and a.off == 0:
            a = a.base
        if a.kind == "const":
            return f"*(volatile {ctype} *){hex(a.val)}" if a.val >= 0x04000000 else f"*({ctype} *){hex(a.val)}"
        return f"*({ctype} *){a.c()}"

    def sym(self, mod, to) -> V:
        r = self.c.resolve(mod, to)
        if r is None:
            raise Bail("unresolved reloc")
        name, off = r
        kind = self.c.sym_kind.get(name)
        if kind == "function":
            if off not in (0, 1):
                raise Bail("mid-function pointer")
            self.externs.setdefault(name, f"void {name}();")
            return V("sym", text=f"(u32){name}")
        self.externs[name] = f"extern u8 {name}[];"
        base = V("sym", text=name, ptr=True)
        return addr_of(base, off) if off else base

    def side_effect(self, live_after: set[str]):
        """Bind live register values that depend on memory before a store/call."""
        for rn, v in list(self.regs.items()):
            if rn in live_after and v.load and not getattr(v, "temp", False):
                ty = "u8 *" if (v.kind == "addr" or v.ptr) else "s32"
                nv = self.temp(v, ty)
                for r2, v2 in list(self.regs.items()):
                    if v2 is v:
                        self.regs[r2] = nv
                if self.flags:
                    self.flags = tuple(nv if x is v else x for x in self.flags)

    def call(self, name, idx, live_after):
        n = self.arity.get(idx, 0)
        args = []
        for k in range(min(n, 4)):
            args.append(self.reg(getattr(A, f"ARM_REG_R{k}")).c())
        for k in range(4, n):
            off = (k - 4) * 4
            if off not in self.stack_args:
                raise Bail("missing stack arg")
            args.append(self.stack_args[off].c())
        self.stack_args = {}
        self.externs[name] = f"s32 {name}();"
        callexpr = f"{name}({', '.join(args)})"
        for rn in ("r0", "r1", "r2", "r3", "ip", "lr"):
            self.regs.pop(rn, None)
        self.side_effect(live_after - {"r0"})
        self.flags = None
        if "r0" in live_after:
            t = f"t{self.ntemp}"
            self.ntemp += 1
            self.emit(f"s32 {t} = {callexpr};")
            v = V("expr", text=t)
            v.temp = True
            self.regs["r0"] = v
        else:
            self.emit(f"{callexpr};")

    def cond_text(self, cc) -> str:
        if self.flags is None:
            raise Bail("flags unknown")
        a, b = self.flags
        if cc in ("mi", "pl"):
            if not (b.kind == "const" and b.val == 0):
                raise Bail("mi/pl")
            return f"({num(a)} {'<' if cc == 'mi' else '>='} 0)"
        op = COND_C.get(cc)
        if op is None:
            raise Bail("cond " + str(cc))
        if op.endswith("u"):
            return f"((u32){num(a)} {op[:-1]} (u32){num(b)})"
        return f"({num(a)} {op} {num(b)})"

    def set_flags_result(self, rd_reg):
        v = self.regs.get(canon(rname(self.md, rd_reg)))
        self.flags = (v, const(0)) if v is not None else None

    def ret_stmt(self):
        if self.ret == "void":
            self.emit("return;")
        else:
            v = self.regs.get("r0")
            if v is None:
                raise Bail("no return value")
            self.emit(f"return {num(v)};")

    def open_block(self, cc, end):
        self.side_effect(self.cur_live)
        cond = self.cond_text(cc)
        self.emit(f"if {cond} {{")
        self.depth += 1
        self.blocks.append(Block(end, dict(self.regs)))

    def merge(self, states, live):
        """Join register states; a live register that differs between paths is unsupported."""
        out = {}
        for rn in set().union(*[s.keys() for s in states]):
            vs = [s.get(rn) for s in states]
            if all(v is vs[0] for v in vs):
                if vs[0] is not None:
                    out[rn] = vs[0]
            elif rn in live:
                raise Bail("merge of live register " + rn)
        return out

    def close_blocks(self, addr, live):
        while self.blocks and self.blocks[-1].end == addr:
            b = self.blocks.pop()
            self.depth -= 1
            self.emit("}")
            states = [b.saved] if b.kind == "if" else []
            if b.kind == "else" and b.then_regs is not None:
                states.append(b.then_regs)
            if not b.returned:
                states.append(self.regs)
            if b.kind == "else" and b.then_regs is None:
                pass
            if not states:
                raise Bail("both arms return")
            self.regs = self.merge(states, live)
            self.flags = None
            self.dead = False

    def run(self):
        f = self.f
        insns = []
        off = 0
        while off < f.size:
            if off in self.pool:
                off += 4
                continue
            i = next(self.md.disasm(f.raw[off:], off, count=1), None)
            if i is None:
                raise Bail("undecodable")
            insns.append(i)
            off += i.size
        index = {i.address: k for k, i in enumerate(insns)}
        live = liveness(self.md, insns, index, f.relocs, {"r0"} if self.ret != "void" else set(),
                        self.arity)
        self.dead = False           # after a return at the current nesting level
        finished = False
        k = 0
        pend = None
        while k < len(insns):
            i = insns[k]
            self.cur_live = live[k]
            if self.blocks and self.blocks[-1].end < i.address:
                raise Bail("block overlap")
            self.close_blocks(i.address, live[k])
            if finished:
                if i.bytes in (b"\0\0", b"\xc0\x46"):
                    k += 1
                    continue
                raise Bail("code after return")
            if self.dead:
                raise Bail("unreachable code")
            la = live[k + 1] if k + 1 < len(live) else set()
            mn = i.mnemonic
            ops = i.operands
            cc = CCNAME.get(i.cc) if i.cc not in (A.ARM_CC_AL, A.ARM_CC_INVALID) else None
            base = mn
            if f.thumb:
                base = THUMB_BASE.get(mn, mn)
            else:
                if cc and mn.endswith(cc):
                    base = mn[:-len(cc)]
                elif cc and mn[-3:-1] == cc:       # e.g. ldrheq -> ldrh + eq (capstone: ldrheq)
                    base = mn[:-2]
                if base.endswith("s") and base[:-1] in ARM_S:
                    base = base[:-1]
            # ---------------- ARM predication: select pairs and predicated runs
            if cc and not f.thumb and self.blocks and getattr(self.blocks[-1], "predicated", None) == cc:
                cc = None                      # executing inside the predicated block
            if cc and not f.thumb and not is_branch(mn) and base not in ("bx", "ldm", "pop"):
                if base in ("mov", "mvn") and k + 1 < len(insns):
                    j = insns[k + 1]
                    jcc = CCNAME.get(j.cc)
                    if (jcc == NEG.get(cc) and j.mnemonic[:3] == mn[:3] and j.operands[0].reg == ops[0].reg
                            and ops[0].reg not in [o.reg for o in j.operands[1:] if o.type == A.ARM_OP_REG]):
                        v1 = self.operand(ops[1])
                        v2 = self.operand(j.operands[1])
                        if base == "mvn":
                            v1 = const(~v1.val) if v1.kind == "const" else expr(f"~{num(v1)}", v1)
                        if j.mnemonic.startswith("mvn"):
                            v2 = const(~v2.val) if v2.kind == "const" else expr(f"~{num(v2)}", v2)
                        cond = self.cond_text(cc)
                        if v1.kind == v2.kind == "const" and (v1.val, v2.val) == (1, 0):
                            nv = expr(cond, *self.flags)
                        elif v1.kind == v2.kind == "const" and (v1.val, v2.val) == (0, 1):
                            nv = expr(self.cond_text(NEG[cc]), *self.flags)
                        else:
                            nv = expr(f"({cond} ? {num(v1)} : {num(v2)})", v1, v2, *self.flags)
                        self.setr(ops[0].reg, nv)
                        k += 2
                        continue
                # predicated run -> if block covering consecutive same-cc instructions
                j = k
                while j < len(insns) and CCNAME.get(insns[j].cc) == cc and insns[j].cc not in (A.ARM_CC_AL,):
                    j += 1
                end = insns[j].address if j < len(insns) else f.size
                self.open_block(cc, end)
                self.blocks[-1].predicated = cc
                cc = None                      # lift k..j-1 unconditionally inside the block
            # ---------------- prologue / epilogue
            if base in ("push", "stmdb") and (base == "push" or ops[0].reg == A.ARM_REG_SP):
                k += 1
                continue
            if base == "sub" and ops[0].type == A.ARM_OP_REG and ops[0].reg == A.ARM_REG_SP:
                k += 1
                continue
            if base == "add" and ops[0].type == A.ARM_OP_REG and ops[0].reg == A.ARM_REG_SP \
                    and ops[-1].type == A.ARM_OP_IMM:
                k += 1
                continue
            is_ret = False
            if base in ("pop", "ldm") and (base == "pop" or ops[0].reg == A.ARM_REG_SP):
                regs = [rname(self.md, o.reg) for o in ops[(0 if base == "pop" else 1):]]
                is_ret = "pc" in regs
                if not is_ret:
                    k += 1
                    continue
            if base == "bx":
                is_ret = True
            if is_ret:
                if cc and not f.thumb:
                    # conditional return (ARM): if (cond) return;
                    self.emit(f"if {self.cond_text(cc)} {{")
                    self.depth += 1
                    self.ret_stmt()
                    self.depth -= 1
                    self.emit("}")
                    k += 1
                    continue
                if self.blocks:
                    self.ret_stmt()
                    self.blocks[-1].returned = True
                    self.dead = True
                    # skip the rest of this epilogue copy up to the block end
                    while k + 1 < len(insns) and insns[k + 1].address < self.blocks[-1].end:
                        k += 1
                    k += 1
                    self.dead = False
                    self.regs = dict(self.blocks[-1].saved) if self.blocks[-1].kind == "if" else dict(self.blocks[-1].saved)
                    continue
                if self.ret == "void":
                    pass
                else:
                    v = self.regs.get("r0")
                    if v is None:
                        raise Bail("no return value")
                    self.emit(f"return {num(v)};")
                finished = True
                k += 1
                continue
            # ---------------- calls
            if is_branch(mn) and i.address in f.relocs:
                if cc:
                    raise Bail("cond call")
                kind, to, tmod = f.relocs[i.address]
                r = self.c.resolve(tmod, to)
                if r is None or r[1] not in (0, 1):
                    raise Bail("call target")
                if r[0] == f.name:
                    raise Bail("recursion")
                if r[0] in ("_s32_div_f", "_u32_div_f"):
                    a0, a1 = self.reg(A.ARM_REG_R0), self.reg(A.ARM_REG_R1)
                    cast = "(u32)" if r[0].startswith("_u") else ""
                    q = expr(f"({cast}{num(a0)} / {cast}{num(a1)})", a0, a1)
                    m = expr(f"({cast}{num(a0)} % {cast}{num(a1)})", a0, a1)
                    for rn in ("r0", "r1", "r2", "r3", "ip", "lr"):
                        self.regs.pop(rn, None)
                    self.regs["r0"], self.regs["r1"] = q, m
                    self.flags = None
                    k += 1
                    continue
                self.call(r[0], k, la | ({"r0"} if mn == "b" else set()))
                if mn == "b":           # ARM tail call
                    if self.ret == "void":
                        pass
                    else:
                        self.emit(f"return {self.regs['r0'].c()};")
                    if self.blocks:
                        raise Bail("tail call in block")
                    finished = True
                k += 1
                continue
            if is_branch(mn):
                tgt = ops[0].imm if ops and ops[0].type == A.ARM_OP_IMM else None
                if tgt is None or tgt <= i.address or tgt not in index and tgt != f.size:
                    raise Bail("local branch")
                if cc:
                    # if (!cond) { fallthrough block } ; target = block end
                    if self.blocks and tgt > self.blocks[-1].end:
                        raise Bail("branch out of block")
                    if cc not in NEG:
                        raise Bail("cond " + cc)
                    self.open_block(NEG[cc], tgt)
                    k += 1
                    continue
                # unconditional forward branch: end of then-arm -> else-arm
                if not self.blocks or self.blocks[-1].kind != "if" \
                        or self.blocks[-1].end != (insns[k + 1].address if k + 1 < len(insns) else f.size):
                    raise Bail("goto")
                b = self.blocks[-1]
                if tgt <= b.end:
                    raise Bail("goto")
                b.kind, b.then_regs = "else", dict(self.regs)
                self.stmts.append("    " * (self.depth - 1) + "} else {")
                b.end = tgt
                self.regs = dict(b.saved)
                b.saved = b.saved
                self.flags = None
                k += 1
                continue
            # ---------------- loads
            if base in LOADT:
                ty = LOADT[base]
                mop = ops[1]
                if mop.mem.base == A.ARM_REG_PC:
                    t = (((i.address + 4) & ~3) if f.thumb else i.address + 8) + mop.mem.disp
                    if base != "ldr" or not (0 <= t < f.size):
                        raise Bail("pc load")
                    if t in f.relocs:
                        kind, to, tmod = f.relocs[t]
                        self.setr(ops[0].reg, self.sym(tmod, to))
                    else:
                        self.setr(ops[0].reg, const(int.from_bytes(f.raw[t:t + 4], "little")))
                    k += 1
                    continue
                if mop.mem.base == A.ARM_REG_SP:
                    raise Bail("stack load")
                if i.writeback:
                    raise Bail("writeback")
                a = self.mem(mop)
                v = V("expr", text=self.lval(ty, a), load=True)
                if self.bind_all:
                    self.setr(ops[0].reg, self.temp(v, ty))
                else:
                    self.setr(ops[0].reg, v)
                k += 1
                continue
            if base in STORET:
                ty = STORET[base]
                if i.writeback:
                    raise Bail("writeback")
                mop = ops[1]
                if mop.mem.base == A.ARM_REG_SP and not mop.mem.index:
                    if ty != "u32":
                        raise Bail("narrow stack store")
                    self.stack_args[mop.mem.disp] = self.reg(ops[0].reg)
                    k += 1
                    continue
                self.side_effect(la)
                a = self.mem(mop)
                val = self.reg(ops[0].reg)
                vt = val.c() if not val.ptr and val.kind not in ("addr",) else "(u32)" + val.c()
                self.emit(f"{self.lval(ty, a)} = {vt};")
                k += 1
                continue
            # ---------------- ALU
            if base in ("cmp", "cmn", "tst"):
                a, b = self.reg(ops[0].reg), self.operand(ops[1])
                if base == "cmp":
                    self.flags = (a, b)
                elif base == "cmn":
                    self.flags = (a, expr(f"-{num(b)}", b))
                else:
                    self.flags = (expr(f"({num(a)} & {num(b)})", a, b), const(0))
                k += 1
                continue
            if base in ("mov", "mvn"):
                v = self.operand(ops[1])
                if base == "mvn":
                    v = const(~v.val) if v.kind == "const" else expr(f"~{num(v)}", v)
                self.setr(ops[0].reg, v)
                if i.update_flags:
                    self.set_flags_result(ops[0].reg)
                k += 1
                continue
            if base == "neg" or (base == "rsb" and ops[-1].type == A.ARM_OP_IMM and ops[-1].imm == 0):
                v = self.reg(ops[1].reg)
                self.setr(ops[0].reg, expr(f"-{num(v)}", v))
                if i.update_flags:
                    self.set_flags_result(ops[0].reg)
                k += 1
                continue
            if base in ("add", "sub", "orr", "and", "eor", "bic", "rsb", "lsl", "lsr", "asr", "mul"):
                if len(ops) == 2 and not f.thumb and base in ("lsl", "lsr", "asr"):
                    self.setr(ops[0].reg, self.operand(ops[1]))   # ARM: mov rd, rm, <shift>
                    if i.update_flags:
                        self.set_flags_result(ops[0].reg)
                    k += 1
                    continue
                if len(ops) == 2:        # thumb two-operand form
                    a, b = self.reg(ops[0].reg), self.operand(ops[1])
                else:
                    if ops[1].type == A.ARM_OP_REG and ops[1].reg == A.ARM_REG_SP:
                        raise Bail("sp address")
                    if ops[1].type == A.ARM_OP_REG and ops[1].reg == A.ARM_REG_PC:
                        raise Bail("pc address")
                    a, b = self.reg(ops[1].reg), self.operand(ops[2])
                if base in ("lsl", "lsr", "asr"):
                    self.setr(ops[0].reg, self.shift(a, base, b))
                elif base in ("add", "sub") and b.kind == "const" and a.kind != "const":
                    d = b.val if b.val < 0x80000000 else b.val - (1 << 32)
                    d = d if base == "add" else -d
                    if d == 0:
                        self.setr(ops[0].reg, a)
                    elif a.kind in ("param", "addr", "sym") or a.ptr:
                        self.setr(ops[0].reg, addr_of(a, d))
                    else:
                        self.setr(ops[0].reg, expr(f"({num(a)} {'+' if d > 0 else '-'} {hex(abs(d))})", a))
                elif a.kind == "const" and b.kind == "const":
                    va, vb = a.val, b.val
                    r = {"add": va + vb, "sub": va - vb, "orr": va | vb, "and": va & vb, "eor": va ^ vb,
                         "bic": va & ~vb, "rsb": vb - va, "mul": va * vb}[base]
                    self.setr(ops[0].reg, const(r))
                else:
                    sym = {"add": "+", "sub": "-", "orr": "|", "and": "&", "eor": "^", "mul": "*"}
                    if base == "bic":
                        t = f"({num(a)} & ~{num(b)})"
                    elif base == "rsb":
                        t = f"({num(b)} - {num(a)})"
                    else:
                        t = f"({num(a)} {sym[base]} {num(b)})"
                    self.setr(ops[0].reg, expr(t, a, b))
                if i.update_flags:
                    if base == "sub" or (f.thumb and base == "sub"):
                        self.flags = (a, b) if False else (self.regs[canon(rname(self.md, ops[0].reg))], const(0))
                    else:
                        self.set_flags_result(ops[0].reg)
                k += 1
                continue
            if base == "mla":
                a, b, cc_ = self.reg(ops[1].reg), self.reg(ops[2].reg), self.reg(ops[3].reg)
                self.setr(ops[0].reg, expr(f"({num(a)} * {num(b)} + {num(cc_)})", a, b, cc_))
                k += 1
                continue
            raise Bail("unsupported " + mn)
        self.close_blocks(f.size, set())
        if self.blocks:
            raise Bail("unclosed block")
        if not finished:
            raise Bail("no return")

    def source(self) -> str:
        self.run()
        f = self.f
        rt = "void" if self.ret == "void" else "s32"
        ps = []
        for i in range(self.np):
            p = self.params[i]
            ps.append(f"u8 *p{i}" if p.ptr else f"s32 p{i}")
        ext = [v for k, v in self.externs.items() if k != f.name]
        out = ["#include <nitro/types.h>", ""]
        if f.thumb:
            out += ["#pragma thumb on", ""]
        out += ext + ([""] if ext else [])
        out.append(f"{rt} {f.name}({', '.join(ps) or 'void'}) {{")
        out += self.stmts
        out.append("}")
        if f.thumb:
            out += ["", "#pragma thumb reset"]
        return "\n".join(out) + "\n"


THUMB_BASE = {"bics": "bic", "orrs": "orr", "movs": "mov", "adds": "add", "subs": "sub",
              "lsls": "lsl", "lsrs": "lsr", "asrs": "asr", "mvns": "mvn", "ands": "and",
              "eors": "eor", "rsbs": "rsb", "muls": "mul", "negs": "neg"}
ARM_S = ("add", "sub", "mov", "and", "orr", "eor", "bic", "rsb", "mvn", "lsl", "lsr", "asr", "mul")


def liveness(md, insns, index=None, relocs=None, ret_live=frozenset({"r0"}), arity=None):
    """live[k] = registers possibly read at/after instruction k before being written
    (forward branches follow both edges)."""
    n = len(insns)
    live = [set() for _ in range(n + 1)]
    index = index or {}
    relocs = relocs or {}
    for k in range(n - 1, -1, -1):
        i = insns[k]
        try:
            rd, wr = i.regs_access()
        except capstone.CsError:
            rd, wr = [], []
        rds = {canon(md.reg_name(r)) for r in rd}
        wrs = {canon(md.reg_name(r)) for r in wr}
        nxt = set(live[k + 1])
        cond = i.cc not in (A.ARM_CC_AL, A.ARM_CC_INVALID)
        if is_branch(i.mnemonic) and i.address in relocs:
            n_ = 4 if arity is None else min(arity.get(k, 4), 4)
            rds |= {f"r{x}" for x in range(n_)}
            wrs |= {"r0", "r1", "r2", "r3", "ip"}
            if i.mnemonic == "b":
                nxt = set(ret_live)
        elif is_branch(i.mnemonic) and i.operands and i.operands[0].type == A.ARM_OP_IMM:
            t = index.get(i.operands[0].imm)
            tl = live[t] if t is not None and t > k else set()
            nxt = (nxt | tl) if cond else set(tl)
        if i.mnemonic.startswith("bx") or (i.mnemonic.startswith(("pop", "ldm")) and "pc" in i.op_str):
            nxt = (nxt | set(ret_live)) if cond else set(ret_live)
            rds -= {"sp", "r0", "r3", "lr"}
        if cond:
            live[k] = nxt | rds          # predicated: the write may not happen
        else:
            live[k] = (nxt - wrs) | rds
        live[k] -= {"sp", "pc", "lr"}
    return live


def canon(n):
    return {"r12": "ip", "sb": "r9", "sl": "r10", "fp": "r11"}.get(n, n)


# ----------------------------------------------------------------------------- variants
def analyse_regs(f):
    """(read-before-write arg regs, per-call (min arity, max arity))."""
    md = mdd(f.thumb)
    pool = pool_words(f)
    defined = set()
    rbw = set()
    calls = {}
    written_since = set()
    off, k = 0, 0
    insns = []
    while off < f.size:
        if off in pool:
            off += 4
            continue
        i = next(md.disasm(f.raw[off:], off, count=1), None)
        if i is None:
            return None
        insns.append(i)
        off += i.size
    for k, i in enumerate(insns):
        try:
            rd, wr = i.regs_access()
        except capstone.CsError:
            return None
        rds = {canon(md.reg_name(r)) for r in rd}
        wrs = {canon(md.reg_name(r)) for r in wr}
        if i.address in f.relocs and is_branch(i.mnemonic):
            hi = max([int(r[1]) + 1 for r in written_since if r in ("r0", "r1", "r2", "r3")] or [0])
            nstack = 0
            calls[k] = [hi, 4, nstack]
            written_since = set()
            defined |= {"r0"}
            continue
        if i.mnemonic in ("push", "stmdb", "pop", "bx") or i.mnemonic.startswith("ldm"):
            continue
        for r in sorted(rds):
            if r in ("r0", "r1", "r2", "r3") and r not in defined:
                rbw.add(r)
        defined |= wrs
        written_since |= wrs
    return insns, rbw, calls


def stack_arg_counts(f, insns, calls):
    """outgoing stack args before each call: count of distinct [sp,#k] word stores."""
    out = {}
    seen = set()
    for k, i in enumerate(insns):
        if k in calls:
            out[k] = len(seen)
            seen = set()
            continue
        if i.mnemonic == "str" and "[sp" in i.op_str:
            m = re.search(r"\[sp(?:, #(0x[0-9a-f]+|\d+))?\]", i.op_str)
            seen.add(int(m.group(1), 0) if m and m.group(1) else 0)
    return out


def variants(c, f, max_variants=24):
    a = analyse_regs(f)
    if a is None:
        return []
    insns, rbw, calls = a
    np_min = max([int(r[1]) + 1 for r in rbw] or [0])
    sargs = stack_arg_counts(f, insns, calls)
    out = []
    call_ids = sorted(calls)
    for np_ in range(np_min, 5):
        opts = []
        for idx_n, k in enumerate(call_ids):
            lo = calls[k][0]
            if sargs.get(k):
                cand = [4 + sargs[k]]
            elif idx_n == 0:
                hi = max(lo, np_)
                cand = [lo] + [x for x in range(hi, -1, -1) if x != lo]
            else:
                cand = [lo] + [x for x in range(max(lo, 1), -1, -1) if x != lo]  # r0: previous result
            opts.append(cand)
        combos = list(itertools.islice(itertools.product(*opts), 64)) if opts else [()]
        if opts and len(combos) >= 64:   # too many: uniform low / high choices only
            combos = [tuple(o[0] for o in opts), tuple(o[-1] for o in opts)]
        for combo in combos:
            ar = dict(zip(call_ids, combo))
            for ret in ("void", "value"):
                for bind in (False, True):
                    out.append((np_, ar, ret, bind))
        if len(out) > max_variants * 8:
            break
    srcs = []
    for np_, ar, ret, bind in out:
        try:
            s = Lifter(c, f, np_, ar, ret, bind).source()
        except Bail as e:
            if str(e) not in VARIANT_BAILS and not str(e).startswith("read undefined"):
                break            # structural: no variant can lift it
            continue
        except Exception:
            continue
        if s not in srcs:
            srcs.append(s)
        if len(srcs) >= max_variants:
            break
    return srcs


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--max", type=lambda x: int(x, 0), default=0x80)
    ap.add_argument("--min", type=lambda x: int(x, 0), default=0)
    ap.add_argument("--module")
    ap.add_argument("--out", default="auto")
    ap.add_argument("--dry-run", action="store_true")
    ap.add_argument("--show")
    ap.add_argument("--variants", type=int, default=24)
    ap.add_argument("-j", type=int, default=2)
    args = ap.parse_args()

    c = Corpus()
    if args.show:
        f = c.by_name[args.show]
        for s in variants(c, f, args.variants):
            print(s)
            print("// ------")
        return 0
    targets = [f for f in c.unmatched()
               if args.min <= f.size <= args.max and (not args.module or f.mod == args.module)
               and not (f.mod == "main" and f.addr < 0x02000800)]
    targets.sort(key=lambda f: (f.size, f.addr))
    out_dir = ROOT / "src" / args.out
    out_dir.mkdir(parents=True, exist_ok=True)
    lifted = [(f, variants(c, f, args.variants)) for f in targets]
    lifted = [(f, v) for f, v in lifted if v]
    print(f"targets {len(targets)}, liftable {len(lifted)}, variants {sum(len(v) for _, v in lifted)}")
    if args.dry_run:
        return 0

    def work(item):
        f, vs = item
        dst = out_dir / f"{f.name}.c"
        if dst.exists():
            return None
        best = None
        for s in vs:
            dst.write_text("/* templates.py: lifted from the ROM instruction stream, verified by match.py. */\n" + s)
            ok, res, _ = run_match(dst)
            if ok and set(res) == {f.name}:
                return f
            d = res.get(f.name)
            if d and (best is None or d[0] < best[0]):
                best = (d[0], s)
        dst.unlink(missing_ok=True)
        return None

    made = 0
    by_mod = {}
    with ThreadPoolExecutor(args.j) as ex:
        for r in ex.map(work, lifted):
            if r:
                made += 1
                by_mod[r.mod] = by_mod.get(r.mod, 0) + 1
                print(f"  OK {r.mod} {r.name} (0x{r.size:x}, {'thumb' if r.thumb else 'arm'})", flush=True)
    print(f"templates: {made} verified functions written to src/{args.out}/ {by_mod}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
