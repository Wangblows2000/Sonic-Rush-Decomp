/*
 * ExtraAct_* (src/game/Zone/Extra_Zone/Results, 37 functions): the Extra Zone
 * results. After a white fade-in (BrightnessLerp 0x10 -> 0) the finish model
 * (eff_dmg_end02, joint animation) plays over the static eff_dmg_end01; then the
 * letterbox (BG1 outside WIN0 / WIN1) closes in with its blue edge sprites, the
 * title "SONIC & BLAZE" scales in letter by letter, "EXTRA ZONE CLEAR" slides in,
 * TIME BONUS / RING BONUS slide in, the TOTAL rolls, the rank stamps (zoom, shake),
 * and after 0x78 frames the screen fades to white (SceneChange_ToExtraEnding).
 * A skips each step (ExtraAct_Main) as on the DS.
 *
 * DS service                          here
 * data_ov001_0231f084 (the work)      s_ex
 * NNS G3D (Model_Init / SetResource /  model/ (dcm_*): the converted packages; camera
 *   SetAnimation / Animate / Draw)    G3i_PerspectiveW(sin, cos of 0x1555, aspect 0x1555,
 *                                     near 1, far 2048) + LookAt (0,0,100) -> (0,0,0); the
 *                                     three lights of ExtraAct_Start. The material
 *                                     animation (eff_dmg_end02.nsbma) is not converted
 * BG1 (ex_cldm_bg_pal: tiles of colour 2 (top half) / 0x12 (bottom half)) outside
 *   WIN0 / WIN1 (data_0208923c)       two flat bands of those colours, rows [0, v) and
 *                                     [0xc0 - v, 0xc0) as the windows give
 * BrightnessLerp_Create / _Main       a task on g_mn_bright (both screens)
 * Sound_PlaySeqArc(0, 8, n) / Sound_PlaySeq(0, 0x12) / NNS_SndPlayerStopSeq
 *                                     g_menu_hooks.se(8, n) / bgm(0x12) / (the roll SE
 *                                     is not stopped: no handle in the hooks)
 * ObjVram_Alloc / OBJ palette rows     not needed / not applied (converted atlas colours)
 */
#include <math.h>
#include <stdlib.h>
#include "mn_internal.h"
#include "extraact.h"
#include "../model/model.h"
#ifdef _arch_dreamcast
#include <kos.h>
#include "../engine/render.h"
#endif

typedef struct {
    u16 spr[39], pad;
    u16 gw[16], txt_b[16], txt_t[14];
    s32 rows[2];
    s32 bonus[10];
    u32 thr[10];
    s32 cam_pos[3], cam_up[3];
    u16 bgpal[256];
    u32 rank_boss[4], rank_norm[4];
} ExData;

typedef struct {
    int ok;
    dcm_model_t mdl;
    u8 *blob;
    dca_set_t anims;
    u8 *anim_blob;
    float frame;      /* +0xe8: frame of the joint animation */
    s32 speed;        /* +0x114: 0x1000 = 1 frame per frame */
    u32 flags;        /* +0x10c: 2 = the animation ended */
} ExModel;

typedef struct {
    MnSpr objs[0x28];     /* +0x18: 39 sprites + the rank stamp (+0xb10) */
    ExModel model_b58;    /* eff_dmg_end01 */
    ExModel model_c90;    /* eff_dmg_end02 */
    MnTask *tasks[7];     /* +0xe08 Main, +0xe0c 9134, +0xe10 8e7c, +0xe14 8a40, +0xe18 8774, +0xe1c tally, +0xe20 rank */
    s32 unkE24, unkE28, unkE2C, unkE30;   /* time bonus, ring bonus, total, rank */
    s32 unkE34;
    u32 unkE38;           /* flags */
    u16 unkE3C;           /* BrightnessLerp done */
    s32 unkE40;
    s32 unkE44;
    s16 unkE48;
    s32 unkE4C, unkE50, unkE54, unkE58;
    s16 unkE5C;
    s16 dig[3][6];        /* +0xe5e */
    u16 digits[6];        /* +0xe82 rolling digits */
    s32 unkE90;
    s16 unkE94[2];
    s16 unkE98;
    s16 unkE9A;
    s32 unkE9C;
    /* the windows (data_0208923c) */
    u8 win[12];
    int bg1_on;           /* DISPCNT BG1 (ExtraAct_Unk_ov001_022f8c78 SetVisible(2)) */
    MnRes *res;
    int done;
} ExWork;

static ExWork *s_ex;
static ExData s_exd;
static char s_model_dir[96] = "/cd/models";
#define G s_ex

void extraact_set_model_dir(const char *dir) {
    if (dir)
        snprintf(s_model_dir, sizeof(s_model_dir), "%s", dir);
}

static void ex_se(s32 id) {
    if (g_menu_hooks.se)
        g_menu_hooks.se(8, id);   /* Sound_PlaySeqArc(0, 8, id) */
}

/* ------------------------------------------------------------- BrightnessLerp */
typedef struct {
    s32 count, total;
    s16 from, to;
    u32 flags;
    u16 *done;
} ExLerpWork;
static void BrightnessLerp_Main(void) {
    ExLerpWork *w = TASK_WORK(ExLerpWork);
    s16 b;
    if (w->done != NULL && *w->done != 0) {
        Task_Destroy(gTaskCurrent);
        return;
    }
    w->count++;
    if (w->count >= w->total) {
        if (w->done != NULL)
            *w->done = 1;
        if (w->flags & 1)
            BRIGHT_MAIN = w->to;
        if (w->flags & 2)
            BRIGHT_SUB = w->to;
        Task_Destroy(gTaskCurrent);
        return;
    }
    b = (s16)((u32)(w->count * w->to + w->from * (w->total - w->count)) / (u32)w->total);
    if (w->flags & 1)
        BRIGHT_MAIN = b;
    if (w->flags & 2)
        BRIGHT_SUB = b;
}
static void BrightnessLerp_Create(s32 total, s16 from, s16 to, u32 flags, u16 *done) {
    ExLerpWork *w = (ExLerpWork *)Task_Create(BrightnessLerp_Main, NULL, 0, 0x4000, 0, sizeof(ExLerpWork))->work;
    w->total = total;
    w->from = from;
    w->to = to;
    w->flags = flags & 3;
    if (done != NULL) {
        *done = 0;
        w->done = done;
    }
}

/* ---------------------------------------------------------------- models */
static int model_load(ExModel *m, const char *name, const char *anim) {
    char path[200];
    u32 size;
    memset(m, 0, sizeof(*m));
    snprintf(path, sizeof(path), "%s/%s", s_model_dir, name);
    if (mn_load_file(path, &m->blob, &size) != 0) {
        fprintf(stderr, "extraact: missing %s\n", path);
        return -1;
    }
    if (dcm_model_load(&m->mdl, m->blob, size,
#ifdef _arch_dreamcast
                       1
#else
                       0
#endif
                       ) != 0) {
        free(m->blob);
        m->blob = NULL;
        return -1;
    }
    if (anim) {
        snprintf(path, sizeof(path), "%s/%s", s_model_dir, anim);
        if (mn_load_file(path, &m->anim_blob, &size) == 0 && dca_set_load(&m->anims, m->anim_blob, size) != 0) {
            free(m->anim_blob);
            m->anim_blob = NULL;
        }
    }
    m->ok = 1;
    return 0;
}
static void Model_Release(ExModel *m) {
    if (m->ok)
        dcm_model_free(&m->mdl);
    free(m->blob);
    free(m->anim_blob);
    memset(m, 0, sizeof(*m));
}
static float model_frames(const ExModel *m) {
    if (!m->anim_blob || !m->anims.hdr || m->anims.hdr->num_anims == 0)
        return 0;
    return (float)m->anims.anims[0].num_frames;
}
/* Model_Animate: the joint animation advances speed / 0x1000 frames; at its end
 * (non-looping) flag 2 is set */
static void Model_Animate(ExModel *m) {
    float n = model_frames(m);
    if (n <= 0) {
        m->flags |= 2;
        return;
    }
    m->frame += m->speed / 4096.0f;
    if (m->frame >= n - 1) {
        m->frame = n - 1;
        m->flags |= 2;
    }
}

/* the camera and lights of ExtraAct_Start */
static void ex_camera(dcm_camera_t *c) {
    float eye[3], up[3], z[3], x[3], y[3], t, r, n = 1.0f, f = 2048.0f, l;
    double a = 0x1555 * 2.0 * 3.14159265358979 / 65536.0;
    int i;
    memset(c, 0, sizeof(*c));
    for (i = 0; i < 3; i++) {
        eye[i] = s_exd.cam_pos[i] / 4096.0f;
        up[i] = s_exd.cam_up[i] / 4096.0f;
    }
    /* target (0, 0, 0) */
    for (i = 0; i < 3; i++)
        z[i] = eye[i];
    l = sqrtf(z[0] * z[0] + z[1] * z[1] + z[2] * z[2]);
    if (l == 0)
        l = 1;
    for (i = 0; i < 3; i++)
        z[i] /= l;
    x[0] = up[1] * z[2] - up[2] * z[1];
    x[1] = up[2] * z[0] - up[0] * z[2];
    x[2] = up[0] * z[1] - up[1] * z[0];
    l = sqrtf(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);
    if (l == 0)
        l = 1;
    for (i = 0; i < 3; i++)
        x[i] /= l;
    y[0] = z[1] * x[2] - z[2] * x[1];
    y[1] = z[2] * x[0] - z[0] * x[2];
    y[2] = z[0] * x[1] - z[1] * x[0];
    dcm_mtx_identity(&c->view);
    for (i = 0; i < 3; i++) {
        c->view.m[i][0] = x[i];
        c->view.m[i][1] = y[i];
        c->view.m[i][2] = z[i];
    }
    c->view.m[3][0] = -(x[0] * eye[0] + x[1] * eye[1] + x[2] * eye[2]);
    c->view.m[3][1] = -(y[0] * eye[0] + y[1] * eye[1] + y[2] * eye[2]);
    c->view.m[3][2] = -(z[0] * eye[0] + z[1] * eye[1] + z[2] * eye[2]);
    t = n * (float)(sin(a) / cos(a));
    r = t * (0x1555 / 4096.0f);
    c->proj[0][0] = n / r;
    c->proj[1][1] = n / t;
    c->proj[2][2] = (f + n) / (n - f);
    c->proj[2][3] = -1.0f;
    c->proj[3][2] = 2 * f * n / (n - f);
    c->vp_x = 0;
    c->vp_y = 0;
    c->vp_w = 640;
    c->vp_h = 480;
    c->near_w = n * 0.5f;
}
static void ex_lights(dcm_lights_t *L, const dcm_camera_t *c) {
    static const u16 col[3] = { 0x7fff, 0x4631, 0x2108 };
    static const s16 vec[3][3] = { { 0x4a0, -0x4a0, 0 }, { 0, -0x617, -0xecd }, { 0xec7, 0x620, 0 } };
    int i, k;
    memset(L, 0, sizeof(*L));
    L->count = 3;
    for (i = 0; i < 3; i++) {
        float d[3] = { vec[i][0] / 4096.0f, vec[i][1] / 4096.0f, vec[i][2] / 4096.0f };
        for (k = 0; k < 3; k++)
            L->dir[i][k] = d[0] * c->view.m[0][k] + d[1] * c->view.m[1][k] + d[2] * c->view.m[2][k];
        L->col[i][0] = (col[i] & 31) / 31.0f;
        L->col[i][1] = (col[i] >> 5 & 31) / 31.0f;
        L->col[i][2] = (col[i] >> 10 & 31) / 31.0f;
    }
}

/* the two models drawn this frame (Model_Draw calls) */
static ExModel *s_draw[2];
static int s_ndraw;
static void Model_Draw(ExModel *m) {
    if (m->ok && s_ndraw < 2)
        s_draw[s_ndraw++] = m;
}
static void pose(const ExModel *m, dcm_mtx_t *pal) {
    static dcm_local_t loc[DCM_MAX_JOINTS];
    if (m->anim_blob && m->anims.hdr && m->anims.hdr->num_anims > 0)
        dca_sample(&m->anims, 0, m->frame, &m->mdl, loc);
    else
        dcm_bind_locals(&m->mdl, loc);
    dcm_pose(&m->mdl, loc, pal);
}

/* ---------------------------------------------------------------- tally */
static s32 ExtraAct_GetTimeBonus(u32 v) {
    s32 i, r = 0;
    for (i = 0; i < 10; i++) {
        if (s_exd.thr[i] >= v) {
            r = s_exd.bonus[i];
            break;
        }
    }
    return r;
}
/* ClearDemo_GetRank (gGameState zone 8 act 2: the boss thresholds) */
static s16 ClearDemo_GetRank(u32 v) {
    s16 i;
    const u32 *t = (gGameState.unk1C >= 9 || gGameState.unk1D >= 2) ? s_exd.rank_boss : s_exd.rank_norm;
    for (i = 0; i < 4; t++, i++)
        if (v >= *t)
            break;
    return i;
}

static void draw_at(MnSpr *o, s32 x, s32 y) {
    o->x = (s16)x;
    o->y = (s16)y;
    AnimatorSprite_Draw(o);
}

/* the TIME BONUS / RING BONUS rows and the line */
static void ExtraAct_Unk_ov001_022f7790(void) {
    s32 k, i, x, y;
    for (k = 0, y = 0x54; k <= 1; k++, y += 0x10) {
        draw_at(&G->objs[s_exd.rows[k]], 0x56, y);
        for (i = 0, x = 0x60; i < 6; i++, x += 10) {
            s32 d = G->dig[k][i];
            if (d != -1)
                draw_at(&G->objs[d], x, y);
        }
    }
    draw_at(&G->objs[23], 0x96, 0x6c);
}
/* TOTAL with its final digits */
static void ExtraAct_State_Unk_ov001_022f7880(void) {
    s32 i, x;
    draw_at(&G->objs[0x16], 0x56, 0x7c);
    for (i = 0, x = 0x60; i < 6; i++, x += 0xa) {
        s16 c = G->dig[2][i];
        if (c != -1)
            draw_at(&G->objs[c + 0xa], x, 0x7c);
    }
    ExtraAct_Unk_ov001_022f7790();
}
/* TOTAL: the digits settle from the left, one every 16 frames */
static void ExtraAct_State_Unk_ov001_022f7924(void) {
    s32 n, i, lim, x;
    if (G->unkE38 & 3) {
        gTaskCurrent->func = ExtraAct_State_Unk_ov001_022f7880;
        ExtraAct_State_Unk_ov001_022f7880();
        G->unkE38 |= 0x4000;
        /* NNS_SndPlayerStopSeq(unkEA0) */
        ex_se(2);
        return;
    }
    G->unkE90++;
    n = G->unkE90 / 0x10 + 1;
    if (G->unkE90 & 1)
        for (i = 0; i < 6; i++)
            G->digits[i] = (u16)(mn_rand() % 10);
    draw_at(&G->objs[0x16], 0x56, 0x7c);
    lim = 6 - n;
    for (i = 0, x = 0x60; i < lim; i++, x += 10)
        draw_at(&G->objs[G->digits[i] + 10], x, 0x7c);
    for (; i < 6; i++, x += 10)
        if (G->dig[2][i] != -1)
            draw_at(&G->objs[G->dig[2][i] + 10], x, 0x7c);
    if (n >= 6) {
        gTaskCurrent->func = ExtraAct_State_Unk_ov001_022f7880;
        G->unkE38 |= 1;
        G->unkE38 |= 0x4000;
        ex_se(2);
    }
    ExtraAct_Unk_ov001_022f7790();
}
/* TOTAL: random digits for 0x2d frames (the roll SE) */
static void ExtraAct_State_Unk_ov001_022f7b94(void) {
    s32 i, x;
    if (G->unkE38 & 3) {
        gTaskCurrent->func = ExtraAct_State_Unk_ov001_022f7880;
        ExtraAct_State_Unk_ov001_022f7880();
        G->unkE38 |= 0x4000;
        ex_se(2);
        return;
    }
    G->unkE90--;
    if (G->unkE90 & 1)
        for (i = 0; i < 6; i++)
            G->digits[i] = (u16)(mn_rand() % 10);
    draw_at(&G->objs[0x16], 0x56, 0x7c);
    for (i = 0, x = 0x60; i < 6; i++, x += 10)
        draw_at(&G->objs[G->digits[i] + 10], x, 0x7c);
    if (G->unkE90 <= 0) {
        gTaskCurrent->func = ExtraAct_State_Unk_ov001_022f7924;
        G->unkE90 = 0;
    }
    ExtraAct_Unk_ov001_022f7790();
}
static void ExtraAct_State_Unk_ov001_022f7d64(void) {
    if (G->unkE38 & 3) {
        gTaskCurrent->func = ExtraAct_State_Unk_ov001_022f7880;
        ExtraAct_State_Unk_ov001_022f7880();
        G->unkE38 |= 0x4000;
        return;
    }
    G->unkE90 -= 1;
    if (G->unkE90 <= 0) {
        gTaskCurrent->func = ExtraAct_State_Unk_ov001_022f7b94;
        G->unkE90 = 0x2d;
        ex_se(1);   /* on the unkEA0 handle */
    }
    ExtraAct_Unk_ov001_022f7790();
}
/* the two rows slide in (30 frames apart), then the line */
static void ExtraAct_Unk_ov001_022f7e20(void) {
    s32 n, i, j, acc, k, t;
    s16 yy;
    if (G->unkE38 & 3) {
        gTaskCurrent->func = ExtraAct_State_Unk_ov001_022f7880;
        ExtraAct_State_Unk_ov001_022f7880();
        G->unkE38 |= 0x4000;
        return;
    }
    G->unkE90++;
    n = G->unkE90 / 0x1e + 1;
    if (n >= 3)
        n = 3;
    for (i = 0; i < n; i++) {
        s16 *v = i < 2 ? &G->unkE94[i] : &G->unkE98;   /* unkE94[2] is unkE98 */
        if (*v != 0x1000) {
            *v = (s16)(*v + 0x100);
            if (*v >= 0x1000) {
                *v = 0x1000;
                ex_se(0);
            }
        }
    }
    for (i = 0, yy = 0x54; i <= 1; i++, yy += 0x10) {
        s32 v = G->unkE94[i];
        if (v != 0) {
            k = 5;
            acc = -0xc8;
            do {
                acc += FX_Mul(-acc, v);
            } while (k-- != 0);
            draw_at(&G->objs[s_exd.rows[i]], acc + 0x56, yy);
            acc = (s16)(acc + 0x60);
            for (j = 0; j < 6; j++, acc = (s16)(acc + 10)) {
                t = G->dig[i][j];
                if (t != -1)
                    draw_at(&G->objs[t], acc, yy);
            }
        }
    }
    if (G->unkE98 != 0) {
        k = 5;
        acc = -0xc8;
        do {
            acc += FX_Mul(-acc, G->unkE98);
        } while (k-- != 0);
        draw_at(&G->objs[23], acc + 0x96, 0x6c);
    }
    if (G->unkE98 >= 0x1000) {
        gTaskCurrent->func = ExtraAct_State_Unk_ov001_022f7d64;
        G->unkE90 = 0x3c;
    }
}

static void set_digits(s32 v, s32 k) {
    s32 i, seen = 0, div = 100000, d;
    for (i = 0; i < 6; i++) {
        d = (s16)(v / div);
        if (d == 0) {
            if (!seen && i < 5)
                d = -1;
        } else {
            seen = 1;
        }
        G->dig[k][i] = (s16)d;
        v = v % div;
        div /= 10;
    }
}
static void ExtraAct_StartScoreTally(void) {
    set_digits(G->unkE24, 0);
    set_digits(G->unkE28, 1);
    set_digits(G->unkE2C, 2);
    G->tasks[5] = Task_Create(ExtraAct_Unk_ov001_022f7e20, NULL, 0, 0x2000, 0, 0);
}

/* ------------------------------------------------------------- rank stamp */
static void ExtraAct_State_RankStampShow(void) { draw_at(&G->objs[0x27], 0x50, 0x8a); }
static void ExtraAct_State_RankStampShake(void) {
    s32 r, x, y;
    if (G->unkE38 & 2) {
        gTaskCurrent->func = ExtraAct_State_RankStampShow;
        ExtraAct_State_RankStampShow();
        G->unkE38 &= ~0x10000u;
        G->unkE38 |= 0x20000;
        return;
    }
    G->unkE9C--;
    if (G->unkE9C <= 0) {
        gTaskCurrent->func = ExtraAct_State_RankStampShow;
        G->unkE38 |= 2;
        G->unkE38 |= 0x10000;
    }
    r = G->unkE9C > 0 ? 0x5000 / (G->unkE9C << 4) : 0;
    if (r == 0) {
        x = 0x50000;
        y = 0x8a000;
    } else {
        x = (s32)(mn_rand() % r) + 0x50000 - (r >> 1);
        y = (s32)(mn_rand() % r) + 0x8a000 - (r >> 1);
    }
    draw_at(&G->objs[0x27], x >> 12, y >> 12);
}
static void ExtraAct_State_RankStampZoom(void) {
    s32 i, s, t;
    if (G->unkE38 & 2) {
        gTaskCurrent->func = ExtraAct_State_RankStampShow;
        ExtraAct_State_RankStampShow();
        G->unkE38 &= ~0x10000u;
        G->unkE38 |= 0x20000;
        return;
    }
    G->unkE9A = (s16)(G->unkE9A + 0x100);
    if (G->unkE9A >= 0x1000) {
        gTaskCurrent->func = ExtraAct_State_RankStampShake;
        G->unkE9C = 0x10;
        ex_se(3);
    }
    i = 5;
    s = 0x1000;
    t = G->unkE9A;
    do {
        s = FX_Mul(s - 0x2000, t) + 0x2000;
    } while (i-- != 0);
    G->objs[0x27].x = 0x50;
    G->objs[0x27].y = 0x8a;
    AnimatorSprite_DrawScaled(&G->objs[0x27], s, s);
}
static void ExtraAct_RankStampDelay(void) {
    G->unkE9C--;
    if (G->unkE9C <= 0)
        gTaskCurrent->func = ExtraAct_State_RankStampZoom;
}
static void ExtraAct_StartRankStamp(void) {
    G->tasks[6] = Task_Create(ExtraAct_RankStampDelay, NULL, 0, 0x1e00, 0, 0);
    G->unkE9C = 0x3c;
}

/* ------------------------------------------------------------- captions */
/* "EXTRA ZONE CLEAR" at y 0xb4 */
static void ExtraAct_State_Unk_ov001_022f8330(void) {
    s32 i, x;
    for (i = 0, x = 0x24; i < 0x10; i++) {
        u16 c = s_exd.txt_b[i];
        if (c != 0xe)
            draw_at(&G->objs[c + 0x18], x, 0xb4);
        x += s_exd.gw[c & 15];
    }
}
static void ExtraAct_State_Unk_ov001_022f83c0(void) {   /* the letters spread to their places */
    s32 i, x;
    if (G->unkE38 & 3) {
        gTaskCurrent->func = ExtraAct_State_Unk_ov001_022f8330;
        ExtraAct_State_Unk_ov001_022f8330();
        G->unkE38 &= ~0x1000u;
        G->unkE38 |= 0x2000;
        return;
    }
    G->unkE54 += 0x2000;
    for (i = 0, x = 0x24; i < 0x10; i++) {
        u16 c = s_exd.txt_b[i];
        s32 v = (G->unkE54 << 4) >> 16;
        if (x < v)
            v = x;
        if (c != 0xe)
            draw_at(&G->objs[c + 0x18], v, 0xb4);
        x += s_exd.gw[c & 15];
    }
    if (x < G->unkE54 >> 12) {
        gTaskCurrent->func = ExtraAct_State_Unk_ov001_022f8330;
        G->unkE38 |= 0x1000;
    }
}
static void ExtraAct_State_Unk_ov001_022f8508(void) {   /* 15 frames stacked at x 0x24 */
    s32 i;
    if (G->unkE38 & 3) {
        gTaskCurrent->func = ExtraAct_State_Unk_ov001_022f8330;
        ExtraAct_State_Unk_ov001_022f8330();
        G->unkE38 &= ~0x1000u;
        G->unkE38 |= 0x2000;
        return;
    }
    G->unkE58--;
    if (G->unkE58 <= 0) {
        G->unkE54 = 0x24000;
        gTaskCurrent->func = ExtraAct_State_Unk_ov001_022f83c0;
    }
    for (i = 0; i < 0x10; i++) {
        u16 c = s_exd.txt_b[i];
        if (c != 0xe)
            draw_at(&G->objs[c + 0x18], 0x24, 0xb4);
    }
}
static void ExtraAct_Unk_ov001_022f8608(void) {   /* rises from y 0xe0 to 0xb4 */
    s32 v, acc, i;
    if (G->unkE38 & 3) {
        gTaskCurrent->func = ExtraAct_State_Unk_ov001_022f8330;
        ExtraAct_State_Unk_ov001_022f8330();
        G->unkE38 &= ~0x1000u;
        G->unkE38 |= 0x2000;
        return;
    }
    G->unkE5C = (s16)(G->unkE5C + 0x333);
    v = G->unkE5C;
    if (v >= 0x1000) {
        gTaskCurrent->func = ExtraAct_State_Unk_ov001_022f8508;
        acc = 0xb4000;
        G->unkE58 = 0xf;
    } else {
        i = 3;
        acc = 0xe0000;
        do {
            acc += FX_Mul(0xb4000 - acc, v);
        } while (i-- != 0);
    }
    for (i = 0; i < 0x10; i++) {
        u16 c = s_exd.txt_b[i];
        if (c != 0xe)
            draw_at(&G->objs[c + 0x18], 0x24, acc >> 12);
    }
}
static void ExtraAct_Unk_ov001_022f8774(void) {
    G->unkE5C = 0;
    G->tasks[4] = Task_Create(ExtraAct_Unk_ov001_022f8608, NULL, 0, 0x2000, 0, 0);
}
/* "SONIC & BLAZE" at y 0x1e */
static void ExtraAct_State_Unk_ov001_022f87c8(void) {
    s32 i, x;
    for (i = 0, x = 0x3d; i < 0xd; i++) {
        u16 c = s_exd.txt_t[i];
        if (c != 0xe)
            draw_at(&G->objs[c + 0x18], x, 0x1e);
        x += s_exd.gw[c & 15];
    }
}
static void ExtraAct_Unk_ov001_022f8858(void) {   /* the letters open vertically one by one */
    s32 i, x;
    u16 k;
    if (G->unkE38 & 3) {
        gTaskCurrent->func = ExtraAct_State_Unk_ov001_022f87c8;
        ExtraAct_State_Unk_ov001_022f87c8();
        G->unkE38 &= ~0x400u;
        G->unkE38 |= 0x800;
        return;
    }
    for (i = 0, x = 0x3d; i < G->unkE4C; i++) {
        k = s_exd.txt_t[i];
        if (k != 0xe)
            draw_at(&G->objs[k + 0x18], x, 0x1e);
        x += s_exd.gw[k & 15];
    }
    k = s_exd.txt_t[i];
    G->unkE50 += 0x333;
    if (G->unkE50 > 0x1000)
        G->unkE50 = 0x1000;
    if (k != 0xe) {
        MnSpr *o = &G->objs[k + 0x18];
        o->x = (s16)x;
        o->y = 0x1e;
        AnimatorSprite_DrawScaled(o, 0x1000, G->unkE50);
    }
    if (G->unkE50 < 0x1000)
        return;
    G->unkE50 = 0;
    G->unkE4C++;
    if (G->unkE4C < 0xd)
        return;
    gTaskCurrent->func = ExtraAct_State_Unk_ov001_022f87c8;
    G->unkE38 |= 0x400;
}
static void ExtraAct_Unk_ov001_022f8a40(void) {
    G->unkE4C = 0;
    G->unkE50 = 0;
    G->tasks[3] = Task_Create(ExtraAct_Unk_ov001_022f8858, NULL, 0, 0x2000, 0, 0);
}

/* ------------------------------------------------------------- letterbox */
/* the blue edge bars (sprite 38, anim 39): top normal, bottom flipped */
static void ExtraAct_State_Unk_ov001_022f8a98(void) {
    MnSpr *o = &G->objs[38];
    o->flags &= ~(u32)SPR_VFLIP;
    draw_at(o, 0, 0);
    o->flags |= SPR_VFLIP;
    draw_at(o, 0, 0xc0);
}
static void ExtraAct_State_Unk_ov001_022f8b04(void) {
    MnSpr *o;
    s32 v, acc, i;
    s16 y;
    if (G->unkE38 & 3) {
        gTaskCurrent->func = ExtraAct_State_Unk_ov001_022f8a98;
        G->unkE38 &= ~0x40u;
        G->unkE38 |= 0x80;
        return;
    }
    G->unkE48 = (s16)(G->unkE48 + 0x100);
    if (G->unkE48 >= 0x1000) {
        G->unkE48 = 0x1000;
        gTaskCurrent->func = ExtraAct_State_Unk_ov001_022f8a98;
        G->unkE38 |= 0x100;
    }
    i = 5;
    acc = 0;
    v = G->unkE48;
    do {
        acc += FX_Mul(0x10 - acc, v);
    } while (i-- != 0);
    o = &G->objs[38];
    y = (s16)acc;
    o->flags &= ~(u32)SPR_VFLIP;
    draw_at(o, 0, y - 0x10);
    o->flags |= SPR_VFLIP;
    draw_at(o, 0, 0xd0 - y);
}
/* data_0208923c: WIN0H / WIN1H / WIN0V / WIN1V bytes (x2, x1, .., y2, y1) */
static void win_final(void) {
    u8 *w = G->win;
    w[1] = 0;
    w[5] = 0x20;
    w[0] = 0x80;
    w[4] = 0xa1;
    w[3] = 0x7f;
    w[7] = 0x20;
    w[2] = 0;
    w[6] = 0xa1;
}
static void ExtraAct_Unk_ov001_022f8c78(void) {   /* the windows close to rows 0x20..0xa1 */
    u8 *w = G->win;
    s32 s, x, i;
    u8 v;
    if (G->unkE38 & 3) {
        win_final();
        G->bg1_on = 1;   /* SetVisible(2) */
        gTaskCurrent->func = ExtraAct_State_Unk_ov001_022f8a98;
        G->unkE38 &= ~0x40u;
        G->unkE38 |= 0x80;
        return;
    }
    G->unkE48 = (s16)(G->unkE48 + 0x88);
    s = G->unkE48;
    if (s < 0x1000) {
        i = 1;
        x = 0x20;
        do {
            x = FX_Mul(x, s);
        } while (i-- != 0);
        v = (u8)x;
        w[5] = v;
        w[4] = (u8)(0xc0 - v);
        w[7] = v;
        w[6] = (u8)(0xc0 - v);
        if (G->unkE38 & 8)
            G->bg1_on = 1;
    } else {
        win_final();
        gTaskCurrent->func = ExtraAct_State_Unk_ov001_022f8b04;
        G->unkE48 = 0;
        G->unkE38 |= 0x40;
    }
    G->unkE38 |= 8;
}
static void ExtraAct_Unk_ov001_022f8e7c(void) {
    u8 *w = G->win;
    G->unkE48 = 0;
    G->tasks[2] = Task_Create(ExtraAct_Unk_ov001_022f8c78, NULL, 0, 0x2000, 0, 0);
    w[1] = 0;
    w[5] = 0;
    w[0] = 0x80;
    w[4] = 0xc0;
    w[3] = 0x7f;
    w[7] = 0;
    w[2] = 0;
    w[6] = 0xc0;
    /* WININ / WINOUT: inside the windows BG0 (3D) + OBJ, outside BG1 + OBJ */
    if (g_menu_hooks.bgm)
        g_menu_hooks.bgm(0x12);   /* Sound_PlaySeq(0, 0x12) */
}
/* BG1 outside the two windows (they cover x 0..0xff): rows [0, y1) and [y2, 0xc0);
 * top half colour 2, bottom half colour 0x12 (the VRAM fills of ExtraAct_Start) */
static void ExtraAct_DrawBg1(void) {
    u32 top, bot;
    s32 y1 = G->win[5], y2 = G->win[4];
    u16 c;
    if (!G->bg1_on)
        return;
    c = s_exd.bgpal[2];
    top = 0xff000000u | (u32)((c & 31) * 255 / 31) << 16 | (u32)((c >> 5 & 31) * 255 / 31) << 8 |
          (u32)((c >> 10 & 31) * 255 / 31);
    c = s_exd.bgpal[0x12];
    bot = 0xff000000u | (u32)((c & 31) * 255 / 31) << 16 | (u32)((c >> 5 & 31) * 255 / 31) << 8 |
          (u32)((c >> 10 & 31) * 255 / 31);
    /* BG1 priority 0, behind the sprites of priority 0: emitted after them */
    if (y1 > 0)
        mn_emit_flat(0, 0, 0, 0, 256, (float)(y1 < 96 ? y1 : 96), top);
    if (y2 < 0xc0)
        mn_emit_flat(0, 0, 0, (float)y2, 256, (float)(0xc0 - y2), y2 >= 96 ? bot : top);
}

/* ------------------------------------------------------------- the model */
static void ExtraAct_State_DrawModel(void) { Model_Draw(&G->model_c90); }
static void ExtraAct_Unk_ov001_022f8f70(void) {
    ExModel *m = &G->model_c90;
    if (G->unkE38 & 3) {
        m->speed = 0;
        m->frame = model_frames(m) > 0 ? model_frames(m) - 1 : 0;   /* the last frame */
        gTaskCurrent->func = ExtraAct_State_DrawModel;
        G->unkE38 &= ~0x10u;
        G->unkE38 |= 0x20;
    }
    Model_Animate(m);
    Model_Draw(m);
    if (!(m->flags & 2))
        return;
    m->speed = 0;
    gTaskCurrent->func = ExtraAct_State_DrawModel;
    G->unkE38 |= 0x10;
}
static void ExtraAct_Unk_ov001_022f904c(void) {
    ExModel *m = &G->model_c90;
    if (G->unkE38 & 3) {
        m->speed = 0;
        m->frame = model_frames(m) > 0 ? model_frames(m) - 1 : 0;
        gTaskCurrent->func = ExtraAct_State_DrawModel;
        G->unkE38 &= ~0x10u;
        G->unkE38 |= 0x20;
    }
    G->unkE44--;
    if (G->unkE44 > 0)
        return;
    gTaskCurrent->func = ExtraAct_Unk_ov001_022f8f70;
    ex_se(6);
}
static void ExtraAct_Unk_ov001_022f9134(void) {
    G->unkE44 = 0x3c;
    G->tasks[1] = Task_Create(ExtraAct_Unk_ov001_022f904c, NULL, 0, 0x2000, 0, 0);
}

/* ------------------------------------------------------------- main */
static void ExtraAct_ReleaseAssets(void) {
    s32 i;
    NNS_SndStopSoundAll();
    for (i = 0; i < 7; i++)
        if (G->tasks[i] != NULL && G->tasks[i] != gTaskCurrent)
            Task_Destroy(G->tasks[i]);
    Model_Release(&G->model_c90);
    Model_Release(&G->model_b58);
    mn_res_free(G->res);
    G->res = NULL;
}

static void ExtraAct_Main(void) {
    u32 *f = &G->unkE38;
    s_ndraw = 0;
    Model_Draw(&G->model_b58);
    if (gPad.trg & PAD_A) {
        if (!(*f & 1)) {
            *f |= 1;
            if (!(*f & 0x30) && G->tasks[1] == NULL) {
                ExtraAct_Unk_ov001_022f9134();
                *f &= ~0x10u;
                *f |= 0x20;
            }
            if (!(*f & 0x3c0) && G->tasks[2] == NULL) {
                ExtraAct_Unk_ov001_022f8e7c();
                *f &= ~0x40u;
                *f |= 0x80;
            }
            if (!(*f & 0xc00) && G->tasks[3] == NULL) {
                ExtraAct_Unk_ov001_022f8a40();
                *f &= ~0x400u;
                *f |= 0x800;
            }
            if (!(*f & 0x3000) && G->tasks[4] == NULL) {
                ExtraAct_Unk_ov001_022f8774();
                *f &= ~0x1000u;
                *f |= 0x2000;
            }
            if (!(*f & 0xc000) && G->tasks[5] == NULL) {
                ExtraAct_StartScoreTally();
                *f &= ~0x4000u;
                *f |= 0x8000;
            }
        } else if (!(*f & 2)) {
            *f |= 2;
            if (!(*f & 0x30000) && G->tasks[6] == NULL) {
                ExtraAct_StartRankStamp();
                *f &= ~0x10000u;
                *f |= 0x20000;
            }
        }
    }
    if (!(*f & 4) && G->unkE3C != 0) {
        G->unkE3C = 0;
        *f |= 4;
        ExtraAct_Unk_ov001_022f9134();
    } else if (*f & 0x10) {
        *f &= ~0x10u;
        *f |= 0x20;
        ExtraAct_Unk_ov001_022f8e7c();
    } else if (*f & 0x40) {
        *f &= ~0x40u;
        *f |= 0x80;
        ExtraAct_Unk_ov001_022f8a40();
    } else if (*f & 0x400) {
        *f &= ~0x400u;
        *f |= 0x800;
        ExtraAct_Unk_ov001_022f8774();
    } else if (*f & 0x1000) {
        *f &= ~0x1000u;
        *f |= 0x2000;
        ExtraAct_StartScoreTally();
    } else if (*f & 0x4000) {
        *f &= ~0x4000u;
        *f |= 0x8000;
        ExtraAct_StartRankStamp();
    } else if (*f & 0x10000) {
        *f &= ~0x10000u;
        *f |= 0x20002;
    } else if (*f & 2) {
        if (G->unkE3C != 0) {
            G->unkE40++;
            if (G->unkE40 >= 0x3c) {
                ExtraAct_ReleaseAssets();
                G->done = 1;   /* SceneChange_ToExtraEnding */
                Task_Destroy(gTaskCurrent);
            }
        } else {
            G->unkE34++;
            if (G->unkE34 == 0x78)
                BrightnessLerp_Create(0x10, 0, 0x10, 3, &G->unkE3C);
        }
    }
}

/* the BG1 bands: drawn by a task at the end of the list (after all the sprites) */
static void ExtraAct_Bg1Task(void) {
    if (G && !G->done)
        ExtraAct_DrawBg1();
}

static int ex_data_load(void) {
    char path[160];
    u8 *b;
    u32 size;
    snprintf(path, sizeof(path), "%s/extraact.bin", g_mn_dir);
    if (mn_load_file(path, &b, &size) != 0)
        return -1;
    if (size < 8 || memcmp(b, "REXA", 4) != 0 || size - 8 != sizeof(ExData)) {
        free(b);
        return -1;
    }
    memcpy(&s_exd, b + 8, sizeof(ExData));
    free(b);
    return 0;
}

static void ExtraAct_Destructor(void) {}

int extraact_start(uint32_t time, uint32_t rings, int character) {
    s32 i;
    Task_DestroyAll();
    mn_display_reset();   /* ExtraAct_InitDisplay */
    mn_comp_reset(0);
    mn_comp_full(0);
    g_mn_comp.fill = 0;
    BRIGHT_MAIN = BRIGHT_SUB = 0x10;
    g_mn_scene = "ExtraAct";
    gGameState.unk0 = character;
    gGameState.unk1C = 8;   /* the Extra boss (zone 8 act 2): ClearDemo_GetRank's boss table */
    gGameState.unk1D = 2;
    if (ex_data_load() != 0) {
        fprintf(stderr, "extraact: %s/extraact.bin missing or invalid\n", g_mn_dir);
        return -1;
    }
    s_ex = (ExWork *)calloc(1, sizeof(ExWork));
    G->unkE24 = ExtraAct_GetTimeBonus(time);
    G->unkE28 = (s32)rings * 100;
    G->unkE2C = G->unkE24 + G->unkE28;
    if (G->unkE2C > 999999)
        G->unkE2C = 999999;
    /* ExtraAct_LoadAssets: extra_act_lz7.narc files 6 (ac_ex_cldm.bac), 5 (the BG
     * palette: extraact.bin), 7..10 (eff_dmg_end01 / 02 .nsbmd, .nsbca, .nsbma) */
    G->res = mn_res_load("act/ac_ex_cldm.bac");
    model_load(&G->model_c90, "narc__extra_act__eff_dmg_end02__eff_dmg_end02.dcm", "narc__extra_act__eff_dmg_end02.dca");
    model_load(&G->model_b58, "narc__extra_act__eff_dmg_end01__eff_dmg_end01.dcm", NULL);
    for (i = 0; i < 0x27; i++) {
        AnimatorSprite_Init(&G->objs[i], G->res, s_exd.spr[i], 0x30, 0);
        AnimatorSprite_ProcessAnimation(&G->objs[i], 0, 0);
    }
    G->unkE30 = ClearDemo_GetRank((u32)G->unkE2C);
    AnimatorSprite_Init(&G->objs[0x27], G->res, (u16)(G->unkE30 + 0x23), 0x60, 0);
    AnimatorSprite_ProcessAnimation(&G->objs[0x27], 0, 0);
    G->model_c90.speed = 0x1000;
    G->tasks[0] = Task_Create(ExtraAct_Main, ExtraAct_Destructor, 0, 0x1000, 0, 0);
    Task_Create(ExtraAct_Bg1Task, NULL, 0, 0x7000, 0, 0);
    BrightnessLerp_Create(0x10, 0x10, 0, 3, &G->unkE3C);
    return 0;
}

int extraact_run_frame(const InputState *in) {
    if (!G)
        return 1;
    if (G->done)
        return 1;
    Pad_Update(&gPad, mn_pad_from_input(in));
    mn_quads_reset();
    s_ndraw = 0;
    Task_RunAll();
    return G->done;
}

void extraact_result(int32_t out[4]) {
    out[0] = G ? G->unkE24 : 0;
    out[1] = G ? G->unkE28 : 0;
    out[2] = G ? G->unkE2C : 0;
    out[3] = G ? G->unkE30 : 0;
}

void extraact_shutdown(void) {
    if (!G)
        return;
    if (!G->done)
        ExtraAct_ReleaseAssets();
    Task_DestroyAll();
    mn_display_reset();
    free(G);
    s_ex = NULL;
}

int extraact_project(float *xyz, int max) {
    static dcm_mtx_t pal[DCM_MAX_PALETTE];
    dcm_camera_t cam;
    dcm_mtx_t id;
    int i, n = 0;
    if (!G)
        return 0;
    ex_camera(&cam);
    dcm_mtx_identity(&id);
    for (i = 0; i < s_ndraw; i++) {
        int nv = (int)s_draw[i]->mdl.hdr->num_vertices;
        if (n + nv > max)
            break;
        pose(s_draw[i], pal);
        dcm_transform_vertices(&s_draw[i]->mdl, pal, &id, &cam, xyz + 3 * n);
        n += nv;
    }
    return n;
}

#ifdef _arch_dreamcast
void extraact_draw(void) {
    static dcm_mtx_t pal[DCM_MAX_PALETTE];
    static const int lists[3] = { PVR_LIST_OP_POLY, PVR_LIST_PT_POLY, PVR_LIST_TR_POLY };
    dcm_camera_t cam;
    dcm_lights_t L;
    dcm_mtx_t id;
    dcm_draw_opts_t o;
    int k, i;
    ex_camera(&cam);
    ex_lights(&L, &cam);
    dcm_mtx_identity(&id);
    for (k = 0; k < 3; k++) {
        render_begin_list(lists[k]);
        if (G)
            for (i = 0; i < s_ndraw; i++) {
                memset(&o, 0, sizeof(o));
                o.list = lists[k];
                o.pt_list = PVR_LIST_PT_POLY;
                o.alpha_scale = 255;
                pose(s_draw[i], pal);
                dcm_draw(&s_draw[i]->mdl, pal, &id, &cam, NULL, &L, &o);
            }
        if (lists[k] == PVR_LIST_TR_POLY)
            menus_draw_tr();   /* the 2D composition and the brightness overlay */
        render_end_list();
    }
}
#else
void extraact_draw(void) {
    dcm_camera_t cam;
    dcm_lights_t L;
    ex_camera(&cam);
    ex_lights(&L, &cam);
    (void)L;
    menus_draw_tr();
}
#endif
