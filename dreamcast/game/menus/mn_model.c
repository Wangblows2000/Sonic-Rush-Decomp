/*
 * The menus' 3D layer (DS BG0 of an engine): the character models the front end draws with
 * Model_Draw under an orthographic camera (ZoneMap_DrawCharModel, TitleDemo_DrawSonicModel /
 * DrawBlazeModel).  A scene adds instances during its task frame (mn_3d_add); the layer sits in
 * the DS compositing order at BG0's place for the given priority (mn_sorted puts an MNQ_3D marker
 * there, menus_draw_tr leaves a depth gap at the marker) and menus_draw_list() submits the
 * models into the OP / PT / TR lists with their PVR depth inside that gap, so BGs and sprites of
 * lower priority stay behind them and the ones of higher priority in front, as on the DS.
 *
 * Resources (/cd/models, the cutscene list cutscene/tools/csdata.py writes): the player models and
 * animations (mod/son/son.nsbmd, mod/blz/blz.nsbmd, narc/player_lz7.narc son.nsbca / son.nsbva),
 * narc/menu.narc's menu.nsbca / menu.nsbva (the zone map's "decided" poses son_ps_dec / blz_ps_dec)
 * and narc/menu_ex.narc's (the title demo's poses), loaded on first use and freed with mn_3d_free().
 *
 * The global state the scenes set before Model_Draw (NNS_G3dGlbPolygonAttr light mask / cull /
 * alpha, NNS_G3dGlbMaterialColorDiffAmb with NNSi_G3dModifyMatFlag / ModifyPolygonAttrMask cleared:
 * every material takes the global values) is applied to the materials at each draw.  Not done: the
 * DS edge marking (outline).
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

#define MN_NANM 6
static MnMdlFile s_mdl[2];        /* 0 Sonic, 1 Blaze */
static MnAnmFile s_anm[MN_NANM];  /* joint / visibility pairs: 0/1 player, 2/3 menu.narc, 4/5 menu_ex.narc */
static int s_tried[2 + MN_NANM];

#define MN_3D_MAX 4
static MnModelDesc s_inst[MN_3D_MAX];
static int s_ninst;
MnLayer3D g_mn_3d;

void mn_3d_reset(void) { s_ninst = 0; }
int mn_3d_count(void) { return s_ninst; }

static int load_file_into(const char *name, u8 **buf, u32 *size) {
    char p[160];
    snprintf(p, sizeof(p), "%s/%s", g_mn_models_dir, name);
    return mn_load_file(p, buf, size);
}

static const dcm_model_t *model_res(int i) {
    static const char *const names[2] = {"mod__son__son.dcm", "mod__blz__blz.dcm"};
    u32 size;
    if (i < 0 || i > 1)
        return NULL;
    if (!s_mdl[i].ok && !s_tried[i]) {
        s_tried[i] = 1;
        if (load_file_into(names[i], &s_mdl[i].buf, &size) == 0 &&
            dcm_model_load(&s_mdl[i].m, s_mdl[i].buf, size, MN_UPLOAD) == 0)
            s_mdl[i].ok = 1;
        else
            printf("menus: model %s missing\n", names[i]);
    }
    return s_mdl[i].ok ? &s_mdl[i].m : NULL;
}
static const dca_set_t *anim_res(int i) {
    static const char *const names[MN_NANM] = {"narc__player_lz7__son.dca", "narc__player_lz7__son.dcv",
                                               "narc__menu__menu.dca", "narc__menu__menu.dcv",
                                               "narc__menu_ex__menu_ex.dca", "narc__menu_ex__menu_ex.dcv"};
    u32 size;
    if (i < 0 || i >= MN_NANM)
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
    for (i = 0; i < MN_NANM; i++) {
        free(s_anm[i].buf);
        memset(&s_anm[i], 0, sizeof(s_anm[i]));
    }
    memset(s_tried, 0, sizeof(s_tried));
    s_ninst = 0;
}

int mn_3d_anim_frames(int set, int idx) {
    const dca_set_t *s = anim_res(set * 2);
    if (!s || idx < 0 || idx >= s->hdr->num_anims)
        return 0;
    return s->anims[idx].num_frames;
}

void mn_3d_add(const MnModelDesc *d) {
    if (s_ninst < MN_3D_MAX)
        s_inst[s_ninst++] = *d;
}

/* ZoneMap_DrawCharModel: ZoneMap_SetupLighting (light mask 1, cull back, the talk fade's alpha, global
 * colours 0x7fff / 0x5ef7, lights 0..2 of data_ov001_0231628c) + G3_OrthoW(top 95, bottom -96, left
 * -128, right 127, near 1, far 200) + LookAt (0, 0, 100) -> 0, scale 4, translation (x - 128, 95 - y,
 * 0) for the DS screen position (x, y), RotY(rot_y); joint / visibility animation of the player archive
 * (set 0) or of menu.narc (set 1, the zone map's "decided" pose). */
void mn_3d_add_zonemap_char(int chara, s32 x, s32 y, u16 rot_y, int set, int jidx, int vidx, s32 frame, int loop,
                             int alpha) {
    MnModelDesc d;
    memset(&d, 0, sizeof(d));
    d.mdl = chara ? 1 : 0;
    d.set = set;
    d.jidx = jidx;
    d.vidx = vidx;
    d.frame = frame;
    d.loop = loop;
    d.alpha = alpha;
    d.t = 95.0f;
    d.b = -96.0f;
    d.l = -128.0f;
    d.r = 127.0f;
    d.n = 1.0f;
    d.f = 200.0f;
    d.eye[2] = 100.0f;
    d.pos[0] = -128.0f + x / 4096.0f;
    d.pos[1] = 95.0f - y / 4096.0f;
    d.scale[0] = d.scale[1] = d.scale[2] = 4.0f;
    d.rot_y = rot_y;
    d.cull = 2;
    d.diff = 0x7fff;
    d.amb = 0x5ef7;
    d.nlights = 3;
    d.lcol[0] = 0x7fff;
    d.lcol[1] = 0x2108;
    d.lcol[2] = 0x2108;
    d.lvec[0][0] = -4096; d.lvec[0][1] = -4096; d.lvec[0][2] = -4096;
    d.lvec[1][0] = 0xfff; d.lvec[1][1] = -4096; d.lvec[1][2] = -4096;
    d.lvec[2][0] = 0xfff; d.lvec[2][1] = 0xfff; d.lvec[2][2] = -4096;
    mn_3d_add(&d);
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

static u32 rgb555_888(u16 c) {
    u32 r = c & 31, g = (c >> 5) & 31, b = (c >> 10) & 31;
    return (r * 255 / 31) << 16 | (g * 255 / 31) << 8 | (b * 255 / 31);
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

/* LookAt(eye, at, up (0, 1, 0)) as a row-vector view matrix */
static void look_at(dcm_mtx_t *v, const float *eye, const float *at) {
    float z[3], x[3], y[3], l;
    int k;
    for (k = 0; k < 3; k++)
        z[k] = eye[k] - at[k];
    l = sqrtf(z[0] * z[0] + z[1] * z[1] + z[2] * z[2]);
    if (l <= 0.0f) {
        z[0] = z[1] = 0.0f;
        z[2] = 1.0f;
    } else {
        for (k = 0; k < 3; k++)
            z[k] /= l;
    }
    /* x = up x z, up = (0, 1, 0) */
    x[0] = z[2];
    x[1] = 0.0f;
    x[2] = -z[0];
    l = sqrtf(x[0] * x[0] + x[2] * x[2]);
    if (l > 0.0f) {
        x[0] /= l;
        x[2] /= l;
    } else {
        x[0] = 1.0f;
    }
    y[0] = z[1] * x[2] - z[2] * x[1];
    y[1] = z[2] * x[0] - z[0] * x[2];
    y[2] = z[0] * x[1] - z[1] * x[0];
    for (k = 0; k < 3; k++) {
        v->m[k][0] = x[k];
        v->m[k][1] = y[k];
        v->m[k][2] = z[k];
    }
    v->m[3][0] = -(x[0] * eye[0] + x[1] * eye[1] + x[2] * eye[2]);
    v->m[3][1] = -(y[0] * eye[0] + y[1] * eye[1] + y[2] * eye[2]);
    v->m[3][2] = -(z[0] * eye[0] + z[1] * eye[1] + z[2] * eye[2]);
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
        const MnModelDesc *in = &s_inst[i];
        const dcm_model_t *m = model_res(in->mdl);
        const dca_set_t *js = anim_res(in->set * 2), *vs = anim_res(in->set * 2 + 1);
        dcm_camera_t cam;
        dcm_mtx_t mm;
        dcm_lights_t l;
        dcm_draw_opts_t o;
        float zn, zf, cn, cf, sy, cy;
        if (!m || !m->hdr)
            continue;
        {   /* the global polygon attributes and material colours */
            dcm_material_t *mat = (dcm_material_t *)m->materials;
            u32 dif = rgb555_888(in->diff), amb = rgb555_888(in->amb);
            for (k = 0; k < m->hdr->num_materials; k++) {
                mat[k].light_mask = 1;
                mat[k].alpha = (uint8_t)(in->alpha > 0 ? in->alpha : 31);
                mat[k].diffuse = (mat[k].diffuse & 0xff000000u) | dif;
                mat[k].ambient = (mat[k].ambient & 0xff000000u) | amb;
                mat[k].flags &= (uint16_t)~(DCM_MF_CULL_BACK | DCM_MF_CULL_FRONT);
                if (in->cull == 1)
                    mat[k].flags |= DCM_MF_CULL_FRONT;
                else if (in->cull == 2)
                    mat[k].flags |= DCM_MF_CULL_BACK;
            }
        }
        if (js && in->jidx < js->hdr->num_anims)
            dca_sample(js, in->jidx, anim_frame(in->frame, js->anims[in->jidx].num_frames, in->loop), m, loc);
        else
            dcm_bind_locals(m, loc);
        dcm_pose(m, loc, pal);
        if (vs && in->vidx < vs->hdr->num_anims)
            dca_visibility(vs, in->vidx, anim_frame(in->frame, vs->anims[in->vidx].num_frames, in->loop), m, visb);
        else
            memset(visb, 1, m->hdr->num_joints);
        dcm_camera_ortho(&cam, in->t, in->b, in->l, in->r, in->n, in->f);
        look_at(&cam.view, in->eye, in->at);
        /* the DS screen px (l..r, t..b) through the engine's region of the composition */
        dcm_camera_set_viewport(&cam, rg->dx + (0.0f - rg->sx0) * rg->scale, rg->dy + (0.0f - rg->sy0) * rg->scale,
                                256.0f * rg->scale, 192.0f * rg->scale);
        /* PVR depth (orthographic: 1 / (ndc z + 2)) mapped into the layer's gap: view depth -near .. -far
         * onto [z1, z0] */
        zn = -in->n;
        zf = -in->f;
        cn = 1.0f / z1 - 2.0f;
        cf = 1.0f / z0 - 2.0f;
        cam.proj[2][2] = (cf - cn) / (zf - zn);
        cam.proj[3][2] = cn - cam.proj[2][2] * zn;
        /* Model_Draw: v * S * RotY + translation */
        sy = sinf(in->rot_y * (6.28318530718f / 65536.0f));
        cy = cosf(in->rot_y * (6.28318530718f / 65536.0f));
        memset(&mm, 0, sizeof(mm));
        mm.m[0][0] = in->scale[0] * cy;
        mm.m[0][2] = -in->scale[0] * sy;
        mm.m[1][1] = in->scale[1];
        mm.m[2][0] = in->scale[2] * sy;
        mm.m[2][2] = in->scale[2] * cy;
        mm.m[3][0] = in->pos[0];
        mm.m[3][1] = in->pos[1];
        mm.m[3][2] = in->pos[2];
        memset(&l, 0, sizeof(l));
        l.count = in->nlights;
        for (k = 0; k < in->nlights && k < 4; k++) {
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
