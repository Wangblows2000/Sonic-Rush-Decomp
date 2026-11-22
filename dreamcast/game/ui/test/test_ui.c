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
#include "../../save/save.h"

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
static int s_voice_n, s_voice_last = -1, s_sfx_last = -1, s_stage_new_calls;
static void t_voice(void *c, s32 a, s32 i) {
    (void)c;
    (void)a;
    s_voice_n++;
    s_voice_last = i;
}
static void t_sfx2(void *c, s32 a, s32 i) {
    (void)c;
    (void)a;
    s_sfx_n++;
    s_sfx_last = i;
}
static int t_stage_new(void *c) {
    (void)c;
    s_stage_new_calls++;
    return 1;
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
int ui_debug_palmap(int n, int *res, int *src, int *dst);
static const char *tex_id(int tex) {
    if (tex == UI_TEX_FACE)
        return G.character == 1 ? "navifaces_c" : "navifaces_t";
    return tex >= 0 && tex < UI_RES_COUNT ? g_ui_res[tex].id : "-";
}
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
    /* the frame overlay is the main screen's brightness only (sub-screen quads carry
     * theirs in argb) */
    fprintf(f, "bright %d 0\n", g_ui_bright_main);
    for (i = 0; i < 4; i++) {
        int r, a, b;
        if (ui_debug_palmap(i, &r, &a, &b))
            fprintf(f, "palmap %d %s %d %d\n", UI_VAR_MAP0 + i, g_ui_res[r].id, a, b);
    }
    for (i = 0; i < n; i++)
        fprintf(f, "%s %d %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %08x %d\n", tex_id(q[i]->tex), q[i]->image,
                q[i]->x, q[i]->y, q[i]->w, q[i]->h, q[i]->u0, q[i]->v0, q[i]->u1, q[i]->v1, (unsigned)q[i]->argb,
                q[i]->variant);
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

static int layer_box(int layer, int screen, float *x0, float *y0, float *x1, float *y1);
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
        if (f == 690) {
            float x0, y0, x1, y1;
            int nb = layer_box(UI_LAYER_RESULTS_BG, -1, &x0, &y0, &x1, &y1);
            CHECK(nb == 4 && x1 - x0 == 640, "results BG: backdrop, cldm_far, 2 cldm_bg planes (%d)", nb);
            dump("results");
        }
        if (f == 12)
            dump("results_open");
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

/* quads of one layer: count and bounding box */
static int layer_box(int layer, int screen, float *x0, float *y0, float *x1, float *y1) {
    const UiQuad *q;
    int n = ui_debug_quads(&q), i, c = 0;
    *x0 = *y0 = 1e9f;
    *x1 = *y1 = -1e9f;
    for (i = 0; i < n; i++) {
        if (q[i].layer != layer || (screen >= 0 && q[i].screen != screen))
            continue;
        c++;
        if (q[i].x < *x0)
            *x0 = q[i].x;
        if (q[i].y < *y0)
            *y0 = q[i].y;
        if (q[i].x + q[i].w > *x1)
            *x1 = q[i].x + q[i].w;
        if (q[i].y + q[i].h > *y1)
            *y1 = q[i].y + q[i].h;
    }
    return c;
}

/* every quad of the gameplay overlays (HUD + navigator layers) inside the safe area,
 * and no quad as large as a DS screen (a second-screen panel) */
static void check_overlays(const char *what) {
    const UiQuad *q;
    int n = ui_debug_quads(&q), i, safe = 1, big = 0;
    for (i = 0; i < n; i++) {
        if (q[i].layer != UI_LAYER_HUD && q[i].layer != UI_LAYER_NAVI)
            continue;
        if (q[i].x < 31.5f || q[i].y < 23.5f || q[i].x + q[i].w > 608.5f || q[i].y + q[i].h > 456.5f)
            safe = 0;
        if (q[i].w * q[i].h > 256.0f * 192.0f)
            big++;
    }
    CHECK(safe, "%s: overlays inside the safe area", what);
    CHECK(big == 0, "%s: no DS-screen-sized panel (%d)", what, big);
}

static void test_boss_navi(void) {
    int f, n, msg_frames = 0, lost_seen = 0, flash = 0, v0;
    float x0, y0, x1, y1;
    const UiQuad *q;
    G.character = 0;
    G.difficulty = 1;
    ui_set_boss_health(8);
    ui_stage_start(0, 2, 0);   /* Leaf Storm boss (act 3) */
    CHECK(StageTitleCard_IsActive(), "boss title card");
    for (f = 0; f < 60; f++)
        frame(0, 0);
    n = layer_box(UI_LAYER_TITLE_BG, 1, &x0, &y0, &x1, &y1);
    CHECK(n == 1 && x0 == 0 && y0 == 0 && x1 == 640 && y1 == 480, "boss zone art full frame (%d: %.0f,%.0f-%.0f,%.0f)",
          n, x0, y0, x1, y1);
    n = layer_box(UI_LAYER_TITLE_BG, 0, &x0, &y0, &x1, &y1);
    CHECK(n > 0 && x0 >= 0x38 * 2.5f - 0.01f && x1 <= 0xc0 * 2.5f + 0.01f, "boss zigzag inside window 0x38..0xc0 (%d: %.1f..%.1f)",
          n, x0, x1);
    dump("boss_titlecard");
    CHECK(!NaviB_IsActive(), "no boss panel before StartStage");
    f = run_until_timer(600);
    CHECK(f > 0 && NaviB_IsActive(), "StartStage creates NaviB (%d)", f);
    /* g_boss_hooks.navi: Boss1_State_WaitForStart -> face 9, message 0xf for 0xf0 frames */
    s_voice_n = 0;
    ui_boss_navi(NULL, 9, 0xf, 0xf0);
    frame(0, 0);
    CHECK(s_voice_n == 1 && s_voice_last == g_ui_navi.voice[0][1], "start voice after 1 frame (%d, id %d)", s_voice_n,
          s_voice_last);
    for (f = 0; f < 400; f++) {
        frame(0, 0);
        if (ui_navi_message_visible())
            msg_frames++;
        if (f == 60)
            dump("boss_message");
    }
    printf("navigator message 0xf: on screen %d frames (time 0xf0)\n", msg_frames);
    CHECK(msg_frames >= 0xf0 - 2 && msg_frames <= 0xf0 + 2, "message time %d", msg_frames);
    /* a hit: Boss1Hammer_OnHit_Damage -> health 7, face 0xb, message 0x10 */
    ui_set_boss_health(7);
    ui_boss_navi(NULL, 0xb, 0x10, 0xf0);
    for (f = 0; f < 24; f++) {
        const UiQuad *qq;
        int i, m = ui_debug_quads(&qq);
        frame(0, 0);
        m = ui_debug_quads(&qq);
        for (i = 0; i < m; i++) {
            if (qq[i].layer == UI_LAYER_NAVI && qq[i].tex == UI_RES_NAVIB && qq[i].variant == UI_VAR_MAP0 + 1)
                lost_seen++;
            if (qq[i].layer == UI_LAYER_NAVI && qq[i].tex == UI_RES_NAVIB && qq[i].image == 14)
                flash++;
        }
        if (f == 8)
            dump("boss_hit");
    }
    printf("boss hit: lost icon %d frames, counter flash %d frames\n", lost_seen, flash);
    /* BLDALPHA eva = Lerp(0x1f, 0, t, 3) reaches 0 after 10 of the 16 steps */
    CHECK(lost_seen == 10, "lost icon animation %d", lost_seen);
    CHECK(flash == 2, "counter flash %d", flash);
    check_overlays("boss");
    /* the boss panel: bottom-right corner, 2x of the 105x56 box */
    n = layer_box(UI_LAYER_NAVI, 1, &x0, &y0, &x1, &y1);
    CHECK(n > 0 && x1 <= 608.01f && y1 <= 456.01f, "navi overlays in the safe area");
    /* the hurt line (Player_OnHurt in a boss stage) */
    s_voice_n = 0;
    ui_navi_player_hurt();
    for (f = 0; f < 30; f++)
        frame(0, 0);
    CHECK(s_voice_n == 1, "hurt voice (%d)", s_voice_n);
    /* the navigator face is streamed: one frame expands to 92x72 */
    {
        static u16 buf[128 * 128];
        int w, h;
        CHECK(ui_navi_face_expand(0, buf, 128, &w, &h) && w == 104 && h == 80, "face frame 0 %dx%d", w, h);
    }
    /* BossHelp (Boss6/7/F): above the panel, bottom right */
    BossHelp_Create(0);
    frame(0, 0);
    v0 = 0;
    {
        int i, m = ui_debug_quads(&q);
        for (i = 0; i < m; i++)
            if (q[i].tex == UI_RES_BOSSHELP) {
                v0++;
                CHECK(q[i].x + q[i].w <= 608.01f && q[i].y + q[i].h <= 456.0f - 112.0f, "BossHelp above the panel (%.0f,%.0f)",
                      q[i].x + q[i].w, q[i].y + q[i].h);
            }
    }
    CHECK(v0 == 1, "BossHelp drawn (%d)", v0);
    dump("boss_help");
    BossHelp_Destroy();
    /* early story: no navigator (Sonic progress < 3), the hurt line keeps its voice */
    G.save_progress[0] = 2;
    ui_stage_start(0, 2, 0);
    run_until_timer(600);
    s_voice_n = 0;
    ui_boss_navi(NULL, 9, 0xf, 0xf0);
    for (f = 0; f < 10; f++)
        frame(0, 0);
    CHECK(!ui_navi_message_visible() && s_voice_n == 0 && NaviB_IsActive(), "early story: panel only");
    G.save_progress[0] = 0xffff;
    /* normal stage: nothing of it */
    ui_stage_start(0, 0, 0);
    frame(0, 0);
    CHECK(!NaviB_IsActive(), "no boss panel in a normal act");
}

static void test_enemy_room(void) {
    int f, appear = -1, counted = 0, gone = -1, finish = 0;
    float x0, y0, x1, y1;
    const UiQuad *q;
    ui_stage_start(4, 0, 0);
    run_until_timer(600);
    s_sfx_last = -1;
    ui_enemy_room(0, 5);
    for (f = 0; f < 200; f++) {
        frame(0, 0);
        if (appear < 0 && f > 10) {
            int i, m = ui_debug_quads(&q);
            for (i = 0; i < m; i++)
                if (q[i].tex == UI_RES_ENEMYROOM && q[i].image == 5 + 1)   /* digit 5 (anim 6) */
                    appear = f;
        }
    }
    dump("enemyroom");
    CHECK(appear > 0, "enemy room: count 5 shown (frame %d)", appear);
    CHECK(s_sfx_last == 0xe7, "enemy room sfx 0xe7 (%d)", s_sfx_last);
    layer_box(UI_LAYER_NAVI, 0, &x0, &y0, &x1, &y1);
    CHECK(x0 >= 32 && x1 <= 608 && y0 >= 24 + 40, "enemy room counter under the HUD row (%.0f,%.0f-%.0f,%.0f)", x0, y0,
          x1, y1);
    check_overlays("enemy room");
    ui_enemy_room(1, 3);
    for (f = 0; f < 20; f++)
        frame(0, 0);
    {
        int i, m = ui_debug_quads(&q);
        for (i = 0; i < m; i++)
            if (q[i].tex == UI_RES_ENEMYROOM && q[i].image == 2 + 1)
                counted = 1;   /* 5 - 3 = 2 */
    }
    CHECK(counted, "enemy room: count 2");
    ui_enemy_room(1, 5);
    ui_enemy_room(2, 5);
    for (f = 0; f < 600; f++) {
        frame(0, 0);
        if (f == 200)
            dump("enemyroom_clear");
        if (layer_box(UI_LAYER_NAVI, 0, &x0, &y0, &x1, &y1) != 0)
            gone = f + 1;
        {
            int i, m = ui_debug_quads(&q);
            for (i = 0; i < m; i++)
                if (q[i].tex == UI_RES_ENEMYROOM && q[i].image == 11)
                    finish++;
        }
    }
    CHECK(finish > 100, "finish sprites shown (%d frames)", finish);
    printf("enemy room: cleared, counter gone after %d frames (0x50 + vanish + 0x20 + 0x80)\n", gone);
    CHECK(s_sfx_last == 0xe8, "clear sfx 0xe8 (%d)", s_sfx_last);
    CHECK(gone > 0x50 + 0x20 + 0x80 && gone < 0x50 + 0x20 + 0x80 + 60, "counter lifetime %d", gone);
}

static void test_backgrounds(void) {
    int f, n, tc_frames = 0;
    float x0, y0, x1, y1;
    /* title card: zone art (sub BG3) full frame, zigzag (main BG2) behind it until StartStage */
    ui_stage_start(1, 0, 1);   /* Blaze, zone index 1 */
    G.character = 1;
    for (f = 0; f < 40; f++)
        frame(0, 0);
    n = layer_box(UI_LAYER_TITLE_BG, 1, &x0, &y0, &x1, &y1);
    CHECK(n == 1 && x1 - x0 == 640 && y1 - y0 == 480, "title zone art 640x480 (%d)", n);
    n = layer_box(UI_LAYER_TITLE_BG, 0, &x0, &y0, &x1, &y1);
    CHECK(n >= 12 && n <= 20, "zigzag quads %d", n);
    for (f = 0; f < 400 && !(W.flags & 1); f++) {
        frame(0, 0);
        if (layer_box(UI_LAYER_TITLE_BG, -1, &x0, &y0, &x1, &y1))
            tc_frames++;
    }
    printf("title card BG: %d frames\n", tc_frames);
    frame(0, 0);
    CHECK(layer_box(UI_LAYER_TITLE_BG, -1, &x0, &y0, &x1, &y1) == 0, "title BG off after StartStage");
    /* stage_new hook (StoryProgress_IsStageNew); the pause needs both brightnesses at 0 */
    for (f = 0; f < 80; f++)
        frame(0, 0);
    G.character = 0;
    g_ui_hooks.stage_new = t_stage_new;
    frame(IN_START, 0);
    CHECK(ui_is_paused() && s_stage_new_calls == 1 && G.stage_new == 1, "pause asks StoryProgress_IsStageNew (%d %d %d)",
          ui_is_paused(), s_stage_new_calls, G.stage_new);
    dump("pause_new");
    frame(IN_START, 0);
    g_ui_hooks.stage_new = NULL;
    G.stage_new = 0;
}


/* time attack: ClearDemoTA tally (time 1'23"45 -> rank against the saved records),
 * CheckpointTimePopup (g_obj_hooks.checkpoint_popup) and MarkerTimeList */
static int count_res(int res, int layer) {
    const UiQuad *q;
    int n = ui_debug_quads(&q), i, k = 0;
    for (i = 0; i < n; i++)
        if (q[i].tex == res && (layer < 0 || q[i].layer == layer))
            k++;
    return k;
}
static void test_time_attack(void) {
    int f, popup_frames = 0, stamped = -1, ta_quads = 0;
    float x0, y0, x1, y1;
    u32 t = 1 * 3600 + 23 * 60 + 27; /* 1'23"45 */
    SaveData_SetDefaults(&gSaveData);
    gSaveData.unkD2[0][0][0] = 6000;  /* the best record is slower: rank 0 (new record) */
    W.mode = 2;
    ui_stage_start(0, 0, 0);
    run_until_timer(600);
    CHECK(Hud_GetFlags() & 0x40000000, "time attack: native score counter hidden");
    /* Marker_OnHit in time attack */
    g_obj_hooks.checkpoint_popup(g_obj_hooks.ctx, &P, 1234);
    CHECK(CheckpointTimePopup_IsActive(), "checkpoint popup created through the objects hook");
    for (f = 0; f < 400 && CheckpointTimePopup_IsActive(); f++) {
        frame(0, 0);
        popup_frames++;
        if (f == 10) {
            int nb = layer_box(UI_LAYER_HUD, 0, &x0, &y0, &x1, &y1);
            CHECK(nb > 0, "popup quads");
            dump("ta_popup");
        }
    }
    printf("checkpoint popup: %d frames (State_Main: > 0x12c)\n", popup_frames);
    CHECK(popup_frames == 0x12d, "popup lives 0x12d frames (%d)", popup_frames);
    /* pass-check marker: list of 5 laps */
    ui_marker_time_list(1);
    ui_marker_time_set(0, 2000);
    ui_marker_time_set(1, 1900);
    frame(0, 0);
    CHECK(MarkerTimeList_IsActive() && count_res(UI_RES_MOJI, UI_LAYER_HUD) > 5 * 8, "marker time list drawn");
    dump("ta_laps");
    ui_marker_time_list(0);
    /* the tally */
    W.frame = t;
    W.flags &= ~1u;
    ui_stage_exit(OBJ_EXIT_CLEAR_DEMO);
    for (f = 0; f < 3000 && ui_state() == UI_RESULTS; f++) {
        frame(0, 0);
        if (stamped < 0 && s_music_last == 0xc)
            stamped = f;
        if (f == 400)
            CHECK(count_res(UI_RES_CLDM_TA, UI_LAYER_RESULTS) == 4, "before the stamp: 2 labels + 2 separators");
        if (f == 560) {
            ta_quads = count_res(UI_RES_CLDM_TA, UI_LAYER_RESULTS);
            dump("ta_results");
        }
    }
    printf("time attack results: rank stamp (new record jingle 0xc) at frame %d, %d dm_cldm_tatk quads, exit after "
           "%d frames, kind %d\n",
           stamped, ta_quads, f, ui_exit_kind());
    CHECK(stamped > 0, "new record: Sound_PlaySeq 0xc at the stamp");
    CHECK(ta_quads == 6, "rank pair + labels + separators drawn (%d)", ta_quads);
    CHECK(ui_state() == UI_EXIT && ui_exit_kind() == UI_EXIT_TRY_AGAIN, "time attack -> TryAgain");
    CHECK(!g_ui_res[UI_RES_CLDM_TA].blob, "dm_cldm_tatk freed with the results");
    /* digits of the time: 1 / 2 3 / 4 5 -> dm_cldm 0x13 + d */
    W.mode = 0;
}

/* DemoPlay_Create: gGameState.unk10 = 3, unk18 |= 4 | 8 (start point): no title card,
 * the timer runs at once, the frame starts white; the blinking label */
static void test_demo_stage(void) {
    int f;
    W.mode = 3;
    W.flags = 0xc;
    ui_stage_start(0, 1, 0);
    CHECK(!StageTitleCard_IsActive(), "demo: no title card with a start point");
    CHECK((W.flags & 0xd) == 0xd, "demo: flags 4 | 8 kept, timer running (%x)", W.flags);
    CHECK(g_ui_bright_main == 16, "demo: starts white");
    frame(0, 0);
    CHECK(W.frame == 1, "demo: timer counts from the first frame (%u)", W.frame);
    ui_fade(3, 0x40);   /* DemoPlay_Main once the timer runs */
    ui_demo_label(1);
    for (f = 0; f < 80; f++)
        frame(0, 0);
    CHECK(g_ui_bright_main == 0, "demo: faded in from white");
    CHECK(count_res(UI_RES_DEMOPLAY, UI_LAYER_GAMEOVER) == 2, "demo: DEMO PLAY + PRESS START drawn");
    dump("demo_label");
    ui_demo_label(0);
    frame(0, 0);
    CHECK(count_res(UI_RES_DEMOPLAY, -1) == 0, "demo: label hidden");
    ui_demo_label(-1);
    CHECK(!g_ui_res[UI_RES_DEMOPLAY].blob, "demo: label sprites released");
    /* boss demo (zone 1 act 2): boss panel instead of the title card */
    W.flags = 0xc;
    ui_stage_start(1, 2, 1);
    CHECK(!StageTitleCard_IsActive() && NaviB_IsActive(), "boss demo: NaviB created by Stage_Create");
    W.mode = 0;
    W.flags = 0;
}

/* paletted atlases (engine/sprpal.h): every texel of every PAL4 page of the loaded
 * resources, through its image's palette, is the ARGB1555 atlas pixel */
static void test_pal4_atlases(const char *sprite_dir) {
    int i, im, x, y, pages = 0, bad = 0, n_px = 0;
    for (i = 0; i < UI_RES_COUNT; i++) {
        const UiRes *r = &g_ui_res[i];
        int pg;
        if (!r->blob || !r->pal.blob)
            continue;
        for (pg = 0; pg < r->n_pages && pg < 4; pg++) {
            char path[300];
            long sz;
            u16 *ref;
            if (!r->texels[pg])
                continue;
            if (snprintf(path, sizeof(path), "%s/%s/atlas%d.argb1555", sprite_dir, r->id, pg) >= (int)sizeof(path))
                continue;
            ref = (u16 *)load(path, &sz);
            if (!ref)
                continue;
            pages++;
            for (im = 0; im < r->n_images; im++) {
                const UiImage *m = &r->images[im];
                if (m->page != pg)
                    continue;
                for (y = m->y; y < m->y + m->h; y++)
                    for (x = m->x; x < m->x + m->w; x++, n_px++)
                        bad += ui_res_pixel(r, im, x, y) != ref[y * r->pages[pg].w + x];
            }
            free(ref);
        }
    }
    CHECK(pages > 0 && bad == 0, "PAL4 atlases: %d pages, %d image pixels, %d differ from ARGB1555", pages, n_px, bad);
    printf("PAL4 atlases: %d pages, %d image pixels decode to the ARGB1555 atlas\n", pages, n_px);
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
    test_pal4_atlases(argv[2]);
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
    g_ui_hooks.voice = t_voice;
    g_ui_hooks.sfx = t_sfx2;
    test_boss_navi();
    test_enemy_room();
    test_backgrounds();
    test_time_attack();
    test_demo_stage();
    ui_stage_end();   /* stage teardown: only ui_init's resources stay (VRAM baseline) */
    CHECK(!g_ui_res[UI_RES_TCBG].blob && !g_ui_res[UI_RES_CLDMBG].blob && !g_ui_res[UI_RES_NAVI].blob &&
              g_ui_res[UI_RES_MOJI].blob && g_ui_res[UI_RES_NAVIB].blob,
          "ui_stage_end frees the stage resources, keeps ui_init's");
    printf("%d passed, %d failed\n", s_pass, s_fail);
    return s_fail ? 1 : 0;
}
