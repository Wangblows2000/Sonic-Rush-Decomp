#!/usr/bin/env python3
"""Shared corpus for the matching accelerators (paramclone, templates, similar, nearmiss).

Loads every function of every arm9 module (main, itcm, dtcm, ov000, ov001) with its ROM
bytes, mode and relocations, works out which functions already have C (registered in a
delinks.txt = matched/template-eligible; present anywhere under src/ = taken), and
provides an instruction "skeleton" view in which literal-pool words are separated from
code and every immediate is lifted out as a substitutable value.

Ideas adapted from tangosdev/sm64ds-decomp (MIT License, Copyright (c) 2026 Tango):
tools/paramclone.py (skeleton/immediate split), tools/coddog.py (opcode sequences).
"""
from __future__ import annotations

import bisect
import re
import subprocess
import sys
from dataclasses import dataclass, field
from functools import lru_cache
from pathlib import Path

import capstone

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))
sys.path.insert(0, str(ROOT / "tools" / "matching"))
from project import modules, parse_symbols  # noqa: E402
from compiler_sweep import module_bytes  # noqa: E402

VERSION = "eur"
REL = re.compile(r"from:0x([0-9a-f]+) kind:(\S+) to:0x([0-9a-f]+)(?: add:(\S+))? module:(\S+)")
_MD = {}


def md(thumb: bool):
    if thumb not in _MD:
        m = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_THUMB if thumb else capstone.CS_MODE_ARM)
        m.skipdata = True
        _MD[thumb] = m
    return _MD[thumb]


def target_module(spec: str) -> str | None:
    if spec in ("main", "itcm", "dtcm"):
        return spec
    m = re.match(r"overlays?\((\d+)", spec)
    return f"ov{int(m.group(1)):03d}" if m else None


@dataclass
class Func:
    mod: str
    name: str
    addr: int
    size: int
    thumb: bool
    raw: bytes
    relocs: dict = field(default_factory=dict)   # offset -> (kind, to_addr, to_module)
    _an: tuple | None = None

    @property
    def key(self):
        return (self.mod, self.addr)

    def analysis(self):
        if self._an is None:
            self._an = analyze(self)
        return self._an


class Corpus:
    def __init__(self, version: str = VERSION):
        self.version = version
        mods = module_bytes(version)
        self.funcs: list[Func] = []
        self.by_name: dict[str, Func] = {}
        self.symtab: dict[str, tuple[list[int], list[str]]] = {}   # module -> sorted addrs, names
        self.sym_kind: dict[str, str] = {}
        for m in modules(version):
            rel = {}
            rp = m["symbols_path"].parent / "relocs.txt"
            if rp.exists():
                for line in rp.read_text().splitlines():
                    r = REL.match(line)
                    if r:
                        rel[int(r[1], 16)] = (r[2], int(r[3], 16), target_module(r[5]))
            base, blob = mods[m["name"]]
            syms = parse_symbols(m["symbols_path"])
            pairs = sorted({(s["addr"], s["name"]) for s in syms if s["kind"] in ("function", "data", "bss")})
            self.symtab[m["name"]] = ([a for a, _ in pairs], [n for _, n in pairs])
            for s in syms:
                self.sym_kind[s["name"]] = s["kind"]
                if s["kind"] != "function" or not s["size"]:
                    continue
                o = s["addr"] - base
                if o < 0 or o + s["size"] > len(blob):
                    continue
                f = Func(m["name"], s["name"], s["addr"], s["size"], s["mode"] == "thumb",
                         blob[o:o + s["size"]])
                self.funcs.append(f)
                self.by_name[s["name"]] = f
            # attach relocs efficiently
            fl = sorted((f for f in self.funcs if f.mod == m["name"]), key=lambda f: f.addr)
            starts = [f.addr for f in fl]
            for a, v in rel.items():
                i = bisect.bisect_right(starts, a) - 1
                if i >= 0 and a < fl[i].addr + fl[i].size:
                    fl[i].relocs[a - fl[i].addr] = v
        self.registered, self.taken = src_index(set(self.by_name))
        # src/auto/ files were verified by match.py when written: template-eligible too
        self.auto_ok = {}
        for p in sorted((ROOT / "src" / "auto").glob("*.c")):
            for n in defined_functions(p.read_text(errors="replace"), set(self.by_name)):
                if n not in self.registered:
                    self.auto_ok[n] = p.relative_to(ROOT).as_posix()
        self._defs = {}

    def src_path(self, name: str) -> str | None:
        return self.registered.get(name) or self.auto_ok.get(name)

    def defined_in(self, path: str) -> list[str]:
        if path not in self._defs:
            self._defs[path] = defined_functions((ROOT / path).read_text(errors="replace"), set(self.by_name))
        return self._defs[path]

    def resolve(self, mod: str | None, addr: int):
        """(symbol name, offset) of the symbol containing addr in module mod."""
        if mod not in self.symtab:
            return None
        addrs, names = self.symtab[mod]
        i = bisect.bisect_right(addrs, addr) - 1
        if i < 0:
            return None
        # prefer exact symbol (thumb function pointers carry bit 0)
        if addr & 1 and (j := bisect.bisect_left(addrs, addr & ~1)) < len(addrs) and addrs[j] == addr & ~1:
            return names[j], 0
        return names[i], addr - addrs[i]

    def matched(self):
        return [f for f in self.funcs if f.name in self.registered or f.name in self.auto_ok]

    def unmatched(self):
        return [f for f in self.funcs if f.name not in self.taken]


# --------------------------------------------------------------------------- sources
_DEF_HEAD = re.compile(r"\b([A-Za-z_]\w*)\s*\(")


def defined_functions(text: str, known: set[str] | None = None) -> list[str]:
    """Names of functions *defined* (not just declared) in C text: `name(...) {`."""
    text = re.sub(r"/\*.*?\*/", " ", text, flags=re.S)
    text = re.sub(r"//[^\n]*", " ", text)
    out = []
    for m in _DEF_HEAD.finditer(text):
        name = m.group(1)
        if known is not None and name not in known:
            continue
        depth, i = 1, m.end()
        while i < len(text) and depth:
            c = text[i]
            depth += (c == "(") - (c == ")")
            i += 1
        j = i
        while j < len(text) and text[j] in " \t\r\n":
            j += 1
        if j < len(text) and text[j] == "{" and name not in out:
            out.append(name)
    return out


def src_index(known: set[str]):
    """(registered: func -> src path, taken: set of funcs that have any src/ file)."""
    reg_files = set()
    for p in (ROOT / "config").rglob("delinks.txt"):
        reg_files |= set(re.findall(r"^(src/\S+\.c):$", p.read_text(), re.M))
    registered, taken = {}, set()
    for p in (ROOT / "src").rglob("*.c"):
        rel = p.relative_to(ROOT).as_posix()
        names = defined_functions(p.read_text(errors="replace"), known)
        if p.stem in known:
            taken.add(p.stem)
        taken |= set(names)
        if rel in reg_files:
            for n in names:
                registered.setdefault(n, rel)
    return registered, taken


# --------------------------------------------------------------------------- skeleton
IMM = re.compile(r"#(-?(?:0x[0-9a-fA-F]+|[0-9]+))")
PCREL = re.compile(r"\[pc(?:,\s*#(-?(?:0x[0-9a-fA-F]+|[0-9]+)))?\]")


def pool_words(f: Func) -> set[int]:
    """Offsets of 4-byte literal-pool words referenced by pc-relative loads."""
    pool = set()
    for i in md(f.thumb).disasm(f.raw, 0):
        if not i.mnemonic.startswith("ldr"):
            continue
        m = PCREL.search(i.op_str)
        if not m:
            continue
        d = int(m.group(1), 0) if m.group(1) else 0
        t = ((i.address + 4) & ~3) + d if f.thumb else i.address + 8 + d
        if 0 <= t < f.size:
            pool.add(t)
    return pool


def analyze(f: Func):
    """(skeleton, values, reloc_seq, mnemonics).

    skeleton: tuple of tokens; instructions with immediates blanked, pool words as
    'POOL:R' (relocated) / 'POOL:C' (constant). values: blanked immediates + constant
    pool words, in order. reloc_seq: (to_module, to_addr) of every relocation in
    address order. mnemonics: opcode sequence (code only)."""
    pool = pool_words(f)
    skel, vals, mn = [], [], []
    insns = []
    off = 0
    # decode code but skip pool words (a pool word must not shift the decoder)
    dec = md(f.thumb)
    while off < f.size:
        if off in pool:
            insns.append(("POOL", off))
            off += 4
            continue
        got = None
        for i in dec.disasm(f.raw[off:], off, count=1):
            got = i
        if got is None:
            insns.append(("DATA", off))
            off += 2 if f.thumb else 4
            continue
        if any(p in pool for p in range(off + 1, off + got.size)):
            insns.append(("DATA", off))
            off += 2
            continue
        insns.append(got)
        off += got.size
    for it in insns:
        if isinstance(it, tuple):
            kind, o = it
            if kind == "POOL":
                if o in f.relocs:
                    skel.append("POOL:R")
                else:
                    skel.append("POOL:C")
                    vals.append(int.from_bytes(f.raw[o:o + 4], "little"))
            else:
                skel.append("DATA:" + f.raw[o:o + 2].hex())
            continue
        i = it
        mn.append(i.mnemonic)
        if is_branch(i.mnemonic):
            if i.address in f.relocs:
                skel.append(i.mnemonic + " <reloc>")
            else:
                skel.append(i.mnemonic + " " + i.op_str)   # local branch: offsets must agree
            continue
        op = i.op_str.replace(" ", "")
        if PCREL.search(op):
            op = PCREL.sub("[pc]", op)
        op = IMM.sub(lambda m: (vals.append(int(m.group(1), 0)), "#")[1], op)
        skel.append(i.mnemonic + "|" + op)
    rseq = [(v[2], v[1]) for o, v in sorted(f.relocs.items())]
    return tuple(skel), vals, rseq, tuple(mn)


def is_branch(mn: str) -> bool:
    mn = mn.split(".")[0]
    return mn in ("b", "bl", "blx") or (mn[:1] == "b" and mn[1:] in COND) or \
        (mn[:2] == "bl" and mn[2:] in COND and mn[2:] != "")


COND = {"eq", "ne", "cs", "cc", "hs", "lo", "mi", "pl", "vs", "vc", "hi", "ls", "ge", "lt", "gt", "le", "al", ""}


# --------------------------------------------------------------------------- oracle
def run_match(path: Path, timeout: int = 60):
    """Run match.py on one file -> (ok, per-function {name: (diff_bytes, got_len, want_len)}, output)."""
    try:
        r = subprocess.run([sys.executable, str(ROOT / "tools/matching/match.py"), str(path)],
                           cwd=ROOT, capture_output=True, text=True, timeout=timeout)
    except subprocess.TimeoutExpired:
        return False, {}, "timeout"
    res = {}
    for m in re.finditer(r"^(OK  |DIFF) (\S+) \(\S+ 0x[0-9a-f]+, (\d+)/(\d+) bytes, (\d+) differ\)",
                         r.stdout, re.M):
        res[m[2]] = (int(m[5]) + abs(int(m[3]) - int(m[4])) * (m[1] == "DIFF"), int(m[3]), int(m[4]))
    ok = r.returncode == 0 and any(l.startswith("OK") for l in r.stdout.splitlines())
    return ok, res, r.stdout + r.stderr


def rename_symbols(text: str, mapping: dict[str, str]) -> str:
    """Simultaneous whole-word rename (avoids a->b, b->c chains)."""
    keys = [k for k, v in mapping.items() if k != v]
    if not keys:
        return text
    pat = re.compile(r"\b(" + "|".join(re.escape(k) for k in sorted(keys, key=len, reverse=True)) + r")\b")
    return pat.sub(lambda m: mapping[m.group(1)], text)


@lru_cache(maxsize=None)
def corpus() -> Corpus:
    return Corpus()
