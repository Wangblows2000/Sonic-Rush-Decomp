/*
 * Zone backgrounds and zone effects: ZoneBg_* / ZoneFx_* translated (zonefx.h).
 *
 * The DS runs these per DS screen (gCamera[0] / gCamera[1]); the DC shows one window,
 * so every per-screen function runs once on a virtual screen = the DC window
 * (camera_get_view, with gCamera[0]'s flags, flags34, limit and water level). The
 * BG1 scroll registers become the 192-row line table drawn by level_draw_bg_lines;
 * BG / map palettes (PaletteTransfer_QueueCopy targets 0x5000000 = BG std palette,
 * ext slots 0x4000 / 0x6000 = map tiles) become level_set_palette / _bg_palette.
 * Colours are kept in the DC format (ARGB1555, red in bits 10-14); the DS formulas
 * that work on BGR555 channels are applied to the matching DC channel.
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zonefx.h"
#include "../camera/camera.h"
#include "../objects/objects.h"
#ifdef _arch_dreamcast
#include <kos.h>
#include "../engine/level.h"
#include "../engine/render.h"
#include "../objects/obj_internal.h"
void zfx_water_sprite_free(void);
#endif
/* the object sprites' underwater tint (objects/obj_draw.c, KOS); weak: absent on the host */
void objects_set_water_tint(float y, uint32_t argb) __attribute__((weak));

/* the tint as a vertex colour (MODULATE) where no second palette bank was free and for the
 * object sprites: red * 0.6 and green * 0.97 for ZoneFx_Zone2_TintPalette's r - 10, g - 1 */
#define ZFX_TINT_ARGB 0xff99f7ffu
#define CLAMP(v, lo, hi) ((v) < (lo) ? (lo) : ((v) > (hi) ? (hi) : (v)))

/* ------------------------------------------------------------ zonefx.bin */
typedef struct ZfxData {
    char magic[4];
    u16 version, size;
    s8 shake_3c[4], shake_40[12], shake_4c[22], pad0[2];
    u8 z4_anim[24][8], z5_anim[2][8], z7_anim[2][8];
    u8 z5_rot[8], z7_rot[16], z5_add[4];
    u8 z4_flash[60];
    u16 z3b_light0[3], z3b_light1[3];
} ZfxData;
static ZfxData s_data;
static int s_data_ok;

int zonefx_load_data_mem(const void *data, uint32_t size) {
    if (size < sizeof(ZfxData) || memcmp(data, "RZFX", 4) != 0)
        return -1;
    memcpy(&s_data, data, sizeof(s_data));
    if (s_data.version != 1)
        return -1;
    s_data_ok = 1;
    camera_set_shake_tables(s_data.shake_3c, s_data.shake_40, s_data.shake_4c);
    return 0;
}
int zonefx_load_data(const char *path) {
    FILE *f = fopen(path, "rb");
    u8 buf[sizeof(ZfxData)];
    size_t n;
    if (!f)
        return -1;
    n = fread(buf, 1, sizeof(buf), f);
    fclose(f);
    return zonefx_load_data_mem(buf, (uint32_t)n);
}

/* ------------------------------------------------------------ state */
/* ZoneFx_InitPaletteAnim entry (0xC bytes on the DS) */
typedef struct ZfxAnim {
    const u16 *src;        /* +0 alternate palette (bg.bin palette file - 3) */
    const u8 *anim;        /* +4 table entry */
    u8 buf;                /* +8 0 = map tiles (ext palette), 1 = BG (std palette) */
    s8 frame;              /* +9 */
    u16 timer;             /* +A */
} ZfxAnim;

static struct Level *s_lv;
static u8 s_zone, s_act, s_on;
static int s_bg_kind;            /* the ZoneBg_<n> update of this stage (0 none) */
static int s_fx_kind;            /* the ZoneFx_<n> of this stage (0 none) */
static u16 s_map_h;
/* palettes: [0] map tiles, [1] BG as animated (ZoneFx buf[0] / buf[1]); shown = with
 * BLDY / the alarm; tinted (Water Palace) */
static u16 s_base[2][256], s_buf[2][256], s_show[4][256];
__attribute__((unused)) static u16 s_sent[4][256];
static u8 s_sent_ok[4];
static u32 s_pal_writes;
static ZfxAnim s_ent[24];
static int s_n_ent;
/* ZoneFx_Zone4 BLDY: timers @0, lengths @4, modes @8, BLDY per screen */
static u16 s_bt[2], s_blen[2];
static s32 s_bmode[2];
static u16 s_bldy[2];
static int s_bldy_on;            /* gCamera[2].flags & 0x100000 */
/* ZoneFx_Zone5 / 7 counters, Zone5 alarm (w+0x200 add, w+0x202 sub counter) */
static s16 s_rcnt[2];
static u8 s_alarm_add[2], s_alarm_sub[2];
/* Zone2: CyclePalette counter (w+0x8b8), water row */
static s16 s_cycle;
static int s_water_row = 192;
/* ZoneBg: work (frame counter w+0, band positions, band rows), the window screen */
static u32 s_t;
static s32 s_ax, s_ay, s_bx, s_by, s_cx, s_cy;
static u8 s_n0, s_n1;
static s32 s_bg_x, s_bg_y;       /* gCamera @0x2C @0x30 of the window */
static s32 s_prev_x;
static int s_have_prev;
static ZfxLine s_lines[192];
/* Battle Play: the second player's camera updates only its BG lines / water row (the frame
 * counter, palette animations and BLDY run once per frame, with the first camera) */
static int s_view_only;
/* Zone3 boss light flash counter */
static u16 s_z3b;

static s32 fx_div(s32 a, s32 b) { return b ? (s32)(((s64)a << 12) / b) : 0; }
static s16 SIN(u16 a) { return dcp_sin(a); }
static s16 COS(u16 a) { return dcp_cos(a); }

/* DC ARGB1555 channels: r 10..14, g 5..9, b 0..4 */
#define R_(c) (((c) >> 10) & 0x1f)
#define G_(c) (((c) >> 5) & 0x1f)
#define B_(c) ((c) & 0x1f)
#define RGB_(a, r, g, b) ((u16)(((a) & 0x8000) | ((r) << 10) | ((g) << 5) | (b)))

/* the window as a DS screen (gCamera[0] fields + the window position) */
typedef struct ZScreen {
    s32 x, y, px;      /* @04 @08 @0C */
    s16 scale;         /* @20 */
    u32 flags, flags34;
    u16 limit, water;
} ZScreen;
static void screen_of(ZScreen *s) {
    s32 x, y;
    camera_get_view(&x, &y);
    s->x = x;
    s->y = y;
    s->px = s_have_prev ? s_prev_x : x;
    s->scale = g_obj_camera[0].h ? g_obj_camera[0].h : 0x100;
    s->flags = g_obj_camera[0].flags;
    s->flags34 = g_obj_camera[0].flags34;
    s->limit = g_obj_camera[0].limit;
    s->water = g_obj_camera[0].water;
}
/* IsMode_*: the zone 8 / 9 acts that reuse a BG without its horizontal scroll */
static int is_mode_xfix(void) { return s_zone >= 7 && s_zone < 9 && s_act < 2; }

/* ================================================================ ZoneBg */
/* ZoneBg_Zone1_Update / ZoneBg_Zone4_Update */
static void bg_parallax(const ZScreen *p, int div) {
    if (p->flags34 & 1)
        return;
    s_bg_x = p->x / div;
    s_bg_y = fx_div(p->y * 0x30, (s_map_h - 0xc0) << 8) >> 4;
}
/* ZoneBg_Zone6_Update: the clouds drift by 1/60 of the camera motion */
static void bg_zone6(const ZScreen *p) {
    if (p->flags34 & 1)
        return;
    s_bg_x += (p->x - p->px) / 60;
    s_bg_y = 0x3000;
}
/* ZoneBg_Zone2_UpdateScreenScroll (Water Palace: sky above, the underwater part below
 * the water line, with the wave) */
static void bg_zone2_scroll(const ZScreen *p) {
    s32 h, v, ya;
    if (p->flags34 & 1)
        return;
    ya = fx_div(p->y * 0x22, (s_map_h - 0xC0) << 8) >> 4;
    s_bg_x = 0;
    s_bg_y = ya;
    s_ax = 0;
    s_ay = ya;
    if (p->flags & 0x1000000) {
        h = fx_div(((s32)p->water << 8) - p->y, p->scale) >> 4;
        h = (s32)((u32)h << 8) >> 16;
        if (h <= 0) {
            s_bx = (p->x >> 4) & 0xFFFF;
            s_by = 0x11A00;
            s_n0 = 0;
            s_n1 = 0;
        } else if (h < 0xFC) {
            s_bx = (p->x >> 4) & 0xFFFF;
            s_by = (0x11A - h) << 8;
            v = h - 0x3C;
            s_n0 = (u8)CLAMP(v, 0, 0xC0);
            s_n1 = (u8)CLAMP(h, 0, 0xC0);
        } else {
            s_bx = s_by = 0;
            s_n0 = s_n1 = 0xC0;
        }
    } else {
        s_bx = s_by = 0;
        s_n0 = s_n1 = 0xC0;
    }
}
/* ZoneBg_Zone2_BuildLineScroll */
static void bg_zone2_lines(void) {
    u16 i, x, y, ang, spd;
    s16 t;
    s32 c;
    x = (u16)(s_ax >> 8);
    y = (u16)(s_ay >> 8);
    for (i = 0; i < s_n0; i++) {
        s_lines[i].x = x;
        s_lines[i].y = y;
    }
    /* the band just above the water line: the zeroed wave (angle 0) */
    x = (u16)(s_bx >> 8);
    y = (u16)(s_by >> 8);
    for (; i < s_n1; i++) {
        s_lines[i].x = (u16)(x + (u16)(((0x500 - 0) * SIN(0)) >> 20));
        s_lines[i].y = (u16)(y + ((COS(0) << 5) >> 16));
    }
    /* ZoneBg_Zone2_InitWave(&wave, 0, frame): angle frame << 9, speed 0x2000 */
    c = (s32)s_t;
    ang = (u16)(c << 9);
    spd = 0x2000;
    t = 0;
    for (; i < 0xc0; i++) {
        ang -= spd;
        if (t % 8 == 0)
            spd -= spd / 10;
        s_lines[i].x = (u16)(x + (u16)(((0x500 - t) * SIN(ang)) >> 20));
        /* (wave.unk4 << ((wave.unk4 << 9) + 10)): an ARM register shift uses the low byte,
         * which is 10 */
        s_lines[i].y = (u16)(y + ((COS((u16)((t << 11) - (c << 10))) << 5) >> 16));
        t++;
    }
}
/* ZoneBg_Zone3_UpdateScreenScroll (Mirage Road: heat haze, the dunes, the quicksand) */
static void bg_zone3_scroll(const ZScreen *p) {
    s32 lim, h, v, t;
    lim = (p->flags34 & 0x10) ? p->limit : 0x7fff;
    h = (s16)((u32)(fx_div((lim << 8) - p->y, p->scale) >> 4) >> 8);
    if (p->flags34 & 1)
        return;
    s_bg_x = is_mode_xfix() ? 0 : p->x / 28;
    s_bg_y = (fx_div(p->y * 5, (s_map_h - 0xc0) << 8) >> 4) + 0x900;
    s_ax = s_bg_x;
    s_ay = s_bg_y;
    s_bx = is_mode_xfix() ? 0 : (p->x / 28) & 0x1ffff;
    s_by = (fx_div(p->y << 4, (s_map_h - 0xc0) << 8) >> 4) + 0x1000;
    s_n0 = (u8)(0xc1 - (s_by >> 8));
    if (h + (s_by >> 8) < 0xc1) {
        s_n0 = (u8)(h > 0 ? h : 0);
        s_by = (0xc1 - h) << 8;
        t = h + 0x27;
        if (t < 0xc0) {
            if (t <= 0)
                t = 0;
            s_n1 = (u8)t;
            s_cx = (p->x / 12) & 0x1ffff;
            s_cy = (((p->y / 12) >> 8) % 0x118 + 0xe8) << 8;
        } else {
            s_n1 = 0xc0;
        }
    } else {
        v = 0xc1 - (s_by >> 8);
        s_n0 = (u8)CLAMP(v, 0, 0xc0);
        s_n1 = 0xc0;
    }
}
/* ZoneBg_Zone3_BuildLineScroll */
static void bg_zone3_lines(void) {
    u32 t = s_t;
    u16 x, y;
    u8 n, i;
    x = (u16)(s_ax >> 8);
    y = (u16)(s_ay >> 8);
    for (i = 0; i < s_n0; i++) {
        s32 c = (s32)(i << 16) / 24;
        s_lines[i].x = (u16)(x + (SIN((u16)((u32)(t << 16) / 96 - c)) >> 11));
        s_lines[i].y = (u16)(y + (SIN((u16)((u32)(t << 16) / 144 - c)) >> 12));
    }
    x = (u16)(s_bx >> 8);
    y = (u16)(s_by >> 8);
    for (; i < s_n1; i++) {
        s_lines[i].x = x;
        s_lines[i].y = y;
    }
    y = (u16)(s_cy >> 8);
    x = (u16)(s_cx >> 8);
    n = (0x200 - y < 0xC0) ? (u8)(0x200 - y) : 0xC0;
    for (; i < n; i++) {
        s_lines[i].x = x;
        s_lines[i].y = y;
    }
    for (; i < 0xC0; i++) {
        s_lines[i].x = x;
        s_lines[i].y = (u16)(y - 0x118);
    }
}
/* ZoneBg_Zone5_UpdateScreenScroll / ZoneBg_Zone7_UpdateScreenScroll: a fixed sky band,
 * the far layer (x / 28) and, below the limit line, the near layer (x / 14) */
static void bg_zone57_scroll(const ZScreen *p, int shift, s32 lim_row, s32 near_ofs) {
    s32 lim, h, v, t;
    lim = (p->flags34 & 0x10) ? p->limit : 0x7fff;
    h = (s16)((u32)(fx_div((lim << 8) - p->y, p->scale) >> 4) >> 8);
    if (p->flags34 & 1)
        return;
    s_bg_x = 0;
    s_bg_y = fx_div(p->y << shift, (s_map_h - 0xc0) << 8) >> 4;
    s_ax = 0;
    s_ay = 0;
    s_bx = (p->x / 28) & 0x1ffff;
    s_by = fx_div(p->y << shift, (s_map_h - 0xc0) << 8) >> 4;
    if (h + (s_by >> 8) < lim_row) {
        s_n0 = (u8)(h > 0 ? h : 0);
        s_by = (lim_row - h) << 8;
        t = h + near_ofs;
        if (t < 0xc0) {
            if (t <= 0)
                t = 0;
            s_n1 = (u8)t;
            s_cx = (p->x / 14) & 0xffff;
            s_cy = ((((p->y / 14) >> 8) & 0xff) + 0x100) << 8;
        } else {
            s_n1 = 0xc0;
        }
    } else {
        v = lim_row - (s_by >> 8);
        s_n0 = (u8)CLAMP(v, 0, 0xc0);
        s_n1 = 0xc0;
    }
}
/* ZoneBg_Zone5_BuildLineScroll / ZoneBg_Zone7_BuildLineScroll */
static void bg_zone57_lines(void) {
    u16 x, y;
    u8 n, i;
    x = (u16)(s_ax >> 8);
    y = (u16)(s_ay >> 8);
    for (i = 0; i < s_n0; i++) {
        s_lines[i].x = x;
        s_lines[i].y = y;
    }
    x = (u16)(s_bx >> 8);
    y = (u16)(s_by >> 8);
    for (; i < s_n1; i++) {
        s_lines[i].x = x;
        s_lines[i].y = y;
    }
    y = (u16)(s_cy >> 8);
    x = (u16)(s_cx >> 8);
    n = (0x200 - y < 0xC0) ? (u8)(0x200 - y) : 0xC0;
    for (; i < n; i++) {
        s_lines[i].x = x;
        s_lines[i].y = y;
    }
    for (; i < 0xC0; i++) {
        s_lines[i].x = x;
        s_lines[i].y = (u16)(y - 0x100);
    }
}
static void lines_flat(void) {
    int i;
    for (i = 0; i < 192; i++) {
        s_lines[i].x = (u16)((s_bg_x >> 8) & 0x1ff);
        s_lines[i].y = (u16)((s_bg_y >> 8) & 0x1ff);
    }
}

/* gZoneBgUpdateTable[zone][act] -> kind */
enum { BG_NONE, BG_Z1, BG_Z2, BG_Z3, BG_Z4, BG_Z5, BG_Z6, BG_Z7 };
static int bg_kind_of(int z, int a) {
    static const u8 t[9][3] = {
        { BG_Z1, BG_Z1, 0 }, { BG_Z2, BG_Z2, 0 }, { BG_Z3, BG_Z3, 0 }, { BG_Z4, BG_Z4, 0 },
        { BG_Z5, BG_Z5, 0 }, { BG_Z6, BG_Z6, 0 }, { BG_Z7, BG_Z7, 0 }, { 0, BG_Z3, 0 }, { BG_Z4, BG_Z5, 0 },
    };
    return (z >= 0 && z < 9 && a >= 0 && a < 3) ? t[z][a] : 0;
}
/* gZoneFxInitTable[zone][act] -> kind (2 = Water Palace, 4 Night Carnival, 5 Huge Crisis,
 * 6 Altitude Limit (camera.c), 7 Dead Line; 3 / 8 = the boss arenas: boss module) */
static int fx_kind_of(int z, int a) {
    static const u8 t[9][3] = {
        { 0, 0, 0 }, { 2, 2, 0 }, { 0, 0, 3 }, { 4, 4, 0 }, { 5, 5, 0 }, { 6, 6, 0 }, { 7, 7, 0 }, { 0, 0, 8 }, { 4, 5, 0 },
    };
    return (z >= 0 && z < 9 && a >= 0 && a < 3) ? t[z][a] : 0;
}

/* ZoneBg_Update (+ the line-scroll tasks ZoneBg_Zone<n>_TaskMain) */
static void zone_bg_update(void) {
    ZScreen p;
    screen_of(&p);
    switch (s_bg_kind) {
    case BG_Z1: bg_parallax(&p, 14); lines_flat(); break;
    case BG_Z4: bg_parallax(&p, 18); lines_flat(); break;
    case BG_Z6: bg_zone6(&p); lines_flat(); break;
    case BG_Z2: if (!s_view_only) s_t++; bg_zone2_scroll(&p); bg_zone2_lines(); break;
    case BG_Z3: if (!s_view_only) s_t++; bg_zone3_scroll(&p); bg_zone3_lines(); break;
    case BG_Z5: bg_zone57_scroll(&p, 5, 0xb0, 0x50); bg_zone57_lines(); break;
    case BG_Z7: bg_zone57_scroll(&p, 6, 0xb8, 0x48); bg_zone57_lines(); break;
    default: lines_flat(); break;   /* the Init position (0, 0) */
    }
    s_prev_x = p.x;
    s_have_prev = 1;
}

/* ================================================================ ZoneFx palettes */
/* ZoneFx_InitPaletteAnim: the source is the event-archive file anim[1] (= bg.bin palette
 * anim[1] - 3) */
static const u16 *alt_palette(int file) {
#ifdef _arch_dreamcast
    int k = file - 3;
    if (s_lv && s_lv->bg_pals && k >= 1 && k < s_lv->bg_num_pals)
        return s_lv->bg_pals + k * 256;
#endif
    (void)file;
    return NULL;
}
static const u16 *(*s_alt_palette_fn)(int file) = alt_palette;
static void ZoneFx_InitPaletteAnim(ZfxAnim *e, const u8 *t) {
    e->src = s_alt_palette_fn(t[1]);
    e->anim = t;
    e->buf = t[0];
    e->frame = 0;
    e->timer = (u16)(t[6] | t[7] << 8);
}
/* ZoneFx_StepPaletteAnim: frame << 4 + base of the source -> dst (count colours); the
 * destination keeps its transparency bit */
static void ZoneFx_StepPaletteAnim(ZfxAnim *e, u16 *pal) {
    const u8 *a = e->anim;
    int k;
    e->timer = (u16)(a[6] | a[7] << 8);
    e->frame++;
    if (e->frame >= a[3])
        e->frame = 0;
    if (e->src == NULL)
        return;
    for (k = 0; k < a[5] && a[4] + k < 256; k++)
        pal[a[4] + k] = (u16)((e->src[(e->frame << 4) + a[2] + k] & 0x7fff) | (pal[a[4] + k] & 0x8000));
}
/* ZoneFx_RotatePaletteRange (desc: unk0, start, count, dir) */
static void ZoneFx_RotatePaletteRange(u16 *pal, const u8 *d) {
    u16 *p = pal + d[1], t;
    u8 i;
    if ((s8)d[3] > 0) {
        t = p[0];
        for (i = 0; i < d[2] - 1; i++)
            p[i] = p[i + 1];
        p[i] = t;
    } else {
        t = p[d[2] - 1];
        for (i = d[2] - 1; i != 0; i--)
            p[i] = p[i - 1];
        p[i] = t;
    }
}
static void step_anims(u8 dirty[2]) {
    int j;
    for (j = 0; j < s_n_ent; j++) {
        ZfxAnim *e = &s_ent[j];
        e->timer--;
        if (e->timer == 0) {
            ZoneFx_StepPaletteAnim(e, s_buf[e->buf & 1]);
            dirty[e->buf & 1] = 1;
        }
    }
}

/* ZoneFx_Zone4 BLDY effects (gZoneFxZone4BldyEffectTable) */
static int bldy_ramp_up(int i) {
    s32 n = s_blen[i];
    s_bt[i]++;
    s_bldy[i] = (u16)(fx_div((s_bt[i] * 0xa) << 0xc, n << 0xc) >> 0xc);
    return s_bt[i] >= n;
}
static int bldy_ramp_down(int i) {
    s32 n = s_blen[i];
    s_bt[i]++;
    s_bldy[i] = (u16)(fx_div(((n - s_bt[i]) * 0xa) << 0xc, n << 0xc) >> 0xc);
    return s_bt[i] >= n;
}
static int bldy_from_table(int i) {
    s_bt[i]++;
    s_bldy[i] = s_data.z4_flash[(s_bt[i] - 1) % 60];
    return s_bt[i] >= s_blen[i];
}
static int bldy_step(int i) {
    switch (s_bmode[i]) {
    case 1: return bldy_ramp_up(i);
    case 2: return bldy_ramp_down(i);
    case 3: return bldy_from_table(i);
    default: return 0;     /* ZoneFx_Zone4_BldyNone */
    }
}
void zonefx_bldy(s32 a, s32 b) {
    u32 mask;
    s32 n;
    u8 i;
    if (!s_on || !s_bldy_on)
        return;
    mask = a == 0 ? (u32)b : (u32)(b >> 8);
    n = b & 0xff;
    if (n <= 0)
        n = 1;
    for (i = 0; i < 2; i++) {
        if (!(mask & (1u << i)))
            continue;
        s_bt[i] = 0;                      /* ZoneFx_Zone4_ResetBldyTimer */
        if (a == 0) {                     /* ZoneFx_Zone4_StartBldyFlash */
            s_blen[i] = 0x3c;
            s_bmode[i] = 3;
        } else {                          /* StartBldyRampUp / RampDown */
            s_blen[i] = (u16)n;
            s_bmode[i] = a == 1 ? 1 : 2;
        }
    }
}
int zonefx_bldy_level(void) { return s_bldy[0]; }

/* ZoneFx_Zone5_ApplyPaletteAdd: the enemy-room alarm reddens the BG palette ranges */
static void zone5_alarm(u16 *dst) {
    const u8 *tbl = s_data.z5_add;
    u8 k = 0, add = s_alarm_add[0];
    u16 i = 0, col;
    s32 r;
    do {
        if (!(k & 1)) {
            i = tbl[k++];
        } else if (k < 4 && i == tbl[k]) {
            k++;
        }
        if (i >= 0x100)
            break;
        col = s_base[1][i];
        r = R_(col) + add;
        r = CLAMP(r, 0, 0x1f);
        dst[i] = RGB_(dst[i], r, G_(col), B_(col));
        i++;
    } while (i < 0x100 && k < 5);
}
/* ZoneFx_Zone2_TintPalette (r - 10, g - 1); ZoneFx_Zone2_DmaTintedPalettes copies 0x1E0 bytes,
 * so rows 0..14 are shown tinted and row 15 keeps its normal colours */
static void zone2_tint(u16 *dst, const u16 *src) {
    int i;
    for (i = 0; i < 256; i++) {
        u16 c = src[i];
        s32 r = R_(c) - 10, g = G_(c) - 1;
        dst[i] = i < 0xf0 ? RGB_(c, CLAMP(r, 0, 0x1f), CLAMP(g, 0, 0x1f), B_(c)) : c;
    }
}
/* BLDY brightness decrease of BG1..3 (ZoneFx_SetBrightnessBlend: BG1 | BG2 | BG3) */
static void darken(u16 *pal, int evy) {
    int i;
    if (evy <= 0)
        return;
    if (evy > 16)
        evy = 16;
    for (i = 0; i < 256; i++) {
        u16 c = pal[i];
        s32 r = R_(c), g = G_(c), b = B_(c);
        r -= (r * evy) >> 4;
        g -= (g * evy) >> 4;
        b -= (b * evy) >> 4;
        pal[i] = RGB_(c, r, g, b);
    }
}

/* ZoneFx_Update (+ ZoneFx_Zone2/5 tasks) */
static void zone_fx_update(void) {
    u8 dirty[2] = { 0, 0 };
    int i;
    switch (s_fx_kind) {
    case 4:   /* ZoneFx_Zone4_Update */
        for (i = 0; i < 2; i++)
            if (bldy_step(i))
                s_bmode[i] = 0;
        step_anims(dirty);
        break;
    case 5:   /* ZoneFx_Zone5_Update + ZoneFx_Zone5_TaskMain (UpdatePalettes) */
        s_rcnt[0]++;
        if (s_rcnt[0] >= (s16)(s_data.z5_rot[4] | s_data.z5_rot[5] << 8)) {
            s_rcnt[0] = 0;
            ZoneFx_RotatePaletteRange(s_buf[s_data.z5_rot[0] & 1], s_data.z5_rot);
            dirty[s_data.z5_rot[0] & 1] = 1;
        }
        step_anims(dirty);
        for (i = 0; i < 2; i++) {
            if (g_obj_camera[i].flags34 & 0x10000) {
                s_alarm_sub[i]++;
                if (s_alarm_sub[i] >= 2) {
                    s_alarm_sub[i] = 0;
                    s_alarm_add[i]++;
                    if (s_alarm_add[i] >= 0x1e)
                        s_alarm_add[i] = 0;
                }
            } else if (s_alarm_add[i] != 0) {
                s_alarm_add[i] = 0;                  /* ZoneFx_Zone5_InactivePaletteNop */
                s_alarm_sub[i] = 0;
            }
        }
        break;
    case 7:   /* ZoneFx_Zone7_Update */
        for (i = 0; i < 2; i++) {
            const u8 *d = s_data.z7_rot + i * 6;
            s_rcnt[i]++;
            if (s_rcnt[i] >= (s16)(d[4] | d[5] << 8)) {
                s_rcnt[i] = 0;
                ZoneFx_RotatePaletteRange(s_buf[d[0] & 1], d);
                dirty[d[0] & 1] = 1;
            }
        }
        step_anims(dirty);
        break;
    case 2: { /* ZoneFx_Zone2_TaskMain: CyclePalette (the shimmer of map palette 0x21..0x25) */
        s_cycle++;
        if (s_cycle >= 3) {
            u16 *p = s_buf[0] + 0x21, t;
            int j;
            s_cycle = 0;
            t = p[0];
            for (j = 0; j < 4; j++)
                p[j] = p[j + 1];
            p[j] = (u16)((t & 0x7fff) | (p[j] & 0x8000));
        }
        s_water_row = camera_window_split_y();
        if (!(g_obj_camera[0].flags & 0x1000000) || (g_obj_camera[0].flags & 0x2000000))
            s_water_row = 192;        /* state 3: no water */
        /* the OBJ palette rows 2..15 tinted below the line (QueueTintedPalettes: 0x5000240..) */
        if (objects_set_water_tint)
            objects_set_water_tint(s_water_row < 192 ? (float)g_obj_camera[0].water : 1e9f, ZFX_TINT_ARGB);
        break;
    }
    default:
        break;
    }
    /* the palettes as shown: BLDY (zone 4), the alarm (zone 5), the tint (zone 2) */
    memcpy(s_show[0], s_buf[0], 512);
    memcpy(s_show[1], s_buf[1], 512);
    if (s_fx_kind == 4) {
        darken(s_show[0], s_bldy[0]);
        darken(s_show[1], s_bldy[0]);
    }
    if (s_fx_kind == 5 && (g_obj_camera[0].flags34 & 0x10000))
        zone5_alarm(s_show[1]);
    if (s_fx_kind == 2) {
        zone2_tint(s_show[2], s_buf[0]);
        zone2_tint(s_show[3], s_buf[1]);
    }
}

void zonefx_update(void) {
    if (!s_on)
        return;
    zone_bg_update();
    if (s_view_only) {   /* Battle Play, second camera: its own water row only */
        if (s_fx_kind == 2) {
            s_water_row = camera_window_split_y();
            if (!(g_obj_camera[0].flags & 0x1000000) || (g_obj_camera[0].flags & 0x2000000))
                s_water_row = 192;
        }
        return;
    }
    zone_fx_update();
}

/* ---------------------------------------------------------------- Battle Play view contexts */
struct ZfxView {
    ZfxLine lines[192];
    int water_row, have_prev;
    s32 ax, ay, bx, by, cx, cy, bg_x, bg_y, prev_x;
    u8 n0, n1;
};
size_t zonefx_view_size(void) { return sizeof(struct ZfxView); }
void zonefx_view_save(void *dst) {
    struct ZfxView *v = (struct ZfxView *)dst;
    memcpy(v->lines, s_lines, sizeof s_lines);
    v->water_row = s_water_row; v->have_prev = s_have_prev;
    v->ax = s_ax; v->ay = s_ay; v->bx = s_bx; v->by = s_by; v->cx = s_cx; v->cy = s_cy;
    v->bg_x = s_bg_x; v->bg_y = s_bg_y; v->prev_x = s_prev_x; v->n0 = s_n0; v->n1 = s_n1;
}
void zonefx_view_load(const void *src) {
    const struct ZfxView *v = (const struct ZfxView *)src;
    memcpy(s_lines, v->lines, sizeof s_lines);
    s_water_row = v->water_row; s_have_prev = v->have_prev;
    s_ax = v->ax; s_ay = v->ay; s_bx = v->bx; s_by = v->by; s_cx = v->cx; s_cy = v->cy;
    s_bg_x = v->bg_x; s_bg_y = v->bg_y; s_prev_x = v->prev_x; s_n0 = v->n0; s_n1 = v->n1;
}
void zonefx_set_view_only(int on) { s_view_only = on; }

const ZfxLine *zonefx_lines(void) { return s_lines; }
int zonefx_water_row(void) { return s_fx_kind == 2 ? s_water_row : 192; }
const uint16_t *zonefx_palette(int which) { return s_show[which & 3]; }
u32 zonefx_palette_updates(void) { return s_pal_writes; }

/* ZoneFx_Zone3Boss_Update: every 7 frames of the fight the lights alternate */
int zonefx_boss3_lights(int boss_active, uint16_t out[3]) {
    const u16 *p;
    if (!s_data_ok || !boss_active)
        return 0;
    s_z3b++;
    if ((s_z3b & ~0x8000) <= 6)
        return 0;
    p = (s_z3b & 0x8000) ? s_data.z3b_light0 : s_data.z3b_light1;
    out[0] = p[0];
    out[1] = p[1];
    out[2] = p[2];
    s_z3b &= 0x8000;
    s_z3b ^= 0x8000;
    return 1;
}

/* ================================================================ init */
void zonefx_init(struct Level *lv, int zone, int act) {
    int i;
    const u8 (*tab)[8] = NULL;
    memset(s_lines, 0, sizeof(s_lines));
    memset(s_ent, 0, sizeof(s_ent));
    memset(s_sent_ok, 0, sizeof(s_sent_ok));
    s_lv = lv;
    s_zone = (u8)zone;
    s_act = (u8)act;
    s_n_ent = 0;
    s_t = 0;
    s_have_prev = 0;
    s_bg_x = s_bg_y = 0;
    s_ax = s_ay = s_bx = s_by = s_cx = s_cy = 0;
    s_n0 = s_n1 = 0xc0;
    s_bt[0] = s_bt[1] = s_blen[0] = s_blen[1] = 0;
    s_bmode[0] = s_bmode[1] = 0;
    s_bldy[0] = s_bldy[1] = 0;
    s_rcnt[0] = s_rcnt[1] = 0;
    s_alarm_add[0] = s_alarm_add[1] = s_alarm_sub[0] = s_alarm_sub[1] = 0;
    s_cycle = 0;
    s_water_row = 192;
    s_z3b = 0;
    s_bg_kind = bg_kind_of(zone, act);
    s_fx_kind = fx_kind_of(zone, act);
    s_map_h = camera_global()->map_h ? camera_global()->map_h : 0x400;
    /* ZoneBg_Zone<n>_Init: Zone6 starts at y 0x30 */
    if (s_bg_kind == BG_Z6)
        s_bg_y = 0x3000;
    /* ZoneFx_Zone<n>_Init: copies of the map (ext) and BG (std) palettes */
    memset(s_base, 0, sizeof(s_base));
#ifdef _arch_dreamcast
    if (lv) {
        memcpy(s_base[0], level_palette(lv), 512);
        if (lv->bg_pals)
            memcpy(s_base[1], lv->bg_pals, 512);
    }
#endif
    memcpy(s_buf, s_base, sizeof(s_buf));
    s_bldy_on = 0;
    if (!s_data_ok && s_fx_kind != 2)
        s_fx_kind = 0;
    switch (s_fx_kind) {
    case 4: s_bldy_on = 1; tab = s_data.z4_anim; s_n_ent = 24; break;
    case 5: tab = s_data.z5_anim; s_n_ent = 2; break;
    case 7: tab = s_data.z7_anim; s_n_ent = 2; break;
    default: break;
    }
    for (i = 0; i < s_n_ent; i++)
        ZoneFx_InitPaletteAnim(&s_ent[i], tab[i]);
#ifdef _arch_dreamcast
    /* Water Palace: the tinted palettes need a second PAL8 bank for the map and the BG */
    if (s_fx_kind == 2 && lv) {
        zone2_tint(s_show[2], s_buf[0]);
        zone2_tint(s_show[3], s_buf[1]);
        printf("zonefx: water tint banks %s (map %d, bg %d)\n",
               level_alloc_alt_banks(lv, s_show[2], s_show[3]) == 0 ? "ok" : "MISSING (no free palette rows)",
               lv->alt_pal_bank, lv->alt_bg_pal_bank);
    }
#endif
    memcpy(s_show[0], s_buf[0], 512);
    memcpy(s_show[1], s_buf[1], 512);
    s_on = 1;
    camera_set_zone_hook(zonefx_update);
    zone_bg_update();   /* the first frame's line table (Init + one update) */
}

/* right after level_load, before objects_init / ui_stage_start take PAL4 rows: the Water
 * Palace tinted banks need the same rows as the map (0..4) and the BG (0..12) in banks of
 * their own, which the sprites (allocated from the top row down) would otherwise occupy */
void zonefx_stage_loaded(struct Level *lv, int zone, int act) {
#ifdef _arch_dreamcast
    static u16 map_t[256], bg_t[256];
    if (!lv || fx_kind_of(zone, act) != 2)
        return;
    zone2_tint(map_t, level_palette(lv));
    if (lv->bg_pals)
        zone2_tint(bg_t, lv->bg_pals);
    level_alloc_alt_banks(lv, map_t, lv->bg_pals ? bg_t : NULL);
#else
    (void)lv;
    (void)zone;
    (void)act;
#endif
}

void zonefx_shutdown(void) {
    if (objects_set_water_tint)
        objects_set_water_tint(1e9f, 0xffffffffu);
#ifdef _arch_dreamcast
    zfx_water_sprite_free();
#endif
    s_on = 0;
    s_lv = NULL;
    camera_set_zone_hook(NULL);
}

/* ================================================================ drawing (KOS) */
#ifdef _arch_dreamcast

/* Palette RAM is read while the PVR renders: a write during the render of the previous
 * frame would show mixed colours. Called at the frame start (after pvr_wait_ready): a
 * changed palette is written when the render in progress ends (pal_write_rows: the
 * render-done interrupt), so the frame being built renders with the new colours and the
 * CPU does not wait for that render (it waited 17-19.7 ms per frame in z21 / z22). */
void zonefx_commit_palettes(void) {
    int k, dirty[4] = { 0, 0, 0, 0 }, any = 0;
    if (!s_on || !s_lv)
        return;
    for (k = 0; k < 4; k++) {
        if (k >= 2 && s_fx_kind != 2)
            break;
        dirty[k] = !s_sent_ok[k] || memcmp(s_sent[k], s_show[k], 512) != 0;
        any |= dirty[k];
    }
    if (!any)
        return;
    for (k = 0; k < 4; k++) {
        if (!dirty[k])
            continue;
        if (k == 0) pal_write_rows(s_lv->pal_bank, s_lv->pal_rows, s_show[0]);
        else if (k == 1) pal_write_rows(s_lv->bg_pal_bank, s_lv->bg_pal_rows, s_show[1]);
        else if (k == 2) pal_write_rows(s_lv->alt_pal_bank, s_lv->pal_rows, s_show[2]);
        else pal_write_rows(s_lv->alt_bg_pal_bank, s_lv->bg_pal_rows, s_show[3]);
        memcpy(s_sent[k], s_show[k], 512);
        s_sent_ok[k] = 1;
        s_pal_writes++;
    }
}

void zonefx_draw_bg(float z) {
    if (!s_on || !s_lv)
        return;
    level_draw_bg_lines(s_lv, (const LevelBgLine *)s_lines, s_fx_kind == 2 ? s_water_row : 192, ZFX_TINT_ARGB, z);
}

void zonefx_draw_layer(int layer, int list, float z) {
    if (!s_on || !s_lv)
        return;
    if (s_fx_kind == 2 && s_water_row < 192)
        level_draw_layer_split(s_lv, layer, (pvr_list_t)list, z, g_cam_y + g_view_row_ofs + s_water_row, ZFX_TINT_ARGB);
    else
        level_draw_layer(s_lv, layer, (pvr_list_t)list, z);
}

/* ZoneFx_Zone2_LoadSprites / _DrawSprites: the water surface (ac_eff_water.bac
 * animation 3) every 0x40 px along the water line, OAM priority 0 */
static ObjSprite s_wspr;
static int s_wspr_ok = -1;
void zfx_water_sprite_free(void) { s_wspr_ok = -1; }
static void water_surface(void) {
    const DsprImage *img;
    SprAtlas *a;
    int bank, im;
    s32 y, x;
    if (s_wspr_ok < 0) {
        s_wspr_ok = 0;
        if (obj_res_ensure(OBJ_BAC_EFF_WATER) == 0 && g_obj.res[OBJ_BAC_EFF_WATER].n_anims > 3) {
            spr_init(&s_wspr, &g_obj.res[OBJ_BAC_EFF_WATER], 3, SPR_LOOP);
            s_wspr_ok = 1;
        }
    }
    if (!s_wspr_ok || s_water_row >= 192 + 3 || !(g_obj_camera[0].flags & 0x1000000))
        return;
    spr_process(&s_wspr, NULL, NULL);
    img = spr_image(&s_wspr);
    a = (SprAtlas *)s_wspr.res->atlas;
    if (img == NULL || a == NULL || img->page >= a->n_pages || a->tex[img->page].ptr == NULL)
        return;
    y = ((s32)g_obj_camera[0].water) - (s32)g_cam_y;
    if (y < -3 || y > (s32)VIEW_H + 3)
        return;
    im = (int)(img - s_wspr.res->images);
    bank = spr_atlas_bank(a, im, -1);
    /* AnimatorSprite_Init(.., 0x4004, ..): OAM priority 0, order 0x15 = the OBJ prio 0 band of
     * obj_draw.c (11 + (0x40 - 0x15) * 0.01), in front of layer A */
    for (x = -0x20; x < (s32)VIEW_W; x += 0x40)
        draw_sprite_world_pal(&a->tex[img->page], bank, PVR_LIST_TR_POLY, g_cam_x + x - img->origin_x,
                              g_cam_y + y - img->origin_y, (float)img->w, (float)img->h, (float)img->x,
                              (float)img->y, (float)(img->x + img->w), (float)(img->y + img->h), 0, 11.43f, 0xffffffff);
}

void zonefx_draw_tr(void) {
    if (!s_on || !s_lv)
        return;
    if (s_fx_kind == 2)
        water_surface();
}
#else
void zonefx_commit_palettes(void) {}
void zonefx_draw_bg(float z) { (void)z; }
void zonefx_draw_layer(int layer, int list, float z) { (void)layer; (void)list; (void)z; }
void zonefx_draw_tr(void) {}
/* host tests: alternate palettes from a bg.bin buffer */
static const u16 *s_host_pals;
static int s_host_npals;
static const u16 *host_alt(int file) {
    int k = file - 3;
    return (s_host_pals && k >= 1 && k < s_host_npals) ? s_host_pals + k * 256 : NULL;
}
void zonefx_host_palettes(const u16 *map_pal, const u16 *bg_pals, int n) {
    s_host_pals = bg_pals;
    s_host_npals = n;
    s_alt_palette_fn = host_alt;
    if (map_pal)
        memcpy(s_base[0], map_pal, 512);
    if (bg_pals)
        memcpy(s_base[1], bg_pals, 512);
}
void zonefx_host_reinit_palettes(void) {
    int i;
    const u8 (*tab)[8] = s_fx_kind == 4 ? s_data.z4_anim : s_fx_kind == 5 ? s_data.z5_anim : s_data.z7_anim;
    memcpy(s_buf, s_base, sizeof(s_buf));
    for (i = 0; i < s_n_ent; i++)
        ZoneFx_InitPaletteAnim(&s_ent[i], tab[i]);
}
#endif
