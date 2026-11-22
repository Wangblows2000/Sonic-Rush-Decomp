# Sonic Rush → Sega Dreamcast: porting report (Milestone 1)

Measured on 2026-11-22 against this repository (EUR `ASCP`, 100 % matched ARM9
decompilation). Numbers below come from scripts over `src/`, `config/` and the
user's extracted ROM (`extract/eur/`, never committed).

## 1. Summary

The ARM9 decompilation gives behaviour-exact C for all game code, but that code
is written against Nintendo DS hardware through NitroSDK and NitroSystem. A
native Dreamcast port is a re-hosting project: the game logic can be reused,
but every service it calls (2D engines, 3D geometry engine, sound sequencer,
file system, VRAM banks, interrupts, touch screen) has to be re-implemented on
KallistiOS. Rough size of that surface:

| Item | Count |
| --- | --- |
| Game translation units (`src/game/`) | 5,962 functions |
| Library TUs (`src/libs/`: NitroSDK, NitroSystem, MSL, runtime) | 1,389 functions |
| Game TUs that call NitroSystem (`NNS_*`) | ~3,000 call sites |
| Game TUs that call `OS_*` / `FX_*` / `DC_*` / `MTX_*` / `MI_*` / `GX_*` | 461 / 298 / 224 / 147 / 112 / 76 TUs |
| Game TUs touching DS IO registers (0x04xxxxxx) directly | 69 |
| Game TUs that use the 3D engine (`G3_*`, `G3X_*`, `NNS_G3d*`) | 308 |
| Game TUs that read the touch panel | 85 |
| Game functions still written as CW `asm` | 20 (+ `__pformatter`) |

The C itself is portable in the narrow sense (no inline asm except the 20
fallbacks, fixed-point `fx32` maths, plain structs) but it is not hardware
independent: register writes, DMA, VRAM-bank layouts and OAM formats appear
throughout the game code, not only in the libraries.

## 2. Hardware responsibilities on the DS

- **ARM9** (decompiled): all game logic, rendering set-up, file loading, sound
  commands (`NNS_Snd*` builds a command queue for the ARM7).
- **ARM7** (not decompiled; `extract/eur/arm7/arm7.bin`, task `r-arm7`): stock
  NitroSDK ARM7 component — sound mixing and sequencing (SSEQ player), touch
  panel/mic/RTC/power via SPI, wireless (WM). On Dreamcast its roles move to
  the AICA (audio) and the SH-4 (everything else).
- **2D engines A/B**: tiled BG layers, affine BGs, OAM sprites from BAC
  animation files, palettes, windows, brightness/blend, H-blank line scroll
  (`ZoneBg_ZoneN_BuildLineScroll`, HDMA in `ZoneMap`).
- **3D engine**: NitroSystem G3D (`.nsbmd` models, `.nsbca`/`.nsbta` animation)
  for bosses, boss stages, special stages, title/demo models and some stage
  objects (`Graphics/Model`, `Boss*`, `SpecialStage*`, `TitleDemo`).
- **Sound**: NitroSystem sound archives (SDAT). Measured in
  `snd/sys/sound_data.sdat`: 33 SSEQ, 19 SSAR, 27 SBNK, 26 SWAR (6.0 MB of
  wave archives), 2 STRM (stereo IMA-ADPCM, 37,403 Hz, 175 s and 213 s). Every
  zone/boss has its own SDAT (~390 KB each) of SSEQ + SBNK + SWAR.
  **Music and most sound effects are sequenced (SSEQ), not recorded streams.**
- **Files**: NitroROM filesystem through `FS_*`, LZ77-compressed NARC archives,
  custom formats: `.bac` (sprite animation), `.bbg` (backgrounds),
  `.ev`/`.dc` (object placement), `.df`, `.bb` (text).
- **Save**: SPI backup memory through `CARD_*` (`SaveData_*`).
- **Input**: buttons via `PAD`, touch panel via `TP_*` (menus, special stage,
  boss mash events, trick system prompts).

## 3. Asset inventory (ROM filesystem, measured)

| Directory | Files | Size | Content |
| --- | --- | --- | --- |
| `act/` | 343 | 3.4 MB | BAC sprite animations (`ac_*.bac`) |
| `act2/` | 61 | 0.5 MB | more BAC |
| `bg/`, `bg2/` | 670 / 557 | 5.0 / 6.1 MB | BBG backgrounds and palettes |
| `mod/` | 149 | 5.6 MB | NSBMD/NSBCA/NSBTA 3D models and animations |
| `narc/` | 141 | 8.6 MB | per-zone act/eve/map archives (LZ77) |
| `snd/` | 33 | 33 MB | SDAT sound archives |
| `talk/` | 240 | 1.0 MB | cutscene scripts/text |
| `ss/` | 8 | 0.1 MB | special-stage data |

## 4. What each DS service becomes on Dreamcast

| DS service | Dreamcast replacement | Difficulty |
| --- | --- | --- |
| 2D BG layers, OAM, palettes, blending | PVR textured quads; tile maps converted to textures or tile atlases; palette lookup via paletted PVR textures (4/8 bpp) | high — biggest single item |
| H-blank line scroll | per-scanline strips as PVR polys | medium |
| G3D models (NSBMD) | PVR triangle strips; same vertex/poly counts (no geometry added); NSBCA joint animation on SH-4 | high |
| Sound sequencer (SSEQ/SBNK/SWAR) | either a real-time sequencer driving AICA channels from converted samples, or offline rendering of each sequence to Yamaha ADPCM streams | high (see AUDIO.md) |
| STRM streams | offline convert to AICA ADPCM, stream from GD-ROM | low |
| NitroROM FS + NARC/LZ77 | ISO9660 via KOS `/cd/`, NARC/LZ77 decoded offline or at load | low |
| Backup memory | VMU (`vmufs`) | medium (save size budget) |
| Touch panel | controller remapping per use (menus: D-pad/A/B; special stage: analog; mash events: buttons) | medium, per feature |
| Two 256×192 screens | one 640×480 view, continuous camera (CAMERA.md) | high — changes every scene's presentation |
| Wireless (WM/MB/WBT, Download Play) | not portable; VS mode would need split-screen or removal | out of scope initially |
| ARM7 | none (AICA + SH-4) | — |
| Fixed-point maths (`FX_*`, `MTX_*`) | portable C | low |

## 5. Memory picture

The DS has 4 MB main RAM; the game's heaps fit in it (system exp-heap arena
128 KB + overlay-resident data). Dreamcast main RAM (16 MB) is not the limit.
VRAM is: DS keeps tile/sprite data in its own 656 KB of VRAM banks; on PVR every
BG layer and sprite sheet becomes a texture (twiddled, power-of-two). Budget
8 MB minus framebuffers (640×480×2×2 = 1.2 MB) and TA buffers (~1–2 MB) leaves
≈4.5–5 MB for textures per scene. Sound RAM (2 MB) cannot hold a zone's
SWAR + streaming buffers + system SFX at 4-bit ADPCM without management
(SWAR is already 4-bit IMA ADPCM / PCM8 on DS).

## 6. Realistic roadmap

1. **Foundation (M2)**: KOS toolchain, bootable CDI, PVR, AICA ADPCM, controller,
   original-asset loading. *(this branch, `dreamcast/`)*
2. **Asset pipeline (M3)**: STRM → ADPCM; SWAR waves → ADPCM; BAC/BBG/NCGR-style
   graphics → PVR textures; NARC/LZ77 unpack; NSBMD → PVR mesh dump with poly
   counts recorded. Assets are generated from each owner's ROM at build time.
3. **NitroSDK/NNS shim (M4)**: a `platform/` layer that implements the subset of
   `OS/FX/MTX/FS/GX/G2/G3/NNS_*` the game calls, on KOS. Compile `src/game/` and
   the portable parts of `src/libs/` with `sh-elf-gcc` against it.
4. **Presentation (M5)**: single-screen compositor, camera, HUD, cutscenes.
5. **Sound (M5/M6)**: sequencer or pre-rendered music.
6. **Zone-by-zone bring-up (M6+)**.

Steps 3–6 are each multi-month efforts for one developer. They have not been
started.
