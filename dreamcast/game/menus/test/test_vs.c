/* Host test of Battle Play (local two-controller VS) in the front end: title menu VS page ->
 * both players' character select (mn_psel.c modes 1 / 2) -> TimeSelect stage select
 * (mn_timeselect.c) -> MENU_REQ_VS_STAGE; menus_vs_exit -> ResultsScreen (mn_results.c) ->
 * rematch / character select / quit; the Vs CPU mode (ONE PLAYER) with player 2's pad ignored. Two pads: player 1 through menus_run_frame, player 2
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

/* the VS page's buttons (engine 1, mnu_titl_eng): two 128-px buttons drawn as pieces of several images
 * (the spelled labels TWO PLAYERS / ONE PLAYER), each with its frame from item 0xd's image */
static int check_vs_buttons(const char *tag) {
    const MnQuad *q;
    int n = mn_quads(&q), k, b, ok = 1, ys[2] = { -1, -1 }, np[2] = { 0, 0 }, nimg[2] = { 0, 0 };
    int x0[2] = { 1 << 20, 1 << 20 }, x1[2] = { -(1 << 20), -(1 << 20) }, img[2][8];
    for (k = 0; k < n; k++) {
        if (!q[k].res || strcmp(q[k].res->id, "act__mnu_titl_eng") != 0 || q[k].engine != 1 || q[k].h != 24)
            continue;
        for (b = 0; b < 2 && ys[b] >= 0 && ys[b] != (int)q[k].y; b++)
            ;
        if (b == 2)
            continue;
        ys[b] = (int)q[k].y;
        np[b]++;
        x0[b] = (int)q[k].x < x0[b] ? (int)q[k].x : x0[b];
        x1[b] = (int)(q[k].x + q[k].w) > x1[b] ? (int)(q[k].x + q[k].w) : x1[b];
        {
            int j;
            for (j = 0; j < nimg[b] && img[b][j] != q[k].image; j++)
                ;
            if (j == nimg[b] && nimg[b] < 8)
                img[b][nimg[b]++] = q[k].image;
        }
    }
    for (b = 0; b < 2; b++) {
        printf("%s button %d: %d pieces from %d images, x %d..%d\n", tag, b, np[b], nimg[b], x0[b], x1[b]);
        ok &= np[b] >= 12 && nimg[b] >= 3 && x1[b] - x0[b] == 128;
    }
    return ok;
}
/* the "CPU" tag of player 2's select column: the font8 strip on engine 2 */
static int cpu_tag_quads(void) {
    const MnQuad *q;
    int n = mn_quads(&q), i, k = 0;
    for (i = 0; i < n; i++)
        if (q[i].kind == MNQ_BG && q[i].engine == MN_ENGINE_P2 && q[i].bg && strcmp(q[i].bg->name, "vs_names") == 0)
            k++;
    return k;
}

/* title menu -> VS page -> TWO PLAYERS: Battle Play's character select */
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
    /* two buttons, TWO PLAYERS and ONE PLAYER, spelled from letters of several English labels */
    CHECK(check_vs_buttons("vs page"));
    tap1(IN_DOWN);   /* ONE PLAYER */
    run(10);
    tap1(IN_UP);     /* TWO PLAYERS */
    run(10);
    menus_vs_set_cpu(1);   /* TWO PLAYERS resets the mode */
    tap1(IN_JUMP);   /* straight to the select */
    i = run_until_scene("PlayerSelectVs", 200);
    CHECK(i >= 0);
    CHECK(gGameState.unk10 == MENU_MODE_VS);
    CHECK((gGameState.unk18 & 0x30) == 0x20);
    CHECK(mn_vs_cpu() == 0);
    CHECK(cpu_tag_quads() == 0);
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
    CHECK(count_res("act2__caution__caution_a_eng", 0) == 3);   /* "QUIT" "BATTLE" "?" */
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
    CHECK(r.cpu == 0);
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

/* a race started without the selects (main.c's debug start): menus_vs_prepare, then the results */
static MenuRequest wait_req(MenuRequest r, int n) {
    int i;
    for (i = 0; i < n && r.kind == MENU_REQ_NONE; i++)
        r = run(1);
    return r;
}
static void test_prepare(void) {
    MenuRequest r;
    const u16 *nm;
    int i, c1, c2;
    /* unnamed players: "PLAYER 1" / "PLAYER 2" */
    menus_set_owner_name(NULL, 0);
    menus_set_player2_name(NULL, 0);
    CHECK(mn_vs_player_name(0, &nm) == -8 && nm[7] == '1');
    CHECK(mn_vs_player_name(1, &nm) == -8 && nm[7] == '2');
    Task_DestroyAll();
    mn_display_reset();
    gGameState.unk4 = gGameState.unk8 = 0;   /* as after a story game */
    menus_vs_prepare(2, 1, 1, 1);           /* equal characters: player 2 gets the other */
    CHECK(gGameState.unk4 == 1 && gGameState.unk8 == 0 && gGameState.unk10 == MENU_MODE_VS);
    results(1, "6_prepared_p2_wins");
    run(2);
    dump("vs37_results_unnamed");
    r = tap1(IN_JUMP);   /* TRY AGAIN */
    r = wait_req(r, 80);
    CHECK(r.kind == MENU_REQ_VS_STAGE && r.zone == 2 && r.act == 1 && r.character == 1 && r.character2 == 0);
    /* again: win, TRY AGAIN; draw, TRY AGAIN */
    results(0, "7_again_win");
    r = wait_req(tap1(IN_JUMP), 80);
    CHECK(r.kind == MENU_REQ_VS_STAGE && r.zone == 2 && r.act == 1 && r.character == 1 && r.character2 == 0);
    results(2, "8_again_draw");
    r = wait_req(tap1(IN_JUMP), 80);
    CHECK(r.kind == MENU_REQ_VS_STAGE && r.zone == 2 && r.act == 1 && r.character == 1 && r.character2 == 0);
    /* CHANGE: new picks (player 1 Sonic, player 2 Blaze), stage zone 0 act 1 */
    results(0, "9_change");
    tap1(IN_DOWN);
    run(3);
    tap1(IN_JUMP);
    CHECK(run_until_scene("PlayerSelectVs", 120) >= 0);
    CHECK(run_until_psel(0, 0, 400) >= 0);
    PlayerSelectMenu_VsState(0, &c1);
    PlayerSelectMenu_VsState(1, &c2);
    CHECK(c1 == 0 && c2 == 1);
    run2(2, IN_JUMP, IN_JUMP, 1);
    CHECK(run_until_scene("TimeSelect", 400) >= 0);
    CHECK(gGameState.unk4 == 0 && gGameState.unk8 == 1);
    CHECK(run_until_ts(2, 300) >= 0);
    tap1(IN_RIGHT);
    run(30);
    tap1(IN_JUMP);
    CHECK(run_until_ts(4, 120) >= 0);
    run2(2, IN_JUMP, IN_JUMP, 1);
    r = wait_req(run(1), 60);
    CHECK(r.kind == MENU_REQ_VS_STAGE && r.zone == 0 && r.act == 1 && r.character == 0 && r.character2 == 1);
    /* the results draw the new characters: Sonic left, Blaze right; TRY AGAIN keeps them */
    results(1, "a_after_change");
    r = wait_req(tap1(IN_JUMP), 80);
    CHECK(r.kind == MENU_REQ_VS_STAGE && r.zone == 0 && r.act == 1 && r.character == 0 && r.character2 == 1);
    /* QUIT -> YES -> title screen */
    results(0, "b_quit");
    tap1(IN_UP);
    run(3);
    tap1(IN_JUMP);
    run(0x10 + 0xa + 20);
    tap1(IN_LEFT);
    run(3);
    r = tap1(IN_JUMP);
    i = run_until_scene("TitleScreen", 200);
    CHECK(i >= 0 && r.kind == MENU_REQ_NONE);
}

/* Vs CPU: title menu ONE PLAYER -> player 1 picks, the CPU takes the other character by itself ->
 * player 1 alone picks the stage, the CPU is ready at once -> MENU_REQ_VS_STAGE with cpu = 1 -> results
 * (player 1 only) -> TRY AGAIN keeps cpu = 1 -> CHANGE CHARACTER/ACT keeps the mode -> QUIT. Player 2's
 * pad holds buttons all the time: it must change nothing. */
static void test_cpu_flow(void) {
    MenuRequest r;
    const u16 *nm;
    int i, c1 = -1, c2 = -1, t0;
    u8 tot1 = gSaveData.unk1D0[1];
    Task_DestroyAll();
    mn_display_reset();
    BRIGHT_MAIN = BRIGHT_SUB = 0;
    menus_vs_set_cpu(0);
    TitleMenu_Start();
    run(90);
    tap1(IN_DOWN);   /* GAME START -> VS */
    run(30);
    tap1(IN_JUMP);   /* VS page */
    run(90);
    tap1(IN_DOWN);   /* ONE PLAYER */
    run(10);
    CHECK(check_vs_buttons("vs cpu page"));
    dump("vs40_title_vs_cpu_item");
    tap1(IN_JUMP);
    i = run_until_scene("PlayerSelectVs", 200);
    CHECK(i >= 0 && mn_vs_cpu() == 1);
    CHECK(gGameState.unk10 == MENU_MODE_VS && (gGameState.unk18 & 0x30) == 0x20);
    CHECK(mn_vs_player_name(1, &nm) == -3 && nm[0] == 'C' && nm[1] == 'P' && nm[2] == 'U');
    /* player 2's pad is not read: RIGHT / A on it do nothing */
    for (i = 0; i < 400 && !(PlayerSelectMenu_VsState(0, NULL) == 0 && PlayerSelectMenu_VsState(1, NULL) == 0); i++)
        run2(1, 0, IN_JUMP | IN_RIGHT, 0);
    CHECK(i < 400);
    run2(40, 0, IN_RIGHT, 1);
    run2(40, 0, IN_JUMP, 1);
    CHECK(PlayerSelectMenu_VsState(1, &c2) == 0 && c2 == 1);   /* the CPU waits on Blaze */
    CHECK(cpu_tag_quads() == 1);
    dump("vs41_cpu_psel");
    /* player 1 takes Blaze: the CPU moves off it to Sonic and decides by itself */
    tap1(IN_RIGHT);
    run(40);
    CHECK(PlayerSelectMenu_VsState(0, &c1) == 0 && c1 == 1);
    tap1(IN_JUMP);
    run(4);
    CHECK(PlayerSelectMenu_VsState(0, NULL) == 1);
    for (i = 0; i < 120 && PlayerSelectMenu_VsState(1, NULL) != 1; i++)
        run2(1, 0, IN_TRICK, 0);
    CHECK(i < 120 && PlayerSelectMenu_VsState(1, &c2) == 1 && c2 == 0);
    dump("vs42_cpu_psel_both");
    i = run_until_scene("TimeSelect", 400);
    CHECK(i > 150);
    CHECK(gGameState.unk4 == 1 && gGameState.unk8 == 0);
    /* the stage select: player 1 alone; player 2's plate says CPU; the CPU is ready at once */
    CHECK(run_until_ts(2, 300) >= 0);
    run(10);
    dump("vs43_cpu_timeselect");
    tap1(IN_RIGHT);
    run(30);
    CHECK(TimeSelectMenu_Selected() == 1);
    run2(30, 0, IN_RIGHT, 1);
    CHECK(TimeSelectMenu_Selected() == 1);
    tap1(IN_JUMP);
    CHECK(run_until_ts(4, 120) >= 0);
    t0 = 0;
    r = tap1(IN_JUMP);
    for (; t0 < 60 && r.kind == MENU_REQ_NONE; t0++)
        r = run(1);
    CHECK(t0 < 40);   /* no 200-frame wait for player 2 */
    CHECK(r.kind == MENU_REQ_VS_STAGE && r.zone == 0 && r.act == 1 && r.character == 1 && r.character2 == 0);
    CHECK(r.cpu == 1 && r.mode == MENU_MODE_VS);
    printf("Vs CPU request: kind %d zone %d act %d character %d/%d cpu %d\n", (int)r.kind, r.zone, r.act,
           r.character, r.character2, r.cpu);
    /* results: the CPU won; player 2's pad cannot choose; TRY AGAIN keeps the mode */
    results(1, "c_cpu_wins");
    CHECK(count_res("act__mnu_conf_result_eng", 0) == 2);
    run2(3, 0, IN_DOWN, 1);
    CHECK(ResultsScreen_Choice() == 0);
    r = wait_req(tap1(IN_JUMP), 80);
    CHECK(r.kind == MENU_REQ_VS_STAGE && r.cpu == 1 && r.zone == 0 && r.act == 1 && r.character == 1 &&
          r.character2 == 0);
    /* the VS record: a loss against "CPU" (its own opponent entry, MAC ff:..:ff) */
    CHECK(gSaveData.unk1D0[1] == (u8)(tot1 + 1));
    CHECK(gSaveData.unk1D4[0].len == 3 && gSaveData.unk1D4[0].name[0] == 'C' && gSaveData.unk1D4[0].name[2] == 'U');
    CHECK(gSaveData.unk1D4[0].mac[0] == 0xff && gSaveData.unk1D4[0].cnt[1] >= 1);
    /* CHANGE CHARACTER/ACT: back to the select in Vs CPU */
    results(0, "d_cpu_change");
    tap1(IN_DOWN);
    run(3);
    tap1(IN_JUMP);
    CHECK(run_until_scene("PlayerSelectVs", 120) >= 0);
    CHECK(mn_vs_cpu() == 1);
    CHECK(run_until_psel(0, 0, 400) >= 0);
    CHECK(cpu_tag_quads() == 1);
    /* player 1 decides Sonic: the CPU keeps Blaze and decides 0x20 frames later; player 1's B in
     * between takes the choice back, the CPU waits again */
    tap1(IN_JUMP);
    run(8);
    CHECK(PlayerSelectMenu_VsState(0, NULL) == 1 && PlayerSelectMenu_VsState(1, NULL) == 0);
    tap1(IN_TRICK);
    run(60);
    CHECK(PlayerSelectMenu_VsState(0, NULL) == 0 && PlayerSelectMenu_VsState(1, NULL) == 0);
    tap1(IN_JUMP);
    run(0x30);
    CHECK(PlayerSelectMenu_VsState(0, &c1) == 1 && c1 == 0);
    CHECK(PlayerSelectMenu_VsState(1, &c2) == 1 && c2 == 1);
    CHECK(run_until_scene("TimeSelect", 400) >= 0);
    CHECK(run_until_ts(2, 300) >= 0);
    tap1(IN_JUMP);
    CHECK(run_until_ts(4, 120) >= 0);
    r = wait_req(tap1(IN_JUMP), 60);
    CHECK(r.kind == MENU_REQ_VS_STAGE && r.cpu == 1 && r.zone == 0 && r.act == 0 && r.character == 0 &&
          r.character2 == 1);
    /* QUIT -> YES -> the title screen */
    results(2, "e_cpu_quit");
    tap1(IN_UP);
    run(3);
    tap1(IN_JUMP);
    run(0x10 + 0xa + 20);
    tap1(IN_LEFT);
    run(3);
    r = tap1(IN_JUMP);
    CHECK(run_until_scene("TitleScreen", 200) >= 0 && r.kind == MENU_REQ_NONE);
}

/* every language: the VS page's two buttons are the English spelled labels (mnu_titl_eng.bac loaded
 * beside the language's own bank); ONE PLAYER still starts Vs CPU */
static void test_vs_page_languages(void) {
    int lang;
    char tag[32];
    for (lang = 0; lang < 6; lang++) {
        menus_set_language(lang);
        Task_DestroyAll();
        mn_display_reset();
        BRIGHT_MAIN = BRIGHT_SUB = 0;
        menus_vs_set_cpu(0);
        TitleMenu_Start();
        run(90);
        tap1(IN_DOWN);
        run(30);
        tap1(IN_JUMP);
        run(90);
        tap1(IN_DOWN);
        run(10);
        snprintf(tag, sizeof(tag), "lang %d", lang);
        CHECK(check_vs_buttons(tag));
        snprintf(tag, sizeof(tag), "vs45_vs_page_lang%d", lang);
        dump(tag);
        tap1(IN_JUMP);
        CHECK(run_until_scene("PlayerSelectVs", 200) >= 0 && mn_vs_cpu() == 1);
    }
    menus_set_language(1);
    Task_DestroyAll();
    mn_display_reset();
}

/* main.c's debug start in Vs CPU: menus_vs_set_cpu(1) + menus_vs_prepare; TRY AGAIN keeps cpu = 1,
 * menus_vs_set_cpu(0) gives the two-player mode again */
static void test_cpu_prepare(void) {
    MenuRequest r;
    Task_DestroyAll();
    mn_display_reset();
    menus_vs_set_cpu(1);
    menus_vs_prepare(1, 0, 0, 0);
    CHECK(mn_vs_cpu() == 1 && gGameState.unk4 == 0 && gGameState.unk8 == 1);
    results(0, "f_cpu_prepared");
    dump("vs44_cpu_results");
    r = wait_req(tap1(IN_JUMP), 80);
    CHECK(r.kind == MENU_REQ_VS_STAGE && r.cpu == 1 && r.zone == 1 && r.act == 0 && r.character == 0 &&
          r.character2 == 1);
    menus_vs_set_cpu(0);
    results(0, "g_two_players_again");
    r = wait_req(tap1(IN_JUMP), 80);
    CHECK(r.kind == MENU_REQ_VS_STAGE && r.cpu == 0 && r.zone == 1 && r.act == 0);
}

/* player 2's column of the select: its portrait (sub BG0 of engine 3) and banner (sub BG2, the
 * "please select" sprite) are drawn, with player 2 idle and with a scripted pad (RIGHT on frames
 * 0-3 of every 60, A on 30-33) */
static int bg_quads(int engine, int layer) {
    const MnDraw *d;
    int n = mn_sorted(&d), i, k = 0;
    for (i = 0; i < n; i++)
        if (d[i].q->kind == MNQ_BG && d[i].q->engine == engine && d[i].q->bg == g_mn_layer[engine][layer].img &&
            g_mn_layer[engine][layer].visible)
            k++;
    return k;
}
static void check_p2_column(const char *tag) {
    char nm[64];
    CHECK(bg_quads(3, 0) > 0);                          /* the portrait */
    CHECK(bg_quads(3, 2) > 0);                          /* the name banner */
    CHECK(count_res("act__mnu_psel", 3) > 0);           /* "PLEASE SELECT A CHARACTER" */
    CHECK(engine_quads(2) > 5);
    if (bg_quads(3, 0) == 0 || count_res("act__mnu_psel", 3) == 0)
        printf("p2 column (%s): portrait %d banner %d sprites e3 %d\n", tag, bg_quads(3, 0), bg_quads(3, 2),
               engine_quads(3));
    snprintf(nm, sizeof(nm), "vs15_p2_%s", tag);
    dump(nm);
}
static void test_p2_column(void) {
    int f, t, shown, c0, c1;
    for (t = 0; t < 2; t++) {
        Task_DestroyAll();
        mn_display_reset();
        mn_vs_reset_records();
        BRIGHT_MAIN = BRIGHT_SUB = 16;
        PlayerSelectMenu_StartVs();
        shown = 0;
        c0 = c1 = 0;
        for (f = 0; f < 330; f++) {
            uint32_t p2 = 0;
            if (t == 1 && f % 60 < 4)
                p2 = IN_RIGHT;
            if (t == 1 && f % 60 >= 30 && f % 60 < 34)
                p2 = IN_JUMP;
            run2(1, 0, p2, 0);
            if (f >= 240 && f < 300) {   /* one period: the portrait is off only during the scroll */
                int ch = -1;
                shown += bg_quads(3, 0) > 0 && bg_quads(3, 2) > 0 && count_res("act__mnu_psel_eng", 2) > 0;
                PlayerSelectMenu_VsState(1, &ch);
                c0 += ch == 0;
                c1 += ch == 1;
            }
            if (t == 1 && getenv("VS_P2_PHASES") && f >= 240 && f < 300 && f % 4 == 0) {
                char tag[32];
                snprintf(tag, sizeof(tag), "phase_%d", f);
                dump(tag);
            }
            if (f == 60 || f == 180 || f == 300) {
                char tag[32];
                snprintf(tag, sizeof(tag), "%s_%d", t ? "script" : "idle", f);
                check_p2_column(tag);
            }
        }
        printf("p2 column %s: portrait + banner + name on %d of 60 frames (Sonic %d, Blaze %d)\n",
               t ? "scripted" : "idle", shown, c0, c1);
        CHECK(shown >= (t ? 25 : 60));
        CHECK(t ? (c0 > 0 && c1 > 0) : c1 == 60);   /* scripted: player 2 alternates Sonic / Blaze */
    }
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
    test_p2_column();
    test_title_to_vs();
    test_player_select();
    r = test_time_select();
    check_vs_request(r, 3, 0);
    printf("VS request: kind %d zone %d act %d character %d/%d mode %d\n", (int)r.kind, r.zone, r.act, r.character,
           r.character2, r.mode);
    test_results();
    test_time_select_back_and_timeout();
    test_prepare();
    test_cpu_flow();
    test_cpu_prepare();
    test_vs_page_languages();
    printf("vs host test: %d/%d checks passed\n", checks - fails, checks);
    return fails ? 1 : 0;
}
