# 0002 — Reference projects

**Question.** Which architecture and tooling should a 2025-era DS matching
decomp use, and what applies to Sonic Rush?
**Sources.** Shallow clones inspected 2026-11-15:
pret/pokediamond `b623abef`, zeldaret/ph `f0799bd`, XorTroll/mkds-re `4124540`,
mariopartyrd/mariopartyDS `881aa69`, RushRE/SonicRushAdventure-Decomp (HEAD).
**Confidence.** High for what each project does; medium for applicability.

## pret/pokediamond (2019–, Make + asm-first)

- Full disassembly committed as `.s` under `arm9/asm`, `arm7/asm`,
  `arm9/overlays/*/asm`; C replaces asm function by function.
- Linker scripts via NitroSDK `makelcf` templates (`arm9.lsf`), built with
  mwldarm through Wine; ROM via `makerom` + `.rsf`.
- Custom tool zoo in `tools/`: knarc, nitrogfx, msgenc, asm_processor, fixrom.
- Two compilers side by side (mwcc versions per library), set per object.
- **Take:** per-library compiler versions; `include-mw/` style SDK headers;
  asset tools when we reach assets.
- **Avoid:** committing disassembly (copyrighted bytes, merge pain).

## zeldaret/ph (2024–, dsd + ninja + objdiff)

- No asm in the repo. `dsd` extracts the ROM, delinks it into ELF objects
  using committed `config/<ver>/arm9/{symbols,relocs,delinks}.txt`, and
  generates the LCF. Unfinished code is linked from delinked objects, so the
  build is byte-perfect from day one.
- `tools/configure.py` writes `build.ninja`; tools (dsd, objdiff, wibo,
  mwccarm) downloaded at pinned versions.
- Single compiler `2.0/sp1p5`, C++ (`-lang=c++`, RTTI/exceptions off).
- objdiff + decomp.dev for progress; decomp.me scratch metadata.
- Requires ARM7 BIOS for a matching ROM (secure-area encryption).
- **Take:** whole pipeline shape (this repo follows it), config layout,
  no-asm policy, objdiff progress.

## mariopartyrd/mariopartyDS

- Early dsd skeleton (dsd v0.7.0, objdiff v2.7.1, mwcc `2.0/sp1p5`), config
  only. Confirms the ph template is reusable with little change.

## XorTroll/mkds-re (MKDS EU)

- Not a matching decomp: IDA-driven RE with exported symbol map
  (`re-export/mkds-eu-symbols.x`), headers, Thumb wrapper shims for
  asm-hacks, a save editor (WASM).
- **Take:** exporting symbols/types to linker-script form for hackers; format
  research documented next to tools.

## RushRE/SonicRushAdventure-Decomp (sister game, same Dimps engine line)

- pret-style Make + asm, mwcc **`2.0/sp1p5`** for game code, `1.2/sp2p3` for
  NitroSDK, `1.2/sp4` for NitroSystem/DWC/WiFi; C99 (`-lang c99`), `-O4,p`,
  `-ipa file`, `-inline on,noauto`, per-file `-O4,s`/`-O2,p` overrides.
- NitroSDK 3.x (2007). Rush (2005) predates CW 2.0, so its flags are a
  starting hypothesis only.
- **Take:** engine-level structure and names as hypotheses to test against
  Rush bytes; compiler-flag search space.

## Other projects checked

- pret/pokeplatinum, pret/pokeheartgold: same pret model; HGSS documents
  NitroSDK versions per game. Not cloned yet.
- No public Sonic Rush code decomp found (search 2026-11-15). Prior Rush
  research is asset-side: BAC sprite format, BB archives, level maps (Sonic
  Retro, NotKit/sonic-rush-tools). See 0009.

## Decision

Use the **dsd pipeline** (ph model) with additions:
- independent GNU-ld relink check (0006) so analysis regressions are caught
  without the proprietary linker;
- explicit symbol confidence (`symbol_evidence.yaml`);
- per-path compiler map with verification status (`compilers.yaml`);
- Python project CLI for analysis/reporting.
