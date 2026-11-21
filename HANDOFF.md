# Handoff (2026-11-21)

## State (verified with `tools/utilities/verify_all.sh`, exit 0)

- ARM9: **7,408 / 7,408 functions, 1,422,472 / 1,422,472 code bytes (100.00 %)**.
- ROM SHA-1 `52e5024efcf7617a820fa23789b37e5168c3c4fc`. The GNU relink check
  reports "all sections match".
- Every function is named. 284 names are subsystem-level `_Unk_` names, logged
  as `tentative` in `config/eur/symbol_evidence.yaml`.
- `check_symbols.py`: 0 errors, 0 warnings.
- The `refactor/function-naming` branch was merged into main and kept.

## Build it

```sh
cp /path/to/your/sonic_rush_eur.nds .   # your own dump, EUR rev 0 only
bash build.sh                           # -> sonic_rush_eur_rebuilt.nds
bash build.sh --verify                  # + GNU relink + objdiff report
```

`build.sh` runs entirely on the local machine. It refuses a ROM whose SHA-1
differs. On first run it bootstraps the tools (`tools/bootstrap_session.sh`).
ROMs (`*.nds`) are gitignored.

## Done this session

- Secure-area BIOS stubs: one GNU-as TU, `src/secure/secure_area.s`
  (docs/research/0012). The filler uses `.incbin` from the extracted
  `arm9.bin`. No ROM bytes are committed.
- dsd patch (`tools/patches/ds-decomp-v0.12.1-asm-entry-points.patch`):
  - conditional first instructions in sized functions;
  - conditional branch placeholders;
  - `$d` for data in `.text`;
  - named `link_time_const(NAME=0xVALUE)`.
- `tools/utilities/lcf_consts.py` writes those named constants into the
  generated LCF.
- 56 asm functions converted to C, including `OS_GetInitArenaHi` and
  `OS_InitThread` (via the linker constants).
- Naming:
  - all `func_*` names replaced;
  - repeated library statics use `<OfficialName>_<file stem>`;
  - audit fixes: `FS_UnloadOverlayImage`, `FS_LoadOverlayImage`,
    `NNS_SndPlayerCountPlayingSeqBySeqArcIdx`, `_fgr`/`_fgeq`;
  - `WMi_StartParentEx` was checked and is correct.
- New tools:
  - `check_symbols.py`;
  - `verify_all.sh`;
  - `swap_fallback.py`;
  - `rename.py`, which now checks for case collisions, repeated statics and
    evidence confidence;
  - `build.sh`.

## Remaining work (updated 2026-11-21)

1. **asm to C (task `asm-to-c`).** 20 files remain in `src/asm_fallback/`;
   `__pformatter` in `src/asm/` stays asm (switch clustering; SM64DS keeps it
   asm too). Best drafts and per-function blockers: `python3
   tools/matching/nearmiss.py list`. Closest: SpecialStageGimmick_Unk_ov001_0230e78c
   (4, first /20 emits asr before lsr), Player_SetAnimation (7),
   Boss4_SubState_SlamStopSpin (8), SpecialStageGimmick_Unk_ov001_0230e94c (10).
   No draft yet: ExtraBoss_TestColliderRotatedBoxes, StageObj_CollideMapVertical.
   Levers that matched this round: `(s32)((u32)a + (u32)b)`, user induction
   variables, `#pragma opt_propagation off` (no `reset` after it), locals
   initialised from a shift, correct struct offsets. To swap in a match: write
   `OK <sym> <path>` to `build/agents/<agent>.txt`, run
   `python3 tools/matching/swap_fallback.py build/agents/<agent>.txt`, then
   `verify_all.sh`; move the file into its system directory (docs/source-organization.md).
2. **Naming (task `naming`).** 284 `_Unk_` names left (blockers in PLAN.md 3);
   zone titles, `UnkStruct_*` and shared headers not started.
3. **Source layout.** Done: by gameplay system and zone (docs/source-organization.md). TU merging not started; `include/game/Stage/stageobj.h` vs `stageobj_alt.h` still to unify.
4. **Data (`r-data`).** Data-TU path proven with `OS_IRQTable`;
   strategy in docs/research/0013-data-ownership.md.
5. Also open: ARM7 (`r-arm7`), SRA library sources (`r-sra-lib`).

## Resume

Read CLAUDE.md, then run `python3 tools/tasks.py brief`. After every batch run
`bash tools/utilities/verify_all.sh`, and commit only when it prints
`verify_all: OK`. Scheduled tasks from the earlier sessions may still exist;
check them with the scheduler's list command and delete any you no longer need.
