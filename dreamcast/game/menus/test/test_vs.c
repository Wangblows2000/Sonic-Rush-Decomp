/* Host test of Battle Play (local two-controller VS) in the front end: title menu VS page ->
 * both players' character select (mn_psel.c modes 1 / 2) -> TimeSelect stage select
 * (mn_timeselect.c) -> MENU_REQ_VS_STAGE; menus_vs_exit -> ResultsScreen (mn_results.c) ->
 * rematch / character select / quit. Two pads: player 1 through menus_run_frame, player 2
 * through menus_set_pad2. With a dump directory, writes the 640x480 compositions as PPM
 * (MN_DUMP_ENGINES=1: each DS engine alone too).
 * Usage: test_vs <build/dreamcast/menus> <build/dreamcast/sprites/bac> [dumpdir] */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mn_raster.h"
#include "../../save/save.h"
#include "../../model/model.h"

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

static int s_voice[16], s_nvoice;
static void h_voice(int id) {
    if (s_nvoice < 16)
        s_voice[s_nvoice++] = id;
}
static int s_bgm = -1;
static void h_bgm(int seq) { s_bgm = seq; }

/* n frames with player 1 / player 2 holding p1 / p2 (tap: only on the first frame) */
static MenuRequest run2(int n, uint32_t p1, uint32_t p2, int tap) {
    InputState a, b;
    MenuRequest r;
    int i;
    memset(&r, 0, sizeof(r));
    for (i = 0; i < n; i++) {
        memset(&a, 0, sizeof(a));
        memset(&b, 0, sizeof(b));
        a.held = (tap && i > 0) ? 0 : p1;
        b.held = (tap && i > 0) ? 0 : p2;
        menus_set_pad2(&b);
        r = menus_run_frame(&a);
        menus_draw_tr();
        if (r.kind != MENU_REQ_NONE)
            return r;
    }
    return r;
}
static MenuRequest run(int n) { return run2(n, 0, 0, 0); }
static MenuRequest tap1(uint32_t b) { return run2(2, b, 0, 1); }
static MenuRequest tap2(uint32_t b) { return run2(2, 0, b, 1); }
static void dump(const char *name) {
    char p[300];
    if (!s_dump)
        return;
    snprintf(p, sizeof(p), "%s/%s.ppm", s_dump, name);
    raster_frame(p);
    if (getenv("MN_QUADS")) {
        const MnQuad *q;
        int n = mn_quads(&q), i;
        printf("%s:\n", name);
        for (i = 0; i < n; i++)
            printf("  q %s e%d p%d o%d k%d %.0f,%.0f %.0fx%.0f\n", q[i].res ? q[i].res->id : "-", q[i].engine,
                   q[i].prio, q[i].oam, q[i].kind, q[i].x, q[i].y, q[i].w, q[i].h);
    }
    if (getenv("MN_DUMP_ENGINES")) {
        MnComp keep = g_mn_comp;
        int e;
        for (e = 0; e < 4; e++) {
            mn_comp_reset(e);
            mn_comp_full(e);
            snprintf(p, sizeof(p), "%s/raw/%s_e%d.ppm", s_dump, name, e);
            raster_frame(p);
        }
        g_mn_comp = keep;
    }
}
/* the 3D layer's vertices of a frame (menus_draw_list through the dcm host hook) */
static float s_vx0, s_vx1, s_vy0, s_vy1;
static int s_nv;
static void count_vertex(int material, const float *p, uint32_t argb, int eol) {
    (void)material;
    (void)argb;
    (void)eol;
    if (s_nv++ == 0) {
        s_vx0 = s_vx1 = p[0];
        s_vy0 = s_vy1 = p[1];
    }
    s_vx0 = p[0] < s_vx0 ? p[0] : s_vx0;
    s_vx1 = p[0] > s_vx1 ? p[0] : s_vx1;
    s_vy0 = p[1] < s_vy0 ? p[1] : s_vy0;
    s_vy1 = p[1] > s_vy1 ? p[1] : s_vy1;
}
static int model_vertices(void) {
    s_nv = 0;
    dcm_host_emit = count_vertex;
    menus_draw_list(0);
    dcm_host_emit = NULL;
    return s_nv;
}

static int scene_is(const char *s) { return strcmp(g_mn_scene, s) == 0; }
static int run_until_scene(const char *scene, int max) {
    int i;
    for (i = 0; i < max; i++) {
        if (scene_is(scene))
            return i;
        run(1);
    }
    return -1;
}
static int run_until_psel(int p1, int p2, int max) {
    int i;
    for (i = 0; i < max; i++) {
        if (PlayerSelectMenu_VsState(0, NULL) == p1 && PlayerSelectMenu_VsState(1, NULL) == p2)
            return i;
        run(1);
    }
    return -1;
}
static int run_until_ts(int st, int max) {
    int i;
    for (i = 0; i < max; i++) {
        if (TimeSelectMenu_StateId() == st)
            return i;
        run(1);
    }
    return -1;
}
static int count_res(const char *id, int engine) {
    const MnQuad *q;
    int n = mn_quads(&q), i, k = 0;
    for (i = 0; i < n; i++)
        if (q[i].res && strcmp(q[i].res->id, id) == 0 && (engine < 0 || q[i].engine == engine))
            k++;
    return k;
}
static int engine_quads(int engine) {
    const MnQuad *q;
    int n = mn_quads(&q), i, k = 0;
    for (i = 0; i < n; i++)
        k += q[i].engine == engine;
    return k;
}

/* title menu -> VS page -> wireless sub-page -> host: Battle Play's character select */
static void test_title_to_vs(void) {
    int i;
    Task_DestroyAll();
    mn_display_reset();
    BRIGHT_MAIN = BRIGHT_SUB = 0;
    TitleMenu_Start();
    run(90);
    tap1(IN_DOWN);   /* GAME START -> VS */
    run(30);
    dump("vs00_title_vs_item");
    tap1(IN_JUMP);   /* VS page */
    run(90);
    dump("vs01_title_vs_page");
    CHECK(scene_is("TitleMenu"));
    tap1(IN_JUMP);   /* DS wireless play -> host / join page */
    run(90);
    dump("vs02_title_vs_subpage");
    tap1(IN_JUMP);   /* host */
    i = run_until_scene("PlayerSelectVs", 200);
    CHECK(i >= 0);
    CHECK(gGameState.unk10 == MENU_MODE_VS);
    CHECK((gGameState.unk18 & 0x30) == 0x20);
}

static void test_player_select(void) {
    int c1 = -1, c2 = -1, i, tasks;
    CHECK(run_until_psel(0, 0, 400) >= 0);
    run(20);
    dump("vs10_psel");
    CHECK(PlayerSelectMenu_VsState(0, &c1) == 0 && c1 == 0);   /* player 1 starts on Sonic */
    CHECK(PlayerSelectMenu_VsState(1, &c2) == 0 && c2 == 1);   /* player 2 on Blaze */
    /* both players' screens: player 1 on engines 0 / 1, player 2 on 2 / 3 */
    CHECK(engine_quads(0) > 5 && engine_quads(1) > 0 && engine_quads(2) > 5 && engine_quads(3) > 0);
    CHECK(g_mn_comp.nbr == 2 && g_mn_comp.n == 4);
    /* player 2 scrolls to Sonic; both press A in the same frame: the guest is sent back
     * (IsSameChoiceAsPartner) and moves off the host's character */
    tap2(IN_RIGHT);
    run(40);
    CHECK(PlayerSelectMenu_VsState(1, &c2) == 0 && c2 == 0);
    s_nvoice = 0;
    run2(2, IN_JUMP, IN_JUMP, 1);
    run(60);
    CHECK(PlayerSelectMenu_VsState(0, &c1) == 1 && c1 == 0);
    CHECK(PlayerSelectMenu_VsState(1, &c2) == 0 && c2 == 1);
    CHECK(s_nvoice >= 1 && s_voice[0] == 6);   /* SysSound_PlayMenuVoice(6): Sonic chosen */
    dump("vs11_psel_p1_confirmed");
    /* player 2 cannot reach Sonic while player 1 holds it: RIGHT scrolls through and back to Blaze */
    tap2(IN_RIGHT);
    run(80);
    CHECK(PlayerSelectMenu_VsState(1, &c2) == 0 && c2 == 1);
    /* player 1 cancels its choice (B: the partner has not decided) */
    tap1(IN_TRICK);
    run(10);
    CHECK(PlayerSelectMenu_VsState(0, &c1) == 0 && c1 == 0);
    CHECK(WirelessSession_GetSendBuffer(0)[1] == 0);
    /* player 1's B in the select: the quit prompt (MsgBox 9); NO (B) returns */
    tap1(IN_TRICK);
    run(30);
    CHECK(PlayerSelectMenu_VsState(0, NULL) == 3);
    dump("vs12_psel_quit_prompt");
    tap1(IN_TRICK);
    run(40);
    CHECK(PlayerSelectMenu_VsState(0, NULL) == 0);
    CHECK(PlayerSelectMenu_VsState(1, NULL) == 0);
    /* player 2's B does nothing in the select (the guest has no back button) */
    tap2(IN_TRICK);
    run(10);
    CHECK(PlayerSelectMenu_VsState(1, NULL) == 0);
    /* both decide: Blaze for player 2, then Sonic for player 1 */
    tap2(IN_JUMP);
    run(10);
    CHECK(PlayerSelectMenu_VsState(1, NULL) == 1);
    /* the guest cancels and decides again */
    tap2(IN_TRICK);
    run(10);
    CHECK(PlayerSelectMenu_VsState(1, NULL) == 0);
    tap2(IN_JUMP);
    run(10);
    tap1(IN_JUMP);
    run(40);
    /* both decided: no more cancel */
    tap1(IN_TRICK);
    tap2(IN_TRICK);
    run(5);
    CHECK(PlayerSelectMenu_VsState(0, NULL) == 1 && PlayerSelectMenu_VsState(1, NULL) == 1);
    dump("vs13_psel_both_confirmed");
    tasks = Task_Count();
    CHECK(tasks > 0);
    i = run_until_scene("TimeSelect", 400);
    CHECK(i > 150);   /* 0x10 frames, then the 0xd2 + 0x10 fade */
    CHECK(gGameState.unk4 == 0 && gGameState.unk8 == 1);
    CHECK(PlayerSelectMenu_VsState(0, NULL) == -1 && PlayerSelectMenu_VsState(1, NULL) == -1);
}

static MenuRequest test_time_select(void) {
    MenuRequest r;
    int i;
    CHECK(run_until_ts(2, 300) >= 0);
    run(10);
    dump("vs20_timeselect");
    CHECK(TimeSelectMenu_Selected() == 0);
    CHECK(count_res("act__mnu_conf_zsel", 0) >= 8);
    CHECK(mn_3d_count() == 2);   /* the two characters' models */
    CHECK(mn_3d_anim_frames(0, 0) > 0 && mn_3d_anim_frames(0, 0x37) > 0);
    CHECK(model_vertices() > 1000);
    printf("timeselect models: %d vertices, x %.0f..%.0f y %.0f..%.0f\n", s_nv, s_vx0, s_vx1, s_vy0, s_vy1);
    CHECK(s_vx0 > 32 && s_vx1 < 608 && s_vy0 > 0 && s_vy1 < 384);   /* the 2.0x top screen (feet at y 0x90) */
    /* save progress Sonic 5 (zones 0 and 1), Blaze 2 (zone 3): stages 0, 1, 2, 3, 6, 7 */
    tap1(IN_RIGHT);
    run(30);
    CHECK(TimeSelectMenu_Selected() == 1);
    tap1(IN_RIGHT);
    run(30);
    tap1(IN_RIGHT);
    run(30);
    CHECK(TimeSelectMenu_Selected() == 3);
    tap1(IN_RIGHT);   /* zone 2 is locked: 3 -> 6 */
    run(30);
    CHECK(TimeSelectMenu_Selected() == 6);
    dump("vs21_timeselect_zone3");
    tap1(IN_RIGHT);
    run(30);
    CHECK(TimeSelectMenu_Selected() == 7);
    tap1(IN_RIGHT);   /* wraps to 0 */
    run(30);
    CHECK(TimeSelectMenu_Selected() == 0);
    tap1(IN_LEFT);   /* 0 -> 7 */
    run(30);
    CHECK(TimeSelectMenu_Selected() == 7);
    tap1(IN_LEFT);
    run(30);
    CHECK(TimeSelectMenu_Selected() == 6);
    /* player 2 cannot choose */
    tap2(IN_RIGHT);
    run(30);
    CHECK(TimeSelectMenu_Selected() == 6);
    tap1(IN_JUMP);
    CHECK(run_until_ts(4, 120) >= 0);
    run(10);
    dump("vs22_timeselect_ready");
    CHECK(gGameState.unk1C == 3 && gGameState.unk1D == 0);
    run(20);
    dump("vs23_timeselect_ready_box");
    /* both ready */
    tap1(IN_JUMP);
    run(5);
    CHECK(TimeSelectMenu_StateId() == 4);
    r = tap2(IN_JUMP);
    for (i = 0; i < 60 && r.kind == MENU_REQ_NONE; i++)
        r = run(1);
    return r;
}

static void check_vs_request(MenuRequest r, int zone, int act) {
    CHECK(r.kind == MENU_REQ_VS_STAGE);
    CHECK(r.zone == zone && r.act == act);
    CHECK(r.character == 0 && r.character2 == 1);
    CHECK(r.mode == MENU_MODE_VS);
}

static MenuRequest results(int winner, const char *name) {
    VsRaceResult res;
    MenuRequest r;
    char nm[64];
    memset(&res, 0, sizeof(res));
    res.winner = winner;
    res.time[0] = 3000;
    res.time[1] = 3100;
    res.finished[0] = res.finished[1] = 1;
    r = menus_vs_exit(&res);
    CHECK(r.kind == MENU_REQ_NONE);
    CHECK(scene_is("Results"));
    run(0x20 + 8 + 4);
    run(0x40);
    snprintf(nm, sizeof(nm), "vs3%s_results", name);
    dump(nm);
    {
        int i;
        for (i = 0; i < 60 && ResultsScreen_Choice() < 0; i++)
            run(1);
    }
    CHECK(ResultsScreen_Choice() == 0);
    return r;
}

static void test_results(void) {
    MenuRequest r;
    int i;
    u8 tot0 = gSaveData.unk1D0[0];
    /* player 1 wins, rematch: the same stage and characters again */
    results(0, "0_win");
    CHECK(count_res("act__mnu_conf_result_eng", 0) == 2);   /* WIN / LOSE stamps */
    CHECK(mn_3d_count() == 2);
    CHECK(mn_3d_anim_frames(1, 2) > 0 && mn_3d_anim_frames(1, 7) > 0);   /* menu.narc win / lose poses */
    CHECK(model_vertices() > 1000);
    printf("results models: %d vertices, x %.0f..%.0f y %.0f..%.0f\n", s_nv, s_vx0, s_vx1, s_vy0, s_vy1);
    run(2);
    dump("vs31_results_choose");
    r = tap1(IN_JUMP);
    for (i = 0; i < 80 && r.kind == MENU_REQ_NONE; i++)
        r = run(1);
    check_vs_request(r, 3, 0);
    CHECK(gSaveData.unk1D0[0] == (u8)(tot0 + 1));   /* the VS record: one more win */
    CHECK(gSaveData.unk1D4[0].len == 8 && gSaveData.unk1D4[0].name[0] == 'P');   /* against player 2 */
    /* player 2 wins; player 2 cannot choose; player 1 picks the character select */
    results(1, "2_lose");
    tap2(IN_DOWN);
    run(3);
    CHECK(ResultsScreen_Choice() == 0);
    tap1(IN_DOWN);
    run(3);
    CHECK(ResultsScreen_Choice() == 1);
    r = tap1(IN_JUMP);
    i = run_until_scene("PlayerSelectVs", 120);
    CHECK(i >= 0);
    CHECK(run_until_psel(0, 0, 400) >= 0);
    /* player 1 quits the select: YES -> the title menu */
    tap1(IN_TRICK);
    run(30);
    CHECK(PlayerSelectMenu_VsState(0, NULL) == 3);
    tap1(IN_LEFT);   /* the cursor starts on NO */
    run(3);
    tap1(IN_JUMP);   /* YES */
    for (i = 0; i < 60 && WirelessSession_GetSendBuffer(0)[2] == 0; i++)
        run(1);
    CHECK(WirelessSession_GetSendBuffer(0)[2] == 1);   /* PlayerSelectMenu_SetLocalQuit */
    run(2);
    CHECK(PlayerSelectMenu_VsState(1, NULL) == 2);     /* the guest: PartnerLeft */
    dump("vs14_psel_quit");
    i = run_until_scene("TitleMenu", 200);
    CHECK(i >= 0);
    /* a draw; quit -> NO -> choose again -> quit -> YES -> title screen */
    gGameState.unk4 = 0;
    gGameState.unk8 = 1;
    results(2, "4_draw");
    tap1(IN_UP);   /* wraps to quit */
    run(3);
    CHECK(ResultsScreen_Choice() == 2);
    tap1(IN_JUMP);
    run(0x10 + 0xa + 20);
    dump("vs35_results_quit_prompt");
    tap1(IN_TRICK);   /* NO */
    for (i = 0; i < 80 && ResultsScreen_Choice() < 0; i++)
        run(1);
    CHECK(ResultsScreen_Choice() == 0);
    tap1(IN_UP);
    run(3);
    tap1(IN_JUMP);
    run(0x10 + 0xa + 20);
    tap1(IN_LEFT);
    run(3);
    r = tap1(IN_JUMP);   /* YES */
    i = run_until_scene("TitleScreen", 200);
    CHECK(i >= 0);
    CHECK(r.kind == MENU_REQ_NONE);
}

/* TimeSelect alone: B goes back to the two players' select; without player 2's A the race starts
 * after the 200-frame wait of TimeSelectMenu_State_ChooserReady */
static void test_time_select_back_and_timeout(void) {
    MenuRequest r;
    int i;
    gGameState.unk4 = 1;   /* the host plays Blaze: player 1's side shows Blaze */
    gGameState.unk8 = 0;
    Task_DestroyAll();
    mn_display_reset();
    TimeSelectMenu_StartMode0();
    CHECK(run_until_ts(2, 300) >= 0);
    tap1(IN_TRICK);
    i = run_until_scene("PlayerSelectVs", 60);
    CHECK(i >= 0);
    gGameState.unk4 = 1;
    gGameState.unk8 = 0;
    Task_DestroyAll();
    mn_display_reset();
    TimeSelectMenu_StartMode0();
    CHECK(run_until_ts(2, 300) >= 0);
    tap1(IN_RIGHT);
    run(30);
    tap1(IN_JUMP);
    CHECK(run_until_ts(4, 120) >= 0);
    tap1(IN_JUMP);
    for (i = 0, r.kind = MENU_REQ_NONE; i < 0xc8 + 0x20 && r.kind == MENU_REQ_NONE; i++)
        r = run(1);
    CHECK(i > 0xc8 - 10);
    CHECK(r.kind == MENU_REQ_VS_STAGE && r.zone == 0 && r.act == 1 && r.character == 1 && r.character2 == 0);
}

int main(int argc, char **argv) {
    MenuRequest r;
    if (argc < 3) {
        printf("usage: %s menus_dir sprites_dir [dumpdir]\n", argv[0]);
        return 2;
    }
    s_dump = argc > 3 ? argv[3] : NULL;
    g_save_backend.present = h_present;
    g_save_backend.read = h_read;
    g_save_backend.write = h_write;
    g_menu_hooks.voice = h_voice;
    g_menu_hooks.bgm = h_bgm;
    menus_set_paths(argv[1], argv[2]);
    snprintf(g_mn_models_dir, sizeof(g_mn_models_dir), "%s/../models", argv[1]);
    CHECK(menus_init() == 0);
    {
        static const uint16_t n1[4] = { 'R', 'U', 'S', 'H' }, n2[8] = { 'P', 'L', 'A', 'Y', 'E', 'R', ' ', '2' };
        menus_set_owner_name(n1, 4);
        menus_set_player2_name(n2, 8);
    }
    gSaveData.unkC = 5;    /* Sonic story: zones 0, 1 */
    gSaveData.unk64 = 2;   /* Blaze story: zone 3 */
    test_title_to_vs();
    test_player_select();
    r = test_time_select();
    check_vs_request(r, 3, 0);
    printf("VS request: kind %d zone %d act %d character %d/%d mode %d\n", (int)r.kind, r.zone, r.act, r.character,
           r.character2, r.mode);
    test_results();
    test_time_select_back_and_timeout();
    printf("vs host test: %d/%d checks passed\n", checks - fails, checks);
    return fails ? 1 : 0;
}
