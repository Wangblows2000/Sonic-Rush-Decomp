# Plan — open work

Measured state: [STATUS.md](STATUS.md). The ARM9 code is fully matched
(7,408 / 7,408 functions, 100 % of code bytes) and every function is named.
What follows is the work that is still open, in priority order.

## 1. ARM9 code bytes — done

Nothing left. The secure-area BIOS stubs are one GNU-as TU
(docs/research/0012-secure-area.md).

## 2. Assembly back to C

20 functions are still CodeWarrior `asm` in `src/asm_fallback/`.
`src/asm/__pformatter.c` stays asm (its switch-table clustering is not
reproducible with CW 1.2; SM64DS keeps it asm too).

- Best drafts and per-function blockers: `python3 tools/matching/nearmiss.py list`.
  Closest: `SpecialStageGimmick_Unk_ov001_0230e78c` (4 differences; the first
  `/20` emits `asr` before `lsr`), `Player_SetAnimation` (7),
  `Boss4_SubState_SlamStopSpin` (8), `SpecialStageGimmick_Unk_ov001_0230e94c` (10).
  No draft yet: `ExtraBoss_TestColliderRotatedBoxes`, `StageObj_CollideMapVertical`.
- Typical blockers: callee-saved register rotation, one instruction scheduled
  differently, Thumb stack-slot order.
- Levers that have worked: `(s32)((u32)a + (u32)b)` (changes IR operand
  order), user induction variables, `#pragma opt_propagation off` (no `reset`
  after the function), locals initialised from a shift, correct struct
  offsets, and porting the analogous Sonic Rush Adventure / SDK source shape
  literally. Full catalogue: AGENTS.md §11 and docs/research/0011.
- To swap in a match: write `OK <sym> <path>` to `build/agents/<agent>.txt`,
  run `python3 tools/matching/swap_fallback.py build/agents/<agent>.txt`, then
  `bash tools/utilities/verify_all.sh`, and move the file into its system
  directory (docs/source-organization.md).

## 3. Naming

Every function is named, with evidence in `config/eur/symbol_evidence.yaml`
(conventions: docs/naming-conventions.md).

- 284 subsystem-level `<Subsystem>_Unk_<addr>` names remain. They depend on
  struct fields whose meaning is not proven (sprite-slot setters,
  gGameState +4/+8/+0x1C/+0x1D, gCamera +0x3C..+0x48, HUD timers); identical
  or constant-only Runtime/System helpers cannot get distinct names.
- Rename `Zone8Act3_*` (Final Zone) and `Zone9Act3_*` (Extra Zone), and decide
  what zone 8/9 acts 1–2 are (looping copies of zones 1/3/4/5). Zone numbers
  and titles: docs/research/0014-zones.md.
- Name the struct types (`UnkStruct_<addr>`) and move shared declarations into
  `include/` headers instead of per-file `extern` prototypes. This includes
  unifying `include/game/Stage/stageobj.h` and `stageobj_alt.h`; until then a
  TU includes one or the other.

## 4. Source layout

The tree is organised by library, gameplay system and zone
(docs/source-organization.md). Open: merge adjacent one-function TUs back
into the original translation units where the evidence supports it. This
needs the original object boundaries, which also decide data ownership
(section 5).

## 5. Data and rodata

The data-TU path is proven with `OS_IRQTable` (DTCM,
docs/research/0013-data-ownership.md). Open:

- Decompile the rest of `.rodata`, `.data` and `.bss` into TUs (strings,
  tables, float constants). Today every TU declares only `extern` data and the
  data comes from dsd's delinked gap objects. Needs a strategy for which data
  belongs to which TU (task `r-data`).
- DTCM: 0x60 bytes are non-zero (word 0, `OS_IRQTable`, word 0x5c); the rest
  is the zero SDK work area.

## 6. Beyond ARM9 EUR

- ARM7: identify the NitroSDK component and version (task `r-arm7`).
- Download Play children (`/mb/contest.srl`, `/mb/preview.srl`).
- USA (`ASCE`) and Japan (`ASCJ`) versions.
- CI workflow for `ninja verify`.
- Upstream the dsd patches (jump tables, asm entry points, ELF symtab / Thumb
  bit / ABS32 fixes from docs/research/0006) and the ds-rom zero-length file
  ordering fix (docs/research/0005).
