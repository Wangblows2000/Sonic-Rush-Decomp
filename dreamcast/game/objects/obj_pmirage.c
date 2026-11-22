/*
 * Player gimmick states of Mirage Road, copied from src/game/Player/ and
 * adapted like obj_pgimmick.c / obj_pwater.c:
 *   Player_Action_Cargo, Player_State_Cargo, Player_Action_Hand2D,
 *   Player_State_Hand2D, Player_Action_SandSlide, Player_State_SandSlide,
 *   Player_Action_SandSlider, Player_State_SandSlider,
 *   Player_Action_SandSlider90, Player_State_SandSlider90,
 *   Player_AttachGimmickTable, Player_AttachGimmickTableNoInput,
 *   Player_AttachGimmickTableReleased, Player_DetachGimmick.
 * Player work @0x3EC/@0x3EE (camera look offsets written by the table
 * attach functions) is kept in ObjPlayerCtx.g3EC/g3EE.
 */
#include <string.h>
#include "obj_internal.h"

#define H g_dcp_hooks

static ObjPlayerCtx s_dummy;
static ObjPlayerCtx *ctx(DcPlayer *w) {
    ObjPlayerCtx *c = obj_player_ctx(w);
    return c ? c : &s_dummy;
}
static void psfx(DcPlayer *p, s32 ch, s32 id) {
    if (H.sfx)
        H.sfx(H.ctx, p, ch, id);
}

/* ============================================================ Cargo */
void Player_Action_Cargo(DcPlayer *w, Obj *o, s32 x, s32 y) {
    ObjPlayerCtx *c = ctx(w);
    dcp_reset_gimmick_state(w);
    dcp_act_land(w, 0);
    w->gimmick = o;
    dcp_set_anim(w, 0x30);
    w->disp_flags |= 4;
    w->state = Player_State_Cargo;
    w->move_flags |= 0x10;
    w->move_flags |= 0x2000;
    w->move_flags |= 0x100;
    w->move_flags &= ~0x80u;
    w->move_flags &= ~0x40u;
    w->pf |= 0x100000;
    c->g3F8 = w->gnd_vel;
    w->vel_x = 0;
    w->vel_y = 0;
    w->move_x = 0;
    w->move_y = 0;
    w->gnd_vel = 0;
    w->angle = 0;
    w->trick_input = 0;
    w->timer = 0;
    c->g3F0 = x << 8;
    c->g3F4 = y << 8;
    w->px = w->x;
    w->py = w->y;
    w->x = o->h.x + c->g3F0;
    w->y = o->h.y + c->g3F4;
    psfx(w, DCP_CH_VOICE, 0x5b);
}
void Player_State_Cargo(DcPlayer *w) {
    ObjPlayerCtx *c = ctx(w);
    Obj *o = (Obj *)w->gimmick;
    if (o != NULL) {
        w->px = w->x;
        w->py = w->y;
        w->x = o->h.x + c->g3F0;
        w->y = o->h.y + c->g3F4;
        if (o->h.move_flags & 0x2000) {
            w->gnd_vel = (s16)-o->h.gnd_vel;
            w->disp_flags ^= 1;
            w->gimmick = NULL;
        }
    }
    if (w->gimmick != NULL)
        return;
    if (w->act_idle == NULL)
        return;
    w->gimmick = NULL;
    w->move_flags |= 1;
    w->move_flags |= 0xc0;
    w->move_flags &= ~0x2000u;
    w->move_flags &= ~0x100u;
    w->move_flags &= ~0x4000u;
    w->pf &= ~0x100000u;
    w->angle = 0;
    w->act_idle(w);
}

/* ============================================================ Hand2D */
void Player_Action_Hand2D(DcPlayer *w, Obj *o, s32 x, s32 y) {
    ObjPlayerCtx *c = ctx(w);
    if (w->state == Player_State_Hand2D)
        return;
    if (w->move_flags & 1)
        w->vel_x = w->gnd_vel;
    dcp_reset_gimmick_state(w);
    dcp_act_land(w, 0);
    w->gimmick = o;
    dcp_set_anim(w, 0x2a);
    w->disp_flags |= 4;
    w->state = Player_State_Hand2D;
    w->move_flags |= 0x10;
    w->move_flags |= 0x2000;
    w->move_flags |= 0x100;
    w->move_flags &= ~0x80u;
    w->move_flags &= ~0x40u;
    w->pf |= 0x100000;
    w->y = o->h.y - 0xc00;
    c->g3F0 = 0;
    w->vel_x = 0;
    w->vel_y = 0;
    w->move_x = 0;
    w->move_y = 0;
    w->gnd_vel = 0;
    w->angle = 0;
    w->trick_input = 0;
    w->timer = 0;
    c->g3F4 = 0;
    c->g3F8 = o->h.x + (x << 8) - w->x;
    c->g3FC = o->h.y + (y << 8) - w->y;
    w->px = w->x;
    w->py = w->y;
    w->x = o->h.x + c->g3F0;
    w->y = o->h.y + c->g3F4;
    w->pf |= 0x2000;
    w->pf2 |= 0x30;
    w->pf2 |= 0x20000;
    psfx(w, DCP_CH_VOICE, 0x5d);
}
void Player_State_Hand2D(DcPlayer *w) {
    ObjPlayerCtx *c = ctx(w);
    Obj *o = (Obj *)w->gimmick;
    ObjMtx33 m, r;
    ObjVec v;
    u16 a;
    if (o != NULL && (o->h.move_flags & 0x2000))
        w->gimmick = o = NULL;
    if (o != NULL) {
        c->g3F4 -= (o->h.z >> 4) + 0x470;
        w->rot3 = (u8)((c->g3F4 & 0xffff) >> 8);
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
    if (w->gimmick != NULL)
        return;
    if (w->act_idle == NULL)
        return;
    w->gimmick = NULL;
    w->pf &= ~0x2000u;
    w->pf2 &= ~0x30u;
    w->pf2 &= ~0x20000u;
    w->move_flags |= 1;
    w->move_flags |= 0xc0;
    w->move_flags &= ~0x2000u;
    w->move_flags &= ~0x100u;
    w->move_flags &= ~0x4000u;
    w->pf &= ~0x100000u;
    w->angle = 0;
    w->rot3 = 0;
    w->vel_z = 0;
    w->z = 0;
    w->gnd_vel = (s16)c->g3F0;
    w->act_idle(w);
}

/* ========================================================= sand slide */
void Player_Action_SandSlide(DcPlayer *w) {
    dcp_reset_gimmick_state(w);
    dcp_set_anim(w, 0x2d);
    w->disp_flags |= 4;
    w->move_flags &= ~0x10u;
    w->move_flags &= ~0x8000u;
    w->move_flags &= ~0x40000u;
    w->move_flags |= 0x200;
    w->pf |= 0x100000;
    w->state = Player_State_SandSlide;
    w->slope_acc = dcp_physics(w->chara)->roll_slope;
    w->slope_lim = 0;
    psfx(w, DCP_CH_VOICE, 0x5c);
}
void Player_State_SandSlide(DcPlayer *w) {
    if (w->angle > 0x80)
        w->disp_flags |= 1;
    else
        w->disp_flags &= ~1;
    if (w->move_flags & 1) {
        if ((w->move_x < 0 && (w->disp_flags & 1)) || (w->move_x > 0 && !(w->disp_flags & 1))) {
            if (!(W.frame & 7))
                EffSandSlide_Create(w, 8, 0x10);
        }
    }
    if (!(W.frame & 0x3f))
        psfx(w, DCP_CH_VOICE, 0x5c);
    if (w->move_flags & 0x10) {
        if (!(w->move_flags & 1))
            return;
        w->move_flags &= ~0x10u;
        w->move_flags &= ~0x8000u;
        w->vel_x = w->gnd_vel;
        dcp_act_land(w, 0);
        w->slope_acc = dcp_physics(w->chara)->roll_slope;
        w->slope_lim = 0;
        w->gnd_vel = (s16)(w->gnd_vel >> 1);
    } else {
        if (w->move_flags & 1)
            return;
        w->move_flags |= 0x10;
        w->move_flags |= 0x8000;
        w->vel_x = (s16)((w->gnd_vel * obj_cos((u16)(w->angle << 8))) >> 12);
        w->vel_y = (s16)((w->gnd_vel * obj_sin((u16)(w->angle << 8))) >> 12);
    }
}

/* ========================================================= sand slider */
void Player_Action_SandSlider(DcPlayer *w, Obj *a) {
    if (w->state == Player_State_SandSlider)
        return;
    dcp_reset_gimmick_state(w);
    dcp_player_load_physics(w);
    w->gimmick = a;
    dcp_set_anim(w, 0x31);
    w->disp_flags |= 4;
    w->state = Player_State_SandSlider;
    w->move_flags |= 0x10;
    w->move_flags |= 0x100;
    w->move_flags |= 0x40000;
    w->move_flags &= ~1u;
    w->move_flags &= ~0x200u;
    w->pf |= 0x100000;
    w->pf &= ~1u;
    w->vel_x = 0;
    w->gnd_vel = 0;
    w->move_x = 0;
    w->move_y = 0;
    w->angle = 0;
    w->trick_input = 0;
    w->timer = 0;
    w->obj_flags |= 1;
    w->move_flags &= ~0x100u;
}
void Player_State_SandSlider(DcPlayer *w) {
    Obj *o = (Obj *)w->gimmick;
    s16 t2;
    if (o != NULL) {
        w->px = w->x;
        w->x = ObjShift_Set(w->x, o->h.x, 4, 0, 0x100);
        w->z = ObjShift_Set(w->z, 0, 1, 0, 0x100);
    }
    if (!(w->move_flags & 1))
        return;
    t2 = w->vel_y;
    w->gnd_vel = 0;
    w->gimmick = NULL;
    w->move_flags &= ~0x100u;
    w->pf &= ~0x100000u;
    w->rot3 = 0;
    w->rot2 = 0;
    w->angle = 0;
    dcp_act_land(w, 0);
    if (w->act_idle != NULL)
        w->act_idle(w);
    w->vel_y = t2;
}
void Player_Action_SandSlider90(DcPlayer *w, Obj *other) {
    if (w->state == Player_State_SandSlider90)
        return;
    dcp_reset_gimmick_state(w);
    w->gimmick = other;
    dcp_set_anim(w, 0x31);
    w->disp_flags |= 4;
    w->state = Player_State_SandSlider90;
    w->move_flags |= 0x10;
    w->move_flags |= 0x100;
    w->move_flags &= ~1u;
    w->obj_flags &= ~1u;
    dcp_stop_highspeed(w);
    dcp_stop_boost(w);
    w->pf |= 0x100000;
    w->pf &= ~1u;
    w->vel_x = 0;
    w->gnd_vel = 0;
    w->move_x = 0;
    w->move_y = 0;
    w->angle = 0;
    w->trick_input = 0;
    w->timer = 0;
    if (w->chara != 1)
        psfx(w, DCP_CH_VOICE, 0);
    else
        psfx(w, DCP_CH_VOICE, 0x11);
}
void Player_State_SandSlider90(DcPlayer *w) {
    Obj *o = (Obj *)w->gimmick;
    s32 y;
    if (o != NULL) {
        w->px = w->x;
        w->x = ObjShift_Set(w->x, o->h.x, 4, 0, 0x100);
        if (!(w->pf & 1)) {
            y = o->h.y + 0x1200;
            if (w->y >= y) {
                w->y = y;
                w->vel_y = 0;
                w->pf |= 1;
                w->move_flags &= ~0x80u;
            }
        }
        if (w->pf & 1) {
            if (w->z >= 0x2800) {
                w->move_flags |= 0x80;
                w->vel_z = (s16)dcp_spd_down(w->vel_z, 0x50);
            } else {
                w->vel_y = (s16)dcp_spd_up(w->vel_y, 8, 0x600);
                w->vel_z = (s16)dcp_spd_up(w->vel_z, 0x36, 0x600);
            }
        }
    }
    if ((w->pf & 1) && w->vel_z == 0)
        w->move_flags &= ~0x100u;
    if (w->move_flags & 1)
        Player_Action_SandSlide(w);
}

/* ============================================ GmkTable attach / detach */
void Player_AttachGimmickTable(DcPlayer *w, Obj *o) {
    ObjPlayerCtx *c = ctx(w);
    w->gimmick = o;
    w->obj_flags |= 1;
    w->pf |= 0x2000;
    w->pf |= 0x1000;
    w->pf |= 0x800;
    w->pf2 |= 8;
    w->pf2 |= 0x30;
    w->pf2 |= 0x40000;
    c->g3EC = 0;
    c->g3EE = 0x28;
    if (!(w->pf2 & 0x200))
        EffShadow_Create(w);
    w->pf2 |= 0x200;
    if (!(g_obj.stats.table_rides++))
        obj_log("table_ride", 0x8b, w->x >> 8);
}
void Player_AttachGimmickTableNoInput(DcPlayer *w, Obj *o) {
    ObjPlayerCtx *c = ctx(w);
    w->gimmick = o;
    w->pf |= 0x2000;
    w->pf |= 0x1000;
    w->pf2 |= 8;
    w->pf2 |= 0x30;
    w->pf2 |= 0x100;
    w->pf |= 0x200000;
    w->held = 0;
    w->pressed = 0;
    w->released = 0;
    w->pf2 |= 0x40000;
    c->g3EC = 0;
    c->g3EE = 0x58;
    if (!(W.objmgr & 0x100))
        w->pf2 &= ~0xc0u;
    W.objmgr |= 0x100;
}
void Player_AttachGimmickTableReleased(DcPlayer *w, Obj *o) {
    ObjPlayerCtx *c = ctx(w);
    w->gimmick = o;
    w->pf |= 0x2000;
    w->pf |= 0x1000;
    w->pf2 |= 8;
    w->pf2 |= 0x30;
    w->pf2 |= 0x100;
    w->pf2 |= 0x40000;
    if (!(w->pf & 0x1000000))
        w->pf &= ~0x200000u;
    c->g3EC = 0;
    c->g3EE = 0x58;
    if (!(W.objmgr & 0x100))
        obj_log("fix_vs_above_player", 0, w->x >> 8); /* FixVs_CreateAbovePlayer (HUD): not translated */
    W.objmgr |= 0x100;
}
void Player_DetachGimmick(DcPlayer *w, Obj *o) {
    ObjPlayerCtx *c = ctx(w);
    if (w->gimmick != o)
        return;
    w->gimmick = NULL;
    w->pf &= ~0x2000u;
    w->pf &= ~0x1000u;
    w->pf &= ~0x800u;
    w->pf2 &= ~8u;
    w->pf2 &= ~0x100u;
    c->g3EC = 0;
    c->g3EE = 0;
    w->pf2 &= ~0x40000u;
    W.objmgr &= ~0x100u;
}
