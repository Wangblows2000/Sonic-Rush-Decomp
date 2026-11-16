# Sonic Rush (ASCP) decomp: naming conventions, evidence standard, subsystem map, anchors

Status: adopted 2026-11-16 for the naming refactor (branch `refactor/function-naming`).
Derived from the repo itself (symbols.txt, symbol_evidence.yaml, src/, include/,
Ghidra exports in build/eur/ghidra/, agent reports) and from reading the naming
conventions of other DS decompilations (no code taken from them). Helpers:
`tools/analysis/fnmap.py <module> [filter]` maps each function to the strings, IO
registers, data and callees it references; `tools/analysis/xref_range.py` lists the
functions that reference an address range.

Nothing here changes the repo. Renames are applied by the lead (single writer):
`config/eur/arm9/**/symbols.txt` plus an entry in `config/eur/symbol_evidence.yaml`,
then a checkpoint.

---

## 0. Reference projects and licence caveats

We borrow **naming conventions and, where proven on our own bytes, API names**. We never
copy code, headers, struct definitions or comments from any of these projects into `src/`
or `include/`.

| Project | What we take | Licence state (checked 2026-11-16) |
| - | - | - |
| RushRE/SonicRushAdventure-Decomp (SRA, the 2007 sequel, same Dimps engine lineage) | Subsystem vocabulary (Task, StageTask, GameObject, AnimatorSprite, MapSys, Player), state, action and callback naming patterns | **No LICENSE file** in the repo, so all rights are reserved by default. Names and conventions only. Read its headers to form hypotheses. Never paste code. The existing "identification only" byte compares of its `lib/NitroSystem` C must stay in scratch dirs and never be committed. |
| pret/pokeplatinum | Style rules: PascalCase types and functions, camelCase fields, `Module_Function` prefixes, UPPER_SNAKE enums | No LICENSE file. Conventions only. |
| pret/pokediamond | NitroSDK and libnns function names. These are already used as an identification oracle (docs/research/0010) | No LICENSE file. Names come from Nintendo's public API. Identification only. |
| zeldaret/ph (Phantom Hourglass) | dsd-based workflow; `func_<addr>`, `data_<addr>` and `func_ov<NNN>_<addr>` defaults; `UnkStruct_<addr>` placeholders | CC0 1.0. |
| tangosdev/sm64ds-decomp | Policy: "Import knowledge, write code". Imported names and field offsets are facts, and every `src/` file is written from scratch | MIT. Its CONTRIBUTING.md states the policy we copy. |
| NitroSDK / NitroSystem (NNS) / MSL | Exact public API names (`OS_*`, `FX_*`, `GX_*`, `NNS_G3d*`, `NNS_Snd*`, `NNS_Fnd*`, MSL `__*`) | Nintendo and Metrowerks proprietary. Function names are interface identifiers. Never commit SDK source. |

Do not confuse the games. Sonic Rush uses NitroSDK 2.1 (0x02017532), 2 overlays and CW
1.2/sp2. SRA uses NitroSDK 3.x, many overlays and mwcc 2.0. SRA's names are
**hypotheses** for Rush. A name only transfers when Rush's own bytes support it
(section 2).

---

## 1. Naming scheme

### 1.1 Libraries: SDK, NNS, MSL, BIOS, runtime

- Use the **exact official name**, including case and trailing underscores:
  `OS_GetArenaLo`, `FX_Div`, `GXx_SetMasterBrightness_`, `MTX_RotY33_`,
  `NNS_G3dGlb`, `NNS_FndAllocFromExpHeapEx`, `FX_SinCosTable_`, `__fill_mem`, `_ddiv`.
- Use static helpers' real file-local names when the reference source has them
  (`InitMBlock`, `SetFirstObject`). This is already the practice.
- Never prefix library names with a module tag. Never "improve" them.
- Wireless demo libraries linked into the game (`wh.c`, `wfs`) count as library code.
  Use the NitroSDK demo names (`WH_ChangeSysState`, `WH_StepDataSharing`, ...).

### 1.2 Game code: `Subsystem_Verb[Object]` (single underscore, PascalCase)

Adopt the pokeplatinum and newer-SRA style with **one** underscore between the subsystem
or class prefix and the method. Reasons:

- It sorts and greps by subsystem.
- It cannot be mistaken for SDK names. SDK prefixes are all-caps (`OS_`, `GX_`), and
  game prefixes are PascalCase (`Task_`, `Pad_`).
- It avoids SRA's mixed `Class__Method` and `Class_Method` styles. `__` is legal in C,
  but it is reserved in C++ and is noisy.

| Kind | Pattern | Example (ours) | SRA analogue |
| - | - | - | - |
| Public method | `Class_Verb` | `Task_Create`, `Task_Destroy`, `Pad_Update` | `TaskCreate_`, `DestroyTask` |
| Constructor of a spawnable object | `Class_Create` | `Spring_Create` | `CreateSpring(MapObject*, x, y, type)` |
| Destructor callback (task dtor) | `Class_Destructor` | `Spring_Destructor` | `GameObject__Destructor` |
| Per-frame task main | `Class_Main` | `TitleScreen_Main` | task `main` field |
| State-machine state (called every frame through a state pointer) | `Class_State_<Name>` | `Spring_State_Idle` | `Spring_State_Idle`, `Player__State_Air` |
| One-shot transition into a state | `Class_Action_<Name>` | `Spring_Action_Activate` | `Spring_Action_Activate`, `Player__Action_Boost` |
| Collision, hit or defend callbacks | `Class_On<Event>[_Variant]` | `Spring_OnDefend` | `Spring_OnDefend`, `Player__OnDefend_Regular` |
| Draw or render callback | `Class_Draw` | `GoalRing_Draw` | `SpringCrystal_Draw` |
| Stage-object pipeline callbacks (in/out/move/collide/last/view-check pointers in the object header) | `Class_In`, `Class_Out`, `Class_Move`, `Class_Collide`, `Class_Last`, `Class_ViewCheck` | n/a | `StageTask.ppIn/ppOut/ppMove/ppCollide/ppLast/ppViewCheck` |
| Sprite frame callback | `Class_SpriteCallback` | n/a | `GameObject__SpriteCallback_Default` |
| Shared default callback | `Class_<Callback>_Default` | `StageObj_Draw_Default` | `GameObject__Collide_Default` |
| Sub-object of a class | `ClassPart_...` | `GoalRingSparkle_State_Spin` | `SpringRopeSpring_State_Idle` |

Rules:

- A state name describes what the state **does** (`_Idle`, `_Fall`, `_Wait`). Use a
  number (`_State_1`) only when the role is unproven and the sibling states are proven.
  Otherwise keep the default name.
- Callbacks are named from **the slot they are stored in**, which is proven by a store
  into a struct field whose meaning is proven. They are not named from what they appear
  to do.
- **Main-loop and subsystem singletons**: use `Subsystem_Init`, `Subsystem_Update` and
  `Subsystem_Flush` for functions that main() calls once or every frame.

> **Case rule.** Never give two symbols names that differ only in letter case
> (e.g. `MIC_Init` vs `Mic_Init`). mwldarm runs under wibo with Windows file
> semantics, so their object files collide and the link silently shifts.
> `tools/analysis/rename.py` rejects such names.

### 1.3 Partially known functions: `Subsystem_Unk_<addr>`

When the subsystem is proven but the role is not, keep the address so the name stays
unique and traceable. This follows the RE pattern `MapSys__Func_2008F28` and
`Unknown2051334__Func_2051334`.

- Main: `Task_Unk_02037480`.
- Overlays: `Boss_Unk_ov000_022cce24`. Always add the overlay tag. ov000 and ov001 share
  base 0x022C5C80, so a bare address is ambiguous.
- Fully unknown: keep the dsd default (`func_XXXXXXXX`, `func_ov000_XXXXXXXX`). Do
  **not** rename a function to `Unk_...` without a proven subsystem.

### 1.4 Data and globals

| Kind | Pattern | Example |
| - | - | - |
| SDK, NNS or MSL object | official name | `NNS_G3dGlb`, `FX_SinCosTable_` |
| Game global (mutable state) | `g<Subsystem><Name>` | `gRandSeed`, `gSystemHeap`, `gTaskListHead` |
| Game constant table (rodata) | `g<Subsystem><Name>Table` or `...List` | `gMapObjectCreateTable` |
| String-name tables | `g<Subsystem>NameTable` | `WH` state names use their wh.c name (`sysStateName`) |
| Singleton task pointer | `g<Class>Task` | `gTitleScreenTask` (SRA: `sBossStageTaskSingleton`) |
| Partially known | `g<Subsystem>_Unk_<addr>` | `gTask_Unk_022b4574` |
| Unknown | keep `data_`/`bss_` default | |

Do not use `s` (static) prefixes. Data lives in delinked sections, and file-locality is
only provable once a translation unit's data split is matched.

### 1.5 Types, fields, enums

- Structs: PascalCase with tag and typedef, `typedef struct Task { ... } Task;`.
  Placeholders: `UnkStruct_<addr of the function or table that defines it>`. The repo
  already uses this, and ph does too.
- Fields: camelCase. Unknown fields: `unk<hexoffset>` (`unk1C4`), padding: `pad<hexoffset>`.
  This is already used.
- Enums: PascalCase type and UPPER_SNAKE members with the type prefix:
  `TASK_FLAG_DESTROYED = 1 << 0`.
- Fixed point: keep the SDK types (`fx32`, `fx16`, `VecFx32`).

### 1.6 Inlines (no symbol of their own)

Matched code uses `static inline` helpers that are copy-pasted per file today
(`UnkAlloc_020372d0`, `UnkFree_020371c8`, `Rand()`, the `Obj_SetVec` family). Once the
role is proven:

- Move them to a shared header under `include/game/<subsystem>.h`. This is the lead's
  job, because agents never edit headers outside their own directory.
- Name them like functions: `Heap_AllocGuarded`, `Heap_FreeGuarded`, `Rand_Next`.
- An inline name is never written to symbols.txt.
- For the SDK's own inlines, use the SDK inline name (`MI_CpuClear16`, `FX_Mul`,
  `FX_SinIdx`).

### 1.7 Files

One function per file stays: `src/<agent>/<symbol>.c`, and the file name equals the
symbol. A rename therefore also renames the file and its delinks.txt entry. Only the
lead does this, in one batch per checkpoint, after `match.py` passes.

---

## 2. Evidence standard

A rename needs **at least one concrete, reproducible piece of evidence**, recorded
verbatim in `symbol_evidence.yaml`. Speculative names ("looks like collision") are not
allowed. If the evidence is not good enough, keep the default name.

### 2.1 Accepted evidence kinds (strongest first)

| Code | Evidence | How to reproduce |
| - | - | - |
| E1 | **ROM metadata**: header entry or callbacks, module params, overlay table | dsd config, header.yaml |
| E2 | **Byte-identical to a known library function** (relocations masked), compiled from reference source with our compiler. The function must be at least ~24 B, or its neighbours in the same file must also match in the same order | docs/research/0010 tool |
| E3 | **Unique string reference**: a debug or format string that names the function (`"WH_Finalize, state = %d\n"`), or an asset path used only by that object (`/act/ac_gmk_spring.bac`) | `fnmap.py <mod>` |
| E4 | **Exact SDK/NNS source equivalence** of matched C (e.g. body is `FX_DivAsync(n,d); return FX_GetDivResult();`) or **argument shape plus callee identity** that equals a public API signature | matched src + SDK docs |
| E5 | **Unique hardware register use**, e.g. KEYINPUT 0x04000130 together with the 0x027FFFA8 ext keys, MASTER_BRIGHT 0x0400006C/0x0400106C, POWCNT 0x04000304, the OAM copy pattern, GXFIFO 0x04000400 | xrefs.csv IO targets |
| E6 | **Call-graph role**: called from a function whose role is proven (e.g. main()'s loop) with a provable behaviour in its body | decomp draft plus matched C |
| E7 | **Struct-field semantics proven in matched C** (the function stores to field X of struct T and every consumer of X agrees) | src/, include/ |
| E8 | **Data content** (table bytes decode as the thing named, e.g. sin/cos pairs) | read bytes from `extract/` |

Not evidence on its own: an SRA name for a function "around the same place", the size of
a function, a Ghidra auto-name, a single caller of unknown role, or "it is called a lot".

### 2.2 Confidence levels (same vocabulary as `symbol_evidence.yaml`)

- **confirmed**: E1, E2 at 64 B or more with its file neighbours, or E8 with an exact
  decode. Do not revisit.
- **strong**: E2 on a short function, any E3/E4/E5, or **two independent** kinds from
  E6, E7 and E8.
- **tentative**: a single E6 or E7 indicator, or an E4 argument-shape match that has not
  been byte-compared. A tentative entry is **recorded in symbol_evidence.yaml as a
  candidate, but the symbol is not renamed**. Library functions are the one exception.
  This keeps the current practice for short byte-identical SDK functions. The yaml must
  still list them as tentative.

### 2.3 Workflow

1. A naming agent writes proposals as CSV (`module,old,new,evidence`), one row per
   symbol, confirmed/strong only; the evidence column carries the E-codes and a
   one-line justification. Tentative ideas go in its report, not the CSV. It does
   not touch `config/` or `src/`.
2. The lead re-checks the evidence, then applies the batch with
   `python3 tools/analysis/rename.py <csv>...`: symbols.txt, every use in
   src/include, TU file names, delinks.txt paths and symbol_evidence.yaml change
   together; collisions and unknown symbols are rejected.
3. A rename never changes matched bytes: after each batch the ROM must still build
   with the same SHA-1, the GNU relink must pass and objdiff must stay at the same
   count. If anything changes, revert the batch.
4. Rename the parent's callees and data only when their own evidence holds. Being
   "called by Task_Create" does not name a function by itself.

---

## 3. Subsystem map

Address extents come from config/eur/arm9/delinks.txt and symbols.txt: main `.text`
0x02000000–0x0206e470, `.rodata` 0x0206e470–0x02077958, `.data` 0x02077960–0x0207cfe0,
`.bss` 0x0207cfe0–0x022c5c80. ov000 code runs 0x022c5c80–0x02362b40+ and ov001 code runs
0x022c5c80–0x02315de8+. "Boundary ?" marks an edge that is not yet proven by a named
function on both sides.

### 3.1 main (arm9)

| Range | Subsystem | Evidence |
| - | - | - |
| 0x02000000–0x02000800 | BIOS SWI Thumb stubs (secure area) | dsd secure-area analysis (yaml) |
| 0x02000800–0x02000b24 | crt0: `Entry`, `AutoloadCallback`, `init_cp15` | ROM header (E1), pokediamond crt0 (E2) |
| 0x02000b24–0x02000cb4 | `main` (game main loop, see 3.4) | dsd entry analysis, Ghidra draft |
| 0x02000cb4–~0x0201b028 | **NitroSDK 2.1**: FX/MTX/VEC 0x02000e7c–0x02002800, GX/G3 0x02002a68–0x02004b78, OS 0x020051f4–0x020079xx, MI 0x020082e8–0x02008640, SND 0x02008dd4–0x02009dc0, PXI 0x0200a18c, FS 0x0200c1ec–0x0200c894, PM ~0x0200df6c–0x0200e3e4 (POWCNT/sleep), RTC 0x0200f0a0–0x0200f22c, CARD 0x02010238, then ~0x02010300–0x0201aae8 not yet named (probably WM/MB: 0x02018c64 references the string `"ltiboot"`, and agent headers include/main_sdk2/{wm,wmsys,mbbeacon,...}.h; *verify*), CTRDG 0x0201aae8, MATH 0x0201afe4. Ranges for PM and RTC are inferred from IO registers and neighbours | ~110 E2 byte matches (yaml), IO registers (E5). Note: the `WM_ERRCODE_*`/`WM_STATECODE_*` strings belong to wh.c name tables at 0x020785c4+, not to SDK WM code |
| ~0x0201b028–~0x02020600 | **NitroSystem sound** (NNS_Snd: handles, players, SeqArc, capture ~0x0201cacc, fader 0x02020534) | Byte match of NNSi_SndFader* against SRA's NitroSystem. Agent note: "snd capture" at 0x0201cacc. Game wrappers call 0x0201e9cc with (h,-1,-1,-1,a,b), which is the StartSeqArcEx shape. Boundary ? at the SDK/NNS edge |
| 0x020205f0–~0x02021200 | **NNS_Fnd** (list 0x020205f0, exp-heap ~0x02020960–0x02020fb0) | E2 (yaml). Heap_Init calls 0x02020a14(start,0x20000,0), the shape of NNS_FndCreateExpHeapEx |
| ~0x02021200–~0x02029xxx | **NNS_G3d** (+ G2d/Gfd?): RenderObj/AnmObj ~0x02021c4c–0x02021f60, G3dGlb users 0x0202205c–0x020245d8, WorldPosToScrPos 0x020254d4, AllocAnmObj 0x02025b44, anim interpolation 0x02027104/0x02027c0c | xrefs to `NNS_G3dGlb` (data_02084d38) and GXFIFO 0x04000400 (q.py). Agent notes. Boundary ? at the upper end |
| ~0x02029xxx–0x0202d5d8 | **MSL C** (`__fill_mem` 0x0202a244, printf/scanf 0x0202a3e8–0x0202cac4, hex-float `"0x0p0"` 0x0202b3f0, decimal conversion with the powers-of-5 strings 0x0202cd1c, strftime `"%I:%M:%S %p"`) | strings (E3), agent notes. Boundary ? at the lower end |
| 0x0202d5d8–0x0202fcc8 | **CW ARM runtime**: soft-float `_d*`/`_f*`, `_ll_*`, `_s32_div_f`, `_u32_div_f` | compiler-call relocs from matched C (yaml, confirmed) |
| 0x0202fcc8–~0x0203dc28 | **Game engine core** (Dimps, cf. SRA `game/system`, `game/graphics`): system init and VBlank (0x0202fd94–0x02030598), sprite/BAC animators (~0x02031xxx–0x02033fbc), 3D model animator 0x02031a68, heap 0x0203485c–0x02034c68, pad 0x02035134–0x020352a8, sound wrappers 0x02036754, OAM 0x02036a0c, task system 0x02036e9c–0x02037a8c (touch-panel and frame task runner 0x02037c60) | section 4 anchors |
| 0x0203dc28–0x020405e0+ | **NitroSDK wireless demo libs** (`wh.c`, WFS) | "WH_* failed", "WFS_Init* call" strings (E3) |
| ~0x02040600–0x0206e470 | **Game shared code** (used by both overlays): DS Download Play descriptors 0x020511ec–0x020512a4, boss-stage model loaders 0x0204a238–0x0204a98c, player resource load 0x0204e934, game over 0x0204da44, pause 0x02055918, demo/talk scenes 0x0205788c–0x0206151c, menus 0x02063228, stage-object helpers 0x02068e40–0x0206e3c0 (sprite/collision slots in the 0x20C object header) | strings (E3). Call counts from overlays: 0x02069818 has 179 ov000 callers |
| 0x0206e470 | `.rodata` start = **FX_SinCosTable_** (4096 × {sin,cos} s16) | bytes `0,0x1000,6,0x1000,0xd,0x1000,...` (E8) |
| 0x02084d38 | **NNS_G3dGlb** | NNS G3D field use (agent notes, E7) |
| 0x0208fe70 / 0x0208fe78 / 0x0208fed4 / 0x0208ff00 | Rand seed / system exp-heap handle / NNS allocator for G3D / 128 KB system-heap arena | section 4 |

### 3.2 ov000: in-game stage overlay

String clusters (fnmap.py, functions bucketed by address):

| Range | Content | Evidence (sample strings) |
| - | - | - |
| 0x022c5c80–~0x022fb000 | **Bosses** (3D models, node names, per-boss effects) | `hammer`, `neck_ham`, `body_core`, `bs4:eff_clash.nsbmd`, `/mod/bsf_sky_b.nsbmd` |
| 0x022ee950 | 3D Sonic/Blaze model object (boss stages) | `/act/son.bac`, `son.nsbmd`, `blz.nsbmd`, `son.nsbca`. It is in the ctor table |
| 0x022fb000–0x02302000 | **Enemies** | `/act/ac_ene_egg_bishop.bac`, `ac_ene_flapper_cannon`, `ac_ene_bomb_hawk`, `ac_ene_kuragen_bomb` |
| 0x02302000–0x0231e000 | **Gimmicks** (stage objects) | `ac_gmk_bubble`, `cannon`, `crane`, `dash_c`, `goalring`, `rail`, `spring`, `switch`, `wheel2d`; `/dat/gmk/*.df` |
| 0x0231e000–0x02326000 | **Decorations** | `ac_dec_animal%02d`, `ac_dec_bird`, `leaf`, `pipe`, `coral` |
| 0x02327000–0x02335000 | **Effects** | `ac_eff_water`, `ac_eff_trick`, `ac_eff_boost`, `eff_barrier.nsbmd` |
| 0x0232a000–0x02330000, 0x02338624 | **HUD / fixed sprites** (`ac_fix_*`) | `ac_fix_moji`, `ac_fix_vs`, `ac_fix_information`, `ac_fix_player_icon`, `ac_fix_bosshelp` |
| 0x0234d62c–0x0234e0f8 | **Player** (create + resources) | `/act/son.bac`, `son.nsbmd`. Called 4× from main 0x0204c28c–0x0204c37c |
| 0x02357000–0x02362b40 | **In-stage/VS menus** (player select, results, time select) | `narc/menu.narc`, `mnu_psel_*`, `mnu_conf_result_*`, `mnu_time_select.bac` |
| 0x0236498c (table at 0x02364a6c–0x02364d20) | **Map-object ctor table**: 173 code pointers, 99 unique, all into ov000 | e.g. index 11+ = 0x02316ba0 (spring), 0x022ee950 at 0x02364ce0 |

### 3.3 ov001: front end / extra / special stage

| Range | Content | Evidence |
| - | - | - |
| 0x022c5c80–0x022c8000 | Ending / staff roll | `snd/extra_end/sound_data.sdat`, `bg/final_ed_*`, `poly_demo_under2` |
| ~0x022c9xxx | Sound test | `snd/%s/sound_data.sdat`, `/st/sound_test.sld`, `mnu_opti_sou` |
| 0x022ce000–0x022cf000 | Options | `/act/mnu_opti.bac` |
| 0x022cf90c | SEGA / Sonic Team logo | `bg/sega_logo_sega.bbg`, `bg/sega_logo_sonicteam.bbg` |
| 0x022d4xxx | Time-attack "try again" | `act/mnu_time_tryagain.bac` |
| 0x022d9458 | Title screen | `/act/mnu_titl.bac`, `bg/title_logo_base.bbg` |
| 0x022e1000–0x022ea000 | Zone map / zone select | `bg2/zmap/*`, `ama/zone_sel.ama` |
| 0x022ed000 | Records | `act/mnu_reco*.bac` |
| 0x022f9000–0x02300000 | Extra mode | `/narc/extra_act_lz7.narc`, `snd/ze/` |
| 0x02307000–0x02315de8 | **Special stage** (3D half-pipe) | `narc/ss_act_lz7.narc`, `ss/ss_4.sst`, `mod/ss_*.nsbmd`, `narc/ss_player_lz7.narc` |

### 3.4 main() loop (Ghidra draft of 0x02000b24; drives several anchors)

```
func_02030598();  func_0204edc0();                       // init
loop: if (func_020105d0()) { func_0202ff74(); func_02010590(); }   // card pulled out?
      if (!(flags&0x40)) func_02035134(&pad, ((KEYINPUT|EXTKEY)^mask)&mask);
      func_02037c60(); func_02034d34();
      if (!(flags&0x10) && (pad.held & 0x30c)==0x30c) func_02030538(param); // L+R+START+SELECT
      func_0202fd94(); func_0202ff80(); func_020302d8(); func_02036fb8(); func_02034114();
      func_02036a0c(0); func_02036a0c(1);
      VBlankIntrWait(); if (flags&0x100) func_02034ec0(); func_0203005c();
```

---

## 4. Anchor functions and data (seed list, in priority order)

Priority = (callers × provability). The strongest and most-called come first. "Proposed"
names follow section 1. Each needs its yaml entry when applied. Rows marked *verify*
need the one check named before the rename. All are written from this repo's bytes and
drafts.

| # | Symbol | Proposed name | Conf. | Evidence |
| - | - | - | - | - |
| 1 | `func_02006abc` (675 callers across modules) | `OS_GetArenaLo` | strong | Body `return ((u32*)0x027FFDA0)[id]` (matched src/main). `OS_SetArenaLo` (0x02006880, E2) writes the same array `0x027FFDA0[id]`, and `OS_SetArenaHi` writes `+0x24` (= lo[9], hi[9] of OSArenaInfo). It sits inside the OS_arena.c cluster. *verify*: E2 compare against pokediamond OS_arena.c |
| 2 | `func_020372d0` (519 callers) | `Task_Create` | strong | Matched C: takes a node from the pool, sets main/dtor/flags/prio/id, allocates guarded work (size+0x43 &~0x1f, 0x02467531 trailer), and does priority-ordered insertion into a doubly linked list with sentinels at ids 0xefff/0xffff. Node layout prev/next/main/dtor/work@0x10 equals the SRA `Task_` prefix (E7+E6) |
| 3 | `func_020371c8` (292 callers) | `Task_Destroy` | strong | Matched C: calls `t->dtor(t)`, frees `t->work`, sets `flags |= 1`, and refuses when the task is protected (`prio&0x20` and not the current task) |
| 4 | `func_02036e9c` | `Task_AllocNode` | strong | Draft: pops from a 0x100-entry pointer pool and returns the `OS_GetArenaLo(0)` sentinel on exhaustion. Only callers are Task_Create and its sibling |
| 5 | `data_022b4574`, `data_022b457c..88` | `gTaskCurrent`, `gTaskList*Sentinel` | strong | Used in matched Task_Create/Task_Destroy as the current task and the 4 list sentinels (E7) |
| 6 | `func_02034a40` (138 callers) | `Heap_Alloc` | strong | Matched: `func_020209d0(heap,size,align)`. On failure it falls back to `OS_GetArenaLo(0)`. Used by every guarded-alloc inline |
| 7 | `func_020349b8` (237 callers) | `Heap_Free` | strong | Matched: frees only if p is in the 0x0208ff00+128 KB, 0x020aff00–0x0224ff00 or DTCM 0x027e0060–0x027e2060 arenas, then `func_02020990(heap,p)` |
| 8 | `func_02034c68` | `Heap_Init` | strong | Draft: `func_02020a14(0x0208ff00, 0x20000, 0)` gives a handle stored to 0x0208fe78, then sets up the allocator struct at 0x0208fed4 |
| 9 | `data_0208fe78`, `data_0208fed4` | `gSystemHeap`, `gSystemHeapAllocator` | strong | Literal pool of Heap_Init (0x02034cfc → 0x0208fe78, 0x02034d00 → 0x0208fed4). 0x0208fe78 is passed to Heap_Alloc/Free in matched Task code. 0x0208fed4 is passed as the alloc argument to 0x02025b44/0x02025b38 |
| 10 | `func_020209d0` / `func_02020990` / `func_02020a14` | `NNS_FndAllocFromExpHeapEx` / `NNS_FndFreeToExpHeap` / `NNS_FndCreateExpHeapEx` | tentative | E4 argument shapes (heap,size,align) / (heap,p) / (start,size,opt), inside the NNS_Fnd exp-heap cluster. *verify*: E2 vs pokediamond libnns NNS_FND_expheap.c. Note that the address order is the reverse of the source order, so check the file layout |
| 11 | `data_0208fe70` | `gRandSeed` | strong | LCG `x*0x196225+0x3c6ef35f`, return `x>>16`, inlined in 10+ matched files (src/ov001_c, main_b3) (E7) |
| 12 | `func_0203492c` | `Rand_SeedFromRTC` | strong | Draft: zeroes gRandSeed, calls `func_0200f0a0(&time)`, and on success seeds `s | m<<6 | h<<12`, mirrored into the high half |
| 13 | `func_0200f0a0` | `RTC_GetTime` | tentative | Fills {hour,min,sec} and returns 0 on success, inside the RTC cluster next to `RTCi_ReadRawTimeAsync`. *verify*: E2 |
| 14 | `data_0206e470` | `FX_SinCosTable_` | confirmed | E8: interleaved s16 {sin,cos} starting `0,4096,6,4096,13,4096` at `.rodata` start. 14 main functions index it |
| 15 | `func_0200225c` (54 ov001 callers) | `FX_Div` | strong | Matched body = `FX_DivAsync(n,d); return FX_GetDivResult();`, the exact NitroSDK FX_Div (E4) |
| 16 | `func_02002978` | `FX_Atan` | tentative | Draft: arctan via a table indexed `>>5`, with ±0x4000/±0x2000 limits, in the FX cluster. *verify*: E2 |
| 17 | `func_020076cc` | `OS_ResetSystem` | strong | Draft: OS_Terminate guard, `func_0200f27c(OS_GetLockID())`, `func_02007ab8(0..3)`, `OS_SetIrqMask(0x40000)`, `OS_ResetRequestIrqMask(~0)`, stores param, `func_0200774c(0x10)`, then the ITCM routine `func_01ff8480`. This is OS_reset.c's sequence. It also gives `MI_StopDma` (0x02007ab8), `CARD_LockRom` (0x0200f27c), `OSi_SendToPxi` (0x0200774c) and `OSi_DoResetSystem` (0x01ff8480) as tentative names |
| 18 | `func_02030538` | `System_SoftReset` | strong | Called from main() when held keys contain 0x30c (L+R+START+SELECT). Sets both master brightness 0x10, unloads overlay 0, calls `OS_ResetSystem(param|0x80000000)` |
| 19 | `func_02035134` | `Pad_Update` | strong | main() passes `((KEYINPUT|EXTKEY)^mask)&mask`. Body computes prev/held/press/release/repeat with per-bit repeat timers (E5+E6) |
| 20 | `func_020352a8` | `Pad_ReadHardware` | strong | Reads 0x04000130 KEYINPUT plus ext keys, then calls `Pad_Update` (E5) |
| 21 | `func_020302d8` | `Overlay_Update` | strong | Every frame: if requested id ≠ loaded id, `FS_UnloadOverlay(0,..)` then `FS_LoadOverlay(0,id)`, then runs the pending one-shot callback |
| 22 | `func_02036a0c` | `Oam_Flush` | strong | main() calls it with 0 and 1. Walks 32 priority buckets of linked 8-byte entries, copies 6-byte OAM attributes, clears the buckets |
| 23 | `func_02030598` | `System_Init` | strong | First call in main(). Writes POWCNT 0x04000304, IE, 0x04000580 and the 3D regs (E5+E6). The sub-role split needs reading |
| 24 | `func_0203005c` | `Display_VBlankUpdate` | strong | Runs after `VBlankIntrWait` in main(). Writes DISPCNT, BG offsets 0x10–0x30, BLDY 0x4c, MASTER_BRIGHT 0x6c (E5+E6) |
| 25 | `func_020405e0` (43 callers) | `WH_ChangeSysState` | strong | Prints `"%s -> "` and `"%s\n"` with names from data_020785c4 = {"WH_SYSSTATE_STOP", "IDLE", "SCANNING", "BUSY", "CONNECTED", "DATASHARING", "KEYSHARING", "MEASURECHANNEL", "CONNECT_FAIL", "ERROR"} and stores the state (E3+E8) |
| 26 | `func_020405bc` (43 callers) | `WH_SetError` | strong | Stores the error code unless state ∈ {9 ERROR, 10} (index proven by the name table) |
| 27 | `func_0203ddd8`, `func_0203dc28`, `func_0203de90` | `WH_StepDataSharing`, `WH_Finalize`, (`"WM not Initialized"` guard) | strong | Self-naming debug strings (E3). Apply the same rule to the `WH_StateIn*/StateOut*` handlers 0x0203eb10–0x02040048 whose string names the function itself |
| 28 | `func_02034ec0` | `Wireless_UpdateDataSharing` | strong | Called from main() after VBlank. Runs only if `func_0203e934()==5` (WH_SYSSTATE_DATASHARING), then `WH_StepDataSharing` (E6+E8). `func_0203e934` → `WH_GetSystemState` (strong by the same evidence) |
| 29 | `func_02036754` (235 callers) / `func_0204b7ec` (130 ov000 callers) | `Sound_PlaySeqArc` / `Sound_PlaySeqArcEx` | strong (role) | Matched: default handle `data_02250454`, stop if active, start, then set volume `data_02250444`. The callee 0x0201e9cc gets `(h,-1,-1,-1,arc,idx)` = NNS_SndArcPlayerStartSeqArcEx shape. Callees 0x0201ea88/0x0201e9cc/0x0201be98 → `NNS_SndArcPlayerStartSeqArc`/`...Ex`/`NNS_SndPlayerSetVolume` are tentative until E2 |
| 30 | `func_02031a68` (215 callers) | `Model_SetAnimation` | strong (role) | Matched: per slot, remove/free the old AnmObj, alloc a new one with `(gSystemHeapAllocator, anm, resMdl)`, init it, add it to the RenderObj. NNS callees 0x02025b44 `NNS_G3dAllocAnmObj`, 0x02025b38 `NNS_G3dFreeAnmObj`, 0x02021f60 `NNS_G3dAnmObjInit`, 0x02021d28 `NNS_G3dRenderObjAddAnmObj`, 0x02021c4c `NNS_G3dRenderObjRemoveAnmObj` are tentative (E4) until E2 |
| 31 | `func_02033550` (211 callers) | `AnimatorSprite_SetAnimation` | tentative→strong | Matched: sets the animation index into a BAC header entry table and resets the frame counters. The shape equals SRA `AnimatorSprite__SetAnimation(animator,u16)`. *needs*: one proof that `header` is a loaded `.bac` file (trace from a ctor that loads `/act/*.bac`) |
| 32 | `func_02069818` (179 ov000 callers) | `StageObj_SetAnimation` | strong (after #30/#31) | Matched: clears flag bits in `+0x5c`, then sets the animation on the 3D model at +0x124 (Model_SetAnimation) and on the sprites at +0x94/+0xdc and the child at +0x12c (AnimatorSprite_SetAnimation). It is the 0x20C object header (include/ov000_b/stageobj.h) |
| 33 | `data_ov000_0236498c` (+0xE0 = 0x02364a6c) | `gMapObjectCreateTable` | strong | 173 consecutive ov000 code pointers, each a ctor that loads one object's assets (E8+E3) |
| 34 | `func_ov000_02316ba0` | `Spring_Create` | strong | Sole asset `/act/ac_gmk_spring.bac`. Referenced only from the ctor table (several type ids). Same rule for `goalring` 0x0230a864/0x0230ab48/0x0230b344, `dash_c` 0x02307380, `dash_ct` 0x02306f08, `cannon`, `crane` and so on |
| 35 | `func_ov000_0234d62c` | `Player_Create` | strong | Loads `/act/son.bac`, `son.nsbmd`, `blz.nsbmd`, `son.nsbca`. Called 4× from main 0x0204c28c–0x0204c37c (stage start). ov001 has no code at that address, so the callee is ov000 |
| 36 | `func_020511ec` / `func_02051248` / `func_020512a4` | `DownloadPlay_GetBlazePreview` / `..._SonicPreview` / `..._Contest` | strong | Each returns a descriptor with `/banner/prev_{blz,son}.nbf[cp]` or `contest`, `/mb/*.srl` and the banner text (E3) |
| 37 | `func_02055918` / `func_0204da44` | `PauseMenu_Create` / `GameOver_Create` | strong | `/act/ac_fix_pause.bac`, `/act/ac_fix_gameover.bac` (E3). Confirm "Create" by checking that it calls Task_Create |
| 38 | `func_ov001_022d9458` / `func_ov001_022cf90c` | `TitleScreen_LoadAssets` / `SegaLogo_LoadAssets` | strong | `/act/mnu_titl.bac` / `bg/sega_logo_*.bbg` (E3) |

Lower-priority follow-ups that are already half-proven:

- `func_02037c60`: frame-level system task (touch panel?) that picks `0x02037a8c` or
  `0x0203787c` by flags. 0x02037a8c polls `func_0200d650`/`func_0200d5f8`. Identify
  those as `TP_*` by E2 first.
- `func_0202fd94`: lid-close/sleep handler that calls `func_0200e098(0xc,0,0)` (IO
  DISPCNT, IE, POWCNT). That callee is probably `PM_GoSleepMode`. *verify* E2.
- `func_02034114`: file-request queue pump (states 1/3/5). See include/main_a2/fileload.h.
- `func_02036fb8` / `func_02036f94`: indirect calls through a stored (fn, arg0, arg1)
  callback slot.
- `func_0204a238..0204a98c`: one boss-stage model loader per boss, named by their
  `/mod/bs<N>_*.nsbmd` strings (`BossStage<N>_LoadModels`).
- `func_02069f1c` (159 callers), `func_0206a054` (143), `func_ov000_022fba50` (112) and
  `func_ov000_0234c4a4` (78): the stage-object init/collision family. Name them after
  #32 fixes the header semantics.

---

## 5. Quick checklist for a naming agent

1. Pick an anchor's neighbourhood (callers or callees).
2. Run `python3 -I tools/analysis/fnmap.py <mod> <addr-or-name>` and read
   `build/eur/ghidra/<mod>/decomp/<sym>.c` and the matched `src/*/<sym>.c`.
3. Assign an E-code. With no E-code, keep the name and move on.
4. Write a `NAME` line (section 2.3). Never edit config/, never rename files yourself.


## 6. Rules added during the refactor

**Repeated library statics.** A file-local (static) SDK/NNS helper whose official
name occurs in more than one source file is named `<OfficialName>_<file stem>`
(no doubled underscore): `AlarmCallback_stream`, `AlarmCallback_capture`,
`AllocFromHead_frameheap`, `updateHintVec_kernel`, `texmtxCalc_flagT_maya`. The
list of such names is `REPEATED_STATICS` in `tools/analysis/rename.py`, which
rejects the bare name.

**Subsystem-only names.** When a function's subsystem is proven but its exact
role is not, it is named `<Subsystem>_Unk_<addr>` (overlays:
`<Subsystem>_Unk_ovNNN_<addr>`) and logged with confidence `tentative` when the
only evidence is that all referencers belong to that subsystem (E6 alone).
Upgrade the name when the behaviour is proven.

**Case rule** (above): enforced by `rename.py` and `check_symbols.py`.

## 7. Refactor status (2026-11-16)

| Round | Renamed | Scope |
|---|---|---|
| 1 | 2,358 | library official names; engine core; shared game code; ov000 map objects, player, rings, effects, HUD; ov001 screens |
| 2 | 1,575 | MB/WFS, wireless manager, stage flow, scene changes, player state machine, bosses, special stage |
| 3 | 1,626 | final sweep plus `_Unk_` upgrades |
| 4 | 2,305 | every remaining function: bosses/part chains, per-zone hooks, menus, library statics, audit fixes |
| 5–6 | ~1,670 | `_Unk_` upgrades to behaviour names; repeated-static scheme; `_fgr`/`_fgeq`, FS overlay and NNS audit fixes |

**All 7,408 functions are named**; 468 of them are subsystem-level
`_Unk_` names (`tentative` in the evidence log) awaiting behaviour names. Every
batch was applied with `tools/analysis/rename.py` and verified with
`tools/utilities/verify_all.sh` (SHA-1, GNU relink, objdiff unchanged).

**Audit (round 4).** All 1,146 named library functions were compared with the
reference function of the same name (609 byte-identical, 358 structurally
≥ 0.95, the rest consistent with the SDK version gap). Three were wrong and were
corrected: `FS_StopOverlay` → `FS_UnloadOverlayImage`, `FS_LoadOverlayFile` →
`FS_LoadOverlayImage` (both had been taken from a newer SDK), and
`NNS_SndPlayerCountPlayingSeqByPlayerNo` → `NNS_SndPlayerCountPlayingSeqBySeqArcIdx`.
`WMi_StartParentEx` was re-verified correct (it issues WM command 8, start
parent; `WM_SetEntry` issues 0x21), as were the FS rom-cluster names. A sample
of 276 game names (all single-E6 names plus 105 random round-1 names) held.

Renamed to avoid case-only collisions: `OSi_UnLockCartridgeAlias` (SDK alias
`OS_UnLockCartridge`) and `Mic_InitSystem` (game MIC init).

Open: upgrade the `_Unk_` names; `_fgr` (0x0202e620) returns 1 on `>=` (the
name comes from the compiler-generated call, kept); zone numbers are not yet
tied to the in-game zone titles.
