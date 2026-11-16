#!/usr/bin/env python3
"""Zero-token Ghidra tier: compile Ghidra decompiler drafts as-is (after mechanical fixes).

Reads build/eur/ghidra/<module>/decomp/<symbol|FUN_addr>.c for every unmatched function,
and rewrites it into compilable C:
  * Ghidra types (undefined4, uint, ushort, byte, bool, code, ...) via typedefs;
  * FUN_xxxxxxxx / thunk_FUN_ -> symbol names from symbols.txt (target module, then main),
    declared K&R style (`int f();`);
  * DAT_/PTR_* names of the function's own literal-pool words -> the pool value
    (relocs.txt symbol address or the constant); `_DAT_xxxxxxxx` -> volatile u32 at that
    absolute address;
  * drafts with register artefacts (in_*, unaff_*, extraout_*, CONCAT*, SUB*, ...) skipped.
Each candidate is verified with match.py; only passing files stay in src/auto/.
Ghidra output itself is never committed; what lands in src/auto/ is the rewritten,
byte-verified C.

    python tools/matching/ghidra_tier.py [--max 0x400] [--dry-run] [--show SYMBOL] [-j 2]
"""
from __future__ import annotations

import argparse
import re
import sys
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from accel_common import ROOT, Corpus, pool_words, run_match  # noqa: E402

GH = ROOT / "build" / "eur" / "ghidra"
BAD = re.compile(r"\b(in_\w+|unaff_\w+|extraout_\w+|CONCAT\d+|SUB\d+|ZEXT\d+|SEXT\d+|halt_baddata|"
                 r"SBORROW\d|SCARRY\d|CARRY\d|POPCOUNT|LZCOUNT|coprocessor_\w+|software_interrupt|"
                 r"FUN_\w+\._|\w+\._\d+_\d+_|__return_address|DAT_\w+\._)\b|\._\d")
HEADER = """typedef unsigned char undefined;
typedef unsigned char undefined1;
typedef unsigned short undefined2;
typedef unsigned int undefined3;
typedef unsigned int undefined4;
typedef unsigned long long undefined8;
typedef unsigned char byte;
typedef unsigned char uchar;
typedef unsigned char bool;
typedef unsigned short ushort;
typedef unsigned short word;
typedef unsigned int uint;
typedef unsigned int dword;
typedef unsigned long ulong;
typedef unsigned long long ulonglong;
typedef long long longlong;
typedef int code();
#define true 1
#define false 0
"""
KEYWORDS = {"if", "while", "for", "switch", "return", "sizeof", "do", "else", "case", "goto"}


def draft_path(f):
    d = GH / f.mod / "decomp"
    for name in (f.name, f"FUN_{f.addr:08x}"):
        p = d / f"{name}.c"
        if p.exists():
            return p
    return None


def rewrite(c: Corpus, f, text: str, ptype: str = "int") -> str | None:
    text = re.sub(r"^//[^\n]*\n", "", text, flags=re.M)
    if BAD.search(text):
        return None
    m = re.search(r"\b(\w+)\s*\(", text)          # first identifier+paren = the definition
    if not m:
        return None
    own = m.group(1)
    text = re.sub(rf"\b{re.escape(own)}\b", f.name, text)
    externs: dict[str, str] = {}
    pool = pool_words(f)

    def fun(mm):
        a = int(mm.group(1), 16)
        for mod in (f.mod, "main", "itcm"):
            r = c.resolve(mod, a)
            if r and r[1] == 0 and c.sym_kind.get(r[0]) == "function":
                return r[0]
        raise KeyError(mm.group(0))

    def dat(mm):
        a = int(mm.group(2), 16)
        o = a - f.addr
        if o in pool:
            rel = f.relocs.get(o)
            if rel:
                r = c.resolve(rel[2], rel[1])
                if r is None:
                    raise KeyError(mm.group(0))
                name, off = r
                if c.sym_kind.get(name) == "function":
                    externs.setdefault(name, f"int {name}();")
                    return f"((int){name})"
                externs[name] = f"extern unsigned char {name}[];"
                if ptype == "int":
                    return f"((int){name} + {off:#x})" if off else f"((int){name})"
                return f"(({ptype})({name} + {off:#x}))" if off else f"(({ptype}){name})"
            v = int.from_bytes(f.raw[o:o + 4], "little")
            return f"{v:#x}"
        raise KeyError(mm.group(0))

    try:
        text = re.sub(r"\b(?:thunk_)?FUN_([0-9a-f]{8})\b", fun, text)
        text = re.sub(r"\b_DAT_([0-9a-f]{8})\b", lambda mm: f"(*(volatile unsigned int *)0x{mm.group(1)})", text)
        text = re.sub(r"\b(PTR_\w+?_|DAT_)([0-9a-f]{8})\b", dat, text)
    except KeyError:
        return None
    if re.search(r"\b(DAT|PTR|LAB_\w+\._|s_\w+_[0-9a-f]{8})\b", text) and re.search(r"\bs_\w+_[0-9a-f]{8}\b", text):
        return None
    defined = {f.name}
    for mm in re.finditer(r"\b([A-Za-z_]\w*)\s*\(", text):
        n = mm.group(1)
        if n in KEYWORDS or n in defined or n in externs or n == "code":
            continue
        if n in c.by_name:
            externs[n] = f"int {n}();"
    out = HEADER + "\n" + "\n".join(externs.values()) + "\n\n"
    if f.thumb:
        out += "#pragma thumb on\n\n"
    out += text.strip() + "\n"
    if f.thumb:
        out += "\n#pragma thumb reset\n"
    return out


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--max", type=lambda x: int(x, 0), default=0x400)
    ap.add_argument("--min", type=lambda x: int(x, 0), default=0)
    ap.add_argument("--module")
    ap.add_argument("--out", default="auto")
    ap.add_argument("--dry-run", action="store_true")
    ap.add_argument("--show")
    ap.add_argument("-j", type=int, default=2)
    args = ap.parse_args()
    c = Corpus()
    if args.show:
        f = c.by_name[args.show]
        p = draft_path(f)
        print(rewrite(c, f, p.read_text(errors="replace")) if p else "no draft")
        return 0
    targets = [f for f in c.unmatched() if args.min <= f.size <= args.max
               and (not args.module or f.mod == args.module)
               and not (f.mod == "main" and f.addr < 0x02000800)]
    targets.sort(key=lambda f: (f.size, f.addr))
    cands, nodraft = [], 0
    for f in targets:
        p = draft_path(f)
        if not p:
            nodraft += 1
            continue
        txt = p.read_text(errors="replace")
        vs = []
        for pt in ("int", "int *", "void *"):
            s = rewrite(c, f, txt, pt)
            if s and s not in vs:
                vs.append(s)
        if vs:
            cands.append((f, vs))
    print(f"targets {len(targets)}, no draft {nodraft}, rewritable {len(cands)}")
    if args.dry_run:
        return 0
    out_dir = ROOT / "src" / args.out
    out_dir.mkdir(parents=True, exist_ok=True)

    def work(item):
        f, vs = item
        dst = out_dir / f"{f.name}.c"
        if dst.exists():
            return None
        for s in vs:
            dst.write_text(f"/* ghidra_tier.py: mechanically fixed Ghidra draft, verified by match.py. */\n{s}")
            ok, res, _ = run_match(dst)
            if ok and set(res) == {f.name}:
                return f
            if res:
                break           # compiled but differs: other pointer spellings give the same code
        dst.unlink(missing_ok=True)
        return None

    made, by_mod = 0, {}
    with ThreadPoolExecutor(args.j) as ex:
        for r in ex.map(work, cands):
            if r:
                made += 1
                by_mod[r.mod] = by_mod.get(r.mod, 0) + 1
                print(f"  OK {r.mod} {r.name} (0x{r.size:x}, {'thumb' if r.thumb else 'arm'})", flush=True)
    print(f"ghidra_tier: {made} verified functions written to src/{args.out}/ {by_mod}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
