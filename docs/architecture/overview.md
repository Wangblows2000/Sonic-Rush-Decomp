# Architecture

## Principles

1. **Byte-perfect at every commit.** Unfinished code is linked from dsd's
   delinked objects (original bytes + relocations), never from committed
   assembly listings. Committed `asm` is limited to CodeWarrior `asm`
   functions in TUs: hand-written runtime routines, C-impossible cases
   (`src/asm/`) and C that did not match (`src/asm_fallback/`).
2. **Analysis lives in config, not in source.** `config/<ver>/arm9/**`
   (`symbols.txt`, `relocs.txt`, `delinks.txt`) holds addresses, names and
   translation-unit boundaries. No game bytes are committed.
3. **Evidence over appearance.** Names carry confidence
   (`symbol_evidence.yaml`); compilers carry verification status
   (`compilers.yaml`); progress counts matched bytes only.
4. **Independent checks.** GNU-ld relink (0006) guards analysis without
   proprietary tools.

## Pipeline

```
baserom.nds ─dsd rom extract─▶ extract/eur/  (+fix_path_order)
                                   │
config/eur/arm9 ─dsd delink──▶ build/eur/delinks/*.o ─┬─▶ gnu_relink (verify)
src/** ─mwccarm 1.2/sp2─────▶ build/eur/src/**.o      │
                  dsd lcf ──▶ arm9.lcf, objects.txt ──┴─▶ mwldarm 1.2/sp3 ─▶ arm9.o
                  dsd rom config/build ─▶ rush_eur.nds ─fix_header─▶ SHA-1 check
objdiff.json (dsd objdiff) ─▶ objdiff-cli report ─▶ project.py progress
```

## Directories

| Path | Contents |
| ---- | -------- |
| `config/versions.yaml` | ROM identities, header fix-ups |
| `config/eur/arm9/` | dsd module configs (main, itcm, dtcm, overlays/ov000, ov001) |
| `config/eur/compilers.yaml` | per-path compiler version/flags + status |
| `config/eur/symbol_evidence.yaml` | naming evidence |
| `src/` | one TU per function: `libs/NitroSDK/<lib>`, `libs/NitroSystem/<lib>`, `libs/MSL`, `libs/Runtime`, `game/` by gameplay system and zone (docs/source-organization.md), plus `asm/`, `asm_fallback/`, `secure/` |
| `include/` | `nitro/` types plus struct headers under `NitroSDK/`, `NitroSystem/`, `MSL/`, `game/<System>/` |
| `nearmiss/` | near-miss C drafts (`db.jsonl`, `tools/matching/nearmiss.py`) |
| `state/` | task board (`tasks.json`, `tools/tasks.py`) |
| `tools/` | `project.py`, `configure.py`, `tasks.py`, `ndslib/`, `extraction/`, `matching/`, `analysis/`, `ghidra/`, `patches/`, `utilities/` |
| `docs/research/` | numbered notes + `README.md` research log |
| `docs/architecture/` | this file, memory map |
| `docs/progress/` | generated dashboard + history |

## ARM7

dsd handles ARM9 only. The ARM7 binary (0x02380000, 0x26F24 bytes) is kept
as extracted and rebuilt verbatim. Identifying it (likely a stock NitroSDK
ARM7 component) is open work (PLAN.md §6).

## Download Play children

`/mb/contest.srl` and `/mb/preview.srl` are full DS programs. They are kept as
files and rebuilt verbatim (PLAN.md §6).
