# Plan — what remains

Measured state: [STATUS.md](STATUS.md). Done so far: research and architecture,
toolchain built from source, byte-identical ROM rebuild, dsd analysis (7,408
functions), GNU relink check, compiler identified (CW 1.2/sp2, linker 1.2/sp3),
full matching ROM build, 100 % of ARM9 code bytes matched and every function named.

## 1. ARM9 code bytes — done

- [x] 7,408 / 7,408 functions, 1,422,472 / 1,422,472 bytes (secure-area BIOS
  stubs matched via one GNU-as TU, docs/research/0012-secure-area.md).

## 2. Assembly back to C

- [x] 54 converted this session (`tools/matching/swap_fallback.py` swaps a
  verified C file in for its asm TU).
- [x] 2026-11-16/21: 15 more converted (TransformDemo_Unk_ov001_022dfc54,
  ExtraBossObj_State_BeamUnitActive, EffTrickSparkle_Create, Mine_State_Main,
  OptionsMenu_DrawItemLabel0, SoundTest_SamplingCallback,
  ExtraBoss_Unk_ov001_02304fb4, StageEvent_SpawnInView,
  OptionsMenu_Unk_ov001_022c9a2c, ExtraBoss_IsRingTouchingPlayer,
  AnimatorSprite_Draw, TryAgain_InitSprites, Valve_State_Active,
  SpecialStage_Unk_ov001_023099ec). Levers that worked: unsigned arithmetic
  `(s32)((u32)a + (u32)b)` (changes IR operand order), user induction
  variables, `#pragma opt_propagation off` (no `reset` after the function),
  locals initialised from a shift, correct struct offsets.
- [ ] 20 functions still in `src/asm_fallback/`. __pformatter is unconvertable, ignore.
  Best drafts and per-function blockers are in `nearmiss/db.jsonl`
  (`python3 tools/matching/nearmiss.py list`).
  Exact blockers per function are the `FAIL` lines in `build/agents/c_*.txt`
  of the session that produced them (not in git); typical: callee-saved
  register rotation, one instruction scheduled differently, Thumb stack-slot
  order. Best lever found: port the analogous Sonic Rush Adventure / SDK
  source shape literally.

## 3. Naming

- [x] Every function named; evidence for each in
  `config/eur/symbol_evidence.yaml`; conventions and audit in
  docs/naming-conventions.md.
- [x] 184 of the 468 `<Subsystem>_Unk_<addr>` names upgraded to behaviour
  names (2026-11-21, evidence per row, `tentative`).
- [ ] 284 `_Unk_` names left: the remaining ones depend on struct fields whose
  meaning is not proven (sprite-slot setters, gGameState +4/+8/+0x1C/+0x1D,
  gCamera +0x3C..+0x48, HUD timers); identical or constant-only Runtime/System
  helpers cannot get distinct names.
- [x] Zone numbers tied to titles (docs/research/0014-zones.md: 1 Leaf Storm …
  7 Dead Line, 8/act 3 = Final Zone boss, 9/act 3 = Extra Zone); the source tree
  uses the titles. Symbol names keep `ZoneN` numbering.
- [ ] Rename `Zone8Act3_*` (Final Zone) and `Zone9Act3_*` (Extra Zone) and
  decide what zone 8/9 acts 1–2 (looping copies of zones 1/3/4/5) are.
- [ ] Name struct types (`UnkStruct_<addr>`) and move shared declarations into
  `include/` headers instead of per-file `extern` prototypes.

## 4. Source reorganisation

- [x] TUs moved out of the per-agent directories into
  `src/libs/{NitroSDK/<lib>,NitroSDK/demolib,NitroSystem/<lib>,MSL,Runtime}` and
  game code by gameplay system and zone (`src/game/{Player,Camera,Stage,Objects,
  Enemies,Effects,Boss,UI,Menus,Cutscenes,SpecialStage,...}`,
  `src/game/Zone/<Zone_Name>/`), headers likewise under `include/`. Conventions:
  docs/source-organization.md; zone evidence: docs/research/0014-zones.md.
  `asm/`, `asm_fallback/`, `secure/` unchanged.
- [ ] Merge adjacent one-function TUs back into original translation units
  where the evidence supports it (library objects link contiguously). Not
  started: needs original object boundaries, which also decide data ownership
  (docs/research/0013).
- [x] Unregistered `.c` files: only 3 were tracked (the 80 were build-local
  leftovers). One was a matching `AnimatorSprite_Draw` (now integrated); the
  two non-matching drafts were removed.

## 5. Data and rodata

- [x] Data-TU path proven: `OS_IRQTable` (DTCM 0x027e0004–0x027e005c) is owned
  by `src/libs/NitroSDK/os/OS_IRQTable.c` (docs/research/0013-data-ownership.md).
- [ ] Decompile the rest of `.rodata`, `.data` and `.bss` into TUs: strings, tables and
  float constants. Today every TU only declares `extern` data, and all data
  comes from dsd's delinked gap objects. Needs a strategy for which data
  belongs to which TU (task `r-data`).
- [ ] DTCM: 0x60 bytes non-zero (word 0, `OS_IRQTable` done, word 0x5c), the
  rest is the zero SDK work area (docs/research/0013).

## 6. Beyond ARM9 EUR

- [ ] ARM7: identify the NitroSDK component and version (task `r-arm7`).
- [ ] Download Play children (`/mb/contest.srl`, `/mb/preview.srl`).
- [ ] CI workflow for `ninja verify`.
- [ ] Upstream the dsd patches (jump tables, asm entry points, ELF symtab /
  Thumb bit / ABS32 fixes from docs/research/0006) and the ds-rom zero-length
  file ordering fix (docs/research/0005).
