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

## Findings 2026-11-15 (session 2)

### Availability
- CodeWarrior for NINTENDO DS was distributed by Nintendo to licensed DS
  developers (Metrowerks/Nintendo agreement, Aug 2004). NXP, the current
  CodeWarrior owner, publishes only embedded-target editions; no DS edition is
  offered publicly. **No legitimate public source was found.**
- The community bundle (`mwccarm.zip`, used by zeldaret/ph's configure) is an
  unlicensed redistribution. Running it was blocked in the setup environment.
- decomp.me hosts 24 `nds_arm9` compilers server-side. Using its API from a
  script was also blocked (Cloudflare bot check; scripted use of another
  user's scratch). Not pursued further.

### decomp.me compiler ids ↔ CodeWarrior versions
| id | version | | id | version |
| -- | ------- | - | -- | ------- |
| mwcc_20_56 | 2.0 b56 | | mwcc_30_133 | 2.0sp1p6 |
| mwcc_20_72 | 1.2base | | mwcc_30_134 | 2.0sp1p7 |
| mwcc_20_79 | 1.2sp2 | | mwcc_30_136 | 2.0sp2 |
| mwcc_20_82 | 1.2sp2p3 | | mwcc_30_137 | 2.0sp2p2 |
| mwcc_20_84 | 1.2sp3 | | mwcc_30_138 | 2.0sp2p3 |
| mwcc_20_87 | 1.2sp4 | | mwcc_30_139 | 2.0sp2p4 |
| mwcc_30_114 | 2.0base | | mwcc_40_1018..1051 | DSi 1.1 … 1.6sp1 |
| mwcc_30_123 | 2.0sp1 | | | |
| mwcc_30_126 | 2.0sp1p2 | | | |
| mwcc_30_131 | 2.0sp1p5 | | | |

### What other DS projects use (decomp.me presets)
| Game | Year | Compiler |
| ---- | ---- | -------- |
| New Super Mario Bros. (A2DE) | 2006 | **1.2sp3** (mwcc_20_84) |
| Pokémon Diamond/Pearl | 2006 | 2.0sp1 |
| Phantom Hourglass, Sonic Rush Adventure | 2007 | 2.0sp1p5 |
| Pokémon HGSS, PMD Sky, FE Shadow Dragon, Ranger SoA | 2008–09 | 2.0sp2p2 |
| Inazuma Eleven 3 | 2011 | DSi 1.2p1 |

NSMB (May 2006) on 1.2sp3 strengthens the hypothesis that Sonic Rush (Nov
2005) uses a CW 1.2 service pack (1.2base/sp2/sp2p3/sp3).

## Why not done

Running the proprietary `mwccarm.exe` was blocked in the environment that set
up this project. This is the main external dependency (STATUS.md).
