/*
 * Battle Play rules for two players simulated on one Dreamcast (vs.c).
 *
 * On the DS each console ran its own player and drew the rival from wireless state
 * (Player_ApplyNetState); rival events (Stage_NetProcessRivalEvents: 7 slowdown, 8 confusion,
 * 9 tension drain, 10 warp to rival; 3 goal) went over the air. Here both players run the full
 * player code (both pid 0 = "local") and the events become direct calls:
 *   ItemBox_GiveItem 8..11        -> the opponent: Player_ApplySlowdown / _ApplyConfusion /
 *                                    _DrainTension / _WarpToRival (+ EffVs* effects)
 *   coll2F8 / Player_OnPushRival  -> vs_after_players: each player is pushed by the other's push box
 *   Player_UpdateBoost (pf 0x20000) + Player_ApplyNetState -> the knock-back of the rival
 *   StageClear_HandleGoalReached / Player_Action_ReachGoal (data_022c4590.unk40) -> race state
 *   Player_CheckTimeOver (VS branch), Player_Last (VS exit), Player_Respawn (no lives, own checkpoint)
 */
#ifndef SRDC_VS_H
#define SRDC_VS_H
#include "../player/dcp_player.h"

enum { VS_RACING = 0, VS_WON = 1, VS_LOST = 2, VS_TIME_OVER = 3 };   /* data_022c4590.unk40 */
enum { VS_EXIT_NONE = 0, VS_EXIT_RESULTS, VS_EXIT_QUIT };

typedef struct VsInfo {
    int winner;            /* 0 / 1 = player, 2 = draw (time over), -1 = not decided */
    int finished[2];
    unsigned time[2];      /* stage frames at the goal */
    int exit;              /* VS_EXIT_* requested */
} VsInfo;

/* start of a race: p[0] / p[1] created, objects_init done, before camera_create */
void vs_begin(DcPlayer *p0, DcPlayer *p1);
void vs_end(void);                       /* hooks restored */
int vs_active(void);
/* around each player's frame: the player-specific world state (checkpoint) */
void vs_player_begin(int i);
void vs_player_end(int i);
/* the input the player gets this frame (Player_Action_ReachGoal: the goal run / no input) */
void vs_filter_input(int i, unsigned short *held, unsigned short *pressed, unsigned short *released);
/* after objects_update: pushes, knock-backs, checkpoints, the race clock rules */
void vs_after_players(void);
/* once per frame at the end: TensionSound countdowns, fades */
void vs_frame_end(void);
const VsInfo *vs_info(void);
/* the viewport's fade (death / warp BrightnessFade, goal fade): 0 .. 16 (black) */
int vs_fade_level(int i);
void vs_request_exit(int kind);

#endif
