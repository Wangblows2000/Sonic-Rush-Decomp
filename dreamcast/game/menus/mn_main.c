/*
 * Front-end entry points (menus.h): boot (Game_StartupSaveDataOnBoot ->
 * SceneChange_ToSegaLogo), the per-frame loop (gPad update, task list), the
 * scene requests to main.c and the way back from a stage.
 */
#include <stdio.h>
#include <stdlib.h>
#include "mn_internal.h"

const char *g_mn_scene = "none";

const char *mn_scene_name(void) { return g_mn_scene; }

void menus_set_paths(const char *menus_dir, const char *sprite_dir) {
    if (menus_dir)
        snprintf(g_mn_dir, sizeof(g_mn_dir), "%s", menus_dir);
    if (sprite_dir)
        snprintf(g_mn_sprite_dir, sizeof(g_mn_sprite_dir), "%s", sprite_dir);
}

int menus_language(void) { return gGameLanguage; }
void menus_set_language(int lang) { gGameLanguage = (u8)lang; }

/* test builds: <menus dir>/../debug/menutrace.txt turns on a serial trace of the
 * scene changes, the requests, the stage exits and the navigator state */
static int s_mn_trace = -1;
static int mn_trace_on(void) {
    if (s_mn_trace < 0) {
        char p[160];
        const char *slash = strrchr(g_mn_dir, '/');
        FILE *f;
        snprintf(p, sizeof(p), "%.*s/debug/menutrace.txt", slash ? (int)(slash - g_mn_dir) : 0, g_mn_dir);
        f = fopen(p, "rb");
        s_mn_trace = f != NULL;
        if (f)
            fclose(f);
    }
    return s_mn_trace > 0;
}

int menus_init(void) {
    char path[160];
    u8 *buf;
    u32 size;
    int ok;
    snprintf(path, sizeof(path), "%s/menudata.bin", g_mn_dir);
    ok = mn_load_file(path, &buf, &size) == 0 && mn_data_load(buf, size) == 0;
    if (buf)
        free(buf);
    if (!ok)
        fprintf(stderr, "menus: %s missing or invalid\n", path);
    memcpy(g_save_zone_order, g_mn_data.zone_ord, sizeof(g_save_zone_order));
    if (g_mn_data.ok) {
        save_set_signature(g_mn_data.save_sig);
        save_set_icon(g_mn_data.vmu_pal, g_mn_data.vmu_icon);
    }
    /* Game_StartupSaveDataOnBoot */
    save_load();
    {
        /* test builds: <menus dir>/../debug/menusave.txt "flags sonic_progress blaze_progress [sonic_emeralds]"
         * ORs gSaveData.unk8 and raises the story progress (unlocks for Flycast runs) */
        char p[160];
        const char *slash = strrchr(g_mn_dir, '/');
        FILE *f;
        unsigned fl = 0, sp = 0, bp = 0, em = 0;
        snprintf(p, sizeof(p), "%.*s/debug/menusave.txt", slash ? (int)(slash - g_mn_dir) : 0, g_mn_dir);
        f = fopen(p, "rb");
        if (f) {
            int n = fscanf(f, "%u %u %u %u", &fl, &sp, &bp, &em);
            if (n >= 1) {
                gSaveData.unk8 |= fl;
                if (n >= 4)
                    gSaveData.unkF |= (u8)em;   /* Sonic's Chaos Emeralds (bits 0..6) */
                if (sp > gSaveData.unkC)
                    gSaveData.unkC = (u16)sp;
                if (bp > gSaveData.unk64)
                    gSaveData.unk64 = (u16)bp;
                printf("menus: debug save preset flags %#x progress %u/%u emeralds %#x\n", fl, gSaveData.unkC, gSaveData.unk64,
                       gSaveData.unkF);
            }
            fclose(f);
        }
    }
    if (mn_trace_on())
        printf("menus trace boot save: error %d, progress %u/%u, lives %u/%u, emeralds %#x, flags %#x, counter %u\n",
               save_last_error(), (unsigned)gSaveData.unkC, (unsigned)gSaveData.unk64, (unsigned)gSaveData.unkE,
               (unsigned)gSaveData.unk66, (unsigned)gSaveData.unkF, (unsigned)gSaveData.unk8, (unsigned)gSaveData.unk4);
    memset(&gGameState, 0, sizeof(gGameState));
    Pad_Reset(&gPad, NULL, NULL);
    BRIGHT_MAIN = BRIGHT_SUB = -16;
    mn_display_reset();
    SegaLogo_Start();
    return ok ? 0 : -1;
}

static void mn_trace_frame(void) {
    static const char *last_scene;
    static int last_panel = -2, last_face_on = -2;
    static u32 frame;
    int panel, alpha, st;
    if (!mn_trace_on())
        return;
    frame++;
    if (g_mn_scene != last_scene) {
        printf("menus trace %u scene %s\n", (unsigned)frame, g_mn_scene ? g_mn_scene : "-");
        last_scene = g_mn_scene;
    }
    if (g_mn_request.kind != MENU_REQ_NONE)
        printf("menus trace %u request %d zone %d act %d mode %d\n", (unsigned)frame, (int)g_mn_request.kind,
               (int)g_mn_request.zone, (int)g_mn_request.act, (int)g_mn_request.mode);
    st = mn_navi_state(NULL, &panel, &alpha);
    if (st && (panel != last_panel || (alpha >= 0) != last_face_on)) {
        printf("menus trace %u navi %d panel %d face %s regions %d\n", (unsigned)frame, st, panel,
               alpha >= 0 ? "on" : "off", g_mn_comp.n);
        last_panel = panel;
        last_face_on = alpha >= 0;
    }
}

MenuRequest menus_run_frame(const InputState *in) {
    memset(&g_mn_request, 0, sizeof(g_mn_request));
    Pad_Update(&gPad, mn_pad_from_input(in));
    mn_quads_reset();
    Task_RunAll();
    mn_trace_frame();
    return g_mn_request;
}

/* ClearDemo_GoToNextScene (story / time attack, after ClearDemo_Create's
 * StoryProgress_Update / SaveData_InsertTimeRecord and ClearDemo_Destructor's
 * SaveData_StoreStageResult + commit) */
static void ClearDemo_GoToNextScene(void) {
    u8 zone;
    if (gGameState.unk10 == MENU_MODE_TIME_ATTACK) {
        TryAgain_Start();   /* SceneChange_ToTryAgain */
        return;
    }
    zone = gGameState.unk1C;
    if (zone == 8) {
        mn_request_unavailable("ExtraEnding");
        TitleScreen_Start();
        return;
    }
    if (zone == 7) {
        mn_request_unavailable("Ending");
        TitleScreen_Start();
        return;
    }
    if (gGameState.new_flags & 1) {
        gGameState.new_flags &= ~1u;
        switch (gGameState.unk1D) {
        case 0:   /* Game_AdvanceAct */
            gGameState.unk1D++;
            mn_request_stage();
            return;
        case 1:   /* Game_AdvanceActWithDemo: the boss intro script, then the boss act */
            gGameState.unk1D++;
            mn_request_stage();
            return;
        case 2:
            if (zone == 6) {
                /* DemoScript_StartScript4 (story cutscene), then the map */
                mn_request_unavailable("DemoScript4");
            }
            ZoneMap_StartFromStageWithFade();
            return;
        }
    } else {
        switch (gGameState.unk1D) {
        case 0:
        case 1:
            if (gGameState.unk0 != 1 && zone == 0 && gSaveData.unkC < 2) {
                ZoneActSelectMode_Start();
                return;
            }
            ZoneMap_StartFromStage();
            return;
        case 2:
            ZoneMap_StartFromStage();
            return;
        }
    }
}

MenuRequest menus_stage_exit(MenuExit how, uint32_t score, int lives, int emeralds, uint32_t time) {
    memset(&g_mn_request, 0, sizeof(g_mn_request));
    Task_DestroyAll();
    mn_display_reset();
    mn_bgm_forget();   /* the stage ran its own music (the DS overlay change: SysSound_Release) */
    gGameState.score = score;
    gGameState.lives = (u8)(lives < 0 ? 0 : lives > 99 ? 99 : lives);
    gGameState.unk28 = (u16)emeralds;
    switch (how) {
    case MENU_EXIT_CLEARED:
        if (gGameState.unk10 == MENU_MODE_TIME_ATTACK) {
            SaveData_InsertTimeRecord(gGameState.unk1C, gGameState.unk1D, time, (u32)gGameState.unk0,
                                      gGameState.unk1C, gGameState.unk1D);
        } else {
            StoryProgress_Update();
            SaveData_StoreStageResult(gGameState.unk0, gGameState.unk1C, gGameState.unk1D, gGameState.lives,
                                      gGameState.unk28, score);
        }
        SaveData_CommitOrShowError(NULL);
        BRIGHT_MAIN = BRIGHT_SUB = 16;
        ClearDemo_GoToNextScene();
        break;
    case MENU_EXIT_QUIT:   /* PauseMenu_QuitToMap */
        SaveData_StoreLives(gGameState.unk0, gGameState.lives, gGameState.unk28);
        SaveData_CommitOrShowError(NULL);
        BRIGHT_MAIN = BRIGHT_SUB = -16;
        if (gGameState.unk10 == MENU_MODE_TIME_ATTACK) {
            TitleMenu_StartAtPlayerSelectItem0();
            break;
        }
        if (gGameState.unk0 != 1 && gGameState.unk1C == 0 && gSaveData.unkC < 2) {
            ZoneActSelectMode_Start();
            break;
        }
        if (StoryProgress_IsStageNew() || (gGameState.unk1C == 7 && gGameState.unk1D == 2))
            ZoneMap_StartFromStageWithFade();
        else
            ZoneMap_StartFromStage();
        break;
    case MENU_EXIT_EXTRA_QUIT:
        /* PauseMenu_Main_Quit, zone 8 act 2: ExtraBoss_Shutdown, SceneChange_ToPlayerSelectMenuMode0 */
        BRIGHT_MAIN = BRIGHT_SUB = 16;
        PlayerSelectMenu_StartMode(0);
        break;
    case MENU_EXIT_TRY_AGAIN:
        /* PauseMenu_QuitToTryAgain / Stage_ExitAfterFail (time attack): no record */
        BRIGHT_MAIN = BRIGHT_SUB = 16;
        TryAgain_Start();
        break;
    case MENU_EXIT_GAME_OVER:
        /* Stage_ExitAfterFail (story): SaveData_SetLives(unk0, lives, 1) (at least 2),
         * gGameState.unk28 = SaveData_GetSonicEmeralds(), commit; then GameOverSeq ->
         * SceneChange_ToSegaLogo */
        if (gGameState.unk10 == MENU_MODE_STORY) {
            SaveData_SetLives(gGameState.unk0, gGameState.lives, 1);
            gGameState.unk28 = SaveData_GetSonicEmeralds();
            SaveData_CommitOrShowError(NULL);
        }
        BRIGHT_MAIN = BRIGHT_SUB = -16;
        SegaLogo_Start();
        break;
    case MENU_EXIT_DEMO_END:
        /* DemoPlay_Main: a key -> StageExit_Create(SceneChange_ToTitleScreen); the
         * screen faded to white (BrightnessFade 5) */
        BRIGHT_MAIN = BRIGHT_SUB = 16;
        gGameState.unk10 = 0;
        TitleScreen_Start();
        break;
    case MENU_EXIT_DEMO_TIMEOUT:
        /* the frame limit -> SceneChange_ToSegaLogo (the attract loop) */
        BRIGHT_MAIN = BRIGHT_SUB = 16;
        gGameState.unk10 = 0;
        SegaLogo_Start();
        break;
    default:
        BRIGHT_MAIN = BRIGHT_SUB = -16;
        SegaLogo_Start();
        break;
    }
    if (mn_trace_on() && gGameState.unk10 == MENU_MODE_TIME_ATTACK && gGameState.unk1C < 7 && gGameState.unk1D < 3)
        printf("menus trace time records zone %d act %d: %u %u %u (time %u)\n", (int)gGameState.unk1C,
               (int)gGameState.unk1D, (unsigned)gSaveData.unkD2[gGameState.unk1C][gGameState.unk1D][0],
               (unsigned)gSaveData.unkD2[gGameState.unk1C][gGameState.unk1D][1],
               (unsigned)gSaveData.unkD2[gGameState.unk1C][gGameState.unk1D][2], (unsigned)time);
    if (mn_trace_on())
        printf("menus trace exit %d zone %d act %d mode %d -> save progress %u/%u lives %u/%u emeralds %#x flags %#x, "
               "scene %s, request %d zone %d act %d\n",
               (int)how, (int)gGameState.unk1C, (int)gGameState.unk1D, (int)gGameState.unk10, (unsigned)gSaveData.unkC,
               (unsigned)gSaveData.unk64, (unsigned)gSaveData.unkE, (unsigned)gSaveData.unk66, (unsigned)gSaveData.unkF,
               (unsigned)gSaveData.unk8, g_mn_scene, (int)g_mn_request.kind, (int)g_mn_request.zone,
               (int)g_mn_request.act);
    return g_mn_request;
}

void menus_shutdown(void) {
    Task_DestroyAll();
    mn_display_reset();
}
