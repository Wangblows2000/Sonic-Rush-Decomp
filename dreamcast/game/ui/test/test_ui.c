/*
 * Host test of the UI module: digit conversion, timer, tension gauge, score /
 * lives rules, ROM bonus tables and rank, title card / pause / results / game
 * over flows (frame counts), and the HUD quad layout.
 * usage: test_ui <ui_dir (uidata.bin)> <sprite_dir> <player_dir (trig.bin, physics.bin)>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../ui_internal.h"

void ui_debug_gauge(s16 *target, s16 *shown, u32 *digits_rings, u32 *digits_time);
void ui_post_update(DcPlayer *p);

static int s_fail, s_pass;
#define CHECK(c, ...)                                                    \
    do {                                                                 \
        if (c) {                                                         \
            s_pass++;                                                    \
        } else {                                                         \
            s_fail++;                                                    \
            printf("FAIL %s:%d: ", __FILE__, __LINE__);                  \
            printf(__VA_ARGS__);                                         \
            printf("\n");                                                \
        }                                                                \
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
        free(b);
        b = NULL;
    }
    fclose(f);
    return b;
}

static DcPlayer P;
static InputState IN;
static int s_sfx_n, s_music_last = -99;
static void t_sfx(void *c, s32 a, s32 i) {
    (void)c;
    (void)a;
    (void)i;
    s_sfx_n++;
}
static void t_music(void *c, s32 id) {
    (void)c;
    s_music_last = id;
}

/* one game frame as main.c runs it (no world update: the player is not stepped) */
static void frame(u32 pressed, u32 held) {
    IN.pressed = pressed;
    IN.held = held | pressed;
    ui_update(&IN, &P);
    ui_post_update(&P);
    ui_draw_tr();
}

/* UI_DUMP=dir: write the sorted quads of a frame (rendered by tools/render_quads.py) */
int ui_debug_sorted(const UiQuad ***out);
static void dump(const char *name) {
    const char *dir = getenv("UI_DUMP");
    const UiQuad **q;
    char path[300];
    FILE *f;
    int i, n;
    if (!dir)
        return;
    n = ui_debug_sorted(&q);
    snprintf(path, sizeof(path), "%s/%s.txt", dir, name);
    f = fopen(path, "w");
    if (!f)
        return;
    fprintf(f, "bright %d %d\n", g_ui_bright_main, g_ui_bright_sub);
    for (i = 0; i < n; i++)
        fprintf(f, "%s %d %.2f %.2f %.2f %.2f %.1f %.1f %.1f %.1f %08x %d\n",
                q[i]->tex >= 0 ? g_ui_res[q[i]->tex].id : "-", q[i]->image, q[i]->x, q[i]->y, q[i]->w, q[i]->h,
                q[i]->u0, q[i]->v0, q[i]->u1, q[i]->v1, (unsigned)q[i]->argb, q[i]->variant);
    fclose(f);
}

static void test_digits(void) {
    u32 d;
    Hud_ToDecimalDigits(&d, 123, 1000);
    CHECK(d == 0x123, "ToDecimalDigits(123, 1000) = %x (ones in nibble 0)", d);
    Hud_ToDecimalDigits(&d, 5000, 1000);
    CHECK(d == 0x999, "clamp 999: %x", d);
    Hud_ToDecimalDigits(&d, 7, 10);
    CHECK(d == 7, "single digit: %x", d);
    Hud_ToDecimalDigits(&d, 12345, 100000);
    CHECK(d == 0x12345, "five digits: %x", d);
    /* 1'23"45: 83 s and 27 frames (27 * 0x1b1 >> 8 = 45) */
    Hud_FramesToTimeDigits(&d, 83 * 60 + 27);
    CHECK(((d >> 16) & 0xf) == 1 && ((d >> 12) & 0xf) == 2 && ((d >> 8) & 0xf) == 3 &&
              ((d >> 4) & 0xf) == 4 && (d & 0xf) == 5,
          "time digits %x", d);
    Hud_FramesToTimeDigits(&d, 100000);
    CHECK((d & 0xfffff) == 0x95999, "time clamp 9'59\"99: %x", d);
}

static void test_tables(void) {
    CHECK(g_ui_data.ok, "uidata loaded");
    CHECK(g_ui_data.gauge_pal[0] == 0x7fff && g_ui_data.gauge_pal[1] == 0x7e00, "gauge colours");
    CHECK(g_ui_data.time_div[5] == 60 && g_ui_data.time_div[6] == 10, "time divisors");
    CHECK(g_ui_data.rank_norm[0] == 100000 && g_ui_data.rank_norm[3] == 0, "rank thresholds");
    CHECK(g_ui_data.lbl_norm[0] == 29 && g_ui_data.lbl_norm[4] == 34, "row labels");
    CHECK(g_ui_data.tc_text2[0] == 10 && g_ui_data.tc_text2[5] == 9, "Ready? string");
    CHECK(g_ui_res[UI_RES_MOJI].n_anims == 39, "ac_fix_moji anims %d", g_ui_res[UI_RES_MOJI].n_anims);
    CHECK(g_ui_res[UI_RES_CLDM].n_anims == 51, "dm_cldm anims %d", g_ui_res[UI_RES_CLDM].n_anims);
    CHECK(g_ui_res[UI_RES_STDM].n_anims == 43, "dm_stdm anims %d", g_ui_res[UI_RES_STDM].n_anims);
    CHECK(g_ui_res[UI_RES_PAUSE].n_anims == 20, "ac_fix_pause anims %d", g_ui_res[UI_RES_PAUSE].n_anims);
    CHECK(g_ui_res[UI_RES_GAMEOVER].n_anims == 23, "ac_fix_gameover anims");
}

static void test_bonus(void) {
    u32 tb, sb;
    W.zone = 0;
    W.act = 0;
    G.fail_flags = 0;
    W.frame = 3000; /* 50 s: first threshold 3599 */
    tb = ClearDemo_GetTimeBonus();
    CHECK(tb == g_ui_data.tb_norm[0] && tb == 85000, "time bonus %u", tb);
    W.frame = 20000; /* > 15599 (last real threshold), the 0xffffffff entry matches */
    tb = ClearDemo_GetTimeBonus();
    CHECK(tb == g_ui_data.tb_norm[9], "slow time bonus %u", tb);
    G.fail_flags = 4;
    CHECK(ClearDemo_GetTimeBonus() == 0, "time over: no time bonus");
    G.fail_flags = 0;
    G.score = 20000;
    sb = ClearDemo_GetScoreBonus();
    CHECK(sb == g_ui_data.sb_bonus[5] && sb == 10000, "trick bonus %u", sb);
    G.score = 0;
    CHECK(ClearDemo_GetScoreBonus() == 0, "trick bonus at 0: %u", ClearDemo_GetScoreBonus());
    CHECK(ClearDemo_GetRank(100000) == 0 && ClearDemo_GetRank(99999) == 1 && ClearDemo_GetRank(50000) == 2 &&
              ClearDemo_GetRank(0) == 3,
          "ranks");
    W.act = 2;
    CHECK(ClearDemo_GetRank(50000) == 0 && ClearDemo_GetRank(49999) == 1, "boss ranks");
    W.act = 0;
}

static void test_lives(void) {
    W.mode = 0;
    W.lives = 98;
    ui_add_lives(1);
    CHECK(W.lives == 99, "lives 99");
    ui_add_lives(1);
    CHECK(W.lives == 99, "lives clamp 99: %d", (int)W.lives);
    W.mode = 2;
    W.lives = 3;
    ui_add_lives(1);
    CHECK(W.lives == 3, "no lives in time attack");
    W.mode = 0;
}

static int run_until_timer(int max) {
    int f;
    for (f = 1; f <= max; f++) {
        frame(0, 0);
        if (W.flags & 1)
            return f;
    }
    return -1;
}

static void test_titlecard_and_hud(void) {
    int f, n, i, in_safe = 1, hud = 0;
    const UiQuad *q;
    s16 tgt, shown;
    u32 dr, dt;
    W.lives = 3;
    G.save_lives[0] = 3;
    ui_stage_start(0, 0, 0);
    CHECK(StageTitleCard_IsActive(), "title card created");
    CHECK(!(W.flags & 1), "timer stopped during the title card");
    CHECK(P.pf & 0x200000, "player input locked");
    for (f = 0; f < 120; f++)
        frame(0, 0);
    dump("titlecard_120");
    for (; f < 240; f++)
        frame(0, 0);
    dump("titlecard_240");
    f += run_until_timer(600);
    printf("title card: timer starts at frame %d (expected 283: 7 delay + 26 fade-in + text\n"
           "  sequence, then the exit waits for the 206-frame player zoom)\n", f);
    CHECK(f == 283, "StartStage frame %d", f);
    CHECK(!(P.pf & 0x200000), "player input unlocked");
    for (n = 0; n < 200 && StageTitleCard_IsActive(); n++)
        frame(0, 0);
    printf("title card: task ends %d frames after StartStage (25 restore + 31 final delay + 2)\n", n);
    CHECK(n > 50 && n < 70, "title card end %d", n);
    CHECK(g_ui_bright_main == 0 && g_ui_bright_sub == 0, "brightness restored");

    /* skip: A at frame 30 */
    ui_stage_start(0, 0, 0);
    for (f = 0; f < 30; f++)
        frame(0, 0);
    frame(IN_JUMP, 0);
    f = run_until_timer(10);
    CHECK(f == 1, "skip: timer starts %d frame(s) after A", f);

    /* timer runs 1 per frame; HUD digits follow */
    {
        u32 t0 = W.frame;
        for (i = 0; i < 120; i++)
            frame(0, 0);
        CHECK(W.frame == t0 + 120, "timer +120: %u", W.frame - t0);
    }
    P.rings = 57;
    frame(0, 0);
    ui_debug_gauge(&tgt, &shown, &dr, &dt);
    CHECK((dr & 0xfff) == 0x057, "ring digits %x", dr);
    /* tension gauge: empty it (mode 0 drain: 1 per frame), then target 150
     * (tension 0x960 >> 4): shown climbs 2 per frame */
    P.tension = 0;
    for (i = 0; i < 310; i++)
        frame(0, 0);
    ui_debug_gauge(&tgt, &shown, &dr, &dt);
    CHECK(tgt == 0 && shown == 0, "gauge empty %d/%d", shown, tgt);
    P.tension = 0x960;
    frame(0, 0);
    ui_debug_gauge(&tgt, &shown, &dr, &dt);
    CHECK(tgt == 150 && shown == 2, "gauge first step %d/%d", shown, tgt);
    for (i = 0; i < 80; i++)
        frame(0, 0);
    ui_debug_gauge(&tgt, &shown, &dr, &dt);
    CHECK(shown == 150, "gauge filled %d", shown);
    /* drain with mode 1 (boost): shown drops 1 per frame */
    P.tension = 0x640;
    g_dcp_hooks.tension_hud(g_dcp_hooks.ctx, &P, 100);
    frame(0, 0);
    ui_debug_gauge(&tgt, &shown, &dr, &dt);
    CHECK(tgt == 100 && shown == 149, "gauge drain %d/%d", shown, tgt);

    P.rings = 57;
    for (i = 0; i < 10; i++)
        frame(0, 0);
    dump("hud");
    /* HUD quads: inside the 4:3 safe area */
    n = ui_debug_quads(&q);
    for (i = 0; i < n; i++)
        if (q[i].layer == UI_LAYER_HUD) {
            hud++;
            if (q[i].x < 32 || q[i].y < 24 || q[i].x + q[i].w > 608 || q[i].y + q[i].h > 456)
                in_safe = 0;
        }
    printf("HUD: %d quads\n", hud);
    CHECK(hud >= 25, "HUD quads %d", hud);
    CHECK(in_safe, "HUD inside the safe area");
}

static void test_score_popup(void) {
    int f;
    u32 s0 = G.score;
    g_dcp_hooks.score(g_dcp_hooks.ctx, &P, 100);
    g_dcp_hooks.score(g_dcp_hooks.ctx, &P, 400);
    CHECK(G.score == s0 + 500, "score +500");
    for (f = 0; f < 12; f++)
        frame(0, 0);
    dump("popup");
    CHECK(g_dcp_hooks.score_active(g_dcp_hooks.ctx), "popup active");
    for (f = 0; f < 200 && ScorePopup_IsActive(); f++)
        frame(0, 0);
    printf("score popup: %d frames\n", f);
    CHECK(!ScorePopup_IsActive() && f > 40 && f < 120, "popup lifetime %d", f);
}

static void test_pause(void) {
    int f;
    u32 t;
    G.stage_new = 0;
    frame(0, 0);
    frame(IN_START, 0);
    CHECK(ui_is_paused(), "START pauses");
    t = W.frame;
    for (f = 0; f < 30; f++)
        frame(0, 0);
    frame(IN_DOWN, 0);
    dump("pause");
    frame(IN_UP, 0);
    CHECK(W.frame == t, "timer frozen while paused");
    frame(IN_START, 0);
    CHECK(!ui_is_paused(), "START resumes");
    frame(0, 0);
    frame(IN_START, 0);
    frame(IN_DOWN, 0);
    frame(IN_JUMP, 0); /* GO BACK */
    for (f = 0; f < 40 && ui_state() != UI_EXIT; f++)
        frame(0, 0);
    printf("pause quit: exit after %d frames (fade 0x100/frame)\n", f);
    CHECK(ui_state() == UI_EXIT && ui_exit_kind() == UI_EXIT_QUIT, "quit to map");
}

static void test_results(void) {
    int f;
    ui_stage_start(0, 0, 0);
    run_until_timer(600);
    for (f = 0; f < 2400; f++) /* 40 s */
        frame(0, 0);
    G.score = 20000;
    g_obj_stage.ring_bonus = 57 * 10;
    g_obj_stage.speed_bonus = 4000;
    W.flags &= ~1u; /* the goal stops the timer */
    ui_stage_exit(OBJ_EXIT_CLEAR_DEMO);
    CHECK(ui_state() == UI_RESULTS, "results");
    s_music_last = -99;
    for (f = 0; f < 3000 && ui_state() == UI_RESULTS; f++) {
        frame(0, 0);
        if (f == 690)
            dump("results");
    }
    printf("results: total %u, %d frames until exit\n", G.total, f);
    CHECK(G.total == 85000u + 10000u + 570u + 4000u, "tally total %u", G.total);
    CHECK(ui_state() == UI_EXIT && ui_exit_kind() == UI_EXIT_NEXT_ACT, "next act");
    CHECK(f > 500 && f < 900, "results length %d", f);

    /* skip with A twice */
    ui_stage_start(0, 0, 0);
    run_until_timer(600);
    ui_stage_exit(OBJ_EXIT_CLEAR_DEMO);
    for (f = 0; f < 20; f++)
        frame(0, 0);
    frame(IN_JUMP, 0);
    frame(0, 0);
    frame(IN_JUMP, 0);
    for (f = 0; f < 3000 && ui_state() == UI_RESULTS; f++)
        frame(0, 0);
    printf("results skipped: exit after %d more frames\n", f);
    CHECK(f < 260, "skip length %d", f);
}

static void test_gameover(void) {
    int f;
    ui_stage_start(0, 0, 0);
    run_until_timer(600);
    W.lives = 0;
    P.pf |= DCP_PF_GAMEOVER; /* Player_Respawn with no lives left */
    frame(0, 0);
    frame(0, 0);
    CHECK(ui_state() == UI_GAMEOVER, "game over sequence (%d)", ui_state());
    CHECK(G.save_lives[0] == 2, "saved lives clamped to 2: %d", G.save_lives[0]);
    for (f = 0; f < 800 && ui_state() == UI_GAMEOVER; f++) {
        frame(0, 0);
        if (f == 300)
            dump("gameover");
    }
    printf("game over: %d frames\n", f);
    CHECK(ui_exit_kind() == UI_EXIT_GAME_OVER && f > 512 && f < 540, "game over length %d", f);
    P.pf &= ~DCP_PF_GAMEOVER;

    /* time over: 9'59"99 kills the player, then the TIME'S UP sequence */
    ui_stage_start(0, 0, 0);
    run_until_timer(600);
    W.lives = 2;
    W.frame = 0x8c9f - 3;
    for (f = 0; f < 5; f++)
        frame(0, 0);
    CHECK(P.pf & DCP_PF_DEAD, "time over kills");
    CHECK(G.fail_flags & 4, "time over flag");
    P.pf &= ~DCP_PF_DEAD;
}

int main(int argc, char **argv) {
    long ts, ps;
    char path[300];
    s16 *trig;
    u8 *phys;
    if (argc < 4) {
        fprintf(stderr, "usage: %s ui_dir sprite_dir player_dir\n", argv[0]);
        return 2;
    }
    snprintf(path, sizeof(path), "%s/trig.bin", argv[3]);
    trig = load(path, &ts);
    snprintf(path, sizeof(path), "%s/physics.bin", argv[3]);
    phys = load(path, &ps);
    if (!trig || !phys) {
        fprintf(stderr, "player tables missing\n");
        return 2;
    }
    dcp_set_trig(trig, trig + 8192);
    dcp_set_physics(phys, (u32)ps);
    dcp_player_create(&P, 0, 0, 1000, 500);
    ui_bind_player(&P);
    ui_set_paths(argv[1], argv[2]);
    CHECK(ui_init() == 0, "ui_init");
    g_ui_hooks.sfx = t_sfx;
    g_ui_hooks.music = t_music;
    test_digits();
    test_tables();
    test_bonus();
    test_lives();
    test_titlecard_and_hud();
    test_score_popup();
    test_pause();
    test_results();
    test_gameover();
    printf("%d passed, %d failed\n", s_pass, s_fail);
    return s_fail ? 1 : 0;
}
