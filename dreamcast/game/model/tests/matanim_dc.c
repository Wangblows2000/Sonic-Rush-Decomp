/* KOS test (emulator / hardware): material animations and near-plane clipping of model.c.
 *   sh dreamcast/game/model/tests/build_matanim_disc.sh   -> build/dreamcast/model_test/mtest.cdi
 * Draws, with the stage camera (DS pixels):
 *   - eff_tornado (z73) twice: left with eff_tornado01 (.dcc alpha fade-in, .dcp texture pattern),
 *     right with eff_tornado02 (the loop), as Boss7PartD6 binds them;
 *   - bsf_sky (zf3) as a sphere around the eye (radius 300): its triangles cross the near plane and are
 *     clipped; bsf.dct scrolls its texture (bsf_sky transT), bsf.dcc its colours.
 * Serial: one "mtest:" line every 30 frames (t_grow texture / alpha, sky transT, PVR triangles). */
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
            .vertex_buf_size = 512 * 1024,
            .opb_overflow_count = 3};
        pvr_init(&params);
    }
    pvr_set_bg_color(0.1f, 0.1f, 0.2f);
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
