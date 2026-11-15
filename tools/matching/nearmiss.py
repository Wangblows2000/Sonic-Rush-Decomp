#!/usr/bin/env python3
"""Near-miss store: keep the closest attempt per function so work is never lost.

    python tools/matching/nearmiss.py save <file.c> [--note "..."] [--agent NAME]
    python tools/matching/nearmiss.py get <symbol> [--source-only]
    python tools/matching/nearmiss.py list [--max-diff 16] [--module ov000]

`save` runs match.py on the file and, for every function the file defines, records
the attempt in nearmiss/db.jsonl if it is closer than the stored one (or new). One row
per symbol: diff bytes (masked differing bytes + length difference), differing
instruction lines, lengths, compiler, source text, origin path, note, timestamp.
Exact matches are recorded with diff 0 (status "match") but belong in src/.
`get` prints the best attempt (source included) so the next agent starts from it.

Idea from tangosdev/sm64ds-decomp tools/nearmiss_db.py (MIT License,
Copyright (c) 2026 Tango); simplified to one best row per symbol.
"""
from __future__ import annotations

import argparse
import json
import os
import re
import subprocess
import sys
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
DB = Path(os.environ.get("NEARMISS_DB", ROOT / "nearmiss" / "db.jsonl"))
LINE = re.compile(r"^(OK  |DIFF) (\S+) \((\S+) (0x[0-9a-f]+), (\d+)/(\d+) bytes, (\d+) differ\) \[(\S+)\]")


def load() -> dict[str, dict]:
    rows = {}
    if DB.exists():
        for line in DB.read_text().splitlines():
            if line.strip():
                r = json.loads(line)
                rows[r["symbol"]] = r
    return rows


def store(rows: dict[str, dict]) -> None:
    DB.parent.mkdir(parents=True, exist_ok=True)
    tmp = DB.with_suffix(".tmp")
    tmp.write_text("".join(json.dumps(rows[k], sort_keys=True) + "\n" for k in sorted(rows)))
    tmp.replace(DB)


def parse(out: str) -> list[dict]:
    """Per-function results + count of differing instruction lines from match.py output."""
    res, cur = [], None
    for line in out.splitlines():
        m = LINE.match(line)
        if m:
            got, want, differ = int(m[5]), int(m[6]), int(m[7])
            cur = {"symbol": m[2], "module": m[3], "addr": m[4], "got_len": got, "want_len": want,
                   "diff": differ + abs(got - want), "insn_diff": 0, "cc": m[8],
                   "status": "match" if m[1].startswith("OK") else "diff"}
            res.append(cur)
        elif cur and re.match(r"^\s+-[0-9a-f]{4}: ", line):
            cur["insn_diff"] += 1
    return res


def cmd_save(a) -> int:
    src = Path(a.file)
    r = subprocess.run([sys.executable, str(ROOT / "tools/matching/match.py"), str(src)],
                       cwd=ROOT, capture_output=True, text=True)
    res = parse(r.stdout)
    if not res:
        print((r.stdout + r.stderr).strip()[-2000:])
        print("nothing recorded (compile error or no known functions)")
        return 2
    rows = load()
    text = src.read_text(errors="replace")
    try:
        origin = src.resolve().relative_to(ROOT).as_posix()
    except ValueError:
        origin = str(src)
    for e in res:
        old = rows.get(e["symbol"])
        key = (e["diff"], e["insn_diff"])
        if old and (old["diff"], old.get("insn_diff", 0)) <= key:
            print(f"kept   {e['symbol']}: stored diff {old['diff']} <= {e['diff']}")
            continue
        e.update({"source": text, "origin": origin, "note": a.note or "", "agent": a.agent or "",
                  "ts": time.strftime("%Y-%m-%dT%H:%M:%S")})
        rows[e["symbol"]] = e
        print(f"saved  {e['symbol']}: diff {e['diff']} bytes, {e['insn_diff']} insns "
              f"({e['got_len']}/{e['want_len']}) {'MATCH' if e['status'] == 'match' else ''}".rstrip())
    store(rows)
    return 0


def cmd_get(a) -> int:
    r = load().get(a.symbol)
    if not r:
        print(f"{a.symbol}: no near-miss recorded")
        return 1
    if not a.source_only:
        print(f"// {r['symbol']} {r['module']} {r['addr']}: diff {r['diff']} bytes, "
              f"{r.get('insn_diff', '?')} insns, {r['got_len']}/{r['want_len']} bytes, cc {r['cc']}, "
              f"from {r['origin']} {r['ts']} {r.get('agent', '')} {r.get('note', '')}".rstrip())
    print(r["source"], end="" if r["source"].endswith("\n") else "\n")
    return 0


def cmd_list(a) -> int:
    rows = sorted(load().values(), key=lambda r: (r["diff"], r["symbol"]))
    for r in rows:
        if r["diff"] > a.max_diff or (a.module and r["module"] != a.module):
            continue
        print(f"{r['diff']:5d} {r.get('insn_diff', 0):4d} {r['module']:6s} {r['symbol']:30s} "
              f"{r['got_len']}/{r['want_len']} {r['origin']}")
    return 0


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    sp = ap.add_subparsers(dest="cmd", required=True)
    s = sp.add_parser("save")
    s.add_argument("file")
    s.add_argument("--note")
    s.add_argument("--agent")
    g = sp.add_parser("get")
    g.add_argument("symbol")
    g.add_argument("--source-only", action="store_true")
    li = sp.add_parser("list")
    li.add_argument("--max-diff", type=int, default=1 << 30)
    li.add_argument("--module")
    a = ap.parse_args()
    return {"save": cmd_save, "get": cmd_get, "list": cmd_list}[a.cmd](a)


if __name__ == "__main__":
    sys.exit(main())
