/*
 * PlayerDemo (src/game/Cutscenes/PlayerDemo/) and PolyDemoUnder2 / PolyDemo
 * (src/game/Cutscenes/PolyDemo/): the 3D story scenes.
 *
 * PlayerDemo runs a #BPD script (time, op, arg; gPlayerDemoOpTable), and
 * three tasks: PlayerDemoModel (the scene list of data_020766a4: per scene a
 * NARC with the joint / visibility animations and a camera rig "cam_node" /
 * "cam_int", plus the scene's models from /mod/; the camera looks from the
 * cam joint to the int joint), PlayerDemoBg (the per-language caption strip
 * on BG3, 256x48 at y 144) and PolyDemoUnder2 (the bottom screen: logo panel
 * and the PRESS START TO SKIP button). Lights: PlayerDemoLight_*.
 *
 * Single screen: the 3D view is the main screen at x2.5 (the full frame) with
 * its caption strip at the bottom, as on the DS top screen. Of the bottom
 * screen only the skip button carries information; it is placed in the top
 * right corner of the frame (x1.5) while the cutscene is skippable.
 */
#include <stdio.h>
#include <stdlib.h>
#include "cs_internal.h"

s16 gPlayerDemoId;

typedef struct {
    s32 pad0, time, op, arg;
} PdCmd;
typedef struct {
    char dca[48], dcv[48], cam[64], intm[64];
    s32 count;
    struct {
        char dcm[48];
        u32 flags;
    } m[16];
} PdScene;
typedef struct {
    s32 first, count;
    s8 row, col;
    u16 pad;
} PdDemo;

typedef struct PlayerDemo PlayerDemo;
struct PlayerDemo {
    void (*fn)(PlayerDemo *);
    u32 flags;
    u32 counter;      /* unk8 */
    s32 lang;         /* unk10 */
    u8 *script;       /* unk14 */
    u32 script_size;
    u16 idx, count;   /* unk18 / unk1a */
    const PdCmd *cmd; /* unk1c */
    u16 time;         /* unk20 */
    s16 scenes;       /* unk22 */
    int shake_on;     /* unk24 child: PlayerDemoCameraShake */
    s32 shake[3];
    s32 shake_timer;
    const s32 *snd;   /* unk2c {group, seqarc, ?, flags} */
    u16 light_period; /* unk28 */
};

/* PlayerDemoModel work */
#define PD_MAX_MODELS 16
typedef struct {
    void (*fn)(void);
    s8 demo;          /* unk8 */
    s8 scene;         /* unk9 */
    u8 nscenes;       /* unkA */
    u32 counter;      /* unkc */
    const PdScene *ent;
    s8 count, sky;    /* unk20 / unk21 */
    CsAnimSet jnt, vis;
    CsAnimSet mat[3];      /* the scene archive's mdl.nsbma / .nsbtp / .nsbta (pd_matx) */
    CsModel models[PD_MAX_MODELS];
    CsInst inst[PD_MAX_MODELS];
    u32 mflags[PD_MAX_MODELS];
    float cam_a[3], cam_b[3];   /* unk1310 / unk131c */
    CsCam cam;
    /* light (unk1334) */
    void (*light_fn)(void);
    u32 light_toggle;
    s8 light_timer, light_period;
    const u16 *col0, *col1;
    u16 cur_col[3];
    s16 cur_vec[3][3];
} PdModel;

/* PlayerDemoBg work */
typedef struct {
    void (*fn)(void);
    s8 idx, count;    /* unkC / unkD */
    u16 t54, t56;
    u16 pal[256];
    u8 *idxbuf;
    CsTex tex;
    int loaded, shown;
} PdBg;

typedef struct {
    CsRes *res1, *res2;
    CsSpr spr[7];
    s16 blink;        /* unk20C */
} PdUnder;

static PlayerDemo s_pd;
static PdModel s_pm;
static PdBg s_pb;
static PdUnder s_pu;
static int s_pd_on, s_pm_on, s_pb_on, s_pu_on;
static int s_skip_on, s_skip_timer;
static int s_rep_on;
static s8 s_rep_timer, s_rep_period;
static u8 s_rep_arc, s_rep_id;
static const char *s_last_scene;

BOOL PlayerDemo_IsActive(void) { return s_pd_on; }
u32 *PlayerDemo_Flags(void) { return s_pd_on ? &s_pd.flags : NULL; }

static const PdDemo *demo_ent(void) {
    const PdDemo *t = (const PdDemo *)cs_sec("pd_demo", NULL);
    return t && gPlayerDemoId >= 0 && gPlayerDemoId < 0x2c ? &t[gPlayerDemoId] : NULL;
}
static const s32 *PlayerDemo_GetSoundEntry(void) {
    const s32 *t = (const s32 *)cs_sec("pd_snd", NULL);
    return t && gPlayerDemoId >= 0 && gPlayerDemoId < 0x2c ? t + 4 * gPlayerDemoId : NULL;
}
static BOOL snd_ok(const s32 *e) { return e && ((u32)e[3] & 0x80000000u); }

/* ------------------------------------------------------------ fades */
/* PlayerDemo_StartFade(n, flags): data_02076adc[flags & 3] = {mode, bright};
 * flags & 4: both screens (single screen: the same) */
static void PlayerDemo_StartFade(u32 n, u32 flags) {
    const s32 *t = (const s32 *)cs_sec("pd_fade", NULL);
    u8 idx = flags & 3;
    if (!t)
        return;
    if (n != 0) {
        if (flags & 4)
            BrightnessFade_Create((u16)t[2 * idx], (u16)(0x1000 / n));
        else
            BrightnessFade_Create((u16)(t[2 * idx] | 0x100), (u16)(0x1000 / n));
    } else {
        g_cs_bright = (s16)t[2 * idx + 1];
        if (flags & 4)
            g_cs_bright_sub = g_cs_bright;
    }
}

/* ------------------------------------------------------------ lights */
static const u16 *PlayerDemoLight_GetColors(s32 stage, s32 act) {
    const s32 *sel = (const s32 *)cs_sec("pd_lsel", NULL);
    const PdDemo *d = (const PdDemo *)cs_sec("pd_demo", NULL);
    const u8 *base = (const u8 *)cs_sec("pd_lcol", NULL), *alt = (const u8 *)cs_sec("pd_lcol2", NULL);
    s32 i;
    if (!sel || !d || !base || !alt || act < 0 || act > 7)
        return NULL;
    i = sel[(stage * 8 + act) * 2];
    if (i == 0) {
        s32 r = d[stage].row < 0 ? 0 : d[stage].row, c = d[stage].col < 0 ? 0 : d[stage].col;
        return (const u16 *)(base + 0x12 * (r & 7) + 6 * (c % 3));
    }
    return (const u16 *)(alt + 6 * (i & 31));
}
static const u16 *PlayerDemoLight_GetColorsAlt(s32 a, s32 b) {
    const s32 *sel = (const s32 *)cs_sec("pd_lsel", NULL);
    const u8 *alt = (const u8 *)cs_sec("pd_lcol2", NULL);
    s32 i;
    if (!sel || !alt || b < 0 || b > 7)
        return NULL;
    i = sel[(a * 8 + b) * 2 + 1];
    if (i == 0)
        return PlayerDemoLight_GetColors(a, b);
    return (const u16 *)(alt + 6 * (i & 31));
}
static const s16 *light_vectors(s32 a, s32 b, int alt_sel) {
    const s32 *sel = (const s32 *)cs_sec("pd_lsel", NULL);
    const PdDemo *d = (const PdDemo *)cs_sec("pd_demo", NULL);
    const u8 *base = (const u8 *)cs_sec("pd_lvec", NULL), *alt = (const u8 *)cs_sec("pd_lvec2", NULL);
    s32 i;
    if (!sel || !d || !base || !alt || b < 0 || b > 7)
        return NULL;
    i = sel[(a * 8 + b) * 2 + alt_sel];
    if (i == 0)
        return (const s16 *)(base + 0x12 * ((d[a].row < 0 ? 0 : d[a].row) & 7));
    return (const s16 *)(alt + 0x12 * (i & 15));
}
static void set_light(PdModel *o, const u16 *c, const s16 *v) {
    int i, k;
    if (c)
        for (i = 0; i < 3; i++)
            o->cur_col[i] = c[i];
    if (v)
        for (i = 0; i < 3; i++)
            for (k = 0; k < 3; k++)
                o->cur_vec[i][k] = v[i * 3 + k];
}
static void PlayerDemoLight_Apply(PdModel *o) {
    set_light(o, PlayerDemoLight_GetColors(gPlayerDemoId, o->scene), light_vectors(gPlayerDemoId, o->scene, 0));
    /* G3X_SetEdgeColorTable: toon edges are not drawn natively (CUTSCENES.md section 8) */
}
static void PlayerDemoLight_ApplyAlt(PdModel *o) {
    set_light(o, PlayerDemoLight_GetColorsAlt(gPlayerDemoId, o->scene), light_vectors(gPlayerDemoId, o->scene, 1));
}
static void PlayerDemoLight_State_Idle(void);
static void PlayerDemoLight_State_Flash(void);
static void PlayerDemoLight_Action_Idle(PdModel *o) {
    o->light_toggle &= ~1u;
    PlayerDemoLight_Apply(o);
    s_pd.flags &= ~0x2000u;
    o->light_fn = PlayerDemoLight_State_Idle;
}
static void PlayerDemoLight_Action_Flash(PdModel *o) {
    o->light_period = (s8)s_pd.light_period;
    o->light_timer = o->light_period;
    o->col0 = PlayerDemoLight_GetColors(gPlayerDemoId, o->scene);
    o->col1 = PlayerDemoLight_GetColorsAlt(gPlayerDemoId, o->scene);
    s_pd.flags |= 0x2000;
    o->light_fn = PlayerDemoLight_State_Flash;
}
static void PlayerDemoLight_State_Flash(void) {
    PdModel *b = &s_pm;
    if (s_pd.flags & 0x4000) {
        s_pd.flags &= ~0x4000u;
        PlayerDemoLight_Action_Idle(b);
        return;
    }
    b->light_timer--;
    if (b->light_timer <= 0) {
        b->light_timer = b->light_period;
        b->light_toggle ^= 1;
        set_light(b, (b->light_toggle & 1) ? b->col1 : b->col0, NULL);
    }
}
static void PlayerDemoLight_State_Idle(void) {
    if (s_pd.flags & 0x1000) {
        s_pd.flags &= ~0x1000u;
        PlayerDemoLight_Action_Flash(&s_pm);
    }
    if (s_pd.flags & 0x8000) {
        s_pd.flags &= ~0x8000u;
        PlayerDemoLight_ApplyAlt(&s_pm);
    }
}
static void PlayerDemoLight_Init(PdModel *o) {
    PlayerDemoLight_Apply(o);
    PlayerDemoLight_Action_Idle(o);
}
static void PlayerDemoLight_Stop(PdModel *o) {
    PlayerDemoLight_Apply(o);
    o->light_fn = NULL;
    s_pd.flags &= ~0x7000u;
}

/* ------------------------------------------------------------ model task */
static void PlayerDemoModel_State_Play(void);
static void PlayerDemoModel_State_Done(void) {}

static void PlayerDemoModel_ReleaseScene(PdModel *w) {
    int i;
    for (i = 0; i < PD_MAX_MODELS; i++)
        cs_model_free(&w->models[i]);
    cs_anim_free(&w->jnt);
    cs_anim_free(&w->vis);
    for (i = 0; i < 3; i++)
        cs_anim_free(&w->mat[i]);
    memset(w->inst, 0, sizeof(w->inst));
    memset(w->mflags, 0, sizeof(w->mflags));
    w->count = 0;
}

static void PlayerDemoModel_LoadScene(PdModel *w) {
    const PdScene *e = w->ent;
    int j;
    w->count = (s8)(e->count > PD_MAX_MODELS ? PD_MAX_MODELS : e->count);
    cs_anim_load(&w->jnt, e->dca);
    cs_anim_load(&w->vis, e->dcv);
    {
        /* pd_matx parallels pd_scene: archive files 2..4 (data_02075704: slots 1, 2, 3) */
        const PdScene *sc0 = (const PdScene *)cs_sec("pd_scene", NULL);
        const char *mx = (const char *)cs_sec("pd_matx", NULL);
        int k;
        if (sc0 && mx)
            for (k = 0; k < 3; k++)
                if (mx[48 * (3 * (e - sc0) + k)])
                    cs_anim_load(&w->mat[k], mx + 48 * (3 * (e - sc0) + k));
    }
    w->sky = -1;
    for (j = 0; j < w->count; j++) {
        CsInst *in = &w->inst[j];
        const char *f = j == 0 ? e->cam : j == 1 ? e->intm : e->m[j].dcm;
        w->mflags[j] = j < 2 ? 5 : e->m[j].flags;
        if (w->mflags[j] & 2)
            w->sky = (s8)j;
        cs_model_load(&w->models[j], f);
        /* Model_Init: scale 1, identity rotation, origin */
        memset(in, 0, sizeof(*in));
        in->model = &w->models[j];
        in->scale[0] = in->scale[1] = in->scale[2] = 1.0f;
        in->rot[0] = in->rot[4] = in->rot[8] = 1.0f;
        if (w->mflags[j] & 2)
            in->scale[0] = in->scale[1] = in->scale[2] = 0x3ddf10 / 4096.0f;
        if (w->mflags[j] & 4) {
            /* Model_SetAnimation(obj, type, anm[k], j, ...): anim j of the scene's archive */
            in->jnt = &w->jnt;
            in->vis = &w->vis;
            in->jnt_idx = j;
            in->vis_idx = j;
            if (!(w->mflags[j] & 1)) {   /* flags & 1 (cam / int nodes): joint and visibility only */
                in->mat[0] = &w->mat[0];
                in->mat[1] = &w->mat[1];
                in->mat[2] = &w->mat[2];
            }
            in->mat_idx = j;
        } else {
            in->jnt_idx = in->vis_idx = -1;
            in->mat_idx = -1;
        }
    }
    cs_log("pdemo %d scene %d %s models %d", gPlayerDemoId, w->scene, e->dca, w->count);
    s_last_scene = e->dca;
}

/* PlayerDemoModel_UpdateCamera: eye = cam_node, at = cam_int (+ camera shake) */
static void PlayerDemoModel_UpdateCamera(PdModel *w) {
    float off[3] = {0, 0, 0};
    int i;
    if (w->count < 2)
        return;
    if (!cs3d_joint_pos(&w->inst[0], "cam_node", w->cam_a) || !cs3d_joint_pos(&w->inst[1], "cam_int", w->cam_b))
        return;
    if (s_pd.shake_on) {
        off[0] = s_pd.shake[0] / 4096.0f;
        off[1] = s_pd.shake[1] / 4096.0f;
        off[2] = s_pd.shake[2] / 4096.0f;
    }
    for (i = 0; i < 3; i++) {
        w->cam.eye[i] = w->cam_a[i] + off[i];
        w->cam.at[i] = w->cam_b[i] + off[i];
    }
    w->cam.up[0] = 0;
    w->cam.up[1] = 1;
    w->cam.up[2] = 0;
}

static void PlayerDemoModel_StartScene(PdModel *w) {
    const PdScene *sc = (const PdScene *)cs_sec("pd_scene", NULL);
    const PdDemo *d = demo_ent();
    PlayerDemoLight_Init(w);
    if (!sc || !d)
        return;
    w->ent = &sc[d->first + w->scene];
    PlayerDemoModel_LoadScene(w);
    PlayerDemoModel_UpdateCamera(w);
    w->fn = PlayerDemoModel_State_Play;
}
static void PlayerDemoModel_NextScene(PdModel *w) {
    PlayerDemoLight_Stop(w);
    PlayerDemoModel_ReleaseScene(w);
    w->scene++;
    if (w->scene >= w->nscenes) {
        w->fn = PlayerDemoModel_State_Done;
        return;
    }
    PlayerDemoModel_StartScene(w);
}
static void PlayerDemoModel_State_Init(void) {
    PdModel *w = &s_pm;
    const PdDemo *d = demo_ent();
    w->nscenes = d ? (u8)d->count : 0;
    /* PlayerDemoModel_InitProjection: half fovy 0x11c7, aspect 4:3, near 1, far 2048 */
    w->cam.fovy = 0x11c7;
    w->cam.aspect = 0x1555 / 4096.0f;
    w->cam.near_ = 1.0f;
    w->cam.far_ = 2048.0f;
    if (!w->nscenes) {
        w->fn = PlayerDemoModel_State_Done;
        return;
    }
    PlayerDemoModel_StartScene(w);
}
static void PlayerDemoModel_DrawModels(PdModel *w) {
    u8 i;
    dcm_lights_t l;
    cs_lights_from_ds(&l, w->cur_col, (const s16(*)[3])w->cur_vec, &w->cam);
    cs3d_begin(&w->cam, &l);
    for (i = 0; i < (u8)w->count; i++) {
        CsInst *in = &w->inst[i];
        if ((s8)i == w->sky)
            continue;   /* drawn by PlayerDemoModel_DrawCurrentModel at the camera */
        cs3d_add(in);
        in->frame += 1.0f;   /* Model_Animate */
        if (in->jnt && in->jnt->set.hdr && in->jnt_idx < in->jnt->set.hdr->num_anims &&
            in->frame >= in->jnt->set.anims[in->jnt_idx].num_frames)
            in->frame = 0;
    }
}
static void PlayerDemoModel_DrawCurrentModel(PdModel *w) {
    CsInst *in;
    if (w->sky < 0)
        return;
    in = &w->inst[w->sky];
    in->trans[0] = w->cam_a[0];
    in->trans[1] = w->cam_a[1];
    in->trans[2] = w->cam_a[2];
    cs3d_add(in);
    in->frame += 1.0f;
    if (in->jnt && in->jnt->set.hdr && in->jnt_idx < in->jnt->set.hdr->num_anims &&
        in->frame >= in->jnt->set.anims[in->jnt_idx].num_frames)
        in->frame = 0;
}
static void PlayerDemoModel_State_Play(void) {
    PdModel *w = &s_pm;
    if (w->light_fn)
        w->light_fn();
    PlayerDemoModel_DrawModels(w);
    PlayerDemoModel_UpdateCamera(w);
    PlayerDemoModel_DrawCurrentModel(w);
    if (s_pd.flags & 0x100) {
        s_pd.flags &= ~0x100u;
        PlayerDemoModel_NextScene(w);
        return;
    }
    if (s_pd.flags & 0x200) {
        s_pd.flags &= ~0x200u;
        PlayerDemoModel_NextScene(w);
        w->fn = PlayerDemoModel_State_Done;
    }
}
static void PlayerDemoModel_Main(void) {
    if (!s_pd_on) {
        PlayerDemoModel_ReleaseScene(&s_pm);
        s_pm_on = 0;
        return;
    }
    s_pm.counter++;
    if (s_pm.fn)
        s_pm.fn();
}

/* ------------------------------------------------------------ captions (BG3) */
static void PlayerDemoBg_State_Done(void) {}
static void PlayerDemoBg_State_ShowImage(void);
static void PlayerDemoBg_State_WaitShow(void);
static void PlayerDemoBg_State_WaitHide(void);
static void PlayerDemoBg_State_WaitNext(void);
static s32 PlayerDemoBg_GetImageCount(void) {
    const s32 *t = (const s32 *)cs_sec("pd_cap", NULL);
    s32 i = gPlayerDemoId;
    if (!t || i < 0 || i >= 44)
        return 0;
    return t[13 * i];
}
static int PlayerDemoBg_LoadImageRes(CsPic *p, u32 idx) {
    const s32 *t = (const s32 *)cs_sec("pd_cap", NULL), *lb = (const s32 *)cs_sec("pd_capl", NULL);
    char path[200];
    if (!t || !lb || idx >= (u32)t[13 * gPlayerDemoId])
        return -1;
    snprintf(path, sizeof(path), "%s/pldm/poly_m.csp", g_cs_dir);
    return cs_pack_read(p, path, t[13 * gPlayerDemoId + 1 + idx] + lb[cs_lang()]);
}
static void PlayerDemoBg_LoadPalette(PdBg *s) {
    char path[200];
    CsPic p;
    int i;
    snprintf(path, sizeof(path), "%s/pldm/poly_m.csp", g_cs_dir);
    if (cs_pack_read(&p, path, 0) == 0) {
        /* 16 colours of entry 0 at palette 0x1e0 (bank 15) */
        for (i = 0xf0; i < 0x100; i++) {
            int k = i - p.img.pofs;
            s->pal[i] = (k >= 0 && k < p.img.npal) ? p.img.pal[k] : 0;
        }
        cs_pic_free(&p);
    }
}
static void PlayerDemoBg_State_Init(void) {
    PdBg *s = &s_pb;
    s->count = (s8)PlayerDemoBg_GetImageCount();
    if (s->count == 0) {
        s->fn = PlayerDemoBg_State_Done;
        return;
    }
    PlayerDemoBg_LoadPalette(s);
    s->fn = PlayerDemoBg_State_ShowImage;
}
static void PlayerDemoBg_State_ShowImage(void) {
    PdBg *s = &s_pb;
    CsPic p;
    if (!s->tex.argb)
        cs_tex_init(&s->tex, 256, 48);
    cs_tex_clear(&s->tex);
    if (PlayerDemoBg_LoadImageRes(&p, (u32)s->idx) == 0) {
        cs_tex_put(&s->tex, 0, 0, p.img.w < 256 ? p.img.w : 256, p.img.h < 48 ? p.img.h : 48, p.img.px, p.img.w, s->pal);
        cs_pic_free(&p);
        s->loaded = 1;
        cs_log("pdemo %d caption %d", gPlayerDemoId, s->idx);
    }
    s->shown = 0;   /* PlayerDemoBg_HideImage: BG3 y offset 48 */
    s->t54 = 0;
    s->t56 = 6;
    s->fn = PlayerDemoBg_State_WaitShow;
}
static void PlayerDemoBg_State_WaitShow(void) {
    if (s_pd.flags & 0x20) {
        s_pd.flags &= ~0x20u;
        s_pb.shown = 1;   /* PlayerDemoBg_ShowImage */
        s_pb.fn = PlayerDemoBg_State_WaitHide;
    }
}
static void PlayerDemoBg_State_Hide(void) {
    s_pb.shown = 0;
    s_pb.fn = PlayerDemoBg_State_WaitNext;
}
static void PlayerDemoBg_State_WaitHide(void) {
    if (s_pd.flags & 0x40) {
        s_pd.flags &= ~0x40u;
        s_pb.t54 = 6;
        s_pb.t56 = 6;
        s_pb.fn = PlayerDemoBg_State_Hide;
    }
}
static void PlayerDemoBg_State_WaitNext(void) {
    if (s_pd.flags & 0x10) {
        s_pd.flags &= ~0x10u;
        s_pb.idx++;
        if (s_pb.idx >= s_pb.count) {
            s_pb.fn = PlayerDemoBg_State_Done;
            return;
        }
        s_pb.fn = PlayerDemoBg_State_ShowImage;
    }
}
static void PlayerDemoBg_Main(void) {
    if (!s_pd_on) {
        cs_tex_free(&s_pb.tex);
        s_pb_on = 0;
        return;
    }
    if (s_pb.fn)
        s_pb.fn();
}

/* ------------------------------------------------------------ PolyDemoUnder2 */
static void PolyDemoUnder2_LoadSprites(PdUnder *w) {
    const char *ids = (const char *)cs_sec("pd_spr", NULL);
    static const struct { int res, anim; u32 loop; } k[7] = {{1, 0, SPR_LOOP}, {1, 1, SPR_LOOP}, {1, 2, SPR_LOOP},
                                                            {1, 3, SPR_LOOP}, {2, 0, SPR_LOOP}, {2, 1, SPR_LOOP},
                                                            {2, 2, SPR_LOOP}};
    int i;
    if (!ids)
        return;
    w->res1 = cs_res_load(ids);
    w->res2 = cs_res_load(ids + 48 * (1 + cs_lang()));
    for (i = 0; i < 7; i++) {
        AnimatorSprite_Init(&w->spr[i], k[i].res == 1 ? w->res1 : w->res2, (u16)k[i].anim, k[i].loop);
        AnimatorSprite_ProcessAnimation(&w->spr[i]);
    }
}
static void PolyDemoUnder2_Create(void) {
    memset(&s_pu, 0, sizeof(s_pu));
    PolyDemoUnder2_LoadSprites(&s_pu);   /* + PolyDemoUnder2_LoadBg: the panel frame (not shown) */
    s_pu_on = 1;
}
static void PolyDemoUnder2_ReleaseSprites(PdUnder *w) {
    cs_res_free(w->res1);
    cs_res_free(w->res2);
    w->res1 = w->res2 = NULL;
}
/* PolyDemoUnder2_DrawSprites: the skip button (sprites 2/5 blinking 40 of 60
 * frames; 3/6 once the skip started). The logo panel (sprites 0, 1, 4 on the
 * BG frame) is decoration of the second screen and is not composed. */
static int s_under_draw;
static void PolyDemoUnder2_Main(void) {
    PdUnder *w = &s_pu;
    u32 f;
    int i;
    if (!s_pd_on) {
        PolyDemoUnder2_ReleaseSprites(w);
        s_pu_on = 0;
        return;
    }
    for (i = 0; i < 2; i++)
        AnimatorSprite_ProcessAnimation(&w->spr[i]);
    AnimatorSprite_ProcessAnimation(&w->spr[4]);
    s_under_draw = 0;
    f = s_pd.flags;
    if (f & 0x100000) {
        if (f & 0x800000) {
            AnimatorSprite_ProcessAnimation(&w->spr[3]);
            AnimatorSprite_ProcessAnimation(&w->spr[6]);
            s_under_draw = 2;
        } else {
            AnimatorSprite_ProcessAnimation(&w->spr[2]);
            AnimatorSprite_ProcessAnimation(&w->spr[5]);
            w->blink = (s16)((w->blink + 1) % 60);
            s_under_draw = w->blink < 0x28 ? 1 : 3;
        }
    }
}

/* ------------------------------------------------------------ sound */
static void PlayerDemo_LoadSound(PlayerDemo *p) {
    const s32 *e = PlayerDemo_GetSoundEntry();
    p->snd = e;
    if (!snd_ok(e)) {
        p->flags |= 0x10000000;
        return;
    }
    if (!((u32)e[3] & 1))
        cs_snd_bank(e[1]);
}
static void PlayerDemo_ReleaseSound(PlayerDemo *p) {
    if (!(p->flags & 0x10000000)) {
        cs_snd_stop(CSH_PD_SE, 0);
        if (!((u32)p->snd[3] & 2)) {
            cs_snd_stop_all();
            cs_snd_bank(-1);
        }
    }
}
void PlayerDemo_StopBGMForTalk(int seq) {
    /* TalkDemoScene_QueuePageSound mode 0xf: Sound_PlaySeq(&pd->snd0, ...) */
    cs_bgm_stop(0);
    cs_bgm_play(seq);
}
static void PlayerDemoRepeatSE_Main(void) {
    if (!s_rep_on)
        return;
    if (!s_pd_on) {
        s_rep_on = 0;
        return;
    }
    s_rep_timer--;
    if (s_rep_timer <= 0) {
        s_rep_timer = s_rep_period;
        cs_snd_stop(CSH_PD_SE, 0);
        cs_se(CSH_PD_SE, s_rep_arc, s_rep_id);
    }
}
static void PlayerDemo_StartRepeatSE(PlayerDemo *o) {
    const s16 *se = (const s16 *)cs_sec("pd_se", NULL);
    s_rep_on = 1;
    s_rep_period = 0xc;
    s_rep_timer = 0;
    s_rep_arc = (u8)o->snd[1];
    s_rep_id = se ? (u8)se[4] : 0;
}

/* ------------------------------------------------------------ script ops */
static void PlayerDemo_StartCameraShake(PlayerDemo *p) {
    if (p->shake_on)
        return;
    p->shake_on = 1;
    p->shake_timer = 0;   /* timer 0: decremented before the test, runs 0xffff... until the demo ends */
}
static void PlayerDemoCameraShake_Main(void) {
    PlayerDemo *p = &s_pd;
    if (!p->shake_on)
        return;
    p->shake_timer--;
    if (p->shake_timer == 0) {
        p->shake_on = 0;
        p->shake[0] = p->shake[1] = p->shake[2] = 0;
        return;
    }
    p->shake[0] = 0xfff - (cs_rand() & 0x1ffe);
    p->shake[1] = 0xfff - (cs_rand() & 0x1ffe);
    p->shake[2] = 0xfff - (cs_rand() & 0x1ffe);
}
static void PlayerDemoOp_None(PlayerDemo *s, s32 a) { (void)s; (void)a; }
static void PlayerDemoOp_FadeFromWhiteMain(PlayerDemo *s, s32 a) { if (!(s->flags & 0x800000)) PlayerDemo_StartFade((u32)a, 0); }
static void PlayerDemoOp_FadeFromBlackMain(PlayerDemo *s, s32 a) { if (!(s->flags & 0x800000)) PlayerDemo_StartFade((u32)a, 1); }
static void PlayerDemoOp_FadeToWhiteMain(PlayerDemo *s, s32 a) { (void)s; PlayerDemo_StartFade((u32)a, 2); }
static void PlayerDemoOp_FadeToBlackMain(PlayerDemo *s, s32 a) { (void)s; PlayerDemo_StartFade((u32)a, 3); }
static void PlayerDemoOp_FadeFromWhite(PlayerDemo *s, s32 a) { if (!(s->flags & 0x800000)) PlayerDemo_StartFade((u32)a, 4); }
static void PlayerDemoOp_FadeFromBlack(PlayerDemo *s, s32 a) { if (!(s->flags & 0x800000)) PlayerDemo_StartFade((u32)a, 5); }
static void PlayerDemoOp_FadeToWhite(PlayerDemo *s, s32 a) { (void)s; PlayerDemo_StartFade((u32)a, 6); }
static void PlayerDemoOp_FadeToBlack(PlayerDemo *s, s32 a) { (void)s; PlayerDemo_StartFade((u32)a, 7); }
static void PlayerDemoOp_CameraShake(PlayerDemo *s, s32 a) { (void)a; PlayerDemo_StartCameraShake(s); }
static void PlayerDemoOp_StartTalkDemo(PlayerDemo *o, s32 v) {
    const s32 *t = (const s32 *)cs_sec("pd_talk", NULL);
    u8 i;
    if (o->flags & 0x800000)
        return;
    if (v < 0) {
        for (i = 0; t && i < 8; i++)
            if (gPlayerDemoId == t[2 * i]) {
                gTalkDemoId = (s16)t[2 * i + 1];
                break;
            }
    } else {
        gTalkDemoId = (s16)v;
    }
    TalkDemo_Create();
}
static void PlayerDemoOp_ShowBgImage(PlayerDemo *s, s32 a) { (void)a; s->flags |= 0x20; }
static void PlayerDemoOp_NextBgImage(PlayerDemo *s, s32 a) { (void)a; s->flags |= 0x40 | 0x10; }
static void PlayerDemoOp_PlaySE(PlayerDemo *o, s32 n) {
    const s16 *se = (const s16 *)cs_sec("pd_se", NULL);
    if ((o->flags & 0x10000000) || !se || n < 0 || n >= 32)
        return;
    switch (n) {
    case 4:
        PlayerDemo_StartRepeatSE(o);
        /* fall through */
    case 1:
    case 0xd:
        cs_snd_stop(CSH_PD_SE, 0);
        cs_se(CSH_PD_SE, o->snd[1], se[n]);
        return;
    default:
        cs_se(CSH_SE, o->snd[1], se[n]);
        return;
    }
}
static void PlayerDemoOp_PlayVoice(PlayerDemo *s, s32 a) {
    const s16 *v = (const s16 *)cs_sec("pd_voice", NULL);
    if (!(s->flags & 0x10000000) && v && a >= 0 && a < 16)
        cs_voice(s->snd[1], v[a]);
}
static void PlayerDemoOp_NextScene(PlayerDemo *s, s32 a) {
    (void)a;
    s->flags |= 2;
    s->time = 0;
}
static void PlayerDemoOp_LightFlash(PlayerDemo *s, s32 v) {
    s->flags |= 0x1000;
    s->light_period = (u16)v;
}
static void PlayerDemoOp_LightAlt(PlayerDemo *s, s32 a) { (void)a; s->flags |= 0x8000; }
static void PlayerDemoOp_PlayBGM(PlayerDemo *p, s32 a) {
    const s16 *bgm = (const s16 *)cs_sec("pd_bgm", NULL);
    if (!(p->flags & 0x10000000) && a >= 0 && a < 0xc && bgm)
        cs_bgm_play(bgm[a]);
}
static void PlayerDemoOp_StopBGM(PlayerDemo *p, s32 a) {
    if (!(p->flags & 0x10000000))
        cs_bgm_stop(a < 0 ? 0 : a);
}
static void PlayerDemoOp_StopSE(PlayerDemo *p, s32 a) {
    if (!(p->flags & 0x10000000))
        cs_snd_stop(CSH_PD_SE, a < 0 ? 0 : a);
}
/* gPlayerDemoOpTable */
static void (*const gPlayerDemoOpTable[21])(PlayerDemo *, s32) = {
    PlayerDemoOp_None, PlayerDemoOp_FadeFromWhiteMain, PlayerDemoOp_FadeFromBlackMain, PlayerDemoOp_FadeToWhiteMain, PlayerDemoOp_FadeToBlackMain, PlayerDemoOp_FadeFromWhite,
    PlayerDemoOp_FadeFromBlack, PlayerDemoOp_FadeToWhite, PlayerDemoOp_FadeToBlack, PlayerDemoOp_CameraShake, PlayerDemoOp_StartTalkDemo, PlayerDemoOp_ShowBgImage,
    PlayerDemoOp_NextBgImage, PlayerDemoOp_PlaySE, PlayerDemoOp_PlayVoice, PlayerDemoOp_NextScene, PlayerDemoOp_LightFlash, PlayerDemoOp_LightAlt, PlayerDemoOp_PlayBGM, PlayerDemoOp_StopBGM,
    PlayerDemoOp_StopSE};
static const char *const k_op_names[21] = {
    "None", "FadeFromWhiteMain", "FadeFromBlackMain", "FadeToWhiteMain", "FadeToBlackMain", "FadeFromWhite",
    "FadeFromBlack", "FadeToWhite", "FadeToBlack", "CameraShake", "StartTalkDemo", "ShowBgImage", "NextBgImage",
    "PlaySE", "PlayVoice", "NextScene", "LightFlash", "LightAlt", "PlayBGM", "StopBGM", "StopSE"};

/* ------------------------------------------------------------ main task */
static void PlayerDemo_State_RunScript(PlayerDemo *s);
static const PdCmd *PlayerDemo_GetCommand(const u8 *script, u32 i) { return (const PdCmd *)(script + 0x14 + 16 * i); }
static void PlayerDemo_State_Finish(PlayerDemo *s) {
    s->fn = NULL;
    s->flags |= 0x80000000u;
}
static void PlayerDemo_State_ScriptDoneWait(PlayerDemo *s) { s->fn = PlayerDemo_State_Finish; }
static void PlayerDemo_State_ScriptDone(PlayerDemo *s) { s->fn = PlayerDemo_State_ScriptDoneWait; }

static u8 *PlayerDemo_LoadData(s32 a, u32 *size) {
    const s32 *idx = (const s32 *)cs_sec("pd_bpd", NULL);
    const char *names = (const char *)cs_sec("pd_bpdn", NULL);
    char path[200];
    if (a == 0 || a >= 0x2c || !idx || !names || !names[24 * idx[a]])
        return NULL;
    snprintf(path, sizeof(path), "%s/pldm/%s.bpd", g_cs_dir, names + 24 * idx[a]);
    return (u8 *)cs_load_file(path, size);
}
static void PlayerDemo_State_Load(PlayerDemo *p) {
    p->script = PlayerDemo_LoadData(gPlayerDemoId, &p->script_size);
    if (!p->script || p->script_size < 0x14 || memcmp(p->script, "#BPD", 4) != 0) {
        cs_log("pdemo %d: script missing", gPlayerDemoId);
        free(p->script);
        p->script = NULL;
        p->flags |= 0x80000000u;
        return;
    }
    p->count = (u16)(((const s32 *)p->script)[3]);
    p->cmd = PlayerDemo_GetCommand(p->script, p->idx);
    cs_log("pdemo %d start cmds %d", gPlayerDemoId, p->count);
    /* PlayerDemoBg_Create, PlayerDemoModel_Create, PolyDemoUnder2_Create */
    memset(&s_pb, 0, sizeof(s_pb));
    s_pb.fn = PlayerDemoBg_State_Init;
    s_pb_on = 1;
    memset(&s_pm, 0, sizeof(s_pm));
    s_pm.demo = (s8)gPlayerDemoId;
    s_pm.fn = PlayerDemoModel_State_Init;
    s_pm_on = 1;
    PolyDemoUnder2_Create();
    PlayerDemo_LoadSound(p);
    p->flags |= 1;
    p->fn = PlayerDemo_State_RunScript;
}
static void PlayerDemo_State_RunScript(PlayerDemo *s) {
    for (;;) {
        const PdCmd *c = s->cmd;
        if (s->time < (u32)c->time)
            return;
        cs_log("pdemo %d cmd %d t %d %s %d", gPlayerDemoId, s->idx, c->time,
               c->op >= 0 && c->op < 21 ? k_op_names[c->op] : "?", c->arg);
        if (c->op >= 0 && c->op < 21)
            gPlayerDemoOpTable[c->op](s, c->arg);
        s->idx++;
        if (s->idx >= s->count) {
            s->flags |= 4;
            s->flags |= 0x200;
            s->fn = PlayerDemo_State_ScriptDone;
            return;
        }
        s->cmd = PlayerDemo_GetCommand(s->script, s->idx);
        if (s->flags & 2) {
            s->scenes++;
            s->flags &= ~2u;
            s->flags |= 0x100;
            return;
        }
    }
}
static void PlayerDemo_InitSkippable(PlayerDemo *p) {
    switch (gPlayerDemoId) {
    case 0:
        break;
    case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9: case 10: case 11: case 12: case 13: case 14:
        p->flags |= 0x100000;
        break;
    case 0x28: case 0x29: case 0x2a:
        if (g_cs_story.save_flags & 0x1000)
            p->flags |= 0x100000;
        break;
    }
}
static void PlayerDemo_StartSkip(PlayerDemo *p) {
    (void)p;
    s_skip_on = 1;
    s_skip_timer = 0x3c;
    PlayerDemo_StartFade(0x3c, 7);
    cs_bgm_stop(0x3c);
    cs_snd_stop(CSH_PD_SE, 0x3c);
    cs_log("pdemo %d skip", gPlayerDemoId);
}
static void PlayerDemoSkip_Main(void) {
    if (!s_skip_on)
        return;
    if (!s_pd_on) {
        s_skip_on = 0;
        return;
    }
    if (--s_skip_timer <= 0) {
        s_pd.flags |= 0x80000000u;
        s_skip_on = 0;
    }
}
static void PlayerDemo_CheckSkip(PlayerDemo *p) {
    if (TalkDemo_IsActive())
        return;
    if ((p->flags & 0x100000) && (gPad.trg & PAD_START))
        p->flags |= 0x200000;
    if (p->flags & 0x200000) {
        if (!(p->flags & 0x400000)) {
            PlayerDemo_StartSkip(p);
            p->flags |= 0x800000;
        }
        p->flags |= 0x400000;
    }
}

static void PlayerDemo_Destructor(void) {
    PlayerDemo *p = &s_pd;
    PlayerDemo_ReleaseSound(p);
    free(p->script);
    p->script = NULL;
    BrightnessFade_Destroy();
    s_pd_on = 0;
    /* the subtasks see PlayerDemo_GetWork() == NULL next frame and destroy themselves */
    PlayerDemoModel_Main();
    PlayerDemoBg_Main();
    PolyDemoUnder2_Main();
    s_rep_on = 0;
    s_skip_on = 0;
    cs_log("pdemo %d end scenes %d", gPlayerDemoId, p->scenes);
}

void PlayerDemo_Start(void) {
    PlayerDemo_Kill();
    memset(&s_pd, 0, sizeof(s_pd));
    s_pd.lang = cs_lang();
    s_pd.fn = PlayerDemo_State_Load;
    s_pd_on = 1;
    PlayerDemo_InitSkippable(&s_pd);
    /* PlayerDemo_InitDisplay: 3D on BG0, captions on BG3 */
}
void PlayerDemo_Kill(void) {
    if (s_pd_on) {
        free(s_pd.script);
        s_pd.script = NULL;
    }
    s_pd_on = 0;
    if (s_pm_on)
        PlayerDemoModel_ReleaseScene(&s_pm);
    cs_tex_free(&s_pb.tex);
    PolyDemoUnder2_ReleaseSprites(&s_pu);
    s_pm_on = s_pb_on = s_pu_on = 0;
    s_rep_on = s_skip_on = 0;
}

void PlayerDemo_RunFrame(void) {
    PlayerDemo *t = &s_pd;
    if (!s_pd_on)
        return;
    if (t->flags & 0x80000000u) {
        PlayerDemo_Destructor();
        return;
    }
    t->counter++;
    if (t->flags & 1)
        t->time++;
    PlayerDemo_CheckSkip(t);
    if (t->fn)
        t->fn(t);
    /* subtasks (0x3efe, creation order), camera shake, repeat SE (7), skip (0x3efd) */
    PlayerDemoSkip_Main();
    if (s_pb_on)
        PlayerDemoBg_Main();
    if (s_pm_on)
        PlayerDemoModel_Main();
    if (s_pu_on)
        PolyDemoUnder2_Main();
    PlayerDemoCameraShake_Main();
    PlayerDemoRepeatSE_Main();
    if (t->flags & 0x80000000u)
        PlayerDemo_Destructor();
}

void PlayerDemo_Draw(void) {
    if (!s_pd_on)
        return;
    g_cs_backdrop = 0xff000000;
    cs_view_full();
    /* BG3 caption strip at y 144 (PlayerDemoBg_ShowImage) */
    if (s_pb.shown && s_pb.tex.argb)
        cs_quad_tex(&s_pb.tex, 0, 144, 256, 48, 0, 0, 256, 48, 0xffffffff);
    /* skip button of the bottom screen: source (64,118)-(192,148) at x1.5, top right */
    if (s_under_draw == 1 || s_under_draw == 2) {
        cs_view_set(64, 118, 1.5f, 640 - 192 - 24, 18);
        AnimatorSprite_Draw(&s_pu.spr[s_under_draw == 1 ? 2 : 3]);
        AnimatorSprite_Draw(&s_pu.spr[s_under_draw == 1 ? 5 : 6]);
        cs_view_full();
    }
}
