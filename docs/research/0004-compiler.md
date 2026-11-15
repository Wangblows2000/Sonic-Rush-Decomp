# 0004 — Compiler identification (OPEN)

**Question.** Which Metrowerks CodeWarrior for NINTENDO DS version and flags
built Sonic Rush's ARM9 code, SDK and middleware?
**Status.** Not established. **No function has been compiled yet.**

## Evidence so far

| Evidence | Implication | Confidence |
| -------- | ----------- | ---------- |
| Build info SDK version 0x02017532 (NitroSDK 2.1) | late-2005 SDK; CW 1.2 era | medium |
| Release Nov 2005; CW for DS 2.0 shipped later | game code likely CW 1.2 spN | low–medium |
| Thumb `switch` tables without `lsl/asr #16` sign extension (0008) | differs from code dsd was tuned on (CW 2.0 titles); compiler-specific idiom | high (observed), meaning unknown |
| Thumb epilogue `pop {r3}; bx r3`, `mov r8, r8` padding | Thumb interworking return, `-interworking` | high |
| Large Thumb regions in main, ov000 tail, most of ov001 first 160 KB | mixed ARM/Thumb objects; Thumb likely per-file `#pragma thumb` or `-thumb` | high |
| SRA (2007) game code = 2.0/sp1p5 | upper bound of candidate range | — |

## Candidate set

`1.2/base, 1.2/sp2, 1.2/sp2p3, 1.2/sp3, 1.2/sp4, 2.0/base, 2.0/sp1` plus the
`dsi/1.x` builds in the community mwccarm bundle. Flags start from SRA's set.

## Method (to run once the compiler is available)

1. Pick ~20 small, self-contained leaf functions per region (main ARM, main
   Thumb, ov000, ov001, SDK-looking code near `FS_*`).
2. Hand-write C for each; compile with every candidate version × `-O4,p` /
   `-O4,s` / `-O2,p`, `-thumb` where relevant.
3. Score by exact-match count via objdiff; the version that matches most
   functions, including idiom-sensitive ones (switch tables, epilogues), wins.
4. Record per-region results in `config/eur/compilers.yaml` with the matched
   functions as evidence and set `status: verified`.

## Why not done

Running the proprietary `mwccarm.exe` was blocked in the environment that set
up this project. This is the main external dependency (STATUS.md).
