#!/usr/bin/env python3
"""Fuzzy opcode similarity: closest MATCHED functions for every unmatched function.

For each unmatched function (no file anywhere under src/), rank the matched functions
(registered in a delinks.txt, or verified in src/auto/) of the same mode by
difflib.SequenceMatcher ratio over the mnemonic sequence (literal pools excluded), and
keep the top K with their src paths. Use it to pick a few-shot example for an agent
and to schedule work (targets with a close matched sibling first).

    python tools/matching/similar.py                 # -> build/eur/reports/similar.jsonl
    python tools/matching/similar.py --for func_ov000_022f7158   # best example source
    python tools/matching/similar.py --for func_x --k 5 --no-source

Adapted from tangosdev/sm64ds-decomp tools/coddog.py (MIT License,
Copyright (c) 2026 Tango): length-window + opcode-set Jaccard prefilter, then the
Ratcliff-Obershelp ratio (autojunk off).
"""
from __future__ import annotations

import argparse
import difflib
import json
import sys
import time
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from accel_common import ROOT, Corpus  # noqa: E402


class Index:
    def __init__(self, c: Corpus):
        self.c = c
        self.alpha: dict[str, str] = {}
        self.pool = {False: [], True: []}
        for f in c.matched():
            self.pool[f.thumb].append(self.rec(f))

    def enc(self, mn) -> str:
        return "".join(self.alpha.setdefault(m, chr(0x100 + len(self.alpha))) for m in mn)

    def rec(self, f):
        s = self.enc(f.analysis()[3])
        return (f, s, frozenset(s))

    def top(self, f, k=3, lenlo=0.5, jmin=0.3):
        _, s, ss = self.rec(f)
        n = len(s)
        if not n:
            return []
        best = []          # (ratio, -size diff, func)
        floor = 0.0
        cands = []
        for m, ms, mset in self.pool[f.thumb]:
            if m is f or m.name == f.name:
                continue
            lm = len(ms)
            if lm < n * lenlo or lm > n / lenlo:
                continue
            ub = 2 * min(n, lm) / (n + lm)      # length upper bound on ratio
            if ub < floor:
                continue
            j = len(ss & mset) / len(ss | mset)
            if j < jmin:
                continue
            cands.append((ub, j, m, ms))
        cands.sort(key=lambda x: (-x[1], -x[0]))
        for ub, j, m, ms in cands:
            if ub <= floor and len(best) >= k:
                continue
            sm = difflib.SequenceMatcher(None, s, ms, autojunk=False)
            if len(best) >= k and sm.real_quick_ratio() <= floor:
                continue
            if len(best) >= k and sm.quick_ratio() <= floor:
                continue
            r = sm.ratio()
            if len(best) < k or r > floor:
                best.append((r, -abs(m.size - f.size), m))
                best.sort(key=lambda x: (-x[0], -x[1]))
                best = best[:k]
                if len(best) >= k:
                    floor = best[-1][0]
        return [(round(r, 4), m) for r, _, m in best]


def row(c, f, sims):
    return {"name": f.name, "module": f.mod, "addr": f"0x{f.addr:08x}", "size": f.size,
            "mode": "thumb" if f.thumb else "arm",
            "similar": [{"name": m.name, "src": c.src_path(m.name), "ratio": r, "size": m.size,
                         "module": m.mod} for r, m in sims]}


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--for", dest="sym", help="print the closest matched examples for one symbol")
    ap.add_argument("--k", type=int, default=3)
    ap.add_argument("--no-source", action="store_true")
    ap.add_argument("--out", type=Path, default=ROOT / "build/eur/reports/similar.jsonl")
    ap.add_argument("--max", type=lambda x: int(x, 0), default=0x2000, help="skip larger targets")
    args = ap.parse_args()

    c = Corpus()
    idx = Index(c)
    if args.sym:
        f = c.by_name.get(args.sym)
        if f is None:
            print(f"{args.sym}: not a function in symbols.txt")
            return 2
        sims = idx.top(f, args.k)
        status = "matched" if c.src_path(f.name) else ("taken" if f.name in c.taken else "unmatched")
        print(f"{f.name} {f.mod} 0x{f.addr:08x} size 0x{f.size:x} "
              f"{'thumb' if f.thumb else 'arm'} [{status}]")
        for r, m in sims:
            print(f"  {r:.3f}  {m.name:28s} 0x{m.size:<5x} {c.src_path(m.name)}")
        if sims and not args.no_source:
            p = c.src_path(sims[0][1].name)
            print(f"\n// ---- best example: {p}")
            print((ROOT / p).read_text(errors="replace"))
        return 0

    t0 = time.time()
    targets = [f for f in c.unmatched() if f.size <= args.max]
    targets.sort(key=lambda f: f.size)
    args.out.parent.mkdir(parents=True, exist_ok=True)
    hist = [0] * 11
    with args.out.open("w") as fh:
        for i, f in enumerate(targets):
            sims = idx.top(f, args.k)
            fh.write(json.dumps(row(c, f, sims)) + "\n")
            if sims:
                hist[int(sims[0][0] * 10)] += 1
            if i and i % 500 == 0:
                print(f"  {i}/{len(targets)} ({time.time() - t0:.0f}s)", flush=True)
    print(f"wrote {len(targets)} rows to {args.out.relative_to(ROOT)} in {time.time() - t0:.0f}s")
    print("best-ratio histogram (0.0-0.1 ... 1.0):", hist)
    return 0


if __name__ == "__main__":
    sys.exit(main())
