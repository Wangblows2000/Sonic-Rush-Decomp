/*
 * Player gimmick states of Huge Crisis, copied from src/game/Player/ and
 * adapted like obj_pwater.c / obj_pmirage.c:
 *   Player_Action_Pipe, Player_State_Pipe, Player_Action_PipeExit,
 *   Player_Action_JetCatapult, Player_State_JetCatapult,
 *   Player_Action_JetCatapultLaunch, Player_State_JetCatapultLaunch.
 * (Player_Action_GrabCranePart is in obj_pcarnival.c next to
 * Player_Action_GrabCraneSlide; AttackHitbox_SetJetCatapultState and
 * AttackHitbox_State_JetCatapult are in obj_player.c.)
 */
#include "obj_zone.h"

/* -------------------------------------------------------------- Pipe */
void Player_Action_Pipe(DcPlayer *p0, Obj *p1, u8 p2) {
    ObjPlayerCtx *c = z_ctx(p0);
    if (p0->gimmick == (void *)p1)
        return;
    obj_init_state(p0);
    p0->gimmick = p1;
    dcp_set_anim(p0, 0x31);
    p0->disp_flags |= 4;
    p0->state = Player_State_Pipe;
    p0->disp_flags &= ~1;
    p0->move_flags |= 0x10;
    p0->move_flags &= ~0x80u;
    p0->move_flags &= ~0x40000u;
    p0->move_flags &= ~0x20000u;
    p0->move_flags |= 0x200;
    p0->move_flags |= 0x4000;
    p0->move_flags |= 0x100;
    p0->pf &= ~1u;
    dcp_stop_boost(p0);
    p0->pf |= 0x100000;
    p0->gnd_vel = 0;
    p0->vel_x = 0;
    p0->vel_y = 0;
    p0->angle = 0;
    p0->trick_input = 0;
    p0->timer = 0;
    c->g3F0 = p2;
    c->g3F4 = p0->x;
    c->g3F8 = p0->y;
    if (p0->chara != 1) {
        z_psfx(p0, DCP_CH_VOICE, 0);
        return;
    }
    z_psfx(p0, DCP_CH_VOICE, 0x11);
}
static void pipe_arrive(DcPlayer *w) {
    ObjPlayerCtx *c = z_ctx(w);
    w->move_flags &= ~0x10u;
    w->move_flags &= ~0x100u;
    w->trick_input = 1;
    w->angle = (u8)c->g3F0;
}
void Player_State_Pipe(DcPlayer *w) {
    ObjPlayerCtx *c = z_ctx(w);
    Obj *g = (Obj *)w->gimmick;
    s32 d;
    switch (w->trick_input) {
    case 0:
        if (g != NULL) {
            w->px = w->x;
            w->py = w->y;
            if (c->g3F0 == 0 || c->g3F0 == 0x80) {
                w->y = ObjShift_SetFromRef(w->y, g->h.y, c->g3F8, 1, 0x400, 0x100);
                d = w->y - g->h.y;
                if (d < 0)
                    d = -d;
                if (d < 0x800)
                    w->x = ObjShift_SetFromRef(w->x, g->h.x, c->g3F4, 1, 0x400, 0x100);
            } else {
                w->x = ObjShift_SetFromRef(w->x, g->h.x, c->g3F4, 1, 0x400, 0x100);
                d = w->x - g->h.x;
                if (d < 0)
                    d = -d;
                if (d < 0x800)
                    w->y = ObjShift_SetFromRef(w->y, g->h.y, c->g3F8, 1, 0x400, 0x100);
            }
            if (g->h.x != w->x)
                return;
            if (g->h.y != w->y)
                return;
            pipe_arrive(w);
        } else {
            pipe_arrive(w);
        }
        return;
    case 1:
        if (!(w->move_flags & 1)) {
            w->angle = (u8)(w->angle + 0x80);
            w->pf ^= 1;
        }
        w->timer++;
        if (w->timer <= 0xc)
            return;
        if (!(w->move_flags & 1))
            return;
        w->gnd_vel = 0x800;
        if (w->pf & 1)
            w->gnd_vel = (s16)-w->gnd_vel;
        w->trick_input = 2;
        z_psfx(w, DCP_CH_VOICE, 0x40);
        return;
    default:
        if (w->pf & 1)
            w->gnd_vel = (s16)dcp_spd_up(w->gnd_vel, -0x10, 0xd00);
        else
            w->gnd_vel = (s16)dcp_spd_up(w->gnd_vel, 0x10, 0xd00);
        z_anim_speed(w, w->gnd_vel);
        return;
    }
}
void Player_Action_PipeExit(DcPlayer *w, s16 spd) {
    u8 dir;
    s32 t;
    if (w->state != Player_State_Pipe)
        return;
    dir = (u8)(((u8)(w->angle - 0x20) & 0xc0) >> 6);
    if (w->pf & 1)
        dir = (u8)((dir + 2) & 3);
    if (spd == 0) {
        t = w->gnd_vel;
        if (t < 0)
            t = -t;
        spd = (s16)t;
    }
    w->gimmick = NULL;
    w->move_flags &= ~0x2000u;
    w->move_flags |= 0x80;
    w->move_flags |= 0x40000;
    w->move_flags |= 0x20000;
    w->move_flags &= ~0x200u;
    w->angle = 0;
    w->move_flags &= ~0x100u;
    w->move_flags &= ~0x4000u;
    w->pf &= ~0x100000u;
    z_psfx(w, DCP_CH_VOICE, 0x40);
    w->gnd_vel = 0;
    z_call(w->act_jump, w);
    switch (dir) {
    case 0:
    default:
        w->vel_y = spd;
        break;
    case 1:
        w->disp_flags |= 1;
        w->vel_x = (s16)-spd;
        w->vel_y = 0;
        break;
    case 2:
        w->vel_y = (s16)-spd;
        break;
    case 3:
        w->vel_x = spd;
        w->vel_y = 0;
        break;
    }
    w->pf |= 1;
}

/* --------------------------------------------------------- JetCatapult */
void Player_Action_JetCatapult(DcPlayer *p0, Obj *p1) {
    ObjPlayerCtx *c = z_ctx(p0);
    Obj *hb;
    if (p0->gimmick == (void *)p1)
        return;
    obj_init_state(p0);
    p0->gimmick = p1;
    dcp_set_anim(p0, 3);
    p0->state = Player_State_JetCatapult;
    p0->move_flags &= ~0x200000u;
    p0->pf &= ~0xfu;
    p0->pf |= 0x100000;
    p0->move_flags |= 0x10000000;
    p0->move_flags |= 0x100;
    p0->move_flags |= 0x2000;
    p0->move_flags &= ~0x80u;
    p0->disp_flags &= ~1;
    p0->x = p1->h.x;
    p0->y = p1->h.y - 0xc00;
    p0->gnd_vel = 0;
    p0->vel_x = 0;
    p0->vel_y = 0;
    p0->angle = 0;
    p0->trick_input = 0;
    p0->timer = 0;
    c->g3F0 = 0;
    z_psfx(p0, DCP_CH_VOICE, 0x6d);
    hb = AttackHitbox_Create(p0, -32, -32, 0x20, 0x20, 0);
    AttackHitbox_SetJetCatapultState(hb);
}
void Player_Action_JetCatapultLaunch(DcPlayer *w) {
    Obj *g = (Obj *)w->gimmick;
    w->move_flags |= 0x200000;
    w->move_flags |= 0x80;
    w->move_flags &= ~0x10000000u;
    w->move_flags &= ~0x100u;
    w->move_flags &= ~0x2000u;
    if (!(w->move_flags & 0x10))
        w->jump_y = (s16)(w->y >> 8);
    w->move_flags |= 0x10;
    w->move_flags |= 0x8000;
    w->move_flags &= ~1u;
    w->state = Player_State_JetCatapultLaunch;
    dcp_set_anim(w, 0x18);
    w->disp_flags |= 4;
    w->vel_x = (s16)(g != NULL ? g->h.vel_x << 1 : 0);
    w->vel_y = -0x680;
    w->angle = 0;
    w->timer = 0;
    w->trick_input = 0;
    w->charge = 0;
    w->trick_cnt = 0;
    w->fx_timer = 8;
}
void Player_State_JetCatapult(DcPlayer *p0) {
    Obj *g = (Obj *)p0->gimmick;
    if (g == NULL)
        return;
    p0->px = p0->x;
    p0->py = p0->y;
    p0->x = g->h.x;
    p0->y = g->h.y - 0xc00;
    p0->angle = g->h.angle;
    if ((p0->pf & 1) == 0) {
        if (g->h.gnd_vel == 0)
            return;
        p0->fx_timer = 0x10;
        p0->pf |= 1;
        return;
    }
    if ((g->h.move_flags & 0x100) == 0)
        return;
    Player_Action_JetCatapultLaunch(p0);
}
void Player_State_JetCatapultLaunch(DcPlayer *w) {
    s16 vy = w->vel_y;
    if (w->pressed & 3)
        w->trick_input = w->pressed;
    if (w->anim >= 0x39 && w->anim <= 0x3b) {
        w->charge++;
        if (w->charge > 0xff)
            w->charge = 0xff;
    }
    if (!(w->pf & 4) && (w->anim < 0x39 || w->charge > 0x18) && w->trick_input != 0)
        dcp_act_trick_combo(w);
    if (w->timer != 0 && vy > -0x30) {
        w->vel_y = 0;
        w->move_flags |= 0x2000;
        w->move_flags &= ~0x80u;
        w->timer--;
        if (w->timer == 0) {
            w->move_flags &= ~0x2000u;
            w->move_flags |= 0x80;
        }
    }
    if (w->anim == 0x18 && vy > 0x40) {
        w->disp_flags |= 0x400;
        dcp_set_anim(w, 0x19);
        w->disp_flags |= 4;
    }
    if (!(w->move_flags & 1))
        return;
    w->pf &= ~0x100000u;
    dcp_act_land(w, 0);
    z_call(w->act_idle, w);
}
