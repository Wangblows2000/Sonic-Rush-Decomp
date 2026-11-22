# Dreamcast port — graphics

Renderer: `dreamcast/game/engine/render.c` (PVR lists OP, PT, TR; 640x480 RGB565;
textured quads in world space at 2.5x, CAMERA.md), `engine/level.c` (PAL8 twiddled block
pages and BG; the BG line scroll and palette effects of section 8), `model/model.c` (NSBMD meshes), and the module drawers (objects, UI, menus,
special stage, cutscenes, bosses).

This file is about PVR texture memory (VRAM): the per-stage budget, the paletted sprite
atlases, the palette RAM allocator and the stage teardown.

## 1. Texture memory

`pvr_init` (main.c) uses `opb_sizes = {16, 0, 16, 0, 16}` (OP, TR, PT lists),
`vertex_buf_size = 512 KB`, `opb_overflow_count = 3`. KOS `pvr_allocate_buffers` reserves,
for each of the two frame halves: the vertex buffer (524,288), the object pointer blocks
(3 lists x 64 B x 300 tiles x (1 + 3 overflow) = 230,400), the tile matrix (0x48 + 7,224)
and the 640x480x2 frame buffer (614,400), all 128-aligned: 1,376,384 bytes per half.
**Texture RAM = 8 MiB - 2 x 1,376,384 = 5,635,840 bytes (5,504 KB).** Flycast prints
exactly this after `pvr_init` (section 4).

Every `pvr_mem_malloc(n)` costs `(n + 4)` rounded up to 32 (KOS dlmalloc chunk: size word,
32-byte alignment). PVR textures are power-of-two, 8..1024.

What is resident during a stage:

| holder | loaded / freed | size |
|---|---|---|
| level | `level_load` / `level_free` per stage | blocktex pages 512x512 PAL8 (256 KB each) + BG (PAL8) |
| objects | `objects_init` (all slots of the placed types, preloaded) / `objects_shutdown` | sprite atlas pages |
| UI | `ui_init` once (HUD, title card, results, pause, game over, NaviB, BossHelp, enemy room); title-card BG, results BG, navigator per stage | sprite atlas pages, 128x128 face texture (boss) |
| player | `load_player_model` (son.dcm or blz.dcm) | 139,424 bytes (5 textures) |
| boss | `boss_init` / `boss_shutdown` (boss acts) | the boss models of `boss/assets.txt` |
| special stage | `special_init` / `special_shutdown` (the act's level and objects are freed first) | models, BG, sprites |

## 2. Per-stage budget

`tools/dreamcast/vram_budget.py` computes the table from the converted files with the rules
above (objects: the resources `objects/test/test_coverage` with `COV_RES=1` reports after
creating every placed object and decoration of the stage; no resource is loaded later).
"Before" = every sprite page ARGB1555 with the old shelf packer (the loaders up to commit
d1248d81); "PAL4" = the PAL4 atlases of section 3 (commit 7da1f648); "repacked" = PAL4 with
the MaxRects packer (SPRITES.md section 4) and the objects' current preload list (it now
includes the player effects of `obj_peffect.c`, about 75 KB). "Total now" / palette rows
are for "repacked". KB = 1024 bytes, all pages counted with their allocation chunk.
UI = ui_init's resources + the title-card background of the stage (+ the navigator and the
face texture on boss acts; before: + the 256x256 ARGB1555 variant cache the HUD tension
gauge allocates on its first frame). Palette rows: of 64 (section 3.2).

```
python3 tools/dreamcast/vram_budget.py [--res res.txt] [--json out.json]
```

| stage | level | objects: before / PAL4 / PAL4 repacked | UI: before / PAL4 / repacked | player | boss | total before (free) | total now (free) | palette rows |
|---|---|---|---|---|---|---|---|---|
| z11 | 2304 | 1562 / 391 / 335 | 1120 / 248 / 145 | 136 | 0 | 5122 (381) | 2921 (2583) | 45 |
| z12 | 2304 | 1622 / 406 / 344 | 1120 / 248 / 145 | 136 | 0 | 5182 (321) | 2930 (2574) | 46 |
| z13 | 256 | 316 / 79 / 47 | 1280 / 312 / 209 | 136 | 585 | 2555 (2949) | 1233 (4271) | 13 |
| z21 | 2176 | 2047 / 513 / 411 | 1120 / 248 / 145 | 136 | 0 | 5480 (24) | 2868 (2636) | 44 |
| z22 | 1920 | 2095 / 525 / 420 | 1120 / 248 / 145 | 136 | 0 | 5272 (232) | 2621 (2883) | 45 |
| z23 | 256 | 452 / 113 / 67 | 1280 / 312 / 209 | 136 | 741 | 2846 (2657) | 1409 (4095) | 16 |
| z31 | 2816 | 2154 / 540 / 454 | 1120 / 248 / 145 | 136 | 0 | 6227 (**-723**) | 3551 (1952) | 48 |
| z32 | 2304 | 2186 / 548 / 460 | 1120 / 248 / 145 | 136 | 0 | 5747 (**-243**) | 3045 (2458) | 48 |
| z33 | 256 | 316 / 79 / 48 | 1280 / 312 / 209 | 136 | 619 | 2572 (2932) | 1268 (4236) | 13 |
| z41 | 2304 | 2165 / 542 / 445 | 1120 / 248 / 145 | 136 | 0 | 5726 (**-222**) | 3030 (2473) | 64 |
| z42 | 2304 | 2105 / 527 / 437 | 1120 / 248 / 145 | 136 | 0 | 5666 (**-162**) | 3022 (2481) | 62 |
| z43 | 256 | 308 / 77 / 47 | 1280 / 312 / 209 | 136 | 730 | 2532 (2972) | 1378 (4126) | 13 |
| z51 | 1408 | 2449 / 613 / 503 | 1120 / 248 / 145 | 136 | 0 | 5114 (390) | 2193 (3311) | 50 |
| z52 | 1408 | 2433 / 609 / 502 | 1120 / 248 / 145 | 136 | 0 | 5098 (406) | 2192 (3312) | 51 |
| z53 | 256 | 308 / 77 / 47 | 1280 / 312 / 209 | 136 | 595 | 2541 (2963) | 1243 (4261) | 13 |
| z61 | 1152 | 1590 / 398 / 363 | 1120 / 248 / 145 | 136 | 0 | 3998 (1505) | 1796 (3708) | 50 |
| z62 | 896 | 1598 / 400 / 365 | 1120 / 248 / 145 | 136 | 0 | 3750 (1753) | 1542 (3962) | 52 |
| z63 | 256 | 308 / 77 / 47 | 1280 / 312 / 209 | 136 | 691 | 2637 (2867) | 1339 (4165) | 13 |
| z71 | 2432 | 1645 / 412 / 344 | 1120 / 248 / 145 | 136 | 0 | 5334 (170) | 3058 (2446) | 57 |
| z72 | 1920 | 2253 / 564 / 446 | 1120 / 248 / 145 | 136 | 0 | 5430 (74) | 2648 (2856) | 59 |
| z73 | 256 | 300 / 75 / 43 | 1280 / 312 / 209 | 136 | 734 | 2622 (2882) | 1378 (4125) | 13 |
| z81 | 768 | 698 / 175 / 181 | 1120 / 248 / 145 | 136 | 0 | 2723 (2781) | 1230 (4274) | 34 |
| z82 | 1024 | 504 / 126 / 151 | 1120 / 248 / 145 | 136 | 0 | 2785 (2719) | 1456 (4047) | 34 |
| zf3 | 256 | 292 / 73 / 42 | 1280 / 312 / 209 | 136 | 1087 | 2989 (2515) | 1731 (3773) | 13 |
| z91 | 1024 | 865 / 217 / 219 | 1120 / 248 / 145 | 136 | 0 | 3145 (2359) | 1524 (3980) | 44 |
| z92 | 896 | 708 / 177 / 198 | 1120 / 248 / 145 | 136 | 0 | 2861 (2643) | 1375 (4129) | 38 |

- Extra Zone boss (no level): models 1,002 KB, `boss/extra` sprites 18 KB (PAL4, was 80 KB
  ARGB1555), UI 113 KB, player 136 KB = 1,270 KB, 4,234 KB free.
- Cutscenes (`/cd/cutscene`, run between stages or as boss-talk overlays): all 17 sprite
  sets together 909 KB before, 173 KB now (`ac_sf` 512 -> 80 KB, `ac_sf_logo` 96 -> 37 KB
  (one page keeps 16+ colour images), `poly_demo_under2` 64 -> 16 KB). A scene loads only
  its own sets.
- Before, a negative "free" means allocations failed: on z31/z32/z41/z42 the last object
  atlases, the title-card background, the variant cache and effects had no texture.
  At `objects_init` z41 had 34,600 bytes left (measured; the model gives 34.9 KB, the
  remaining title-card BG and variant cache then failed).
- Now every stage keeps at least 1.9 MB free (z31). No page falls back to ARGB1555 for
  want of palette rows, but z41 now uses all 64 rows at `objects_init` (z42 62, z72 59):
  the UI rows taken later (title card, pause) reuse idle rows or expand a page (section 3.2).

## 3. Paletted sprite atlases

### 3.1 Converter output

`tools/dreamcast/sprites/bac2dc.py` keeps the DS palette indices of every composed frame
(`bac_format.render` returns the index plane next to the RGBA image) and writes, per BAC,
besides `frames.bin`, `atlas<N>.png` and `atlas<N>.argb1555` (unchanged layout, still read
by the boss module and the host tests):

- `atlas<N>.pt4`: PVR PAL4 twiddled page (4 bpp, w*h/2 bytes; KOS `PVR_TXRLOAD_4BPP` order).
- `palettes.bin` ("DCPL" v1, `engine/sprpal.h`): page formats, the 16-colour palettes
  (ARGB1555, entry 0 transparent), the entries each palette's texels use (mask), the
  palette of every image, the OBJ row-0 palette of every animation's first frame
  (`ObjDraw_LoadPaletteRow`, PaletteAnim), and the DS row each image's texels index.

Image classes: **raw** (all opaque pixels read one 16-colour DS palette row with a non-zero
index: the texel is the DS index and the palette is that row, so palette swaps and
PaletteAnim are a bank choice); **remapped** (at most 15 colours, e.g. 8bpp sprites: the
colours are grouped first-fit into shared 16-entry palettes); **direct** (16+ colours: only
`ac_sf_logo`, `title_demo*`; these images go on separate ARGB1555 pages). The converter
checks every texel: PAL4 texel + image palette == ARGB1555 atlas pixel (exact fidelity).
All 412 BACs convert; 4 BACs keep an ARGB1555 page. `tools/dreamcast/sprites/atlas_pal4.py`
does the remapped conversion for atlases not made from a BAC (UI backgrounds of `uibg.py`:
all 32 become PAL4).

Sprite VRAM drops 4x (pages are the same sizes, 4 instead of 16 bits per texel).

### 3.2 Palette RAM

The PVR has 1024 palette entries (64 rows of 16, `pvr_set_pal_entry`), shared by every
paletted texture, in one format (ARGB1555). `engine/render.c` owns them:

- `pal8_alloc(pal, rowmask)` (level, BG): a 256-entry bank whose rows the texels use
  (`pal8_rowmask`) are free; only those rows are taken and written. z41: level 11 rows +
  BG 13 rows instead of 32.
- `pal4_get(col, mask)` / `pal4_put`: counted 16-entry rows (objects, special stage,
  cutscenes: taken at load). Rows are shared by content: a request whose used entries
  agree with a row's defined entries reuses it and defines its other entries. New rows are
  taken from row 63 down, so the low 256-entry banks stay free for the level.
- `pal4_use(PalRef *, col, mask)` (UI, menus): taken on first draw, kept while drawn; a row
  not drawn for 3 frames (the PVR is done with it) may be given to another request. The
  level's `pal8_alloc` may also take such rows.
- If no row is free, the page is expanded to ARGB1555 at load (static) or at its first draw
  (UI/menus keep the texels in RAM): exact, only 4x the VRAM. VQ is not needed: no stage
  runs out (section 2).

`spr_atlas_load` / `spr_atlas_adopt` / `spr_atlas_bank` (render.h) wrap this for the
module loaders; `draw_sprite_world_pal` / `draw_rect_screen_pal` / `tex_txr_format` draw a
paletted page with a bank.

### 3.3 Palette effects as bank selection

- **UI variants** (`ui_draw.c`): the tension-gauge colours (`UI_VAR_GAUGE0+k`), the pause
  dimming LUT (`UI_VAR_DIM`) and the NaviB row-swap colour maps (`UI_VAR_MAP0+n`) are
  colour functions; on a PAL4 page the variant is the image's palette through the function,
  drawn as another bank with the same texels (exact: the function sees only colours). The
  256x256 ARGB1555 variant cache remains only for ARGB1555 pages or a full palette RAM.
- **PaletteAnim** (`objects/obj_palanim.c`, `OBJ_FX_PALETTE_ANIM`): PaletteAnim_Create /
  _Main / _Release translated; the sequence tables come from ov000
  (`objects/tools/objdata.py` -> `palanim.bin`). A decoration started with it draws its
  row-0 images with the bank of `anim_pal[current animation]` (DecGroupAF neon ball /
  pillar / star, DecArrow, DecGroup95 lights). The effect hook is still called.
- **Blaze palette swaps**: not done (section 6).

## 4. Flycast verification (2026-11-22)

Test discs from the tree with a probe in main.c (VRAM free, palette rows, object and UI
page bytes); display :95.

| point | before (HEAD d1248d81) | after |
|---|---|---|
| after `pvr_init` | 5,635,840 | 5,635,840 |
| after `ui_init` + player model | 4,611,336 | 5,274,888 |
| z41 after `objects_init` | 34,600 (owner's run) | 2,360,104 (objects 553,984 B, 33 PAL4 + 24 PAL8 rows) |
| z71 after `objects_init` | 436,232 | 2,362,376 (objects 420,864 B) |
| z31 after `objects_init` (largest level) | (does not fit) | 1,838,440 (objects 551,168 B) |

The offline model is within 344 bytes of every measurement (z11, z31, z41, z71). Screenshots of
z41 and z71 show the HUD (gauge colours from variant banks), the neon decorations, rings
and enemies with their DS colours; 59.5 fps.

Stage changes (cycle of z11..z42, 300 or 200 frames each, through the stage exit path):
with `ui_stage_end()` wired, VRAM before every `level_load` is 5,274,888, the value after
boot, for normal and boss acts (z13, z23, z33). Without it the previous stage's title-card
background and navigator (32 KB / 64 KB) stayed until the next stage start (not cumulative).
Special-stage round trip (z11, entered at frame 300, played to the end): 5,274,888 before
`special_init`, 5,274,888 after `special_shutdown` (all 64 palette rows free), and z11
again gives 2,514,632 after `objects_init`, the same as the first entry. The special-stage
HUD and sprites draw from PAL4 pages.

Front end (HEAD d59309c8, 2026-11-22): title -> attract demo (z12) -> title -> main menu
-> new game -> z11 -> z12 -> DemoScript cutscene -> z13 boss -> zone map with the
navigator talk. Before every `level_load` VRAM is 5,364,232 (after boot: 5,520,104 minus
the player model and the loading ring), the same after the demo, after the menus and after
each act; the front end takes 1-7 palette rows (taken on first draw, given back when idle),
the zone map 4,562,088 bytes free. No page fell back to ARGB1555 in any run.

## 5. Teardown and leaks

- objects: `objects_shutdown` frees every resource (pages, palette rows, palanim state).
- UI: `ui_stage_end()` (new) frees the title-card / results backgrounds, the navigator and
  the face texture; main.c must call it at the stage exit and before the special stage.
- menus: textures are made on first draw and freed with their resource (`mn_res_free`).
- special: `special_shutdown` frees the sprite atlases with the resources.
- cutscene: `cs_res_free` frees the atlas.
- level: `level_free` frees the pages and the palette rows; a failed `level_load` now frees
  what it had allocated.
- main.c `load_player_model`: switching Sonic <-> Blaze loads the other model without
  freeing the previous one (139 KB of VRAM and the file buffers per switch).
- boss: boss acts return to the baseline (Flycast, z13/z23/z33).

## 6. Stage loading I/O

Measured in Flycast (main-thread `fs_open` / `fs_read` counted with a link-time wrapper in a
test build; stage cycle z11 -> z92, 60 frames each, from a debug start; emulated time).

What a stage load opened before (HEAD c49a9e18): z11 = 13 opens / 470 reads, of which
`objects_init` 169 opens / 402 reads (frames.bin, .hbx, palettes.bin and the pages of each
of the ~40 sprite resources, plus `objdata.bin` read by stdio in 1 KB requests). Every open
is an ISO 9660 walk of a directory with hundreds of entries (~25-30 ms), every request
several ms; while the zone music streams, the two readers seek against each other and the
same requests take seconds (z12 `level_load` 7.1 s against 0.16 s for z11, whose music had
not started yet).

Changes (engine/pak.c, tools/dreamcast/mkpak.py, objects):
- `/cd/data.pak`: the small files of `/cd/sprites`, `/cd/objects` (hit boxes, objdata.bin,
  palanim.bin), `/cd/ui/res`, special-stage and Extra-boss sprites (and `/cd/models`, see the
  wiring) in one file, opened once, a hash index in RAM (12 bytes per file), files in load
  order (frames.bin, hit box, palettes.bin, pages) and a 64 KB read-ahead so a resource's
  small files come from one request. The ARGB1555 copies of PAL4 pages are left out (391
  files): the disc gets smaller. A directory in the pack is authoritative: a missing file
  costs no disc lookup.
- `/cd/objects/stageNN.pak`: per stage, the files `objects_init` loads (from
  `make -C objects preload`, the test_coverage list) in load order, read whole in one
  request (`pak_stage_begin` / `pak_stage_end` around the preload; 160-740 KB of RAM during
  `objects_init` only).
- All module file loaders (objects, UI, menus, cutscene, special stage, Extra boss, sprite
  atlases) ask `pak_load` first; reads go through `loading_read` (the ring keeps turning).

Per stage: total ms from `load_stage` to the end of `spawn_player` (objects_init ms; the
main thread's opens and read requests during the whole load).

| stage | before (HEAD c49a9e18) | data.pak + stage packs | + models packed, music after the load |
|---|---|---|---|
| z11 | 6291 (4144; 199 opens, 546 reads) | 1976 (55; 11 opens, 114 reads) | 1943 (47; 8 opens, 109 reads) |
| z12 | 6342 (4094; 200 opens, 549 reads) | 1839 (40; 11 opens, 116 reads) | 1804 (31; 8 opens, 111 reads) |
| z13 | 18536 (2807; 75 opens, 1933 reads) | 21163 (48; 33 opens, 1193 reads) | 1447 (29; 8 opens, 175 reads) |
| z21 | 6712 (4547; 186 opens, 531 reads) | 1899 (56; 11 opens, 112 reads) | 742 (55; 8 opens, 107 reads) |
| z22 | 4558 (2688; 189 opens, 526 reads) | 794 (56; 11 opens, 104 reads) | 1914 (54; 8 opens, 99 reads) |
| z23 | 34158 (2990; 106 opens, 2255 reads) | 20093 (48; 56 opens, 1508 reads) | 1111 (46; 7 opens, 136 reads) |
| z31 | 7801 (5684; 223 opens, 588 reads) | 861 (57; 11 opens, 134 reads) | 777 (51; 8 opens, 129 reads) |
| z32 | 8151 (6034; 223 opens, 572 reads) | 811 (57; 11 opens, 119 reads) | 758 (49; 8 opens, 114 reads) |
| z33 | 28906 (2873; 87 opens, 1902 reads) | 20646 (48; 44 opens, 1161 reads) | 2014 (46; 7 opens, 102 reads) |
| z41 | 8151 (5901; 210 opens, 559 reads) | 727 (57; 11 opens, 118 reads) | 1797 (48; 8 opens, 113 reads) |
| z42 | 6362 (4411; 215 opens, 565 reads) | 761 (57; 11 opens, 119 reads) | 677 (48; 8 opens, 114 reads) |
| z43 | 26550 (2790; 87 opens, 1818 reads) | 16563 (48; 45 opens, 1078 reads) | 1898 (46; 7 opens, 99 reads) |
| z51 | 7332 (5467; 204 opens, 523 reads) | 711 (57; 11 opens, 90 reads) | 691 (52; 8 opens, 85 reads) |
| z52 | 6414 (4244; 196 opens, 516 reads) | 1962 (57; 11 opens, 91 reads) | 611 (52; 8 opens, 86 reads) |
| z53 | 20393 (767; 81 opens, 1964 reads) | 15393 (48; 39 opens, 1224 reads) | 2081 (46; 7 opens, 105 reads) |
| z61 | 6865 (4914; 194 opens, 509 reads) | 744 (56; 11 opens, 83 reads) | 1864 (50; 8 opens, 78 reads) |
| z62 | 4424 (2238; 198 opens, 503 reads) | 1931 (56; 11 opens, 73 reads) | 658 (49; 8 opens, 68 reads) |
| z63 | 34676 (683; 94 opens, 2101 reads) | 29893 (48; 52 opens, 1361 reads) | 2098 (46; 7 opens, 120 reads) |
| z71 | 6363 (4344; 172 opens, 525 reads) | 1948 (55; 11 opens, 120 reads) | 677 (55; 8 opens, 115 reads) |
| z72 | 7402 (5415; 185 opens, 522 reads) | 727 (56; 11 opens, 106 reads) | 644 (57; 8 opens, 101 reads) |
| z73 | 44641 (2807; 91 opens, 2697 reads) | 22283 (48; 53 opens, 1961 reads) | 1931 (46; 8 opens, 138 reads) |
| z81 | 5109 (3325; 99 opens, 390 reads) | 711 (52; 10 opens, 53 reads) | 661 (51; 8 opens, 52 reads) |
| z82 | 5844 (3844; 83 opens, 382 reads) | 727 (51; 10 opens, 60 reads) | 1781 (51; 8 opens, 59 reads) |
| zf3 | 55982 (2708; 101 opens, 2840 reads) | 34627 (48; 67 opens, 2108 reads) | 2231 (45; 7 opens, 148 reads) |
| z91 | 5829 (4078; 109 opens, 409 reads) | 641 (52; 10 opens, 62 reads) | 594 (53; 8 opens, 61 reads) |
| z92 | 4488 (3578; 90 opens, 385 reads) | 1898 (54; 10 opens, 57 reads) | 658 (43; 8 opens, 56 reads) |

- Normal acts: 4.4-8.2 s -> 0.6-2.0 s; `objects_init` 2.2-6.0 s -> 31-57 ms (one open).
- Boss acts stay 15-35 s with the packs alone: the boss models are ~20-60 loose files in
  `/cd/models` (`boss_model.c` reads them with stdio). With the models in the pack (one
  hook in `boss/boss_model.c`, see the wiring) they load in 1.1-2.2 s.
- The rest of a load is the level (130-210 ms) and the sound bank: its 4 opens in
  `/cd/sound` take 50-150 ms or 1.2-1.3 s (the remaining variance between runs).
- With the current audio reader (c49a9e18) starting the music after the load changes
  little; it mattered with the old reader (z12 level 7.1 s).

## 7. Gaps

- Blaze palette reload of the results glyphs (ClearDemo_InitSprites: palette data at
  +0xcc instead of +4) is not done; it would be one more palette per glyph sprite.
- The BG alternate palettes of zones 4/5/7 (`rbgd_header_t.num_palettes`) are used: they are
  the sources of the zone palette animations (section 8), kept in RAM (`Level.bg_pals`), not
  more PAL8 rows.
- Water Palace: the tinted copies need a second PAL8 bank; on z21/z22 only the map tiles get
  one (rows free after `objects_init`), the BG is tinted with a vertex colour (section 8).
- Power-of-two pages are still a third empty on average.
- z41 uses all 64 palette rows at `objects_init`: one more palette there and a page goes
  ARGB1555.
- Not measured in Flycast: the Extra boss, cutscene overlays during boss acts.

## 8. Zone backgrounds and zone effects

Module `dreamcast/game/zonefx/` (zonefx.c, zonefx.h, tools/zonefxdata.py), run from inside
Camera_Main (`camera_set_zone_hook`), drawn through `engine/level.c`. Translation of
`src/game/Stage/ZoneBackground` (ZoneBg_*, ZoneFx_*) and `src/game/Zone/<zone>/Background`.
The DS runs these per DS screen; here every per-screen function runs once on a virtual
screen = the DC window (camera.h) with gCamera[0]'s flags, flags34, limit and water level.

### 8.1 BG scroll (gZoneBgUpdateTable)

The zone BG is DS BG1 (512x512 or 256x512 text BG, `ZoneBg_SetupBgControl`). Its scroll
(gCamera @0x2C/@0x30 -> BG1HOFS/VOFS, `ZoneBg_Update`), or the per-line table the line-scroll
task DMAs into the scroll registers, becomes a 192-row table (`zonefx_lines`).
`level_draw_bg_lines` draws rows with the same offset as one quad with repeating UVs (the
texture is the BG image size), so the wrap of the DS BG is kept.

| stage | function | rule |
|---|---|---|
| zone 1, z81 (Init only) | ZoneBg_Zone1_Update | x / 14, y * 48 / (map height - 192) |
| zone 2 | ZoneBg_Zone2_UpdateScreenScroll + BuildLineScroll | sky band (x 0, y * 34 / ...), above the water line - 0x3C the underwater part at x / 16, row 0x11A at the water line, below the line the wave (InitWave: angle frame << 9, speed 0x2000 - 1/10 every 8 rows, amplitude (0x500 - row) / 256 px, y cos wave) |
| zone 3, z82 (x fixed) | ZoneBg_Zone3_* | heat-haze band (two sine waves per row and frame), the dunes (x / 28), below the limit line (MapObj70) the near layer (x / 12, wrap 0x118) |
| zone 4, z91 | ZoneBg_Zone4_Update | x / 18, y * 48 / ... |
| zone 5, z92 | ZoneBg_Zone5_* | fixed sky band, far layer x / 28, near layer x / 14 under the limit line |
| zone 6 | ZoneBg_Zone6_Update | the clouds drift by 1/60 of the camera motion, y 0x30 |
| zone 7 | ZoneBg_Zone7_* | as zone 5 with the zone 7 rows (0xB8, 0x48) |

The title card / glider sequence freeze (`flags34 & 1`) is honoured.

### 8.2 Palette effects (gZoneFxUpdateTable)

PaletteTransfer targets map to the two PAL8 banks of `Level`: BG std palette 0x5000000 ->
`level_set_bg_palette`, ext palettes (slots 0x4000 / 0x6000, the map) -> `level_set_palette`.
Colours stay ARGB1555 (the DS channel formulas act on the DC channels; the alpha bit of the
destination is kept). `zonefx_commit_palettes` writes only changed palettes, only the rows
the texels use.

- **Palette animations** (`ZoneFx_InitPaletteAnim` / `StepPaletteAnim`): the tables
  data_ov000_02364df0 (Night Carnival, 24), _dd0 (Huge Crisis, 2), _de0 (Dead Line, 2)
  (`zonefx.bin`); the source of entry e is event-archive file e[1] = bg.bin palette e[1] - 3
  (the z4pnNN / z5pnNN / z7pnNN `.bbg`).
- **Rotations** (`ZoneFx_RotatePaletteRange`): Huge Crisis BG 0xCD..0xCF every 8 frames, Dead
  Line BG 0x9A..0x9C every 12 and map 0xB1..0xB7 (backwards) every 6.
- **Night Carnival darkness** (BLDY, `ZoneFx_Zone4_StartBldyFlash / RampUp / RampDown`, the
  effect table, flash table gZoneFxZone4BldyFlashTable): brightness decrease of BG1..BG3 =
  the BG and map palettes darkened (c - c * evy / 16), not the sprites and the 3D player, as
  on the DS. Started by `OBJ_FX_ZONE4_BLDY` (main.c -> `zonefx_bldy`).
- **Huge Crisis alarm** (`ZoneFx_Zone5_UpdatePalettes / ApplyPaletteAdd`): while an enemy
  room holds the camera (flags34 & 0x10000) the BG entries 0x50..0xCC and 0xD0..0xFF get
  +0..29 red, stepping every 2 frames.
- **Water Palace** (`ZoneFx_Zone2_*`): tinted copies (r - 10, g - 1) of the map and BG
  palettes for the rows below the water line (the DS VAlarm split, states 0..3 of
  `UpdateScreenStates` = the window's water row). `DmaTintedPalettes` copies 0x1E0 bytes:
  rows 0..14 are tinted, row 15 keeps its colours; the OBJ copy starts at 0x5000240, so OBJ
  rows 2..15 are tinted and rows 0 / 1 (rings, ring sparkles, the HUD) are not; the 3D
  layer (player model, 3D effects, the boost ribbon) has no palette tint on the DS either.
  `CyclePalette` rotates map entries 0x21..0x25 every 3 frames (normal and tinted); the
  surface sprites (`act/ac_eff_water.bac` animation 3, every 0x40 px along the line,
  `DrawSprites`) in the TR list. The tinted map / BG palettes go to a second PAL8 bank
  (`level_alloc_alt_banks`); where no bank is free the rows below the line are drawn with the
  vertex colour 0x99F7FF (MODULATE: red x 0.6, green x 0.97). z21 / z22: map bank 2 exact,
  BG vertex colour. Object sprites: `objects_set_water_tint` (objects patch,
  camera/patches/objects.patch) splits every sprite at the water line and draws the lower
  part with the same vertex colour (no free palette rows for tinted sprite rows).
  main.c's old translucent water rectangle is removed.
- **Mirage Road boss** (`ZoneFx_Zone3Boss_Update`): once the fight runs the three global
  light colours alternate every 7 frames between data_02074990 and data_ov000_02364dbc
  (`zonefx_boss3_lights`); boss3.c applies them to its lights (stage light vectors), which
  light the boss, the arena and the player model, as NNS_G3dGlbLightColor does.

### 8.3 Measurements (Flycast, display :97, debug-start discs)

Per stage: BG draw ms/frame (`draw ms/frame: bg`), palette writes, fps of the frame log.

| stage | BG | BG ms | palette writes / 120 frames | fps |
|---|---|---|---|---|
| z11 | parallax x / 14 | 0.04 | 0 | 59.3 |
| z21 / z22 | line scroll + wave, tint split, shimmer | 0.28 | 80 (shimmer + tinted) | 59.3 |
| z31 / z32 | haze / dunes / quicksand | 0.04 | 0 | 59.3 |
| z41 | parallax x / 18, 24 animations | 0.04 | 53 | 59.3 |
| z51 | bands, rotation + 2 animations | 0.04 | 35 | 59.3 |
| z61 | cloud drift | 0.04 | 0 | 59.3 |
| z71 | bands, 2 rotations + 2 animations | 0.04 | 30 | 59.3 |
| z82 | haze, x fixed | 0.14 | 0 | 59.3 |
| z91 | as z41 | 0.04 | 53 | 59.3 |

Host test (`make -C dreamcast/game/camera host-test`): BG tables against the formulas
(zone 1, 4, 6), the zone 2 wave below the water line, the zone 3 haze, palette animations
step from the alternate palettes (z41 2588 entry changes in 240 frames, z51 18986, z71 960),
BLDY ramp to 10 and back, flash, the alarm.

Palette RAM timing: `zonefx_commit_palettes` runs at the frame start (after `pvr_wait_ready`,
main.c patch); when a palette changed it waits for the render in progress
(`pvr_wait_render_done`) before writing, so no frame renders with mixed colours. The object
PaletteAnim rows and the UI's lazy rows (engine/render.c, objects) still write during list
building.

Known limits: the sprite tint is the vertex-colour approximation, not the exact r - 10.

Layering found in the sand-ship run (not zone effects): Stage_InitDisplay sets BG0 (3D:
player model, table model) priority 1, BG1 (zone BG) 3, BG2 (layer A) 0, BG3 (layer B) 2, so
OBJ priority 2 / 3 sprites (DecTableWater 0x53, shadows) go behind the player. The DC draws
every sprite (z 1.5..4.5) in front of the player model (~0.006): on the sand ship the water
strip hides the player. The mirage-road ship model (GmkTable) is not drawn by main.c.
