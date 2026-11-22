# BAC sprite animations (DS) and the Dreamcast sprite pipeline

Status: converter working. All 1018 BAC sources in the EUR ROM convert (412 unique files).
Results are in the validation section below.

Tools (all written for this port, no third-party code):

| file | role |
|------|------|
| `tools/dreamcast/sprites/nds_archive.py` | Reusable module. NARC reader (BTAF/BTNF/GMIF, file names) and DS decompression: stored `0x00`, LZ77 `0x10` (LZ10), `0x11` (LZ11), Huffman `0x24`/`0x28`, RLE `0x30`. |
| `tools/dreamcast/sprites/bac_format.py` | BAC parser, command-stream walker, and VRAM/palette emulation. Composes frames from OAM pieces. |
| `tools/dreamcast/sprites/bac2dc.py` | Batch converter: atlas, raw PVR texture, frame table, previews, manifest. |
| `tools/dreamcast/sprites/dspr_check.c` | Reference reader and validator for `frames.bin` ("DSPR"). Builds with host gcc and sh-elf-gcc. |

## 1. Where BAC files are

- `files/act/*.bac` (343) and `files/act2/{caution,tmenu_vs,zmap}/*.bac` (61) are loose files.
- 614 BAC files sit inside NARC archives: `narc/z??_act_lz7.narc`, `act_com_lz7.narc`,
  `extra_act[_lz7].narc`, and `player_lz7.narc` (`act/son.bac`).
  An `*_lz7.narc` file is an LZ10 stream that wraps a NARC. No BAC inside a NARC is compressed
  on its own.
- Many files are byte-identical copies. The converter converts each SHA-1 once, and
  `manifest.json` maps every source to that one conversion.

Player: Sonic and Blaze are 3D models (`player_lz7.narc`: `son.nsbmd`, `blz.nsbmd`, `.nsbca`, `.nsbva`;
see `Player_Create` -> `StageObj_LoadModel`). `act/son.bac` in that archive, and its twin
`act/ac_sonic.bac`, hold no tiles and no palettes. They contain only the timing (37 animations),
frame assemblies, and per-frame collision boxes (command 7). `Player_SpriteCallback` feeds those boxes
to `ObjRect_SetBox3D`. These two are the only "data-only" BACs. Their frame tables and
hitboxes are still converted. The 2D Sonic/Blaze art lives in other BACs, for example
`ac_dec_sonic_neon`, `ac_fix_player_icon`, and `mnu_psel`.

## 2. NARC and compression (nds_archive.py)

NARC: header `NARC`, BOM `FFFE`, version `0x0100`, u32 file size, u16 header size (0x10),
u16 section count (3). Sections follow:
`BTAF` (u32 size, u16 count, u16 reserved, count × {u32 start, u32 end} relative to the GMIF data),
`BTNF` (standard NitroFS name table: main directory table of {u32 subtable offset, u16 first id,
u16 count/parent}; entries are `len|0x80 name u16 dirId` for directories and `len name` for files),
`GMIF` (u32 size, then the data).

Compressed streams use the header the engine checks in `VRAMTransfer_Flush` / `PaletteTransfer_Flush`.
The first u32 is `type | size << 8`, where `type & 0xF0` gives
`0x10` `MI_UncompressLZ16`, `0x20` `MI_UncompressHuffman`, `0x30` `MI_UncompressRL16`.
Any other value copies `size` bytes from header+4 (`Compress_GetData`).
In the ROM: NARCs use LZ10. BAC tile streams use LZ10 (10,460) or stored (13,575). Palettes are always
stored. LZ11, RLE and Huffman are implemented for other converters. They are tested only with
synthetic streams (`/tmp/sprites/test_comp.py`). The LZ11 4-byte long-match form (length > 0x110)
has not been exercised.

```python
from nds_archive import open_narc, decompress, maybe_decompress
for name, data in open_narc("extract/eur/files/narc/z11_act_lz7.narc").files: ...
```
CLI: `python3 tools/dreamcast/sprites/nds_archive.py list|extract|decompress ...`

## 3. BAC format

All values are little-endian. Every block starts with a u32 block size, and that size includes the u32.

### 3.1 Header

| off | type | meaning |
|-----|------|---------|
| 0x00 | char[4] | `"BAC\n"` |
| 0x04 | u32 | animation table offset (`res+4`: `BAC_FrameGroupFunc_ChangeAnimation`, `AnimatorSprite_Init`) |
| 0x08 | u32 | command-stream block offset (`res+8`: `AnimatorSprite_ProcessAnimation`) |
| 0x0C | u32 | frame-assembly block offset (`res+0xC`: `AnimatorSprite_Draw`, `FrameAssembly`) |
| 0x10 | u32 | palette block offset (`res+0x10`: `BAC_FrameGroupFunc_Palette`) |
| 0x14 | u32 | tile/texture block offset (`res+0x14`: `BAC_FrameGroupFunc_SpriteParts`) |
| 0x18 | u32 | info block offset (`MsgBox_InitSprite`). Present only when the header is 0x1C long, that is, when the animation table starts at or after 0x1C. In `e3_dm_title.bac` and `ac_gmk_bane.bac` the header is 0x18 bytes and there is no info block. |

The block order in the files is: info, animations, commands, frames, palettes, tiles.

### 3.2 Info block (optional)
`u32 size`, then a 0x18-byte header:
`u16 animCount`, `u16 maxUnits[4]`, then 7 u16 values (seen: `0,0,2,0xFFFF,0,0,0`, meaning unknown).
At block+0x1C come `animCount` entries of 0x14 bytes: `u16 units[4]` + 6 u16 values (meaning unknown, often `0,0,2,0,0,0`).
`units[k]` is the OBJ VRAM the animation needs under 1D mapping boundary k
(32K/64K/128K/256K). It is counted in boundary units. `MsgBox_InitSprite` selects it from the screen's
`GX_OBJVRAMMODE_CHAR_1D_*` value and passes it to `ObjVram_Alloc`.

### 3.3 Animation table
`u32 size`, then `(size-4)/8` entries of `{u32 cmdPos, u16 format, u16 pad}`.
`cmdPos` is relative to the command block start, so it already skips the size word.
`format` indexes `data_02072e64` (log2 bytes per 8×8 unit) and `data_02072d78` (GX texture format):

| format | meaning | count in ROM (with duplicates) |
|--------|---------|------------------------|
| 0 | OBJ 4bpp, 8×8 tiles, 1D mapping | 6032 animations |
| 1 | OBJ 8bpp, 8×8 tiles | 12 (`ac_sf_logo`, `title_demo*`) |
| 2 | OBJ bitmap (16bpp) | not used |
| 3..9 | 3D texture: pal4, pal16, pal256, direct, A3I5, A5I3, 4x4 | only 4 (pal16) is used: 223 animations in `*3d*.bac` |

Format 0/1 also selects the char-name step in `AnimatorSprite_Draw`: 8bpp doubles it.

### 3.4 Command stream
Each command starts with `u16 type, u16 length` (length includes these 4 bytes).
`AnimatorSprite_ProcessAnimation` runs handlers from `gBACFrameGroupFuncTable` (0x02072e00)
until one returns FALSE. Only EndFrame returns FALSE.

| type | handler | body after type/len | effect |
|------|---------|---------------------|--------|
| 0 | EndFrame | `u16 frameCount, u16 frameNo, u16 duration, u16 pad` | Shows the frame. `wait = duration << 4`. The speed is 0x10 per game frame, so `duration` = 60 Hz frames. Sets `unkC = frameCount` and `unkA = frameNo` (1-based). When the wait ends with `frameNo == frameCount`, the animation-end flag (2) is set. A non-looping sprite (flag 4 clear) then freezes on that frame. |
| 1 | FrameAssembly | `u32 frameOfs` | Selects the frame record (§3.5). Its `flags & 1` sets sprite flag 0x80000000. |
| 2 | SpriteParts | `n × {u32 tileOfs, u16 units, u16 pad}` | For each entry, transfers the stream at tiles+tileOfs (compressed header, §2) to `dst`. The whole stream is copied. Then `dst += ceil(units / 2^shift) << (shift + unitShift)`. `units` = 8×8 tiles; `shift` = OBJ 1D boundary of the screen (0 for 3D). Flag 0x8000 streams the same data from the file instead of RAM. |
| 3 | Palette | `u32 palOfs, u16 colours, u16 slot` | Transfers palette stream at pal+palOfs to `base + (slot + spritePal) * 32` (or `* 512` for extended palettes, modes 2/4). Seen: 16-colour (5400) and 256-colour (18) palettes. Slots 0..15 are used. |
| 4 | EndAnimation | `u32 cmdPos` | Jumps to `cmdPos`. This is the loop point: always a frame start of the same animation, usually frame 0 (3369 of 3392 unique animations). |
| 5, 10, 11 | Skip | – | no-op (not used) |
| 6, 7, 8 | Callback | type-specific | Calls the object's sprite callback. Type 7 (16 bytes) = collision box `u16 slot, u16 flags, s16 left, top, right, bottom`. Used by `Player_SpriteCallback`, `Decoration_SpriteCallback_Default`, `MapObject_SpriteCallback_Default` (441 uses in unique files, 1661 counting duplicates). Types 6 and 8 are not used. |
| 9 | ChangeAnimation | `u16 animIndex` | Jumps to another animation's start (not used). |

A typical frame is: `[1 FrameAssembly] [2 SpriteParts] [3 Palette] [7 hitbox]* 0 EndFrame`. The animation
ends with `4 EndAnimation`. VRAM and palette state carries over between frames. A frame without
SpriteParts reuses the previous tiles.

### 3.5 Frame assembly (frames block)
At `frames + frameOfs`: `u16 count, u16 flags, s16 left, top, right, bottom, s16 cx, s16 cy`,
then `count` pieces of 8 bytes: `u16 attr0, attr1, attr2, unused(0xFFFF)`. These are DS OAM attributes:
- attr0: Y (8 bit), bit 8 affine, bit 9 double size, bits 10-11 mode (3 = bitmap), bit 13 8bpp,
  bits 14-15 shape.
- attr1: X (9 bit), bits 12/13 h/v flip, bits 14-15 size. Shape and size map to
  w×h through `data_02072da0` and to tile count through `data_02072d60` (§bac_format.OBJ_SIZE/OBJ_TILES).
- attr2: bits 0-9 tile number (used only when `flags & 1`), bits 12-15 palette slot (relative).
  The priority bits are replaced by the sprite's own value.

Placement (`AnimatorSprite_Draw`): a piece is drawn at `spriteX - cx + X`, `spriteY - cy + Y`.
Flipping the whole sprite mirrors it about the origin (`x = cx - X - w`). So the origin is
(cx, cy) in assembly space, and `left/top/right/bottom` is the box relative to the origin.
The first piece gets the lowest OAM index, so it is drawn in front.
Tiles: if `flags & 1 == 0`, the pieces use VRAM in order. Each piece advances
`ceil(tiles / 2^shift) << (8bpp)` units of `32 << shift` bytes. If `flags & 1`, the char
number is `attr2 & 0x3FF` units from the sprite's VRAM base. Only `ac_gmk_needle` uses this mode,
and it is authored for the 64K boundary (shift 1) that the stage display sets
(`Stage_InitDisplay`: `Vram_SetBankForOBJ(..., 0x100010, ...)`). Menus use 128K (`Vram_SetBankConfig`).
All other frames render the same under shift 0, 1 and 2 (checked across all 5640 frames of the loose act/ and act2/ files).

3D formats (`Model_DrawSprite3D`) use the same record. Format 0/1 draw each 8×8 tile as a quad.
Format ≥3 makes each piece one linear texture of w×h in that texture format. `texAddr` advances by
`tiles << unitShift` bytes. Colour 0 is transparent (TEXIMAGE_PARAM bit 29), and the palette is the
sprite's palette base. Y points up in model space, but placement matches the 2D path.

Pixel data: 4bpp tiles are 32 bytes with the low nibble on the left. 8bpp tiles are 64 bytes. Index 0 is transparent.
Colours are BGR555.

### 3.6 Statistics (unique files)
412 BACs, 3392 animations, 5762 frames, 11291 OAM pieces. 48 frames have no pieces (blank frames). 222 frames belong to the
hitbox-only player BAC. No affine or bitmap pieces are used. 47 pieces are 8bpp, 2 are flipped, 11 frames
use explicit tile numbers, and 441 hitbox commands appear.

## 4. Dreamcast output (bac2dc.py)

```
python3 tools/dreamcast/sprites/bac2dc.py [--extract extract/eur] [--out build/dreamcast/sprites]
        [--only SUBSTR ...] [--no-preview] [--format 1555|4444] [--twiddle] [--mapping-shift 1] [-j N]
```
The full run takes about 13 s on 2 cores. Output for each unique BAC goes in `bac/<id>/`. `<id>` is the source path with `/` → `__`
and no `.bac` (for example `act__ac_ene_eggpawn` or `narc__z82_act_lz7__act__ac_ene_egg_hammer`).

- **Rendering**: `RenderState` replays the command stream like the engine does. SpriteParts
  rewrites VRAM from the sprite base (old bytes beyond the new data stay). Palette writes relative
  16-colour slots. Each frame is composed back to front with the current VRAM and palette,
  with flips and the per-piece palette slot. The result is cropped to its opaque pixels.
  Identical images (pixels and origin) are stored once.
- **Atlas** `atlas<N>.png` + `atlas<N>.argb1555`: power-of-two pages, at most 1024×1024 and
  aspect ratio at most 8:1, 1-pixel transparent gutter, MaxRects packing (best short side fit).
  The packer picks the page set with the least texels: one page, or a first page filled
  and the rest on further pages (at most 4 pages, an extra page must save 1024 texels).
  Pages are 65% full on average (median 65%, quartiles 51%/79%, gutters included); 261
  BACs use one page, 151 two to four. BACs read by single-page loaders (`ac_spst*`,
  `ac_ex_*`, `ac_itm_ring`: special stage, Extra boss) stay on one page. Against the old
  shelf packer: 34% fewer texels over all 412 BACs (18.4 M -> 12.1 M).
  ARGB1555 is lossless for DS colours (BGR555 plus 1-bit alpha). `--format 4444` is optional.
  The raw file is row-major (stride = width) by default. With `--twiddle` it is written in PVR twiddled order
  (y bits even, x bits odd, rectangular pages as square blocks, same as KOS `TWIDOUT`).
- **Paletted atlas** `atlas0.pt4` + `palettes.bin` (what the game loads; GRAPHICS.md section 3):
  the renderer also keeps the palette index of every pixel. An image whose opaque pixels
  read one 16-colour DS palette row keeps the DS indices (palette = that row); an image with
  at most 15 colours (8bpp sprites, mixed rows) gets its colours remapped into shared
  16-entry palettes; an image with more colours goes on a separate ARGB1555 page (only
  `ac_sf_logo` and `title_demo*`). `.pt4` is the page as PVR PAL4 twiddled (4 bpp, texel m
  of the twiddled order in byte m >> 1, low nibble first). `palettes.bin` ("DCPL" v1,
  layout in `dreamcast/game/engine/sprpal.h`): page formats, palettes (ARGB1555, entry 0
  transparent), the entries each palette's texels use, the palette and DS row of every image,
  and the OBJ row-0 palette after each animation's first frame (PaletteAnim). The converter
  checks that every PAL4 texel plus its image's palette equals the ARGB1555 pixel.
  `tools/dreamcast/sprites/atlas_pal4.py DIR...` writes the same two files for ARGB1555
  atlases made by other tools (UI backgrounds), with remapped palettes.
- **frames.json**: pages; images `{page, x, y, w, h, origin_x, origin_y}`, where the origin is
  the sprite position inside the image rectangle, so draw at `pos - origin`; animations
  `{index, format, loop_frame, loop_anim, next_anim, vram_units_1d, frames}`; frames
  `{image (-1 = nothing drawn), duration (60 Hz frames), frame_no, frame_count, bbox, pieces,
  events (hitboxes)}`.
- **frames.bin** ("DSPR" v1, little-endian, read by `dspr_check.c`):
  ```
  header 16 B : char magic[4]="DSPR"; u16 version=1, pages, images, anims, frames, reserved
  pages  4 B  : u16 w, h
  images 16 B : u8 page, pad; u16 x, y, w, h; s16 origin_x, origin_y; u16 pad
  anims  12 B : u16 first_frame, frame_count; s16 loop_frame (-1 none), next_anim (-1 none); u8 format, pad; u16 pad
  frames 4 B  : u16 image (0xFFFF = blank), u16 duration (60 Hz frames)
  ```
  Loop semantics: when the last frame's wait ends, a looping sprite (the game sets flag 4
  per object) jumps to `loop_frame`. A non-looping sprite stops on the last frame and
  reports "animation finished". The BAC itself always holds the loop point.
- **preview/<id>/aNNN_fMMM.png**: every frame, uncropped, origin at `(-left, -top)` of the union box.
  `sheet.png` shows one row per animation on a dark background, with a red cross at each frame's origin.
- **manifest.json**: each source gets `source` (path or `narc:member`), `sha1`, `bytes`,
  `converted_as`, `status`, `outputs`, and `output_sizes`. **report.json** has the counts, failure reasons,
  frame statistics, and the data-only IDs.

## 5. Validation (2026-11-22 run)

- Batch: 1018 sources (343 act + 61 act2 + 614 in NARCs), 412 unique. Results: 411 converted
  with images, 1 data-only (`ac_sonic` = `son.bac`), 0 failed, 0 archive errors.
  All 141 NARCs parse. Output size is 41.6 MB (36.8 MB raw ARGB1555, 2.0 MB PNG atlases, 2.6 MB JSON,
  0.15 MB DSPR), plus 28 MB of previews.
- `dspr_check` passes on all 412 `frames.bin` files. It checks size consistency, rects inside pages,
  image indices, and loop/next ranges. It also compiles with `sh-elf-gcc -ml -m4-single-only`.
- All 411 raw `.argb1555` pages were decoded and compared pixel by pixel with their PNGs: 0 mismatches.
  The twiddle index was checked to be a bijection for 8×8, 64×32, 32×128 and 512×512.
- Visual checks of `sheet.png` with origin crosses:
  - `ac_ene_eggpawn` (walk cycle, feet on origin);
  - `ac_ene_egg_hammer` (z31 palette and the z82 variant palette, 4 animations including the 12-frame
    hammer swing);
  - `ac_itm_ring` / `ac_itm_ring3d` (spin and sparkles, pal16 linear texture format);
  - `ac_ene_eggpawn3d`: the format-4 texture path gives the same pixels as the 2D OBJ version
    (11 of 14 frame PNGs identical; the other 3 differ only in the uncropped box size);
  - `ac_gmk_needle` (explicit-tile mode: rise and retract and the sideways variant are coherent
    only with the 64K boundary);
  - `title_demo` and `ac_sf_logo` (8bpp + 256-colour palettes: SONIC TEAM / Dimps / SEGA logos);
  - `mnu_time_tryagain_deu` (palette slots 5/6, palette swap between frames);
  - `mnu_psel` (quadrant pieces with origins at the shared corner);
  - `ac_fix_player_icon` and `ac_dec_sonic_neon` (Sonic and Blaze 2D art, correct colours).
- Compression: LZ10 is checked on the real data (141 NARCs, 10,460 tile streams). LZ10/LZ11/RLE/Huffman
  pass synthetic round-trip tests.

## 6. Known gaps

- Loop *policy* (whether an object sets flag 4) is chosen at runtime by the game code. It is not in the BAC.
  The port's object code must decide it, the same way the DS objects do.
- Runtime palette overrides are not baked in: the sprite palette base (`unk44`) and the
  `StageObj_AllocSpritePalette` sharing. Frames use the BAC's own palette; palette animation
  tasks select another palette (`palettes.bin` anim_pal) at draw time. Zone
  variants that ship their own BAC (for example z82 `egg_hammer`) are converted separately.
- Explicit-tile frames assume the 64K boundary (`--mapping-shift`). This affects only `ac_gmk_needle`.
- Affine/double-size pieces and bitmap OBJ (format 2) are handled in code, but the ROM does not use them.
  Texture formats 2, 6–9 raise "not supported". The ROM does not use them.
- Info-block fields after the four VRAM counts, and the hitbox `flags` u16, are not decoded. They are passed through raw in JSON.
- Paletted atlases are PAL4 only (no PAL8 pages): the few images with 16+ colours stay ARGB1555.
- Power-of-two pages still leave a third of the page empty on average (section 4).
  A max-rects packer or non-power-of-two strides would shrink VRAM use.
