# Handoff (2026-11-16)

## State (verified with `tools/utilities/verify_all.sh`, exit 0)

- ARM9: **7,408 / 7,408 functions, 1,422,472 / 1,422,472 code bytes (100.00 %)**.
- ROM SHA-1 `52e5024efcf7617a820fa23789b37e5168c3c4fc`. The GNU relink check
  reports "all sections match".
- Every function is named. 468 names are subsystem-level `_Unk_` names, logged
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

## Remaining work

1. **asm to C (task `asm-to-c`).** 34 files remain in `src/asm_fallback/`,
   plus `__pformatter` in `src/asm/`. All of them are register-colouring or
   scheduling near misses. The bytes after each name are the distance from a
   match.
   - Callee-saved permutations:
     - `EffInvincibility_Main` 25
     - `Valve_State_Active` 21
     - `SpecialStage_Unk_ov001_023099ec` 14
     - `TryAgain_InitSprites` 10
     - `StageObj_DrawSpriteAlt` 132
   - Scheduling only:
     - `Boss4_State_WaitForStart` 20
     - `Boss4_SubState_SlamStopSpin` 8
     - `StageEvent_SpawnInView` 51
     - `StageEvent_SpawnInRect` 22
     - `SpecialStageGimmick_Unk_ov001_0230e78c` 4
     - `SpecialStageGimmick_Unk_ov001_0230e94c` 10
   - Scratch registers or temporaries:
     - `Player_SetAnimation` 8
     - `Mine_State_Main` 36
     - `BossEffect_Create` 17
     - `ExtraBoss_Unk_ov001_022fdbc0` 14
     - `ExtraBoss_Unk_ov001_02304fb4` 35
     - `ZoneMap_Unk_ov001_022e612c` 19
     - `ZoneMap_Unk_ov001_022e519c` 8 (spill slots)
     - `Stage_Create` 33
   - Switch clustering. CW 1.2 groups the cases differently from the target;
     this may be impossible in C:
     - `ZoneMap_Unk_ov001_022e1aac` 47
     - `__pformatter` 32
   - Large functions, not reworked:
     - `StageObj_CollideMapVertical` (3,424 B)
     - `ExtraBoss_TestColliderRotatedBoxes`
     - `AnimatorSprite_Draw`
     - `AnimatorSprite_DrawRotScale`
     - `SoundTest_SamplingCallback`
     - `OptionsMenu_DrawItemLabel0`
     - `OptionsMenu_Unk_ov001_022c9a2c`
     - `ZoneMap_Unk_ov001_022e794c`
     - `TransformDemo_Unk_ov001_022dfc54`
   - Others:
     - `EffTrickSparkle_Create` 10
     - `ExtraBossObj_State_BeamUnitActive` 36
     - `ExtraBoss_IsRingTouchingPlayer` 4
     - `Boss7PartD5_InputState_ClashMash` 172
     - `MapObj8F_Action_StartCameraRelease` 13
   - Drafts are in `nearmiss/db.jsonl` (`tools/matching/nearmiss.py`).
     Unfinished drafts for `Stage_Create` and `OptionsMenu_DrawItemLabel0`
     are in `build/agents/wip/`. That folder is not committed.
   - To swap in a file that matches, write `OK <sym> <path>` to
     `build/agents/<agent>.txt`, then run `python3 tools/matching/swap_fallback.py`
     and `verify_all.sh`.
2. **Naming (task `naming`).** Give the 468 `_Unk_` names behaviour names,
   using `tools/analysis/rename.py names.csv`. Evidence is required.
3. Also open:
   - zone-specific titles;
   - struct recovery (`UnkStruct_*`);
   - data/rodata decompilation (`r-data`);
   - ARM7 (`r-arm7`);
   - SRA library sources (`r-sra-lib`).

## Resume

Read CLAUDE.md, then run `python3 tools/tasks.py brief`. After every batch run
`bash tools/utilities/verify_all.sh`, and commit only when it prints
`verify_all: OK`. Scheduled tasks from the earlier sessions may still exist;
check them with the scheduler's list command and delete any you no longer need.
