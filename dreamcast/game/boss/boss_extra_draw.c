/*
 * Extra Zone boss: native rendering (no DS counterpart function by function).
 *
 * The DS draws the fight on the main screen: BG1 = zex.bbg with a per-line horizontal wave (HDMA)
 * and palette cycling, BG0 = the 3D field through camera slot 0 (a 256x192 window that follows the
 * player), OBJ = rings and the HUD (ac_ex_fix_moji.bac).  The sub screen shows a "VS" splash when a
 * round starts, and during the character switch both screens show one 3D panorama (camera slots 1 and
 * 2, rendered on alternate frames).  The Dreamcast shows ONE 640x480 view:
 *   - battle: the main screen at 2.5x (the 256x192 window is already 4:3);
 *   - the VS splash: a cut-in panel (sub screen rows 40..152 at 1.25x) in the upper part of the view while
 *     it animates, held 60 frames, faded out;
 *   - character switch: one camera (slot 3, composed from slots 1 and 2) that shows both screens'
 *     content in one picture;
 *   - master brightness (BrightnessLerp): a full-screen white / black overlay.
 * Model_Draw is split as in boss_model.c: ex_draw_model poses the joints where the DS draws (the node
 * callbacks run there, so the game logic gets its node matrices) and queues the model; ex_draw_frame
 * submits the queue.  Host builds rasterise the queue in software for the tests.
 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "boss_extra_int.h"

#ifdef _arch_dreamcast
#include <dc/pvr.h>
#include <malloc.h>
#include "../engine/render.h"
#endif

void *ex_file_load(const char *rel, u32 *n);
const dcm_lights_t *ex_lights(void);

/* ---------------------------------------------------------- sprite resources */
int ex_sprres_load(ExSprRes *r, const char *id) {
    char rel[128];
    u32 n;
    u8 *f;
    u16 hdr[6];
    memset(r, 0, sizeof(*r));
    snprintf(rel, sizeof(rel), "sprites/%s/frames.bin", id);
    f = (u8 *)ex_file_load(rel, &n);
    if (!f || n < 20 || memcmp(f, "DSPR", 4) != 0) {
        free(f);
        return -1;
    }
    memcpy(hdr, f + 4, 12);
    r->frames = f;
    r->n_images = hdr[2];
    r->n_anims = hdr[3];
    r->n_frames = hdr[4];
    memcpy(&r->page_w, f + 16, 2);
    memcpy(&r->page_h, f + 18, 2);
    r->images = (const ExSprImage *)(f + 16 + 4 * hdr[1]);
    r->anims = (const ExSprAnim *)((const u8 *)r->images + 16 * r->n_images);
    r->frametab = (const u16 *)((const u8 *)r->anims + 12 * r->n_anims);
#ifdef _arch_dreamcast
    {   /* PAL4 page with its palette rows (palettes.bin + atlas0.pt4), else ARGB1555 */
        char dir[224];
        snprintf(dir, sizeof(dir), "%s/sprites/%s", g_ex_data_dir, id);
        r->atlas = calloc(1, sizeof(SprAtlas));
        if (r->atlas && spr_atlas_load((SprAtlas *)r->atlas, dir, f, 0) > 0) {
            r->loaded = 1;
            return 0;
        }
        if (r->atlas)
            spr_atlas_free((SprAtlas *)r->atlas);
        free(r->atlas);
        r->atlas = NULL;
    }
#endif
    snprintf(rel, sizeof(rel), "sprites/%s/atlas0.argb1555", id);
    r->pixels = (u16 *)ex_file_load(rel, &n);
    if (!r->pixels || n != (u32)r->page_w * r->page_h * 2) {
        free(r->pixels);
        free(f);
        memset(r, 0, sizeof(*r));
        return -1;
    }
    r->loaded = 1;
    return 0;
}
void ex_sprres_free(ExSprRes *r) {
#ifdef _arch_dreamcast
    if (r->tex)
        pvr_mem_free((pvr_ptr_t)r->tex);
    if (r->atlas)
        spr_atlas_free((SprAtlas *)r->atlas);
#endif
    free(r->atlas);
    free(r->frames);
    free(r->pixels);
    memset(r, 0, sizeof(*r));
}

/* AnimatorSprite subset on the converted BAC tables (as special/sp_sprite.c: wait = duration << 4,
 * speed 0x10 per frame, flag 2 at the end of the last frame, non-looping sprites stop with flag 1) */
void ex_sprite_init(ExSprite *s, const ExSprRes *res, u16 anim, u32 flags) {
    memset(s, 0, sizeof(*s));
    s->res = res;
    s->speed = 0x10;
    s->flags = flags & ~0x4000u;   /* 0x4000 / 0x20 / 0x40: OAM attribute bits of the DS call */
    s->frame = -1;
    s->anim = anim;
    s->cur_image = -1;
    s->sx = s->sy = 0x1000;
}
static void ex_sprite_enter_frame(ExSprite *s) {
    const ExSprRes *r = s->res;
    const ExSprAnim *a;
    if (!r || !r->loaded || s->anim >= r->n_anims) {
        s->cur_image = -1;
        s->wait = 0x7fffffff;
        return;
    }
    a = &r->anims[s->anim];
    s->frame++;
    if (s->frame >= a->frame_count) {
        if (a->next_anim >= 0 && a->next_anim < r->n_anims && a->loop_frame < 0) {
            s->anim = (u16)a->next_anim;
            a = &r->anims[s->anim];
            s->frame = 0;
        } else
            s->frame = a->loop_frame >= 0 ? a->loop_frame : 0;
    }
    if (a->frame_count == 0) {
        s->cur_image = -1;
        s->wait = 0x7fffffff;
        return;
    }
    {
        const u16 *fr = &r->frametab[(a->first_frame + s->frame) * 2];
        s->cur_image = fr[0] == 0xffff ? -1 : (s16)fr[0];
        s->wait = (s32)fr[1] << 4;
        s->frameNo = (u16)(s->frame + 1);
        s->frameCount = a->frame_count;
    }
}
void ex_sprite_process(ExSprite *s) {
    if (s->flags & 1)
        return;
    s->flags &= ~2u;
    if (s->wait > 0) {
        s->wait -= s->speed;
        if (s->wait > 0)
            return;
        if (s->frameNo == s->frameCount) {
            s->flags |= 2;
            if (!(s->flags & 4)) {
                s->flags |= 1;
                return;
            }
        }
    }
    ex_sprite_enter_frame(s);
}

/* ----------------------------------------------------------------- draw list */
#define EX_MAX_ITEMS 64
#define EX_MAX_SPR 160
#define EX_MAX_TRAIL 24
typedef struct { const ExSprRes *res; s16 image; float x, y, sx, sy, rot; u32 flags; } ExSprItem;
typedef struct { float v[4][3]; u32 argb; } ExTrailQuad;
static ExDrawItem s_items[EX_MAX_ITEMS];
static int s_nitems;
static ExSprItem s_spr[EX_MAX_SPR];
static int s_nspr;
static ExTrailQuad s_trail[EX_MAX_TRAIL];
static int s_ntrail, s_trail_slot;

void ex_draw_reset(void) {
    s_nitems = 0;
    s_nspr = 0;
    s_ntrail = 0;
}

/* node callbacks (NNS_G3dRenderObjSetCallBack on the model's render object) */
static struct { const BossModel *m; void (*fn)(BossModel *); } s_cb[8];
void ex_set_node_callback(BossModel *m, void (*fn)(BossModel *)) {
    int i, fr = -1;
    for (i = 0; i < 8; i++) {
        if (s_cb[i].m == m) {
            s_cb[i].fn = fn;
            if (fn == NULL)
                s_cb[i].m = NULL;
            return;
        }
        if (s_cb[i].m == NULL && fr < 0)
            fr = i;
    }
    if (fn != NULL && fr >= 0) {
        s_cb[fr].m = m;
        s_cb[fr].fn = fn;
    }
}
int ex_model_index(const BossModel *m) {
    int i;
    for (i = 0; i < 5; i++)
        if (m->res == &g_ex_bsex[i])
            return i;
    return -1;
}
/* NNS_G3dGetCurrentMtx after MTX_RESTORE slot (position matrix, fx32) */
void ex_get_slot_mtx(int slot, ExMtx43 *out) {
    const dcm_mtx_t *m = &g_boss_mtx_slot[slot & 31];
    int r, c;
    for (r = 0; r < 4; r++)
        for (c = 0; c < 3; c++)
            out->m[r * 3 + c] = (fx32)lrintf(m->m[r][c] * 4096.0f);
}

/* Model_Draw: pose (+ node callback) and queue */
void ex_draw_model(BossModel *m, int offaxis, const float fr[4]) {
    ExDrawItem *it;
    int i;
    if (m == NULL || m->res == NULL || !m->res->loaded)
        return;
    boss_model_pose(m, NULL);
    for (i = 0; i < 8; i++)
        if (s_cb[i].m == m && s_cb[i].fn != NULL)
            s_cb[i].fn(m);
    for (i = 0; i < s_nitems; i++)   /* a model drawn twice in a frame keeps its last pose */
        if (s_items[i].m == m)
            break;
    if (i == s_nitems) {
        if (s_nitems >= EX_MAX_ITEMS)
            return;
        s_nitems++;
    }
    it = &s_items[i];
    it->m = m;
    it->mtx = m->mdl_mtx;
    it->offaxis = offaxis;
    if (offaxis && fr != NULL)
        memcpy(it->fr, fr, sizeof(it->fr));
    it->slot = gExtraBossWork ? gExtraBossWork->unk4 : 0;
    it->has_vis = 0;
    if (m->anm[4] != NULL && m->anm[4]->res != NULL && m->anm[4]->res->loaded &&
        m->anm[4]->res->set.hdr != NULL && m->anm[4]->res->set.hdr->kind == DCA_KIND_VIS) {
        dca_visibility(&m->anm[4]->res->set, m->anm[4]->idx, (float)(m->anm[4]->frame >> 12), &m->res->mdl, it->vis);
        it->has_vis = 1;
    }
}

/* ExtraBossObj_Draw_PlayerTrail: one triangle-strip quad per segment between the node pairs,
 * colour Math_LerpRGB555(unk2B0, unk2B2), alpha from the c0 -> c1 -> c2 ramp */
static u16 ex_lerp_rgb555(u16 a, u16 b, s32 n, s32 i) {
    s32 r0 = a & 31, g0 = (a >> 5) & 31, b0 = (a >> 10) & 31;
    s32 r1 = b & 31, g1 = (b >> 5) & 31, b1 = (b >> 10) & 31;
    if (n <= 0)
        return a;
    return (u16)((r0 + (r1 - r0) * i / n) | ((g0 + (g1 - g0) * i / n) << 5) | ((b0 + (b1 - b0) * i / n) << 10));
}
void ex_draw_trail(const ExObj *p) {
    const ExTrailWork *s = (const ExTrailWork *)p->unk1DC;
    const ExTrailNode *cur, *nx;
    s32 ox, oy, dx, dy, n, mid, i, j, c;
    if (s->unk2A8 <= 0)
        return;
    ox = s->unk2A4->b[0] + data_ov001_0231f524;
    oy = s->unk2A4->b[1] + data_ov001_0231f520;
    dx = p->unk30 - ox;
    dy = p->unk34 - oy;
    cur = s->unk2A4;
    nx = cur->prev;
    mid = FX_DivS32(s->unk2A8 * s->unk2C4, s->unk2C0);
    n = s->unk2A8;
    s_trail_slot = gExtraBossWork ? gExtraBossWork->unk4 : 0;
    for (i = 0; i < n && s_ntrail < EX_MAX_TRAIL; i++) {
        const fx32 *v[4];
        ExTrailQuad *q = &s_trail[s_ntrail++];
        u16 col;
        if (i < mid)
            c = s->unk2B4 + FX_DivS32(i * (s->unk2B8 - s->unk2B4), mid);
        else
            c = s->unk2B8 + FX_DivS32((s->unk2BC - s->unk2B8) * (i - mid), n - mid);
        v[0] = cur->b;
        v[1] = cur->a;
        v[2] = nx->b;
        v[3] = nx->a;
        col = ex_lerp_rgb555(s->unk2B0, s->unk2B2, s->unk2A8, i);
        for (j = 0; j < 4; j++) {
            s16 X = (s16)((data_ov001_0231f524 + (dx + v[j][0])) >> 12);
            s16 Y = (s16)-(s16)((data_ov001_0231f520 + (dy + v[j][1])) >> 12);
            q->v[j][0] = X + (ox - 0x10000) / 4096.0f;
            q->v[j][1] = Y - (oy - 0x4000) / 4096.0f;
            q->v[j][2] = 1.0f;
        }
        q->argb = ((u32)((c & 31) * 255 / 31) << 24) | ((u32)((col & 31) * 255 / 31) << 16) |
                  ((u32)(((col >> 5) & 31) * 255 / 31) << 8) | (u32)(((col >> 10) & 31) * 255 / 31);
        cur = nx;
        nx = nx->prev;
    }
}

/* AnimatorSprite_Draw / DrawRotScale (main engine OAM) */
static void ex_queue_sprite(const ExSprite *s, float sx, float sy, float rot) {
    ExSprItem *q;
    if (s->res == NULL || s->cur_image < 0 || s_nspr >= EX_MAX_SPR)
        return;
    q = &s_spr[s_nspr++];
    q->res = s->res;
    q->image = s->cur_image;
    q->x = s->x;
    q->y = s->y;
    q->sx = sx;
    q->sy = sy;
    q->rot = rot;
    q->flags = s->flags;
}
void ex_sprite_draw(const ExSprite *s) { ex_queue_sprite(s, 1.0f, 1.0f, 0.0f); }
void ex_sprite_draw_rs(const ExSprite *s, s32 sx, s32 sy, u16 rot) {
    ex_queue_sprite(s, sx / 4096.0f, sy / 4096.0f, rot * (6.2831853f / 65536.0f));
}
void ex_hud_sprite(const ExSprite *s) { ex_sprite_draw(s); }

/* ------------------------------------------------------------------- camera */
static void ex_norm3(float *v) {
    float l = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    if (l > 0) {
        v[0] /= l;
        v[1] /= l;
        v[2] /= l;
    }
}
/* G3i_PerspectiveW_ + G3i_LookAt_ of a camera slot (ExtraBoss_ApplyScreenFitCamera / ApplyCamera).
 * Slot 3 is native: the character switch shows slots 1 (top screen) and 2 (bottom screen) as one
 * picture - eye and target averaged, vertical field of view widened to cover both screens. */
#define EX_SWITCH_FOVY 0x2400
static void ex_camera_ex(int slot, const float *fr, dcm_camera_t *c) {
    float eye[3], at[3], up[3], x[3], y[3], z[3], cot, n, f, aspect;
    int i;
    u16 fovy;
    const ExCamSlot *p;
    memset(c, 0, sizeof(*c));
    if (data_ov001_0231f080 == NULL)
        return;
    if (slot == 3) {
        const ExCamSlot *a = &data_ov001_0231f080[1], *b = &data_ov001_0231f080[2];
        p = a;
        for (i = 0; i < 3; i++) {
            eye[i] = (a->unk14[i] + a->unk38[i] + b->unk14[i] + b->unk38[i]) / 8192.0f;
            at[i] = (a->unk20[i] + a->unk38[i] + b->unk20[i] + b->unk38[i]) / 8192.0f;
            up[i] = a->unk2C[i] / 4096.0f;
        }
        fovy = EX_SWITCH_FOVY;
    } else {
        p = &data_ov001_0231f080[slot];
        for (i = 0; i < 3; i++) {
            eye[i] = (p->unk14[i] + p->unk38[i]) / 4096.0f;
            at[i] = (p->unk20[i] + p->unk38[i]) / 4096.0f;
            up[i] = p->unk2C[i] / 4096.0f;
        }
        fovy = p->unk0;
    }
    for (i = 0; i < 3; i++)
        z[i] = eye[i] - at[i];
    ex_norm3(z);
    x[0] = up[1] * z[2] - up[2] * z[1];
    x[1] = up[2] * z[0] - up[0] * z[2];
    x[2] = up[0] * z[1] - up[1] * z[0];
    ex_norm3(x);
    y[0] = z[1] * x[2] - z[2] * x[1];
    y[1] = z[2] * x[0] - z[0] * x[2];
    y[2] = z[0] * x[1] - z[1] * x[0];
    for (i = 0; i < 3; i++) {
        c->view.m[i][0] = x[i];
        c->view.m[i][1] = y[i];
        c->view.m[i][2] = z[i];
    }
    c->view.m[3][0] = -(x[0] * eye[0] + x[1] * eye[1] + x[2] * eye[2]);
    c->view.m[3][1] = -(y[0] * eye[0] + y[1] * eye[1] + y[2] * eye[2]);
    c->view.m[3][2] = -(z[0] * eye[0] + z[1] * eye[1] + z[2] * eye[2]);
    n = p->unk4 / 4096.0f;
    f = p->unk8 / 4096.0f;
    aspect = p->unkC / 4096.0f;
    if (fr != NULL && slot != 3) {
        /* G3i_FrustumW_(t, b, l, r, n, f) of ExtraBossObj_DrawModel (camera flag 1) */
        float t = fr[0], bt = fr[1], l = fr[2], r = fr[3];
        c->proj[0][0] = 2 * n / (r - l);
        c->proj[1][1] = 2 * n / (t - bt);
        c->proj[2][0] = (r + l) / (r - l);
        c->proj[2][1] = (t + bt) / (t - bt);
        c->proj[2][2] = (f + n) / (n - f);
        c->proj[2][3] = -1.0f;
        c->proj[3][2] = 2 * f * n / (n - f);
    } else {
        cot = cosf(fovy * (6.2831853f / 65536.0f)) / sinf(fovy * (6.2831853f / 65536.0f));
        c->proj[0][0] = cot / aspect;
        c->proj[1][1] = cot;
        c->proj[2][2] = (f + n) / (n - f);
        c->proj[2][3] = -1.0f;
        c->proj[3][2] = 2 * f * n / (n - f);
    }
    c->vp_x = 0;
    c->vp_y = 0;
    c->vp_w = 640;
    c->vp_h = 480;
    c->near_w = 0.5f;
}
#ifndef _arch_dreamcast
int g_ex_host_switch_slot = 3;   /* host test: 1 / 2 render what the DS top / bottom screen shows */
#define EX_SWITCH_SLOT g_ex_host_switch_slot
#else
#define EX_SWITCH_SLOT 3
#endif
static int ex_view_slot(int slot) {
    /* the dual-screen slots of the character switch are drawn with the composed camera */
    return (slot == 1 || slot == 2) ? EX_SWITCH_SLOT : slot;
}
void ex_camera(int slot, dcm_camera_t *out) { ex_camera_ex(ex_view_slot(slot), NULL, out); }
static void ex_item_camera(const ExDrawItem *it, dcm_camera_t *c) {
    ex_camera_ex(ex_view_slot(it->slot), it->offaxis ? it->fr : NULL, c);
}
#ifdef _arch_dreamcast
/* NNS light vectors go through the camera rotation */
static void ex_view_lights(const dcm_camera_t *c, dcm_lights_t *out) {
    const dcm_lights_t *l = ex_lights();
    int i, k;
    *out = *l;
    for (i = 0; i < l->count; i++)
        for (k = 0; k < 3; k++)
            out->dir[i][k] = l->dir[i][0] * c->view.m[0][k] + l->dir[i][1] * c->view.m[1][k] +
                             l->dir[i][2] * c->view.m[2][k];
}
#endif

/* -------------------------------------------------------------- frame state */
static int ex_bg_visible(void) {
    /* BG1 (zex) is shown from ExtraBoss_StartBattle (DISPCNT 0x1300); the title card shows its own BGs */
    return gExtraBossWork != NULL && data_ov001_0231f54c != NULL && g_ex_bg.bytes != NULL &&
           gExtraBossWork->unk8->fn != ExtraBoss_Main;
}
/* The VS splash: the DS sub screen keeps it for the whole round; the single view shows it as a cut-in
 * panel (sub screen at 1.25x, rows EX_CUTIN_Y0..Y1 only) while the sequence runs, holds it for
 * EX_CUTIN_HOLD frames and fades it out in 16. */
#define EX_CUTIN_HOLD 60
#define EX_CUTIN_Y0 40
#define EX_CUTIN_Y1 152
#define EX_CUTIN_K 1.25f
#define EX_CUTIN_OX 160.0f
#define EX_CUTIN_OY (64.0f - EX_CUTIN_Y0 * EX_CUTIN_K)
static s32 s_cutin_alpha;
static void ex_cutin_update(void) {
    ExSubWork *s = data_ov001_0231f5a8;
    s32 t;
    s_cutin_alpha = 0;
    if (s == NULL || !s->drawn || !g_ex_vs_sprites.loaded)
        return;
    t = s->cutin_t;
    if (t < EX_CUTIN_HOLD)
        s_cutin_alpha = 255;
    else if (t < EX_CUTIN_HOLD + 16)
        s_cutin_alpha = 255 - (t - EX_CUTIN_HOLD) * 16;
}

/* --------------------------------------------------------- VS portrait (sub BG) */
static ExBgImage s_vsbg[2];
#ifdef _arch_dreamcast
static pvr_ptr_t s_vs_tex[2];
#endif
void ex_vsbg_load(void) {
    static const char *n[2] = {"vs_son.bin", "vs_blz.bin"};
    int i;
    for (i = 0; i < 2; i++) {
        u32 sz;
        u8 *b;
        if (s_vsbg[i].bytes)
            continue;
        b = (u8 *)ex_file_load(n[i], &sz);
        if (b == NULL || sz < 12 || memcmp(b, "EXBG", 4) != 0) {
            free(b);
            continue;
        }
        s_vsbg[i].bytes = b;
        s_vsbg[i].w = *(const u16 *)(b + 4);
        s_vsbg[i].h = *(const u16 *)(b + 6);
        s_vsbg[i].base = (const u16 *)(b + 12);
        s_vsbg[i].px = (const u8 *)(s_vsbg[i].base + 256);
    }
}
void ex_vsbg_free(void) {
    int i;
    for (i = 0; i < 2; i++) {
#ifdef _arch_dreamcast
        if (s_vs_tex[i])
            pvr_mem_free(s_vs_tex[i]);
        s_vs_tex[i] = NULL;
#endif
        free(s_vsbg[i].bytes);
        memset(&s_vsbg[i], 0, sizeof(s_vsbg[i]));
    }
}

#ifdef _arch_dreamcast
/* ======================================================= Dreamcast (PVR) */
static pvr_ptr_t s_bg_tex;
/* indexed image -> ARGB1555 texture.  backdrop: colour 0 of every palette bank is the BG backdrop
 * (palette colour 0, opaque), else transparent */
static void ex_upload_indexed(const ExBgImage *img, const u16 *pal, int tw, int th, pvr_ptr_t *tex, int backdrop) {
    u16 *buf;
    int x, y;
    if (*tex == NULL)
        *tex = pvr_mem_malloc((size_t)tw * th * 2);
    if (*tex == NULL)
        return;
    buf = (u16 *)memalign(32, (size_t)tw * th * 2);
    if (buf == NULL)
        return;
    memset(buf, 0, (size_t)tw * th * 2);
    for (y = 0; y < img->h && y < th; y++)
        for (x = 0; x < img->w && x < tw; x++) {
            u8 i = img->px[y * img->w + x];
            u16 c = (i & 15) ? pal[i] : pal[0];
            buf[y * tw + x] = ((i & 15) || backdrop)
                                  ? (u16)(0x8000 | (c & 0x1f) << 10 | (c & 0x3e0) | (c >> 10 & 0x1f))
                                  : 0;
        }
    pvr_txr_load(buf, *tex, (size_t)tw * th * 2);
    free(buf);
}
/* fmt: texture-format word (tex_txr_format), 0 = ARGB1555 non-twiddled */
static void ex_quad_fmt(pvr_list_t list, pvr_ptr_t tex, int fmt, int tw, int th, float x0, float y0, float x1,
                        float y1, float u0, float v0, float u1, float v1, float z, u32 argb) {
    pvr_poly_cxt_t cxt;
    pvr_poly_hdr_t hdr;
    pvr_vertex_t v __attribute__((aligned(32)));   /* pvr_prim: 8-byte aligned or dropped */
    if (tex != NULL)
        pvr_poly_cxt_txr(&cxt, list, fmt ? fmt : PVR_TXRFMT_ARGB1555 | PVR_TXRFMT_NONTWIDDLED, tw, th, tex,
                         PVR_FILTER_NONE);
    else
        pvr_poly_cxt_col(&cxt, list);
    cxt.gen.culling = PVR_CULLING_NONE;
    pvr_poly_compile(&hdr, &cxt);
    pvr_prim(&hdr, sizeof(hdr));
    v.argb = argb;
    v.oargb = 0;
    v.z = z;
    v.flags = PVR_CMD_VERTEX;
    v.x = x0; v.y = y1; v.u = u0; v.v = v1; pvr_prim(&v, sizeof(v));
    v.x = x0; v.y = y0; v.u = u0; v.v = v0; pvr_prim(&v, sizeof(v));
    v.x = x1; v.y = y1; v.u = u1; v.v = v1; pvr_prim(&v, sizeof(v));
    v.flags = PVR_CMD_VERTEX_EOL;
    v.x = x1; v.y = y0; v.u = u1; v.v = v0; pvr_prim(&v, sizeof(v));
}
static void ex_quad(pvr_list_t list, pvr_ptr_t tex, int tw, int th, float x0, float y0, float x1, float y1, float u0,
                    float v0, float u1, float v1, float z, u32 argb) {
    ex_quad_fmt(list, tex, 0, tw, th, x0, y0, x1, y1, u0, v0, u1, v1, z, argb);
}
/* the resource can be drawn (PAL4 atlas, or the uploaded ARGB1555 page) */
static int ex_sprres_ready(const ExSprRes *r) {
    return r->atlas ? ((SprAtlas *)r->atlas)->tex[0].ptr != NULL : r->tex != NULL;
}
static void ex_sprres_upload(ExSprRes *r) {
    if (r->loaded && r->tex == NULL && r->atlas == NULL && r->pixels) {
        r->tex = pvr_mem_malloc((size_t)r->page_w * r->page_h * 2);
        if (r->tex)
            pvr_txr_load(r->pixels, (pvr_ptr_t)r->tex, (size_t)r->page_w * r->page_h * 2);
    }
}
static void ex_draw_sprite_item(pvr_list_t list, const ExSprItem *q, float ox, float oy, float k, u32 argb, float z) {
    const ExSprImage *im = &q->res->images[q->image];
    float w = im->w, h = im->h, iox = (q->flags & 0x80) ? im->w - im->ox : im->ox;
    float x0 = ox + (q->x - iox * q->sx) * k, y0 = oy + (q->y - im->oy * q->sy) * k;
    float x1 = x0 + w * q->sx * k, y1 = y0 + h * q->sy * k;
    float u0 = (float)im->x / q->res->page_w, v0 = (float)im->y / q->res->page_h;
    float u1 = (float)(im->x + im->w) / q->res->page_w, v1 = (float)(im->y + im->h) / q->res->page_h;
    if (q->flags & 0x80) {
        float t = u0;
        u0 = u1;
        u1 = t;
    }
    if (q->res->atlas) {   /* PAL4 page, the image's palette row */
        SprAtlas *a = (SprAtlas *)q->res->atlas;
        int bank = spr_atlas_bank(a, q->image, -1);
        ex_quad_fmt(list, a->tex[0].ptr, tex_txr_format(&a->tex[0], bank), q->res->page_w, q->res->page_h, x0, y0,
                    x1, y1, u0, v0, u1, v1, z, argb);
        return;
    }
    ex_quad(list, (pvr_ptr_t)q->res->tex, q->res->page_w, q->res->page_h, x0, y0, x1, y1, u0, v0, u1, v1, z, argb);
}
/* BG1: 192 one-line strips with the line's HOFS (the HDMA wave), opaque (colour 0 = backdrop) */
static void ex_dc_bg(void) {
    ExBgWork *b = data_ov001_0231f54c;
    const s16 *hofs = b->unk5AC[b->unkB2C];
    pvr_poly_cxt_t cxt;
    pvr_poly_hdr_t hdr;
    pvr_vertex_t v __attribute__((aligned(32)));   /* pvr_prim: 8-byte aligned or dropped */
    int l;
    if (b->pal_dirty || s_bg_tex == NULL) {
        ex_upload_indexed(&g_ex_bg, b->pal, 256, 256, &s_bg_tex, 1);
        b->pal_dirty = 0;
    }
    if (s_bg_tex == NULL)
        return;
    pvr_poly_cxt_txr(&cxt, PVR_LIST_OP_POLY, PVR_TXRFMT_ARGB1555 | PVR_TXRFMT_NONTWIDDLED, 256, 256, s_bg_tex,
                     PVR_FILTER_NONE);
    cxt.gen.culling = PVR_CULLING_NONE;
    pvr_poly_compile(&hdr, &cxt);
    pvr_prim(&hdr, sizeof(hdr));
    v.argb = 0xffffffffu;
    v.oargb = 0;
    /* behind every model, but in front of the PVR background plane: KOS puts that at depth 0.0001
     * (pvr_state.zclip, PVR_BGPLANE_Z) and OP polygons pass the depth test only above it (at 0.00002
     * the whole backdrop failed and the arena stayed black) */
    v.z = 0.00012f;
    for (l = 0; l < 192; l++) {
        float u0 = (hofs[l] & 0xff) / 256.0f, u1 = u0 + 1.0f;
        float tv = ((l + b->vofs) & 0xff) / 256.0f + 0.5f / 256.0f;
        float y0 = l * 2.5f, y1 = (l + 1) * 2.5f;
        v.flags = PVR_CMD_VERTEX;
        v.x = 0; v.y = y1; v.u = u0; v.v = tv; pvr_prim(&v, sizeof(v));
        v.x = 0; v.y = y0; v.u = u0; v.v = tv; pvr_prim(&v, sizeof(v));
        v.x = 640; v.y = y1; v.u = u1; v.v = tv; pvr_prim(&v, sizeof(v));
        v.flags = PVR_CMD_VERTEX_EOL;
        v.x = 640; v.y = y0; v.u = u1; v.v = tv; pvr_prim(&v, sizeof(v));
    }
}
static void ex_dc_cutin(pvr_list_t list) {
    ExSubWork *s = data_ov001_0231f5a8;
    const float k = EX_CUTIN_K, ox = EX_CUTIN_OX, oy = EX_CUTIN_OY;
    u32 a = (u32)s_cutin_alpha << 24;
    int l, i;
    if (s_cutin_alpha <= 0 || s == NULL)
        return;
    if (s_vsbg[s->bg & 1].bytes && s_vs_tex[s->bg & 1] == NULL)
        ex_upload_indexed(&s_vsbg[s->bg & 1], s_vsbg[s->bg & 1].base, 512, 256, &s_vs_tex[s->bg & 1], 0);
    if (list == PVR_LIST_TR_POLY && s_vs_tex[s->bg & 1]) {
        int y0 = EX_CUTIN_Y0, y1 = EX_CUTIN_Y1;
        if ((s->win[12] & 1) && s->win[5] > y0)   /* window 0: the band that opens */
            y0 = s->win[5];
        if ((s->win[12] & 1) && s->win[4] < y1)
            y1 = s->win[4];
        /* colour 0 of the portrait BG (the sub screen's backdrop): a half-transparent black panel */
        if (y1 > y0)
            ex_quad(list, NULL, 0, 0, ox, oy + y0 * k, ox + 256 * k, oy + y1 * k, 0, 0, 0, 0, 9.9f,
                    (u32)(s_cutin_alpha / 2) << 24);
        for (l = y0; l < y1; l++) {
            float u = (s->unk2D8[s->unk5DE][l] & 0x1ff) / 512.0f, v = (l + 0.5f) / 256.0f;
            ex_quad(list, s_vs_tex[s->bg & 1], 512, 256, ox, oy + l * k, ox + 256 * k, oy + (l + 1) * k, u, v,
                    u + 0.5f, v, 10.0f, a | 0xffffff);
        }
    }
    if (list != PVR_LIST_TR_POLY)
        return;
    ex_sprres_upload(&g_ex_vs_sprites);
    for (i = 0; i < 8; i++) {
        ExSprItem q;
        const ExSprite *o = &s->unk98[i];
        int names = i < 4 && (o == &s->unk98[s->bg] || o == &s->unk98[s->bg + 2]);
        if (o->cur_image < 0 || !ex_sprres_ready(&g_ex_vs_sprites))
            continue;
        if (names && !((s->unk0 & 1) || s->unk4[2] != NULL))
            continue;
        if (!names && (i < 4 || !((s->unk0 & 2) || s->unk4[4] != NULL)))
            continue;
        q.res = o->res;
        q.image = o->cur_image;
        q.x = o->x;
        q.y = o->y;
        q.sx = names ? 1.0f : s->text_scale / 4096.0f;
        q.sy = names ? ((s->unk0 & 1) ? 1.0f : s->name_scale / 4096.0f) : s->text_scale / 4096.0f;
        q.rot = 0;
        q.flags = 0;
        ex_draw_sprite_item(list, &q, ox, oy, k, a | 0xffffff, 11.0f);
    }
}
void ex_draw_frame(int list) {
    int i;
    if (list == PVR_LIST_OP_POLY) {
        ex_cutin_update();
        if (ex_bg_visible())
            ex_dc_bg();
    }
    for (i = 0; i < s_nitems; i++) {
        const ExDrawItem *it = &s_items[i];
        dcm_camera_t cam;
        dcm_lights_t lights;
        dcm_draw_opts_t o;
        ex_item_camera(it, &cam);
        ex_view_lights(&cam, &lights);
        o.list = list;
        o.pt_list = PVR_LIST_PT_POLY;
        o.alpha_scale = 255;
        dcm_draw_ex(&it->m->res->mdl, it->m->palette, &it->mtx, &cam, it->has_vis ? it->vis : NULL, &lights, &o,
                    boss_model_matstates(it->m));
    }
    if (list == PVR_LIST_TR_POLY) {
        dcm_camera_t cam;
        ex_camera(s_trail_slot, &cam);
        for (i = 0; i < s_ntrail; i++) {
            /* project the four corners with the camera (z = 1 plane) */
            float sx[4], sy[4], sz[4];
            int k, ok = 1;
            pvr_poly_cxt_t cxt;
            pvr_poly_hdr_t hdr;
            pvr_vertex_t v __attribute__((aligned(32)));   /* pvr_prim: 8-byte aligned or dropped */
            for (k = 0; k < 4; k++) {
                const float *p = s_trail[i].v[k];
                float e[3], cx, cy, cw;
                int r;
                for (r = 0; r < 3; r++)
                    e[r] = p[0] * cam.view.m[0][r] + p[1] * cam.view.m[1][r] + p[2] * cam.view.m[2][r] + cam.view.m[3][r];
                cx = e[0] * cam.proj[0][0] + e[2] * cam.proj[2][0];
                cy = e[1] * cam.proj[1][1] + e[2] * cam.proj[2][1];
                cw = -e[2];
                if (cw < cam.near_w)
                    ok = 0;
                sx[k] = (cx / cw + 1) * 320;
                sy[k] = (1 - cy / cw) * 240;
                sz[k] = 1 / cw;
            }
            if (!ok)
                continue;
            pvr_poly_cxt_col(&cxt, PVR_LIST_TR_POLY);
            cxt.gen.culling = PVR_CULLING_NONE;
            pvr_poly_compile(&hdr, &cxt);
            pvr_prim(&hdr, sizeof(hdr));
            v.argb = s_trail[i].argb;
            v.oargb = 0;
            v.u = v.v = 0;
            for (k = 0; k < 4; k++) {
                v.flags = k == 3 ? PVR_CMD_VERTEX_EOL : PVR_CMD_VERTEX;
                v.x = sx[k];
                v.y = sy[k];
                v.z = sz[k];
                pvr_prim(&v, sizeof(v));
            }
        }
    }
    if (list == PVR_LIST_PT_POLY) {
        ex_sprres_upload(&gExtraBossRingSprite);
        ex_sprres_upload(&data_ov001_0231f320);
        /* OAM: the sprite drawn first is in front */
        for (i = 0; i < s_nspr; i++)
            if (ex_sprres_ready(s_spr[i].res))
                ex_draw_sprite_item(PVR_LIST_PT_POLY, &s_spr[i], 0, 0, 2.5f, 0xffffffffu, 6.0f - i * 0.001f);
    }
    if (list == PVR_LIST_TR_POLY) {
        ex_dc_cutin(PVR_LIST_TR_POLY);
        if (g_ex.bright_main != 0) {   /* master brightness: + white, - black */
            s32 b = g_ex.bright_main;
            u32 a = (u32)((b < 0 ? -b : b) * 255 / 16);
            if (a > 255)
                a = 255;
            ex_quad(PVR_LIST_TR_POLY, NULL, 0, 0, 0, 0, 640, 480, 0, 0, 0, 0, 100.0f,
                    (a << 24) | (b > 0 ? 0xffffffu : 0));
        }
    }
}
void ex_draw_shutdown(void) {
    if (s_bg_tex)
        pvr_mem_free(s_bg_tex);
    s_bg_tex = NULL;
    memset(s_cb, 0, sizeof(s_cb));
    ex_draw_reset();
}
#else
/* ====================================================== host (software) */
static u8 *s_fb;          /* 640x480 RGB */
static float *s_zb;
void boss_extra_host_target(u8 *rgb);
void boss_extra_host_target(u8 *rgb) { s_fb = rgb; }
static void px_blend(int x, int y, u32 c, u32 a) {
    u8 *p;
    if (x < 0 || y < 0 || x >= 640 || y >= 480)
        return;
    p = s_fb + 3 * (y * 640 + x);
    p[0] = (u8)((p[0] * (255 - a) + ((c >> 16) & 255) * a) / 255);
    p[1] = (u8)((p[1] * (255 - a) + ((c >> 8) & 255) * a) / 255);
    p[2] = (u8)((p[2] * (255 - a) + (c & 255) * a) / 255);
}
static u32 c555(u16 c) {
    return ((u32)(c & 31) * 255 / 31) << 16 | ((u32)((c >> 5) & 31) * 255 / 31) << 8 | (u32)((c >> 10) & 31) * 255 / 31;
}
static u32 argb1555(u16 c) {
    return (c & 0x8000 ? 0xff000000u : 0) | ((u32)((c >> 10) & 31) * 255 / 31) << 16 |
           ((u32)((c >> 5) & 31) * 255 / 31) << 8 | (u32)(c & 31) * 255 / 31;
}
static void host_bg(void) {
    ExBgWork *b = data_ov001_0231f54c;
    const s16 *hofs = b->unk5AC[b->unkB2C];
    int x, y;
    u32 back = c555(b->pal[0]);
    for (y = 0; y < 480; y++) {
        int l = y * 2 / 5, sy = (l + b->vofs) & 0xff;
        for (x = 0; x < 640; x++) {
            int sx = (x * 2 / 5 + hofs[l]) & 0xff;
            u8 i = g_ex_bg.px[sy * g_ex_bg.w + sx];
            u32 c = (i & 15) ? c555(b->pal[i]) : back;
            u8 *p = s_fb + 3 * (y * 640 + x);
            p[0] = (u8)(c >> 16);
            p[1] = (u8)(c >> 8);
            p[2] = (u8)c;
        }
    }
}
static u32 tex_sample(const dcm_model_t *m, int ti, float u, float v, u16 flags) {
    const dcm_texture_t *t = &m->textures[ti];
    const u16 *pix = (const u16 *)(m->base + t->ofs);
    int x = (int)floorf(u * t->width), y = (int)floorf(v * t->height);
    u16 c;
    if (flags & DCM_MF_REPEAT_S) {
        if ((flags & DCM_MF_FLIP_S) && ((x / t->width) & 1))
            x = t->width - 1 - (((x % t->width) + t->width) % t->width);
        else
            x = ((x % t->width) + t->width) % t->width;
    } else
        x = x < 0 ? 0 : x >= t->width ? t->width - 1 : x;
    if (flags & DCM_MF_REPEAT_T) {
        if ((flags & DCM_MF_FLIP_T) && ((y / t->height) & 1))
            y = t->height - 1 - (((y % t->height) + t->height) % t->height);
        else
            y = ((y % t->height) + t->height) % t->height;
    } else
        y = y < 0 ? 0 : y >= t->height ? t->height - 1 : y;
    c = pix[y * t->width + x];
    switch (t->format) {
    case DCM_TEX_RGB565:
        return 0xff000000u | ((u32)(c >> 11) * 255 / 31) << 16 | ((u32)((c >> 5) & 63) * 255 / 63) << 8 |
               (u32)(c & 31) * 255 / 31;
    case DCM_TEX_ARGB4444:
        return ((u32)(c >> 12) * 17) << 24 | ((u32)((c >> 8) & 15) * 17) << 16 | ((u32)((c >> 4) & 15) * 17) << 8 |
               (u32)(c & 15) * 17;
    default:
        return argb1555(c);
    }
}
static float s_xyz[3 * 8192];
static void host_model(const ExDrawItem *it) {
    const dcm_model_t *m = &it->m->res->mdl;
    dcm_camera_t cam;
    u32 d;
    if (m->hdr->num_vertices > 8192)
        return;
    ex_item_camera(it, &cam);
    dcm_transform_vertices(m, it->m->palette, &it->mtx, &cam, s_xyz);
    for (d = 0; d < m->hdr->num_draws; d++) {
        const dcm_draw_t *dr = &m->draws[d];
        const dcm_material_t *mat = &m->materials[dr->material];
        u32 s;
        float shade[3];
        int k;
        if (dr->vis_node != 0xFFFF) {
            int visible = it->has_vis ? it->vis[dr->vis_node] : dr->vis_default;
            if (!visible)
                continue;
        }
        for (k = 0; k < 3; k++) {   /* lighting approximation: emission + ambient + 0.6 diffuse */
            int sh = 16 - 8 * k;
            float e = ((mat->emission >> sh) & 255) / 255.0f, a = ((mat->ambient >> sh) & 255) / 255.0f,
                  df = ((mat->diffuse >> sh) & 255) / 255.0f;
            shade[k] = mat->light_mask ? e + 0.55f * (a + df) : df;
            if (shade[k] > 1)
                shade[k] = 1;
        }
        for (s = dr->first_strip; s < dr->first_strip + dr->num_strips; s++) {
            const dcm_strip_t *st = &m->strips[s];
            int t;
            for (t = 0; t + 2 < st->count; t++) {
                int i0 = (int)st->first_vertex + t, i1 = i0 + 1, i2 = i0 + 2, idx[3] = {i0, i1, i2};
                float x[3], y[3], z[3], u[3], v[3];
                float minx, maxx, miny, maxy, area;
                int px, py, j;
                for (j = 0; j < 3; j++) {
                    const dcm_vertex_t *vv = &m->vertices[idx[j]];
                    x[j] = s_xyz[3 * idx[j]];
                    y[j] = s_xyz[3 * idx[j] + 1];
                    z[j] = s_xyz[3 * idx[j] + 2];
                    u[j] = mat->uv_a * vv->u + mat->uv_e;
                    v[j] = mat->uv_d * vv->v + mat->uv_f;
                }
                if (z[0] <= 0 || z[1] <= 0 || z[2] <= 0)
                    continue;
                area = (x[1] - x[0]) * (y[2] - y[0]) - (x[2] - x[0]) * (y[1] - y[0]);
                if (fabsf(area) < 1e-6f)
                    continue;
                minx = fminf(x[0], fminf(x[1], x[2]));
                maxx = fmaxf(x[0], fmaxf(x[1], x[2]));
                miny = fminf(y[0], fminf(y[1], y[2]));
                maxy = fmaxf(y[0], fmaxf(y[1], y[2]));
                if (maxx < 0 || minx > 639 || maxy < 0 || miny > 479 || maxx - minx > 2000 || maxy - miny > 2000)
                    continue;
                for (py = (int)fmaxf(0, miny); py <= (int)fminf(479, maxy); py++)
                    for (px = (int)fmaxf(0, minx); px <= (int)fminf(639, maxx); px++) {
                        float fx = px + 0.5f, fy = py + 0.5f;
                        float w0 = ((x[1] - fx) * (y[2] - fy) - (x[2] - fx) * (y[1] - fy)) / area;
                        float w1 = ((x[2] - fx) * (y[0] - fy) - (x[0] - fx) * (y[2] - fy)) / area;
                        float w2 = 1 - w0 - w1;
                        float zz;
                        u32 c = 0xffffffffu, a;
                        if (w0 < 0 || w1 < 0 || w2 < 0)
                            continue;
                        zz = w0 * z[0] + w1 * z[1] + w2 * z[2];
                        if (zz <= s_zb[py * 640 + px])
                            continue;
                        if (mat->texture != 0xFFFF && mat->texture < m->hdr->num_textures) {
                            float uu = (w0 * u[0] * z[0] + w1 * u[1] * z[1] + w2 * u[2] * z[2]) / zz;
                            float vv = (w0 * v[0] * z[0] + w1 * v[1] * z[1] + w2 * v[2] * z[2]) / zz;
                            c = tex_sample(m, mat->texture, uu, vv, mat->flags);
                            if ((c >> 24) < 8)
                                continue;
                        }
                        a = (c >> 24) * mat->alpha / 31;
                        c = (u32)(((c >> 16) & 255) * shade[0]) << 16 | (u32)(((c >> 8) & 255) * shade[1]) << 8 |
                            (u32)((c & 255) * shade[2]);
                        if (!(mat->flags & DCM_MF_TRANSLUCENT) && a > 0)
                            s_zb[py * 640 + px] = zz;
                        px_blend(px, py, c, a > 255 ? 255 : a);
                    }
            }
        }
    }
}
static void host_sprite(const ExSprItem *q, float ox, float oy, float k, u32 alpha) {
    const ExSprImage *im = &q->res->images[q->image];
    int x, y;
    float iox = (q->flags & 0x80) ? im->w - im->ox : im->ox;
    float x0 = ox + (q->x - iox * q->sx) * k, y0 = oy + (q->y - im->oy * q->sy) * k;
    float w = im->w * q->sx * k, h = im->h * q->sy * k;
    if (w <= 0 || h <= 0)
        return;
    for (y = (int)y0; y < (int)(y0 + h); y++)
        for (x = (int)x0; x < (int)(x0 + w); x++) {
            int sx = (int)((x + 0.5f - x0) / w * im->w), sy = (int)((y + 0.5f - y0) / h * im->h);
            u16 c;
            if (q->flags & 0x80)
                sx = im->w - 1 - sx;
            if (sx < 0 || sy < 0 || sx >= im->w || sy >= im->h)
                continue;
            c = q->res->pixels[(im->y + sy) * q->res->page_w + im->x + sx];
            if (c & 0x8000)
                px_blend(x, y, argb1555(c), alpha);
        }
}
static void host_trail(void) {
    dcm_camera_t cam;
    int i;
    ex_camera(s_trail_slot, &cam);
    for (i = 0; i < s_ntrail; i++) {
        float sx[4], sy[4];
        int k, t;
        for (k = 0; k < 4; k++) {
            const float *p = s_trail[i].v[k];
            float e[3], cw;
            int r;
            for (r = 0; r < 3; r++)
                e[r] = p[0] * cam.view.m[0][r] + p[1] * cam.view.m[1][r] + p[2] * cam.view.m[2][r] + cam.view.m[3][r];
            cw = -e[2];
            sx[k] = ((e[0] * cam.proj[0][0] + e[2] * cam.proj[2][0]) / cw + 1) * 320;
            sy[k] = (1 - (e[1] * cam.proj[1][1] + e[2] * cam.proj[2][1]) / cw) * 240;
        }
        for (t = 0; t < 2; t++) {   /* strip: (0, 1, 2), (1, 2, 3) */
            float *X = &sx[t], *Y = &sy[t];
            float area = (X[1] - X[0]) * (Y[2] - Y[0]) - (X[2] - X[0]) * (Y[1] - Y[0]);
            int px, py;
            if (fabsf(area) < 1e-6f)
                continue;
            for (py = (int)fmaxf(0, fminf(Y[0], fminf(Y[1], Y[2]))); py <= (int)fminf(479, fmaxf(Y[0], fmaxf(Y[1], Y[2]))); py++)
                for (px = (int)fmaxf(0, fminf(X[0], fminf(X[1], X[2]))); px <= (int)fminf(639, fmaxf(X[0], fmaxf(X[1], X[2]))); px++) {
                    float fx = px + 0.5f, fy = py + 0.5f;
                    float w0 = ((X[1] - fx) * (Y[2] - fy) - (X[2] - fx) * (Y[1] - fy)) / area;
                    float w1 = ((X[2] - fx) * (Y[0] - fy) - (X[0] - fx) * (Y[2] - fy)) / area;
                    if (w0 < 0 || w1 < 0 || 1 - w0 - w1 < 0)
                        continue;
                    px_blend(px, py, s_trail[i].argb, s_trail[i].argb >> 24);
                }
        }
    }
}
static void host_cutin(void) {
    ExSubWork *s = data_ov001_0231f5a8;
    const float k = EX_CUTIN_K, ox = EX_CUTIN_OX, oy = EX_CUTIN_OY;
    int i, x, y;
    if (s_cutin_alpha <= 0 || s == NULL)
        return;
    if (s_vsbg[s->bg & 1].bytes) {
        const ExBgImage *img = &s_vsbg[s->bg & 1];
        int y0 = EX_CUTIN_Y0, y1 = EX_CUTIN_Y1;
        if ((s->win[12] & 1) && s->win[5] > y0)
            y0 = s->win[5];
        if ((s->win[12] & 1) && s->win[4] < y1)
            y1 = s->win[4];
        for (y = (int)(oy + y0 * k); y < (int)(oy + y1 * k); y++) {
            int l = (int)((y - oy) / k);
            for (x = (int)ox; x < (int)(ox + 256 * k); x++) {
                int sx = ((int)((x - ox) / k) + s->unk2D8[s->unk5DE][l]) & 0x1ff;
                u8 i8 = img->px[l * img->w + sx];
                if (i8 & 15)
                    px_blend(x, y, c555(img->base[i8]), (u32)s_cutin_alpha);
                else
                    px_blend(x, y, 0, (u32)s_cutin_alpha / 2);
            }
        }
    }
    for (i = 0; i < 8; i++) {
        ExSprItem q;
        const ExSprite *o = &s->unk98[i];
        int names = i < 4 && (i == s->bg || i == s->bg + 2);
        if (o->cur_image < 0 || !g_ex_vs_sprites.loaded)
            continue;
        if (names && !((s->unk0 & 1) || s->unk4[2] != NULL))
            continue;
        if (!names && (i < 4 || !((s->unk0 & 2) || s->unk4[4] != NULL)))
            continue;
        q.res = o->res;
        q.image = o->cur_image;
        q.x = o->x;
        q.y = o->y;
        q.sx = names ? 1.0f : s->text_scale / 4096.0f;
        q.sy = names ? ((s->unk0 & 1) ? 1.0f : s->name_scale / 4096.0f) : s->text_scale / 4096.0f;
        q.rot = 0;
        q.flags = 0;
        host_sprite(&q, ox, oy, k, (u32)s_cutin_alpha);
    }
}
void ex_draw_frame(int list) {
    int i;
    (void)list;
    ex_cutin_update();
    if (s_fb == NULL)
        return;
    if (s_zb == NULL)
        s_zb = (float *)malloc(640 * 480 * sizeof(float));
    memset(s_fb, 0, 640 * 480 * 3);
    for (i = 0; i < 640 * 480; i++)
        s_zb[i] = 0;
    if (ex_bg_visible())
        host_bg();
    for (i = 0; i < s_nitems; i++)
        host_model(&s_items[i]);
    host_trail();
    for (i = s_nspr - 1; i >= 0; i--)   /* OAM: the sprite drawn first is in front */
        host_sprite(&s_spr[i], 0, 0, 2.5f, 255);
    host_cutin();
    if (g_ex.bright_main != 0) {
        s32 b = g_ex.bright_main;
        u32 a = (u32)((b < 0 ? -b : b) * 255 / 16);
        int x, y;
        if (a > 255)
            a = 255;
        for (y = 0; y < 480; y++)
            for (x = 0; x < 640; x++)
                px_blend(x, y, b > 0 ? 0xffffff : 0, a);
    }
}
void ex_draw_shutdown(void) {
    memset(s_cb, 0, sizeof(s_cb));
    ex_draw_reset();
}
/* test access */
int boss_extra_host_items(void);
int boss_extra_host_items(void) { return s_nitems; }
#endif
