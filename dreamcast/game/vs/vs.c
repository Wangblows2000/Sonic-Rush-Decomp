/*
 * Battle Play rules for two locally simulated players (see vs.h). Translated from
 * Stage_NetProcessRivalEvents, ItemBox_GiveItem (8..11), Player_ApplySlowdown,
 * Player_ApplyConfusion, Player_DrainTension, Player_WarpToRival, Player_OnPushRival,
 * Player_ApplyNetState (knock-back part), Player_Action_ReachGoal, StageClear_HandleGoalReached
 * (VS part), Player_CheckTimeOver / Player_Last (VS branches), TensionSound_Create / _Main.
 */
#include <string.h>
#include "vs.h"
#include "../objects/objects.h"
#include "../camera/camera.h"

#define W g_dcp_world

static struct {
    int on;
    DcPlayer *p[2];
    int cur;                       /* the player whose frame runs (-1: none) */
    int race[2];                   /* data_022c4590.unk40 as each console saw it */
    VsInfo info;
    s32 check_x[2], check_y[2];    /* data_022c4590.unk1C / unk20 of each console */
    int fade[2], fade_target[2], fade_step[2]; /* BrightnessFade of each viewport, 0..16 << 8 */
    int tsnd_t[2];                 /* TensionSound_Main countdown (-1: none) */
    int timeover_fade;             /* the time-over BrightnessFade_Create(4, 0x80) */
    DcpHooks saved_dcp;
    ObjHooks saved_obj;
} V;
static int s_tsnd_owner[2];        /* owners of the TensionSound handles (g_obj_hooks.sfx_own) */

int vs_active(void) { return V.on; }
const VsInfo *vs_info(void) { return &V.info; }
void vs_request_exit(int kind) { if (V.info.exit == VS_EXIT_NONE) V.info.exit = kind; }

static int idx_of(const DcPlayer *p) { return p == V.p[1] ? 1 : 0; }
static DcPlayer *rival_of(const DcPlayer *p) { return V.p[1 - idx_of(p)]; }

/* ------------------------------------------------------------ fades (per viewport) */
/* BrightnessFade_Create(kind, speed): 4 = to black, 2 = back from black; speed 0x100 = one step
 * of 16 per frame; 0x80 half that */
static void fade_start(int i, s32 kind, s32 speed) {
    V.fade_target[i] = kind == 4 ? 16 << 8 : 0;
    V.fade_step[i] = speed > 0 ? (int)speed : 0x100;
}
static int fade_done(int i) { return V.fade[i] == V.fade_target[i]; }
int vs_fade_level(int i) { return V.fade[i & 1] >> 8; }

/* ------------------------------------------------------------ TensionSound_Create(id, frames) */
static void tension_sound(int i, s32 id, int frames) {
    if (V.tsnd_t[i] >= 0 && g_obj_hooks.sfx_own_stop)
        g_obj_hooks.sfx_own_stop(g_obj_hooks.ctx, &s_tsnd_owner[i], 0x20);
    if (g_obj_hooks.sfx_own)
        g_obj_hooks.sfx_own(g_obj_hooks.ctx, &s_tsnd_owner[i], id, -1);
    V.tsnd_t[i] = frames;
}

/* ------------------------------------------------------------ hooks */
/* Player_AddTension (VS, +0x12c0) / Player_DrainTension: the sound and effect */
static void h_vs_tension(void *ctx, DcPlayer *p, s32 max) {
    (void)ctx;
    if (max) {
        tension_sound(idx_of(p), 0x2f, 0x80);
        EffVsTensionMax_Create(p);
    } else {
        tension_sound(idx_of(p), 0x32, 0x80);
        EffVsTensionDrain_Create(p);
    }
}

/* Player_WarpToRival(w): w is sent to the rival who took the Attract item */
static void warp_to_rival(DcPlayer *w, const DcPlayer *rival) {
    if (w->pf & 0x1000000)
        return;
    if (rival->pf2 & 0x800000)
        w->obj_flags |= 1;
    else
        w->obj_flags &= ~1u;
    w->warp_x = rival->x;
    w->warp_y = rival->y;
    dcp_act_warp(w);
}

/* ItemBox_GiveItem 8..11 (the collector's part ran in the objects module: sound 0xdf, +0x12c0 for
 * 10); here the rival's console part (Stage_NetProcessRivalEvents 7..10) */
static void h_item(void *ctx, DcPlayer *p, s32 k) {
    DcPlayer *o;
    if (V.saved_obj.item)
        V.saved_obj.item(ctx, p, k);
    if (k < OBJ_ITEM_VS_SLOW || k > OBJ_ITEM_VS_11)
        return;
    o = rival_of(p);
    if (o == NULL || o == p)
        return;
    switch (k) {
    case OBJ_ITEM_VS_SLOW: /* Player_ApplySlowdown */
        EffVsSlowdown_Create(o);
        o->slowdown = 0x200;
        break;
    case OBJ_ITEM_VS_CONFUSE: /* Player_ApplyConfusion */
        EffVsConfusion_Create(o);
        o->confuse = 0x258;
        break;
    case OBJ_ITEM_VS_DRAIN: /* Player_DrainTension */
        dcp_drain_tension(o);
        break;
    case OBJ_ITEM_VS_11: /* Player_WarpToRival */
        warp_to_rival(o, p);
        break;
    }
}

/* Player_Action_ReachGoal */
static void reach_goal(DcPlayer *w, void *goal) {
    int ok = 0;
    if (w->pf & 0x200000)
        return;
    dcp_reset_gimmick_state(w);
    w->gimmick = goal;
    w->pf2 |= 0x30;
    w->pf |= 0x200000 | 0x400000 | 0x1000000;
    if (W.zone >= 7 && W.zone < 9 && W.act < 2)
        ok = 1;
    if (!ok) {
        w->obj_flags |= 2;
    } else {
        w->gimmick = NULL;
        w->pf2 &= ~0x30u;
    }
    w->tension_max = 0;
    w->invinc = 0;
}

/* StageClear_HandleGoalReached (VS part) through g_obj_hooks.goal: the first player at the goal wins;
 * the rival's console saw the puppet's ReachGoal: Player_Action_ReachGoal's VS tail */
static void h_goal(void *ctx, DcPlayer *p) {
    int i = idx_of(p);
    DcPlayer *o = rival_of(p);
    (void)ctx;
    if (V.info.finished[i])
        return;
    reach_goal(p, NULL);
    V.info.finished[i] = 1;
    V.info.time[i] = W.frame;
    if (V.info.winner < 0) {
        V.info.winner = i;
        V.race[i] = VS_WON;                  /* unk40 = 1 (local), event 3 to the rival */
        V.race[1 - i] = VS_LOST;             /* unk40 = 2 (p != gPlayers) */
        if (o != NULL) {                     /* the rival's gPlayers[0] */
            o->pf |= 0x400000 | 0x1000000;
            o->tension_max = 0;
            o->invinc = 0;
        }
    }
}

static void h_stage_exit(void *ctx, s32 kind) {
    (void)ctx;
    if (kind == OBJ_EXIT_RESULTS || kind == OBJ_EXIT_CLEAR_DEMO)
        vs_request_exit(VS_EXIT_RESULTS);    /* GoalRing_State_Idle: StageShutdown_Create(ResultsScreen_Create) */
}
/* objects: BrightnessFade_Create (goal ring): both viewports */
static void h_obj_fade(void *ctx, s32 kind, s32 speed) {
    (void)ctx;
    fade_start(0, kind, speed);
    fade_start(1, kind, speed);
}
/* the player module (death / respawn): the viewport of the player whose frame runs */
static void h_dcp_fade(void *ctx, s32 kind, s32 speed) {
    (void)ctx;
    if (V.cur >= 0)
        fade_start(V.cur, kind, speed);
    else
        h_obj_fade(ctx, kind, speed);
}
static dcp_bool h_dcp_fade_done(void *ctx) {
    (void)ctx;
    return V.cur >= 0 ? (dcp_bool)fade_done(V.cur) : (dcp_bool)(fade_done(0) && fade_done(1));
}
/* Player_Respawn, VS: Camera_ResetOnRespawn, no StageRestart (the camera module chains in front) */
static void h_restart(void *ctx, DcPlayer *p) {
    (void)ctx;
    Camera_ResetOnRespawn();   /* the player's camera is the one loaded (main.c) */
    p->pf2 |= 0x200000;
}

/* ------------------------------------------------------------ race */
void vs_begin(DcPlayer *p0, DcPlayer *p1) {
    memset(&V, 0, sizeof V);
    V.p[0] = p0;
    V.p[1] = p1;
    V.cur = -1;
    V.info.winner = -1;
    V.tsnd_t[0] = V.tsnd_t[1] = -1;
    V.check_x[0] = V.check_x[1] = W.check_x;
    V.check_y[0] = V.check_y[1] = W.check_y;
    V.saved_dcp = g_dcp_hooks;
    V.saved_obj = g_obj_hooks;
    g_dcp_hooks.vs_tension = h_vs_tension;
    g_dcp_hooks.fade = h_dcp_fade;
    g_dcp_hooks.fade_done = h_dcp_fade_done;
    g_dcp_hooks.restart = h_restart;
    g_obj_hooks.item = h_item;
    g_obj_hooks.goal = h_goal;
    g_obj_hooks.stage_exit = h_stage_exit;
    g_obj_hooks.fade = h_obj_fade;
    g_obj_hooks.checkpoint_popup = NULL;
    V.on = 1;
}

void vs_end(void) {
    int i;
    if (!V.on)
        return;
    for (i = 0; i < 2; i++)
        if (V.tsnd_t[i] >= 0 && g_obj_hooks.sfx_own_stop)
            g_obj_hooks.sfx_own_stop(g_obj_hooks.ctx, &s_tsnd_owner[i], 0);
    /* the hooks of the single-player modules come back as they were (the camera / sound modules
     * re-chain themselves at the next stage start) */
    g_dcp_hooks.vs_tension = V.saved_dcp.vs_tension;
    g_dcp_hooks.fade = V.saved_dcp.fade;
    g_dcp_hooks.fade_done = V.saved_dcp.fade_done;
    g_dcp_hooks.restart = V.saved_dcp.restart;
    g_obj_hooks.item = V.saved_obj.item;
    g_obj_hooks.goal = V.saved_obj.goal;
    g_obj_hooks.stage_exit = V.saved_obj.stage_exit;
    g_obj_hooks.fade = V.saved_obj.fade;
    g_obj_hooks.checkpoint_popup = V.saved_obj.checkpoint_popup;
    W.flags &= ~0x20u;
    V.on = 0;
}

void vs_player_begin(int i) {
    V.cur = i;
    W.check_x = V.check_x[i];
    W.check_y = V.check_y[i];
}
void vs_player_end(int i) {
    V.check_x[i] = W.check_x;
    V.check_y[i] = W.check_y;
    V.cur = -1;
}

/* Player_Action_ReachGoal / Player_ReadInput: the goal run (the winner), nothing once the race is over */
void vs_filter_input(int i, unsigned short *held, unsigned short *pressed, unsigned short *released) {
    DcPlayer *w = V.p[i];
    if (!(w->pf & 0x1000000))
        return;
    if (V.race[i] >= VS_LOST || (w->pf & 0x400000 && !(w->pf & 0x200000))) {
        *held = *pressed = *released = 0;
        return;
    }
    *pressed = *released = 0;
    *held = (unsigned short)((*held & 0xc03) | (w->grav_rot ? 0x20 : 0x10));
}

/* Player_OnPushRival(me pushed by o's push box) */
static void on_push_rival(DcPlayer *me, DcPlayer *o) {
    s32 v, d, ad;
    int ok = 1;
    if (me->pf & 0x80000)
        return;
    if (o->pf & 0x80000)
        return;
    if (!(W.flags & 1))
        return;
    if (me->pf & 0x100000)
        return;
    if (me->move_flags & 0x2100)
        return;
    if (o->gimmick != NULL || me->gimmick != NULL)
        return;
    if (me->vs_timer != 0)
        return;
    v = (s16)(me->vel_x + me->push_x);
    if (!(me->move_flags & 0x8000))
        v = (s16)(v + me->gnd_vel);
    if ((v > 0 && me->x > o->x) || (v < 0 && me->x < o->x))
        v = 0;
    if ((me->px > o->px && me->x < o->x) || (me->px < o->px && me->x > o->x)) {
        ok = 0;
        me->vel_x = 0;
        me->gnd_vel = 0;
        v = (s16)(v << 1);
    }
    d = me->x - o->x;
    ad = d < 0 ? -d : d;
    if (ad < 0x2800 && ((me->move_x < -0x800 && o->move_x > 0x800) || (me->move_x > 0x800 && o->move_x < -0x800))) {
        ok = 0;
        me->vel_x = 0;
        me->gnd_vel = 0;
        v = (s16)(v << 1);
    }
    if (ad < 0x1800) {
        if (ad > 0x1000)
            v = (v * (0x800 - (ad - 0x1000)) << 5) >> 16;
        if (o->move_x == 0) {
            if (me->x > o->x && v > -0x100 && v < 0x80)
                v = -0x100;
            if (me->x < o->x && v < 0x100 && v > -0x80)
                v = 0x100;
        }
    } else if (ok) {
        v = 0;
    }
    me->push_x = (s16)(me->push_x - v);
    if (v == 0)
        return;
    if (!(me->pf & 0x10000))
        FixVsPushIcon_Create(me);
    me->pf |= 0x10000;
}

/* Player_ApplyNetState, rival flag 0x20000 (the rival boosted into this player): knock-back */
static void knock_back(DcPlayer *w, DcPlayer *rival) {
    s16 *v;
    w->vs_timer = 0xc;
    v = (w->move_flags & 0x10) ? &w->vel_x : &w->gnd_vel;
    if (rival->move_x < 0) {
        if (*v > -0x480)
            *v = -0x480;
    } else {
        if (*v < 0x480)
            *v = 0x480;
    }
    w->hitstop = 0x44;
    EffVs_Create(objects_player_box_cx(rival, w), objects_player_box_cy(rival, w));
}

void vs_after_players(void) {
    int i;
    DcPlayer *a = V.p[0], *b = V.p[1];
    if (!V.on || a == NULL || b == NULL)
        return;
    /* the attacks of this frame (Player_UpdateBoost: EffVs at the attacker, sound 0x40) */
    for (i = 0; i < 2; i++) {
        DcPlayer *w = V.p[i], *o = V.p[1 - i];
        if (w->vs_attack) {
            w->vs_attack = 0;
            EffVs_Create(objects_player_box_cx(w, o), objects_player_box_cy(w, o));
            if (!(o->pf & 0x1000000))
                knock_back(o, w);
        }
    }
    /* Player_Collide: each player against the other's push box (coll2F8: -0x30..0x30 x -0x20..0x20) */
    for (i = 0; i < 2; i++) {
        DcPlayer *me = V.p[i], *o = V.p[1 - i];
        if ((me->pf | o->pf) & 0x1000000)
            continue;   /* a finished player no longer takes part */
        if (objects_player_box_hit(me, o->x >> 8, o->y >> 8, -0x30, -0x20, 0x60, 0x40))
            on_push_rival(me, o);
    }
    /* Marker_OnHit: the checkpoint belongs to the player who passed it */
    if (g_obj_stage.check_player != NULL) {
        int k = idx_of(g_obj_stage.check_player);
        V.check_x[k] = W.check_x;
        V.check_y[k] = W.check_y;
        g_obj_stage.check_player = NULL;
    }
    /* Player_CheckTimeOver (VS): at 0x8c9f nobody is at the goal -> fade, ReachGoal, unk40 = 3 */
    if (W.frame >= 0x8c9f) {
        for (i = 0; i < 2; i++) {
            DcPlayer *w = V.p[i];
            if (!(w->pf & 0x1000000)) {
                if (!V.timeover_fade) {
                    h_obj_fade(NULL, 4, 0x80);
                    V.timeover_fade = 1;
                }
                reach_goal(w, NULL);
                V.race[i] = VS_TIME_OVER;
            }
        }
        if (V.info.winner < 0)
            V.info.winner = 2;
    }
    /* Player_Last (VS): time over and the fade done -> StageShutdown_Create(Stage_ExitAfterFail) ->
     * ResultsScreen_Create */
    if (W.frame >= 0x8c9f && fade_done(0) && fade_done(1) && (V.race[0] >= VS_TIME_OVER || V.race[1] >= VS_TIME_OVER))
        vs_request_exit(VS_EXIT_RESULTS);
}

void vs_frame_end(void) {
    int i;
    for (i = 0; i < 2; i++) {
        /* TensionSound_Main */
        if (V.tsnd_t[i] >= 0) {
            V.tsnd_t[i]--;
            if (V.tsnd_t[i] < 0 && g_obj_hooks.sfx_own_stop)
                g_obj_hooks.sfx_own_stop(g_obj_hooks.ctx, &s_tsnd_owner[i], 0x20);
        }
        /* BrightnessFade_Main */
        if (V.fade[i] < V.fade_target[i]) {
            V.fade[i] += V.fade_step[i];
            if (V.fade[i] > V.fade_target[i])
                V.fade[i] = V.fade_target[i];
        } else if (V.fade[i] > V.fade_target[i]) {
            V.fade[i] -= V.fade_step[i];
            if (V.fade[i] < V.fade_target[i])
                V.fade[i] = V.fade_target[i];
        }
    }
}
