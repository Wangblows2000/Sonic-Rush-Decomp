/*
 * Host test for dcp_model.c (player model presentation) with the owner's
 * converted assets.  Expected values come from the decompiled code:
 *   Player_SetAnimation tables (ov000 0x02368bdc / c44 / cac), the Blaze +0x1b
 *   offset, Model_AnimateMdl (fx32 frame += speed, loop / clamp, ended bit),
 *   StageObj_Draw3DEx (facing from flags 0x5C bit 0, ANIM_DONE bit 8, loop bit 4),
 *   Player_Create (scale 0x34cc, model-space translation (0,-16,0)).
 *
 * Usage: test_model <dir with son.dcm son_ball.dcm blz.dcm blz_spin.dcm son.dca son.dcv animtab.bin>
 *                   <build/dreamcast/player> [<z11 level.bin>]
 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../dcp_model.h"

static int g_fail, g_checks;
#define CHECK(c, ...)                                                                                                  \
    do {                                                                                                               \
        g_checks++;                                                                                                    \
        if (!(c)) {                                                                                                    \
            g_fail++;                                                                                                  \
            printf("  FAIL: ");                                                                                        \
            printf(__VA_ARGS__);                                                                                       \
            printf("\n");                                                                                              \
        }                                                                                                              \
    } while (0)

static void *load(const char *path, long *size) {
    FILE *f = fopen(path, "rb");
    void *b;
    if (!f)
        return NULL;
    fseek(f, 0, SEEK_END);
    *size = ftell(f);
    fseek(f, 0, SEEK_SET);
    b = malloc((size_t)*size);
    if (fread(b, 1, (size_t)*size, f) != (size_t)*size) {
        fclose(f);
        return NULL;
    }
    fclose(f);
    return b;
}

static DcpModel M;
static DcPlayer P;

static const char *anim_name(int a) { return a >= 0 && a < M.jnt.hdr->num_anims ? M.jnt.anims[a].name : "?"; }

/* ------------------------------------------------- selection (Player_SetAnimation) */
static void test_selection(const char *dir) {
    int r;
    printf("[selection] Player_SetAnimation tables\n");
    memset(&P, 0, sizeof(P));
    r = dcp_model_load(&M, 0, dir, 0);
    CHECK(r == 0 && M.res[0].hdr && M.res[1].hdr, "Sonic body + ball load (%d)", r);
    if (r)
        return;
    dcp_model_attach(&M, &P);
    dcp_set_anim(&P, 0x3c);
    CHECK(M.cur_res == 0 && !strcmp(anim_name(M.jnt_anim), "com_fw") && M.vis_anim == 1, "idle 0x3c -> body com_fw, vis com_gen00 (%s res %d vis %d)",
          anim_name(M.jnt_anim), M.cur_res, M.vis_anim);
    dcp_set_anim(&P, 0x42);
    CHECK(M.cur_res == 0 && !strcmp(anim_name(M.jnt_anim), "com_walk6"), "run 0x42 -> com_walk6 (%s)", anim_name(M.jnt_anim));
    {
        static const struct { int id; const char *name; } ball[] = {
            {0x30, "son_jump_01"}, {0x31, "son_screw"}, {0x32, "son_scr_up"}, {0x33, "son_scr_wt"}};
        int i;
        for (i = 0; i < 4; i++) {
            dcp_set_anim(&P, ball[i].id);
            CHECK(M.cur_res == 1 && !strcmp(anim_name(M.jnt_anim), ball[i].name) && M.vis_anim == -1 &&
                      M.jnt.anims[M.jnt_anim].num_joints == M.res[1].hdr->num_joints,
                  "ball 0x%02x -> son_ball model, %s (%d joints = ball %d), no visibility anim", ball[i].id,
                  anim_name(M.jnt_anim), M.jnt.anims[M.jnt_anim].num_joints, M.res[1].hdr->num_joints);
        }
    }
    dcp_set_anim(&P, 2); /* fall: back to the body, visibility re-attached */
    CHECK(M.cur_res == 0 && M.vis_anim == 1 && !strcmp(anim_name(M.jnt_anim), "son_jump_03"), "fall 2 -> body son_jump_03 + vis (%s)",
          anim_name(M.jnt_anim));
    dcp_model_free(&M);

    memset(&P, 0, sizeof(P));
    P.chara = 1;
    r = dcp_model_load(&M, 1, dir, 0);
    CHECK(r == 0 && M.res[0].hdr && M.res[1].hdr, "Blaze body + spin load (%d)", r);
    if (r)
        return;
    {
        /* data_02074960: stage lights (Player_Draw / Player_Last), after-image and under-water overrides */
        const u16 *l0 = dcp_model_stage_lights(0, 0), *l3 = dcp_model_stage_lights(3, 2);
        dcm_lights_t L;
        u8 zone = g_dcp_world.zone, act = g_dcp_world.act;
        CHECK(l0[0] == 0x7fff && l0[1] == 0x4631 && l0[2] == 0x2108, "lights.bin zone 0 act 0: %04x %04x %04x", l0[0],
              l0[1], l0[2]);
        CHECK(l3[0] == 0x7fff && l3[1] == 0x75ed && l3[2] == 0x2c39, "lights.bin zone 3 act 2 (boss): %04x %04x %04x", l3[0],
              l3[1], l3[2]);
        g_dcp_world.zone = 3, g_dcp_world.act = 2;
        dcp_model_lights(&P, 0, &L);
        CHECK(fabsf(L.col[1][0] - 13 / 31.0f) < 1e-4f && fabsf(L.col[1][1] - 15 / 31.0f) < 1e-4f &&
                  fabsf(L.col[1][2] - 29 / 31.0f) < 1e-4f,
              "dcp_model_lights light 1 = 0x75ed (%.3f %.3f %.3f)", L.col[1][0], L.col[1][1], L.col[1][2]);
        dcp_model_lights(&P, 1, &L);
        CHECK(fabsf(L.col[0][0] - (0x1a98 & 31) / 31.0f) < 1e-4f, "Blaze after-image light 0x1a98");
        g_dcp_world.zone = zone, g_dcp_world.act = act;
    }
    dcp_model_attach(&M, &P);
    dcp_set_anim(&P, 0x3c);
    CHECK(!strcmp(anim_name(M.jnt_anim), "blz_fw"), "Blaze idle 0x3c+0x1b -> blz_fw (%s)", anim_name(M.jnt_anim));
    dcp_set_anim(&P, 0x42);
    CHECK(!strcmp(anim_name(M.jnt_anim), "blz_walk6"), "Blaze run -> blz_walk6 (%s)", anim_name(M.jnt_anim));
    dcp_set_anim(&P, 0x30);
    CHECK(M.cur_res == 1 && !strcmp(anim_name(M.jnt_anim), "blz_screw") &&
              M.jnt.anims[M.jnt_anim].num_joints == M.res[1].hdr->num_joints,
          "Blaze jump 0x30+0x1b -> blz_spin model, blz_screw (%s)", anim_name(M.jnt_anim));
    dcp_set_anim(&P, 0x0f);
    CHECK(M.cur_res == 0 && !strcmp(anim_name(M.jnt_anim), "com_dmg_b"), "Blaze hurt 0x0f (< 0x30, no offset) -> com_dmg_b (%s)",
          anim_name(M.jnt_anim));
    dcp_model_free(&M);
}

/* ------------------------------------------------- timing (Model_AnimateMdl) */
static int frames_to_done(int loop, s32 speed_s, int *pulses, int run) {
    int n, first = -1;
    *pulses = 0;
    for (n = 1; n <= run; n++) {
        if (loop)
            P.disp_flags |= 4;
        dcp_model_step(&M, &P, 0);
        if (P.disp_flags & DCP_DF_ANIM_DONE) {
            (*pulses)++;
            if (first < 0)
                first = n;
        }
    }
    (void)speed_s;
    return first;
}

static void test_timing(const char *dir) {
    int nf, first, pulses;
    printf("[timing] frame advance, loop, ANIM_DONE\n");
    memset(&P, 0, sizeof(P));
    g_dcp_world.timescale = 0x100;
    if (dcp_model_load(&M, 0, dir, 0))
        return;
    dcp_model_attach(&M, &P);
    /* skid 0x05 (com_brake1_01, non-looping): ANIM_DONE on the frame the fx32 frame reaches the end, then every frame */
    dcp_set_anim(&P, 5);
    nf = M.jnt.anims[M.jnt_anim].num_frames;
    first = frames_to_done(0, 0, &pulses, nf + 10);
    CHECK(first == nf && pulses == 11 && (M.jnt_frame >> 12) == nf - 1,
          "one-shot %s (%d frames) at speed 1.0: done after %d frames (want %d), held on the last frame, set on %d of %d later frames",
          anim_name(M.jnt_anim), nf, first, nf, pulses, nf + 10 - nf + 1);
    /* looping idle at speed 1.0: wraps, one pulse per cycle */
    dcp_set_anim(&P, 0x3c);
    CHECK(!(P.disp_flags & DCP_DF_ANIM_DONE), "Player_SetAnimation clears ANIM_DONE");
    nf = M.jnt.anims[M.jnt_anim].num_frames;
    first = frames_to_done(1, 0, &pulses, 3 * nf);
    CHECK(first == nf && pulses == 3 && (M.jnt_frame >> 12) == 0, "loop %s (%d frames): %d pulses in 3 cycles, first at %d",
          anim_name(M.jnt_anim), nf, pulses, first);
    /* Player_SetAnimSpeedFromVelocity: v 0x900 -> s = 0x6c -> speed 6.75 */
    dcp_set_anim(&P, 0x42);
    dcp_model_set_speed(&M, 0x6c);
    nf = M.jnt.anims[M.jnt_anim].num_frames;
    first = frames_to_done(1, 0x6c, &pulses, 30);
    CHECK(M.speed == 0x6c00 && first == (nf * 0x1000 + 0x6c00 - 1) / 0x6c00, "run at s=0x6c: speed 0x%x, first wrap at %d (%d frames)",
          (unsigned)M.speed, first, nf);
    dcp_set_anim(&P, 0x42);
    CHECK(M.speed == 0x1000 && M.jnt_frame == 0, "re-setting the same id restarts at frame 0 with speed 1.0");
    /* hit-stop: no advance, flags untouched */
    {
        s32 f0;
        dcp_model_step(&M, &P, 0);
        f0 = M.jnt_frame;
        P.hitstop = 3;
        P.disp_flags |= DCP_DF_ANIM_DONE;
        dcp_model_step(&M, &P, 0);
        CHECK(M.jnt_frame == f0 && (P.disp_flags & DCP_DF_ANIM_DONE), "hit-stop freezes the frame and keeps the flags");
        P.hitstop = 0;
    }
    /* blend request 0x400: 8-frame cross-fade (0x200 per frame), then 0x400 cleared */
    {
        int n;
        dcp_set_anim(&P, 0x3c);
        dcp_model_step(&M, &P, 0);
        P.disp_flags |= 0x400;
        dcp_model_set_anim(&M, &P, 0x3d);
        P.disp_flags |= 0x400;
        for (n = 0; n < 7; n++)
            dcp_model_step(&M, &P, 0);
        CHECK(M.blend_anim >= 0 && M.ratio == 0xe00 && (P.disp_flags & 0x400), "blend ratio 0x%x after 7 frames", (unsigned)M.ratio);
        dcp_model_step(&M, &P, 0);
        CHECK(M.blend_anim < 0 && !(P.disp_flags & 0x400), "blend finished after 8 frames, 0x400 cleared");
    }
    dcp_model_free(&M);
}

/* ------------------------------------------------- placement (Draw3DEx + Model_Draw) */
static void extent(const dcm_mtx_t *mdl, float *x0, float *x1, float *y0, float *y1) {
    dcm_camera_t cam;
    float *v = malloc(sizeof(float) * 3 * M.res[M.cur_res].hdr->num_vertices);
    int i, n;
    dcm_camera_stage(&cam);
    n = dcm_transform_vertices(&M.res[M.cur_res], M.pal, mdl, &cam, v);
    *x0 = *y0 = 1e9f;
    *x1 = *y1 = -1e9f;
    for (i = 0; i < n; i++) {
        if (v[3 * i] < *x0) *x0 = v[3 * i];
        if (v[3 * i] > *x1) *x1 = v[3 * i];
        if (v[3 * i + 1] < *y0) *y0 = v[3 * i + 1];
        if (v[3 * i + 1] > *y1) *y1 = v[3 * i + 1];
    }
    free(v);
}

static void test_placement(const char *dir) {
    dcm_mtx_t mdl;
    float x0, x1, y0, y1, lx0, lx1, ly0, ly1;
    printf("[placement] model matrix\n");
    memset(&P, 0, sizeof(P));
    if (dcp_model_load(&M, 0, dir, 0))
        return;
    dcp_model_attach(&M, &P);
    dcp_set_anim(&P, 0x3c);
    dcp_model_step(&M, &P, 0);
    /* player centre at the camera centre (128,-96); the feet probe is centre + 13 px, the floor at +14 */
    dcp_model_matrix(&P, 128.0f, -96.0f, 0.0f, &mdl);
    extent(&mdl, &x0, &x1, &y0, &y1);
    printf("  standing, facing right: screen x %.1f..%.1f, y %.1f..%.1f (centre y 240, floor at %.1f)\n", x0, x1, y0, y1,
           240.0f + 14 * 2.5f);
    CHECK(fabsf(y1 - (240.0f + 16 * 2.5f)) < 2.5f, "lowest vertex 16 DS px below the centre (y %.1f, want %.1f)", y1, 240.0f + 40.0f);
    P.disp_flags |= 1;
    dcp_model_matrix(&P, 128.0f, -96.0f, 0.0f, &mdl);
    extent(&mdl, &lx0, &lx1, &ly0, &ly1);
    /* RotY(0xC000) vs RotY(0x4000): a half turn, so x mirrors about the centre (z flips, so the
     * perspective widths differ slightly); the width centre and the floor contact must mirror */
    CHECK(fabsf(((lx0 + lx1) / 2 - 320.0f) + ((x0 + x1) / 2 - 320.0f)) < 1.0f && fabsf(ly1 - y1) < 1.0f,
          "facing left mirrors the model about the centre (%.1f..%.1f vs %.1f..%.1f, feet %.1f vs %.1f)", lx0, lx1, x0, x1, ly1, y1);
    P.disp_flags &= ~1;
    /* angle 0x20 (ground direction (cos, sin) = down-right with y down): the feet go 16 px along the
     * floor normal (-sin, cos) = down-left, i.e. (-11.31, -11.31) in model space (y up) */
    P.angle = 0x20;
    dcp_model_matrix(&P, 0.0f, 0.0f, 0.0f, &mdl);
    CHECK(fabsf(mdl.m[3][0] + 11.3137f) < 0.01f && fabsf(mdl.m[3][1] + 11.3137f) < 0.01f,
          "angle 0x20: model origin at (%.2f, %.2f) = 16 px along the floor normal", mdl.m[3][0], mdl.m[3][1]);
    P.angle = 0;
    dcp_model_free(&M);
}

/* ------------------------------------------------- z11 run: no stuck / restarting animation */
static void test_run(const char *dir, const char *tables, const char *lvl) {
    long n, np;
    char path[512];
    s16 *trig;
    u8 *phys, *file;
    static rlvl_map_t map;
    int f, chara;
    snprintf(path, sizeof(path), "%s/trig.bin", tables);
    trig = load(path, &n);
    snprintf(path, sizeof(path), "%s/physics.bin", tables);
    phys = load(path, &np);
    file = lvl ? load(lvl, &n) : NULL;
    if (!trig || !phys || !file || rlvl_bind(&map, file) != 0) {
        printf("[run] skipped (no tables or level)\n");
        return;
    }
    dcp_set_trig(trig, trig + 8192);
    dcp_set_physics(phys, (u32)np);
    dcp_bind_map(&map);
    for (chara = 0; chara < 2; chara++) {
        int max_same = 0, same = 0, wrong_face = 0, left_frames = 0, ball_frames = 0, ball_wrong = 0, restarts = 0;
        u32 last_set;
        s16 last_anim = -1;
        memset(&g_dcp_world, 0, sizeof(g_dcp_world));
        g_dcp_world.flags = 1;
        g_dcp_world.objmgr = 0x18;
        g_dcp_world.map_right = map.right;
        g_dcp_world.death_y = map.bottom;
        g_dcp_world.timescale = 0x100;
        g_dcp_world.lives = 3;
        memset(&P, 0, sizeof(P));
        if (dcp_model_load(&M, chara, dir, 0))
            return;
        dcp_player_create(&P, chara, 0, 128, 768);
        dcp_model_attach(&M, &P);
        last_set = M.set_count;
        printf("[run] %s on z11: right 300 frames, left 200, right + jump\n", chara ? "Blaze" : "Sonic");
        for (f = 0; f < 900; f++) {
            u16 k = f < 300 ? DCP_KEY_RIGHT : f < 500 ? DCP_KEY_LEFT : DCP_KEY_RIGHT;
            u16 pr = (f >= 500 && f % 60 == 0) ? DCP_KEY_A : 0;
            if (f >= 500 && f % 60 < 20)
                k |= DCP_KEY_A;
            dcp_player_frame(&P, k, pr, 0);
            dcp_model_step(&M, &P, 0);
            if (M.set_count != last_set && P.anim == last_anim)
                restarts++;
            same = M.set_count != last_set ? same + 1 : 0;
            if (same > max_same)
                max_same = same;
            last_set = M.set_count;
            last_anim = P.anim;
            /* the model faces the travel direction while running on the ground */
            if ((P.move_flags & 1) && abs(P.gnd_vel) > 0x200 && P.anim >= 0x3d && P.anim <= 0x42) {
                int left = (P.disp_flags & 1) != 0;
                if (left != (P.gnd_vel < 0))
                    wrong_face++;
                left_frames += left;
            }
            if (P.anim >= 0x30 && P.anim <= 0x33) {
                ball_frames++;
                if (M.cur_res != 1)
                    ball_wrong++;
            }
        }
        printf("  longest run of consecutive Player_SetAnimation frames %d, same-id restarts %d, running frames facing left %d,"
               " wrong facing %d, ball frames %d\n", max_same, restarts, left_frames, wrong_face, ball_frames);
        CHECK(wrong_face == 0 && left_frames > 20, "running faces the travel direction in both directions");
        CHECK(max_same < 4, "no animation re-set every frame (longest %d)", max_same);
        CHECK(ball_frames > 0 && ball_wrong == 0, "every ball frame uses the ball model (%d of %d)", ball_frames - ball_wrong, ball_frames);
        dcp_model_free(&M);
    }
    free(file);
}

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("usage: %s <model dir> <player tables dir> [z11 level.bin]\n", argv[0]);
        return 2;
    }
    test_selection(argv[1]);
    test_timing(argv[1]);
    test_placement(argv[1]);
    test_run(argv[1], argv[2], argc > 3 ? argv[3] : NULL);
    printf("%d checks, %d failed\n", g_checks, g_fail);
    return g_fail != 0;
}
