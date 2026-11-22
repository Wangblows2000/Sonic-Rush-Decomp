/*
 * Player gimmick states of Dead Line, copied from src/game/Player/ and
 * adapted like obj_pwater.c / obj_pmirage.c:
 *   Player_Action_Swing, Player_State_Swing, Player_SwingAroundPivot,
 *   Player_SwingAroundPivotRetract (Swing, 0x96),
 *   Player_Action_Cannon, Player_State_Cannon (Cannon, 0x95),
 *   Player_Action_Rocket, Player_State_Rocket (Rocket, 0x93).
 * Player work @0x3F0..@0x3FC lives in ObjPlayerCtx.g3F0..g3FC.
 */
#include "obj_zone.h"

/* ------------------------------------------------------------ Swing */
void Player_SwingAroundPivot(DcPlayer *w) {
    ObjPlayerCtx *c = z_ctx(w);
    Obj *o = (Obj *)w->gimmick;
    s32 r;
    u16 i;
    c->g3F4 -= w->gnd_vel;
    if (w->gnd_vel > 0) {
        w->gnd_vel = (s16)dcp_spd_up(w->gnd_vel, 0x18, w->gnd_max);
        if (c->g3F4 < c->g3F0)
            c->g3F4 = c->g3F0;
    } else {
        w->gnd_vel = (s16)dcp_spd_up(w->gnd_vel, -0x18, w->gnd_max);
        if (c->g3F4 > c->g3F0)
            c->g3F4 = c->g3F0;
    }
    if (w->land_speed < (s16)ZABS(w->gnd_vel)) {
        w->land_speed = (s16)ZABS(w->gnd_vel);
        if (w->land_speed > w->gnd_max)
            w->land_speed = w->gnd_max;
    }
    w->angle = (u8)((c->g3F4 & 0xffff) >> 8);
    w->px = w->x;
    w->py = w->y;
    r = c->g3FC;
    i = (u16)((u8)(w->angle + w->grav_rot) << 8);
    w->x = o->h.x - (s16)((r * ZSIN(i)) >> 12);
    w->y = o->h.y + (s16)((r * ZCOS(i)) >> 12);
    w->move_x = (s16)(w->x - w->px);
    w->move_y = (s16)(w->y - w->py);
}
void Player_SwingAroundPivotRetract(DcPlayer *w) {
    ObjPlayerCtx *c = z_ctx(w);
    Obj *o = (Obj *)w->gimmick;
    s16 vx, vy;
    c->g3F4 -= w->gnd_vel;
    if (w->timer > 0) {
        w->gnd_vel = (s16)dcp_spd_up(w->gnd_vel, -0x18, w->gnd_max);
        if (w->gnd_vel < 0x300 && !(w->pf & 2)) {
            w->pf |= 2;
            w->vel_y = -0x300;
            FixInformation_CreateSwing(w);
        }
        if ((w->pf & 4) && c->g3F4 > c->g3F0)
            c->g3F4 = c->g3F0;
    } else {
        w->gnd_vel = (s16)dcp_spd_up(w->gnd_vel, 0x18, w->gnd_max);
        if (w->gnd_vel > -0x300 && !(w->pf & 2)) {
            w->pf |= 2;
            w->vel_y = -0x300;
            FixInformation_CreateSwing(w);
        }
        if ((w->pf & 4) && c->g3F4 < c->g3F0)
            c->g3F4 = c->g3F0;
    }
    w->angle = (u8)((c->g3F4 & 0xffff) >> 8);
    c->g3F8 -= w->vel_y;
    if ((w->pf & 2) && !(w->pf & 4)) {
        w->rot3 = (u8)(w->rot3 + 8);
        if (w->rot3 > 0x80)
            w->rot3 = 0x80;
        w->vel_y = (s16)dcp_spd_up(w->vel_y, 0x2a, w->gnd_max);
        if (c->g3F8 < 0) {
            c->g3F8 = 0;
            w->vel_y = 0;
            w->rot3 = 0;
            w->disp_flags ^= 1;
            w->pf |= 4;
            z_psfx(w, DCP_CH_VOICE, 0x62);
        }
    }
    w->px = w->x;
    w->py = w->y;
    vx = (s16)(c->g3F8 >> 8);
    if (w->timer <= 0)
        vx = (s16)-vx;
    vy = (s16)(c->g3FC >> 8);
    dcp_rotate_vec(&vx, &vy, (u8)(w->angle + w->grav_rot));
    w->x = o->h.x + (vx << 8);
    w->y = o->h.y + (vy << 8);
    w->move_x = (s16)(w->x - w->px);
    w->move_y = (s16)(w->y - w->py);
}
void Player_Action_Swing(DcPlayer *w, Obj *o, s32 unused, s32 r) {
    ObjPlayerCtx *c = z_ctx(w);
    (void)unused;
    if (w->state == Player_State_Swing)
        return;
    dcp_reset_gimmick_state(w);
    w->gimmick = o;
    dcp_set_anim(w, 0x2c);
    w->disp_flags |= 4;
    w->state = Player_State_Swing;
    w->move_flags |= 0x10;
    w->move_flags |= 0x2000;
    w->move_flags |= 0x100;
    w->move_flags &= ~1u;
    w->move_flags &= ~0x80u;
    w->pf &= ~7u;
    w->pf |= 0x100000;
    c->g3F0 = w->gnd_vel;
    w->vel_x = 0;
    w->vel_y = 0;
    w->trick_input = 0;
    w->timer = 0;
    if (w->angle > 0x40 && w->angle < 0xc0) {
        w->pf |= 1;
        if (w->gnd_vel > 0)
            c->g3F0 = 0x10000;
        else
            c->g3F0 = 0x20000;
    } else {
        c->g3F0 = 0x18000;
        if (w->gnd_vel < 0)
            c->g3F0 = 0x8000;
        if (w->gnd_vel > 0x400)
            w->gnd_vel = 0x400;
        if (w->gnd_vel < -0x400)
            w->gnd_vel = -0x400;
        w->timer = w->gnd_vel;
    }
    c->g3F4 = (w->angle << 8) + 0x10000;
    c->g3FC = r << 8;
    z_psfx(w, DCP_CH_VOICE, 0x62);
}
void Player_State_Swing(DcPlayer *p0) {
    ObjPlayerCtx *c = z_ctx(p0);
    if (p0->gimmick != NULL) {
        if (p0->pf & 1) {
            Player_SwingAroundPivot(p0);
            if (c->g3F4 == c->g3F0)
                p0->gimmick = NULL;
        } else {
            Player_SwingAroundPivotRetract(p0);
            if (c->g3F4 == c->g3F0)
                p0->gimmick = NULL;
        }
    }
    if (p0->gimmick != NULL) {
        if ((p0->pressed & 3) == 0)
            return;
    }
    p0->rot3 = 0;
    p0->gimmick = NULL;
    p0->move_flags |= 0x80;
    p0->move_flags &= ~0x2000u;
    p0->move_flags &= ~0x100u;
    p0->pf &= ~0x100000u;
    if ((p0->pressed & 3) != 0) {
        p0->angle = 0;
        p0->gnd_vel = 0;
        z_call(p0->act_jump, p0);
        return;
    }
    if ((p0->pf & 1) == 0)
        z_psfx(p0, DCP_CH_VOICE, 0x40);
    p0->move_flags |= 1;
    z_call(p0->act_idle, p0);
}

/* ------------------------------------------------------------ Cannon */
void Player_Action_Cannon(DcPlayer *w, Obj *o) {
    ObjPlayerCtx *c = z_ctx(w);
    s32 a, dx, dy;
    if (w->state == Player_State_Cannon)
        return;
    dcp_reset_gimmick_state(w);
    w->gimmick = o;
    w->disp_flags |= 4;
    w->state = Player_State_Cannon;
    w->move_flags |= 0x2000;
    w->move_flags |= 0x100;
    w->pf &= ~7u;
    w->pf |= 0x100000;
    w->pf |= 0x2000;
    w->pf2 |= 0x30;
    a = w->vel_x;
    if (a < 0)
        a = -a;
    c->g3F0 = a;
    if (c->g3F0 < ZABS(w->gnd_vel))
        c->g3F0 = ZABS(w->gnd_vel);
    if (c->g3F0 < ZABS(w->vel_y))
        c->g3F0 = ZABS(w->vel_y);
    w->vel_x = 0;
    w->vel_y = 0;
    w->gnd_vel = 0;
    w->angle = 0;
    dx = (o->h.x - w->x) >> 8;
    if (dx < 0)
        dx = -dx;
    dy = (o->h.y - w->y) >> 8;
    if (dy < 0)
        dy = -dy;
    dx = dx * dx << 12;
    dy = dy * dy << 12;
    w->timer = (s32)(FX_Sqrt(dx + dy) >> 4);
    c->g3F8 = dcp_atan2(o->h.y - w->y, o->h.x - w->x);
    w->angle = (u8)((c->g3F8 & 0xffff) >> 8);
    w->trick_input = 0;
    if (w->chara != 1)
        z_psfx(w, DCP_CH_VOICE, 0);
    else
        z_psfx(w, DCP_CH_VOICE, 0x11);
    z_psfx(w, DCP_CH_BOOST, 0x60); /* (handle @0x388) */
}
void Player_State_Cannon(DcPlayer *w) {
    ObjPlayerCtx *c = z_ctx(w);
    Obj *o = (Obj *)w->gimmick;
    s16 vx, vy, t;
    s32 sx, sy = 0;
    if (o != NULL) {
        c->g3F0 = (s16)dcp_spd_up(c->g3F0, 0x10, 0x400);
        w->timer -= c->g3F0 >> 2;
        if (w->timer < 0) {
            w->timer = 0;
            w->trick_input++;
            w->pf &= ~0x2000u;
            w->pf2 &= ~0x30u;
        }
        c->g3F8 -= c->g3F0 << 2;
        w->angle = (u8)((c->g3F8 & 0xffff) >> 8);
        w->px = w->x;
        w->py = w->y;
        w->x = o->h.x - ((w->timer * ZCOS((u16)(w->angle << 8))) >> 12);
        w->y = o->h.y - ((w->timer * ZSIN((u16)(w->angle << 8))) >> 12);
        w->move_x = (s16)(w->x - w->px);
        w->move_y = (s16)(w->y - w->py);
    }
    if (o != NULL) {
        if (!(w->pressed & 3) && (u32)w->trick_input <= 0x258)
            return;
        if (w->timer != 0)
            return;
    }
    vx = 0x38;
    vy = 0;
    z_psfx_stop(w, DCP_CH_BOOST);
    sx = 0xa00;
    if (o != NULL) {
        w->angle = o->h.angle;
        t = (s16)sx;
        sx = (s16)((ZCOS((u16)(w->angle << 8)) * t) >> 12);
        sy = (s16)((ZSIN((u16)(w->angle << 8)) * t) >> 12);
        dcp_rotate_vec(&vx, &vy, (u8)(w->angle + w->grav_rot));
        EffZakoBombPuff_Create((Obj *)(void *)w, vx, vy); /* (reads only the StageObj header @0x00, = the DcPlayer) */
    }
    w->rot3 = 0;
    w->gimmick = NULL;
    w->move_flags &= ~0x2000u;
    w->move_flags &= ~0x100u;
    w->pf &= ~0x2000u;
    w->pf &= ~0x100000u;
    dcp_start_highspeed(w);
    Player_Action_GimmickLaunch(w, (s16)sx, (s16)sy);
    dcp_set_anim(w, 0x30);
    w->disp_flags |= 4;
    w->timer = 0x1e;
    w->move_flags &= ~0x80u;
    obj_screen_shake(4);
    z_fxhook(w, OBJ_FX_SCREEN_FLASH, 0, 0);
    w->fx_timer = 8;
    z_psfx(w, DCP_CH_VOICE, 0x61);
}

/* ------------------------------------------------------------ Rocket */
void Player_Action_Rocket(DcPlayer *w, Obj *o, s32 x, s32 y) {
    ObjPlayerCtx *c = z_ctx(w);
    if (w->state == Player_State_Rocket)
        return;
    dcp_reset_gimmick_state(w);
    w->gimmick = o;
    dcp_set_anim(w, 0x25);
    w->disp_flags |= 4;
    w->state = Player_State_Rocket;
    w->move_flags |= 0x10;
    w->move_flags |= 0x8000;
    w->move_flags |= 0x2000;
    w->move_flags &= ~0x80u;
    w->disp_flags &= ~1;
    w->disp_flags |= o->h.disp_flags & 1;
    w->disp_flags ^= 1;
    w->pf |= 0x100000;
    w->vel_x = 0;
    w->vel_y = 0;
    w->gnd_vel = 0;
    w->angle = 0;
    w->trick_input = 0;
    w->timer = 0;
    c->g3F0 = x << 8;
    c->g3F4 = y << 8;
    c->g3F8 = 0;
    c->g3FC = 0;
    if (w->grav_rot != 0)
        c->g3F0 = -c->g3F0;
    w->px = w->x;
    w->py = w->y;
    w->x = o->h.x + c->g3F0;
    w->y = o->h.y + c->g3F4;
    w->fx_timer = 8;
}
void Player_State_Rocket(DcPlayer *w) {
    ObjPlayerCtx *c = z_ctx(w);
    Obj *o = (Obj *)w->gimmick;
    u16 m1, m2;
    s32 r;
    if (o != NULL) {
        m1 = 0xa0;
        m2 = 0x50;
        if (!(o->h.disp_flags & 1)) {
            m1 ^= m2;
            m2 ^= m1;
            m1 ^= m2;
        }
        w->px = w->x;
        w->py = w->y;
        r = c->g3F0;
        w->x = o->h.x + (s16)((r * ZCOS((u16)(w->angle << 8))) >> 12);
        w->y = o->h.y + (s16)((r * ZSIN((u16)(w->angle << 8))) >> 12);
        if (w->hitstop == 0)
            w->hitstop = 0x40;
        if (w->held & m1)
            c->g3FC = (s16)dcp_spd_up(c->g3FC, 0x10, 0x180);
        else if (c->g3FC > 0)
            c->g3FC = (s16)dcp_spd_down(c->g3FC, 8);
        if (w->held & m2)
            c->g3FC = (s16)dcp_spd_up(c->g3FC, -0x10, 0x180);
        else if (c->g3FC < 0)
            c->g3FC = (s16)dcp_spd_down(c->g3FC, 8);
        if (w->disp_flags & 1)
            c->g3F8 -= c->g3FC;
        else
            c->g3F8 += c->g3FC;
        w->angle = (u8)(c->g3F8 >> 8);
        if (o->h.move_flags & 0x2000) {
            w->vel_x = o->h.move_x;
            w->vel_y = o->h.move_y;
            w->gimmick = NULL;
        }
    }
    if (w->gimmick != NULL && !(w->pressed & 3))
        return;
    if (w->act_jump == NULL)
        return;
    if (w->act_idle == NULL)
        return;
    w->gimmick = NULL;
    w->move_flags |= 0xc0;
    w->move_flags &= ~0x2000u;
    w->move_flags &= ~0x4000u;
    w->pf &= ~0x100000u;
    w->pf2 &= ~0x20000u;
    w->angle = 0;
    if (w->pressed & 3) {
        w->act_jump(w);
        return;
    }
    dcp_act_land(w, 0);
    w->act_idle(w);
}

/* ------------------------------------------------------- Air (0x94) */
void Player_Action_Air(DcPlayer *w, Obj *o, s32 x0, s32 y, s32 x1) {
    ObjPlayerCtx *c = z_ctx(w);
    if (w->state == Player_State_Air) {
        if (x0 - 0x100 < c->g3F0)
            c->g3F0 = x0 - 0x100;
        if (x1 + 0x100 > c->g3F8)
            c->g3F8 = x1 + 0x100;
        if (y < c->g3F4 || c->g3F4 == 0)
            c->g3F4 = y;
        return;
    }
    if (w->y < y && !(o->h.disp_flags & 2))
        return;
    if (w->y > y && (o->h.disp_flags & 2))
        return;
    dcp_reset_gimmick_state(w);
    dcp_set_anim(w, 0x2b);
    if (w->move_flags & 1)
        w->vel_x = w->gnd_vel;
    w->move_flags |= 0x8000;
    w->disp_flags |= 4;
    w->move_flags |= 0x10;
    w->move_flags &= ~0x80u;
    w->gimmick = o;
    c->g3F0 = x0 - 0x100;
    c->g3F4 = y;
    c->g3F8 = x1 + 0x100;
    w->state = Player_State_Air;
}
void Player_State_Air(DcPlayer *w) {
    ObjPlayerCtx *c = z_ctx(w);
    Obj *o = (Obj *)w->gimmick;
    s32 v;
    dcp_apply_air_input(w);
    if (o != NULL) {
        if ((w->y > c->g3F4 && !(o->h.disp_flags & 2)) || (w->y < c->g3F4 && (o->h.disp_flags & 2))) {
            w->vel_y = (s16)dcp_spd_up(w->vel_y, -0x32, 0x480);
        } else {
            if (w->vel_y < 0) {
                v = w->move_y;
                if (v < 0)
                    v = -v;
                if (v > 0x200)
                    w->vel_y = (s16)dcp_spd_down(w->vel_y, 0x68);
            }
            w->vel_y = (s16)dcp_spd_up(w->vel_y, 0x32, 0x480);
        }
        if (w->x <= c->g3F0 || w->x >= c->g3F8)
            w->gimmick = o = NULL;
    }
    if (o != NULL) {
        c->g3F4 = 0;
        return;
    }
    w->move_flags |= 0x80;
    w->move_flags |= 0x10;
    w->move_flags |= 0x8000;
    w->gimmick = NULL;
    dcp_act_land(w, 0);
    z_call(w->act_idle, w);
}

/* ---------------------------------- Player_Unk_ov000_02350adc (MapObj9C depth lane) */
void Player_Unk_ov000_02350adc(DcPlayer *w, Obj *o) {
    s32 t;
    u16 m1, m2;
    w->pf2 |= 0x1000;
    t = 0x40;
    w->move_flags &= ~0x80u;
    w->move_flags |= 0x100;
    if (w->pf2 & 0x100000) {
        if (w->move_flags & 0x10) {
            if (w->held & 0xc0) {
                m1 = (u16)t;
                m2 = 0x80;
                if (w->grav_rot != 0) {
                    m1 = 0x80;
                    m2 = (u16)t;
                }
                if (w->held & m1) {
                    t = -0x40;
                    if (w->vel_y > 0)
                        w->vel_y = (s16)dcp_spd_down(w->vel_y, 0x20);
                }
                if ((w->held & m2) && w->vel_y < 0)
                    w->vel_y = (s16)dcp_spd_down(w->vel_y, 0x20);
                w->vel_y = (s16)dcp_spd_up(w->vel_y, t, 0x200);
            } else {
                w->vel_y = (s16)dcp_spd_down(w->vel_y, 0x20);
            }
        }
    } else if (!(w->move_flags & 1) && w->rot2 != 0x40 && w->rot2 != 0xc0) {
        if (w->grav_rot != 0) {
            if (o->h.y > w->y)
                t = -0x40;
        } else {
            if (o->h.y <= w->y)
                t = -0x40;
        }
        w->vel_y = (s16)dcp_spd_up(w->vel_y, t, 0x200);
    }
    if (o->h.z > 0) {
        w->rot2 = dcp_angle_toward8(w->rot2, 0x40, 2);
        w->vel_z = (s16)dcp_spd_up(w->vel_z, w->gravity, w->fall_max);
        if (w->z < o->h.z) {
            w->move_flags &= ~1u;
            return;
        }
        w->z = o->h.z;
        w->vel_z = 0;
        w->move_flags |= 1;
        w->pf2 |= 0x100000;
        return;
    }
    w->rot2 = dcp_angle_toward8(w->rot2, 0xc0, 2);
    w->vel_z = (s16)dcp_spd_up(w->vel_z, -w->gravity, w->fall_max);
    if (w->z > o->h.z) {
        w->move_flags &= ~1u;
        return;
    }
    w->z = o->h.z;
    w->vel_z = 0;
    w->vel_y = 0;
    w->move_flags |= 1;
    w->pf2 |= 0x100000;
}
