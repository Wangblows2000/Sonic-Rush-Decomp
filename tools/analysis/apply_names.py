#!/usr/bin/env python3
"""Apply names from an sdk_ident CSV to symbols.txt with evidence entries.

Acceptance rule (docs/research/0010-sdk-identification.md):
  * the masked byte pattern is unique among Rush functions and the Rush
    address is claimed by exactly one reference name, AND
  * size >= 32 bytes, OR another accepted-candidate from the same reference
    source file lies within 0x200 bytes (library objects link contiguously).
Only default-named (func_XXXXXXXX) symbols are renamed; existing names and
name collisions are reported, never overwritten.

    python tools/analysis/apply_names.py build/eur/reports/sdk_ident_pokediamond_sdk.csv \
        --source "pret/pokediamond NitroSDK C compiled with CW 1.2/sp3" [--dry]
"""

from __future__ import annotations

import argparse
import csv
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))
from project import modules  # noqa: E402

DEFAULT = re.compile(r"^func_(ov\d{3}_)?[0-9a-f]{8}$")


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("csv", type=Path)
    ap.add_argument("--source", required=True)
    ap.add_argument("--version", default="eur")
    ap.add_argument("--dry", action="store_true")
    args = ap.parse_args()

    rows = [r for r in csv.DictReader(args.csv.open()) if r["unique"] == "True"]
    for r in rows:
        r["addr"] = int(r["rush_address"], 16)
        r["size"] = int(r["size"])
    accepted = []
    for r in rows:
        near = any(o is not r and o["ref_file"] == r["ref_file"] and o["module"] == r["module"]
                   and abs(o["addr"] - r["addr"]) <= 0x200 for o in rows)
        if r["size"] >= 32 or near:
            accepted.append(r)

    mods = {m["name"]: m for m in modules(args.version)}
    ev_path = ROOT / "config" / args.version / "symbol_evidence.yaml"
    ev_text = ev_path.read_text()
    renamed, skipped = 0, []
    by_mod: dict[str, list] = {}
    for r in accepted:
        by_mod.setdefault(r["module"], []).append(r)
    new_ev = []
    for mod, lst in by_mod.items():
        p = mods[mod]["symbols_path"]
        lines = p.read_text().split("\n")
        names = {l.split(" ", 1)[0] for l in lines if l}
        for r in lst:
            pat = f"addr:0x{r['addr']:08x}"
            for i, l in enumerate(lines):
                if pat in l and "kind:function" in l:
                    old = l.split(" ", 1)[0]
                    if not DEFAULT.match(old):
                        skipped.append(f"{old} already named (candidate {r['ref_name']})")
                    elif r["ref_name"] in names:
                        skipped.append(f"{r['ref_name']} already used elsewhere (at {pat})")
                    else:
                        lines[i] = r["ref_name"] + l[len(old):]
                        names.add(r["ref_name"])
                        renamed += 1
                        conf = "strong" if r["size"] >= 32 else "tentative"
                        new_ev.append(f'{r["ref_name"]}: {{confidence: {conf}, evidence: "{mod} 0x{r["addr"]:08x} '
                                      f'({r["size"]} B) byte-identical (relocs masked) to {r["ref_file"]} '
                                      f'from {args.source}"}}')
                    break
        if not args.dry:
            p.write_text("\n".join(lines))
    if not args.dry and new_ev:
        ev_path.write_text(ev_text.rstrip("\n") + "\n\n# --- " + args.source + " ---\n" + "\n".join(new_ev) + "\n")
    print(f"{len(rows)} unique, {len(accepted)} accepted, {renamed} renamed, {len(skipped)} skipped")
    for s in skipped:
        print("  skip:", s)
    return 0


if __name__ == "__main__":
    sys.exit(main())
