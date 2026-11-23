# Install

Quick path (Linux): copy your own EUR ROM into the repository root and run
`bash build.sh` (sets up the tools on first run, builds, checks the SHA-1) or
`bash build.sh --verify` (also the GNU relink check and the objdiff report).
The sections below are the manual steps.

## 1. Prerequisites

- Linux (tested: Ubuntu 24.04) or Windows. macOS should work with wibo.
- Python 3.11+, Git, Ninja.
- `binutils-arm-none-eabi` (independent relink check, disassembly).
- To build tools from source: Rust (cargo ≥ 1.88, via rustup), CMake, a C++ compiler,
  clang/libclang and lld (for wibo).

```sh
sudo apt install python3-pip git ninja-build cmake clang lld binutils-arm-none-eabi
python -m pip install -r tools/requirements.txt
```

## 2. Open-source tools

```sh
bash tools/setup_tools.sh
```

This builds, at pinned versions, into `tools/bin/`:

| Tool | Version | Purpose |
| ---- | ------- | ------- |
| dsd (ds-decomp) | v0.12.1 + `tools/patches/` | extract, analyse, delink, LCF, ROM build |
| objdiff-cli | v3.8.3 | per-function diffs and progress reports |
| wibo | 1.2.0 | runs the Windows Metrowerks tools on Linux |

The patches in `tools/patches/` make dsd handle this ROM:

- `ds-decomp-v0.12.1-thumb-unsigned-jumptable.patch`: recognises Sonic Rush's
  unsigned Thumb `switch` jump tables. Without it, analysis of overlay 1 misses
  ~1,500 functions (docs/research/0008-dsd-thumb-jumptables.md).
- `ds-decomp-v0.12.1-asm-entry-points.patch`: three fixes needed by
  hand-written assembly TUs. Functions defined in the config may start with a
  conditional instruction (secondary entry points). An ARM-branch relocation
  keeps its condition code when dsd blanks it. Data symbols inside `.text` get
  a `$d` mapping symbol so they are not disassembled as code.

## 3. Metrowerks CodeWarrior for NINTENDO DS (matching builds only)

Matching requires the original compiler (`mwccarm.exe`, `mwldarm.exe`). It is
proprietary and not distributed here. Place it as
`tools/mwccarm/<version>/mwccarm.exe`. Sonic Rush uses compiler **1.2/sp2**
(`tools/mwccarm/1.2/sp2/`) and links with **1.2/sp3**
(`tools/mwccarm/1.2/sp3/`); see config/eur/compilers.yaml and
docs/research/0004-compiler.md. wibo runs both on Linux.

`tools/bootstrap_session.sh` fetches the community bundle that zeldaret/ph
uses and checks its SHA-256. That bundle is an unlicensed redistribution;
use it only if you accept that (docs/research/0004).

## 4. Base ROM

Copy your own dump to `extract/baserom_rush_eur.nds`. The tools verify its
SHA-1 (`52e5024efcf7617a820fa23789b37e5168c3c4fc`) and refuse other revisions.

Optional: `arm7_bios.bin` (your console's ARM7 BIOS) in the repository root.
Without it the secure-area CRC is restored from `config/versions.yaml`.

## 5. Extract, verify, build

```sh
python tools/project.py doctor        # what is present / missing
python tools/project.py extract eur   # extract/eur/ (gitignored)
python tools/configure.py eur         # writes build.ninja
ninja verify                          # delink + GNU relink byte check
ninja                                 # full ROM build + SHA-1 (needs Metrowerks)
ninja report                          # objdiff report -> build/eur/report.json
python tools/project.py progress      # matched code bytes per module
```

`bash tools/bootstrap_session.sh /path/to/rom.nds` runs sections 1–5 in one
step on a fresh Linux machine.

`ninja` without the compiler defaults to `verify`.

## 6. Ghidra (optional)

Install Ghidra 12.2 and a JDK 25 (12.2 rejects JDK 21), then:

```sh
export JAVA_HOME=/usr/lib/jvm/java-25-openjdk-amd64
export GHIDRA_INSTALL_DIR=/path/to/ghidra_12.2_PUBLIC
bash tools/ghidra/import.sh eur       # or: python tools/project.py ghidra eur
```

Per-function decompiler drafts are written to
`build/eur/ghidra/<module>/decomp/`. See docs/research/0007-ghidra.md.
