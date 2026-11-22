/*
 * Player states (Player_State_*), actions (Player_Action_*) and movement
 * helpers. Faithful translations of the files of the same name in
 * src/game/Player/; side effects (sound, effects, HUD) go through g_dcp_hooks.
 */
#include "dcp_player.h"

#define H g_dcp_hooks
#define W g_dcp_world

static void sfx(DcPlayer *p, s32 ch, s32 id) { if (H.sfx) H.sfx(H.ctx, p, ch, id); }
static void sfx_stop(DcPlayer *p, s32 ch) { if (H.sfx_stop) H.sfx_stop(H.ctx, p, ch); }
static void fx(DcPlayer *p, s32 id, s32 a, s32 b) { if (H.effect) H.effect(H.ctx, p, id, a, b); }
static void atkdef(DcPlayer *p, s32 box, s32 a, s32 d) { if (H.atkdef) H.atkdef(H.ctx, p, box, a, d); }
static void shake(s32 s) { if (H.shake) H.shake(H.ctx, s); }
static void anim_speed(DcPlayer *p, s32 v) {
    /* Player_SetAnimSpeedFromVelocity */
    s32 t = (v >> 5) + (v >> 6);
    s16 s = (s16)DCP_ABS(t);
    if (s <= 0x10)
        s = 0x10;
    if (s >= 0x80)
        s = 0x80;
    if (H.anim_speed)
        H.anim_speed(H.ctx, p, s);
}
static dcp_bool score_active(void) { return H.score_active ? H.score_active(H.ctx) : DCP_FALSE; }

/* ================================================================ helpers */

/* Player_UpdateHighSpeedTimer */
void dcp_update_highspeed_timer(DcPlayer *w) {
    s16 lim = dcp_physics(w->chara)->hs_time;
    if (W.zone >= 9 || W.act >= 2)
        return;
    if (w->pf & 0x80) {
        w->hs_timer = lim;
        return;
    }
    if (!(w->held & 0x30))
        return;
    if (DCP_ABS(w->gnd_vel) < w->top)
        return;
    w->hs_timer++;
    if (w->hs_timer < lim)
        return;
    if (w->move_flags & 1)
        dcp_start_highspeed(w);
    w->hs_timer = lim;
}

/* Player_ApplyVerticalInput (3D sections: pf2 & 0x100) */
void dcp_apply_vertical_input(DcPlayer *w) {
    s32 acc, max, dec;
    if (!(w->pf2 & 0x100))
        return;
    if (w->z >= 0) {
        w->z = 0;
        w->vel_z = 0;
    }
    if (w->z <= -0x2800) {
        w->z = -0x2800;
        w->vel_z = 0;
    }
    /* prm = {@0x356, @0x358, @0x35A}, alt = {@0x366, @0x368, @0x36A} */
    acc = (s16)(w->acc << 1);
    max = w->top;
    dec = w->dec;
    if (w->pf & 0x100)
        max = w->hs_top;
    if (w->pf & 0x80) {
        acc = w->boost_acc;
        max = w->boost_top;
        dec = w->boost_dec;
    }
    if (w->held & 0xC0) {
        if (w->held & 0x80) {
            if (w->vel_z < 0)
                w->vel_z = (s16)dcp_spd_down(w->vel_z, dec);
            w->vel_z = (s16)dcp_spd_up(w->vel_z, acc, max);
        } else {
            if (w->vel_z > 0)
                w->vel_z = (s16)dcp_spd_down(w->vel_z, dec);
            w->vel_z = (s16)dcp_spd_up(w->vel_z, -acc, max);
        }
    } else {
        w->vel_z = (s16)dcp_spd_down(w->vel_z, dec);
    }
}

/* Player_ApplyGroundInput */
void dcp_apply_ground_input(DcPlayer *w) {
    s16 max = w->top;
    s32 acc = w->acc;
    s32 dec = w->dec;
    s32 k, t, nmax;

    if (w->pf & 0x100)
        max = w->hs_top;
    if (w->pf & 0x80) {
        acc = w->boost_acc;
        dec = w->boost_dec;
        max = w->boost_top;
    }
    dcp_update_highspeed_timer(w);
    if (w->angle != 0) {
        t = (s32)((u32)(w->slope_bonus * dcp_sin((u16)(w->angle << 8))) << 4) >> 16;
        if (t > 0)
            max = (s16)(max + t);
    }
    if (w->dash_timer != 0) {
        dec = 0;
    } else {
        k = 0;
        if (DCP_ABS(w->gnd_vel) > w->run_thr[1]) {
            k = (s16)(((DCP_ABS(w->gnd_vel) - w->run_thr[1]) << 8) / (max - w->run_thr[1]));
            if (k > 0x100)
                k = 0x100;
            k = (s32)((u32)((s16)k * 0xf8) << 8) >> 16;
        }
        acc = (s16)(acc - ((acc * k) >> 8));
    }
    if (w->pf & 0x4000000) {
        acc >>= 1;
        dec >>= 1;
    }
    if (w->land_speed >= max) {
        if (DCP_ABS(w->gnd_vel) >= max) {
            if (w->land_speed > w->gnd_vel)
                w->land_speed = (s16)DCP_ABS(w->gnd_vel);
            max = w->land_speed;
        }
    }
    if (w->held & 0x30) {
        if (w->held & 0x10) {
            if (w->anim != 0 && (w->anim < 5 || w->anim > 10))
                w->disp_flags &= ~1;
            if (w->gnd_vel < 0)
                w->gnd_vel = (s16)dcp_spd_down(w->gnd_vel, dec);
            w->gnd_vel = (s16)dcp_spd_up(w->gnd_vel, acc, max);
        } else {
            if (w->anim != 0 && (w->anim < 5 || w->anim > 10))
                w->disp_flags |= 1;
            if (w->gnd_vel > 0)
                w->gnd_vel = (s16)dcp_spd_down(w->gnd_vel, dec);
            w->gnd_vel = (s16)dcp_spd_up(w->gnd_vel, -acc, max);
        }
    } else {
        w->hs_timer = 0;
        t = w->vel_x;
        nmax = -max;
        if (t < nmax)
            t = nmax;
        else if (t > max)
            t = max;
        w->vel_x = (s16)t;
        w->gnd_vel = (s16)(w->gnd_vel < nmax ? nmax : (w->gnd_vel > max ? max : w->gnd_vel));
        if (((w->angle + 0x20) & 0xff) <= 0x40) {
            if (w->pf & 0x8000000) {
                w->pf &= ~0x8000000;
                return;
            }
            w->gnd_vel = (s16)dcp_spd_down(w->gnd_vel, dec);
        }
    }
    dcp_apply_vertical_input(w);
}

/* Player_ApplyAirInput */
void dcp_apply_air_input(DcPlayer *w) {
    s32 airAcc;
    s32 dec0 = w->dec;
    s32 acc = w->acc;
    s16 max = w->top;
    s32 dec = w->dec;
    s32 k, t, nmax;

    w->land_speed = 0;
    if (w->pf & 0x100)
        max = w->hs_top;
    if (w->pf & 0x80) {
        acc = w->boost_acc;
        dec = w->boost_dec;
        max = w->boost_top;
    }
    dcp_update_highspeed_timer(w);
    if (((w->angle + 0x20) & 0xc0) || w->angle == 0xe0)
        dec = (s32)((u32)dec << 14) >> 16;
    if (w->dash_timer != 0) {
        airAcc = (s32)((u32)acc << 14) >> 16;
        dec = 0;
    } else {
        k = 0;
        if (DCP_ABS(w->vel_x) > w->run_thr[1]) {
            k = (s16)(((DCP_ABS(w->vel_x) - w->run_thr[1]) << 8) / (max - w->run_thr[1]));
            if (k > 0x100)
                k = 0x100;
            k = (s32)((u32)((s16)k * 0xf8) << 8) >> 16;
        }
        airAcc = (s16)(acc - ((acc * k) >> 8));
    }
    if (w->pf & 0x4000000) {
        airAcc >>= 1;
        dec >>= 1;
    }
    if (w->air_lock != 0)
        return;
    if (w->held & 0x30) {
        if (w->held & 0x10) {
            w->disp_flags &= ~1;
            if (w->vel_x < 0)
                w->vel_x = (s16)dcp_spd_down(w->vel_x, (s16)(dec << 1));
            if (w->gnd_vel < 0)
                w->gnd_vel = (s16)dcp_spd_down(w->gnd_vel, (s16)(dec0 << 1));
            w->vel_x = (s16)dcp_spd_up(w->vel_x, airAcc, max);
        } else {
            w->disp_flags |= 1;
            if (w->vel_x > 0)
                w->vel_x = (s16)dcp_spd_down(w->vel_x, (s16)(dec << 1));
            if (w->gnd_vel > 0)
                w->gnd_vel = (s16)dcp_spd_down(w->gnd_vel, (s16)(dec0 << 1));
            w->vel_x = (s16)dcp_spd_up(w->vel_x, -airAcc, max);
        }
    } else {
        t = w->vel_x;
        nmax = -max;
        if (t < nmax)
            t = nmax;
        else if (t > max)
            t = max;
        w->vel_x = (s16)t;
        w->gnd_vel = (s16)(w->gnd_vel < nmax ? nmax : (w->gnd_vel > max ? max : w->gnd_vel));
        w->hs_timer = 0;
        w->vel_x = (s16)dcp_spd_down(w->vel_x, dec);
        w->gnd_vel = (s16)dcp_spd_down(w->gnd_vel, dec0);
    }
    dcp_apply_vertical_input(w);
}

/* Player_ApplySlopeForce */
void dcp_apply_slope_force(DcPlayer *w, const u8 *o) {
    u8 ang = w->angle;
    if (o != NULL)
        ang = o[1];
    w->gnd_vel = (s16)dcp_spd_up(w->gnd_vel, (w->slope_acc * dcp_sin((u16)(ang << 8))) >> 12, w->gnd_max);
    w->pf |= 0x8000000;
}

/* Player_ApplyRollFriction */
void dcp_apply_roll_friction(DcPlayer *w) {
    s32 v;
    if (w->dash_timer != 0) {
        w->dash_timer--;
        w->slope_acc = 0;
    } else {
        w->slope_acc = dcp_physics(w->chara)->roll_slope;
        w->slope_lim = 0xe;
    }
    v = w->gnd_vel;
    if ((v > 0 && (w->held & 0x10)) || (v < 0 && (w->held & 0x20))) {
        w->gnd_vel = (s16)dcp_spd_down(v, w->roll_dec >> 1);
        return;
    }
    if ((v <= 0 || !(w->held & 0x10)) && (v >= 0 || !(w->held & 0x20))) {
        w->gnd_vel = (s16)dcp_spd_down(v, w->roll_dec << 1);
        return;
    }
    w->gnd_vel = (s16)dcp_spd_down(v, w->roll_dec);
}

/* Player_CheckFallOffGround */
dcp_bool dcp_check_fall_off(DcPlayer *w) {
    s32 v;
    if (!(w->move_flags & 1)) {
        dcp_player_load_physics(w);
        return DCP_TRUE;
    }
    if (w->fall_grace != 0) {
        w->fall_grace--;
    } else {
        if (!((w->angle + 0x40) & 0x80) && w->angle != 0xc0)
            return DCP_FALSE;
        v = w->gnd_vel;
        if (DCP_ABS(v) >= 0x1e0)
            return DCP_FALSE;
        dcp_player_load_physics(w);
        return DCP_TRUE;
    }
    return DCP_FALSE;
}

/* Player_SettleAirRotation */
void dcp_settle_air_rotation(DcPlayer *w) {
    s32 t;
    w->angle = dcp_angle_toward8(w->angle, 0, 2);
    if (w->pf2 & 0x1100)
        return;
    t = (s32)((u32)dcp_spd_down((s16)(w->z >> 8), 4) << 16);
    w->vel_z = (s16)dcp_spd_down(w->vel_z, 0x20);
    w->z = t >> 8;
    w->rot2 = dcp_angle_toward8(w->rot2, 0, 4);
}

/* Player_StopSkid */
void dcp_stop_skid(DcPlayer *w) {
    if (w->anim >= 5 && w->anim <= 10)
        sfx_stop(w, DCP_CH_VOICE);
    if (w->anim == 0 || (w->anim >= 5 && w->anim <= 10)) {
        if (w->pf & 1)
            w->disp_flags |= 1;
        else
            w->disp_flags &= ~1;
    }
}

/* Player_UpdateRunAnimation */
void dcp_update_run_anim(DcPlayer *w) {
    s32 a = DCP_ABS(w->gnd_vel);
    u16 sp = (u16)a;
    if (w->anim < 0x3d || w->anim > 0x42)
        dcp_set_anim(w, 0x3d);
    if (w->anim == 0x3d && (w->disp_flags & 8) && sp >= w->run_thr[0])
        dcp_set_anim(w, 0x3e);
    if (w->anim == 0x3e && (w->disp_flags & 8))
        dcp_set_anim(w, sp >= w->run_thr[1] ? 0x3f : 0x3d);
    if (w->anim == 0x3f && (w->disp_flags & 8))
        dcp_set_anim(w, sp >= w->run_thr[2] ? 0x40 : 0x3e);
    if (w->pf & 0x180) {
        if (w->anim == 0x40 && (w->disp_flags & 8))
            dcp_set_anim(w, sp >= w->run_thr[3] ? 0x41 : 0x3f);
        if (w->anim == 0x41 && (w->disp_flags & 8))
            dcp_set_anim(w, sp >= w->run_thr[4] ? 0x42 : 0x40);
        if (w->anim == 0x42 && (w->disp_flags & 8) && sp < w->run_thr[4])
            dcp_set_anim(w, 0x41);
    } else if (w->anim == 0x40 && (w->disp_flags & 8) && sp < w->run_thr[3]) {
        dcp_set_anim(w, 0x3f);
    }
    w->disp_flags |= 4;
}

/* Player_SetRunAnimationForSpeed */
void dcp_set_run_anim_for_speed(DcPlayer *w) {
    u16 a = (u16)DCP_ABS(w->gnd_vel);
    s32 anim;
    if (a < w->run_thr[0])
        anim = 0x3d;
    else if (a < w->run_thr[1])
        anim = 0x3e;
    else if (a < w->run_thr[2])
        anim = 0x3f;
    else if (a < w->run_thr[3])
        anim = 0x40;
    else if (w->pf & 0x180)
        anim = a < w->run_thr[4] ? 0x41 : 0x42;
    else
        anim = 0x40;
    dcp_set_anim(w, anim);
    w->disp_flags |= 4;
}

/* Player_CheckLedgeBalance */
dcp_bool dcp_check_ledge_balance(DcPlayer *w, dcp_bool set) {
    s32 l, r;
    s32 x = w->x >> 8, y = (w->y >> 8) + w->box_b;
    if (dcp_probe_long(x, y, w->obj_flags & 1, 2, NULL, NULL) > 0) {
        l = dcp_probe_long(x + w->box_l - 2, y, w->obj_flags & 1, 2, NULL, NULL);
        r = dcp_probe_long(x + w->box_r + 2, y, w->obj_flags & 1, 2, NULL, NULL);
        if (l <= 0 && r >= 0x10) {
            if (set) {
                w->disp_flags &= ~0x400;
                dcp_set_anim(w, (w->disp_flags & 1) ? 0xe : 0xd);
            }
            return DCP_TRUE;
        }
        if (l >= 0x10 && r <= 0) {
            if (set) {
                w->disp_flags &= ~0x400;
                dcp_set_anim(w, (w->disp_flags & 1) ? 0xd : 0xe);
            }
            return DCP_TRUE;
        }
    }
    return DCP_FALSE;
}

/* ========================================================= high speed / boost */

void dcp_start_highspeed(DcPlayer *w) {
    if (w->pf & 0x1000000)
        return;
    if (w->pf & 0x100)
        return;
    w->still_cnt = 0;
    if (!(w->pf & 0x80))
        fx(w, DCP_FX_HIGHSPEED, 0, 0);
    w->fx_timer = 8;
    w->pf |= 0x100;
    w->hs_timer = 0x60;
    if (w->pf & 0x80)
        return;
    sfx(w, DCP_CH_VOICE, w->chara != 1 ? 9 : 0x16);
}

void dcp_stop_highspeed(DcPlayer *s) {
    s->still_cnt = 0;
    s->hs_timer = 0;
    s->pf &= ~0x100;
}

void dcp_start_boost(DcPlayer *w) {
    if (!(w->pf & 0x80)) {
        if (!(W.objmgr & 0x100))
            w->move_flags |= 0x4000;
        if (w->chara == 0 && !(w->pf2 & 0x100))
            fx(w, DCP_FX_TRAIL, 0x18, 0xa);
        sfx(w, DCP_CH_VOICE, w->chara != 1 ? 0xa : 0x17);
        sfx_stop(w, DCP_CH_BOOST);
        w->fx_timer = 0xc;
        w->pf |= 0x80;
        shake(4);
        if (w->held & 0x20)
            w->disp_flags |= 1;
        else if (w->held & 0x10)
            w->disp_flags &= ~1;
        fx(w, DCP_FX_BOOST_BURST, 0, 0);
        if (w->move_flags & 0x10) {
            if (DCP_ABS(w->vel_x) < w->boost_top)
                w->vel_x = (w->disp_flags & 1) ? -w->boost_top : w->boost_top;
        } else {
            if (DCP_ABS(w->gnd_vel) < w->boost_top)
                w->gnd_vel = (w->disp_flags & 1) ? -w->boost_top : w->boost_top;
        }
        fx(w, DCP_FX_ATTACK_HITBOX_BOOST, 0, 0); /* AttackHitbox_Create(-0x20,-0x20,0x20,0x20) + SetBoostState */
        if (w->tension_max != 0) {
            w->pf |= 0x40000;
        } else {
            dcp_add_tension(w, -0x70);
            if (H.tension_hud)
                H.tension_hud(H.ctx, w, w->tension >> 4);
        }
    }
    dcp_start_highspeed(w);
}

void dcp_stop_boost(DcPlayer *w) {
    if (!(w->pf & 0x80))
        return;
    w->boost_cool = 8;
    sfx_stop(w, DCP_CH_BOOST);
    w->pf &= ~0x80;
}

static void boost_min_speed(DcPlayer *w, s16 *p) {
    s32 a = DCP_ABS(*p);
    if (a < w->boost_top)
        *p = (w->disp_flags & 1) ? -w->boost_top : w->boost_top;
}

/* Player_UpdateBoost (VS-mode rival hit and effects reduced to hooks) */
void dcp_update_boost(DcPlayer *w) {
    s32 a, b;
    if (w->pid != 0)
        return;
    if ((w->pf & 0x10000) && (w->pressed & 0x100)) {
        sfx(w, DCP_CH_VOICE, 0x40);
        w->vs_timer = 0x10;
        if (w->move_flags & 0x10)
            boost_min_speed(w, &w->vel_x);
        else
            boost_min_speed(w, &w->gnd_vel);
        w->pf |= 0x20000;
        w->hitstop = 0x44;
    }
    if ((w->pressed & 0xc00) && (w->state != dcp_state_in_air || !(w->pf & 2)) && W.zone < 9 && W.act < 2 &&
        !(w->pf & 0x100000) && !(w->move_flags & 0x10) && w->boost_cool == 0 && !(w->pf & 0x400) &&
        w->tension >= 0x50 && !(w->pf & 0x80))
        dcp_start_boost(w);
    if (((w->gimmick == NULL || (w->pf2 & 8)) && (w->move_flags & 1) && !(w->move_flags & 0x2000) &&
         !(w->move_flags & 0x8000) && (w->anim <= 0x24 || w->anim >= 0x30)) ||
        w->tension == 0) {
        a = DCP_ABS(w->move_y);
        b = DCP_ABS(w->move_x);
        if (w->hs_min > b + a) {
            w->still_cnt++;
            if (w->still_cnt > 8 || w->tension == 0)
                dcp_stop_highspeed(w);
        } else {
            w->still_cnt = 0;
        }
    }
    if (!(w->held & 0xc00) || !(w->pf & 0x100) || w->tension == 0)
        dcp_stop_boost(w);
    if (w->gimmick == NULL && !(w->pf2 & 8)) {
        if (!(W.frame & 7) && (W.flags & 1)) {
            a = DCP_ABS(w->move_y);
            b = DCP_ABS(w->move_x);
            if (b + a > 0x200 && !(w->pf & 0x80) && (w->pf & 0x100) && (w->move_flags & 1))
                fx(w, DCP_FX_BOOST, -0xd, (w->pf2 & 0x1000) ? 0 : 0xd);
        }
        if (!(W.frame & 3) && (W.flags & 1) && w->chara == 1 && (w->pf & 0x80))
            fx(w, DCP_FX_BLAZE_FLAME, 4, 0); /* EffBlazeBoostFlame_Create (a: 0 run, 1 spin, 2 jump, 3 trick, 4 boost) */
    }
    if (w->boost_cool != 0)
        w->boost_cool--;
}

/* Player_DrainBoostTension */
void dcp_drain_boost_tension(DcPlayer *w) {
    if (!(w->pf & 0x80))
        return;
    if (w->tension_max != 0)
        return;
    if (w->gimmick != NULL && !(w->pf2 & 8))
        return;
    if (w->pf & 0x100000)
        return;
    dcp_add_tension(w, -8);
}

/* =================================================================== actions */

void dcp_act_ground_idle(DcPlayer *w) {
    s16 s;
    if (w->gnd_vel != 0 && w->act_move != NULL) {
        w->act_move(w);
        return;
    }
    s = w->anim;
    if (s < 0x30 && ((w->disp_flags & 8) || (s != 0x16 && s != 0x39 && s != 0x3a && s != 0x3b)))
        w->disp_flags |= 0x400;
    if (w->state == dcp_state_crouch /* && model == NULL */ && 0) {
        dcp_set_anim(w, 4);
        w->disp_flags &= ~4;
    } else {
        dcp_set_anim(w, 0x3c);
        w->disp_flags |= 4;
    }
    w->move_flags &= ~0x10;
    w->state = dcp_state_ground_idle;
    w->timer = 0;
}

void dcp_act_ground_move(DcPlayer *w) {
    if (((w->disp_flags & 1) && (w->held & 0x10)) || (!(w->disp_flags & 1) && (w->held & 0x20)))
        dcp_set_anim(w, 0);
    else
        dcp_set_run_anim_for_speed(w);
    w->move_flags &= ~0x10;
    w->state = dcp_state_ground_move;
    if (w->anim == 0) {
        if (w->held & 0x20)
            w->pf |= 1;
        else if (w->held & 0x10)
            w->pf &= ~1;
    }
    dcp_apply_ground_input(w);
}

void dcp_act_crouch(DcPlayer *w) {
    s16 v = w->gnd_vel;
    if (v > 0x80 || v < -0x80) {
        dcp_act_roll(w);
        return;
    }
    dcp_set_anim(w, 3);
    w->move_flags &= ~0x10;
    w->state = dcp_state_crouch;
    w->gnd_vel = 0;
    w->vel_x = 0;
}

void dcp_act_roll(DcPlayer *w) {
    dcp_set_anim(w, 0x31);
    w->disp_flags |= 4;
    w->move_flags &= ~0x10;
    w->state = dcp_state_roll;
    atkdef(w, 1, 0x13, 0);
    w->slope_acc = dcp_physics(w->chara)->roll_slope;
    w->slope_lim = 0xe;
    sfx(w, DCP_CH_VOICE, w->chara != 1 ? 0 : 0x11);
}

void dcp_act_spindash(DcPlayer *w) {
    dcp_set_anim(w, 0x32);
    w->disp_flags |= 4;
    w->move_flags &= ~0x10;
    w->state = dcp_state_spindash;
    w->charge = w->sd_base;
    atkdef(w, 1, 0x13, 0);
    w->pf |= 1;
    if (!(w->pf2 & 0x1000))
        fx(w, DCP_FX_SPIN_SMOKE2, 0, 0);
    sfx(w, DCP_CH_VOICE, w->chara != 1 ? 0 : 0x11);
}

void dcp_act_jump(DcPlayer *w) {
    dcp_set_anim(w, 0x30);
    if (!(w->move_flags & 0x10))
        w->jump_y = (s16)(w->y >> 8);
    w->disp_flags |= 4;
    w->move_flags |= 0x10;
    w->move_flags |= 0x8000;
    w->state = dcp_state_in_air;
    w->vel_x = (s16)((w->gnd_vel * dcp_cos((u16)(w->angle << 8))) >> 12);
    w->vel_y = (s16)((w->gnd_vel * dcp_sin((u16)(w->angle << 8))) >> 12);
    w->vel_x += (s16)((s32)((u32)(w->jump * dcp_sin((u16)(w->angle << 8))) << 4) >> 16);
    w->vel_y += (s16)((s32)((u32)(-w->jump * dcp_cos((u16)(w->angle << 8))) << 4) >> 16);
    if (w->pf2 & 0x1000) {
        w->vel_z = w->vel_y;
        w->vel_y = 0;
        if (w->z < 0)
            w->vel_z = -w->vel_z;
    }
    atkdef(w, 1, 0x13, 0);
    w->pf &= ~0xf;
    w->pf &= ~0xc000;
    w->timer = 0;
    w->trick_input = 0;
    w->charge = 0;
    sfx(w, DCP_CH_VOICE, w->chara != 1 ? 2 : 0x13);
    sfx_stop(w, DCP_CH_TRICK);
}

void dcp_act_fall(DcPlayer *w) {
    dcp_set_anim(w, 2);
    w->disp_flags |= 4;
    w->move_flags |= 0x10;
    w->move_flags |= 0x8000;
    w->state = dcp_state_in_air;
    w->vel_x = (s16)((w->gnd_vel * dcp_cos((u16)(w->angle << 8))) >> 12);
    w->vel_y = (s16)((w->gnd_vel * dcp_sin((u16)(w->angle << 8))) >> 12);
    atkdef(w, 1, 0, 0);
    w->pf &= ~0xF;
    w->pf |= 1;
    if (((w->hit | w->prev_hit) & 1) && w->vel_y < -0x80)
        w->pf |= 1;
    w->timer = 0;
    w->trick_input = 0;
    w->charge = 0;
}

/* Player_Action_LandOnGround */
void dcp_act_land(DcPlayer *w, u8 ang) {
    s32 a, b, i, c, v;
    dcp_player_load_physics(w);
    w->trick_obj = NULL;
    w->pf &= ~0xc000;
    w->move_flags &= ~0x8000;
    w->move_flags &= ~0x10;
    w->move_flags |= 0x80;
    w->hover_timer = 0x78;
    w->disp_flags &= ~0x20;
    w->air_lock = 0;
    if (w->anim < 0x1a || w->anim > 0x20) {
        if (w->layer_ovr != 0 && (w->layer_set & 2)) {
            w->obj_flags &= ~1;
            w->obj_flags |= w->layer_set & 1;
            w->layer_set = 0;
        }
        w->layer_ovr = 0;
    }
    if (ang != 0) {
        w->gnd_vel += (s16)((w->move_x * dcp_cos((u16)(ang << 8))) >> 12);
        w->gnd_vel += (s16)((w->move_y * dcp_sin((u16)(ang << 8))) >> 12);
        if (dcp_angle_upside_down(w->grav_rot))
            w->gnd_vel = -w->gnd_vel;
    } else {
        a = DCP_ABS(w->vel_x);
        b = DCP_ABS(w->gnd_vel);
        if (b < a)
            w->gnd_vel = w->vel_x;
        i = dcp_sin((u16)(w->angle << 8));
        c = DCP_ABS(w->vel_x);
        w->gnd_vel += (s16)((s32)((u32)(c * i) << 4) >> 16);
    }
    v = DCP_ABS(w->gnd_vel);
    w->land_speed = (s16)v;
    if (w->land_speed > w->gnd_max)
        w->land_speed = w->gnd_max;
    w->vel_x = 0;
    w->vel_y = 0;
    if (ang != 0)
        w->angle = ang;
    if (!(w->pf2 & 0x1000))
        w->rot2 = 0;
    w->jump_y = 0;
    sfx_stop(w, DCP_CH_TRICK);
}

void dcp_act_airdash(DcPlayer *w) {
    u8 a;
    dcp_set_anim(w, 0x18);
    w->disp_flags |= 4;
    w->move_flags |= 0x10;
    w->move_flags |= 0x8000;
    w->state = dcp_state_in_air;
    a = (w->disp_flags & 1) ? 0xC0 : 0x40;
    w->vel_y = 0;
    w->vel_x += (s16)((w->jump * dcp_sin((u16)(a << 8))) >> 12);
    w->vel_y += (s16)((-w->jump * dcp_cos((u16)(a << 8))) >> 12);
    w->pf &= ~0xF;
    w->pf |= 1;
    w->timer = 0;
    w->trick_input = 0;
    w->charge = 0;
    w->dash_timer = 0xC;
    sfx(w, DCP_CH_VOICE, 8);
}

void dcp_act_homing(DcPlayer *w) {
    s32 dx, dy, d;
    u16 ang;
    if (w->homing == NULL) {
        dcp_act_airdash(w);
        return;
    }
    dx = (w->homing->x - w->x) >> 8;
    dy = (w->homing->y - w->y) >> 8;
    ang = (u16)(dcp_atan2(dy, dx) + (w->grav_rot << 8));
    d = dx * dx + dy * dy;
    if (ang >= 0x8000 || d >= 0x8000) {
        dcp_act_airdash(w);
        return;
    }
    dcp_set_anim(w, 0x30);
    w->disp_flags |= 4;
    w->move_flags |= 0x10;
    w->move_flags |= 0x8000;
    w->move_flags &= ~0x80;
    w->pf |= 0x40;
    w->angle = 0;
    w->state = dcp_state_homing;
    w->timer = 0x20;
    atkdef(w, 1, 0x13, 0);
    w->pf &= ~3;
    sfx(w, DCP_CH_VOICE, 7);
}

void dcp_act_hover(DcPlayer *w) {
    if (w->hover_timer == 0)
        return;
    w->disp_flags |= 0x400;
    dcp_set_anim(w, 0x34);
    w->disp_flags |= 4;
    w->pf &= ~3;
    if (!(w->pf2 & 0x1000))
        fx(w, DCP_FX_BOOST_JET, 0, 0);
    w->vel_y = 0;
    if (w->pf2 & 0x1000)
        w->vel_z = 0;
    w->gravity = 8;
    w->fall_max = 0x220;
    w->acc = 0x30;
    w->dec = 0x60;
    w->boost_acc = 0x38;
    w->boost_dec = 0x60;
    w->state = dcp_state_hover;
    if (w->move_y > 0x300)
        w->hitstop = 0x44;
    sfx(w, DCP_CH_VOICE, 0x15);
}

void dcp_act_trick_up(DcPlayer *w) {
    s32 v;
    u16 f;
    if (!score_active())
        w->trick_cnt = 0;
    dcp_set_anim(w, 0x35);
    w->move_flags |= 0x10;
    w->move_flags |= 0x8000;
    w->pf |= 0x8000;
    w->state = dcp_state_in_air;
    w->vel_y = -w->jump;
    if (w->chara == 1) {
        atkdef(w, 1, 0x13, 0);
        w->flame_cnt = 0;
        w->fx_timer = 6;
        w->vel_y <<= 1;
        shake(2);
        w->hitstop = 0x44;
        fx(w, DCP_FX_BOOST_JET, 0, 0);
        if (w->held & 0x10) {
            v = DCP_ABS(w->vel_x);
            w->vel_x = (s16)v;
            w->gnd_vel = w->vel_x;
        } else if (w->held & 0x20) {
            v = DCP_ABS(w->vel_x);
            w->vel_x = (s16)-v;
            w->gnd_vel = w->vel_x;
        } else {
            w->vel_x = 0;
            w->gnd_vel = 0;
        }
        sfx(w, DCP_CH_VOICE, 0x1b);
        sfx(w, DCP_CH_VOICE, (dcp_rand() & 3) + 0x26);
    } else {
        f = w->held;
        if (!(f & 0x30) || (w->vel_x > 0 && (f & 0x20)) || (w->vel_x < 0 && (f & 0x10))) {
            w->vel_x = 0;
            w->gnd_vel = 0;
        }
        sfx(w, DCP_CH_VOICE, 0xf);
        sfx(w, DCP_CH_VOICE, (dcp_rand() & 3) + 0x1d);
    }
    w->pf &= ~5;
    w->pf |= 1;
    w->pf |= 8;
    w->trick_input = 0;
    w->charge = 0;
    dcp_add_trick_score(w, 200);
}

void dcp_act_trick_forward(DcPlayer *w) {
    u8 ang = 0;
    s32 t, a;
    if (!score_active())
        w->trick_cnt = 0;
    dcp_set_anim(w, 0x37);
    w->disp_flags |= 4;
    w->move_flags |= 0x10;
    w->move_flags |= 0x8000;
    w->pf |= 0x8000;
    w->state = dcp_state_in_air;
    w->vel_y = 0;
    w->pf &= ~1;
    w->pf |= 1;
    w->pf |= 8;
    if (w->chara != 1) {
        if (w->disp_flags & 1)
            ang -= 0x40;
        else
            ang += 0x40;
        t = (s32)((u32)((w->jump << 1) * dcp_sin((u16)(ang << 8))) << 4) >> 16;
        if ((t > 0 && w->vel_x < 0) || (t < 0 && w->vel_x > 0))
            w->vel_x = 0;
        w->vel_x += (s16)t;
        atkdef(w, 1, 0x13, 0);
        fx(w, DCP_FX_TRICK_FORWARD, 0, 0);
        sfx(w, DCP_CH_VOICE, 0x10);
        sfx(w, DCP_CH_VOICE, (dcp_rand() & 3) + 0x1d);
        w->dash_timer = 0xc;
    } else {
        w->saved_vx = w->vel_x;
        w->vel_x = (s16)(w->boost_top << 1);
        if (w->disp_flags & 1)
            w->vel_x = -w->vel_x;
        a = DCP_ABS(w->saved_vx);
        if (a < 0x300)
            w->saved_vx = w->vel_x >> 3;
        w->hitstop = 0x44;
        w->dash_timer = 0xc;
        w->air_lock = 0xc;
        fx(w, DCP_FX_BOOST_JET, 0, 0);
        sfx(w, DCP_CH_VOICE, 0x1c);
        sfx(w, DCP_CH_VOICE, (dcp_rand() & 3) + 0x26);
    }
    w->move_flags |= 0x4000;
    w->trick_input = 0;
    w->charge = 0;
    dcp_add_trick_score(w, 200);
}

void dcp_act_trick_combo(DcPlayer *w) {
    if (!score_active())
        w->trick_cnt = 0;
    if (w->trick_obj != NULL) {
        w->trick_lvl = w->trick_obj[0xA];
        if (w->trick_obj[0xA] < 0xff)
            w->trick_obj[0xA]++;
        w->trick_obj = NULL;
    }
    if ((w->pf & 0x4000) && (w->trick_input & 1)) {
        dcp_set_anim(w, 0x3b);
        w->pf |= 4;
        if (w->trick_lvl == 0)
            fx(w, DCP_FX_TRICK_COMBO, 0, 0);
        dcp_add_tension(w, (s16)(0x280 >> w->trick_lvl));
        if (w->chara != 1)
            sfx(w, DCP_CH_VOICE, (dcp_rand() & 1) ? 0x1f : 0x20);
        else
            sfx(w, DCP_CH_VOICE, (dcp_rand() & 1) ? 0x28 : 0x29);
        sfx_stop(w, DCP_CH_TRICK);
        sfx(w, DCP_CH_TRICK, 0x31);
        dcp_add_trick_score(w, 200);
    } else if (w->trick_input & 2) {
        if (w->anim == 0x39) {
            dcp_set_anim(w, 0x3a);
            w->pf |= 0x4000;
            dcp_add_tension(w, (s16)(400 >> w->trick_lvl));
            if (w->chara != 1)
                sfx(w, DCP_CH_VOICE, (dcp_rand() & 1) ? 0x1e : 0x25);
            else
                sfx(w, DCP_CH_VOICE, (dcp_rand() & 1) ? 0x27 : 0x2e);
            sfx(w, DCP_CH_VOICE, 0x2f);
        } else {
            dcp_set_anim(w, 0x39);
            dcp_add_tension(w, (s16)(400 >> w->trick_lvl));
            sfx(w, DCP_CH_VOICE, w->chara != 1 ? 0x1d : 0x26);
            sfx(w, DCP_CH_VOICE, 0x30);
        }
    } else {
        return;
    }
    w->move_flags |= 0x10;
    w->move_flags |= 0x8000;
    fx(w, DCP_FX_TRICK_SPARKLE, w->trick_lvl, 0);
    dcp_add_trick_score(w, 200);
    w->trick_input = 0;
    w->charge = 0;
}

/* Player_Action_Launch: springs / gimmick launches (vx/vy = 0 keep ground velocity) */
void dcp_act_launch(DcPlayer *w, s16 vx, s16 vy) {
    if (w->pf & 0x400)
        return;
    dcp_reset_gimmick_state(w);
    if (w->move_flags & 1)
        w->vel_x = w->gnd_vel;
    dcp_act_land(w, 0);
    dcp_rotate_vec(&vx, &vy, w->grav_rot);
    if (!(w->move_flags & 0x10))
        w->jump_y = (s16)(w->y >> 8);
    w->move_flags |= 0x10;
    w->move_flags |= 0x8000;
    w->move_flags &= ~1;
    w->state = dcp_state_in_air;
    if (vx != 0) {
        dcp_set_anim(w, 0x16);
        w->vel_x = vx;
        if (w->vel_x < 0) {
            if (w->gnd_vel > 0)
                w->gnd_vel = 0;
            w->disp_flags |= 1;
        } else {
            if (w->gnd_vel < 0)
                w->gnd_vel = 0;
            w->disp_flags &= ~1;
        }
    } else {
        dcp_set_anim(w, 0x13);
        w->disp_flags |= 4;
        w->vel_x = (s16)((w->gnd_vel * dcp_cos((u16)(w->angle << 8))) >> 12);
    }
    if (vy != 0)
        w->vel_y = vy;
    else
        w->vel_y = (s16)((w->gnd_vel * dcp_sin((u16)(w->angle << 8))) >> 12);
    atkdef(w, 1, 0, 0);
    w->pf &= ~0xf;
    w->pf |= 1;
    w->pf |= 2;
    if (vx != 0)
        w->dash_timer = 0x40;
    w->timer = 0;
    w->trick_input = 0;
    w->charge = 0;
    w->trick_cnt = 0;
    if (w->pf & 0x4000000) {
        w->vel_x = (w->vel_x >> 1) + (w->vel_x >> 2);
        w->vel_y = (w->vel_y >> 1) + (w->vel_y >> 2);
        fx(w, DCP_FX_AIR_BUBBLE, 0, 0);
    }
    sfx(w, DCP_CH_VOICE, 0x42);
}

/* Player_Action_SpringLaunch */
void dcp_act_spring(DcPlayer *w, s16 vx, s16 vy) {
    dcp_act_launch(w, vx, vy);
    sfx(w, DCP_CH_VOICE, 0x42);
}

/* Player_Action_DashPanel */
void dcp_act_dash_panel(DcPlayer *w, s16 vx, s16 vy) {
    u32 dir;
    w->dash_timer = 0x80;
    sfx(w, DCP_CH_VOICE, 0x40);
    if (vx < 0)
        w->disp_flags |= 1;
    else
        w->disp_flags &= ~1;
    if (w->move_flags & 0x10) {
        dir = w->disp_flags & 1;
        if ((dir && w->vel_x > vx) || (!dir && w->vel_x < vx))
            w->vel_x = vx;
        if (DCP_ABS(w->vel_y) < DCP_ABS(vy))
            w->vel_y = vy;
    } else {
        switch (((w->angle + 0x20) & 0xc0) >> 6) {
        case 0:
        case 2:
            dir = w->disp_flags & 1;
            if ((dir && w->gnd_vel > vx) || (!dir && w->gnd_vel < vx))
                w->gnd_vel = vx;
            if (DCP_ABS(w->vel_y) < DCP_ABS(vy)) {
                w->vel_y = vy;
                if (w->vel_y < 0)
                    w->move_flags |= 0x10;
            }
            break;
        case 1:
            dir = w->disp_flags & 1;
            if ((dir && w->gnd_vel > vy) || (!dir && w->gnd_vel < vy))
                w->gnd_vel = vy;
            if (DCP_ABS(w->vel_x) < DCP_ABS(vx))
                w->vel_x = vx;
            break;
        case 3:
            dir = w->disp_flags & 1;
            if ((dir && w->gnd_vel > -vy) || (!dir && w->gnd_vel < -vy))
                w->gnd_vel = -vy;
            if (DCP_ABS(w->vel_x) < DCP_ABS(vx))
                w->vel_x = vx;
            break;
        }
    }
    dcp_start_highspeed(w);
}

void dcp_act_knockback(DcPlayer *w, s16 vx, s16 vy) {
    dcp_player_init_state(w);
    dcp_set_anim(w, 0xf);
    w->vel_x = vx;
    w->vel_y = vy;
    w->gnd_vel = 0;
    if (vx < 0)
        w->disp_flags &= ~1;
    else
        w->disp_flags |= 1;
    w->move_flags |= 0x10;
    w->move_flags |= 0x8000;
    w->move_flags &= ~1;
    w->invuln = w->invuln_len;
    w->state = dcp_state_hurt;
}

void dcp_act_die(DcPlayer *w) {
    if (w->pf & 0x400)
        return;
    dcp_player_init_state(w);
    dcp_set_anim(w, 0x11);
    w->tension_max = 0;
    w->invinc = 0;
    w->pf &= ~0x40000;
    w->disp_flags &= ~1;
    w->move_flags |= 0x100;
    w->move_flags |= 0x200;
    w->vel_x = 0;
    w->gnd_vel = 0;
    w->vel_y = -w->jump;
    dcp_stop_highspeed(w);
    dcp_stop_boost(w);
    w->pf &= ~0x30000000;
    w->state = dcp_state_die;
    w->timer = 0;
    w->unk3CA = 0;
    w->pf |= 0x400;
    shake(4);
    sfx(w, DCP_CH_VOICE, (w->pf & 0x4000000) ? 0x52 : 6);
}

/* Player_OnHurt (hit_flags = attacker collision box unk20: 0x20 = instant kill,
 * 0x40 = extra sound). Ring scatter goes through dcp_scatter_rings with the
 * level kept by the caller (ring manager). */
static u8 s_scatter_level;
/* RingManager unk9D5: the ring manager owns it and resets it once every scattered ring is gone
 * (objects_init points this at it); the static stands in without a ring manager */
u8 *g_dcp_scatter_level;
#define SCATTER_LEVEL (g_dcp_scatter_level ? g_dcp_scatter_level : &s_scatter_level)
void dcp_on_hurt(DcPlayer *w, u32 hit_flags) {
    s32 v;
    u32 old;
    if (w->air_lock != 0)
        return;
    if (w->hitstop & 0xf)
        return;
    v = (w->move_flags & 0x8000) ? w->vel_x : w->gnd_vel;
    if (hit_flags & 0x40)
        sfx(w, DCP_CH_VOICE, 0x43);
    if (!(w->pf & 0x10000000)) {
        dcp_add_tension(w, -0x4b0);
        if (H.tension_hud)
            H.tension_hud(H.ctx, w, (u32)(w->tension << 12) >> 16);
    }
    if (hit_flags & 0x20) {
        if (w->rings != 0) {
            sfx(w, DCP_CH_VOICE, 5);
            dcp_scatter_rings(w, SCATTER_LEVEL);
        }
        dcp_act_die(w);
        return;
    }
    if (!(w->pf & 0x10000000) && w->rings == 0) {
        dcp_act_die(w);
        return;
    }
    old = w->pf2;
    dcp_player_init_state(w);
    w->pf2 |= old & 0x40000;
    dcp_set_anim(w, DCP_ABS(v) >= w->run_thr[2] ? 0x10 : 0xf);
    if (!(w->pf & 0x10000000)) {
        sfx(w, DCP_CH_VOICE, 5);
        dcp_scatter_rings(w, SCATTER_LEVEL);
    }
    w->pf &= ~0x30000000;
    shake(3);
    w->invuln = w->invuln_len;
    if (DCP_ABS(v) < 0x180)
        v = 0;
    w->vel_x = 0x180;
    w->vel_y = -0x300;
    w->gnd_vel = 0;
    if (v >= 0) {
        if (v > 0)
            w->vel_x = -w->vel_x;
        else if (!(w->disp_flags & 1))
            w->vel_x = -w->vel_x;
    }
    if (w->anim == 0x10)
        w->vel_x = (s16)(v >> 1);
    w->move_flags |= 0x10;
    w->move_flags |= 0x8000;
    w->move_flags &= ~1;
    w->state = dcp_state_hurt;
    sfx(w, DCP_CH_VOICE, w->chara != 1 ? 0x21 : 0x2a);
}

/* ==================================================================== states */

void dcp_state_ground_idle(DcPlayer *w) {
    u16 f;
    s32 x;
    if (w->anim == 4 && (w->disp_flags & 8)) {
        dcp_set_anim(w, 0x3c);
        w->disp_flags |= 4;
    }
    if (!(w->held & 0x40) && w->anim >= 0x21 && w->anim <= 0x22) {
        w->disp_flags |= 0x400;
        dcp_set_anim(w, 0x3c);
        w->disp_flags |= 4;
    }
    if (!(w->pf2 & 0x1100) && (w->held & 0x40)) {
        if (w->anim < 0x21 || w->anim > 0x22)
            dcp_set_anim(w, 0x21);
    } else if (w->anim >= 0xd && w->anim <= 0xe) {
        if (w->pid == 0 && !dcp_check_ledge_balance(w, DCP_FALSE)) {
            w->disp_flags |= 0x400;
            dcp_set_anim(w, 0x3c);
            w->disp_flags |= 4;
        }
    } else if (w->anim == 0x3c && dcp_check_ledge_balance(w, DCP_TRUE)) {
        w->disp_flags |= 4;
    }
    if (dcp_check_fall_off(w)) {
        w->disp_flags &= ~0x400;
        dcp_act_fall(w);
        return;
    }
    if ((w->pressed & 3) && w->act_jump != NULL) {
        w->disp_flags &= ~0x400;
        w->act_jump(w);
        return;
    }
    if (!(w->pf2 & 0x1100) && (w->held & 0x80) && w->act_crouch != NULL) {
        w->disp_flags &= ~0x400;
        w->act_crouch(w);
        return;
    }
    if (((w->angle + 0x20) & 0xff) >= 0x40)
        dcp_apply_slope_force(w, NULL);
    f = w->held;
    if (((f & 0x30) || w->gnd_vel != 0) &&
        (!(w->pf2 & 0x40000) || !(w->move_flags & 4) ||
         ((!(w->disp_flags & 1) || !(f & 0x20)) && ((w->disp_flags & 1) || !(f & 0x10)))) &&
        (!(w->move_flags & 0xc) ||
         (((x = w->x >> 8) > W.map_left + 0xe || !(f & 0x20)) && (x < W.map_right - 0xe || !(f & 0x10)))) &&
        w->act_move != NULL) {
        w->disp_flags &= ~0x400;
        w->act_move(w);
        return;
    }
    dcp_apply_vertical_input(w);
    if ((w->pf2 & 0x100) && w->vel_z != 0 && w->act_move != NULL) {
        w->disp_flags &= ~0x400;
        w->act_move(w);
        return;
    }
    /* StageObj_IsTouched(hurt box): pushed animation, object module (anim 0x45) */
    if (w->chara != 1) {
        if (w->anim != 0x45 || !(w->disp_flags & 8))
            return;
        w->disp_flags |= 0x400;
        dcp_set_anim(w, 0x3c);
        w->disp_flags |= 4;
        return;
    }
    if (w->anim != 0x45 || !(w->disp_flags & 8))
        return;
    dcp_set_anim(w, 0x47);
    w->disp_flags |= 4;
}

void dcp_state_ground_move(DcPlayer *w) {
    s32 a;
    u32 f;
    s16 st;

    if ((w->anim < 5 || w->anim > 10) &&
        (((w->held & 0x20) && w->gnd_vel >= 0x200) || ((w->held & 0x10) && w->gnd_vel <= -0x200))) {
        sfx_stop(w, DCP_CH_VOICE);
        sfx(w, DCP_CH_VOICE, w->chara != 1 ? 3 : 0x14);
        a = DCP_ABS(w->gnd_vel);
        dcp_set_anim(w, a > 0x700 ? 8 : 5);
    }
    if (w->anim == 5 && (w->disp_flags & 8)) {
        dcp_set_anim(w, 6);
        w->disp_flags |= 4;
    }
    if (w->anim == 8 && (w->disp_flags & 8)) {
        dcp_set_anim(w, 9);
        w->disp_flags |= 4;
    }
    if ((w->anim == 6 || w->anim == 9) && (w->gnd_vel <= 0 || !(w->held & 0x20)) &&
        (w->gnd_vel >= 0 || !(w->held & 0x10))) {
        sfx_stop(w, DCP_CH_VOICE);
        dcp_set_anim(w, w->anim == 6 ? 7 : 10);
    }
    if (w->anim == 0 || (w->anim >= 5 && w->anim <= 10)) {
        if (w->held & 0x20)
            w->pf |= 1;
        else if (w->held & 0x10)
            w->pf &= ~1;
    }
    if (dcp_check_fall_off(w)) {
        dcp_stop_skid(w);
        dcp_act_fall(w);
        return;
    }
    if (!(w->pf2 & 0x40000) && (st = w->anim) != 0 && (w->move_flags & 4)) {
        f = w->disp_flags;
        if ((((f & 1) && (w->held & 0x20)) || (!(f & 1) && (w->held & 0x10))) &&
            (w->x >> 8) > W.map_left + 0xe && (w->x >> 8) < W.map_right - 0xe) {
            /* pushing against a wall */
            if (st != 0xb && st != 0xc) {
                dcp_set_anim(w, 0xb);
            } else if (st == 0xb && (f & 8)) {
                dcp_set_anim(w, 0xc);
                w->disp_flags |= 4;
            }
            goto next;
        }
    }
    if (w->gnd_vel == 0 && w->vel_z == 0) {
        dcp_stop_skid(w);
        w->act_idle(w);
        return;
    }
    st = w->anim;
    if ((st != 0 || (w->disp_flags & 8)) && (st < 5 || st > 10 || ((st == 7 || st == 10) && (w->disp_flags & 8)))) {
        dcp_stop_skid(w);
        dcp_update_run_anim(w);
    }
next:
    dcp_apply_ground_input(w);
    if ((w->pressed & 3) && w->act_jump != NULL) {
        dcp_stop_skid(w);
        w->act_jump(w);
        return;
    }
    if (!(w->pf2 & 0x100) && (w->held & 0x80) && w->act_crouch != NULL) {
        dcp_stop_skid(w);
        w->act_crouch(w);
        return;
    }
    anim_speed(w, w->gnd_vel);
    if (w->anim >= 5 && w->anim <= 10) {
        if (!(W.objmgr & 0x100) && !(w->timer & 7))
            fx(w, DCP_FX_BRAKE_SMOKE, 0, 0);
        w->timer++;
    }
    if (W.objmgr & 0x100)
        return;
    if (w->chara != 1)
        return;
    a = DCP_ABS(w->gnd_vel);
    if (w->run_thr[1] >= a)
        return;
    if (w->pf & 0x100)
        return;
    if (!(W.flags & 1))
        return;
    if (W.frame & 3)
        return;
    fx(w, DCP_FX_BLAZE_FLAME, 0, 0);
}

void dcp_state_crouch(DcPlayer *w) {
    s16 v;
    if (((w->angle + 0x30) & 0xff) >= 0x60)
        dcp_apply_slope_force(w, NULL);
    if (dcp_check_fall_off(w)) {
        dcp_act_fall(w);
        return;
    }
    v = w->gnd_vel;
    if (v > 0x80 || v < -0x80) {
        dcp_act_roll(w);
        return;
    }
    if (v == 0 && w->vel_x == 0 && (w->pressed & 3)) {
        dcp_act_spindash(w);
        return;
    }
    if (!(w->held & 0x80))
        w->act_idle(w);
}

void dcp_state_roll(DcPlayer *w) {
    s32 t;
    if (dcp_check_fall_off(w)) {
        dcp_player_load_physics(w);
        dcp_act_fall(w);
        return;
    }
    if ((w->pressed & 3) && w->act_jump != NULL) {
        t = w->dash_timer;
        if (t >= 0x14)
            w->dash_timer = 0x14;
        dcp_player_load_physics(w);
        w->act_jump(w);
        return;
    }
    dcp_apply_roll_friction(w);
    t = w->gnd_vel;
    if (t < 0x80 && t > -128) {
        w->gnd_vel = 0;
        dcp_player_load_physics(w);
        w->act_idle(w);
        return;
    }
    anim_speed(w, w->gnd_vel);
    if (W.objmgr & 0x100)
        return;
    if (w->chara != 1)
        return;
    if (!(W.flags & 1))
        return;
    if (W.frame & 7)
        return;
    fx(w, DCP_FX_BLAZE_FLAME, 1, 0);
}

void dcp_state_spindash(DcPlayer *w) {
    u16 held;
    if (w->anim == 0x33 && (w->disp_flags & 8)) {
        dcp_set_anim(w, 0x32);
        w->disp_flags |= 4;
    }
    held = w->held;
    if (!(w->held & 0x80)) {
        /* release */
        w->dash_timer = 0x48;
        w->fx_timer = 8;
        w->gnd_vel = (s16)(((w->charge >> 8) << 7) + 0x800);
        if (w->disp_flags & 1)
            w->gnd_vel = -w->gnd_vel;
        w->move_flags |= 0x4000;
        dcp_act_roll(w);
        sfx(w, DCP_CH_VOICE, w->chara != 1 ? 1 : 0x12);
        return;
    }
    if (w->pressed & 3) {
        sfx_stop(w, DCP_CH_VOICE);
        sfx(w, DCP_CH_VOICE, w->chara != 1 ? 0 : 0x11);
        dcp_set_anim(w, 0x33);
        w->charge = (s16)dcp_spd_up(w->charge, w->sd_add, w->sd_max);
        if (!(w->pf2 & 0x1000) && !(w->pf & 1))
            fx(w, DCP_FX_SPIN_SMOKE2, 0, 0);
        w->pf |= 1;
        return;
    }
    if (held & 0x40) {
        w->act_idle(w);
        return;
    }
    anim_speed(w, (s16)(((w->charge >> 8) << 7) + 0x800));
    if (w->charge < (w->sd_base >> 1)) {
        if (w->pf & 1) {
            w->timer = 5;
            w->pf &= ~1;
        }
    }
    w->charge = (s16)dcp_spd_down(w->charge, w->charge >> 5);
    if (w->pf & 1)
        return;
    if (!(w->timer & 0xf))
        fx(w, DCP_FX_SPIN_SMOKE1, 0, 0);
    w->timer++;
}

void dcp_state_in_air(DcPlayer *w) {
    s16 v = w->vel_y;
    if (w->pf2 & 0x1000) {
        v = w->vel_z;
        if (w->rot2 > 0x80)
            v = -v;
    }
    dcp_settle_air_rotation(w);
    dcp_apply_air_input(w);
    if (w->timer != 0) {
        w->timer--;
        if (w->timer == 0)
            w->move_flags |= 0x80;
    }
    /* variable jump height: releasing A/B cuts the rise to -0x40 */
    if (!(w->pf & 1) && !(w->held & 3) && v < -0x40) {
        if (w->pf2 & 0x1000)
            w->vel_z = 0;
        else
            w->vel_y = -0x40;
    }
    if (v > 0)
        w->pf |= 1;
    switch (w->anim) {
    case 0x30:
        if (!(W.objmgr & 0x100) && w->chara == 1 && (W.flags & 1) && !(W.frame & 7))
            fx(w, DCP_FX_BLAZE_FLAME, 2, 0);
        break;
    case 1:
        if (w->disp_flags & 8) {
            dcp_set_anim(w, 2);
            w->disp_flags |= 4;
        }
        break;
    case 0x13:
        if (v > 0x40)
            dcp_set_anim(w, 0x14);
        break;
    case 0x14:
        if (w->disp_flags & 8) {
            dcp_set_anim(w, 0x15);
            w->disp_flags |= 4;
        }
        break;
    case 0x16:
        if (w->disp_flags & 8) {
            w->disp_flags |= 0x400;
            dcp_set_anim(w, 0x17);
            w->disp_flags |= 4;
        }
        break;
    case 0x18:
        if (v > 0x1c0) {
            w->disp_flags |= 0x400;
            dcp_set_anim(w, 0x19);
            w->disp_flags |= 4;
        }
        break;
    case 0x35:
        if (w->chara == 1) {
            if (!(w->flame_cnt & 3))
                fx(w, DCP_FX_BLAZE_FLAME, 3, w->flame_cnt);
            w->flame_cnt++;
        }
        if (w->disp_flags & 8)
            dcp_set_anim(w, 0x36);
        break;
    case 0x36:
        if (w->chara == 1) {
            if (v < -0x200 && !(w->flame_cnt & 3))
                fx(w, DCP_FX_BLAZE_FLAME, 3, w->flame_cnt);
            w->flame_cnt++;
        }
        if (v > 0x40 && (w->disp_flags & 8)) {
            w->disp_flags |= 0x400;
            dcp_set_anim(w, 2);
            w->disp_flags |= 4;
        }
        break;
    case 0x38:
        if (!(W.frame & 7))
            fx(w, DCP_FX_TRICK, 0xf - (dcp_rand() & 0x1e), 0xf - (dcp_rand() & 0x1e));
        if (w->disp_flags & 8) {
            dcp_set_anim(w, 2);
            w->disp_flags |= 4;
        }
        break;
    case 0x37:
        if (w->chara == 1 && w->air_lock != 0) {
            w->disp_flags ^= 0x20;
            w->vel_y = 0;
            w->air_lock--;
            if (w->air_lock == 0) {
                w->disp_flags &= ~0x20;
                w->vel_x = w->saved_vx;
                w->hitstop = 0x44;
            }
        }
        break;
    default:
        break;
    }
    if (w->move_flags & 1) {
        dcp_act_land(w, 0);
        w->act_idle(w);
        return;
    }
    if (w->pf & 2) {
        if (!(w->pf & 8) && (w->anim < 0x35 || w->anim > 0x37)) {
            if ((w->pressed & 0x100) && (w->held & 0x40)) {
                dcp_act_trick_up(w);
                return;
            }
            if (w->pressed & 0x100) {
                dcp_act_trick_forward(w);
                return;
            }
        }
        if (w->pressed & 3)
            w->trick_input = w->pressed;
        if (w->anim >= 0x37 && w->anim <= 0x3b) {
            if (w->anim == 0x37 && w->chara == 1 && v < 0)
                w->charge = 0;
            w->charge++;
            if (w->charge > 0xff)
                w->charge = 0xff;
        }
        if (w->pf & 4)
            return;
        if (w->anim >= 0x37 && w->charge <= 0x18)
            return;
        if (w->trick_input == 0)
            return;
        dcp_act_trick_combo(w);
    } else {
        if (w->anim >= 0x18 && w->anim != 0x30)
            return;
        if (!(w->pressed & 0x100))
            return;
        if (w->act_air == NULL)
            return;
        if (!(w->pf2 & 0x1000))
            w->move_flags |= 0x80;
        w->act_air(w);
    }
}

void dcp_state_homing(DcPlayer *w) {
    s32 i, v;
    if (w->timer == 0) {
        w->move_flags |= 0x80;
        w->pf &= ~0x40;
        dcp_act_fall(w);
        return;
    }
    w->timer--;
    if (w->move_flags & 1) {
        w->move_flags |= 0x80;
        w->pf &= ~0x40;
        dcp_act_land(w, 0);
        w->act_idle(w);
        return;
    }
    if (w->homing == NULL)
        return;
    i = (u16)(dcp_atan2((w->homing->y - w->y) >> 8, (w->homing->x - w->x) >> 8) + (w->grav_rot << 8));
    w->vel_x = dcp_cos((u16)i) >> 1;
    w->vel_y = dcp_sin((u16)i) >> 1;
    if (w->vel_x < 0)
        w->disp_flags |= 1;
    else if (w->vel_x > 0)
        w->disp_flags &= ~1;
    v = DCP_ABS(w->vel_x);
    if (v <= 0x100)
        return;
    if (!(w->move_flags & 4))
        return;
    w->move_flags |= 0x80;
    w->pf &= ~0x40;
    dcp_act_land(w, 0);
    w->act_idle(w);
}

void dcp_state_hover(DcPlayer *w) {
    if (!(w->pf2 & 0x1000) && !(W.frame & 7) && (W.flags & 1))
        fx(w, DCP_FX_BOOST_JET, 0, 0);
    if (w->hover_timer != 0)
        w->hover_timer--;
    dcp_settle_air_rotation(w);
    dcp_apply_air_input(w);
    if (!(w->held & 0x100) || w->hover_timer == 0) {
        dcp_player_load_physics(w);
        w->gnd_vel = w->vel_x;
        w->disp_flags |= 0x400;
        dcp_act_fall(w);
        return;
    }
    if (!(w->move_flags & 1))
        return;
    dcp_player_load_physics(w);
    dcp_act_land(w, 0);
    w->act_idle(w);
}

void dcp_state_hurt(DcPlayer *w) {
    dcp_settle_air_rotation(w);
    if (w->obj_flags & 2)
        return;
    if (!(w->move_flags & 1))
        return;
    dcp_act_land(w, 0);
    w->act_idle(w);
}

void dcp_state_die(DcPlayer *w) {
    w->timer++;
    if (w->anim == 0x11 && (w->disp_flags & 8)) {
        dcp_set_anim(w, 0x12);
        w->disp_flags |= 4;
    }
    if (w->timer == 0x3c && H.fade)
        H.fade(H.ctx, 4, (W.zone >= 9 || W.act >= 2) ? 0x40 : 0x100);
    if (w->timer < 0x3c)
        return;
    if (H.fade_done && !H.fade_done(H.ctx))
        return;
    if (H.fade)
        H.fade(H.ctx, 2, 0x100);
    w->pf &= ~0x400;
    dcp_respawn(w);
    w->act_idle(w);
}
