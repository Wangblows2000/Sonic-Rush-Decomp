/* Host test of the front end: drives the translated state machines with
 * scripted pad input and checks scene changes, requests, timings and save
 * effects. With a dump directory, writes 640x480 PPM frames for inspection.
 * Usage: test_menus <build/dreamcast/menus> <build/dreamcast/sprites/bac> [dumpdir] */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mn_raster.h"

static int fails, checks;
#define CHECK(c)                                                                 \
    do {                                                                         \
        checks++;                                                                \
        if (!(c)) {                                                              \
            fails++;                                                             \
            printf("FAIL %s:%d: %s (scene %s)\n", __FILE__, __LINE__, #c, g_mn_scene); \
        }                                                                        \
    } while (0)

static const char *s_dump;
static char s_save_path[256];
static int h_present(void) { return 1; }
static int h_read(uint8_t *img, int size) {
    FILE *f = fopen(s_save_path, "rb");
    int n;
    if (!f)
        return 1;
    n = (int)fread(img, 1, (size_t)size, f);
    fclose(f);
    return n == size ? 0 : 1;
}
static int h_write(const uint8_t *img, int size) {
    FILE *f = fopen(s_save_path, "wb");
    if (!f)
        return -1;
    fwrite(img, 1, (size_t)size, f);
    fclose(f);
    return 0;
}

static int s_frame;
static MenuRequest s_last;
static int s_se[64], s_nse, s_arc0;
static int s_st_index = -1, s_st_type = -1;
static void hook_st(int index, int archive, int type, int group, int arc, int id) {
    (void)archive;
    (void)group;
    (void)arc;
    (void)id;
    s_st_index = index;
    s_st_type = type;
}
static void hook_se(int arc, int id) {
    if (s_nse == 0)
        s_arc0 = arc;
    if (s_nse < 64)
        s_se[s_nse++] = id;
}

/* run n frames holding `held` (InputState bits) on the first frame only when tap */
static MenuRequest run(int n, uint32_t held, int tap) {
    InputState in;
    MenuRequest r;
    int i;
    memset(&r, 0, sizeof(r));
    for (i = 0; i < n; i++) {
        memset(&in, 0, sizeof(in));
        in.held = (tap && i > 0) ? 0 : held;
        r = menus_run_frame(&in);
        menus_draw_tr();
        s_frame++;
        if (r.kind != MENU_REQ_NONE) {
            s_last = r;
            return r;
        }
    }
    return r;
}
static void tap(uint32_t b) { run(2, b, 1); }
/* n frames; an open message box (mn_msgbox.c) is answered YES / OK: LEFT, then A */
static MenuRequest run_answering(int n) {
    MenuRequest r;
    int i;
    memset(&r, 0, sizeof(r));
    for (i = 0; i < n; i++) {
        r = run(1, MsgBox_IsClosed() ? 0 : (s_frame & 1) ? IN_JUMP : IN_LEFT, 0);
        if (r.kind != MENU_REQ_NONE)
            return r;
    }
    return r;
}
static void dump(const char *name) {
    char p[300];
    if (!s_dump)
        return;
    snprintf(p, sizeof(p), "%s/%s.ppm", s_dump, name);
    raster_frame(p);
    if (getenv("MN_DUMP_ENGINES")) {   /* debug: each DS engine alone, full frame */
        MnComp keep = g_mn_comp;
        int e;
        for (e = 0; e < 2; e++) {
            mn_comp_reset(e);
            mn_comp_full(e);
            snprintf(p, sizeof(p), "%s/raw/%s_e%d.ppm", s_dump, name, e);
            raster_frame(p);
        }
        g_mn_comp = keep;
    }
}
static int run_until_scene(const char *scene, int max) {
    int i;
    for (i = 0; i < max; i++) {
        if (strcmp(g_mn_scene, scene) == 0)
            return i;
        run(1, 0, 0);
    }
    return -1;
}

int main(int argc, char **argv) {
    int t;
    if (argc < 3) {
        printf("usage: %s menus_dir sprites_dir [dumpdir]\n", argv[0]);
        return 2;
    }
    s_dump = argc > 3 ? argv[3] : NULL;
    snprintf(s_save_path, sizeof(s_save_path), "/tmp/menus/test_vmu.bin");
    remove(s_save_path);
    g_save_backend.present = h_present;
    g_save_backend.read = h_read;
    g_save_backend.write = h_write;
    g_menu_hooks.se = hook_se;
    g_menu_hooks.sound_test = hook_st;
    menus_set_paths(argv[1], argv[2]);
    CHECK(menus_init() == 0);
    CHECK(g_mn_data.ok);
    CHECK(strcmp(g_mn_scene, "SegaLogo") == 0);
    CHECK(save_last_error() == 0 && gSaveData.unkE == 2);

    /* SegaLogo: 2 + 32 fade-in + 120 wait + 32 fade-out + 30 end */
    run(2 + 16, 0, 0);
    dump("01_segalogo_fadein");
    run(40, 0, 0);
    CHECK(g_mn_bright[0] == 0);
    dump("02_segalogo");
    t = run_until_scene("TitleDemo", 400);
    CHECK(t >= 0);
    printf("SegaLogo -> TitleDemo at frame %d\n", s_frame);
    CHECK(s_frame >= 2 + 32 + 120 + 32 + 30 && s_frame <= 2 + 32 + 120 + 32 + 30 + 3);
    {   /* TitleDemo (mn_titledemo.c): scenes of 0x20, 0x48, 0x48, 0x20, 10, 0x28 frames */
        int f0 = s_frame, n3d = 0, i;
        run(0x20 + 0x40, 0, 0);
        CHECK(g_mn_bright[1] == 0);            /* scene 2: the logo screen in, the picture fading */
        dump("02a_titledemo_blaze");
        run(0x48, 0, 0);
        dump("02b_titledemo_sonic");
        for (i = 0; i < 0x20 + 10; i++) {
            run(1, 0, 0);
            n3d = mn_3d_count() > n3d ? mn_3d_count() : n3d;
            if (i == 0x10)
                dump("02c_titledemo_spin");
            if (i == 10) {   /* scene 4 frame 2: the magnified RUSH (scale ~0.5) is cut to the bottom screen's strip */
                const MnDraw *sd;
                int ns = mn_sorted(&sd), k, found = 0;
                for (k = 0; k < ns; k++)
                    if (sd[k].q->kind == MNQ_AFFINE) {
                        float p[8][4], xy[4][2];
                        int n = mn_affine_clip(sd[k].q, sd[k].r, p), j, inside = 1, outside = 0;
                        mn_affine_corners(sd[k].q, xy);
                        for (j = 0; j < 4; j++)
                            outside |= xy[j][0] < sd[k].r->sx0 || xy[j][0] > sd[k].r->sx1 || xy[j][1] < sd[k].r->sy0 ||
                                       xy[j][1] > sd[k].r->sy1;
                        for (j = 0; j < n; j++)
                            inside &= p[j][0] >= sd[k].r->sx0 - 0.01f && p[j][0] <= sd[k].r->sx1 + 0.01f &&
                                      p[j][1] >= sd[k].r->sy0 - 0.01f && p[j][1] <= sd[k].r->sy1 + 0.01f;
                        CHECK(outside && n >= 3 && inside);
                        found = 1;
                    }
                CHECK(found);
            }
        }
        CHECK(n3d == 2);                       /* scenes 4 / 5: Sonic and Blaze */
        dump("02d_titledemo_models");
        t = run_until_scene("TitleScreen", 100);
        CHECK(t >= 0);
        printf("TitleDemo -> TitleScreen after %d frames\n", s_frame - f0);
        CHECK(s_frame - f0 >= 0x20 + 0x48 + 0x48 + 0x20 + 10 + 0x28 && s_frame - f0 <= 0x20 + 0x48 + 0x48 + 0x20 + 10 + 0x28 + 3);
    }
    run(80, 0, 0);
    dump("03_title");
    run(30, 0, 0);
    dump("04_title_blink");

    /* attract demo: no input for 0x4b0 frames after the title screen starts */
    {
        MenuRequest r = run(0x4b0 + 10, 0, 0);
        CHECK(r.kind == MENU_REQ_DEMO && r.demo == 1);
        printf("attract demo request at frame %d (demo %d)\n", s_frame, r.demo);
    }
    menus_stage_exit(MENU_EXIT_DEMO_END, 0, 0, 0, 0);
    CHECK(strcmp(g_mn_scene, "TitleScreen") == 0 || run_until_scene("TitleScreen", 5) >= 0);
    run(0x50, 0, 0);
    /* START -> TitleMenu after 0x20 frames (fade to white) */
    tap(IN_START);
    t = run_until_scene("TitleMenu", 60);
    CHECK(t >= 0);
    run(4, 0, 0);
    dump("05_titlemenu_open");
    run(40, 0, 0);
    dump("06_titlemenu_root");
    /* DOWN moves the cursor, UP back, then A on GAME START */
    s_nse = 0;
    tap(IN_DOWN);
    run(3, 0, 0);
    /* SysSound_PlaySE(4) = cursor: sys SDAT archive 1, sequence 5 (mn_story.c) */
    CHECK(s_nse >= 1 && s_se[0] == 5 && s_arc0 == 1);
    dump("07_titlemenu_down");
    tap(IN_UP);
    run(3, 0, 0);
    tap(IN_JUMP);
    /* new save: progress 0 -> Game_StartStoryStage(0, 0) */
    {
        MenuRequest r = run(200, 0, 0);
        CHECK(r.kind == MENU_REQ_STAGE && r.zone == 0 && r.act == 0 && r.character == 0 && r.mode == 0);
        CHECK(r.lives == 2);
        printf("GAME START -> stage request zone %d act %d at frame %d\n", r.zone, r.act, s_frame);
    }

    /* ---------------- story: zone map at progress 2 (after the first act) */
    SaveData_SetSonicProgress(2);
    gSaveData.unkE = 5;
    {
        MenuRequest r = menus_stage_exit(MENU_EXIT_GAME_OVER, 0, 0, 0, 0);
        CHECK(r.kind == MENU_REQ_NONE && strcmp(g_mn_scene, "SegaLogo") == 0);
    }
    /* the title demo skipped with A in scene 2: 0x20 frames to white, then the title screen */
    t = run_until_scene("TitleDemo", 400);
    CHECK(t >= 0);
    run(0x30, 0, 0);
    tap(IN_JUMP);
    t = run_until_scene("TitleScreen", 0x20 + 4);
    CHECK(t >= 0 && g_mn_bright[0] == 0x10);
    run(0x50, 0, 0);
    tap(IN_START);
    run_until_scene("TitleMenu", 60);
    run(50, 0, 0);
    tap(IN_JUMP);
    t = run_until_scene("ZoneMap", 200);
    CHECK(t >= 0);
    run(20, 0, 0);
    dump("10_zonemap_start");
    /* step 0: talk demo (skipped) -> 0xf0 wait -> msgbox 11 -> path 0 revealed
     * from node 1 to node 2 -> progress saved as 3 */
    {
        int i;
        for (i = 0; i < 1200 && gSaveData.unkC != 3; i++)
            run_answering(1);
        CHECK(gSaveData.unkC == 3);
        printf("zone map: path to zone 2 revealed, progress 3 saved after %d frames\n", i);
        run(40, 0, 0);
        dump("11_zonemap_revealed");
    }
    /* RIGHT walks to node 2, A asks to start zone 2 (index 1): story stage */
    run(5, IN_RIGHT, 0);
    run(80, 0, 0);
    dump("12_zonemap_walked");
    tap(IN_JUMP);
    {
        MenuRequest r;
        run(40, 0, 0);
        CHECK(!MsgBox_IsClosed());   /* MsgBox 0x12 "go to this zone?" */
        dump("12b_zonemap_msgbox");
        r = run_answering(400);
        CHECK(r.kind == MENU_REQ_STAGE && r.zone == 1 && r.act == 0 && r.character == 0);
        printf("zone 2 story stage request at frame %d\n", s_frame);
    }
    /* clear act 1 -> act 2 straight away, then the boss act, then the map */
    {
        MenuRequest r = menus_stage_exit(MENU_EXIT_CLEARED, 40000, 6, 0, 3000);
        CHECK(r.kind == MENU_REQ_STAGE && r.zone == 1 && r.act == 1);
        CHECK(gSaveData.unk10[1][0] == 40000);
        r = menus_stage_exit(MENU_EXIT_CLEARED, 30000, 6, 0, 3000);
        CHECK(r.kind == MENU_REQ_STAGE && r.zone == 1 && r.act == 2);
        CHECK(gSaveData.unkC == 4);
        r = menus_stage_exit(MENU_EXIT_CLEARED, 20000, 7, 1, 3000);
        CHECK(r.kind == MENU_REQ_NONE && strcmp(g_mn_scene, "ZoneMap") == 0);
        CHECK(gSaveData.unkC == 5 && gSaveData.unkE == 7 && gSaveData.unk10[1][2] == 20000);
    }
    /* reboot: the save comes back from the "VMU" */
    {
        int prog = gSaveData.unkC;
        memset(&gSaveData, 0, sizeof(gSaveData));
        CHECK(save_load() == SAVE_OK && gSaveData.unkC == prog);
    }
    /* replay zone 1 through the act select: walk LEFT to node 1, A */
    {
        extern s32 g_zm_node, g_zm_select_ticks;
        int i, t0 = g_zm_select_ticks;
        for (i = 0; i < 3000 && g_zm_select_ticks < t0 + 2; i++)
            run_answering(1);
        printf("zone map: story sequence done after %d frames, node %d, progress %d\n", i, g_zm_node, gSaveData.unkC);
        /* step 6..11: path to the house (node 15), walk there, path to zone 3 */
        CHECK(g_zm_node == 15 && gSaveData.unkC == 7);
        dump("13a_zonemap_house");
        /* START: map pause, B closes it, the map continues */
        tap(IN_START);
        run(20, 0, 0);
        dump("13b_zonemap_pause");
        tap(IN_TRICK);
        run(30, 0, 0);
        CHECK(g_zm_node == 15);
        run(5, IN_RIGHT, 0);
        run(100, 0, 0);
        CHECK(g_zm_node == 2);
        run(5, IN_DOWN, 0);
        run(100, 0, 0);
        CHECK(g_zm_node == 1);
        run(100, 0, 0);
        dump("13_zonemap_node1");
        tap(IN_JUMP);
        t = run_until_scene("ActSelect", 120);
        CHECK(t >= 0);
        run(40, 0, 0);
        tap(IN_RIGHT);
        run(10, 0, 0);
        dump("14_actselect");
        tap(IN_DOWN);
        run(5, 0, 0);
        tap(IN_JUMP);
        {
            MenuRequest r = run(300, 0, 0);
            CHECK(r.kind == MENU_REQ_STAGE && r.act == 1);
            printf("act select -> zone %d act %d\n", r.zone, r.act);
        }
    }

    /* ---------------- player select (progress >= 3): Sonic / Blaze */
    menus_stage_exit(MENU_EXIT_GAME_OVER, 0, 0, 0, 0);
    run_until_scene("TitleScreen", 700);
    run(0x50, 0, 0);
    tap(IN_START);
    run_until_scene("TitleMenu", 60);
    run(50, 0, 0);
    tap(IN_JUMP);
    t = run_until_scene("PlayerSelect", 200);
    CHECK(t >= 0);
    run(120, 0, 0);
    dump("20_psel_sonic");
    run(2, IN_RIGHT, 0);
    run(40, 0, 0);
    dump("21_psel_blaze");
    tap(IN_JUMP);
    run(60, 0, 0);
    dump("22_psel_confirm");
    t = run_until_scene("ZoneMap", 400);
    CHECK(t >= 0 && gGameState.unk0 == 1);
    run(40, 0, 0);
    dump("23_zonemap_blaze");

    /* ---------------- options: difficulty, time limit, sound test, erase */
    SaveData_UnlockSoundTest();
    menus_stage_exit(MENU_EXIT_GAME_OVER, 0, 0, 0, 0);
    run_until_scene("TitleScreen", 700);
    run(0x50, 0, 0);
    tap(IN_START);
    run_until_scene("TitleMenu", 60);
    run(50, 0, 0);
    tap(IN_DOWN);
    run(3, 0, 0);
    tap(IN_DOWN);
    run(3, 0, 0);
    dump("30_titlemenu_options");
    tap(IN_JUMP);
    t = run_until_scene("Options", 200);
    CHECK(t >= 0);
    run(30, 0, 0);
    dump("31a_options_opening");   /* the list slides in (mnu_opti.ama seqs 1.., 20, 25, 27: 41 frames) */
    run(15, 0, 0);
    dump("31_options");
    /* DIFFICULTY -> EASY (PLAYER DATA is the first item) */
    tap(IN_DOWN);
    run(3, 0, 0);
    tap(IN_JUMP);
    run(36, 0, 0);
    tap(IN_RIGHT);
    run(3, 0, 0);
    dump("32_options_difficulty");
    tap(IN_JUMP);
    run(30, 0, 0);
    dump("32a_options_back_slide");   /* the page slides out to the right, the list in (seqs 23, 17, 18) */
    run(25, 0, 0);
    CHECK((gSaveData.unk8 & 1) == 1);
    /* TIME LIMIT -> OFF */
    tap(IN_DOWN);
    run(3, 0, 0);
    tap(IN_JUMP);
    run(36, 0, 0);
    tap(IN_RIGHT);
    run(3, 0, 0);
    tap(IN_JUMP);
    run(55, 0, 0);
    CHECK((gSaveData.unk8 & 2) == 2);
    /* SOUND TEST: RIGHT twice, A plays track 2 */
    tap(IN_DOWN);
    run(3, 0, 0);
    tap(IN_JUMP);
    run(36, 0, 0);
    tap(IN_RIGHT);
    run(3, 0, 0);
    tap(IN_RIGHT);
    run(3, 0, 0);
    tap(IN_JUMP);
    run(40, 0, 0);
    dump("33_options_soundtest");
    CHECK(s_st_index == 2 && s_st_type == 0);
    tap(IN_TRICK);
    run(3, 0, 0);
    tap(IN_TRICK);
    run(40, 0, 0);
    /* B: exit commits the save and returns to the title menu (at OPTIONS) */
    tap(IN_TRICK);
    run(20, 0, 0);
    dump("34_options_exit_slide");   /* the list slides out, white after 0x1e frames */
    t = run_until_scene("TitleMenu", 60);
    CHECK(t >= 0);
    memset(&gSaveData, 0, sizeof(gSaveData));
    CHECK(save_load() == SAVE_OK && (gSaveData.unk8 & 3) == 3);
    printf("options: easy + no time limit saved, sound test played track %d\n", s_st_index);

    printf("menus host test: %d/%d checks passed\n", checks - fails, checks);
    return fails ? 1 : 0;
}
