/* Host test of the time-attack / records scenes and the attract-demo exits of the
 * front end (mn_timeattack.c, mn_tryagain.c, mn_records.c, mn_options.c PLAYER DATA,
 * menus_stage_exit) on the owner's converted data. With a dump directory, writes
 * the single-screen 640x480 compositions as PPM (MN_DUMP_ENGINES=1: each DS engine
 * alone too).
 * Usage: test_flow <build/dreamcast/menus> <build/dreamcast/sprites/bac> [dumpdir] */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mn_raster.h"
#include "../extraact.h"
#include "../../save/save.h"

static int fails, checks;
#define CHECK(c)                                                                         \
    do {                                                                                 \
        checks++;                                                                        \
        if (!(c)) {                                                                      \
            fails++;                                                                     \
            printf("FAIL %s:%d: %s (scene %s)\n", __FILE__, __LINE__, #c, g_mn_scene); \
        }                                                                                \
    } while (0)

static const char *s_dump;
static uint8_t s_img[SAVE_IMAGE_SIZE];
static int s_have_img;
static int h_present(void) { return 1; }
static int h_read(uint8_t *img, int size) {
    if (!s_have_img || size != (int)sizeof(s_img))
        return 1;
    memcpy(img, s_img, (size_t)size);
    return 0;
}
static int h_write(const uint8_t *img, int size) {
    if (size != (int)sizeof(s_img))
        return -1;
    memcpy(s_img, img, (size_t)size);
    s_have_img = 1;
    return 0;
}

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
        if (r.kind != MENU_REQ_NONE)
            return r;
    }
    return r;
}
static MenuRequest tap(uint32_t b) { return run(2, b, 1); }
static void dump(const char *name) {
    char p[300];
    if (!s_dump)
        return;
    snprintf(p, sizeof(p), "%s/%s.ppm", s_dump, name);
    raster_frame(p);
    if (getenv("MN_DUMP_ENGINES")) {
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
/* quads of a resource in the current frame */
static int count_res(const char *id) {
    const MnQuad *q;
    int n = mn_quads(&q), i, k = 0;
    for (i = 0; i < n; i++)
        if (q[i].res && strcmp(q[i].res->id, id) == 0)
            k++;
    return k;
}
static void start_scene(void (*fn)(void)) {
    Task_DestroyAll();
    mn_display_reset();
    fn();
}

static void test_time_attack(void) {
    MenuRequest r;
    int t, frames;
    /* records of zone 0 act 0 (Sonic): 1'02"50 by Sonic, 1'10"00 by Blaze */
    SaveData_InsertTimeRecord(0, 0, 3750, 0, 0, 0);
    SaveData_InsertTimeRecord(0, 0, 4200, 1, 0, 0);
    gGameState.unk0 = 0;
    gGameState.unk1C = 0;
    gGameState.unk1D = 0;
    gGameState.unk10 = 2;
    BRIGHT_MAIN = BRIGHT_SUB = -16;
    start_scene(TimeAttackMenu_StartMode0);
    CHECK(strcmp(g_mn_scene, "TimeAttackMenu") == 0);
    run(10, 0, 0);
    dump("40_ta_enter");
    for (frames = 0; frames < 200 && strcmp(mn_scene_name(), "TimeAttackMenu") == 0; frames++) {
        run(1, 0, 0);
        if (count_res("act__mnu_time_ranking") >= 5 * 8 && frames > 40)
            break;
    }
    run(30, 0, 0);
    dump("41_ta_select");
    if (getenv("MN_QUADS")) {
        const MnQuad *q;
        int n = mn_quads(&q), i;
        for (i = 0; i < n; i++)
            printf("  q %s e%d p%d k%d %.0f,%.0f %.0fx%.0f uv %.0f,%.0f\n", q[i].res ? q[i].res->id : "-", q[i].engine,
                   q[i].prio, q[i].kind, q[i].x, q[i].y, q[i].w, q[i].h, q[i].u0, q[i].v0);
    }
    CHECK(TimeAttackMenu_Selected() == 0);
    CHECK(count_res("act__mnu_time_oneself_eng") == 2);   /* SONIC + BLAZE names of the two records */
    /* RIGHT: act 2 of zone 1 (the rows slide out and back) */
    tap(IN_RIGHT);
    run(6, 0, 0);
    dump("42_ta_scroll");
    run(60, 0, 0);
    CHECK(TimeAttackMenu_Selected() == 1);
    /* LEFT twice wraps to the last entry (zone X act 4) */
    tap(IN_LEFT);
    run(40, 0, 0);
    tap(IN_LEFT);
    run(60, 0, 0);
    CHECK(TimeAttackMenu_Selected() == 17);
    dump("43_ta_zonex");
    tap(IN_RIGHT);
    run(60, 0, 0);
    tap(IN_RIGHT);
    run(60, 0, 0);
    CHECK(TimeAttackMenu_Selected() == 1);
    /* A: Game_StartTimeAttackStage(zone order[0], act 1) after 16 frames */
    r = tap(IN_JUMP);
    for (t = 0; t < 40 && r.kind == MENU_REQ_NONE; t++)
        r = run(1, 0, 0);
    printf("time attack menu: stage request after %d frames: zone %d act %d mode %d\n", t + 2, r.zone, r.act, r.mode);
    CHECK(r.kind == MENU_REQ_STAGE && r.zone == 0 && r.act == 1 && r.mode == MENU_MODE_TIME_ATTACK);

    /* the stage is cleared in 1'05"00: the record is inserted, TryAgain opens */
    r = menus_stage_exit(MENU_EXIT_CLEARED, 0, 3, 0, 3900);
    CHECK(r.kind == MENU_REQ_NONE && strcmp(g_mn_scene, "TryAgain") == 0);
    CHECK(gSaveData.unkD2[0][1][0] == 3900);
    run(8, 0, 0);
    dump("44_tryagain_open");
    run(40, 0, 0);
    dump("45_tryagain");
    CHECK(TryAgain_Selected() == 0);
    /* TRY AGAIN: the same stage again */
    tap(IN_JUMP);
    r = run(0x40, 0, 0);
    CHECK(r.kind == MENU_REQ_STAGE && r.zone == 0 && r.act == 1 && r.mode == MENU_MODE_TIME_ATTACK);
    /* again, CHANGE ACT -> the time-attack menu at that stage */
    menus_stage_exit(MENU_EXIT_CLEARED, 0, 3, 0, 4000);
    run(60, 0, 0);
    tap(IN_DOWN);
    run(3, 0, 0);
    tap(IN_DOWN);
    run(3, 0, 0);
    CHECK(TryAgain_Selected() == 2);
    dump("46_tryagain_change_act");
    tap(IN_JUMP);
    run(0x20, 0, 0);
    dump("47_tryagain_confirm");
    run(0x20, 0, 0);
    CHECK(strcmp(g_mn_scene, "TimeAttackMenu") == 0 && TimeAttackMenu_Selected() == 1);
    /* B: back to the player select (mode 3) */
    run(200, 0, 0);
    tap(IN_TRICK);
    run(30, 0, 0);
    CHECK(strcmp(g_mn_scene, "PlayerSelect") == 0);
    /* QUIT from TryAgain: the title screen */
    gGameState.unk10 = 2;
    gGameState.unk1D = 2;   /* a boss: CHANGE BOSS */
    menus_stage_exit(MENU_EXIT_CLEARED, 0, 3, 0, 5000);
    run(40, 0, 0);
    tap(IN_UP);
    run(3, 0, 0);
    CHECK(TryAgain_Selected() == 3);
    dump("48_tryagain_boss_quit");
    tap(IN_JUMP);
    run(0x45, 0, 0);
    CHECK(strcmp(g_mn_scene, "TitleScreen") == 0);

    /* boss mode: 7 bosses, A starts act 2 */
    gGameState.unk0 = 1;   /* Blaze: zone order 3, 0, 2, 1 ... */
    gGameState.unk1C = 3;
    gGameState.unk1D = 2;
    start_scene(TimeAttackMenu_StartMode1);
    run(120, 0, 0);
    dump("49_ta_boss");
    CHECK(TimeAttackMenu_Selected() == 0);
    tap(IN_RIGHT);
    run(60, 0, 0);
    r = tap(IN_JUMP);
    for (t = 0; t < 40 && r.kind == MENU_REQ_NONE; t++)
        r = run(1, 0, 0);
    CHECK(r.kind == MENU_REQ_STAGE && r.zone == 0 && r.act == 2 && r.character == 1);
    printf("time attack boss: zone %d act %d (Blaze order)\n", r.zone, r.act);
}

static void test_records(void) {
    static const u16 names[6][5] = { { 'T', 'A', 'I', 'L', 'S' }, { 'C', 'R', 'E', 'A', 'M' },
                                     { 'A', 'M', 'Y' },           { 'K', 'N', 'U', 'X' },
                                     { 'R', 'O', 'U', 'G', 'E' }, { 'S', 'H', 'A', 'D', 'O' } };
    static const u16 me[5] = { 'S', 'O', 'N', 'I', 'C' };
    int i, j, t;
    MenuRequest r;
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 5; j++)
            gSaveData.unk1D4[i].name[j] = names[i][j];
        gSaveData.unk1D4[i].len = (u8)(names[i][2] == 'Y' ? 3 : names[i][3] == 'X' ? 4 : 5);
        gSaveData.unk1D4[i].cnt[0] = (u8)(i + 1);
        gSaveData.unk1D4[i].cnt[1] = (u8)(2 * i);
        gSaveData.unk1D4[i].cnt[2] = (u8)(i == 3);
    }
    gSaveData.unk1D0[0] = 21;
    gSaveData.unk1D0[1] = 30;
    gSaveData.unk1D0[2] = 1;
    menus_set_owner_name(me, 5);
    /* Options > PLAYER DATA > BATTLE PLAY RECORD */
    start_scene(Options_StartFromTitleMenu);
    run(30, 0, 0);
    tap(IN_JUMP);
    run(5, 0, 0);
    dump("50_options_player_data");
    tap(IN_JUMP);
    t = run_until_scene("Records", 60);
    CHECK(t >= 0);
    run(30, 0, 0);
    dump("51_records");
    CHECK(Records_Scroll() == 0);
    tap(IN_DOWN);
    run(12, 0, 0);
    CHECK(Records_Scroll() == 16);   /* one 16-px page down (max (6 - 4) * 16) */
    tap(IN_DOWN);
    run(12, 0, 0);
    CHECK(Records_Scroll() == 32);
    tap(IN_DOWN);
    run(12, 0, 0);
    CHECK(Records_Scroll() == 32);   /* the end */
    dump("52_records_scrolled");
    tap(IN_UP);
    run(12, 0, 0);
    tap(IN_UP);
    run(12, 0, 0);
    CHECK(Records_Scroll() == 0);
    tap(IN_TRICK);
    t = run_until_scene("Options", 40);
    CHECK(t >= 0);
    run(20, 0, 0);
    dump("53_options_back_from_records");
    /* TIME ATTACK RECORD > BOSS: the record list, A does nothing, B back */
    tap(IN_DOWN);
    run(3, 0, 0);
    tap(IN_JUMP);
    run(5, 0, 0);
    tap(IN_RIGHT);
    run(3, 0, 0);
    dump("54_options_ta_kind");
    tap(IN_JUMP);
    t = run_until_scene("TimeAttackMenu", 60);
    CHECK(t >= 0);
    run(120, 0, 0);
    dump("55_ta_records_boss");
    r = tap(IN_JUMP);
    run(30, 0, 0);
    CHECK(r.kind == MENU_REQ_NONE && strcmp(g_mn_scene, "TimeAttackMenu") == 0);
    tap(IN_TRICK);
    t = run_until_scene("Options", 40);
    CHECK(t >= 0);
    printf("records: 6 opponents, scroll 0 -> 16 -> 32 (end) -> 0; options PLAYER DATA round trips ok\n");
}

static void test_demo_exits(void) {
    MenuRequest r;
    start_scene(TitleMenu_Start);
    r = menus_stage_exit(MENU_EXIT_DEMO_TIMEOUT, 0, 0, 0, 0);
    CHECK(r.kind == MENU_REQ_NONE && strcmp(g_mn_scene, "SegaLogo") == 0);
    r = menus_stage_exit(MENU_EXIT_DEMO_END, 0, 0, 0, 0);
    CHECK(r.kind == MENU_REQ_NONE);
    CHECK(run_until_scene("TitleScreen", 5) >= 0);
    printf("attract demo exits: timeout -> SegaLogo, key -> TitleScreen\n");
}


/* ExtraAct: the 3D model vertices are plotted where nothing else is drawn */
static void dump_ex(const char *name) {
    static float xyz[3 * 60000];
    char p[300];
    int n, i;
    FILE *f;
    if (!s_dump)
        return;
    snprintf(p, sizeof(p), "%s/%s.ppm", s_dump, name);
    raster_frame(p);
    n = extraact_project(xyz, 60000);
    for (i = 0; i < n; i++) {
        int x = (int)xyz[3 * i], y = (int)xyz[3 * i + 1];
        if (x >= 0 && x < 640 && y >= 0 && y < 480 && !g_img[y][x][0] && !g_img[y][x][1] && !g_img[y][x][2]) {
            g_img[y][x][0] = 200;
            g_img[y][x][1] = 200;
            g_img[y][x][2] = 220;
        }
    }
    f = fopen(p, "wb");
    if (!f)
        return;
    fprintf(f, "P6\n640 480\n255\n");
    fwrite(g_img, 1, sizeof(g_img), f);
    fclose(f);
}
static int ex_run_until_done(int max, int tap_every, int *first_white) {
    InputState in;
    int f;
    for (f = 0; f < max; f++) {
        memset(&in, 0, sizeof(in));
        if (tap_every && f % tap_every == 0 && f > 20)
            in.held = IN_JUMP;
        if (extraact_run_frame(&in))
            return f + 1;
        extraact_draw();
        if (first_white && *first_white < 0 && f > 100 && BRIGHT_MAIN == 16)
            *first_white = f;
        if (!first_white)
            continue;   /* frame dumps of the first (unskipped) run only */
        if (f == 10)
            dump_ex("60_extraact_fadein");
        if (f == 300)
            dump_ex("61_extraact_model");
        if (f == 680)
            dump_ex("62_extraact_title");
        if (f == 900)
            dump_ex("63_extraact_tally");
        if (f == 1200)
            dump_ex("64_extraact_rank");
    }
    return -1;
}
static void test_extraact(const char *menus_dir) {
    char dir[300];
    int32_t r[4];
    int frames, verts, white = -1;
    static float xyz[3 * 60000];
    snprintf(dir, sizeof(dir), "%s/../models", menus_dir);
    extraact_set_model_dir(dir);
    CHECK(extraact_start(17000, 120, 0) == 0);
    CHECK(strcmp(g_mn_scene, "ExtraAct") == 0);
    extraact_result(r);
    printf("extraact: time 17000 -> time bonus %d, rings 120 -> %d, total %d, rank %d\n", r[0], r[1], r[2], r[3]);
    CHECK(r[0] == 38000 && r[1] == 12000 && r[2] == 50000);
    {
        InputState in;
        memset(&in, 0, sizeof(in));
        extraact_run_frame(&in);
        verts = extraact_project(xyz, 60000);
    }
    CHECK(verts > 0);
    frames = ex_run_until_done(3000, 0, &white);
    printf("extraact: %d model vertices, scene over after %d frames (white from %d)\n", verts, frames, white);
    CHECK(frames > 600 && frames < 2000);
    extraact_shutdown();
    /* A skips: the same end, much sooner */
    CHECK(extraact_start(30000, 0, 1) == 0);
    extraact_result(r);
    CHECK(r[0] == 0 && r[3] >= 0);
    frames = ex_run_until_done(3000, 30, NULL);
    printf("extraact skipped with A: over after %d frames (rank %d)\n", frames, r[3]);
    CHECK(frames > 0 && frames < 400);
    extraact_shutdown();
}

/* the zone-map navigator (mn_navi.c): no frame dumps here, only its state */
static void test_navi(void) {
    int face, panel, alpha, i, n0;
    gSaveData.unkC = 5;           /* Sonic's story far enough for the navigator */
    gGameState.unk0 = 0;
    gGameState.lives = 12;
    start_scene(ZoneMap_StartDefault);
    gGameState.unkE = 3;
    NaviM_ResetPending();
    gGameState.unkE = 4;          /* one step further than when the stage started */
    CHECK(NaviM_CheckStepAdvanced() == 1);
    NaviM_Destroy();
    run(1, 0, 0);
    NaviM_Create();
    run(2, 0, 0);
    CHECK(mn_navi_state(NULL, NULL, NULL) == 3);
    CHECK(mn_navi_counter() == 5);           /* data_020778b0[3] until the count starts */
    NaviM_StartCounter();
    CHECK(NaviM_IsCounting());
    run(2, 0, 0);
    CHECK(mn_navi_counter() == 5);           /* 5 -> 5: no step */
    CHECK(!NaviM_IsCounting());
    /* intro: the face fades in over 60 frames, then the intro panel (26) for 0xf0 frames */
    NaviMsg_StartIntro();
    run(30, 0, 0);
    mn_navi_state(&face, &panel, &alpha);
    CHECK(alpha >= 14 && alpha <= 16 && face >= 0);
    CHECK(mn_navi_face_pixels() > 2000);    /* an RLE frame expanded into the face texture */
    run(31, 0, 0);
    mn_navi_state(&face, &panel, &alpha);
    CHECK(alpha == 31);
    run(6, 0, 0);
    mn_navi_state(&face, &panel, &alpha);
    CHECK(panel == 26);
    n0 = g_mn_comp.n;
    CHECK(n0 == 4);                          /* map, NaviM panel, face, balloon */
    run(0xf0, 0, 0);
    mn_navi_state(NULL, &panel, NULL);
    CHECK(panel == -1);
    /* a map message: panel 4 for 180 frames */
    NaviMsg_ShowMessage(4, 0xb4);
    NaviMsg_SetFaceAnim(6);
    for (i = 0; i < 8; i++) {
        run(1, 0, 0);
        mn_navi_state(NULL, &panel, NULL);
        if (panel == 4)
            break;
    }
    CHECK(panel == 4);
    run(0xb4, 0, 0);
    mn_navi_state(NULL, &panel, NULL);
    CHECK(panel == -1);
    /* fade out: the face is gone after 30 frames, the map stays */
    NaviMsg_FadeVisible(FALSE);
    run(31, 0, 0);
    mn_navi_state(NULL, NULL, &alpha);
    CHECK(alpha == -1 && g_mn_comp.n == 2);
    /* locked early in the story: no messages */
    gSaveData.unkC = 1;
    NaviMsg_ShowMessage(4, 0xb4);
    run(8, 0, 0);
    mn_navi_state(NULL, &panel, NULL);
    CHECK(panel == -1);
    gSaveData.unkC = 5;
    /* act select over the map: its intro message (panel 0), the act-chosen one (4) */
    ZoneActSelect_Create(0, 1, 0);
    for (i = 0; i < 8; i++) {
        run(1, 0, 0);
        mn_navi_state(NULL, &panel, NULL);
        if (panel == 0)
            break;
    }
    CHECK(panel == 0 && g_mn_comp.n >= 2);
    run(40, 0, 0);
    tap(IN_JUMP);
    for (i = 0; i < 8; i++) {
        run(1, 0, 0);
        mn_navi_state(NULL, &panel, NULL);
        if (panel == 4)
            break;
    }
    CHECK(panel == 4);
    start_scene(TitleMenu_Start);
    CHECK(mn_navi_state(NULL, NULL, NULL) == 0);
}

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("usage: %s menus_dir sprites_dir [dumpdir]\n", argv[0]);
        return 2;
    }
    s_dump = argc > 3 ? argv[3] : NULL;
    g_save_backend.present = h_present;
    g_save_backend.read = h_read;
    g_save_backend.write = h_write;
    menus_set_paths(argv[1], argv[2]);
    CHECK(menus_init() == 0);
    test_time_attack();
    test_records();
    test_demo_exits();
    test_extraact(argv[1]);
    test_navi();
    printf("flow host test: %d/%d checks passed\n", checks - fails, checks);
    return fails ? 1 : 0;
}
