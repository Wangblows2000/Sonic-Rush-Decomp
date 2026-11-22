/*
 * Player gimmick states of Water Palace (and the shared ones zone 3 also
 * uses), copied from src/game/Player/ and adapted like obj_pgimmick.c
 * (offset structs -> DcPlayer fields, Sound_PlaySeqArcEx(w->unk384/388) ->
 * g_dcp_hooks.sfx channel VOICE/BOOST, NNS_SndPlayerStopSeq -> sfx_stop,
 * Eff*_Create -> the object module's effects, @0x3F0..@0x3FC ->
 * ObjPlayerCtx.g3F0..g3FC):
 *   Player_Action_WaterRun, Player_State_WaterRun,
 *   Player_Action_ZoneLaunch, Player_State_ZoneLaunch,
 *   Player_Action_BreatheAirBubble, Player_State_BreatheAirBubble,
 *   Player_Action_Wheel2D, Player_State_Wheel2D,
 *   Player_Action_MapObj80, Player_State_MapObj80,
 *   Player_Action_MapObj40, Player_State_MapObj40,
 *   Player_Action_AttachToObject, Player_State_AttachedToObject,
 *   Player_ApplyAutoRunAcceleration, Player_UpdateUnderwater,
 *   the gimmick-object check of Player_In.
 * They run on the player only through its documented entry points.
 */
#include <string.h>
#include "obj_internal.h"

#define H g_dcp_hooks
#define ABS(x) ((x) < 0 ? -(x) : (x))

static ObjPlayerCtx s_dummy;
static ObjPlayerCtx *ctx(DcPlayer *w) {
    ObjPlayerCtx *c = obj_player_ctx(w);
    return c ? c : &s_dummy;
}
static void psfx(DcPlayer *p, s32 ch, s32 id) {
    if (H.sfx)
        H.sfx(H.ctx, p, ch, id);
}
static void psfx_stop(DcPlayer *p, s32 ch) {
    if (H.sfx_stop)
        H.sfx_stop(H.ctx, p, ch);
}
static void fxhook(DcPlayer *p, s32 kind, s32 a, s32 b) {
    if (g_obj_hooks.effect)
        g_obj_hooks.effect(g_obj_hooks.ctx, p, kind, a, b);
}
/* Player_SetAnimSpeedFromVelocity (same code as the player module's) */
static void anim_speed(DcPlayer *p, s32 v) {
    s32 t = (v >> 5) + (v >> 6);
    s16 s = (s16)ABS(t);
    if (s <= 0x10)
        s = 0x10;
    if (s >= 0x80)
        s = 0x80;
    if (H.anim_speed)
        H.anim_speed(H.ctx, p, s);
}
static void call(DcpFn f, DcPlayer *w) {
    if (f != NULL)
        f(w);
}
#define WATER (g_obj_camera[0].water) /* gCamera.unk62 */

/* ------------------------------------------- Player_ApplyAutoRunAcceleration */
void Player_ApplyAutoRunAcceleration(DcPlayer *w) {
    const DcpPhysics *t = dcp_physics(w->chara);
    s32 acc, dec;
    s16 top, s, tt;
    w->land_speed = 0;
    acc = t->acc;
    dec = t->dec;
    top = t->top;
    if (w->pf & 0x100)
        top = t->hs_top;
    if (w->pf & 0x80) {
        acc = t->boost_acc;
        top = t->boost_top;
        dec = t->boost_dec;
    }
    dcp_update_highspeed_timer(w);
    if (w->angle != 0) {
        s = (s16)((w->slope_bonus * obj_sin((u16)(w->angle << 8))) >> 12);
        if (s > 0)
            top = (s16)(top + s);
    }
    if (w->dash_timer != 0) {
        dec = 0;
    } else {
        tt = 0;
        if (ABS(w->gnd_vel) > w->run_thr[1]) {
            tt = (s16)(((ABS(w->gnd_vel) - w->run_thr[1]) << 8) / (top - w->run_thr[1]));
            if (tt > 0x100)
                tt = 0x100;
            tt = (s16)((tt * 0xf8) >> 8);
        }
        acc = (s16)(acc - ((acc * tt) >> 8));
    }
    if (w->pf & 0x4000000) {
        acc >>= 1;
        dec >>= 1;
    }
    if (!(w->disp_flags & 1)) {
        if (w->gnd_vel < 0)
            w->gnd_vel = (s16)dcp_spd_down(w->gnd_vel, dec);
        w->gnd_vel = (s16)dcp_spd_up(w->gnd_vel, acc, top);
    } else {
        if (w->gnd_vel > 0)
            w->gnd_vel = (s16)dcp_spd_down(w->gnd_vel, dec);
        w->gnd_vel = (s16)dcp_spd_up(w->gnd_vel, -acc, top);
    }
}

/* ===================================================== water run */
void Player_State_WaterRun(DcPlayer *w);
void Player_Action_WaterRun(DcPlayer *w) {
    if (w->state == Player_State_WaterRun)
        return;
    dcp_reset_gimmick_state(w);
    dcp_set_run_anim_for_speed(w);
    w->disp_flags |= 4;
    w->move_flags |= 0x10;
    w->move_flags &= ~0x80u;
    w->y = (WATER - 0xF) << 8;
    w->state = Player_State_WaterRun;
    w->charge = 8;
    psfx(w, DCP_CH_VOICE, 0x54);
    g_obj.stats.water_runs++;
}
void Player_State_WaterRun(DcPlayer *w) {
    s32 v2 = w->gnd_vel;
    if (v2 < 0)
        v2 = -v2;
    if (v2 < w->run_thr[2]) {
        if (dcp_check_fall_off(w)) {
            psfx_stop(w, DCP_CH_VOICE);
            w->angle = 0;
            w->move_flags |= 0x80;
            dcp_act_fall(w);
            return;
        }
    }
    if ((w->pressed & 3) && w->act_jump != NULL) {
        psfx_stop(w, DCP_CH_VOICE);
        w->angle = 0;
        w->move_flags |= 0x80;
        w->act_jump(w);
        return;
    }
    if (w->charge != 0) {
        w->charge--;
    } else if (w->move_flags & 0xd) {
        psfx_stop(w, DCP_CH_VOICE);
        w->angle = 0;
        w->move_flags &= ~0x2000u;
        w->move_flags |= 0x80;
        w->vel_x = w->gnd_vel;
        dcp_act_land(w, 0);
        if (ctx(w)->g3F8 == 2) {
            Player_Action_Grind(w);
            return;
        }
        call(w->act_idle, w);
        return;
    }
    anim_speed(w, w->gnd_vel);
    dcp_update_run_anim(w);
    dcp_apply_ground_input(w);
    if (W.frame & 7)
        return;
    EffWaterRunSplash_Create(w, 0, 0, WATER);
}

/* ===================================================== zone launch */
void Player_State_ZoneLaunch(DcPlayer *w);
void Player_Action_ZoneLaunch(DcPlayer *w, s16 x, s16 y) {
    u8 b3 = w->rot3;
    s32 v18 = w->z;
    if (w->state == Player_State_ZoneLaunch) {
        w->vel_x = (s16)((w->move_x + x) >> 1);
        w->vel_y = (s16)((w->move_y + y) >> 1);
        w->timer = 0;
        w->move_flags &= ~0x80u;
        return;
    }
    dcp_player_init_state(w);
    dcp_set_anim(w, 0x29);
    w->disp_flags |= 4;
    w->state = Player_State_ZoneLaunch;
    w->move_flags |= 0x10;
    w->move_flags &= ~0x80u;
    w->pf |= 0x100000;
    w->vel_x = (s16)((w->move_x + x) >> 1);
    w->vel_y = (s16)((w->move_y + y) >> 1);
    w->gnd_vel = 0;
    w->vel_z = 0;
    w->angle = 0;
    w->rot2 = 0;
    w->rot3 = b3;
    w->z = v18;
    psfx(w, DCP_CH_BOOST, 0x58);
    g_obj.stats.zone_launches++;
}
void Player_State_ZoneLaunch(DcPlayer *w) {
    dcp_settle_air_rotation(w);
    w->rot3 = (u8)dcp_spd_down(w->rot3, 2);
    w->vel_x = (s16)dcp_spd_down(w->vel_x, 0x60);
    if (w->timer == 0)
        w->vel_y = (s16)dcp_spd_down(w->vel_y, 0x60);
    w->timer = 1;
    w->move_flags |= 0x80;
    if (ABS(w->vel_x) >= 0x400)
        return;
    if (ABS(w->vel_y) >= 0x400)
        return;
    w->z = 0;
    w->rot3 = 0;
    w->pf &= ~0x100000u;
    w->move_flags |= 0x80;
    if (w->move_flags & 1)
        w->vel_x = w->gnd_vel;
    dcp_act_land(w, 0);
    call(w->act_idle, w);
}

/* ================================================= air bubble */
void Player_State_BreatheAirBubble(DcPlayer *w);
void Player_Action_BreatheAirBubble(DcPlayer *w) {
    if (w->gimmick != NULL) {
        w->unk3CA = 0;
        psfx(w, DCP_CH_VOICE, 0x53);
        return;
    }
    psfx(w, DCP_CH_VOICE, 0x53);
    dcp_set_anim(w, 0x23);
    w->state = Player_State_BreatheAirBubble;
    w->vel_y = 0;
    w->unk3CA = 0;
    w->timer = 4;
    w->move_flags |= 0x2000;
}
void Player_State_BreatheAirBubble(DcPlayer *w) {
    w->unk3CA = 0;
    w->timer--;
    if (w->timer == 0)
        w->move_flags &= ~0x2000u;
    if (!(w->move_flags & 0x2000)) {
        if (w->move_flags & 1) {
            if (w->move_flags & 0x10) {
                dcp_act_land(w, 0);
                w->move_flags &= ~0x10u;
            }
            dcp_apply_ground_input(w);
        } else {
            dcp_apply_air_input(w);
        }
    }
    if (!(w->disp_flags & 8))
        return;
    w->move_flags &= ~0x2000u;
    if (w->move_flags & 1) {
        call(w->act_idle, w);
        return;
    }
    w->disp_flags |= 0x400;
    dcp_act_fall(w);
}

/* ===================================================== Wheel2D */
void Player_State_Wheel2D(DcPlayer *w);
void Player_Action_Wheel2D(DcPlayer *w, Obj *o) {
    ObjPlayerCtx *c = ctx(w);
    if (w->state == Player_State_Wheel2D)
        return;
    dcp_player_init_state(w);
    w->gimmick = o;
    dcp_set_anim(w, 0x28);
    w->disp_flags |= 4;
    w->state = Player_State_Wheel2D;
    w->move_flags |= 0x10;
    w->move_flags |= 0x2000;
    w->move_flags |= 0x4000;
    w->disp_flags &= ~1;
    w->disp_flags &= ~0x200;
    w->pf2 |= 0x20000;
    w->pf |= 0x100000;
    w->vel_x = 0;
    w->vel_y = 0;
    w->vel_z = 0;
    w->angle = 0;
    w->pf &= ~3u;
    w->gnd_vel = (s16)(w->move_x >> 4);
    if (w->gnd_vel < 0x40)
        w->gnd_vel = 0x40;
    if (w->move_x < w->run_thr[2])
        w->pf |= 1;
    w->trick_input = 0;
    c->g3F4 = 0;
    w->timer = 0;
    c->g3F8 = o->h.x - w->x;
    c->g3FC = o->h.y - w->y;
    w->pf |= 0x2000;
    w->pf2 |= 0x10;
    psfx(w, DCP_CH_VOICE, 0x58);
}
void Player_State_Wheel2D(DcPlayer *w) {
    ObjPlayerCtx *c = ctx(w);
    Obj *o = (Obj *)w->gimmick;
    ObjMtx33 m, r;
    ObjVec v;
    u8 prev;
    u16 a;
    if (o == NULL) {
        w->gnd_vel = 0;
        w->gimmick = NULL;
        w->obj_flags &= ~2u;
        w->move_flags &= ~0x2000u;
        w->move_flags &= ~0x4000u;
        w->pf &= ~0x2000u;
        w->pf2 &= ~0x30u;
        w->pf2 &= ~0x20000u;
        w->pf &= ~0x100000u;
        w->disp_flags |= 0x200;
        w->rot3 = 0;
        w->rot2 = 0;
        w->angle = 0;
        call(w->act_jump, w);
        return;
    }
    if (!(W.frame & 3) && !(RAND() & 3))
        EffGmkBubble_Create(w, o->h.x, o->h.y, 0, 0, WATER);
    if (w->gnd_vel < 0x40)
        w->gnd_vel = 0x40;
    prev = w->rot3;
    c->g3F4 -= w->gnd_vel * 6;
    w->rot3 = (u8)((c->g3F4 & 0xffff) >> 8);
    if (w->rot3 > prev)
        psfx(w, DCP_CH_VOICE, 0x58);
    if (w->rot3 < 0x80 && prev >= 0x80)
        psfx(w, DCP_CH_VOICE, 0x58);
    c->g3F8 -= (c->g3F8 - 0x5000) >> 4;
    w->timer++;
    if (w->timer < 0x40) {
        c->g3FC -= (c->g3FC - 0x1800) >> 4;
    } else {
        if (w->timer == 0x70)
            w->pf |= 2;
        if (w->anim == 0x28) {
            dcp_set_anim(w, 0x29);
            w->disp_flags |= 4;
        }
        if (w->pf & 1)
            c->g3FC -= 0x80;
        else
            c->g3FC += 0x180;
    }
    w->px = w->x;
    w->py = w->y;
    w->pz = w->z;
    a = (u16)-(w->rot3 << 8);
    MTX_Identity33_(&m);
    MTX_RotY33_(&r, obj_sin(a), obj_cos(a));
    MTX_Concat33(&m, &r, &m);
    v.x = c->g3F8 << 4;
    v.y = c->g3FC << 4;
    v.z = 0;
    MTX_MultVec33(&v, &m, &v);
    w->x = o->h.x - (v.x >> 4);
    w->y = o->h.y - (v.y >> 4);
    w->z = o->h.z - (v.z >> 4);
    w->move_x = (s16)(w->x - w->px);
    w->move_y = (s16)(w->y - w->py);
    w->move_z = (s16)(w->z - w->pz);
}

/* =================================================== MapObj80 (slide) */
void Player_State_MapObj80(DcPlayer *w);
void Player_Action_MapObj80(DcPlayer *w, Obj *o) {
    ObjPlayerCtx *c = ctx(w);
    if (w->state == Player_State_MapObj80) {
        if (c->g3F0 != 0)
            return;
        if (w->disp_flags & 1) {
            c->g3F0 = w->x - (o->h.x + 0x3e00);
            if (c->g3F0 < 0)
                c->g3F0 = 8;
        } else {
            c->g3F0 = (o->h.x - 0x1000) - w->x;
            if (c->g3F0 > 0)
                c->g3F0 = -1;
        }
        return;
    }
    dcp_reset_gimmick_state(w);
    dcp_act_land(w, 0);
    w->vel_x = 0;
    w->vel_y = 0;
    w->angle = 0;
    w->rot3 = 0;
    dcp_set_anim(w, 0x27);
    if (w->gnd_vel > 0)
        w->disp_flags &= ~1;
    else
        w->disp_flags |= 1;
    if (w->disp_flags & 1) {
        if (w->gnd_vel > -0x200)
            w->gnd_vel = -0x200;
    } else {
        if (w->gnd_vel < 0x200)
            w->gnd_vel = 0x200;
    }
    w->x = o->h.x;
    w->y = o->h.y + 0x200;
    c->g3F0 = 0;
    w->move_flags &= ~0x80u;
    w->move_flags |= 0x2000;
    w->move_flags &= ~0x20000u;
    w->pf |= 0x100000;
    w->slope_acc = dcp_physics(w->chara)->roll_slope;
    w->slope_lim = 0xe;
    w->state = Player_State_MapObj80;
    w->charge = 8;
    w->move_flags &= ~0xfu;
    psfx(w, DCP_CH_BOOST, 0x55);
}
void Player_State_MapObj80(DcPlayer *w) {
    ObjPlayerCtx *c = ctx(w);
    s16 spd = 0;
    if (w->charge != 0) {
        w->charge--;
    } else if (w->move_flags & 0xd) {
        psfx_stop(w, DCP_CH_BOOST);
        dcp_player_load_physics(w);
        w->angle = 0;
        w->move_flags &= ~0x2000u;
        w->move_flags |= 0x80;
        w->move_flags |= 0x20000;
        w->pf &= ~0x100000u;
        w->vel_x = w->gnd_vel;
        dcp_act_land(w, 0);
        call(w->act_idle, w);
        return;
    }
    if (!(W.frame & 7))
        EffWaterGmk_Create(w, 0, 0xc);
    if (w->disp_flags & 1)
        w->angle = 0xf6;
    else
        w->angle = 0xa;
    Player_ApplyAutoRunAcceleration(w);
    if (c->g3F0 != 0) {
        spd = w->gnd_vel;
        if (w->disp_flags & 1)
            c->g3F0 += spd;
        else
            c->g3F0 -= spd;
        w->rot3 = (u8)(c->g3F0 >> 8);
        w->gnd_vel = (s16)((w->gnd_vel * obj_cos((u16)(w->rot3 << 8))) >> 12);
        if (w->disp_flags & 1)
            w->angle += (u8)(((0x100 - w->angle) * obj_sin((u16)(w->rot3 << 8)) * 0x290) >> 20);
        else
            w->angle -= (u8)((w->angle * obj_sin((u16)(w->rot3 << 8))) >> 12);
        if (c->g3F0 < 0) {
            if (w->rot3 <= 0xc0)
                w->angle = (u8)-w->angle;
            if (w->rot3 <= 0x80) {
                c->g3F0 = 0;
                w->rot3 = 0;
                w->disp_flags ^= 1;
                spd = (s16)-spd;
            }
        } else {
            if (w->rot3 >= 0x40)
                w->angle = (u8)-w->angle;
            if (w->rot3 >= 0x80) {
                c->g3F0 = 0;
                w->rot3 = 0;
                w->disp_flags ^= 1;
                spd = (s16)-spd;
            }
        }
    }
    w->move_flags &= ~0x2000u;
    dcp_obj_move(w);
    w->move_flags |= 0x2000;
    w->angle = 0;
    if (spd != 0)
        w->gnd_vel = spd;
}

/* =================================================== MapObj40 (0x7e) */
void Player_State_MapObj40(DcPlayer *w);
void Player_Action_MapObj40(DcPlayer *w) {
    if (w->state == Player_State_MapObj40)
        return;
    if (w->state == Player_State_MapObj80)
        return;
    if (!(w->move_flags & 1))
        return;
    dcp_reset_gimmick_state(w);
    dcp_set_anim(w, 0x26);
    w->disp_flags |= 4;
    w->move_flags &= ~0x10u;
    w->move_flags &= ~0x40000u;
    w->pf |= 0x100000;
    w->state = Player_State_MapObj40;
    w->slope_acc = dcp_physics(w->chara)->roll_slope;
    w->slope_lim = 0xe;
    psfx(w, DCP_CH_BOOST, 0x55);
    g_obj.stats.gimmicks++;
    obj_log("mapobj40_slide", 0x7e, w->x >> 8);
}
void Player_State_MapObj40(DcPlayer *w) {
    if (w->angle > 0x80)
        w->disp_flags |= 1;
    else
        w->disp_flags &= ~1;
    if (w->angle == 0 && ABS(w->gnd_vel) < 0x100)
        w->gnd_vel = (s16)dcp_spd_up(w->gnd_vel, 0x10, 0x100);
    if ((w->move_flags & 1) && !(W.frame & 7))
        EffWaterGmk_Create(w, 0, 0xc);
    if (w->move_flags & 0x10) {
        if (!(w->move_flags & 1))
            return;
        psfx_stop(w, DCP_CH_BOOST);
        dcp_player_load_physics(w);
        w->move_flags |= 0x40000;
        w->pf &= ~0x100000u;
        w->vel_x = w->gnd_vel;
        dcp_act_land(w, 0);
        call(w->act_idle, w);
    } else {
        if (w->move_flags & 1)
            return;
        w->move_flags |= 0x10;
        w->move_flags |= 0x8000;
        w->vel_x = (s16)((w->gnd_vel * obj_cos((u16)(w->angle << 8))) >> 12);
        w->vel_y = (s16)((w->gnd_vel * obj_sin((u16)(w->angle << 8))) >> 12);
    }
}

/* ============================================= attach to an object (Mine) */
void Player_State_AttachedToObject(DcPlayer *w);
void Player_Action_AttachToObject(DcPlayer *w, Obj *o, s32 x, s32 y) {
    ObjPlayerCtx *c = ctx(w);
    if (w->gimmick == o)
        return;
    dcp_reset_gimmick_state(w);
    w->gimmick = o;
    dcp_set_anim(w, 0x2c);
    w->disp_flags |= 4;
    w->state = Player_State_AttachedToObject;
    w->move_flags |= 0x10;
    w->move_flags |= 0x8000;
    w->move_flags |= 0x2000;
    w->move_flags &= ~0x80u;
    w->move_flags &= ~0x40u;
    w->pf |= 0x100000;
    w->pf &= ~0xfu;
    w->vel_x = 0;
    w->vel_y = 0;
    w->gnd_vel = 0;
    w->angle = 0;
    w->trick_input = 0;
    w->timer = 0;
    c->g3F0 = x << 8;
    c->g3F4 = y << 8;
    w->px = w->x;
    w->py = w->y;
    psfx(w, DCP_CH_VOICE, 0x6d);
}
void Player_State_AttachedToObject(DcPlayer *w) {
    ObjPlayerCtx *c = ctx(w);
    Obj *o = (Obj *)w->gimmick;
    ObjMtx33 m, r;
    ObjVec v;
    if (o != NULL) {
        w->px = w->x;
        w->py = w->y;
        w->pz = w->z;
        if (w->pf & 2) {
            MTX_Identity33_(&m);
            if (w->pf & 4) {
                MTX_RotX33_(&r, obj_sin((u16)-o->wC), obj_cos((u16)-o->wC));
                w->rot2 = (u8)(-o->wC >> 8);
            } else {
                MTX_RotY33_(&r, obj_sin((u16)o->w8), obj_cos((u16)o->w8));
            }
            if (w->pf & 8)
                w->rot3 = (u8)-(o->w8 >> 8);
            MTX_Concat33(&m, &r, &m);
            v.x = -c->g3F0 << 4;
            v.y = -c->g3F4 << 4;
            v.z = 0;
            MTX_MultVec33(&v, &m, &v);
            w->x = o->h.x - (v.x >> 4);
            w->y = o->h.y - (v.y >> 4);
            w->z = o->h.z - (v.z >> 4);
        } else {
            w->x = o->h.x + c->g3F0;
            w->y = o->h.y + c->g3F4;
        }
        if (w->pf & 1)
            w->angle = (u8)((w->move_x << 1) >> 8);
        if (o->h.obj_flags & 0x1000) {
            w->vel_x = o->h.gnd_vel;
            w->gimmick = o = NULL;
        } else if (o->h.obj_flags & 0x2000) {
            w->vel_x = o->h.vel_x;
            w->gimmick = o = NULL;
        } else if ((w->pf & 1) && (o->h.obj_flags & 2)) {
            w->vel_x = o->h.move_x;
            w->vel_y = o->h.move_y;
            w->gimmick = o = NULL;
        }
    }
    if (w->held & 0x20)
        w->disp_flags |= 1;
    if (w->held & 0x10)
        w->disp_flags &= ~1;
    if (w->gimmick != NULL && !(w->pressed & 3))
        return;
    if (w->act_jump == NULL)
        return;
    if (w->act_idle == NULL)
        return;
    w->gimmick = NULL;
    w->pf &= ~0x2000u;
    w->pf2 &= ~0x30u;
    w->pf2 &= ~0x20000u;
    w->disp_flags |= 0x200;
    w->move_flags |= 0xc0;
    w->move_flags &= ~0x2000u;
    w->move_flags &= ~0x4000u;
    w->pf &= ~0x100000u;
    w->rot2 = 0;
    w->rot3 = 0;
    w->angle = 0;
    psfx_stop(w, DCP_CH_BOOST);
    if (w->pressed & 3) {
        w->act_jump(w);
        return;
    }
    dcp_act_land(w, 0);
    w->act_idle(w);
}

/* ================================================= Player_UpdateUnderwater */
void Player_UpdateUnderwater(DcPlayer *w) {
    u16 lvl;
    const DcpPhysics *t;
    if (w->pid != 0)
        return;
    if (W.zone != 1)
        return;
    if (!(W.flags & 1))
        return;
    lvl = g_obj_camera[w->cam < 2 ? w->cam : 0].water;
    if (lvl != 0) {
        if ((w->y >> 8) > lvl) {
            if (!(w->pf & 0x4000000)) {
                if (w->invinc == 0)
                    fxhook(w, OBJ_FX_BGM_LAYER, 1, 0);
                EffAirBubble_Spawn(w, 0, 8, lvl);
                EffAirBubble_Spawn(w, 6, 7, lvl);
                EffAirBubble_Spawn(w, -4, 6, lvl);
                w->vel_y = 0;
                if (g_obj_stage.check_time + 1 < W.frame) {
                    EffWaterSplash_Spawn(w);
                    fxhook(w, OBJ_FX_RIPPLE3D, 0, 0);
                    psfx(w, DCP_CH_VOICE, 0x51);
                }
                g_obj.stats.water_entries++;
            }
            w->pf |= 0x4000000;
            if (W.zone < 9 && W.act < 2 && !(w->pf & 0x400))
                w->unk3CA++;
        } else {
            if (w->pf & 0x4000000) {
                if (w->invinc == 0)
                    fxhook(w, OBJ_FX_BGM_LAYER, 0, 0);
                if (g_obj_stage.check_time + 1 < W.frame) {
                    EffWaterSplash_Spawn(w);
                    fxhook(w, OBJ_FX_RIPPLE3D, 0, 0);
                    psfx(w, DCP_CH_VOICE, 0x51);
                }
                t = dcp_physics(w->chara);
                w->gravity = t->gravity;
                w->fall_max = t->fall_max;
                g_obj.stats.water_exits++;
            }
            w->pf &= ~0x4000000u;
            w->unk3CA = 0;
        }
    } else {
        w->pf &= ~0x4000000u;
        w->unk3CA = 0;
    }
    if ((w->pf & 0x4000000) && (W.flags & 1) && !(W.frame & 0x1f) && !(RAND() & 3))
        EffAirBubble_Spawn(w, 0, 0, lvl);
    if (!(w->pf & 0x4000000))
        return;
    switch ((s16)(w->p374 - w->unk3CA)) {
    case 0x78: EffDrownCount_Spawn(w, 0, -0x18, 0); break;
    case 0xf0: EffDrownCount_Spawn(w, 0, -0x18, 1); break;
    case 0x168: EffDrownCount_Spawn(w, 0, -0x18, 2); break;
    case 0x1e0: EffDrownCount_Spawn(w, 0, -0x18, 3); break;
    case 0x258: EffDrownCount_Spawn(w, 0, -0x18, 4); break;
    case 0x2a8: fxhook(w, OBJ_FX_JINGLE, 0x17, 0); break;
    case 0x2d0:
        EffDrownCount_Spawn(w, 0, -0x18, 5);
        EffAirBubble_Spawn(w, 0, 0, lvl);
        break;
    }
    if (w->unk3CA <= w->p374)
        return;
    dcp_act_die(w);
    if (W.zone < 9 && W.act < 2)
        w->vel_y = 0;
    EffAirBubble_Spawn(w, 0, 0, lvl);
    EffAirBubble_Spawn(w, 1, 4, lvl);
    EffAirBubble_Spawn(w, 6, 2, lvl);
    EffAirBubble_Spawn(w, -4, -6, lvl);
    g_obj.stats.drownings++;
    obj_log("drowned", 0, w->x >> 8);
}

/* Player_In: "if (w->unk3E8 != NULL && (w->unk3E8->unk54 & 4)) w->unk3E8 = NULL" */
void obj_player_check_gimmick(DcPlayer *w) {
    Obj *o = (Obj *)w->gimmick;
    if (o == NULL)
        return;
    if (o < &g_obj.pool[0] || o >= &g_obj.pool[OBJ_MAX])
        return; /* not an object of this module */
    if (!o->used || (o->h.obj_flags & 4))
        w->gimmick = NULL;
}
