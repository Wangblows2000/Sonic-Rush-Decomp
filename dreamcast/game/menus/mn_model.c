/*
 * The menus' 3D layer (DS BG0 of an engine): the character models the front end draws with
 * Model_Draw under an orthographic camera (ZoneMap_DrawCharModel; the same path suits the other
 * front-end models).  A scene adds instances during its task frame (mn_3d_add); the layer sits in
 * the DS compositing order at BG0's place for the given priority (mn_sorted puts an MNQ_3D marker
 * there, menus_draw_tr leaves a depth gap at the marker) and menus_draw_list() submits the
 * models into the OP / PT / TR lists with their PVR depth inside that gap, so BGs and sprites of
 * lower priority stay behind them and the ones of higher priority in front, as on the DS.
 *
 * Resources (/cd/models, the cutscene list cutscene/tools/csdata.py writes): the player models and
 * animations (mod/son/son.nsbmd, mod/blz/blz.nsbmd, narc/player_lz7.narc son.nsbca / son.nsbva) and
 * narc/menu.narc's menu.nsbca / menu.nsbva (the "decided" poses son_ps_dec / blz_ps_dec), loaded on
 * first use and freed with mn_3d_free().
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "mn_internal.h"
#include "../model/model.h"

char g_mn_models_dir[96] = "/cd/models";
#ifdef _arch_dreamcast
#define MN_UPLOAD 1
#else
#define MN_UPLOAD 0   /* host tests: no textures */
#endif

typedef struct {
    u8 *buf;
    dcm_model_t m;
    int ok;
} MnMdlFile;
typedef struct {
    u8 *buf;
    dca_set_t s;
    int ok;
} MnAnmFile;

static MnMdlFile s_mdl[2];        /* 0 Sonic, 1 Blaze */
static MnAnmFile s_anm[4];        /* 0/1 player joint / visibility, 2/3 menu.narc */
static int s_tried[6];

#define MN_3D_MAX 4
typedef struct {
    int mdl;                      /* 0 Sonic, 1 Blaze */
    int jset, jidx, vset, vidx;   /* animation file (0 player, 1 menu) and index */
    s32 jframe, vframe;           /* fx32 frames: past the end -> last frame unless loop */
    int loop;
    int alpha;                    /* polygon alpha 1..31 of every material */
    float t, b, l, r, n, f;       /* G3_OrthoW of the scene (DS px around the screen centre) */
    float eye_z;                  /* LookAt eye (0, 0, eye_z) -> origin, up (0, 1, 0) */
    float pos[3], scale;          /* model translation (view units) and uniform scale */
    u16 rot_y, rot_x;
    u16 lcol[3];                  /* NNS_G3dGlbLightColor 0..2 (BGR555) */
    s16 lvec[3][3];               /* NNS_G3dGlbLightVector 0..2 */
} MnInst3D;

static MnInst3D s_inst[MN_3D_MAX];
static int s_ninst;
MnLayer3D g_mn_3d;

void mn_3d_reset(void) { s_ninst = 0; }
int mn_3d_count(void) { return s_ninst; }

static int load_file_into(const char *name, u8 **buf, u32 *size) {
    char p[160];
    snprintf(p, sizeof(p), "%s/%s", g_mn_models_dir, name);
    return mn_load_file(p, buf, size);
}

/* ZoneMap_SetupLighting: NNS_G3dGlbPolygonAttr(light mask 1, modulate, cull back, id 0, alpha 31) and
 * NNS_G3dGlbMaterialColorDiffAmb(0x7fff, 0x5ef7) with NNSi_G3dModifyPolygonAttrMask(0xf light,
 * 0x1f0000 alpha) / NNSi_G3dModifyMatFlag(0x40 diffuse, 0x80 ambient) cleared: every material of the
 * model takes the global light mask (light 0 only), alpha and colours instead of its own */
static void zonemap_materials(dcm_model_t *m) {
    dcm_material_t *mat = (dcm_material_t *)m->materials;
    int k;
    for (k = 0; k < m->hdr->num_materials; k++) {
        mat[k].light_mask = 1;
        mat[k].diffuse = (mat[k].diffuse & 0xff000000u) | 0xffffff;
        mat[k].ambient = (mat[k].ambient & 0xff000000u) | 0xbdbdbd;   /* 0x5ef7: 23 / 31 per channel */
    }
}

static const dcm_model_t *model_res(int i) {
    static const char *const names[2] = {"mod__son__son.dcm", "mod__blz__blz.dcm"};
    u32 size;
    if (i < 0 || i > 1)
        return NULL;
    if (!s_mdl[i].ok && !s_tried[i]) {
        s_tried[i] = 1;
        if (load_file_into(names[i], &s_mdl[i].buf, &size) == 0 &&
            dcm_model_load(&s_mdl[i].m, s_mdl[i].buf, size, MN_UPLOAD) == 0) {
            s_mdl[i].ok = 1;
            zonemap_materials(&s_mdl[i].m);
        } else
            printf("menus: model %s missing\n", names[i]);
    }
    return s_mdl[i].ok ? &s_mdl[i].m : NULL;
}
static const dca_set_t *anim_res(int i) {
    static const char *const names[4] = {"narc__player_lz7__son.dca", "narc__player_lz7__son.dcv",
                                         "narc__menu__menu.dca", "narc__menu__menu.dcv"};
    u32 size;
    if (i < 0 || i > 3)
        return NULL;
    if (!s_anm[i].ok && !s_tried[2 + i]) {
        s_tried[2 + i] = 1;
        if (load_file_into(names[i], &s_anm[i].buf, &size) == 0 && dca_set_load(&s_anm[i].s, s_anm[i].buf, size) == 0)
            s_anm[i].ok = 1;
        else
            printf("menus: animation set %s missing\n", names[i]);
    }
    return s_anm[i].ok ? &s_anm[i].s : NULL;
}

void mn_3d_free(void) {
    int i;
    for (i = 0; i < 2; i++) {
        if (s_mdl[i].ok)
            dcm_model_free(&s_mdl[i].m);
        free(s_mdl[i].buf);
        memset(&s_mdl[i], 0, sizeof(s_mdl[i]));
    }
    for (i = 0; i < 4; i++) {
        free(s_anm[i].buf);
        memset(&s_anm[i], 0, sizeof(s_anm[i]));
    }
    memset(s_tried, 0, sizeof(s_tried));
    s_ninst = 0;
}

/* ZoneMap_DrawCharModel: ZoneMap_SetupLighting + G3_OrthoW(top 95, bottom -96, left -128, right 127,
 * near 1, far 200) + LookAt (0, 0, 100) -> 0, scale 4, translation (x - 128, 95 - y, 0) for the DS
 * screen position (x, y), RotY(rot_y) * RotX(rot_x); joint / visibility animation of the player
 * archive (set 0) or of menu.narc (set 1, the zone map's "decided" pose). */
void mn_3d_add_zonemap_char(int chara, s32 x, s32 y, u16 rot_y, int set, int jidx, int vidx, s32 frame, int loop,
                             int alpha) {
    MnInst3D *in;
    if (s_ninst >= MN_3D_MAX)
        return;
    in = &s_inst[s_ninst++];
    memset(in, 0, sizeof(*in));
    in->mdl = chara ? 1 : 0;
    in->jset = in->vset = set;
    in->jidx = jidx;
    in->vidx = vidx;
    in->jframe = in->vframe = frame;
    in->loop = loop;
    in->alpha = alpha;
    in->t = 95.0f;
    in->b = -96.0f;
    in->l = -128.0f;
    in->r = 127.0f;
    in->n = 1.0f;
    in->f = 200.0f;
    in->eye_z = 100.0f;
    in->pos[0] = -128.0f + x / 4096.0f;
    in->pos[1] = 95.0f - y / 4096.0f;
    in->pos[2] = 0.0f;
    in->scale = 4.0f;
    in->rot_y = rot_y;
    in->rot_x = 0;
    /* ZoneMap_SetupLighting: lights 0..2 (light 3 is black) */
    in->lcol[0] = 0x7fff;
    in->lcol[1] = 0x2108;
    in->lcol[2] = 0x2108;
    in->lvec[0][0] = -4096; in->lvec[0][1] = -4096; in->lvec[0][2] = -4096;
    in->lvec[1][0] = 0xfff; in->lvec[1][1] = -4096; in->lvec[1][2] = -4096;
    in->lvec[2][0] = 0xfff; in->lvec[2][1] = 0xfff; in->lvec[2][2] = -4096;
}

/* the depth gap the 3D layer has in the menus' TR order: [z0, z1] (menus_draw_tr adds
 * MN_3D_GAP at the MNQ_3D marker); 0 when the layer is not in this frame's composition */
int mn_3d_band(float *z0, float *z1) {
    const MnDraw *s;
    int n = mn_sorted(&s), i;
    for (i = 0; i < n; i++)
        if (s[i].q->kind == MNQ_3D) {
            *z0 = 10.0f + 0.005f * (float)i + 0.001f;
            *z1 = *z0 + MN_3D_GAP - 0.002f;
            return 1;
        }
    return 0;
}

/* clamp / wrap an fx32 frame into the animation's frames */
static float anim_frame(s32 fx, int nf, int loop) {
    u32 f = (u32)fx >> 12;
    if (nf <= 0)
        return 0.0f;
    if (loop)
        f %= nf;
    else if (f > (u32)nf - 1)
        f = (u32)nf - 1;
    return (float)f;
}

#ifdef _arch_dreamcast
#include <kos.h>
#define MN_PT_LIST PVR_LIST_PT_POLY
#else
#define MN_PT_LIST 2   /* host: dcm_draw only transforms (dcm emit hook) */
#endif

static const MnRegion *region_of(int engine) {
    int i;
    for (i = 0; i < g_mn_comp.n; i++)
        if (g_mn_comp.r[i].engine == engine && !(g_mn_comp.r[i].hide_bg & 1))
            return &g_mn_comp.r[i];
    return NULL;
}

void menus_draw_list(int list) {
    static dcm_local_t loc[DCM_MAX_JOINTS];
    static dcm_mtx_t pal[DCM_MAX_PALETTE];
    static uint8_t visb[DCM_MAX_JOINTS];
    float z0, z1;
    int i, k;
    const MnRegion *rg;
    if (s_ninst == 0 || !g_mn_3d.on || !mn_3d_band(&z0, &z1) || !(rg = region_of(g_mn_3d.engine)))
        return;
    for (i = 0; i < s_ninst; i++) {
        const MnInst3D *in = &s_inst[i];
        const dcm_model_t *m = model_res(in->mdl);
        const dca_set_t *js = anim_res(in->jset ? 2 : 0), *vs = anim_res(in->vset ? 3 : 1);
        dcm_camera_t cam;
        dcm_mtx_t mm;
        dcm_lights_t l;
        dcm_draw_opts_t o;
        float zn, zf, cn, cf, R;
        if (!m || !m->hdr)
            continue;
        {   /* NNS_G3dGlbPolygonAttr alpha (the materials take the global one): 31 opaque, below in the TR list */
            dcm_material_t *mat = (dcm_material_t *)m->materials;
            for (k = 0; k < m->hdr->num_materials; k++)
                mat[k].alpha = (uint8_t)in->alpha;
        }
        if (js && in->jidx < js->hdr->num_anims)
            dca_sample(js, in->jidx, anim_frame(in->jframe, js->anims[in->jidx].num_frames, in->loop), m, loc);
        else
            dcm_bind_locals(m, loc);
        dcm_pose(m, loc, pal);
        if (vs && in->vidx < vs->hdr->num_anims)
            dca_visibility(vs, in->vidx, anim_frame(in->vframe, vs->anims[in->vidx].num_frames, in->loop), m, visb);
        else
            memset(visb, 1, m->hdr->num_joints);
        /* G3_OrthoW + LookAt (0, 0, eye_z) -> 0: the view is a translation by -eye_z */
        dcm_camera_ortho(&cam, in->t, in->b, in->l, in->r, in->n, in->f);
        for (k = 0; k < 3; k++)
            memset(cam.view.m[k], 0, sizeof(cam.view.m[k]));
        cam.view.m[0][0] = cam.view.m[1][1] = cam.view.m[2][2] = 1.0f;
        cam.view.m[3][0] = cam.view.m[3][1] = 0.0f;
        cam.view.m[3][2] = -in->eye_z;
        /* the DS screen px (l..r, t..b) through the engine's region of the composition */
        dcm_camera_set_viewport(&cam, rg->dx + (0.0f - rg->sx0) * rg->scale, rg->dy + (0.0f - rg->sy0) * rg->scale,
                                (in->r - in->l) * rg->scale, (in->t - in->b) * rg->scale);
        /* PVR depth (orthographic: 1 / (ndc z + 2)) mapped into the layer's gap: the model's view
         * depth -eye_z +- R onto [z0, z1] */
        R = 24.0f * in->scale;
        zn = -in->eye_z + R;
        zf = -in->eye_z - R;
        cn = 1.0f / z1 - 2.0f;
        cf = 1.0f / z0 - 2.0f;
        cam.proj[2][2] = (cf - cn) / (zf - zn);
        cam.proj[3][2] = cn - cam.proj[2][2] * zn;
        /* Model_Draw: v * S * RotY * RotX + translation */
        dcm_stage_model_matrix(&mm, in->scale, in->rot_y, 0, in->pos[0], in->pos[1], in->pos[2]);
        if (in->rot_x) {
            float a = (float)in->rot_x * (6.28318530718f / 65536.0f), c = cosf(a), sn = sinf(a);
            dcm_mtx_t rx, t2;
            memset(&rx, 0, sizeof(rx));
            rx.m[0][0] = 1.0f;
            rx.m[1][1] = c;
            rx.m[1][2] = sn;
            rx.m[2][1] = -sn;
            rx.m[2][2] = c;
            t2 = mm;
            t2.m[3][0] = t2.m[3][1] = t2.m[3][2] = 0.0f;
            dcm_mtx_mul(&mm, &rx, &t2);
            mm.m[3][0] = in->pos[0];
            mm.m[3][1] = in->pos[1];
            mm.m[3][2] = in->pos[2];
        }
        memset(&l, 0, sizeof(l));
        l.count = 3;
        for (k = 0; k < 3; k++) {
            l.dir[k][0] = in->lvec[k][0] / 4096.0f;
            l.dir[k][1] = in->lvec[k][1] / 4096.0f;
            l.dir[k][2] = in->lvec[k][2] / 4096.0f;
            l.col[k][0] = (in->lcol[k] & 31) / 31.0f;
            l.col[k][1] = (in->lcol[k] >> 5 & 31) / 31.0f;
            l.col[k][2] = (in->lcol[k] >> 10 & 31) / 31.0f;
        }
        o.list = list;
        o.pt_list = MN_PT_LIST;
        o.alpha_scale = 255;
        dcm_draw(m, pal, &mm, &cam, visb, &l, &o);
    }
}
