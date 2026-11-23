/*
 * Native in-stage presentation and flow (Dreamcast port of Sonic Rush) - public API.
 *
 * Translated from the decompiled DS code (see docs/dreamcast/translation/UI.md):
 *   src/game/UI/Hud (Hud_*, ScorePopup_*), src/game/UI/TitleCard (StageTitleCard*),
 *   src/game/UI/GameOver (GameOverSeq_*, GameOver_*), src/game/Menus/Pause (PauseMenu_*),
 *   src/game/Cutscenes/ClearDemo (act results: ClearDemo*, ClearDemoScore_*),
 *   src/game/Graphics/Screen (BrightnessFade_*), and the score/time/lives parts of
 *   Stage_Main, Stage_ResetProgress, Player_AddLives, Player_AddTrickScore,
 *   Player_CheckTimeOver, Player_Last (game over / time over exit), Stage_ExitAfterFail.
 *
 * Frame order (main.c):
 *
 *     input_poll(&in);
 *     ui_update(&in, &player);        // Stage_Main (HUD feed, timer, START -> pause),
 *                                     // pause menu / results / game over tasks
 *     if (!ui_is_paused() && !ui_world_frozen()) {
 *         objects_frame_begin(); dcp_player_frame(...); objects_update(&player);
 *     }
 *     ui_post_update(&player);        // every frame: time over, Player_Last exits,
 *                                     // ScorePopup, HUD, fade, title card
 *     ... render OP/PT lists ...
 *     render_begin_list(PVR_LIST_TR_POLY); ...; ui_draw_tr(); render_end_list();
 *
 * Game state: gGameState / data_022c4590 fields live in g_dcp_world (zone, act, mode,
 * flags, frame = stage timer, lives) and g_obj_stage (speed/ring bonus), plus UiGame
 * below for the fields no other module owns (score, character, total, fail flags).
 */
#ifndef SRDC_UI_H
#define SRDC_UI_H

#include <stdint.h>
#include "../player/dcp_player.h"
#include "../engine/input.h"

/* ui_state() */
enum {
    UI_PLAYING = 0,   /* stage running (title card may be on screen)             */
    UI_RESULTS = 1,   /* act clear tally (ClearDemo) on screen                    */
    UI_GAMEOVER = 2,  /* GAME OVER / TIME'S UP sequence on screen                 */
    UI_EXIT = 3       /* stage finished: read ui_exit_kind(), load the next scene */
};
/* ui_exit_kind(): which scene the original would change to */
enum {
    UI_EXIT_NONE = 0,
    UI_EXIT_NEXT_ACT = 1,    /* ClearDemo_GoToNextScene (act clear)                 */
    UI_EXIT_QUIT = 2,        /* PauseMenu "GO BACK"/"QUIT": PauseMenu_QuitToMap     */
    UI_EXIT_GAME_OVER = 3,   /* GameOverSeq_Main -> SceneChange_ToSegaLogo          */
    UI_EXIT_TRY_AGAIN = 4,   /* time attack: SceneChange_ToTryAgain                 */
    UI_EXIT_VS_RESULTS = 5   /* VS mode: ResultsScreen_Create (not translated here) */
};
/* gGameState.unk10 */
enum { UI_MODE_STORY = 0, UI_MODE_VS = 1, UI_MODE_TIME_ATTACK = 2 };

typedef struct UiGame {
    u32 character;      /* gGameState.unk0  (0 Sonic, 1 Blaze)                          */
    u32 total;          /* data_022c4590.unk0  (tally total, written by the results)    */
    u32 score;          /* data_022c4590.unk4  (trick / popup score)                    */
    u32 fail_flags;     /* data_022c4590.unk24 (1/2: progress initialised, 4: time over) */
    u32 frame24;        /* gGameState.unk24 (frames since stage start)                  */
    u32 save_flags;     /* gSaveData.unk8 (bit 1: no time limit option)                 */
    u8 stage_new;       /* StoryProgress_IsStageNew(): 1 = pause menu shows no entries  */
    u8 language;        /* gGameLanguage (0 jpn, 1 eng, 2 fra, 3 deu, 4 ita, 5 spa: data_0207b9bc) */
    u8 save_lives[2];   /* gSaveData.unkE / unk66: lives restored on stage entry.
                           Native default 3 (the save-file default was not traced). */
    u8 difficulty;      /* gGameState.unk14 (1 = normal: 8 boss hit points, NaviB gauge 0x1a) */
    u16 save_progress[2]; /* gSaveData.unkC / unk64 (story progress): NaviMsg_Create shows no
                           navigator before Sonic 3 / Blaze 4 (and Blaze 0x1b). Default 0xffff */
} UiGame;
extern UiGame g_ui_game;

/* Where uidata.bin and the converted sprite directories live
 * (defaults "/cd/ui" and "/cd/sprites"). Call before ui_init. */
void ui_set_paths(const char *ui_dir, const char *sprite_dir);
/* Load uidata.bin and the UI sprite resources (atlases go to PVR VRAM on KOS).
 * Returns 0 when everything loaded; the module still runs (draws nothing missing)
 * otherwise. Call once. */
int ui_init(void);
void ui_shutdown(void);

/* gPlayers[0]: the player the HUD, title card and timer rules read. */
void ui_bind_player(DcPlayer *p);
/* Stage_Create / Stage_ResetProgress / Player_Create part: reset timer, flags and
 * score, set zone/act/character, create the HUD and the stage title card.
 * zone/act are gGameState.unk1C/unk1D (Leaf Storm = zone 0; act 0, 1, boss 2).
 * Call after dcp_player_create and objects_init (it installs hooks). */
void ui_stage_start(int zone, int act, int character);
/* Same, after a death: Player_Respawn keeps time/score; the DS restarts the stage
 * task list (StageRestart) without a new title card in story mode. */
void ui_stage_restart(void);

/* One DS frame of the presentation (title card, HUD, pause, results, game over,
 * fades). input = this frame's pad state. */
void ui_update(const InputState *in, DcPlayer *player);
/* The UI tasks that run after the world on the DS (Player_CheckTimeOver,
 * Player_Last exits, ScorePopup, Hud_Main, BrightnessFade, StageTitleCard).
 * Call every frame after the world update, also while paused. */
void ui_post_update(DcPlayer *player);
void ui_player_last(DcPlayer *player);   /* = ui_post_update */
/* Screen-space quads for everything above, into the open TR list (KOS only; the
 * host build records the quads for tests). */
void ui_draw_tr(void);
/* Stage teardown (with objects_shutdown / boss_shutdown): frees the stage's own UI
 * resources (title card / results backgrounds, navigator, face texture) so the PVR
 * memory returns to what ui_init holds. */
void ui_stage_end(void);
/* KOS: PVR texture bytes the UI sprite pages hold (VRAM budget probe) */
uint32_t ui_vram_used(void);

int ui_is_paused(void);       /* pause menu open: skip the world update           */
int ui_world_frozen(void);    /* results / game over / exit: skip the world update */
int ui_state(void);           /* UI_PLAYING / UI_RESULTS / UI_GAMEOVER / UI_EXIT   */
int ui_exit_kind(void);       /* valid when ui_state() == UI_EXIT                  */
int ui_titlecard_active(void);
/* 1 once the title card ran StageTitleCard_State_StartStage (also after it ended): in boss acts the
 * DS sets gBossStatus.flags |= 1 there, so the boss (and the arena camera of BossF) starts at "GO!",
 * under the white flash, not when the title card task ends ~62 frames later */
int ui_titlecard_started(void);
/* StageTitleCardPlayer_Draw: 1 while the title card's close-up of the player model replaces
 * the normal player draw; speed = per-slot animation speed (fx32), eye/at = LookAt (fx32 DS
 * units, y up), x/y = model translation (20.12), zoom = off-axis frustum phase */
int ui_titlecard_player_draw(s32 *speed, s32 eye[3], s32 at[3], s32 *x, s32 *y, int *zoom);
/* depth band for that close-up (PVR 1/w): above the title card BG quads (z 100 + 0.01 per quad),
 * below the title card sprites, which ui_draw_tr moves up by UI_TC_GAP while the close-up shows */
#define UI_TC_GAP 2000.0f
#define UI_TC_MODEL_Z0 400.0f
#define UI_TC_MODEL_Z1 1900.0f
/* ClearDemo_DrawModel (act / boss results): 1 while the results show the player's model.
 * pos = translation (DS screen px, 20.12, y up), scale (fx32), rot_y (DS angle), jnt / vis =
 * animation indices into the player's joint (son.dca) / visibility (son.dcv) sets, frame = fx32
 * frame (clamp to the last frame: the animations do not loop).  Camera: LookAt (128, -134.4,
 * 96 cot 0x1555) -> (128, -96, 0), up (0, 1, 0), perspective half-fovy 0x1555, aspect 4:3, near 1,
 * far 2048.  Lights (BGR555, DS vectors): 0x6f7b (0, -0x4a0, -0xf40), 0x2108 (0xec7, -0x600, 0),
 * 0x2108 (0, 0x620, 0xec7).  Depth band (UI_TC_MODEL_Z0, UI_TC_MODEL_Z1): above the results
 * background, below the tally sprites (ui_draw_tr moves the results layer up by UI_TC_GAP while
 * this returns 1). */
int ui_results_model(s32 pos[3], s32 *scale, u16 *rot_y, s32 *jnt, s32 *vis, s32 *frame);

/* ----------------------------------------------------- event hooks */
/* ScorePopup_Create(obj, n) + data_022c4590.unk4 += n (enemy kill, trick, ...).
 * x/y = 24.8 world position the popup follows (the object). */
void ui_add_score(s32 x, s32 y, s32 points);
/* Player_AddLives */
void ui_add_lives(s32 n);
/* Hud_SetTensionGauge(v, mode) (Player_StartBoost / DrainTension / OnHurt pass mode 1) */
void ui_tension_event(s32 v, s32 mode);
/* BrightnessFade_Create / BrightnessFade_IsDone (the player/object fade hooks) */
void ui_fade(s32 kind, s32 speed);
int ui_fade_done(void);
/* StageShutdown_Create(...) from the goal: OBJ_EXIT_RESULTS / OBJ_EXIT_CLEAR_DEMO */
void ui_stage_exit(s32 kind);

/* Sound requests (all optional). arc/id are the DS sound archive group / sequence
 * numbers of the original calls (see UI.md for the list). */
typedef struct UiHooks {
    void *ctx;
    void (*sfx)(void *ctx, s32 arc, s32 id);   /* Sound_PlaySeqArc / _PlaySeqArcEx */
    void (*voice)(void *ctx, s32 arc, s32 id); /* Sound_PlayVoice */
    void (*music)(void *ctx, s32 id);          /* Sound_PlaySeq(0, id); -1 = NNS_SndStopSoundAll */
    void (*music_pause)(void *ctx, int on);    /* pause menu: NNS_SndPlayerPauseByPlayerNo(3..9) */
    /* StoryProgress_IsStageNew (menus module), read by PauseMenu_Create. NULL: g_ui_game.stage_new */
    int (*stage_new)(void *ctx);
} UiHooks;
extern UiHooks g_ui_hooks;

/* Install the UI's implementations into g_dcp_hooks (score, score_active,
 * add_life, tension_hud, fade, fade_done, restart) and g_obj_hooks (fade,
 * stage_exit), chaining to the previous ones. ui_stage_start calls it; call it
 * again after objects_init if objects_shutdown restored older hooks. */
void ui_install_hooks(void);

/* HUD layout (DC redesign, see UI.md): integer scale of the HUD art and the
 * 4:3 safe area (default 2x, safe area 32,24 .. 608,456). */
void ui_hud_layout(int scale, int safe_x0, int safe_y0, int safe_x1, int safe_y1);
/* ScorePopups are drawn in world space: set the camera (top-left of the 256x192
 * world window, world px) once per frame before ui_draw_tr (main.c: g_cam_x/y). */
void ui_set_camera(float x, float y);

/* ------------------------------------------- boss stages (ui_navi.c) */
/* gBossStatus.unk8: the boss hit points NaviB shows. Call every frame of a boss
 * stage before ui_update (main.c: ui_set_boss_health(boss_health())). */
void ui_set_boss_health(s32 health);
/* g_boss_hooks.navi: NaviMsg_SetFaceAnim(face) + NaviMsg_ShowMessage(msg, time).
 * Assign it directly: g_boss_hooks.navi = ui_boss_navi. */
void ui_boss_navi(void *ctx, s32 face, s32 msg, s32 time);
/* Player_OnHurt, boss stages: NaviMsg_SetFaceAnim(0xc); NaviMsg_ShowMessage(0x11, 0x96).
 * The player module has no hook for it: call after dcp_on_hurt (see UI.md). */
void ui_navi_player_hurt(void);
/* BossHelp_Create(idx) / BossHelp_Destroy (Boss6_StartTiltCamera, Boss7_State_ClashStart,
 * BossFPartD8_State_HandRideStartWarn: idx = ac_fix_bosshelp animation 0 / 1) */
void BossHelp_Create(u32 idx);
void BossHelp_Destroy(void);
/* NaviMsg_IsVisible: the navigator message box is on screen */
int ui_navi_message_visible(void);

/* -------------------------------------- enemy rooms (ui_enemyroom.c) */
/* OBJ_FX_ENEMY_ROOM (g_obj_hooks.effect, kind OBJ_FX_ENEMY_ROOM, a, b):
 *   a = 0: EnemyRoomHud_Create, b = enemies to defeat
 *   a = 1: count, b = enemies defeated so far
 *   a = 2: room cleared (b = defeated)
 * main.c calls it from its g_obj_hooks.effect handler (the UI does not install that hook). */
void ui_enemy_room(s32 a, s32 b);
/* EnemyRoom_GetActive(1)->unk20C & 1 (the finish sprites swap sides); default 0 */
void ui_enemy_room_flags(u32 f20c);

/* ------------------------------------- time attack, attract demo (ui_tahud.c) */
/* CheckpointTimePopup_Create(p, time): ui_install_hooks installs it as
 * g_obj_hooks.checkpoint_popup (Marker_OnHit in time attack, pass-check laps). */
void ui_checkpoint_popup(u32 time);
/* Marker_Action_InitPassCheck: MarkerTimeList_Create (create 1) / destroy (0);
 * Marker_State_WaitPlayerPass: CheckpointTimePopup_SetTimeDigits(lap, time).
 * The objects module has no hook for these two yet (MENUS.md 9.4). */
void ui_marker_time_list(int create);
void ui_marker_time_set(int lap, u32 time);
/* DemoPlay_Main's blinking "DEMO PLAY" / "PRESS START" label (demo module): 1 shown,
 * 0 hidden, -1 released (DemoPlay_Destructor) */
void ui_demo_label(int visible);
int ui_demo_label_shown(void);

/* ------------------------------------------------ host test access */
typedef struct UiQuad {
    float x, y, w, h;      /* 640x480 screen */
    float u0, v0, u1, v1;  /* texels (flipped when u0 > u1) */
    u32 argb;
    int tex;               /* resource index, -1 flat */
    int image;
    int variant;
    int layer;
    int screen;            /* 0 DS main screen, 1 composed in from the sub screen */
} UiQuad;
int ui_debug_quads(const UiQuad **out);
#endif
