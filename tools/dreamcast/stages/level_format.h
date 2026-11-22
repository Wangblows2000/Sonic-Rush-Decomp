/*
 * level_format.h - Dreamcast stage package written by tools/dreamcast/stages/stage2dc.py
 *
 * Three files per stage (build/dreamcast/stages/<id>/):
 *   level.bin    - RLVL: palette, 8x8 char texture page, block defs, the two
 *                  foreground layer maps, collision tables, split-line map,
 *                  object / ring / decoration placement.
 *   blocktex.bin - RBTX: the stage blocks pre-rendered (flips applied) as
 *                  512x512 PAL8 twiddled texture pages, 64 blocks per page.
 *   bg.bin       - RBGD: the zone background (.bbg) as one PAL8 twiddled
 *                  texture + its palette(s).  Absent for boss stages.
 *
 * All values little-endian (SH-4 native).  Every section offset is relative to
 * the start of its file and 32-byte aligned.  Format semantics are documented
 * in docs/dreamcast/formats/STAGES.md.
 *
 * The rlvl_* query functions at the end are the reference implementation of
 * the DS map-collision queries (MapCollision_GetTileHeight / _GetTileWidth /
 * _ProbeDistance / _ProbeDistanceLong) on top of this package; their results
 * equal the DS functions bit for bit (checked by test_level.c against the
 * Python transcription in dscoll.py, which reads the raw DS files).
 */
#ifndef RUSH_DC_LEVEL_FORMAT_H
#define RUSH_DC_LEVEL_FORMAT_H

#include <stdint.h>
#include <stddef.h>

/* ------------------------------------------------------------------ */
/* level.bin                                                           */
/* ------------------------------------------------------------------ */

#define RLVL_MAGIC   0x4C564C52u /* "RLVL" */
#define RLVL_VERSION 1

#define RLVL_BLOCK_PX     64   /* a block is 64x64 px = 8x8 chars          */
#define RLVL_CHAR_PX      8
#define RLVL_REGION_PX    256  /* object/ring/decor spawn regions          */
#define RLVL_CHARTEX_SIZE 256  /* char page: 256x256 PAL8, 32x32 chars    */

/* block entry / tile word (same bit layout as a DS BG screen entry) */
#define RLVL_TILE_CHAR(t) ((t) & 0x3FF)  /* char id == collision id       */
#define RLVL_TILE_HFLIP   0x400
#define RLVL_TILE_VFLIP   0x800

/* header.flags */
#define RLVL_FLAG_BOSS    0x01  /* act 3 arena (3D boss stage)           */
#define RLVL_FLAG_HAS_BG  0x02  /* bg.bin present                        */

enum {
    RLVL_SEC_PALETTE = 0,  /* uint16_t[256] ARGB1555; index 0 alpha 0     */
    RLVL_SEC_CHARTEX,      /* uint8_t[256*256] PAL8 twiddled, char c at
                              ((c%32)*8, (c/32)*8)                         */
    RLVL_SEC_BLOCKS,       /* uint16_t[num_blocks][64] tile words, row-major
                              8x8 per block                                */
    RLVL_SEC_LAYER_A,      /* uint16_t[height_blocks][width_blocks] block ids;
                              front layer (DS BG2, priority 0), collision
                              layer 0                                      */
    RLVL_SEC_LAYER_B,      /* same; back layer (DS BG3, priority 2),
                              collision layer 1 (player default)           */
    RLVL_SEC_COLL_HEIGHT,  /* int8_t[num_chars][8]: per column x=0..7,
                              decoded: 0 none, 8 full, 1..7 floor whose top
                              row is h, -1..-7 ceiling covering rows 0..-h-1 */
    RLVL_SEC_COLL_WIDTH,   /* int8_t[num_chars][8]: per row y=0..7, same code
                              horizontally: 1..7 solid from column w to 7,
                              -1..-7 solid columns 0..-w-1                  */
    RLVL_SEC_COLL_ANGLE,   /* uint8_t[num_chars]: surface angle, 256 = 360 deg */
    RLVL_SEC_COLL_ATTR,    /* uint8_t[num_chars]: bit0 top-solid only,
                              bits1-3 surface kind (1 unk, 2 grind rail,
                              3 grind rail + launch)                      */
    RLVL_SEC_SPLIT,        /* uint8_t[]: 2 bits per block (index y*w+x,
                              byte idx>>2, shift (idx&3)*2); dual-screen
                              split-line markers (Stage_GetSplitLineY)     */
    RLVL_SEC_OBJECTS,      /* rlvl_object_t[num_objects], region order     */
    RLVL_SEC_OBJ_REGION,   /* uint32_t[region_cols*region_rows + 1] first
                              index of each region (prefix sums)            */
    RLVL_SEC_RINGS,        /* rlvl_ring_t[num_rings]                       */
    RLVL_SEC_RING_REGION,  /* uint32_t[regions + 1]                        */
    RLVL_SEC_DECOR,        /* rlvl_decor_t[num_decor]                      */
    RLVL_SEC_DECOR_REGION, /* uint32_t[regions + 1]                        */
    RLVL_SEC_COUNT
};

typedef struct {
    uint32_t offset;
    uint32_t size;
} rlvl_section_t;

typedef struct {
    uint32_t magic;          /* RLVL_MAGIC                                */
    uint16_t version;        /* RLVL_VERSION                              */
    uint16_t header_size;    /* sizeof(rlvl_header_t)                     */
    uint8_t  stage;          /* 0..26 = zone*3 + act                      */
    uint8_t  zone;           /* 0..8 (0-based)                            */
    uint8_t  act;            /* 0..2                                      */
    uint8_t  flags;          /* RLVL_FLAG_*                               */
    uint16_t width_blocks;
    uint16_t height_blocks;
    uint16_t num_blocks;
    uint16_t num_chars;      /* 1024                                      */
    uint16_t region_cols;    /* ceil(width_px / 256)                      */
    uint16_t region_rows;    /* ceil(height_px / 256)                     */
    uint32_t num_objects;
    uint32_t num_rings;
    uint32_t num_decor;
    rlvl_section_t sec[RLVL_SEC_COUNT];
} rlvl_header_t;

/* Map object placement (.ev entry, absolute coordinates). */
typedef struct {
    uint16_t x, y;      /* pixels                                         */
    uint16_t type;      /* index into gMapObjectCreateTable (< 0xF2)      */
    uint16_t flags;     /* entry +4 (s16 on DS); 0x4000 = spawned also in
                           restricted spawn mode (StageEvent f & 8)        */
    int8_t   p0, p1;    /* entry +6, +7 (signed)                          */
    uint8_t  p2, p3;    /* entry +8, +9                                   */
} rlvl_object_t;        /* 12 bytes. DS entry +0xA (u16) is runtime state */

typedef struct {
    uint16_t x, y;      /* pixels (.rg entry)                             */
} rlvl_ring_t;

typedef struct {
    uint16_t x, y;      /* pixels                                         */
    uint16_t type;      /* index into gDecorationCreateTable (< 0xF2)     */
    uint16_t pad;
} rlvl_decor_t;         /* .dc entry                                      */

/* ------------------------------------------------------------------ */
/* blocktex.bin                                                        */
/* ------------------------------------------------------------------ */
#define RBTX_MAGIC 0x58544252u /* "RBTX" */
#define RBTX_FMT_PAL8_TWIDDLED 1

typedef struct {
    uint32_t magic;
    uint16_t version;          /* 1                                       */
    uint16_t format;           /* RBTX_FMT_PAL8_TWIDDLED                  */
    uint16_t page_size;        /* 512                                     */
    uint16_t blocks_per_row;   /* 8 -> 64 blocks per page                 */
    uint16_t num_pages;
    uint16_t num_blocks;
    uint32_t page_offset;      /* first page; page n at page_offset +
                                  n * page_size * page_size                */
    uint32_t reserved[3];
} rbtx_header_t;               /* 32 bytes; palette = level.bin PALETTE  */
/* block b -> page b/64, texel origin ((b%64)%8*64, (b%64)/8*64)          */

/* ------------------------------------------------------------------ */
/* bg.bin                                                              */
/* ------------------------------------------------------------------ */
#define RBGD_MAGIC 0x44474252u /* "RBGD" */

typedef struct {
    uint32_t magic;
    uint16_t version;          /* 1                                       */
    uint16_t src_type;         /* .bbg flags & 0xF (1: 4bpp chars, 16 x 16
                                  colour palettes)                         */
    uint16_t width_px;         /* image size, 256 or 512 (wraps)          */
    uint16_t height_px;        /* 512                                     */
    uint16_t tex_width;        /* texture size (power of two, == image)   */
    uint16_t tex_height;
    uint16_t num_palettes;     /* 1 + alternate palettes (z4pn/z5pn/z7pn) */
    uint16_t reserved0;
    uint32_t palette_offset;   /* uint16_t[num_palettes][256] ARGB1555;
                                  index p*16+0 alpha 0 except index 0
                                  (backdrop colour, alpha 1)              */
    uint32_t texture_offset;   /* uint8_t[tex_h][tex_w] PAL8 twiddled:
                                  texel = palbank*16 + colour             */
    uint32_t reserved[2];
} rbgd_header_t;               /* 36 bytes */

/* ------------------------------------------------------------------ */
/* Reference collision queries                                         */
/* ------------------------------------------------------------------ */

/* query flags (DS StageObj_GetMapProbeFlags):                          */
#define RLVL_Q_LAYER1     0x01  /* use layer B instead of layer A         */
#define RLVL_Q_ARENA      0x20  /* ProbeDistanceLong: distance to arena rect */
#define RLVL_Q_EDGE_SOLID 0x40  /* outside the map is solid (else clamp)  */
#define RLVL_Q_SKIP_TOP   0x80  /* ignore attr bit0 (top-solid) chars     */

/* probe directions */
#define RLVL_DIR_RIGHT 0
#define RLVL_DIR_LEFT  1
#define RLVL_DIR_DOWN  2
#define RLVL_DIR_UP    3

typedef struct {
    const rlvl_header_t *hdr;
    const uint16_t *blocks;
    const uint16_t *layer[2];
    const int8_t   *height;    /* [num_chars][8] */
    const int8_t   *width;     /* [num_chars][8] */
    const uint8_t  *angle;
    const uint8_t  *attr;
    int32_t left, top, right, bottom;           /* pixel bounds           */
    int32_t arena_left, arena_top, arena_right, arena_bottom; /* Q_ARENA  */
} rlvl_map_t;

static inline const void *rlvl_section(const void *file, int sec, uint32_t *size)
{
    const rlvl_header_t *h = (const rlvl_header_t *)file;
    if (size)
        *size = h->sec[sec].size;
    return (const uint8_t *)file + h->sec[sec].offset;
}

/* Bind a loaded level.bin. Returns 0 on success. */
static inline int rlvl_bind(rlvl_map_t *m, const void *file)
{
    const rlvl_header_t *h = (const rlvl_header_t *)file;
    if (h->magic != RLVL_MAGIC || h->version != RLVL_VERSION)
        return -1;
    m->hdr = h;
    m->blocks = (const uint16_t *)rlvl_section(file, RLVL_SEC_BLOCKS, NULL);
    m->layer[0] = (const uint16_t *)rlvl_section(file, RLVL_SEC_LAYER_A, NULL);
    m->layer[1] = (const uint16_t *)rlvl_section(file, RLVL_SEC_LAYER_B, NULL);
    m->height = (const int8_t *)rlvl_section(file, RLVL_SEC_COLL_HEIGHT, NULL);
    m->width = (const int8_t *)rlvl_section(file, RLVL_SEC_COLL_WIDTH, NULL);
    m->angle = (const uint8_t *)rlvl_section(file, RLVL_SEC_COLL_ANGLE, NULL);
    m->attr = (const uint8_t *)rlvl_section(file, RLVL_SEC_COLL_ATTR, NULL);
    m->left = 0;
    m->top = 0;
    m->right = (int32_t)h->width_blocks * RLVL_BLOCK_PX;
    m->bottom = (int32_t)h->height_blocks * RLVL_BLOCK_PX;
    m->arena_left = m->arena_top = m->arena_right = m->arena_bottom = 0;
    return 0;
}

/* MapCollision_GetTileAt: tile word at pixel (x,y) (x,y >= 0, in bounds). */
static inline uint16_t rlvl_tile_at(const rlvl_map_t *m, int32_t x, int32_t y, int layer)
{
    int32_t tx = x >> 3, ty = y >> 3;
    int32_t bx = tx / 8, by = ty / 8;
    uint16_t blk = m->layer[layer][m->hdr->width_blocks * by + bx];
    return m->blocks[blk * 64 + (tx - bx * 8) + (ty - by * 8) * 8];
}

/* Shared prologue of GetTileHeight/Width for RLVL_Q_EDGE_SOLID.
 * a = probed axis, b = other axis. Returns 1 and *out if decided. */
static inline int rlvl__edge(int32_t a, int32_t b, int32_t lo_a, int32_t hi_a,
                             int32_t lo_b, int32_t hi_b, int32_t *out)
{
    int32_t hi = hi_a - 1;
    int32_t ta = a & ~7;
    if (ta > hi || a < lo_a - 7) { *out = 8; return 1; }
    if (ta + 8 > hi) {
        int32_t h = (int8_t)(hi & 7);
        *out = h == 0 ? 8 : h;
        return 1;
    }
    if (ta < lo_a) {
        int8_t t = (lo_a & 7) ? (int8_t)(0x10 - (lo_a & 7)) : 8;
        t = (int8_t)(t | ~0xF);
        *out = t == -8 ? 8 : t;
        return 1;
    }
    if (b > hi_b - 1 || b < lo_b) { *out = 8; return 1; }
    return 0;
}

static inline int32_t rlvl__clamp(int32_t v, int32_t lo, int32_t hi)
{
    return v < lo ? lo : (v > hi - 1 ? hi - 1 : v);
}

/*
 * MapCollision_GetTileHeight(y, x, flags, angle, attr): vertical mask value of
 * the 8x8 tile containing (x,y), column x&7, after flips.
 *   0  = column empty            8 = column full
 *   1..7  = solid from row h down to row 7 (floor surface at row h)
 *   -1..-7 = solid rows 0..-h-1  (ceiling)
 * *angle / *attr are written only when the result is non-zero (and the
 * edge rules did not decide it).
 */
static inline int32_t rlvl_tile_height(const rlvl_map_t *m, int32_t y, int32_t x,
                                       uint32_t flags, uint8_t *angle, uint8_t *attr)
{
    uint16_t tile, id;
    int32_t xi, h;
    if (flags & RLVL_Q_EDGE_SOLID) {
        int32_t r;
        if (rlvl__edge(y, x, m->top, m->bottom, m->left, m->right, &r))
            return r;
    } else {
        x = rlvl__clamp(x, m->left, m->right);
        y = rlvl__clamp(y, m->top, m->bottom);
    }
    tile = rlvl_tile_at(m, x, y, flags & 1);
    id = RLVL_TILE_CHAR(tile);
    xi = x & 7;
    if (tile & RLVL_TILE_HFLIP)
        xi = 7 - xi;
    h = m->height[id * 8 + xi];
    if ((flags & RLVL_Q_SKIP_TOP) && (m->attr[id] & 1))
        h = 0;
    if ((tile & RLVL_TILE_VFLIP) && h != 8 && h != 0)
        h = h > 0 ? h - 8 : h + 8;
    if (h != 0) {
        if (angle) {
            uint8_t a = m->angle[id];
            if (tile & RLVL_TILE_HFLIP)
                a = (uint8_t)(-(int8_t)a);
            if (tile & RLVL_TILE_VFLIP)
                a = (uint8_t)(-0x80 - (int8_t)a);
            *angle = a;
        }
        if (attr)
            *attr = m->attr[id];
    }
    return h;
}

/* MapCollision_GetTileWidth(x, y, flags, angle, attr): horizontal mask value
 * for row y&7 (1..7 solid from column w to 7, -1..-7 solid columns 0..-w-1). */
static inline int32_t rlvl_tile_width(const rlvl_map_t *m, int32_t x, int32_t y,
                                      uint32_t flags, uint8_t *angle, uint8_t *attr)
{
    uint16_t tile, id;
    int32_t yi, h;
    if (flags & RLVL_Q_EDGE_SOLID) {
        int32_t r;
        if (rlvl__edge(x, y, m->left, m->right, m->top, m->bottom, &r))
            return r;
    } else {
        x = rlvl__clamp(x, m->left, m->right);
        y = rlvl__clamp(y, m->top, m->bottom);
    }
    tile = rlvl_tile_at(m, x, y, flags & 1);
    id = RLVL_TILE_CHAR(tile);
    yi = y & 7;
    if (tile & RLVL_TILE_VFLIP)
        yi = 7 - yi;
    h = m->width[id * 8 + yi];
    if ((flags & RLVL_Q_SKIP_TOP) && (m->attr[id] & 1))
        h = 0;
    if ((tile & RLVL_TILE_HFLIP) && h != 8 && h != 0)
        h = h > 0 ? h - 8 : h + 8;
    if (h != 0) {
        if (angle) {
            uint8_t a = m->angle[id];
            if (tile & RLVL_TILE_VFLIP)
                a = (uint8_t)(-0x80 - (int8_t)a);
            if (tile & RLVL_TILE_HFLIP)
                a = (uint8_t)(-(int8_t)a);
            *angle = a;
        }
        if (attr)
            *attr = m->attr[id];
    }
    return h;
}

/* distance from in-tile offset `sub` to the surface of mask value h */
static inline int32_t rlvl__calc(int32_t h, int32_t step, int32_t sub)
{
    if (h > 0)
        return step > 0 ? h - (sub + 1) : 8 - sub;
    return step > 0 ? -(sub + 1) : h + sub;
}

typedef int32_t (*rlvl__fn)(const rlvl_map_t *, int32_t, int32_t, uint32_t, uint8_t *, uint8_t *);

/*
 * MapCollision_ProbeDistance (single tile).  dir: RLVL_DIR_*.
 * Result: signed distance from (x,y) to the last free pixel before the surface
 * in direction dir; < 0 = embedded by that many pixels.  For an empty tile it
 * returns the distance to the tile boundary (8-sub forward / sub+1 back).
 */
static inline int32_t rlvl_probe_distance(const rlvl_map_t *m, int32_t x, int32_t y,
                                          uint32_t flags, uint32_t dir,
                                          uint8_t *angle, uint8_t *attr)
{
    uint8_t sa = 0, sb = 0;
    int32_t step = (dir & 1) ? -8 : 8;
    int32_t h, ofs;
    if (angle) sa = *angle;
    if (attr) sb = *attr;
    if (dir & 2) {
        h = rlvl_tile_height(m, y, x, flags, angle, attr);
        ofs = y & 7;
    } else {
        h = rlvl_tile_width(m, x, y, flags, angle, attr);
        ofs = x & 7;
    }
    if (h == 0) {
        if (angle) *angle = sa;
        if (attr) *attr = sb;
        return step > 0 ? 8 - ofs : ofs + 1;
    }
    if (h == 8)
        return step > 0 ? -(ofs + 1) : ofs - 8;
    return rlvl__calc(h, step, ofs);
}

/*
 * MapCollision_ProbeDistanceLong: the probe the player / objects use
 * (StageObj_ProbeCollision).  Looks up to 3 tiles (24 px) along dir when the
 * first tile is empty, and up to 2 tiles back when it is full.  Result range
 * about -24..+24 (callers truncate to s8); "nothing within reach" = > 16.
 * dir: RLVL_DIR_RIGHT/LEFT probe the width masks, DOWN/UP the height masks.
 */
static inline int32_t rlvl_probe_distance_long(const rlvl_map_t *m, int32_t x, int32_t y,
                                               uint32_t flags, uint32_t dir,
                                               uint8_t *angle, uint8_t *attr)
{
    uint8_t sa = 0, sb = 0;
    rlvl__fn fn;
    int32_t sub, off = 0, step = 8, h;

    if (flags & RLVL_Q_ARENA) {
        int32_t d = 0;
        switch (dir) {
        case 2: d = m->arena_bottom - y; break;
        case 3: d = y - m->arena_top; break;
        case 1: d = x - m->arena_left; break;
        case 0: d = m->arena_right - x; break;
        }
        return d < -0x1f ? -0x1f : (d > 0x1f ? 0x1f : d);
    }
    if (angle) sa = *angle;
    if (attr) sb = *attr;
    if (dir & 1)
        step = -8;
    if (dir & 2) {
        int32_t t;
        sub = (int8_t)(y & 7);
        t = x; x = y; y = t;           /* fn(a,b) = GetTileHeight(a=y, b=x) */
        fn = rlvl_tile_height;
    } else {
        fn = rlvl_tile_width;
        sub = (int8_t)(x & 7);
    }
    h = fn(m, x, y, flags, angle, attr);
    if (h == 0) {
        off += step;
        h = fn(m, x + off, y, flags, angle, attr);
        if (h == 0) {
            h = fn(m, x + (off + step), y, flags, angle, attr);
            if (h == 0) {
                if (angle) *angle = sa;
                if (attr) *attr = sb;
                return (step > 0 ? 8 - sub : sub + 1) + 16;
            }
            if (h == 8)
                return (step > 0 ? -(sub + 1) : sub - 8) + 16;
            return rlvl__calc(h, step, sub) + 16;
        }
        if (h == 8)
            return (step > 0 ? -(sub + 1) : sub - 8) + 8;
        return rlvl__calc(h, step, sub) + 8;
    } else if (h == 8) {
        if (angle) sa = *angle;
        if (attr) sb = *attr;
        off -= step;
        h = fn(m, x + off, y, flags, angle, attr);
        if (h == 8) {
            if (angle) sa = *angle;
            if (attr) sb = *attr;
            h = fn(m, x + (off - step), y, flags, angle, attr);
            if (h == 0) {
                if (angle) *angle = sa;
                if (attr) *attr = sb;
                if (step > 0)
                    sub = 8 - (sub + 1);
                return sub - 16;
            }
            if (h == 8)
                return (step > 0 ? -(sub + 1) : sub - 8) - 16;
            return rlvl__calc(h, step, sub) - 16;
        }
        if (h == 0) {
            if (angle) *angle = sa;
            if (attr) *attr = sb;
            if (step > 0)
                sub = 8 - (sub + 1);
            return sub - 8;
        }
        return rlvl__calc(h, step, sub) - 8;
    }
    return rlvl__calc(h, step, sub);
}

/* Split-line value (0..3) of block (bx,by); see Stage_GetSplitLineY. */
static inline int rlvl_split_at(const void *file, int bx, int by)
{
    const rlvl_header_t *h = (const rlvl_header_t *)file;
    const uint8_t *s = (const uint8_t *)rlvl_section(file, RLVL_SEC_SPLIT, NULL);
    uint32_t idx = (uint32_t)by * h->width_blocks + (uint32_t)bx;
    return (s[idx >> 2] >> ((idx & 3) * 2)) & 3;
}

#endif /* RUSH_DC_LEVEL_FORMAT_H */
