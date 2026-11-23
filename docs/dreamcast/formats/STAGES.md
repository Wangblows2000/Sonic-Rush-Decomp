# Stage data formats and the Dreamcast stage package

This file covers the 2D stage data of Sonic Rush (EUR): foreground maps,
collision, object placement and zone backgrounds. It also covers the converter
that turns them into the Dreamcast package the native game reads.

- Converter: `tools/dreamcast/stages/stage2dc.py`
- Binary layout and reference collision queries: `tools/dreamcast/stages/level_format.h`
- Host test: `tools/dreamcast/stages/test_level.c`
- DS-side Python transcription of the collision code: `tools/dreamcast/stages/dscoll.py`
- NARC / LZ77 / RL readers: `tools/dreamcast/stages/ndsfmt.py` (own code, written from the loader semantics)

No third-party code is used.

Sources read: `src/game/Stage/StageLoader_*`, `Stage_InitMapCollision`,
`Stage_LoadEventResources`, `StageEvent_Spawn*InBlock`, `Stage_GetSplitLineY`,
`Stage_InitDisplay`, `Camera_InitBoundsAndBuffers`, `Camera_DrawMapBg`,
`src/game/Stage/ObjectSystem/MapCollision_*`, `StageObj_ProbeCollision`,
`StageObj_GetMapProbeFlags`, `StageObj_CheckCeiling`,
`StageObj_CollideMapHorizontal`, `StageObj_ApplyCollisionAttr`,
`src/asm_fallback/StageObj_CollideMapVertical.c` (asm), `ZoneBg_*`,
`ZoneBg_Zone<N>_*`, and `Player_State_Grind`.

## 1. Stage slots

There are 27 slots: `stage = zone*3 + act` (0-based; see `docs/research/0014-zones.md`).
26 of them have map data:

| zone idx | act 0 | act 1 | act 2 (boss arena) |
|---|---|---|---|
| 0-6 | z11, z21 .. z71 | z12 .. z72 | z13 .. z73 |
| 7 | z81 | z82 | zf3 |
| 8 | z91 | z92 | none (Extra Zone: `extra_act_lz7.narc`, 3D, not converted here) |

Each slot has `narc/<id>_map.narc` (graphics and collision), `narc/<id>_eve.narc`
(placement and background) and `narc/<id>_act_lz7.narc` (sprites; not covered here).

## 2. Map archive `<id>_map.narc` (9 files, every file compressed)

Each NARC member has the BIOS compression header: byte0 & 0xF0 is the type
(0x10 LZ77, 0x20 Huffman, 0x30 RL; anything else means raw data after a 4-byte
header), and `u32 >> 8` is the decompressed size.
`StageLoader_UnpackMapArchive` decompresses the files into
`data_ov000_0236aabc[0..6]`, which is copied to `gCamera+0x124..0x13C`.

| idx | name | size | contents | used as |
|---|---|---|---|---|
| 0 | `.at` | 1024 | `u8 attr[1024]` per char id | collision attributes |
| 1 | `.bk` | 128 per block | blocks: `u16 tile[8][8]` (row-major) | graphics and collision |
| 2 | `.ca` | 4 + ceil(w*h/4) | `u16 w, h` + 2 bits per block | dual-screen split line |
| 3 | `.ch` | 65536 | 1024 chars, 8x8, **8 bpp** linear | BG chars at `0x06010000` (main) and `0x06210000` (sub) |
| 4 | `.df` | 8192 | `u8 mask[1024][8]` | height masks (see section 4) |
| 5 | `.di` | 1024 | `u8 angle[1024]` | surface angle per char |
| 6 | `.pl` | 512 | 256 x BGR555 | BG ext palette slots 2 and 3 (acts) or the standard BG palette (boss) |
| 7 | `_a.mp` | 4 + 2*w*h | `u16 w, h` (in blocks) + `u16 block[h][w]` | layer A |
| 8 | `_b.mp` | same | same | layer B |

Level building:
- **Char** (tile) is 8x8 px, 8 bpp, with one 256-colour palette. Colour 0 is transparent.
- **Block** (chunk) is 64x64 px = 8x8 tile words.
  - A tile word is `bits 0-9` char id, `bit 10` h-flip, `bit 11` v-flip.
  - Bits 12-15 are 0 in every stage.
  - The char id is **also the collision id**. Collision is attached to the 8x8 graphic char, so a flipped char has flipped collision.
- **Layers**: there are two foreground layers, A and B. They have the same size and share one block set.
  - Map size in px = `(w<<6, h<<6)`. Maximum sizes: 655x45 blocks (z42), 285x171 blocks (z61).
  - Block 0 is the empty block in every level.
  - On DS, layer A is drawn by BG2, an extended-affine 256-colour BG with `BG2CNT 0x8010` and priority 0, in front of everything.
  - Layer B is drawn by BG3 (`0x8410`), priority 2, behind the 3D layer BG0 (priority 1). The 3D layer holds the player and other models.
  - `Camera_DrawMapBg` copies the visible blocks of `map0` into BG2 and of `map1` into BG3.
  - Draw order, back to front: zone BG (BG1, priority 3), layer B, 3D models / player, layer A. Sprite priorities vary per object.
- **Collision layers** have the same maps:
  - Collision layer 0 = map A and collision layer 1 = map B (`MapCollision_GetTileAt` uses `layers[flags & 1]`).
  - The player starts on layer 1 (`Player_Create`: `unk54 |= 1`). Path-switch objects toggle bit 0 of `obj+0x54`.
  - Layer A therefore doubles as the "front path": loop fronts, alternate routes, and pillars drawn in front.

`.ca` split line: `Stage_GetSplitLineY(x)` scans the block column `x>>14` from the top (x is 8.8 fixed point).
- The first value 1 returns that block's y.
- Otherwise, a run that starts with 2 or 3 returns the midpoint between where the run starts and the first 1 after it.
- The result is used for dual-screen camera placement. The package keeps the packed form.

Boss arenas (act 2) use the same files. `StageLoader_LoadBossMapGraphics`
loads the palette as the standard BG palette. The maps are tiny (5x14 .. 29x9 blocks): the arena floor and walls for collision.

## 3. Event archive `<id>_eve.narc`

`Stage_LoadEventResources` loads index 1 into `gStageEventFiles[0]` (objects), index 2 into `[1]` (rings) and index 0 into `[2]` (decorations). Index 3 goes to `ZoneBg_Load`.

| idx | name | entry | contents |
|---|---|---|---|
| 0 | `.dc` | 4 bytes | decorations: `u8 x, u8 y, u16 type` (type indexes `gDecorationCreateTable`) |
| 1 | `.ev` | 12 bytes | map objects: `u8 x, u8 y, u16 type, u16 flags, s8 p0, s8 p1, u8 p2, u8 p3, u16 runtime` |
| 2 | `.rg` | 2 bytes | rings: `u8 x, u8 y` |
| 3.. | `.bbg` | | zone background, then optional palette-only BBGs (`z4pn01-16`, `z5pn01-02`, `z7pn01-02`) used by the ZoneFx palette effects |

All three placement files share one layout:
- `u16 cols, u16 rows`, where `cols = ((map_w_px-1)>>8)+1`. Regions are 256x256 px.
- Then `u32 offset[cols*rows]`, measured from the start of the file.
- Each region holds `u16 count` followed by `count` entries.
- Absolute position = `(region%cols)*256 + x`, `(region/cols)*256 + y`.

Entry fields and spawn rules:
- `x == 0xFF` marks a consumed entry (set at runtime; none are set in the ROM).
- Types `>= 0xF2` are skipped, and so are NULL create functions.
- The `.ev` entry is passed to the create function. `MapObject_Spawn` shows the field types (+6 s8, +7 s8, +8 u8, +9 u8, +A u16 = 0).
- `flags & 0x4000` marks objects that are still spawned when the spawner runs with `f & 8`.
- The per-type spawn margin tables are `data_ov000_023686e0` (objects) and `data_ov000_023685ec` (decorations).

The converter reads both create tables from `ov000.bin` and names every type through `symbols.txt`. All types used in the 26 stages resolve (`unknown_object_types` is empty everywhere).

## 4. Collision (the part the native physics needs)

### Storage

The storage is per char id (1024 per stage):
- `.df` byte: **high nibble = vertical mask** for column x, **low nibble = horizontal mask** for row y.
- Both nibbles are signed 4-bit values. The DS code decodes them as follows; the package stores the decoded values:

| value | vertical (height, per column) | horizontal (width, per row) |
|---|---|---|
| 0 | column empty | row empty |
| 8 (stored as -8 / 0x8) | column full | row full |
| 1..7 | solid from row h to row 7 (a floor whose top pixel is row h) | solid from column w to 7 (wall on the right) |
| -1..-7 | solid rows 0..(-h-1) (ceiling) | solid columns 0..(-w-1) |

Flips are applied at query time, exactly as in `MapCollision_GetTileHeight`/`Width`:
- **h-flip**: the column index mirrors (`7-x`), and the horizontal value maps `h>0 → h-8`, `h<0 → h+8`. The angle becomes `-a`.
- **v-flip**: the row index mirrors, and the vertical value maps `h>0 → h-8`, `h<0 → h+8`. The angle becomes `0x80 - a`.
- 0 and 8 are unchanged.

The `angle` byte uses 256 units per turn:
- 0 is a flat floor.
- Values grow clockwise on screen (y down). For example, `0xF2` is a floor rising to the right.
- `0x40` is a wall with the solid on the left, `0x80` is a ceiling, and `0xC0` is a wall with the solid on the right.
- The ground mode / quadrant is `((angle+0x20)&0xC0)>>6`. The floor probe direction is then down, left, up, right for quadrants 0..3.

The `attr` byte:
- bit 0: top-solid only (one-way platform). These chars are ignored when the query has flag 0x80.
- `attr>>1` (`StageObj_ApplyCollisionAttr` sets bits in `obj+0x5E`):
  - 1 sets bit 0x01 (meaning unknown).
  - 2 marks a **grind rail** (0x08).
  - 3 marks a grind rail plus launch at its end (0x08|0x10, `Player_State_Grind`).
- Leaf Storm rails use `attr = 5`, which means top-solid + rail.

Map bounds are `left=0, top=0, right=w*64, bottom=h*64` (`Stage_InitMapCollision`). They live in `gStageMapCollision` and gimmicks can change them at runtime.

### Query API (reference: `level_format.h`, bit-exact)

Query `flags`:
- bit 0: layer (0 = A, 1 = B).
- 0x80: ignore top-solid chars.
- 0x40: outside the map counts as solid (otherwise coordinates are clamped into the map).
- 0x20: (`ProbeDistanceLong` only) return the distance to the arena rect `data_022c4dcc`, clamped to ±31.

Objects build their flags with `StageObj_GetMapProbeFlags`:
- They add 0x80 unless `obj->flags & 0x100000`.
- The layer comes from `obj+0x54 & 1`.
- They add 0x40 if `obj->flags & 0x80000`.

`StageObj_CollideMapVertical` (asm) controls `0x100000` for floor probes:
- Before probing, it probes once with 0x80 cleared. If the result is >= 0 (not embedded in a platform), it sets 0x100000, so platforms count.
- Otherwise it clears the flag, so a platform is passed through from below.
- Wall probes (`StageObj_CollideMapHorizontal`) always add 0x80.

| function | DS original | meaning |
|---|---|---|
| `rlvl_tile_at(m,x,y,layer)` | `MapCollision_GetTileAt` | tile word under pixel (x,y) |
| `rlvl_tile_height(m,y,x,flags,&ang,&attr)` | `MapCollision_GetTileHeight` (note the arguments are y, x) | vertical mask value of column x&7 after flips and the 0x80/0x40 rules. Writes ang/attr only when the result != 0 |
| `rlvl_tile_width(m,x,y,flags,&ang,&attr)` | `MapCollision_GetTileWidth` | horizontal mask value of row y&7 |
| `rlvl_probe_distance(m,x,y,flags,dir,..)` | `MapCollision_ProbeDistance` | single-tile distance |
| `rlvl_probe_distance_long(m,x,y,flags,dir,..)` | `MapCollision_ProbeDistanceLong` | the probe that players and objects use |

Directions (`dir`): 0 right, 1 left (both use the width masks), 2 down, 3 up (both use the height masks).

**Probe result semantics.** `sub` is the in-tile offset of the probe point along `dir`. The result is the signed number of pixels the probe point can move along `dir` and stay free; the last free pixel is the one before the surface.
- `> 0`: free space ahead. A floor sensor moves down by this amount to touch the ground.
- `0`: exactly on the surface.
- `< 0`: embedded by that many pixels. Moving by this (negative) amount pushes the point back out.

`ProbeDistanceLong` searches up to 3 tiles forward when the first tile is empty, then adds +8 / +16. When the first tile is full it searches up to 2 tiles back and adds -8 / -16. The range is about -24..+24; "nothing within 24 px" gives 17..24. Callers cast the result to s8 and take the minimum with `ObjCollisionMap_ProbeForObj` (moving solid objects; that is runtime data, not stage data).

Exact distances per mask value (step = +8 forward, -8 backward, sub = in-tile offset):
- empty tile: `8-sub` forward, `sub+1` backward.
- full tile: `-(sub+1)` forward, `sub-8` backward.
- `h>0`: `h-(sub+1)` forward, `8-sub` backward.
- `h<0`: `-(sub+1)` forward, `h+sub` backward.

Sensor layout:
- Sensor offsets come from the object's hitbox bytes `obj+0x64..0x67`.
- Each check uses two sensors, and the smaller distance wins (`StageObj_CheckCeiling` / `CollideMapHorizontal`).
- The floor-landing and angle-smoothing logic (`StageObj_CollideMapVertical`, 0xD60 bytes) exists only as asm (`src/asm_fallback/StageObj_CollideMapVertical.c`). The native player must port it from that listing on top of the queries above.

## 5. Zone background `.bbg`

Header, all fields little-endian:

| offset | field |
|---|---|
| 0 | `"BBG\0"` |
| 4 | u32 total size |
| 8 | u32 chr offset |
| 0xC | u32 screen offset |
| 0x10 | u32 palette offset |
| 0x14 | u32 flags: bits 0-3 type, 0x1000 screen-transfer flag |
| 0x18 | u16 width (tiles) |
| 0x1A | u16 height (tiles) |
| 0x1C | u32 (0x10000 for stage BGs; meaning unknown) |

- Every sub-resource starts with its own 4-byte compression header. The chars are LZ77; the screen and palette are raw.
- The screen is linear `u16[h][w]` DS text-BG entries, with bits 12-15 as the palette bank. Stage BGs are 64x64 (512x512 px) or 32x64 (256x512 px).
- Type 1 (all stage BGs): **4 bpp** chars, with the 512-byte palette loaded as the standard BG palette (16 banks).
  - `ZoneBg_SetupBgControl` switches BG1 to 16 colours (`0xE808`, priority 3, wraps).
  - Colour 0 of the standard palette is the backdrop (sky).
- Type 2 (menus only): 8 bpp with an ext palette.
- `ZoneBg_LoadBgResource` loads the chars to `0x06008000` (BG1) and the screen at 64x64.

Parallax is code, per zone (`ZoneBg_Zone<N>_Update` / `_UpdateParallax`).

| zone | bg x | bg y | line scroll |
|---|---|---|---|
| 1 Leaf Storm | camX/14 | camY*48/(mapH-192) | no |
| 2 Water Palace | camX>>4 | wave code | yes (`ZoneBg_Zone2_BuildLineScroll`) |
| 3 Mirage Road | camX/12 | ((camY/12)+0x110)%0x118+0xE8 | yes |
| 4 Night Carnival | camX/18 | camY*48/(mapH-192) | no; palette animation `ZoneFx_Zone4` + z4pn palettes |
| 5 Huge Crisis | camX/14 | ((camY/14)+0x110)&0xFF + 0x100 | yes |
| 6 Altitude Limit | drift `x += (camX-prev)/60` | code | no |
| 7 Dead Line | camX/14 | as zone 5 | yes |

In dual-screen mode the second screen's y gets `+0x110` (0x11000 in 8.8 fixed point). z81/z82/z91/z92 use the zone 1/3/4/5 background code.

## 6. Dreamcast package (`build/dreamcast/stages/<id>/`)

Read the exact layout from `level_format.h`. All data is little-endian and sections are 32-byte aligned.

- **level.bin** (`RLVL` v1, 85-371 KB):
  - 164-byte header with the section table.
  - Palette: 256 x ARGB1555, index 0 transparent.
  - Char page: 256x256 PAL8, twiddled, char c at ((c%32)*8, (c/32)*8).
  - Block tile words, unchanged.
  - Layer A and B maps (`u16` block ids).
  - Decoded collision: height s8[1024][8], width s8[1024][8], angle u8[1024], attr u8[1024].
  - Split-line bits.
  - Objects: 12 bytes each, absolute u16 px.
  - Rings: 4 bytes each.
  - Decorations: 8 bytes each.
  - For each of objects, rings and decorations, a region prefix-sum table `u32[cols*rows+1]`, so the native spawner can keep the DS 256-px region streaming.
- **blocktex.bin** (`RBTX`):
  - Every block pre-rendered with its flips applied, as 512x512 PAL8 twiddled pages of 64 blocks each. The palette is the level.bin palette.
  - Size: 1-10 pages, 0.26-2.6 MB.
  - These pages are the cheap render path: one quad per visible 64x64 block per layer, about 100 quads per layer at 640x480. The char page is the fallback for per-tile rendering.
  - Load with `PVR_TXRFMT_PAL8BPP | PVR_TXRFMT_TWIDDLED`.
- **bg.bin** (`RBGD`):
  - The background with the 4 bpp banks flattened into one PAL8 image: texel = bank*16 + colour.
  - Size 512x512 or 256x512, twiddled. Non-square images use the KOS rule: squares of the smaller side, placed in sequence.
  - Palettes: the main palette, then the alternate palette-only BBGs.
  - In every bank, index `p*16` is transparent except index 0, which is the opaque backdrop.
  - Omitted for boss arenas (their eve narc has no BBG).
- **manifest.json**:
  - Size, counts, and object / decoration type histograms with create-function names.
  - Background source, scroll hint and alternate palettes.
  - SHA-1 of the sources and of level.bin.
  - Round-trip verify results.
- `build/dreamcast/stages/manifest.json`: the batch summary, including the failed and no-data slots.
- `preview/<id>_fg.png` (A over B on the backdrop), `_layer_a.png`, `_layer_b.png`, `_fg_quarter.png`, `_chars.png`, `_bg.png`, and the collision overlays `<id>_collision_X_Y.png`.

## 7. Tool usage

```sh
python3 tools/dreamcast/stages/stage2dc.py --all                        # 26 stages + previews (~40 s)
python3 tools/dreamcast/stages/stage2dc.py --stage z11 --testvec 20000 \
        --overlay z11:5300,1550,700,500                                  # one stage, test vectors, overlay
gcc -O2 -Wall -o /tmp/stages/test_level tools/dreamcast/stages/test_level.c
/tmp/stages/test_level build/dreamcast/stages/z11                        # C queries vs DS transcription
```

Overlay colours: red = layer B solid, blue = layer A only, yellow = top-solid chars on B, white line = top edge of the B surface. Each overlay also checks 8000 random pixels of the vectorised mask against the transcribed `GetTileHeight`.

## 9. Gaps / unknowns

- `attr>>1 == 1` (sets `obj+0x5E` bit 0): meaning unknown.
- `.bbg` +0x1C field: meaning unknown.
- Menu `bg/`/`bg2/` BBGs with char offsets (+0x1C = 208/224) belong to the UI pipeline and are not converted here.
- Line-scroll and palette effects (ZoneFx, z4pn/z5pn/z7pn) are code. Only the data is packaged.
- The floor-collision state machine is asm-only (see section 4).
- `ObjCollisionMap_*` (solid moving objects) is runtime-only and is not part of the stage files.
- The z81/z82/z91/z92 arena variants have 0 rings in their `.rg`.
