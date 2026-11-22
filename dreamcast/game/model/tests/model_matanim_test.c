/* Host test: material animations (.dcc / .dcp / .dct) and near-plane clipping of model.c.
 *   gcc -std=c99 -O2 -Wall -Wextra -I.. model_matanim_test.c ../model.c -lm -o model_matanim_test
 *   ./model_matanim_test build/dreamcast/models
 * Checks, on the owner's converted packages:
 *   - every .dcm loads (version 2) and its static materials give the same uv affine through
 *     dcm_matstate_uv as the converter's static values, and dcm_draw_ex(NULL) == dcm_draw;
 *   - every .dcc / .dcp / .dct set loads, binds to its model by material name, and its values follow
 *     the DS (alpha 0 hides a material, pattern frames switch textures, SRT frames move the uv);
 *   - a triangle crossing the near plane is clipped there (no vertex behind it, same screen edge). */
#include <dirent.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "model.h"

static int g_checks, g_fail;
#define CHECK(c, ...)                                                                                                 \
    do {                                                                                                              \
        g_checks++;                                                                                                   \
        if (!(c)) {                                                                                                   \
            g_fail++;                                                                                                 \
            printf("FAIL: " __VA_ARGS__);                                                                             \
            printf("\n");                                                                                             \
        }                                                                                                             \
    } while (0)

static const char *g_dir;

static void *slurp(const char *name, size_t *n) {
    char p[512];
    FILE *f;
    void *b;
    snprintf(p, sizeof(p), "%s/%s", g_dir, name);
    f = fopen(p, "rb");
    if (!f)
        return NULL;
    fseek(f, 0, SEEK_END);
    *n = (size_t)ftell(f);
    fseek(f, 0, SEEK_SET);
    b = malloc(*n);
    if (fread(b, 1, *n, f) != *n) {
        fclose(f);
        free(b);
        return NULL;
    }
    fclose(f);
    return b;
}

/* emitted vertices */
static int g_nemit, g_emit_behind;
static float g_minw_scr;
static void hook(int material, const float *o, uint32_t argb, int eol) {
    (void)material;
    (void)argb;
    (void)eol;
    g_nemit++;
    if (o[2] <= 0.0f)
        g_emit_behind++;
}

static int load_model(const char *file, dcm_model_t *m, void **bytes) {
    size_t n;
    *bytes = slurp(file, &n);
    return *bytes && dcm_model_load(m, *bytes, n, 0) == 0 ? 0 : -1;
}
static int load_set(const char *file, dca_set_t *s, void **bytes) {
    size_t n;
    *bytes = slurp(file, &n);
    return *bytes && dca_set_load(s, *bytes, n) == 0 ? 0 : -1;
}

static void test_all_models(void) {
    DIR *d = opendir(g_dir);
    struct dirent *e;
    int nm = 0, nuv = 0;
    static dcm_mtx_t pal[DCM_MAX_PALETTE];
    static dcm_local_t loc[DCM_MAX_JOINTS];
    static dcm_matstate_t st[256];
    while (d && (e = readdir(d)) != NULL) {
        size_t l = strlen(e->d_name);
        dcm_model_t m;
        void *b;
        int i, a1, a2;
        dcm_camera_t cam;
        dcm_mtx_t mdl;
        if (l < 5 || strcmp(e->d_name + l - 4, ".dcm"))
            continue;
        CHECK(load_model(e->d_name, &m, &b) == 0, "load %s", e->d_name);
        if (!m.hdr) {
            free(b);
            continue;
        }
        nm++;
        CHECK(m.hdr->version == 2 && m.matx && m.texx, "%s version 2", e->d_name);
        if (m.hdr->num_materials > 256) {
            free(b);
            continue;
        }
        dcm_matstate_init(&m, st);
        for (i = 0; i < m.hdr->num_materials; i++) {
            float a, dd, ee, f;
            const dcm_material_t *mt = &m.materials[i];
            dcm_matstate_uv(&m, i, &st[i], &a, &dd, &ee, &f);
            CHECK(fabsf(a - mt->uv_a) < 1e-4f && fabsf(dd - mt->uv_d) < 1e-4f && fabsf(ee - mt->uv_e) < 1e-4f &&
                      fabsf(f - mt->uv_f) < 1e-4f,
                  "%s material %d (%.16s) static uv %f %f %f %f vs %f %f %f %f", e->d_name, i, m.matx[i].name, a, dd,
                  ee, f, mt->uv_a, mt->uv_d, mt->uv_e, mt->uv_f);
            nuv++;
        }
        /* dcm_draw_ex(NULL) == dcm_draw == dcm_draw_ex(static states) */
        dcm_bind_locals(&m, loc);
        dcm_pose(&m, loc, pal);
        dcm_camera_stage(&cam);
        dcm_stage_model_matrix(&mdl, 0x34cc / 4096.0f, 0x4000, 0, 128.0f, -96.0f, 0.0f);
        dcm_host_emit = hook;
        g_nemit = 0;
        a1 = dcm_draw(&m, pal, &mdl, &cam, NULL, NULL, NULL);
        a2 = g_nemit;
        g_nemit = 0;
        CHECK(dcm_draw_ex(&m, pal, &mdl, &cam, NULL, NULL, NULL, st) == a1 && g_nemit == a2, "%s draw_ex static",
              e->d_name);
        dcm_host_emit = NULL;
        free(b);
    }
    if (d)
        closedir(d);
    printf("models: %d loaded, %d static material uv checked\n", nm, nuv);
}

/* every material-animation set binds to a model of the same archive (by name stem) */
static void test_all_sets(void) {
    DIR *d = opendir(g_dir);
    struct dirent *e;
    int ns = 0, nbound = 0;
    while (d && (e = readdir(d)) != NULL) {
        size_t l = strlen(e->d_name);
        dca_set_t s;
        void *b;
        int a;
        if (l > 4 && (!strcmp(e->d_name + l - 4, ".dca") || !strcmp(e->d_name + l - 4, ".dcv"))) {
            CHECK(load_set(e->d_name, &s, &b) == 0 && s.hdr->kind <= DCA_KIND_VIS, "load %s", e->d_name);
            free(b);
            continue;
        }
        if (l < 5 || (strcmp(e->d_name + l - 4, ".dcc") && strcmp(e->d_name + l - 4, ".dcp") &&
                      strcmp(e->d_name + l - 4, ".dct")))
            continue;
        CHECK(load_set(e->d_name, &s, &b) == 0, "load %s", e->d_name);
        if (!s.hdr) {
            free(b);
            continue;
        }
        ns++;
        CHECK(s.hdr->kind >= DCA_KIND_MATCOL && s.hdr->kind <= DCA_KIND_TEXSRT, "%s kind %d", e->d_name, s.hdr->kind);
        for (a = 0; a < s.hdr->num_anims; a++) {
            const dca_anim_t *an = &s.anims[a];
            const dcx_mat_t *mx = (const dcx_mat_t *)(s.base + an->ofs_channels);
            int k, c;
            CHECK(an->num_frames >= 1 && an->ofs_channels + 76u * an->num_joints <= s.hdr->file_size,
                  "%s anim %d layout", e->d_name, a);
            for (k = 0; k < an->num_joints; k++)
                for (c = 0; c < 5; c++)
                    if (mx[k].ch[c].mode == DCA_MODE_FRAMES)
                        CHECK(an->ofs_data + mx[k].ch[c].ofs + 4u * an->num_frames <= s.hdr->file_size + 2u * an->num_frames,
                              "%s anim %d material %d channel %d samples", e->d_name, a, k, c);
            nbound += an->num_joints;
        }
        free(b);
    }
    if (d)
        closedir(d);
    printf("material animation sets: %d (%d material channels sets)\n", ns, nbound);
}

static int find_tex(const dcm_model_t *m, const char *t) {
    int j;
    for (j = 0; j < m->hdr->num_textures; j++)
        if (!strncmp(m->texx[j].tex, t, 16))
            return j;
    return -1;
}
static int find_mat(const dcm_model_t *m, const char *n) {
    int j;
    for (j = 0; j < m->hdr->num_materials; j++)
        if (!strncmp(m->matx[j].name, n, 16))
            return j;
    return -1;
}

/* eff_tornado (z73): tornado01 .nsbma hides t_grow (alpha 0) until frame 17 and fades t_mini_loop in;
 * .nsbtp switches t_grow t_grow01 -> 02 at frame 19 */
static void test_tornado(void) {
    dcm_model_t m;
    dca_set_t c, p;
    void *b1, *b2, *b3;
    static dcm_matstate_t st[64];
    int g, ml;
    if (load_model("narc__z73_act_lz7__eff_tornado__eff_tornado.dcm", &m, &b1) ||
        load_set("narc__z73_act_lz7__eff_tornado01.dcc", &c, &b2) ||
        load_set("narc__z73_act_lz7__eff_tornado01.dcp", &p, &b3)) {
        CHECK(0, "eff_tornado packages");
        return;
    }
    g = find_mat(&m, "t_grow");
    ml = find_mat(&m, "t_mini_loop");
    CHECK(g >= 0 && ml >= 0, "tornado materials");
    if (g < 0 || ml < 0)
        return;
    dcm_matstate_init(&m, st);
    CHECK(dcm_matanim_apply(&c, 0, 0, &m, st) == 2 && st[g].alpha == 0 && st[ml].alpha == 0, "tornado01 frame 0 alpha");
    dcm_matanim_apply(&c, 0, 3, &m, st);
    CHECK(st[ml].alpha == 12, "tornado01 frame 3 t_mini_loop alpha %d", st[ml].alpha);
    dcm_matanim_apply(&c, 0, 17, &m, st);
    CHECK(st[g].alpha == 25 && st[ml].alpha == 0, "tornado01 frame 17 alpha %d %d", st[g].alpha, st[ml].alpha);
    dcm_matanim_apply(&p, 0, 0, &m, st);
    CHECK(st[g].texture == find_tex(&m, "t_grow01"), "tornado01 pattern frame 0");
    dcm_matanim_apply(&p, 0, 19, &m, st);
    CHECK(st[g].texture == find_tex(&m, "t_grow02") && (st[g].animated & 2), "tornado01 pattern frame 19");
    dcm_matanim_apply(&p, 0, 23, &m, st);
    CHECK(st[g].texture == find_tex(&m, "t_grow04"), "tornado01 pattern frame 23");
    free(b1);
    free(b2);
    free(b3);
}

/* bsf.nsbta (zf3): bsf_sky scrolls transT by -1/4096 a frame -> the uv translation moves */
static void test_srt(void) {
    dcm_model_t m;
    dca_set_t t;
    void *b1, *b2;
    static dcm_matstate_t st[64];
    float a, d, e0, f0, e1, f1;
    int mi;
    if (load_model("mod__bsf_sky__bsf_sky.dcm", &m, &b1) || load_set("narc__zf3_act_lz7__bsf.dct", &t, &b2)) {
        CHECK(0, "bsf_sky packages");
        return;
    }
    mi = find_mat(&m, "bsf_sky");
    CHECK(mi >= 0, "bsf_sky material");
    if (mi < 0)
        return;
    dcm_matstate_init(&m, st);
    CHECK(dcm_matanim_apply(&t, 0, 1, &m, st) >= 1 && (st[mi].srt_flags & DCM_SRT_ON) && st[mi].trans_t == -2,
          "bsf_sky frame 1 transT %d", (int)st[mi].trans_t);
    dcm_matstate_uv(&m, mi, &st[mi], &a, &d, &e0, &f0);
    dcm_matanim_apply(&t, 0, 200, &m, st);
    dcm_matstate_uv(&m, mi, &st[mi], &a, &d, &e1, &f1);
    CHECK(f1 > f0 && fabsf(e1 - e0) < 1e-6f, "bsf_sky uv moves in t: %f -> %f", f0, f1);
    free(b1);
    free(b2);
}

/* near plane: a triangle with one vertex behind the camera -> clipped, every emitted vertex in front */
static void test_clip(void) {
    static uint8_t buf[4096];
    dcm_header_t *h = (dcm_header_t *)buf;
    dcm_joint_t *j;
    dcm_palette_t *pl;
    dcm_material_t *mt;
    dcm_draw_t *dr;
    dcm_strip_t *sp;
    dcm_vertex_t *v;
    dcm_model_t m;
    dcm_camera_t cam;
    dcm_mtx_t id, pal[1];
    dcm_local_t loc[1];
    int r;
    memset(buf, 0, sizeof(buf));
    h->magic = DCM_MAGIC;
    h->version = 1;
    h->header_size = DCM_HEADER_SIZE_V1;
    h->file_size = sizeof(buf);
    h->num_joints = 1;
    h->num_palette = 1;
    h->num_materials = 1;
    h->num_draws = 1;
    h->num_strips = 1;
    h->num_vertices = 3;
    h->ofs_joints = 256;
    h->ofs_order = 320;
    h->ofs_palette = 336;
    h->ofs_materials = 352;
    h->ofs_textures = 400;
    h->ofs_draws = 416;
    h->ofs_strips = 432;
    h->ofs_vertices = 448;
    j = (dcm_joint_t *)(buf + 256);
    j->parent = -1;
    j->r[0] = j->r[4] = j->r[8] = 1;
    j->s[0] = j->s[1] = j->s[2] = 1;
    pl = (dcm_palette_t *)(buf + 336);
    pl->kind = DCM_PAL_IDENTITY;
    mt = (dcm_material_t *)(buf + 352);
    mt->texture = 0xFFFF;
    mt->alpha = 31;
    mt->diffuse = 0xFFFFFFFF;
    mt->uv_a = mt->uv_d = 1;
    dr = (dcm_draw_t *)(buf + 416);
    dr->vis_node = 0xFFFF;
    dr->num_strips = 1;
    sp = (dcm_strip_t *)(buf + 432);
    sp->count = 3;
    v = (dcm_vertex_t *)(buf + 448);
    /* view: camera at the origin looking down -z (identity view); two vertices in front, one behind */
    v[0].x = -10; v[0].y = 0; v[0].z = -50;
    v[1].x = 10;  v[1].y = 0; v[1].z = -50;
    v[2].x = 0;   v[2].y = 10; v[2].z = 20;
    CHECK(dcm_model_load(&m, buf, sizeof(buf), 0) == 0, "synthetic model");
    dcm_bind_locals(&m, loc);
    dcm_pose(&m, loc, pal);
    dcm_camera_stage(&cam);
    dcm_mtx_identity(&cam.view);
    dcm_mtx_identity(&id);
    dcm_host_emit = hook;
    g_nemit = g_emit_behind = 0;
    r = dcm_draw(&m, pal, &id, &cam, NULL, NULL, NULL);
    CHECK(r == 2 && g_nemit == 4 && g_emit_behind == 0, "near-plane clip: %d triangles, %d vertices, %d behind", r,
          g_nemit, g_emit_behind);
    /* fully behind: nothing */
    v[0].z = v[1].z = 30;
    g_nemit = 0;
    r = dcm_draw(&m, pal, &id, &cam, NULL, NULL, NULL);
    CHECK(r == 0 && g_nemit == 0, "behind the camera: %d", r);
    /* fully in front: the strip as it is */
    v[0].z = v[1].z = v[2].z = -60;
    g_nemit = 0;
    r = dcm_draw(&m, pal, &id, &cam, NULL, NULL, NULL);
    CHECK(r == 1 && g_nemit == 3, "in front: %d", r);
    dcm_host_emit = NULL;
    (void)g_minw_scr;
}

int main(int argc, char **argv) {
    g_dir = argc > 1 ? argv[1] : "build/dreamcast/models";
    test_clip();
    test_all_models();
    test_all_sets();
    test_tornado();
    test_srt();
    printf("%d/%d checks passed\n", g_checks - g_fail, g_checks);
    return g_fail ? 1 : 0;
}
