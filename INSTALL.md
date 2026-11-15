# Install

## 1. Prerequisites

- Linux (tested: Ubuntu 24.04) or Windows. macOS should work with wibo.
- Python 3.11+, Git, Ninja.
- `binutils-arm-none-eabi` (independent relink check, disassembly).
- To build tools from source: Rust (cargo ≥ 1.85), CMake, a C++ compiler,
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

The dsd patch adds recognition of Sonic Rush's Thumb `switch` jump tables. Without
it, analysis of overlay 1 misses ~1,500 functions
(docs/research/0008-dsd-thumb-jumptables.md).

## 3. Metrowerks CodeWarrior for NINTENDO DS (matching builds only)

Matching requires the original compiler (`mwccarm.exe`, `mwldarm.exe`). It is
proprietary and not distributed here. Place it as
`tools/mwccarm/<version>/mwccarm.exe`, e.g. `tools/mwccarm/1.2/sp3/`. The
version Sonic Rush used is **not established yet**
(config/eur/compilers.yaml, docs/research/0004-compiler.md).

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
```

`ninja` without the compiler defaults to `verify`.

## 6. Ghidra (optional)

Install Ghidra 11.x and a JDK 21, then:

```sh
export GHIDRA_INSTALL_DIR=/path/to/ghidra
python tools/project.py ghidra eur
```

See docs/research/0007-ghidra.md.
