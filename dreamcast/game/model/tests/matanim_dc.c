/* KOS test (emulator / hardware): draw-path exactness, material animations and near-plane clipping of model.c.
 *   sh dreamcast/game/model/tests/build_matanim_disc.sh   -> build/dreamcast/model_test/mtest.cdi
 * First, every model on the disc is drawn with the slot fills and submission in C (dcm_dc_cfills, the
 * vertices through dcm_host_emit) and with the SH-4 routines (vertices captured as the TA would receive
 * them), cold and with warm caches, for several cameras (in front, across the near plane, orthographic),
 * poses, lights and material states; one "mtest: exact" line reports the bitwise comparison.
 * Draws, with the stage camera (DS pixels):
 *   - eff_tornado (z73) twice: left with eff_tornado01 (.dcc alpha fade-in, .dcp texture pattern),
 *     right with eff_tornado02 (the loop), as Boss7PartD6 binds them;
 *   - bsf_sky (zf3) as a sphere around the eye (radius 300): its triangles cross the near plane and are
 *     clipped; bsf.dct scrolls its texture (bsf_sky transT), bsf.dcc its colours.
 * Serial: one "mtest:" line every 30 frames (t_grow texture / alpha, sky transT, PVR triangles). */
#include <dirent.h>
#include <kos.h>
#include <stdio.h>
#include <stdlib.h>

#include "model.h"

typedef struct {
    dcm_model_t m;
    void *b;
} Mdl;
typedef struct {
    dca_set_t s;
    void *b;
} Set;

static void *slurp(const char *name, size_t *n) {
    char p[160];
    file_t f;
    void *b;
    snprintf(p, sizeof(p), "/cd/models/%s", name);
    f = fs_open(p, O_RDONLY);
    if (f == FILEHND_INVALID) {
        printf("mtest: missing %s\n", p);
        return NULL;
    }
    *n = fs_total(f);
    b = malloc(*n);
    if (b && fs_read(f, b, *n) != (ssize_t)*n) {
        free(b);
        b = NULL;
    }
    fs_close(f);
    return b;
}
static int load_model(Mdl *m, const char *name) {
    size_t n;
    m->b = slurp(name, &n);
    return m->b && dcm_model_load(&m->m, m->b, n, 1) == 0 ? 0 : -1;
}
static int load_set(Set *s, const char *name) {
    size_t n;
    s->b = slurp(name, &n);
    return s->b && dca_set_load(&s->s, s->b, n) == 0 ? 0 : -1;
}

static int mat_index(const dcm_model_t *m, const char *n) {
    int i;
    for (i = 0; i < m->hdr->num_materials; i++)
        if (!strncmp(m->matx[i].name, n, 16))
            return i;
    return 0;
}

/* ---- draw-path exactness: C reference vs SH-4 routines */
typedef struct {
    float o[5];
    uint32_t col;
    int eol;
} xrec_t;
#define XCAP 16384
static xrec_t g_ref[XCAP];
static int g_nref;
static uint32_t g_cap[8 * XCAP] __attribute__((aligned(32)));

static void ref_hook(int material, const float *o, uint32_t col, int eol) {
    (void)material;
    if (g_nref < XCAP) {
        memcpy(g_ref[g_nref].o, o, sizeof(g_ref[0].o));
        g_ref[g_nref].col = col;
        g_ref[g_nref].eol = eol;
        g_nref++;
    }
}

/* draws of one list: C reference (hook) then the SH-4 path (capture); returns mismatching vertices */
static int exact_list(const dcm_model_t *m, const dcm_mtx_t *pal, const dcm_mtx_t *mt, const dcm_camera_t *cam,
                      const dcm_lights_t *L, const dcm_matstate_t *st, int list, int ref, long *nv) {
    dcm_draw_opts_t o;
    int k, bad = 0;
    o.list = list;
    o.pt_list = PVR_LIST_PT_POLY;
    o.alpha_scale = 255;
    if (ref) {
        g_nref = 0;
        dcm_dc_cfills = 1;
        dcm_host_emit = ref_hook;
        dcm_draw_ex(m, pal, mt, cam, NULL, L, &o, st);
        dcm_host_emit = NULL;
        dcm_dc_cfills = 0;
        return 0;
    }
    dcm_dc_capture = g_cap;
    dcm_dc_capture_n = 0;
    dcm_draw_ex(m, pal, mt, cam, NULL, L, &o, st);
    dcm_dc_capture = NULL;
    if ((int)dcm_dc_capture_n != g_nref)
        return 1000000;
    for (k = 0; k < g_nref; k++) {
        const pvr_vertex_t *v = (const pvr_vertex_t *)&g_cap[8 * k];
        const xrec_t *r = &g_ref[k];
        if (memcmp(&v->x, &r->o[0], 12) || memcmp(&v->u, &r->o[3], 8) || v->argb != r->col || v->oargb ||
            v->flags != (r->eol ? PVR_CMD_VERTEX_EOL : PVR_CMD_VERTEX))
            bad++;
    }
    *nv += g_nref;
    return bad;
}

static void exact_check(void) {
    static dcm_mtx_t pal[DCM_MAX_PALETTE];
    static dcm_local_t loc[DCM_MAX_JOINTS];
    static dcm_matstate_t st[256];
    static const int lists[3] = {PVR_LIST_OP_POLY, PVR_LIST_PT_POLY, PVR_LIST_TR_POLY};
    DIR *dp = opendir("/cd/models");
    struct dirent *de;
    long models = 0, nv = 0, bad = 0, draws = 0;
    if (!dp) {
        printf("mtest: exact: no /cd/models\n");
        return;
    }
    while ((de = readdir(dp))) {
        size_t l = strlen(de->d_name), n;
        void *b;
        int c;
        if (l < 4 || strcmp(de->d_name + l - 4, ".dcm"))
            continue;
        b = slurp(de->d_name, &n);
        if (!b)
            continue;
        for (c = 0; c < 5; c++) {
            dcm_model_t m;
            dcm_camera_t cam;
            dcm_lights_t L;
            dcm_mtx_t mt;
            int li, pass, p;
            if (dcm_model_load(&m, b, n, 0) != 0)
                break;
            if (c == 0)
                models++;
            dcm_bind_locals(&m, loc);
            dcm_pose(&m, loc, pal);
            if (c == 3)
                for (p = 0; p < m.hdr->num_palette; p++)
                    pal[p].m[3][1] += 0.5f; /* another pose */
            dcm_matstate_init(&m, st);
            if (c < 4)
                dcm_camera_stage(&cam);
            else
                dcm_camera_ortho(&cam, 96, -96, -128, 128, -1000, 1000);
            /* in front / across the near plane / scaled up around the eye / posed / orthographic */
            dcm_stage_model_matrix(&mt, c == 2 ? 6.0f : 1.0f, (uint16_t)(0x1000 + c * 0x2345), (uint8_t)(c * 9),
                                   128.0f, -110.0f, c == 1 || c == 2 ? 0.0f - cam.view.m[3][2] : -60.0f);
            dcm_lights_stage(&L, c & 1);
            if (c == 3) {
                L.count = 4;
                L.dir[3][0] = 0.3f;
                L.dir[3][1] = -0.5f;
                L.dir[3][2] = 0.2f;
                L.col[3][0] = 0.7f;
            }
            pvr_wait_ready();
            pvr_scene_begin();
            for (li = 0; li < 3; li++) {
                pvr_list_begin(lists[li]);
                /* reference (C, cold caches), then the model again from cold caches: SH-4 fills, then warm */
                exact_list(&m, pal, &mt, &cam, &L, c & 1 ? st : NULL, lists[li], 1, &nv);
                dcm_model_free(&m);
                dcm_model_load(&m, b, n, 0);
                for (pass = 0; pass < 2; pass++) {
                    bad += exact_list(&m, pal, &mt, &cam, &L, c & 1 ? st : NULL, lists[li], 0, &nv);
                    draws++;
                }
                pvr_list_finish();
            }
            pvr_scene_finish();
            dcm_model_free(&m);
        }
        free(b);
    }
    closedir(dp);
    printf("mtest: exact: %ld models, %ld draws, %ld vertices compared, %ld differ (%s)\n", models, draws, nv, bad,
           bad ? "FAIL" : "ok");
}

int main(int argc, char **argv) {
    static dcm_matstate_t st1[16], st2[16], sts[8];
    static dcm_mtx_t pal1[DCM_MAX_PALETTE], pals[DCM_MAX_PALETTE];
    static dcm_local_t loc[DCM_MAX_JOINTS];
    Mdl tor, sky;
    Set c1, p1, c2, p2, skt, skc;
    dcm_camera_t cam;
    dcm_lights_t L;
    dcm_mtx_t mt1, mt2, msky;
    float eye[3];
    int f = 0, grow, skym, list, tri[3];
    static const int lists[3] = {PVR_LIST_OP_POLY, PVR_LIST_PT_POLY, PVR_LIST_TR_POLY};
    (void)argc;
    (void)argv;
    {
        pvr_init_params_t params = {
            .opb_sizes = {PVR_BINSIZE_16, PVR_BINSIZE_0, PVR_BINSIZE_16, PVR_BINSIZE_0, PVR_BINSIZE_16},
            .vertex_buf_size = 1536 * 1024,
            .opb_overflow_count = 3};
        pvr_init(&params);
    }
    pvr_set_bg_color(0.1f, 0.1f, 0.2f);
    exact_check();
    if (load_model(&tor, "narc__z73_act_lz7__eff_tornado__eff_tornado.dcm") ||
        load_model(&sky, "mod__bsf_sky__bsf_sky.dcm") || load_set(&c1, "narc__z73_act_lz7__eff_tornado01.dcc") ||
        load_set(&p1, "narc__z73_act_lz7__eff_tornado01.dcp") || load_set(&c2, "narc__z73_act_lz7__eff_tornado02.dcc") ||
        load_set(&p2, "narc__z73_act_lz7__eff_tornado02.dcp") || load_set(&skt, "narc__zf3_act_lz7__bsf.dct") ||
        load_set(&skc, "narc__zf3_act_lz7__bsf.dcc")) {
        printf("mtest: data missing\n");
        for (;;)
            thd_sleep(1000);
    }
    grow = mat_index(&tor.m, "t_grow");
    skym = mat_index(&sky.m, "bsf_sky");
    printf("mtest: loaded (tornado %d materials %d textures, sky %d materials), vram free %lu\n",
           tor.m.hdr->num_materials, tor.m.hdr->num_textures, sky.m.hdr->num_materials,
           (unsigned long)pvr_mem_available());
    dcm_camera_stage(&cam);
    eye[0] = -cam.view.m[3][0];
    eye[1] = -cam.view.m[3][1];
    eye[2] = -cam.view.m[3][2];
    dcm_lights_stage(&L, 0);
    dcm_bind_locals(&tor.m, loc);
    dcm_pose(&tor.m, loc, pal1);
    dcm_bind_locals(&sky.m, loc);
    dcm_pose(&sky.m, loc, pals);
    dcm_stage_model_matrix(&mt1, 2.0f, 0, 0, 72.0f, -180.0f, 0.0f);
    dcm_stage_model_matrix(&mt2, 2.0f, 0, 0, 184.0f, -180.0f, 0.0f);
    dcm_stage_model_matrix(&msky, 300.0f, 0, 0, eye[0], eye[1], eye[2]);
    for (;;) {
        int a1 = f % c1.s.anims[0].num_frames, a2 = f % c2.s.anims[0].num_frames,
            as = f % skt.s.anims[0].num_frames;
        dcm_draw_opts_t o;
        dcm_matstate_init(&tor.m, st1);
        dcm_matanim_apply(&c1.s, 0, a1, &tor.m, st1);
        dcm_matanim_apply(&p1.s, 0, a1, &tor.m, st1);
        dcm_matstate_init(&tor.m, st2);
        dcm_matanim_apply(&c2.s, 0, a2, &tor.m, st2);
        dcm_matanim_apply(&p2.s, 0, a2, &tor.m, st2);
        dcm_matstate_init(&sky.m, sts);
        dcm_matanim_apply(&skc.s, 0, as % skc.s.anims[0].num_frames, &sky.m, sts);
        dcm_matanim_apply(&skt.s, 0, as, &sky.m, sts);
        pvr_wait_ready();
        pvr_scene_begin();
        for (list = 0; list < 3; list++) {
            memset(&o, 0, sizeof(o));
            o.list = lists[list];
            o.pt_list = PVR_LIST_PT_POLY;
            o.alpha_scale = 255;
            pvr_list_begin(lists[list]);
            tri[list] = dcm_draw_ex(&sky.m, pals, &msky, &cam, NULL, &L, &o, sts);
            tri[list] += dcm_draw_ex(&tor.m, pal1, &mt1, &cam, NULL, &L, &o, st1);
            tri[list] += dcm_draw_ex(&tor.m, pal1, &mt2, &cam, NULL, &L, &o, st2);
            pvr_list_finish();
        }
        pvr_scene_finish();
        if (f % 30 == 0)
            printf("mtest: f %d tornado01 frame %d t_grow tex %d alpha %d | tornado02 frame %d t_grow tex %d alpha %d | "
                   "sky transT %ld srt %d | tris %d %d %d\n",
                   f, a1, st1[grow].texture, st1[grow].alpha, a2, st2[grow].texture, st2[grow].alpha,
                   (long)sts[skym].trans_t, sts[skym].srt_flags, tri[0], tri[1], tri[2]);
        f++;
    }
    return 0;
}
