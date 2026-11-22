/*
 * Player gimmick states of Night Carnival and the FixInformation hint
 * sprites, copied from src/game/Player/ and src/game/UI/Hud/ and adapted
 * like obj_pwater.c / obj_pmirage.c:
 *   Player_Action_GrabCraneSlide, Player_Action_GrabCranePart (zone 5),
 *   Player_Action_SwingSky, Player_State_SwingSky,
 *   FixInformation_CreateSwingSky, FixInformation_State_SwingSky,
 *   FixInformation_CreateSwing, FixInformation_State_Swing (zone 7).
 * FixInformation is a world sprite over the player (ac_fix_information.bac,
 * task 0xf6), drawn in the stage view: no second-screen output.
 */
#include "obj_zone.h"

/* ---------------------------------------------- Player_Action_GrabCraneSlide */
void Player_Action_GrabCraneSlide(DcPlayer *w, Obj *p, s32 a, s32 b) {
    if (w->gimmick == (void *)p)
        return;
    Player_Action_AttachToObject(w, p, a, b);
    w->pf |= 6;
    w->disp_flags &= ~0x200;
    w->pf2 |= 0x20000;
}
/* ---------------------------------------------- Player_Action_GrabCranePart */
void Player_Action_GrabCranePart(DcPlayer *w, Obj *p, s32 a, s32 b) {
    if (w->gimmick == (void *)p)
        return;
    Player_Action_AttachToObject(w, p, a, b);
    w->pf |= 2;
    w->pf |= 8;
    w->disp_flags &= ~0x200;
    w->pf2 |= 0x20000;
}

/* ---------------------------------------------------------- FixInformation */
static void FixInformation_State_SwingSky(Obj *w) {
    const DcPlayer *t0 = w->ppar;
    if (t0 == NULL)
        return;
    w->h.x = t0->x;
    w->h.y = t0->y - 0x2000;
    w->h.z = t0->z;
    w->wC++;
    if (w->wC > 0x1e0)
        w->h.obj_flags |= 4;
    if (t0->state != Player_State_SwingSky)
        w->h.obj_flags |= 4;
}
void FixInformation_CreateSwingSky(DcPlayer *arg) {
    Obj *w = z_effobj(OBJ_BAC_FIX_INFORMATION, 0xf6);
    if (w == NULL)
        return;
    /* (spr.unk44 = 1: palette slot) */
    w->spr.order = 3;
    z_set_prio(w, 0);
    obj_set_anim(w, 1);
    w->h.disp_flags |= 4;
    w->h.disp_flags |= 1;
    w->h.x = arg->x;
    w->h.y = arg->y - 0x2000;
    w->ppar = arg;
    w->h.move_flags |= 0x100;
    w->h.move_flags |= 0x2000;
    w->h.obj_flags |= 2;
    w->h.obj_flags |= 0x10;
    w->state = FixInformation_State_SwingSky;
    obj_log("fix_information", 1, arg->x >> 8);
}
static void FixInformation_State_Swing(Obj *w) {
    const DcPlayer *p = w->ppar;
    if (p == NULL)
        return;
    w->h.x = p->x;
    w->h.y = p->y;
    w->h.z = p->z;
    if (w->h.disp_flags & 2)
        w->h.y = p->y + 0x2000;
    else
        w->h.y = p->y - 0x2000;
    w->wC++;
    if (w->wC == 0x20) {
        if (w->spr.anim == 4)
            obj_set_anim(w, 5);
        else
            obj_set_anim(w, 4);
        w->h.disp_flags |= 4;
        w->scale_y = w->scale_x = 0x200;
    }
    if (w->wC >= 0x20) {
        w->scale_x = (s16)ObjShift_Set(w->scale_x, 0x100, 1, 0x20, 0);
        w->scale_y = w->scale_x;
    }
    if (w->wC > 0x78)
        w->h.obj_flags |= 4;
    if (w->ppar->state != Player_State_Swing)
        w->h.obj_flags |= 4;
}
void FixInformation_CreateSwing(DcPlayer *arg) {
    Obj *w = z_effobj(OBJ_BAC_FIX_INFORMATION, 0xf6);
    if (w == NULL)
        return;
    w->spr.order = 3;
    z_set_prio(w, 0);
    w->spr.flags |= 0x200;
    w->h.disp_flags |= 1;
    w->h.x = arg->x;
    w->h.y = arg->y;
    w->ppar = arg;
    if (obj_gravity_flipped(w->h.x, w->h.y))
        w->h.disp_flags |= 2;
    if (w->h.disp_flags & 2) {
        if (arg->disp_flags & 1)
            obj_set_anim(w, 5);
        else
            obj_set_anim(w, 4);
    } else {
        if (arg->disp_flags & 1)
            obj_set_anim(w, 4);
        else
            obj_set_anim(w, 5);
    }
    if (w->h.disp_flags & 2)
        w->h.y = arg->y + 0x2000;
    else
        w->h.y = arg->y - 0x2000;
    w->h.move_flags |= 0x100;
    w->h.move_flags |= 0x2000;
    w->h.obj_flags |= 2;
    w->h.obj_flags |= 0x10;
    w->state = FixInformation_State_Swing;
    obj_log("fix_information", 2, arg->x >> 8);
}

/* ------------------------------------------------------ SwingSky (player) */
void Player_Action_SwingSky(DcPlayer *w, Obj *other, s32 flip) {
    ObjPlayerCtx *c = z_ctx(w);
    if (w->gimmick == (void *)other)
        return;
    if (g_obj_stage.info_shown == 0) {
        FixInformation_CreateSwingSky(w);
        g_obj_stage.info_shown = 1;
    }
    dcp_reset_gimmick_state(w);
    w->gimmick = other;
    dcp_set_anim(w, 0x25);
    w->disp_flags |= 4;
    w->state = Player_State_SwingSky;
    w->move_flags |= 0x10;
    w->move_flags |= 0x2000;
    if (flip)
        w->disp_flags |= 1;
    else
        w->disp_flags &= ~1;
    w->pf |= 0x100000;
    w->pf &= ~0xfu;
    w->vel_x = 0;
    w->vel_y = 0;
    w->gnd_vel = 0;
    w->angle = 0;
    w->trick_input = 0;
    w->timer = 0;
    obj_sfx(0xec); /* Sound_PlaySeqArcEx(NULL, 0, 0xEC) */
    w->pf |= 0x2000;
    w->pf2 |= 0x20;
    c->g3EE = -0x3a;
}
void Player_State_SwingSky(DcPlayer *w) {
    ObjPlayerCtx *c = z_ctx(w);
    Obj *o = (Obj *)w->gimmick;
    u8 ang;
    s32 vy;
    if (o != NULL) {
        w->px = w->x;
        w->py = w->y;
        w->pz = w->z;
        w->x = o->h.px;
        w->y = o->h.py;
        w->angle = (u8)(o->h.angle - 0x40);
        w->move_x = (s16)(w->x - w->px);
        w->move_y = (s16)(w->y - w->py);
        w->gnd_vel = o->h.move_x;
        w->vel_y = o->h.move_y;
    }
    if (w->gimmick != NULL && !(w->pressed & 3))
        return;
    if (w->act_jump == NULL)
        return;
    if (w->act_idle == NULL)
        return;
    ang = (u8)(w->angle + 0x40);
    vy = w->vel_y >> 1;
    w->gimmick = NULL;
    w->pf &= ~0x2000u;
    w->move_flags &= ~0x2000u;
    w->pf &= ~0x100000u;
    w->angle = 0;
    w->gnd_vel = (s16)-(w->gnd_vel << 1);
    c->g3EE = 0;
    w->pf2 &= ~0x20u;
    if (w->pressed & 3) {
        w->act_jump(w);
        w->vel_y = (s16)(w->vel_y - (s16)ZABS((vy * ZSIN((u16)(ang << 8))) >> 12));
        return;
    }
    dcp_act_land(w, 0);
    w->act_idle(w);
}
