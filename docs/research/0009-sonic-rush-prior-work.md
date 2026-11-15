# 0009 — Prior Sonic Rush / Rush Adventure work

**Question.** What public reverse-engineering exists for Sonic Rush, and what
must not be confused with Sonic Rush Adventure?
**Sources.** Web search 2026-11-15; Sonic Retro wiki "Sonic Rush/Technical
information"; The VG Resource wiki; TCRF; GitHub (RushRE).
**Confidence.** Medium (search-based).

## Findings

- **No public matching decompilation of Sonic Rush** was found. Repos named
  like `RushRE/SonicRush*` are not public.
- **RushRE/SonicRushAdventure-Decomp** is a matching decomp of the *sequel*
  (game code `A3Y*`, 2007, NitroSDK 3.x, mwcc 2.0/sp1p5). Same Dimps engine
  lineage (Rush → Rush Adventure → Colors DS). Useful for engine structure and
  naming hypotheses. Its code is not Sonic Rush code.
- Asset-format research (Sonic Retro, VG Resource): `.bac` sprite animation
  (Sonic Rush BAC format thread), `.bb` archives (QuickBMS script), `.narc`
  stage archives, `.nsbmd` models, `.sdat` sound. NotKit/sonic-rush-tools
  (Python) handles some formats. Level map rendering research by Techokami
  and later Cruizer (rings maps, `.dc` decoration files).
- TCRF: unused level `z82`, unused `DUMMY.BAC`, Sonic Advance leftovers.

## Do not confuse

| | Sonic Rush | Sonic Rush Adventure |
| - | - | - |
| Year | 2005 | 2007 |
| Codes | ASCE/ASCP/ASCJ | A3YE/A3YP/A3YJ |
| SDK | NitroSDK 2.1 (0x02017532) | NitroSDK 3.x |
| Overlays | 2 (shared base 0x022C5C80) | many |

## Next

- Study SRA's engine headers to form hypotheses for Rush's task/process
  system, then test them on Rush bytes.
- Evaluate NotKit/sonic-rush-tools for asset extraction (license check first).
