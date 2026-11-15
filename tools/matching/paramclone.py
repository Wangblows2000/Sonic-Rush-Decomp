#!/usr/bin/env python3
"""Parameterized clones: reuse matched C for functions with the same instruction skeleton.

clone_dups.py only handles byte-identical twins. This pairs an unmatched function U
with a matched function M whose capstone skeleton (mnemonics + operand structure, with
every immediate and constant literal-pool word blanked) is identical, then rewrites M's
C source:
  * numeric literals: positional M-immediate -> U-immediate map (field offsets, shifts,
    constants); a map that sends one M value to two U values is rejected;
  * pad-aware tier: struct pad sizes / pad expressions that are not literally an
    immediate (`u8 pad[0x68 - 0x30]`) are shifted by the delta of the nearest field;
  * relocation symbols: M's reloc targets (relocs.txt) are renamed positionally to U's,
    so the clone calls/loads the RIGHT symbols (match.py masks relocated bytes, so a
    wrong name would still byte-match -- the rename is what keeps clones honest);
  * M's own name -> U's name, and M's address suffix in local type names -> U's.
Every candidate is verified with match.py; only passing files stay in src/auto/.
The pass compounds: each new match becomes a template, so it iterates to a fixpoint.

Adapted from tangosdev/sm64ds-decomp tools/paramclone.py (MIT License,
Copyright (c) 2026 Tango); reloc renaming follows our clone_dups.py.

    python tools/matching/paramclone.py [--dry-run] [--per-skel 6] [--max 0x400] [-j 2]
"""
from __future__ import annotations

import argparse
import re
import sys
from collections import defaultdict
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from accel_common import ROOT, Corpus, rename_symbols, run_match  # noqa: E402

NUM = re.compile(r"(?<![A-Za-z0-9_.])(-?0x[0-9a-fA-F]+|-?\d+)([uUlL]*)(?![A-Za-z0-9_.])")
CODE_SPLIT = re.compile(r"(/\*.*?\*/|//[^\n]*|^[ \t]*#[^\n]*|\"(?:\\.|[^\"\\])*\")", re.S | re.M)


def value_map(mvals, uvals):
    cmap = {}
    for mv, uv in zip(mvals, uvals):
        if mv == uv:
            continue
        if cmap.get(mv, uv) != uv:
            return None
        cmap[mv] = uv
    # an M value that stays the same somewhere and changes elsewhere is ambiguous
    same = {mv for mv, uv in zip(mvals, uvals) if mv == uv}
    if same & set(cmap):
        return None
    return cmap


def fmt(tok: str, v: int) -> str:
    if tok.lower().lstrip("-").startswith("0x"):
        return ("-0x%x" % -v) if v < 0 else ("0x%x" % v)
    return str(v)


def sub_code(src: str, fn) -> str:
    """Apply fn to code segments only (not comments, preprocessor lines, strings)."""
    parts = CODE_SPLIT.split(src)
    return "".join(p if (i % 2) else fn(p) for i, p in enumerate(parts))


def substitute(src: str, cmap: dict, pad: bool) -> str:
    def rep(m):
        tok, suf = m.group(1), m.group(2)
        v = int(tok, 0)
        alts = {v, v & 0xFFFFFFFF, -((-v) & 0xFFFFFFFF)} if v < 0 else {v, v - (1 << 32) if v >= 1 << 31 else v}
        for a in alts:
            if a in cmap:
                return fmt(tok, cmap[a]) + suf
        if pad and 0 < v < 0x10000:
            # field end / pad size: shift by the delta of the nearest field at or below it
            ks = [k for k in cmap if isinstance(k, int) and 0 <= v - k <= 4 and 0 <= k < 0x10000]
            if ks:
                k = max(ks)
                return fmt(tok, v + cmap[k] - k) + suf
        return tok + suf
    return sub_code(src, lambda s: NUM.sub(rep, s))


def reloc_map(c: Corpus, m, u):
    sm, su = m.analysis()[2], u.analysis()[2]
    if len(sm) != len(su):
        return None
    mp = {m.name: u.name}
    for (mm, ma), (um, ua) in zip(sm, su):
        a, b = c.resolve(mm, ma), c.resolve(um, ua)
        if a is None or b is None or a[1] != b[1]:
            return None
        if mp.get(a[0], b[0]) != b[0]:
            return None
        mp[a[0]] = b[0]
    # a target symbol that two donor symbols map to would collide in one TU
    if len(set(mp.values())) != len(mp):
        return None
    return mp


def addr_suffix(f) -> str:
    return f"{f.addr:08x}"


def candidates(c: Corpus, m, u, src: str):
    rm = reloc_map(c, m, u)
    if rm is None:
        return []
    cmap = value_map(m.analysis()[1], u.analysis()[1])
    if cmap is None:
        return []
    out = []
    for pad in (False, True):
        t = substitute(src, cmap, pad)
        t = rename_symbols(t, rm)
        t = re.sub(rf"(?<=_){addr_suffix(m)}\b", addr_suffix(u), t)
        if t not in out:
            out.append(t)
    return out


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--max", type=lambda x: int(x, 0), default=0x800)
    ap.add_argument("--per-skel", type=int, default=6, help="templates tried per unmatched function")
    ap.add_argument("--out", default="auto")
    ap.add_argument("--dry-run", action="store_true", help="count pairable functions only")
    ap.add_argument("--rounds", type=int, default=4)
    ap.add_argument("-j", type=int, default=2)
    args = ap.parse_args()

    c = Corpus()
    out_dir = ROOT / "src" / args.out
    out_dir.mkdir(parents=True, exist_ok=True)
    total = 0
    tried_pairs = set()
    for rnd in range(args.rounds):
        templates = defaultdict(list)
        for f in c.funcs:
            if f.name in c.registered or f.name in c.auto_ok:
                templates[(f.thumb, f.analysis()[0])].append(f)
        todo = []
        for u in c.funcs:
            if u.name in c.taken or u.size > args.max:
                continue
            ts = templates.get((u.thumb, u.analysis()[0]), [])
            ts = [m for m in ts if m.size == u.size and (m.name, u.name) not in tried_pairs]
            if ts:
                todo.append((u, ts[:args.per_skel]))
        print(f"round {rnd}: skeleton families {len(templates)}, pairable unmatched {len(todo)}")
        if args.dry_run or not todo:
            break

        def work(item):
            u, ts = item
            dst = out_dir / f"{u.name}.c"
            for m in ts:
                tried_pairs.add((m.name, u.name))
                path = c.src_path(m.name)
                src = (ROOT / path).read_text(errors="replace")
                if len(c.defined_in(path)) != 1:
                    continue          # multi-function TU: renaming would drag siblings along
                for cand in candidates(c, m, u, src):
                    hdr = f"/* paramclone of {m.name} ({path}); immediates and relocations remapped. */\n"
                    dst.write_text(hdr + cand)
                    ok, res, _ = run_match(dst)
                    if ok and set(res) == {u.name}:
                        return u, m
            dst.unlink(missing_ok=True)
            return None

        made = 0
        with ThreadPoolExecutor(args.j) as ex:
            for r in ex.map(work, todo):
                if r:
                    u, m = r
                    made += 1
                    c.taken.add(u.name)
                    c.auto_ok[u.name] = f"src/{args.out}/{u.name}.c"
                    print(f"  OK {u.mod} {u.name} (0x{u.size:x}) <- {m.name}", flush=True)
        total += made
        print(f"round {rnd}: verified {made}")
        if not made:
            break
    print(f"paramclone: {total} verified functions written to src/{args.out}/")
    return 0


if __name__ == "__main__":
    sys.exit(main())
