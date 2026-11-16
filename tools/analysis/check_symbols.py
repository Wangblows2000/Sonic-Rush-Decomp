#!/usr/bin/env python3
"""Consistency checks for symbols, sources and evidence (exit 1 on errors).

Errors (break the build or the naming rules):
  * the same symbol name defined twice (within a module or across modules);
  * two symbol names, or two registered TU file names, that differ only in
    letter case (mwldarm runs under wibo, so object files collide);
  * a registered TU whose file is missing;
  * a default name (func_/data_[ovNNN_]ADDR) used in src/ or include/ that no
    longer exists in any symbols.txt (stale reference after a rename);
  * an evidence entry with an invalid confidence value.

Warnings (reported, do not fail):
  * non-default names without an evidence entry;
  * evidence entries whose name no longer exists (orphans).

    python3 tools/analysis/check_symbols.py [--version eur] [--strict]
"""

from __future__ import annotations

import argparse
import collections
import re
import sys
from pathlib import Path

import yaml

ROOT = Path(__file__).resolve().parents[2]
DEFAULT = re.compile(r"^(func|data|bss)_(ov\d{3}_)?[0-9a-f]{8}$")
DEFAULT_USE = re.compile(r"\b(?:func|data)_(?:ov\d{3}_)?[0-9a-f]{8}\b")
CONF = {"confirmed", "strong", "tentative"}


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--version", default="eur")
    ap.add_argument("--strict", action="store_true", help="treat warnings as errors")
    a = ap.parse_args()
    cfg = ROOT / "config" / a.version
    errors, warnings = [], []

    defs = collections.defaultdict(list)          # name -> [module file]
    for p in (cfg / "arm9").rglob("symbols.txt"):
        for line in p.read_text().splitlines():
            if " kind:" in line:
                defs[line.split(" ", 1)[0]].append(p.relative_to(ROOT).as_posix())
    for n, where in defs.items():
        if len(where) > 1 and not n.startswith("."):
            errors.append(f"duplicate symbol {n}: {', '.join(where)}")
    lower = collections.defaultdict(set)
    for n in defs:
        lower[n.lower()].add(n)
    for v in lower.values():
        if len(v) > 1:
            errors.append(f"case-only symbol collision: {' / '.join(sorted(v))}")

    regs = []
    for dl in (cfg / "arm9").rglob("delinks.txt"):
        regs += re.findall(r"^(src/\S+\.(?:c|cpp|s)):$", dl.read_text(), re.M)
    stems = collections.defaultdict(set)
    for r in regs:
        if not (ROOT / r).exists():
            errors.append(f"registered TU missing: {r}")
        stems[Path(r).stem.lower()].add(r)
    for v in stems.values():
        if len(v) > 1:
            errors.append(f"case-only/duplicate TU file name collision: {' / '.join(sorted(v))}")

    names = set(defs)
    srcs = [*ROOT.glob("src/**/*.c"), *ROOT.glob("src/**/*.h"), *ROOT.glob("include/**/*.h")]
    regset = set(regs)
    for f in srcs:
        rel = f.relative_to(ROOT).as_posix()
        if rel.endswith(".c") and rel not in regset:
            continue  # unregistered work in progress
        for m in set(DEFAULT_USE.findall(f.read_text(errors="ignore"))):
            if m not in names:
                errors.append(f"stale reference {m} in {rel}")

    ev_path = cfg / "symbol_evidence.yaml"
    ev = yaml.safe_load(ev_path.read_text()) or {}
    for k, v in ev.items():
        c = (v or {}).get("confidence") if isinstance(v, dict) else None
        if c not in CONF:
            errors.append(f"evidence {k}: invalid confidence {c!r}")
        if k not in names:
            warnings.append(f"orphan evidence entry {k}")
    for n in names:
        if not DEFAULT.match(n) and not n.startswith((".", "$")) and n not in ev:
            warnings.append(f"no evidence for {n}")

    for w in warnings[:50]:
        print("WARN ", w)
    if len(warnings) > 50:
        print(f"WARN  ... {len(warnings) - 50} more")
    for e in errors:
        print("ERROR", e)
    print(f"check_symbols: {len(errors)} errors, {len(warnings)} warnings "
          f"({len(names)} symbols, {len(regs)} TUs, {len(ev)} evidence entries)")
    return 1 if errors or (a.strict and warnings) else 0


if __name__ == "__main__":
    sys.exit(main())
