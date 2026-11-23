/*
 * Stage flow and game state (score / time / lives), translated from:
 *   Stage_Main (HUD feed, timer, pause request), Stage_Create (timer reset, HUD
 *   flags), Stage_ResetProgress, Player_Create (Init_0234d62c: stage-entry reset of
 *   data_022c4590), Player_AddLives, Player_AddTrickScore (score part),
 *   Player_CheckTimeOver, Player_Last (game over / time over exits), StageExit_Main,
 *   Stage_ExitAfterFail, BrightnessFade_*, Math_LerpFx32, StoryProgress_GetZoneIndex.
 *
 * Ownership of the original globals:
 *   gGameState.unk1C/1D/10/18/20 -> g_dcp_world.zone/act/mode/flags/frame
 *   data_022c4590.unk14 (lives)  -> g_dcp_world.lives
 *   data_022c4590.unk8/unk10/unk18/unk40 -> g_obj_stage.speed_bonus/ring_bonus/
 *                                   check_time/goal_state
 *   the rest                     -> g_ui_game (ui.h)
 *
 * Timer: Stage_Main owns gGameState.unk20 (+1 per frame while flags & 1). The
 * player module's dcp_player_frame also does W.frame++ (stand-in from before this
 * module existed); ui_post_update restores the Stage_Main value so the timer is
 * counted once. Objects updated after the player see timer + 1 during that frame.
 */
#include <stdio.h>
#include <stdlib.h>
#include "ui_internal.h"

UiGame g_ui_game = { 0, 0, 0, 0, 0, 0, 0, 1, { 3, 3 }, 1, { 0xffff, 0xffff } };
int g_ui_layer;
UiHooks g_ui_hooks;
u16 g_ui_trig, g_ui_held;

static int s_state = UI_PLAYING;
static int s_exit_kind = UI_EXIT_NONE;
static int s_exit_cleared;   /* ui_exit_cleared() */
static int s_pending_exit = -1;       /* StageExit_Create: runs next frame */
static void (*s_pending_cb)(void);
static u32 s_timer;                   /* gGameState.unk20 as Stage_Main left it */
static DcPlayer *s_player;
static int s_pending_restart;   /* StageRestart_Create -> Stage_Create */
static int s_inited;

/* ----------------------------------------------------------- small helpers */
/* ObjShift_Set / ObjShift_SetFromRef (Stage/ObjectSystem): the objects module's
 * copies (objects/obj_fx.c) are used. */

s32 Math_LerpFx32(s32 a, s32 b, s32 t) {
    if (t == 0)
        return b;
    if (t != 0x1000)
        a = b + (((a - b) * t) >> 12);
    return a;
}

u8 StoryProgress_GetZoneIndex(u8 v) {
    const u8 *tbl = g_ui_data.zone_ord[G.character == 1 ? 1 : 0];
    u8 i;
    for (i = 0; i < 9; i++) {
        if (v == tbl[i])
            return i;
    }
    return 0xff;
}

int ui_lang(void) {
    switch (G.language) {
    case 0: case 1: case 2: case 3: case 4: case 5:
        return G.language;
    default:
        return 1;
    }
}

/* DS KEYINPUT bit order */
u16 ui_pad_bits(u32 b) {
    u16 o = 0;
    if (b & IN_JUMP) o |= 0x001;  /* A */
    if (b & IN_TRICK) o |= 0x002; /* B */
    if (b & IN_START) o |= 0x008;
    if (b & IN_RIGHT) o |= 0x010;
    if (b & IN_LEFT) o |= 0x020;
    if (b & IN_UP) o |= 0x040;
    if (b & IN_DOWN) o |= 0x080;
    if (b & IN_BOOST) o |= 0x400; /* X */
    if (b & IN_ALT) o |= 0x800;   /* Y */
    if (b & IN_R) o |= 0x100;     /* R (right trigger) */
    if (b & IN_L) o |= 0x200;     /* L (left trigger) */
    return o;
}

/* ------------------------------------------------------------ BrightnessFade */
typedef struct { u16 flags; s16 bright; s16 step; s16 delay; } FadeWork;
static FadeWork s_fade;
static int s_fade_task;
s16 g_ui_bright_main, g_ui_bright_sub;

static void BrightnessFade_Apply(FadeWork *w) {
    s16 b;
    if (w->bright > 0x1000)
        b = 0x10;
    else
        b = w->bright >> 8;
    if (!(w->flags & 1))
        b = -b;
    if (w->flags & 0x300) {
        if (w->flags & 0x100)
            g_ui_bright_main = b;
        if (w->flags & 0x200)
            g_ui_bright_sub = b;
    } else {
        g_ui_bright_main = b;
        g_ui_bright_sub = b;
    }
    w->flags &= ~0x40; /* GXx_SetMasterBrightness_: the overlay reads the values each frame */
}

/* flags: 1 white (else black), 2 fade in (else out), 4 keep after done,
 * 8 no immediate apply, 0x10 restart level, 0x100/0x200 main/sub only */
void *BrightnessFade_Create(u16 flags, u16 v) {
    FadeWork *w = &s_fade;
    int exists = s_fade_task;
    if (!exists) {
        memset(w, 0, sizeof(*w));
        s_fade_task = 1;
    }
    w->step = (s16)v;
    w->flags = flags;
    w->delay = 0;
    if (!exists || (w->flags & 0x10)) {
        if (w->flags & 2)
            w->bright = 0x1100;
        else
            w->bright = 0;
    }
    if (!(w->flags & 8))
        BrightnessFade_Apply(w);
    return w;
}

void BrightnessFade_Destroy(void) { s_fade_task = 0; }

void BrightnessFade_Main(void) {
    FadeWork *w = &s_fade;
    if (!s_fade_task)
        return;
    if (w->flags & 2) {
        if (w->bright <= 0) {
            w->flags |= 0x20;
            if (!(w->flags & 4)) {
                s_fade_task = 0;
                return;
            }
        }
        if (w->delay != 0)
            w->delay--;
        else
            w->bright -= w->step;
        if (w->bright < 0)
            w->bright = 0;
    } else {
        if (w->bright >= 0x1000) {
            w->flags |= 0x20;
            if (!(w->flags & 4)) {
                s_fade_task = 0;
                return;
            }
        }
        w->bright += w->step;
        if (w->bright > 0x1000)
            w->bright = 0x1000;
    }
    BrightnessFade_Apply(w);
}

int BrightnessFade_IsDone(void) {
    if (!s_fade_task)
        return 1;
    return (s_fade.flags & 0x20) != 0;
}

/* ------------------------------------------------------------------- init */
void ui_set_paths(const char *ui_dir, const char *sprite_dir) {
    if (ui_dir)
        snprintf(g_ui_dir, sizeof(g_ui_dir), "%s", ui_dir);
    if (sprite_dir)
        snprintf(g_ui_sprite_dir, sizeof(g_ui_sprite_dir), "%s", sprite_dir);
}

int ui_init(void) {
    char path[160];
    u8 *b;
    u32 size;
    int err = 0, lang;
    char id[48];
    snprintf(path, sizeof(path), "%s/uidata.bin", g_ui_dir);
    if (ui_load_file(path, &b, &size) == 0) {
        if (ui_data_load(b, size) != 0)
            err = -1;
        free(b);
    } else {
        err = -1;
    }
    snprintf(path, sizeof(path), "%s/navidata.bin", g_ui_dir);
    if (ui_load_file(path, &b, &size) == 0) {
        if (ui_navi_data_load(b, size) != 0)
            err = -1;
        free(b);
    } else {
        err = -1;
    }
    lang = ui_lang();
    /* act_com_lz7.narc members, converted by bac2dc.py under these ids */
    err |= ui_res_load(UI_RES_MOJI, "act__ac_fix_moji");
    err |= ui_res_load(UI_RES_CLDM, "act__dm_cldm");
    err |= ui_res_load(UI_RES_STDM, "act__dm_stdm");
    /* data_02076b74[lang] selects dm_stdm_jpn (3) or dm_stdm_eng (4) */
    snprintf(id, sizeof(id), "act__dm_stdm_%s", (g_ui_data.ok && g_ui_data.tc_lang[lang] == 3) ? "jpn" : "eng");
    err |= ui_res_load(UI_RES_STDM_LANG, id);
    err |= ui_res_load(UI_RES_PAUSE, "act__ac_fix_pause");
    err |= ui_res_load(UI_RES_PAUSE_FRAME, "act__ac_fix_pause_frame");
    err |= ui_res_load(UI_RES_GAMEOVER, "act__ac_fix_gameover");
    /* boss panel (NaviB_Load), boss help, enemy room counter */
    err |= ui_res_load(UI_RES_NAVIB, "act__navi_b");
    err |= ui_res_load(UI_RES_BOSSHELP, "act__ac_fix_bosshelp");
    err |= ui_res_load(UI_RES_ENEMYROOM, "act__ac_eff_enemyroom_fix");
    s_inited = 1;
    return err ? -1 : 0;
}

void ui_shutdown(void) {
    int i;
    for (i = 0; i < UI_RES_COUNT; i++)
        ui_res_free(i);
    s_inited = 0;
}

/* ------------------------------------------------------------ stage entry */
static void Stage_ResetProgress(void) {
    G.fail_flags = 0;           /* data_022c4590.unk24 = 0 */
    g_obj_stage.goal_state = 0; /* data_022c4590.unk40 = 0 */
    W.frame = 0;                /* gGameState.unk20 */
    G.frame24 = 0;              /* gGameState.unk24 */
    W.flags = 0;                /* gGameState.unk18 */
}

/* Player_Create: Init_0234d62c (data_022c4590 reset on stage entry) */
static void Player_Create_InitProgress(void) {
    if (!(G.fail_flags & 2) && !(W.flags & 0x40)) {
        if (!(G.fail_flags & 1)) {
            G.fail_flags &= ~4u;
            if (W.mode == 0)
                W.lives = G.save_lives[G.character == 1 ? 1 : 0]; /* gSaveData.unkE / unk66 */
        }
        G.fail_flags |= 1;
        G.total = 0;
        G.score = 0;
        g_obj_stage.speed_bonus = 0;
        g_obj_stage.bonus_count = 0;
        g_obj_stage.ring_bonus = 0;
        g_obj_stage.check_time = 0;
        G.fail_flags |= 2;
        g_obj_stage.check_idx = 0;  /* data_022c4590.unk15 */
    }
}

/* Stage_Create: the parts that touch the HUD / timer / title card */
static void Stage_Create_UI(void) {
    /* timer reset unless returning from a special stage */
    if (!(W.flags & 0x40) || ((W.flags & 2) && W.mode == 1)) {
        W.frame = 0;
        G.frame24 = 0;
    }
    Player_Create_InitProgress();
    ScorePopup_Destroy();
    EnemyRoomHud_Destroy();
    ui_navi_stage_start();   /* NaviBg_Load / NaviB_Load (Player_LoadArchive, BossStage*_OnModelLoaded) */
    ui_tahud_reset();
    Hud_Create();
    if (!UI_NORMAL_STAGE())
        Hud_SetFlags(Hud_GetFlags() | 4);
    if ((u32)(W.mode - 1) <= 1)
        Hud_SetFlags(Hud_GetFlags() | 0x10);
    /* native layout: the DS has no score counter; in time attack the split-time
     * popup (CheckpointTimePopup) uses its top-right place */
    if (W.mode == 2)
        Hud_SetFlags(Hud_GetFlags() | 0x40000000);
    if (W.flags & 8) {
        /* Stage_Create with gGameState.unk18 & 8 (DemoPlay_Create: a start point): no
         * title card, the timer runs at once; boss acts get the boss panel here */
        if (!UI_NORMAL_STAGE()) {
            NaviMsg_Create();
            NaviMsg_SetVisible(1);
            NaviB_Create();
        }
        W.flags |= 1;
        /* the attract demo starts white (TitleScreen_State_WaitForStart set both
         * screens to 0x10); DemoPlay_Main fades in once the timer runs */
        if (W.flags & 4)
            g_ui_bright_main = g_ui_bright_sub = 16;
    } else {
        StageTitleCard_Create(s_player);
    }
    s_timer = W.frame;
}

void ui_stage_start(int zone, int act, int character) {
    W.zone = (u8)zone;
    W.act = (u8)act;
    G.character = (u32)character;
    {
        /* DemoPlay_Create: Stage_ResetProgress, then gGameState.unk18 |= 4 (| 8 with a
         * start point). demo_start sets them before the stage is built: keep them. */
        u32 demo = W.mode == 3 ? (W.flags & 0xc) : 0;
        Stage_ResetProgress(); /* StageEntry_Start */
        W.flags |= demo;
    }
    G.fail_flags &= ~3u;       /* new stage entry: data_022c4590 reset runs */
    PauseMenu_Destroy();
    ClearDemo_Destroy();
    GameOverSeq_Destroy();
    BrightnessFade_Destroy();
    g_ui_bright_main = g_ui_bright_sub = 0;
    s_state = UI_PLAYING;
    s_exit_kind = UI_EXIT_NONE;
    s_exit_cleared = 0;
    s_pending_exit = -1;
    s_pending_cb = NULL;
    s_pending_restart = 0;
    ui_install_hooks();
    ui_cdbg_prefetch();   /* DC: the results BG, read now (under the loading ring) */
    if (s_player)
        Stage_Create_UI();
    else
        s_pending_restart = 1; /* built on the first ui_post_update, with the player */
}

void ui_bind_player(DcPlayer *p) { s_player = p; }

/* StageRestart_Create -> Stage_Create (gGameState.unk18 |= 2) */
void ui_stage_restart(void) {
    W.flags |= 2;
    PauseMenu_Destroy();
    s_state = UI_PLAYING;
    /* Player_Respawn calls this before it moves the player to the checkpoint:
     * the new stage task list (Stage_Create) is built in ui_post_update */
    s_pending_restart = 1;
}

/* -------------------------------------------------------------- events */
/* Player_AddLives */
void ui_add_lives(s32 n) {
    s32 v;
    if ((u32)(W.mode - 1) <= 1u)
        return;
    v = (u8)(W.lives + n);
    if ((u32)v > 0x63)
        v = 0x63;
    W.lives = v;
    /* Sound_PlayBGMOnPlayer3(0, 0x18): the 1-up jingle (audio module) */
}

void ui_score_follow(const DcPlayer *p, s32 n);
void ui_add_score(s32 x, s32 y, s32 points) {
    ScorePopup_Create(x, y, points);
    G.score += (u32)points;
}

void ui_tension_event(s32 v, s32 mode) { Hud_SetTensionGauge(v, mode); }
void ui_fade(s32 kind, s32 speed) { BrightnessFade_Create((u16)kind, (u16)speed); }
int ui_fade_done(void) { return BrightnessFade_IsDone(); }

/* StageExit_Create(cb, flags): the stage is torn down next frame, then cb runs */
static void StageExit_Create(void (*cb)(void)) {
    s_pending_cb = cb;
    s_pending_exit = 1;
}
void ui_request_exit(int kind) {
    /* the results (ClearDemo) end the stage only after a clear; a time-attack
     * UI_EXIT_TRY_AGAIN also comes from the pause menu and Stage_ExitAfterFail */
    s_exit_cleared = s_state == UI_RESULTS;
    s_exit_kind = kind;
    s_state = UI_EXIT;
}
/* 1 when the stage ended through the act results (a clear: ClearDemo_GoToNextScene),
 * 0 for the pause menu, game over, time over and Stage_ExitAfterFail. Valid with
 * ui_state() == UI_EXIT; main.c passes MENU_EXIT_CLEARED only then. */
int ui_exit_cleared(void) { return s_state == UI_EXIT && s_exit_cleared; }

/* StageShutdown_Create(ResultsScreen_Create / ClearDemo_Create) from the goal */
void ui_stage_exit(s32 kind) {
    if (kind == OBJ_EXIT_RESULTS) {
        ui_request_exit(UI_EXIT_VS_RESULTS);
        return;
    }
    ClearDemo_Create();
    s_state = UI_RESULTS;
}


void Stage_ExitAfterFail(void) {
    switch (W.mode) {
    case 1:
        G.fail_flags &= ~1u;
        G.fail_flags &= ~2u;
        ui_request_exit(UI_EXIT_VS_RESULTS); /* ResultsScreen_Create */
        return;
    case 2:
        ui_request_exit(UI_EXIT_TRY_AGAIN); /* SceneChange_ToTryAgain */
        return;
    case 0: {
        /* SaveData_SetLives(gGameState.unk0, data_022c4590.unk14, 1) */
        u32 v = (u8)W.lives;
        if (v <= 2)
            v = 2;
        G.save_lives[G.character == 1 ? 1 : 0] = (u8)v;
        if (!(G.save_flags & 2) && W.frame > 0x8c9f) {
            GameOverSeq_StartTimeOver();
            s_state = UI_GAMEOVER;
            return;
        }
        GameOverSeq_StartGameOver();
        s_state = UI_GAMEOVER;
        return;
    }
    default:
        GameOverSeq_StartGameOver();
        s_state = UI_GAMEOVER;
        return;
    }
}

/* -------------------------------------------- Player_CheckTimeOver / Player_Last */
static void Player_CheckTimeOver(DcPlayer *w) {
    if (w->pid != 0)   /* unk325: second player (VS) */
        return;
    if (W.mode == 1) {
        if (W.frame >= 0x8c9f && !(w->pf & DCP_PF_GOAL)) {
            BrightnessFade_Create(4, 0x80);
            /* Player_Action_ReachGoal(w, 0): the goal action belongs to the
             * player/objects modules; the VS exit follows in Player_Last */
            g_obj_stage.goal_state = 3;
        }
    } else if (!(G.save_flags & 2) || W.mode == 2) {
        if (W.frame >= 0x8c9f) {
            g_obj_stage.check_time = 0;   /* data_022c4590.unk18 */
            G.fail_flags |= 4;            /* data_022c4590.unk24 |= 4: no time bonus */
            dcp_act_die(w);               /* Player_Action_Die (ignored once dead) */
        }
    }
    if (!(W.flags & 1))
        return;
    if (W.frame < 0x87ef)
        return;
    /* last 10 seconds: Sound_PlaySeqArcEx(NULL, 0, 0x36) on frames 0x87ef, 0x8867, .. 0x8c27 (every 0x78) */
    if (W.frame <= 0x8c27 && (W.frame - 0x87ef) % 0x78 == 0 && g_ui_hooks.sfx)
        g_ui_hooks.sfx(g_ui_hooks.ctx, 0, 0x36);
}

static void Player_Last_Exits(DcPlayer *w) {
    if (W.mode == 1 && w->pid == 0 && W.frame >= 0x8c9f && BrightnessFade_IsDone() &&
        g_obj_stage.goal_state >= 3) {
        Stage_ExitAfterFail(); /* StageShutdown_Create(Stage_ExitAfterFail) */
        return;
    }
    if (w->pf & DCP_PF_GAMEOVER) {
        if (s_pending_exit < 0)
            StageExit_Create(Stage_ExitAfterFail);
        return;
    }
    /* 0x40000000: special stage entry (Player_ExitToSpecialStage) - not part of this module */
}

/* ------------------------------------------------------------ frame */
void ui_update(const InputState *in, DcPlayer *p) {
    u16 trig = ui_pad_bits(in->pressed);
    g_ui_trig = trig;
    g_ui_held = ui_pad_bits(in->held);
    s_player = p;
    ui_cmd_reset();
    if (s_state == UI_EXIT)
        return;
    if (s_pending_exit >= 0) {
        /* StageExit_Main: tasks destroyed, then the callback */
        void (*cb)(void) = s_pending_cb;
        s_pending_exit = -1;
        s_pending_cb = NULL;
        PauseMenu_Destroy();
        StageTitleCard_Destroy();
        Hud_Destroy();
        ScorePopup_Destroy();
        EnemyRoomHud_Destroy();
        NaviB_Destroy();      /* Stage_FreeZoneFiles: NaviMsg_CloseFiles, NaviB_Release */
        NaviMsg_Destroy();
        BossHelp_Destroy();
        if (cb)
            cb();
    }
    if (s_state == UI_GAMEOVER) {
        g_ui_layer = UI_LAYER_GAMEOVER;
        GameOverSeq_Main(in);
        BrightnessFade_Main();
        if (!GameOverSeq_IsActive())
            ui_request_exit(UI_EXIT_GAME_OVER); /* SceneChange_ToSegaLogo */
        return;
    }
    if (s_state == UI_RESULTS) {
        g_ui_layer = UI_LAYER_RESULTS;
        ClearDemo_Main(in);
        return;
    }
    if (PauseMenu_IsActive()) {
        g_ui_layer = UI_LAYER_PAUSE;
        PauseMenu_Main(in);
        return;
    }
    /* Stage_Main */
    Hud_SetLifeCount((u8)W.lives);
    Hud_SetRingCount(p ? p->rings : 0);
    Hud_SetTime((s32)W.frame);
    if (W.flags & 1)
        W.frame++;
    G.frame24++;
    if (p) {
        Hud_SetTensionGauge((u16)(p->tension >> 4), 0);
        Hud_SetTensionMax(p->tension_max);
    }
    Hud_SetScore(G.score);
    s_timer = W.frame;
    if (trig & 8)
        PauseMenu_Create(0);
}

void ui_post_update(DcPlayer *p) {
    if (s_state == UI_EXIT || s_state == UI_GAMEOVER || s_state == UI_RESULTS) {
        if (s_state == UI_RESULTS)
            BrightnessFade_Main();
        return;
    }
    if (s_pending_restart) {
        s_pending_restart = 0;
        if (p)
            s_player = p;
        Stage_Create_UI();
    }
    if (!PauseMenu_IsActive()) {
        W.frame = s_timer; /* see the timer note at the top */
        if (p) {
            Player_CheckTimeOver(p);
            Player_Last_Exits(p);
            /* Player_In: Hud_SetFlags(... | 0x20) while the tension is at max */
            if (p->tension_max != 0)
                Hud_SetFlags(Hud_GetFlags() | 0x20);
            else
                Hud_SetFlags(Hud_GetFlags() & ~0x20);
        }
        g_ui_layer = UI_LAYER_POPUP;
        ScorePopup_Main();
    }
    /* System_Freeze keeps the HUD task's last sprites on screen; the fade task
     * runs while frozen (PauseMenu_Main_Quit waits for it) */
    g_ui_layer = UI_LAYER_HUD;
    Hud_Main();
    /* EnemyRoomHud (StageObj task 0xa00), NaviMsg / NaviB (sub screen), BossHelp:
     * frozen while paused, still drawn */
    /* CheckpointTimePopup / MarkerTimeList (StageObj tasks 0xff) */
    ui_tahud_frame(!PauseMenu_IsActive());
    g_ui_layer = UI_LAYER_NAVI;
    EnemyRoomHud_Frame(!PauseMenu_IsActive(), p);
    ui_navi_frame(PauseMenu_IsActive());
    BrightnessFade_Main();
    if (!PauseMenu_IsActive()) {
        g_ui_layer = UI_LAYER_TITLE;
        StageTitleCard_Main();
        if (p && StageTitleCard_IsActive())
            StageTitleCard_HoldPlayer(p);
    }
    /* DemoPlay_Main's label (demo module, ui_demo_label) */
    g_ui_layer = UI_LAYER_GAMEOVER;
    ui_demo_label_draw();
}

void ui_player_last(DcPlayer *p) { ui_post_update(p); }

int ui_is_paused(void) { return PauseMenu_IsActive(); }
int ui_world_frozen(void) { return s_state != UI_PLAYING; }
int ui_state(void) { return s_state; }
int ui_exit_kind(void) { return s_exit_kind; }
int ui_titlecard_active(void) { return StageTitleCard_IsActive(); }

/* -------------------------------------------------------------- hooks */
static DcpHooks s_prev_dcp;
static ObjHooks s_prev_obj;

static void h_score(void *ctx, DcPlayer *p, s32 points) {
    /* Player_AddTrickScore: ScorePopup_Create(w, a); data_022c4590.unk4 += a */
    ui_score_follow(p, points);
    G.score += (u32)points;
    if (s_prev_dcp.score)
        s_prev_dcp.score(ctx, p, points);
}
static dcp_bool h_score_active(void *ctx) {
    (void)ctx;
    return ScorePopup_IsActive() ? DCP_TRUE : DCP_FALSE;
}
static void h_add_life(void *ctx, DcPlayer *p, s32 n) {
    ui_add_lives(n);
    if (s_prev_dcp.add_life)
        s_prev_dcp.add_life(ctx, p, n);
}
/* every player call site passes mode 1 (Player_StartBoost, _DrainTension, _OnHurt) */
static void h_tension(void *ctx, DcPlayer *p, s32 v) {
    Hud_SetTensionGauge(v, 1);
    if (s_prev_dcp.tension_hud)
        s_prev_dcp.tension_hud(ctx, p, v);
}
static void h_fade(void *ctx, s32 kind, s32 speed) {
    (void)ctx;
    BrightnessFade_Create((u16)kind, (u16)speed);
}
static dcp_bool h_fade_done(void *ctx) {
    (void)ctx;
    return BrightnessFade_IsDone() ? DCP_TRUE : DCP_FALSE;
}
/* Player_Respawn: StageRestart_Create in story/time attack, Camera_ResetOnRespawn in VS */
static void h_restart(void *ctx, DcPlayer *p) {
    if (W.mode != 1 && !(p->pf2 & 0x400000))
        ui_stage_restart();
    if (s_prev_dcp.restart)
        s_prev_dcp.restart(ctx, p);
}
static void h_obj_fade(void *ctx, s32 kind, s32 speed) {
    BrightnessFade_Create((u16)kind, (u16)speed);
    if (s_prev_obj.fade)
        s_prev_obj.fade(ctx, kind, speed);
}
static void h_stage_exit(void *ctx, s32 kind) {
    ui_stage_exit(kind);
    if (s_prev_obj.stage_exit)
        s_prev_obj.stage_exit(ctx, kind);
}

/* g_obj_hooks.checkpoint_popup: CheckpointTimePopup_Create(p, time) */
static void h_checkpoint_popup(void *ctx, DcPlayer *p, u32 time) {
    CheckpointTimePopup_Create(0, (s32)time);
    if (s_prev_obj.checkpoint_popup)
        s_prev_obj.checkpoint_popup(ctx, p, time);
}

void ui_install_hooks(void) {
    if (g_dcp_hooks.score != h_score) {
        s_prev_dcp = g_dcp_hooks;
        g_dcp_hooks.score = h_score;
        g_dcp_hooks.score_active = h_score_active;
        g_dcp_hooks.add_life = h_add_life;
        g_dcp_hooks.tension_hud = h_tension;
        g_dcp_hooks.fade = h_fade;
        g_dcp_hooks.fade_done = h_fade_done;
        g_dcp_hooks.restart = h_restart;
    }
    if (g_obj_hooks.stage_exit != h_stage_exit) {
        /* The objects module never restores g_obj_hooks, and main.c wraps stage_exit
         * after ui_stage_start (its special-stage hook chains to ours). Taking the
         * current hooks as "previous" again on the next stage would make the two
         * handlers call each other forever at the goal (2nd act of a session: hang).
         * So the previous hooks are the ones found the first time only. */
        static int s_obj_prev_taken;
        if (!s_obj_prev_taken) {
            s_prev_obj = g_obj_hooks;
            s_obj_prev_taken = 1;
        }
        g_obj_hooks.fade = h_obj_fade;
        g_obj_hooks.stage_exit = h_stage_exit;
        g_obj_hooks.checkpoint_popup = h_checkpoint_popup;
    }
}

/* Battle Play: the race start is the original title card (the VS card, "Ready? / GO!") over the
 * whole screen. Once per frame in the TR list, after the viewports; 1 while it runs. The race
 * starts at GO (ui_titlecard_started). */
int ui_vs_titlecard_draw(void) {
    if (!StageTitleCard_IsActive())
        return 0;
    ui_cmd_reset();
    g_ui_layer = UI_LAYER_TITLE;
    StageTitleCard_Main();
    BrightnessFade_Main();
    ui_draw_tr();
    ui_cmd_reset();
    return 1;
}

