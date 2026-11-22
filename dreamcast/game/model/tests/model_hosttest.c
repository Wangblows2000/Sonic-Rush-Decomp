/* Host test for model.c: load a .dcm + .dca, pose at (anim, frame), print transformed vertices.
 *   gcc -O2 -I.. model_hosttest.c ../model.c -lm -o model_hosttest
 *   ./model_hosttest son.dcm son.dca com_walk6 12.5 > verts.txt
 * Output: one line per vertex "x y z" (640x480 screen, z = 1/w), stage camera, player transform. */
#include <stdio.h>
#include <stdlib.h>

#include "model.h"

static void *slurp(const char *p, size_t *n) {
    FILE *f = fopen(p, "rb");
    void *b;
    if (!f)
        return NULL;
    fseek(f, 0, SEEK_END);
    *n = (size_t)ftell(f);
    fseek(f, 0, SEEK_SET);
    b = malloc(*n);
    if (fread(b, 1, *n, f) != *n) {
        fclose(f);
        return NULL;
    }
    fclose(f);
    return b;
}

int main(int argc, char **argv) {
    static dcm_local_t loc[DCM_MAX_JOINTS];
    static dcm_mtx_t pal[DCM_MAX_PALETTE];
    dcm_model_t m;
    dca_set_t s;
    dcm_camera_t cam;
    dcm_mtx_t mdl;
    size_t n1, n2;
    void *b1, *b2;
    float *out;
    int i, nv, a;
    if (argc < 5)
        return 2;
    b1 = slurp(argv[1], &n1);
    b2 = slurp(argv[2], &n2);
    if (!b1 || !b2 || dcm_model_load(&m, b1, n1, 0) || dca_set_load(&s, b2, n2))
        return 1;
    a = dca_set_find(&s, argv[3]);
    dca_sample(&s, a, (float)atof(argv[4]), &m, loc);
    dcm_pose(&m, loc, pal);
    dcm_camera_stage(&cam);
    dcm_stage_model_matrix(&mdl, 0x34cc / 4096.0f, 0x4000, 0, 128.0f, -96.0f, 0.0f);
    out = malloc(sizeof(float) * 3 * m.hdr->num_vertices);
    nv = dcm_transform_vertices(&m, pal, &mdl, &cam, out);
    for (i = 0; i < nv; i++)
        printf("%.4f %.4f %.6f\n", out[3 * i], out[3 * i + 1], out[3 * i + 2]);
    fprintf(stderr, "anim %d, %d vertices, %d palette\n", a, nv, m.hdr->num_palette);
    return 0;
}
