# 0003 — Toolchain and environment

**Question.** What can run where, and what is pinned?
**Confidence.** High (observed 2026-11-15).

> Superseded in part (2026-11-16): mwccarm and Ghidra both run in the cloud
> workspace now. mwccarm 1.2/sp2 and mwldarm 1.2/sp3 run via wibo (0004);
> Ghidra 12.2 runs with OpenJDK 25 (0007). The table below is the original
> observation.

## Setup environment (cloud workspace, Ubuntu 24.04, x86_64)

| Component | Result |
| --------- | ------ |
| Python 3.13, git 2.43, gcc, clang 18, cmake, ninja, Java (OpenJDK) | present |
| cargo 1.97 | present |
| binutils-arm-none-eabi 2.42 | installed via apt |
| pyyaml, pyelftools, capstone 5.0.7 | installed via pip |
| dsd v0.11.0, v0.12.1 | **built from source** (prebuilt binaries could not be executed by policy) |
| objdiff-cli v3.8.3 | built from source |
| wibo 1.2.0 | built from source; needs `-DLIBCLANG_LIBRARY=…` and `-fuse-ld=lld` (GNU ld 2.42 lacks `--image-base`) |
| mwccarm (community bundle: 1.2/{base,b56,sp2,sp2p3,sp3,sp4}, 2.0/{base,sp1,sp1p2,sp1p5,sp1p6,sp1p7,sp2,sp2p2,sp2p3,sp2p4}, dsi/1.x) | downloadable, but **execution was blocked** in this environment; removed |
| Ghidra | not installed (prebuilt release would hit the same execution policy) |

## Pins

dsd v0.12.1 (+ patches in `tools/patches/`, 0008), objdiff v3.8.3, wibo 1.2.0. ph pins dsd v0.11.0 /
objdiff v3.4.0 / wibo 0.6.16; newer releases were used because they built
cleanly and pass all checks here.

## Consequence (superseded: see the note at the top)

Everything except compiling/linking with Metrowerks runs here. The matching
build (`ninja`) and compiler identification (0004) need a machine where the
user can run mwccarm (their own PC, via wibo or Windows).
