/*
 * TransformDemo (src/game/Cutscenes/TransformDemo/): the boss-act intro (the
 * character flies up the shaft to the boss, ids 0..3) and the super
 * transformation before the final / extra boss (id 4: Super Sonic and
 * Burning Blaze circling up among the Chaos / Sol Emerald effects).
 *
 * The DS renders the 3D scene twice, alternating every frame between the top
 * screen (TransformDemo_SetCameraFrameA) and the bottom screen (FrameB, display
 * capture of the other one): one tall column. Single screen: one 4:3 view with
 * the same projection that follows the character's height (the target the two
 * DS frames converge on once the character leaves the middle band), so the
 * character stays in the frame for the whole climb (CUTSCENES.md section 3).
 */
#include <stdio.h>
#include <stdlib.h>
#include "cs_internal.h"

s16 gTransformDemoId;

typedef struct {
    s32 pos[3];       /* trans */
    s32 vel[3];
    u16 ang[2];       /* rot x, rot y */
    u16 rot;          /* orbit angle (super) */
    CsInst inst;
} TrPlayer;

typedef struct TransformDemo TransformDemo;
struct TransformDemo {
    void (*fn)(TransformDemo *);
    u32 flags;        /* 0x10 intro done, 0x20 fading, 0x40 end, 0x100 skippable, 0x200 skip, 0x400/0x1000 sound stop */
    s32 count;        /* unk8 */
    s16 intro;        /* unkc */
    s32 mode;         /* unk10: 0 / 1 shaft variants, 2 super */
    CsCam cam;        /* unk14 */
    s32 tan, dist;    /* unk38 / unk3c */
    /* unk454: archive models */
    CsAnimSet jnt, vis;
    CsModel models[6];
    CsInst objs[6];
    s32 tbl[6];
    /* unkbd4: player / super models */
    s32 kind;
    s32 cam_y;        /* unkbd8 */
    CsModel pmodel[2];
    CsAnimSet pjnt, pvis;
    TrPlayer pl[2];
    /* fade (unkea0) */
    void (*fade)(TransformDemo *);
    /* light flash (unkeb0) */
    s8 flash_t, flash_on;
    u16 col[3];
    s16 vec[3][3];
};
#define MAX_FX 32
typedef struct {
    int on;
    u8 k;
    s32 x, y, z, vy, w;
    CsInst inst;
} TrFx;

static TransformDemo s_td;
static TrFx s_fx[MAX_FX];
static int s_td_on;

BOOL TransformDemo_IsActive(void) { return s_td_on; }

static const u8 *trl(void) { return (const u8 *)cs_sec("tr_light", NULL); }

static void TransformDemo_SetupLights(TransformDemo *s) {
    const u8 *t = trl();
    const u16 *col;
    const s16 *vec;
    int i;
    if (!t)
        return;
    if (s->mode != 2) {
        col = (const u16 *)(t + 0x14);   /* data_ov001_023161f8 */
        vec = (const s16 *)(t + 0x28);   /* data_ov001_0231620c */
    } else {
        col = (const u16 *)(t + 0x1a);   /* data_ov001_023161fe */
        vec = (const s16 *)(t + 0x3a);   /* data_ov001_0231621e */
    }
    for (i = 0; i < 3; i++) {
        s->col[i] = col[i];
        s->vec[i][0] = vec[3 * i];
        s->vec[i][1] = vec[3 * i + 1];
        s->vec[i][2] = vec[3 * i + 2];
    }
}
static void TransformDemo_UpdateLightFlash(TransformDemo *p) {
    const u8 *t = trl();
    const u16 *a;
    const s16 *c;
    int i;
    if (p->mode != 2 || !t)
        return;
    if (++p->flash_t < 6)
        return;
    p->flash_t = 0;
    p->flash_on = !p->flash_on;
    a = (const u16 *)(t + (p->flash_on ? 0x1a : 0x20));
    c = (const s16 *)(t + (p->flash_on ? 0x3a : 0x4c));
    for (i = 0; i < 3; i++) {
        p->col[i] = a[i];
        p->vec[i][0] = c[3 * i];
        p->vec[i][1] = c[3 * i + 1];
        p->vec[i][2] = c[3 * i + 2];
    }
}

static void inst_init(CsInst *in, CsModel *m) {
    memset(in, 0, sizeof(*in));
    in->model = m;
    in->scale[0] = in->scale[1] = in->scale[2] = 1.0f;
    in->rot[0] = in->rot[4] = in->rot[8] = 1.0f;
    in->jnt_idx = in->vis_idx = -1;
}
static void anim_step(CsInst *in) {
    in->frame += 1.0f;
    if (in->jnt && in->jnt->set.hdr && in->jnt_idx >= 0 && in->jnt_idx < in->jnt->set.hdr->num_anims &&
        in->frame >= in->jnt->set.anims[in->jnt_idx].num_frames)
        in->frame = 0;
}
static void rot_xy(CsInst *in, u16 ax, u16 ay) {
    /* MTX_RotX33 * MTX_RotY33 (DS row layout) */
    float sx = cs_sin(ax) / 4096.0f, cx = cs_cos(ax) / 4096.0f, sy = cs_sin(ay) / 4096.0f, cy = cs_cos(ay) / 4096.0f;
    float rx[9] = {1, 0, 0, 0, cx, sx, 0, -sx, cx}, ry[9] = {cy, 0, -sy, 0, 1, 0, sy, 0, cy};
    int r, c, k;
    for (r = 0; r < 3; r++)
        for (c = 0; c < 3; c++) {
            float v = 0;
            for (k = 0; k < 3; k++)
                v += rx[r * 3 + k] * ry[k * 3 + c];
            in->rot[r * 3 + c] = v;
        }
}

static const char *mlist(int i) {
    const char *t = (const char *)cs_sec("tr_mlist", NULL);
    return t && i >= 0 && i < 16 ? t + 48 * i : "";
}

static void TransformDemo_LoadPlayerArchive(TransformDemo *w) {
    const s32 *tbl = (const s32 *)cs_sec("tr_models", NULL);
    int i;
    cs_anim_load(&w->jnt, "narc__pldm_trans_lz7__mdl.dca");
    cs_anim_load(&w->vis, "narc__pldm_trans_lz7__mdl.dcv");
    for (i = 0; i < 6; i++) {
        s32 e = tbl ? tbl[6 * (w->mode < 0 || w->mode > 2 ? 0 : w->mode) + i] : -1;
        w->tbl[i] = e;
        if (e == -1)
            continue;
        cs_model_load(&w->models[i], mlist(e));
        inst_init(&w->objs[i], &w->models[i]);
        w->objs[i].jnt = &w->jnt;
        w->objs[i].vis = &w->vis;
        w->objs[i].jnt_idx = w->objs[i].vis_idx = e;
    }
    w->objs[0].trans[1] = 0x226000 / 4096.0f;
    w->objs[1].trans[1] = -0x226000 / 4096.0f;
}
static void TransformDemo_LoadPlayerModels(TransformDemo *w) {
    int i;
    if (w->kind != 2) {
        /* TransformDemo_LoadPlayerNarc: son.nsbmd / blz.nsbmd, anim 15, visibility 1 */
        TrPlayer *s = &w->pl[0];
        cs_model_load(&w->pmodel[0], w->kind == 0 ? "mod__son__son.dcm" : "mod__blz__blz.dcm");
        cs_anim_load(&w->pjnt, "narc__player_lz7__son.dca");
        cs_anim_load(&w->pvis, "narc__player_lz7__son.dcv");
        inst_init(&s->inst, &w->pmodel[0]);
        s->inst.jnt = &w->pjnt;
        s->inst.vis = &w->pvis;
        s->inst.jnt_idx = 0xf;
        s->inst.vis_idx = 1;
        s->ang[1] = 0x1700;
        s->vel[1] = 0x3000;
        s->pos[1] = -0x190000;
        w->cam_y = -0x18b000;
        return;
    }
    /* TransformDemo_LoadTransModels: dm_trans_ssn / dm_trans_bbz with archive anims 13 / 14 */
    for (i = 0; i < 2; i++) {
        TrPlayer *s = &w->pl[i];
        cs_model_load(&w->pmodel[i], mlist(i == 0 ? 13 : 14));
        inst_init(&s->inst, &w->pmodel[i]);
        s->inst.jnt = &w->jnt;
        s->inst.vis = &w->vis;
        s->inst.jnt_idx = s->inst.vis_idx = i == 0 ? 0xd : 0xe;
        s->ang[1] = 0x1700;
        s->vel[1] = 0x2000;
        s->pos[1] = -0x190000;
    }
    w->pl[1].rot = 0x8000;
    w->cam_y = -0x18b000;
}
static void TransformDemo_ReleaseModels(TransformDemo *w) {
    int i;
    for (i = 0; i < 6; i++)
        cs_model_free(&w->models[i]);
    for (i = 0; i < 2; i++)
        cs_model_free(&w->pmodel[i]);
    cs_anim_free(&w->jnt);
    cs_anim_free(&w->vis);
    cs_anim_free(&w->pjnt);
    cs_anim_free(&w->pvis);
    for (i = 0; i < MAX_FX; i++)
        s_fx[i].on = 0;
}

static void TransformDemo_InitCamera(TransformDemo *w) {
    u16 b = w->mode != 2 ? 0x71c : 0x4fa;
    w->cam.fovy = 0x888;
    w->cam.aspect = 0x1555 / 4096.0f;
    w->cam.near_ = 1.0f;
    w->cam.far_ = 2048.0f;
    w->cam.eye[0] = 0;
    w->cam.eye[1] = 0;
    w->cam.eye[2] = 0x19000 / 4096.0f;
    w->cam.up[0] = 0;
    w->cam.up[1] = 1;
    w->cam.up[2] = 0;
    w->cam.at[0] = w->cam.at[1] = w->cam.at[2] = 0;
    w->tan = FX_Div(cs_sin((u16)(0x1110 + b)), cs_cos((u16)(0x1110 + b)));
    w->dist = (s32)(((s64)FX_Div(cs_sin((u16)(0x888 + (b >> 1))), cs_cos((u16)(0x888 + (b >> 1)))) * 0x19000 + 0x800) >> 12);
}
/* single view: target height = the character's height (TransformDemo_SetCameraFrameA/B
 * follow it outside [-dist, dist]; inside, the two DS frames hold +-dist) */
static void TransformDemo_SetCamera(TransformDemo *w) {
    w->cam.at[1] = w->cam_y / 4096.0f;   /* G3i_LookAt(pos (0,0,25), up, target (0, ty, 0)) */
}

static void TransformDemoEffect_Spawn(TransformDemo *w) {
    TrFx *o = NULL;
    int i;
    u8 k;
    for (i = 0; i < MAX_FX; i++)
        if (!s_fx[i].on) {
            o = &s_fx[i];
            break;
        }
    k = (u8)(cs_rand() % 3);
    if (!o) {
        cs_rand();
        cs_rand();
        cs_rand();
        cs_rand();
        return;
    }
    memset(o, 0, sizeof(*o));
    o->k = k;
    o->x = ((cs_rand() & 0x1f) - 0x10) << 12;
    o->y = 0xfa000;
    o->z = ((cs_rand() & 0x1f) - 0x10) << 12;
    o->w = ((cs_rand() & 0xf) + 1) << 12;
    if (w->mode == 2)
        o->vy = -(((cs_rand() & 7) + 2) << 11);
    else
        o->vy = -(((cs_rand() & 7) + 5) << 11);
    k = (u8)(o->k + 2);
    if (w->tbl[k] < 0)
        return;
    inst_init(&o->inst, &w->models[k]);
    o->inst.jnt = &w->jnt;
    o->inst.vis = &w->vis;
    o->inst.jnt_idx = o->inst.vis_idx = k;
    o->on = 1;
}
static void TransformDemoEffect_Main(void) {
    int i;
    for (i = 0; i < MAX_FX; i++) {
        TrFx *o = &s_fx[i];
        if (!o->on)
            continue;
        if (!s_td_on) {
            o->on = 0;
            continue;
        }
        o->y += o->vy;
        if (o->y <= -0x12c000) {
            o->on = 0;
            continue;
        }
        o->inst.trans[0] = o->x / 4096.0f;
        o->inst.trans[1] = o->y / 4096.0f;
        o->inst.trans[2] = o->z / 4096.0f;
        cs3d_add(&o->inst);
        anim_step(&o->inst);
    }
}

static void TransformDemo_UpdatePlayerModels(TransformDemo *w) {
    int i;
    if (w->kind != 2) {
        TrPlayer *o = &w->pl[0];
        /* TransformDemo_Unk_ov001_022dedf4 */
        if (o->pos[1] < 0)
            o->vel[1] -= 10;
        else
            o->vel[1] += 10;
        o->pos[1] += o->vel[1];
        rot_xy(&o->inst, o->ang[0], o->ang[1]);
        o->inst.trans[0] = o->pos[0] / 4096.0f;
        o->inst.trans[1] = o->pos[1] / 4096.0f;
        w->cam_y = o->pos[1] + 0x5000;
        return;
    }
    /* TransformDemo_Unk_ov001_022de774 */
    for (i = 0; i < 2; i++) {
        TrPlayer *o = &w->pl[i];
        if (o->pos[1] < 0)
            o->vel[1] -= 0x10;
        else
            o->vel[1] += 0x10;
        o->pos[1] += o->vel[1];
        rot_xy(&o->inst, o->ang[0], o->ang[1]);
        o->rot = (u16)(o->rot + 0x100);
        o->pos[2] = FX_Mul(cs_cos(o->rot), 0x4000);
        o->pos[0] = FX_Mul(cs_sin(o->rot), 0x4000);
        o->inst.trans[0] = o->pos[0] / 4096.0f;
        o->inst.trans[1] = o->pos[1] / 4096.0f;
        o->inst.trans[2] = o->pos[2] / 4096.0f;
    }
    w->cam_y = w->pl[0].pos[1] + 0x5000;
}
static void TransformDemo_DrawModels(TransformDemo *w) {
    cs3d_add(&w->objs[0]);
    anim_step(&w->objs[0]);
    cs3d_add(&w->objs[1]);
    anim_step(&w->objs[1]);
}
static void TransformDemo_DrawPlayerModels(TransformDemo *w) {
    if (w->kind != 2) {
        cs3d_add(&w->pl[0].inst);
        anim_step(&w->pl[0].inst);
        return;
    }
    cs3d_add(&w->pl[0].inst);
    anim_step(&w->pl[0].inst);
    cs3d_add(&w->pl[1].inst);
    anim_step(&w->pl[1].inst);
    if (w->tbl[5] >= 0) {
        w->objs[5].trans[1] = w->cam_y / 4096.0f;
        cs3d_add(&w->objs[5]);
        anim_step(&w->objs[5]);
    }
}
static void begin3d(TransformDemo *w) {
    dcm_lights_t l;
    TransformDemo_SetCamera(w);
    cs_lights_from_ds(&l, w->col, (const s16(*)[3])w->vec, &w->cam);
    cs3d_begin(&w->cam, &l);
}
static void TransformDemo_UpdateDualScreen3D(TransformDemo *w) {
    if (w->mode == 2) {
        if ((cs_rand() & 7) == 0)
            TransformDemoEffect_Spawn(w);
    } else {
        if ((cs_rand() & 3) == 0)
            TransformDemoEffect_Spawn(w);
    }
    begin3d(w);
    TransformDemo_DrawModels(w);
    TransformDemo_UpdatePlayerModels(w);
    TransformDemo_DrawPlayerModels(w);
}
static void TransformDemo_UpdateDualScreen3DIntro(TransformDemo *p) {
    p->intro--;
    if (p->intro <= 0) {
        p->flags |= 0x10;
        p->fn = TransformDemo_UpdateDualScreen3D;
    }
    begin3d(p);
    if (p->mode == 2) {
        if ((cs_rand() & 7) == 0)
            TransformDemoEffect_Spawn(p);
    } else {
        if ((cs_rand() & 3) == 0)
            TransformDemoEffect_Spawn(p);
    }
}
static void TransformDemo_State_Init(TransformDemo *p) {
    TransformDemo_LoadPlayerArchive(p);
    TransformDemo_LoadPlayerModels(p);
    TransformDemo_SetupLights(p);
    TransformDemo_InitCamera(p);
    p->intro = 0x1e;
    p->fn = TransformDemo_UpdateDualScreen3DIntro;
    cs_log("transform %d mode %d kind %d", gTransformDemoId, p->mode, p->kind);
}

/* fade states (unkea0) */
static void TransformDemo_FadeState_WaitStart(TransformDemo *p);
static void TransformDemo_FadeState_FadeIn(TransformDemo *p);
static void TransformDemo_FadeState_WaitEnd(TransformDemo *p);
static void TransformDemo_FadeState_FadeOut(TransformDemo *p);
static void TransformDemo_FadeState_Init(TransformDemo *p) {
    g_cs_bright = -16;
    g_cs_bright_sub = -16;
    p->fade = TransformDemo_FadeState_WaitStart;
}
static void TransformDemo_FadeState_WaitStart(TransformDemo *p) {
    if (!(p->flags & 0x10))
        return;
    p->flags &= ~0x10u;
    BrightnessFade_Create(2, 0x88);
    p->flags |= 0x20;
    p->fade = TransformDemo_FadeState_FadeIn;
}
static void TransformDemo_FadeState_FadeIn(TransformDemo *p) {
    if (BrightnessFade_IsDone()) {
        p->flags &= ~0x20u;
        p->fade = TransformDemo_FadeState_WaitEnd;
    }
}
static void TransformDemo_FadeState_WaitEnd(TransformDemo *p) {
    if (p->flags & 0x40000000)
        return;
    if (!(p->flags & 0x200) && p->cam_y < 0x64000)
        return;
    p->flags &= ~0x200u;
    p->flags |= 0x420;
    BrightnessFade_Create(5, 0x44);
    cs_log("transform %d fade out at frame %d", gTransformDemoId, p->count);
    p->fade = TransformDemo_FadeState_FadeOut;
}
static void TransformDemo_FadeState_FadeOut(TransformDemo *p) {
    if (!BrightnessFade_IsDone())
        return;
    p->flags &= ~0x20u;
    p->flags |= 0x40;
    p->fade = NULL;
}

static void TransformDemo_UpdateSound(TransformDemo *w) {
    if (w->count == 0x1e)
        cs_se(CSH_TRANS, 0xb, w->mode != 2 ? 0 : 1);
    if ((w->flags & 0x400) && !(w->flags & 0x1000)) {
        cs_snd_stop(CSH_TRANS, 0x32);
        w->flags |= 0x1000;
    }
    if (w->mode != 2) {
        if (w->kind == 0) {
            if (w->count == 0x78)
                cs_voice(0xb, 2);
        } else {
            if (w->count == 0xa0)
                cs_voice(0xb, 3);
        }
        if (w->count == 0xdc && !(w->flags & 0x1000)) {
            cs_snd_stop(CSH_TRANS, 0x32);
            w->flags |= 0x1000;
        }
    } else {
        if (w->count == 0xb4)
            cs_voice(0xb, 2);
        if (w->count == 0x12c)
            cs_voice(0xb, 3);
        if (w->count == 0x195 && !(w->flags & 0x1000)) {
            cs_snd_stop(CSH_TRANS, 0x32);
            w->flags |= 0x1000;
        }
    }
}

void TransformDemo_Start(void) {
    const u8 *id = (const u8 *)cs_sec("tr_id", NULL);
    TransformDemo *w = &s_td;
    TransformDemo_Kill();
    memset(w, 0, sizeof(*w));
    if (!id || gTransformDemoId < 0 || gTransformDemoId > 5)
        return;
    w->kind = id[gTransformDemoId * 2];
    w->mode = id[gTransformDemoId * 2 + 1];
    w->fn = TransformDemo_State_Init;
    if (!g_cs_story.stage_new)
        w->flags |= 0x100;
    TransformDemo_FadeState_Init(w);
    cs_snd_bank(0xb);   /* TransformDemo_LoadSound: group 0xf BOSS_TRANS (SSAR 11) */
    s_td_on = 1;
}

void TransformDemo_Kill(void) {
    if (s_td_on)
        TransformDemo_ReleaseModels(&s_td);
    s_td_on = 0;
}

static void TransformDemo_Destructor(void) {
    TransformDemo_ReleaseModels(&s_td);
    BrightnessFade_Destroy();
    cs_snd_stop_all();
    cs_snd_bank(-1);   /* TransformDemo_ReleaseSound */
    s_td_on = 0;
    cs_log("transform %d end frames %d", gTransformDemoId, s_td.count);
}

void TransformDemo_RunFrame(void) {
    TransformDemo *t = &s_td;
    u32 f;
    if (!s_td_on)
        return;
    t->count++;
    f = t->flags;
    if ((f & 0x100) && !(f & 0x600) && (gPad.trg & PAD_START)) {
        t->flags = f | 0x200;
        cs_log("transform %d skip", gTransformDemoId);
    }
    if (t->flags & 0x40) {
        TransformDemo_Destructor();
        return;
    }
    if (t->fn)
        t->fn(t);
    if (t->fade)
        t->fade(t);
    TransformDemo_UpdateSound(t);
    TransformDemo_UpdateLightFlash(t);
    TransformDemoEffect_Main();
    if (t->flags & 0x80000000u)
        TransformDemo_Destructor();
}

void TransformDemo_Draw(void) {
    if (s_td_on)
        g_cs_backdrop = 0xff000000;   /* G3X_SetClearColor black */
}
