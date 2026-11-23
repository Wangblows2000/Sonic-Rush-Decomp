/* Host test of the whole story progression, the way main.c strings the front end,
 * the cutscenes and the stages together: a model of main.c's scene flow (front-end
 * requests, stage exits, DemoScript 0 / after-boss script 1, zone-map talks, the
 * Extra Zone, endings, staff roll, PostStaffRoll unlock notices) drives the real
 * menus, save and cutscene modules on the owner's converted data. Every stage is
 * "cleared" at once (MENU_EXIT_CLEARED); the zone map is walked by the menus'
 * test autopilot (g_mn_automap, mn_zonemap.c); talks and cutscenes get an A press
 * every 6 frames. The model follows main.c with the integrator patches in
 * dreamcast/game/menus/patches/ applied: keep the two in step.
 *
 * Usage: test_story <build/dreamcast/menus> <sprites/bac> <build/dreamcast/cutscene> <models>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../mn_internal.h"
#include "../extraact.h"
#include "../../cutscene/cutscene.h"

static int fails, checks;
#define CHECK(c)                                                                              \
    do {                                                                                      \
        checks++;                                                                             \
        if (!(c)) {                                                                           \
            fails++;                                                                          \
            printf("FAIL %s:%d: %s (scene %s, frame %ld)\n", __FILE__, __LINE__, #c, g_mn_scene, s_frame); \
        }                                                                                     \
    } while (0)

/* ------------------------------------------------------------ "VMU" */
static uint8_t s_img[SAVE_IMAGE_SIZE];
static int s_have_img, s_writes;
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
    s_writes++;
    return 0;
}

/* ------------------------------------------------------------ event log */
static long s_frame;
static char s_log[4096][96];
static int s_nlog;
static void ev(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
#include <stdarg.h>
static void ev(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    if (s_nlog < 4096)
        vsnprintf(s_log[s_nlog++], sizeof(s_log[0]), fmt, ap);
    va_end(ap);
    if (getenv("STORY_VERBOSE"))
        printf("  [%ld] %s\n", s_frame, s_log[s_nlog - 1]);
}
static int find_ev(const char *s, int from) {
    int i;
    for (i = from; i < s_nlog; i++)
        if (!strcmp(s_log[i], s))
            return i;
    return -1;
}
static int count_prefix(const char *p) {
    int i, n = 0;
    for (i = 0; i < s_nlog; i++)
        if (!strncmp(s_log[i], p, strlen(p)))
            n++;
    return n;
}

/* ------------------------------------------------------------ model of main.c */
static int g_chara, g_mode, g_lives, g_emeralds, g_zone, g_act;
static uint32_t g_extra_res_time = 20000, g_extra_res_rings = 150;
static int g_give_emeralds = 1;   /* the special stage of every zone is cleared (Sonic) */
static int g_stage_count;
/* what the speakers play: a sys SDAT sequence (menus bgm hook), -1 silence,
 * 1000 stage music, 2000 a cutscene's own music */
static int g_music = -1, g_silent_map_frames, g_map_frames;
static void hook_bgm(int seq) { g_music = seq; }
static void hook_stop_all(void) { g_music = -1; }

static InputState auto_in(void) {
    InputState in;
    memset(&in, 0, sizeof(in));
    if ((s_frame % 6) == 0)
        in.held = in.pressed = IN_JUMP;
    return in;
}

static void cs_story(int zone) {
    CsStory st;
    memset(&st, 0, sizeof(st));
    st.character = g_chara;
    st.zone = zone;
    st.stage_new = StoryProgress_IsStageNew();
    st.save_flags = gSaveData.unk8;
    st.sonic_progress = gSaveData.unkC;
    st.blaze_progress = gSaveData.unk64;
    st.language = menus_language();
    st.sonic_emeralds = gSaveData.unkF & 0x7f;
    cutscene_set_story(&st);
}
static CsNext cs_finish(void) {
    const CsResult *r = cutscene_result();
    CsNext next = r ? r->next : CS_NEXT_NONE;
    if (r) {
        if (r->progress >= 0) {
            if (g_chara)
                SaveData_SetBlazeProgress((uint16_t)r->progress);
            else
                SaveData_SetSonicProgress((uint16_t)r->progress);
            SaveData_StoreLives(g_chara, (uint8_t)g_lives, (uint16_t)g_emeralds);
        }
        if (r->set_save_flags)
            SaveData_SetProgressFlags(SaveData_GetProgressFlags() | r->set_save_flags);
        if (r->set_sonic_progress >= 0)
            SaveData_SetSonicProgress((uint16_t)r->set_sonic_progress);
        if (r->set_blaze_progress >= 0)
            SaveData_SetBlazeProgress((uint16_t)r->set_blaze_progress);
        if (r->game_cleared)
            SaveData_SetGameCleared();
        if (r->progress >= 0 || r->set_save_flags || r->set_sonic_progress >= 0 || r->set_blaze_progress >= 0 ||
            r->game_cleared)
            SaveData_Commit();
    }
    cutscene_shutdown();
    return next;
}
static CsNext run_cutscene(CsKind kind, int id, int zone) {
    InputState in;
    long f;
    cs_story(zone);
    g_music = 2000;
    if (cutscene_start(kind, id, g_chara) != 0) {
        ev("cutscene %d/%d missing", kind, id);
        return CS_NEXT_NONE;
    }
    for (f = 0; f < 200000; f++) {
        in = auto_in();
        s_frame++;
        if (cutscene_run_frame(&in))
            break;
    }
    {
        CsNext n = cs_finish();
        ev("cutscene %d/%d zone %d -> next %d", kind, id, zone, n);
        CHECK(f < 200000);
        return n;
    }
}
static int menu_talk_start(int c, int step) {
    int id = cutscene_zonemap_talk_id(c, step);
    cs_story(0);
    g_music = 2000;   /* the talk plays its own BGM */
    ev("talk %#x (char %d step %d)", id, c, step);
    return cutscene_start(CS_TALK_MAP, id, c);
}
static int menu_talk_active(void) { return cutscene_active(); }

static MenuRequest after_ending(CsNext n) {
    MenuRequest r;
    memset(&r, 0, sizeof(r));
    /* integrator patch: PostStaffRoll -> the unlock notices (menus), then the SEGA logo */
    if (n == CS_NEXT_UNLOCK_NOTICES) {
        menus_post_staff_roll();
        ev("post staff roll notices");
    }
    return r;
}

static void run_unavailable(const MenuRequest *r) {
    if (!r->scene)
        return;
    ev("unavailable %s", r->scene);
    if (!strcmp(r->scene, "Ending"))
        after_ending(run_cutscene(CS_ENDING, 0, 7));
    else if (!strcmp(r->scene, "ExtraEnding"))
        after_ending(run_cutscene(CS_EXTRA_ENDING, 0, 8));
    else if (!strcmp(r->scene, "DemoScript4")) {
        run_cutscene(CS_SCRIPT, 4, 6);
        menus_restart_bgm();   /* integrator patch */
    }
}

static MenuRequest run_extra_zone(void) {
    CsNext n = run_cutscene(CS_SCRIPT, 3, 8);
    InputState in;
    long f;
    CHECK(n == CS_NEXT_EXTRA_BOSS);
    ev("extra boss cleared");
    SaveData_SetLivesBoth(gSaveData.unkE, gSaveData.unk66);
    run_cutscene(CS_SCRIPT, 2, 8);
    CHECK(extraact_start(g_extra_res_time, g_extra_res_rings, g_chara) == 0);
    for (f = 0; f < 5000; f++) {
        in = auto_in();
        s_frame++;
        if (extraact_run_frame(&in))
            break;
    }
    extraact_shutdown();
    ev("extraact done");
    return menus_stage_exit(MENU_EXIT_CLEARED, g_extra_res_rings, gSaveData.unk66, g_emeralds, g_extra_res_time);
}

/* a stage request: play the stage(s) until the front end takes over again */
static void play(MenuRequest r) {
    for (;;) {
        if (r.kind == MENU_REQ_STAGE && r.zone == 8 && r.act == 2) {
            g_chara = r.character;
            ev("extra zone");
            r = run_extra_zone();
            if (r.kind == MENU_REQ_UNAVAILABLE)
                run_unavailable(&r);
            return;
        }
        if (r.kind != MENU_REQ_STAGE) {
            if (r.kind == MENU_REQ_UNAVAILABLE)
                run_unavailable(&r);
            return;
        }
        g_chara = r.character;
        if (r.mode == MENU_MODE_STORY && r.act == 2) {
            CsNext n = run_cutscene(CS_SCRIPT, 0, r.zone);
            CHECK(n == CS_NEXT_STAGE);
        }
        g_mode = r.mode;
        g_lives = r.lives;
        g_emeralds = r.emeralds;
        g_zone = r.zone;
        g_act = r.act;
        ev("stage z%d a%d char %d mode %d lives %d", r.zone, r.act, r.character, r.mode, r.lives);
        g_music = 1000;   /* load_stage: start_zone_music */
        g_stage_count++;
        /* the act: the special stage (Sonic, act 1 of each zone) gives the zone's emerald */
        if (g_give_emeralds && g_chara == 0 && g_act == 0 && g_zone < 7)
            g_emeralds |= 1 << g_zone;
        g_lives++;
        if (g_mode == MENU_MODE_STORY && g_act == 2 && g_zone != 7) {
            /* boss_cleared_hook -> DemoScript_StartAfterBoss -> ClearDemo */
            CsNext n = run_cutscene(CS_SCRIPT, 1, g_zone);
            CHECK(n == CS_NEXT_CLEARDEMO);
        }
        r = menus_stage_exit(MENU_EXIT_CLEARED, 30000, g_lives, g_emeralds, 4000);
    }
}

static void handle(MenuRequest r) {
    if (r.kind == MENU_REQ_STAGE || (r.kind == MENU_REQ_UNAVAILABLE))
        play(r);
    else if (r.kind == MENU_REQ_DEMO) {
        ev("demo %d", r.demo);
        menus_stage_exit(MENU_EXIT_DEMO_END, 0, 0, g_emeralds, 0);
    }
}

/* one front-end frame, with the input a player would give on each scene */
static const char *s_last_scene;
static int s_scene_frames, s_psel_moves;
static void front_end_frame(int character_choice) {
    InputState in;
    MenuRequest r;
    memset(&in, 0, sizeof(in));
    if (g_mn_scene != s_last_scene) {
        s_last_scene = g_mn_scene;
        s_scene_frames = 0;
        s_psel_moves = 0;
        ev("scene %s", g_mn_scene);
    }
    s_scene_frames++;
    if (cutscene_active())
        in = auto_in();
    else if (!strcmp(g_mn_scene, "TitleScreen") && s_scene_frames == 0x60)
        in.held = in.pressed = IN_START;
    else if (!strcmp(g_mn_scene, "TitleMenu") && s_scene_frames == 0x50)
        in.held = in.pressed = IN_JUMP;   /* GAME START */
    else if (!strcmp(g_mn_scene, "PlayerSelect") && s_scene_frames >= 0x60 && (s_scene_frames % 0x40) == 0) {
        if (s_psel_moves < character_choice) {
            in.held = in.pressed = IN_RIGHT;
            s_psel_moves++;
        } else if (s_psel_moves == character_choice) {
            in.held = in.pressed = IN_JUMP;
            s_psel_moves++;
        }
    }
    s_frame++;
    r = menus_run_frame(&in);
    menus_draw_tr();
    if (cutscene_active() && !cutscene_is_fullscreen() && cutscene_run_frame(&in))
        cs_finish();
    if (r.kind != MENU_REQ_NONE)
        handle(r);
    {
        /* the zone map waits for the player: its BGM (SEQ_ZONE_NOR 3 / SEQ_ZONE_LAST 4) must play */
        extern s32 g_zm_select_ticks;
        static s32 last_ticks;
        if (g_zm_select_ticks != last_ticks && !cutscene_active()) {
            g_map_frames++;
            if (g_music != 3 && g_music != 4) {
                if (g_silent_map_frames++ == 0)
                    printf("zone map without its BGM (music %d) at frame %ld\n", g_music, s_frame);
            }
        }
        last_ticks = g_zm_select_ticks;
    }
}
/* run the front end until the event `until` is logged after `from` (and the scene is `scene`) */
static int run_until(const char *until, int from, const char *scene, int choice, long max) {
    long f;
    for (f = 0; f < max; f++) {
        front_end_frame(choice);
        if (find_ev(until, from) >= 0 && (!scene || !strcmp(g_mn_scene, scene)))
            return 1;
    }
    printf("timeout waiting for '%s' (scene %s)\n", until, g_mn_scene);
    return 0;
}

static void reboot(void) {
    memset(&gSaveData, 0, sizeof(gSaveData));
    CHECK(save_load() == SAVE_OK);
    menus_shutdown();
    memset(&gGameState, 0, sizeof(gGameState));
    SegaLogo_Start();
}

static void dump_log(int from) {
    int i;
    for (i = from; i < s_nlog; i++)
        if (strncmp(s_log[i], "scene ", 6) || getenv("STORY_SCENES"))
            printf("    %s\n", s_log[i]);
}

int main(int argc, char **argv) {
    char models[300];
    int start;
    if (argc < 5) {
        printf("usage: %s menus_dir sprites_dir cutscene_dir models_dir\n", argv[0]);
        return 2;
    }
    g_save_backend.present = h_present;
    g_save_backend.read = h_read;
    g_save_backend.write = h_write;
    g_menu_hooks.talk_demo_start = menu_talk_start;
    g_menu_hooks.talk_demo_active = menu_talk_active;
    g_menu_hooks.bgm = hook_bgm;
    g_menu_hooks.stop_all = hook_stop_all;
    g_mn_automap = 1;
    menus_set_paths(argv[1], argv[2]);
    cutscene_set_paths(argv[3], argv[2], argv[4], "/nonexistent");
    snprintf(models, sizeof(models), "%s", argv[4]);
    extraact_set_model_dir(models);
    CHECK(menus_init() == 0);

    /* ================= Sonic: new save -> the ending */
    printf("--- Sonic story from a new save\n");
    start = s_nlog;
    CHECK(run_until("post staff roll notices", start, "SegaLogo", 0, 3000000));
    dump_log(start);
    {
        /* zones in story order (act 1 first), script 0 before and script 1 after every boss of zones 0..6 */
        int z, from = start;
        char s[96];
        for (z = 0; z < 8; z++) {
            int k, hit = -1;
            for (k = from; k < s_nlog; k++)
                if (!strncmp(s_log[k], "stage z", 7) && s_log[k][7] - '0' == z && s_log[k][10] == (z < 7 ? '0' : '2')) {
                    hit = k;
                    break;
                }
            CHECK(hit >= 0 && s_log[hit][7] - '0' == z);
            if (hit < 0)
                break;
            from = hit + 1;
            if (z < 7) {
                snprintf(s, sizeof(s), "cutscene %d/0 zone %d -> next %d", CS_SCRIPT, z, CS_NEXT_STAGE);
                CHECK(find_ev(s, from) >= 0);
                snprintf(s, sizeof(s), "cutscene %d/1 zone %d -> next %d", CS_SCRIPT, z, CS_NEXT_CLEARDEMO);
                CHECK(find_ev(s, from) >= 0);
            }
        }
    }
    CHECK(find_ev("unavailable DemoScript4", start) >= 0);
    CHECK(find_ev("unavailable Ending", start) >= 0);
    CHECK(count_prefix("talk ") >= 9);
    CHECK(gSaveData.unkC == 26);
    CHECK((gSaveData.unk8 & 0x300) == 0x300);   /* PostStaffRoll: player-select page + sound test */
    CHECK((gSaveData.unkF & 0x7f) == 0x7f);
    printf("Sonic: progress %d, emeralds %#x, flags %#x, %d stages, %d VMU writes\n", gSaveData.unkC,
           gSaveData.unkF, (unsigned)gSaveData.unk8, g_stage_count, s_writes);
    reboot();
    CHECK(gSaveData.unkC == 26);

    /* ================= Blaze */
    printf("--- Blaze story\n");
    start = s_nlog;
    g_stage_count = 0;
    CHECK(run_until("post staff roll notices", start, "SegaLogo", 1, 3000000));
    dump_log(start);
    CHECK(gSaveData.unk64 == 28);
    {
        /* Blaze's zone order: 3 0 2 1 5 4 6 7 */
        static const int ord[8] = { 3, 0, 2, 1, 5, 4, 6, 7 };
        int k, n = 0;
        for (k = start; k < s_nlog; k++)
            if (!strncmp(s_log[k], "stage z", 7) && (s_log[k][10] == '0' || s_log[k][7] == '7')) {
                CHECK(n < 8 && s_log[k][7] - '0' == ord[n]);
                n++;
            }
        CHECK(n == 8);
    }
    printf("Blaze: progress %d, flags %#x, %d stages\n", gSaveData.unk64, (unsigned)gSaveData.unk8, g_stage_count);
    reboot();

    /* ================= Extra: unlocked by both stories + the seven Chaos Emeralds */
    printf("--- Extra story\n");
    start = s_nlog;
    CHECK(run_until("extraact done", start, "SegaLogo", 2, 400000));
    dump_log(start);
    CHECK(find_ev("unavailable ExtraEnding", start) >= 0);
    CHECK(gSaveData.unk8 & 0x1000);
    printf("Extra: flags %#x\n", (unsigned)gSaveData.unk8);
    /* ================= exits other than a clear */
    printf("--- game over, quit, time attack without a clear\n");
    {
        MenuRequest r;
        uint16_t rec;
        /* story game over: Stage_ExitAfterFail keeps at least 2 lives in the save and commits */
        gGameState.unk0 = 0;
        gGameState.unk10 = MENU_MODE_STORY;
        gGameState.unk1C = 2;
        gGameState.unk1D = 0;
        gSaveData.unkE = 12;
        r = menus_stage_exit(MENU_EXIT_GAME_OVER, 0, 0, g_emeralds, 0);
        CHECK(r.kind == MENU_REQ_NONE && !strcmp(g_mn_scene, "SegaLogo"));
        CHECK(gSaveData.unkE == 2);
        memset(&gSaveData, 0, sizeof(gSaveData));
        CHECK(save_load() == SAVE_OK && gSaveData.unkE == 2);
        /* pause quit to the map: the lives of the act are kept */
        gGameState.unk0 = 0;
        gGameState.unk10 = MENU_MODE_STORY;
        gGameState.unk1C = 2;
        gGameState.unk1D = 1;
        r = menus_stage_exit(MENU_EXIT_QUIT, 0, 9, g_emeralds, 0);
        CHECK(r.kind == MENU_REQ_NONE && gSaveData.unkE == 9);
        CHECK(!strcmp(g_mn_scene, "ZoneMap") || !strcmp(g_mn_scene, "ActSelect"));
        /* time attack quit / fail: TryAgain without a record; a clear inserts one */
        gGameState.unk10 = MENU_MODE_TIME_ATTACK;
        gGameState.unk1C = 1;
        gGameState.unk1D = 0;
        rec = gSaveData.unkD2[1][0][0];
        r = menus_stage_exit(MENU_EXIT_TRY_AGAIN, 0, 3, 0, 1234);
        CHECK(r.kind == MENU_REQ_NONE && !strcmp(g_mn_scene, "TryAgain") && gSaveData.unkD2[1][0][0] == rec);
        gGameState.unk10 = MENU_MODE_TIME_ATTACK;
        gGameState.unk1C = 1;
        gGameState.unk1D = 0;
        r = menus_stage_exit(MENU_EXIT_CLEARED, 0, 3, 0, 1234);
        CHECK(!strcmp(g_mn_scene, "TryAgain") && gSaveData.unkD2[1][0][0] == 1234);
        printf("game over lives %d, quit lives 9, TA record after a clear %d (was %#x)\n", 2, gSaveData.unkD2[1][0][0], rec);
        /* Extra boss pause GO BACK: the player select */
        gGameState.unk10 = MENU_MODE_STORY;
        gGameState.unk1C = 8;
        gGameState.unk1D = 2;
        r = menus_stage_exit(MENU_EXIT_EXTRA_QUIT, 0, 3, 0x7f, 0);
        CHECK(r.kind == MENU_REQ_NONE && !strcmp(g_mn_scene, "PlayerSelect"));
    }
    printf("zone map: %d select frames, %d without the map BGM\n", g_map_frames, g_silent_map_frames);
    CHECK(g_silent_map_frames == 0);
    printf("story test: %d/%d checks passed\n", checks - fails, checks);
    return fails ? 1 : 0;
}
