/* Paletted sprite atlases (portable part, header only: host tests use it too).
 *
 * tools/dreamcast/sprites/bac2dc.py writes, next to frames.bin ("DSPR") and the
 * ARGB1555 pages, for every converted BAC:
 *   atlas<N>.pt4   PVR PAL4 twiddled page (4 bpp; texel m of the twiddled order
 *                  in byte m >> 1, low nibble first = KOS PVR_TXRLOAD_4BPP order)
 *   palettes.bin   "DCPL" v1, little-endian:
 *     header 16 B  char magic[4] = "DCPL"; u16 version = 1, n_pages, n_images,
 *                  n_anims, n_pal, reserved
 *     u8  page_fmt[n_pages], padded to 4   (0 ARGB1555 page: atlas<N>.argb1555,
 *                                           1 PAL4 page: atlas<N>.pt4)
 *     u16 pal[n_pal][16]                   ARGB1555, entry 0 = 0 (transparent)
 *     u16 pal_mask[n_pal]                  entries the images' texels use
 *     u16 image_pal[n_images]              0xFFFF: image on an ARGB1555 page
 *     u16 anim_pal[n_anims]                OBJ row 0 after the animation's first
 *                                          frame (ObjDraw_LoadPaletteRow /
 *                                          PaletteAnim), 0xFFFF none
 *     u8  image_row[n_images]              DS palette row the texels index (raw
 *                                          DS indices), 0xFE remapped colours,
 *                                          0xFF ARGB1555
 *     padded to 4
 * A PAL4 texel plus its image's palette gives exactly the ARGB1555 atlas pixel
 * (checked by the converter for every texel). See docs/dreamcast/GRAPHICS.md. */
#ifndef SRDC_SPRPAL_H
#define SRDC_SPRPAL_H
#include <stdint.h>
#include <string.h>

#define SPRPAL_ROW_REMAP 0xFE
#define SPRPAL_ROW_DIRECT 0xFF
#define SPRPAL_NONE 0xFFFF

typedef struct SprPal {
    uint8_t *blob;                /* whole palettes.bin (owned by the caller) */
    uint16_t n_pages, n_images, n_anims, n_pal;
    const uint8_t *page_fmt;      /* 1 = PAL4 */
    const uint16_t *pal;          /* [n_pal][16] */
    const uint16_t *pal_mask;
    const uint16_t *image_pal;
    const uint16_t *anim_pal;
    const uint8_t *image_row;
} SprPal;

static inline uint16_t sprpal_rd16(const uint8_t *p) { return (uint16_t)(p[0] | p[1] << 8); }

/* Parse palettes.bin against the frames.bin counts. 0 = ok (p points into blob). */
static inline int sprpal_parse(SprPal *p, uint8_t *blob, uint32_t size, int n_pages, int n_images, int n_anims) {
    uint32_t need, o;
    memset(p, 0, sizeof(*p));
    if (!blob || size < 16 || memcmp(blob, "DCPL", 4) != 0 || sprpal_rd16(blob + 4) != 1)
        return -1;
    p->n_pages = sprpal_rd16(blob + 6);
    p->n_images = sprpal_rd16(blob + 8);
    p->n_anims = sprpal_rd16(blob + 10);
    p->n_pal = sprpal_rd16(blob + 12);
    if (p->n_pages != n_pages || p->n_images != n_images || p->n_anims != n_anims)
        return -2;
    o = 16 + ((p->n_pages + 3u) & ~3u);
    need = o + 34u * p->n_pal + 2u * p->n_images + 2u * p->n_anims + p->n_images;
    need = (need + 3u) & ~3u;
    if (need != size || ((uintptr_t)blob & 1))
        return -3;
    p->blob = blob;
    p->page_fmt = blob + 16;
    p->pal = (const uint16_t *)(blob + o);
    p->pal_mask = p->pal + 16u * p->n_pal;
    p->image_pal = p->pal_mask + p->n_pal;
    p->anim_pal = p->image_pal + p->n_images;
    p->image_row = (const uint8_t *)(p->anim_pal + p->n_anims);
    return 0;
}

/* PVR twiddled texel index of (x, y) in a w x h power-of-two texture (rectangular
 * textures: a row / column of square twiddled blocks; y in the even bits). */
static inline uint32_t sprpal_twiddle(int x, int y, int w, int h) {
    int s = w < h ? w : h, bit;
    uint32_t blk = (uint32_t)(w > h ? x / s : y / s), m = 0;
    x %= s;
    y %= s;
    for (bit = 0; (1 << bit) < s; bit++)
        m |= (uint32_t)((y >> bit) & 1) << (2 * bit) | (uint32_t)((x >> bit) & 1) << (2 * bit + 1);
    return blk * (uint32_t)s * (uint32_t)s + m;
}
static inline int sprpal_texel(const uint8_t *pt4, int w, int h, int x, int y) {
    uint32_t m = sprpal_twiddle(x, y, w, h);
    return (pt4[m >> 1] >> ((m & 1) * 4)) & 15;
}
static inline int sprpal_page_is_pal4(const SprPal *p, int page) {
    return p->blob && page >= 0 && page < p->n_pages && p->page_fmt[page] == 1;
}
#endif
