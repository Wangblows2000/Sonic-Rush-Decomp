/*
 * Effects used by the objdata v2 objects, copied from src/game/Effects/ and
 * adapted (task/work struct -> Obj, VRAM/palette calls dropped):
 *   EffZakoBombPuff_Create, EffFound_Create, Effect_SetFollowParentState,
 *   Effect_State_FollowParent, EffBrakeSmoke_Create.
 */
#include "obj_internal.h"

static Obj *eff_alloc(int bac) {
    Obj *o = obj_alloc(OBJ_PRIO_EFFECT, obj_stageobj_dtor);
    if (o == NULL)
        return NULL;
    obj_load_bac(o, bac);
    o->kind = OBJK_EFFECT;
    return o;
}

/* EffZakoBombPuff_Create */
Obj *EffZakoBombPuff_Create(Obj *arg, s16 x, s16 y) {
    Obj *w = eff_alloc(OBJ_BAC_ZAKO_BOMB);
    if (w == NULL)
        return NULL;
    obj_set_anim(w, 1);
    w->spr.order = 9;
    if (arg->h.grav_rot != 0)
        w->spr.flags |= 0x200;
    if (arg->h.disp_flags & 1)
        w->h.disp_flags |= 1;
    w->h.x = arg->h.x;
    w->h.y = arg->h.y;
    w->h.angle = arg->h.grav_rot;
    dcp_rotate_vec(&x, &y, arg->h.grav_rot);
    w->h.x += x << 8;
    w->h.y += y << 8;
    w->h.vel_x = 0;
    w->h.vel_y = 0;
    w->h.move_flags |= 0x2100;
    w->h.obj_flags |= 2;
    w->h.disp_flags |= 0x100;
    eff_set_move_until_anim_end(w);
    return w;
}

/* Effect_State_FollowParent */
static void Effect_State_FollowParent(Obj *w) {
    if (w->parent != NULL) {
        w->h.x = w->parent->h.x;
        w->h.y = w->parent->h.y;
        w->h.z = w->parent->h.z;
        if (w->h.disp_flags & 1)
            w->h.x -= w->h.vel_x * 0x100;
        else
            w->h.x += w->h.vel_x * 0x100;
        w->h.y += w->h.vel_y * 0x100;
    }
    if (w->h.disp_flags & 4) {
        if (w->wC == 0)
            w->h.obj_flags |= 4;
        else
            w->wC--;
        return;
    }
    if (w->h.disp_flags & 8) {
        if (w->wC == 0)
            w->h.obj_flags |= 4;
        else
            w->wC--;
    }
}

/* EffFound_Create ("!" mark over an enemy that noticed the player) */
Obj *EffFound_Create(Obj *arg, s16 x, s16 y) {
    Obj *w = eff_alloc(OBJ_BAC_EFF_FOUND);
    if (w == NULL)
        return NULL;
    w->spr.order = 9;
    if (arg->h.grav_rot != 0)
        w->spr.flags |= 0x200;
    w->h.disp_flags |= 1;
    w->parent = arg;
    w->h.x = arg->h.x;
    w->h.y = arg->h.y;
    w->h.angle = arg->h.grav_rot;
    dcp_rotate_vec(&x, &y, arg->h.grav_rot);
    w->h.vel_x = x;
    w->h.vel_y = y;
    w->h.hitstop = 0x40;
    w->h.move_flags |= 0x2100;
    w->h.obj_flags |= 2;
    w->h.disp_flags |= 0x100;
    w->state = Effect_State_FollowParent; /* Effect_SetFollowParentState */
    return w;
}

/* EffBrakeSmoke_Create */
Obj *EffBrakeSmoke_Create(Obj *arg, s16 x, s16 y) {
    Obj *w;
    u8 a = 0;
    w = eff_alloc(OBJ_BAC_EFF_BRAKE);
    if (w == NULL)
        return NULL;
    w->spr.order = 9;
    w->spr.flags = (w->spr.flags & ~0x60000u) | (1u << 17);
    /* (W.flags & 0x20: spr1.unk2C |= 0x80008, the second DS screen sprite: not kept) */
    if (arg->h.disp_flags & 1)
        w->h.disp_flags |= 1;
    w->h.x = arg->h.x;
    w->h.y = arg->h.y;
    w->h.vel_x = 0x10;
    w->h.vel_y = -6;
    if (arg->h.grav_rot != 0) {
        a = arg->h.grav_rot;
        w->h.vel_x = (s16)((w->h.vel_x * obj_sin((u16)(a << 8))) >> 12);
        w->h.vel_y = (s16)((w->h.vel_y * obj_cos((u16)(a << 8))) >> 12);
    }
    w->wC = (w->h.vel_x << 16) | (w->h.vel_y & 0xffff);
    w->h.vel_x = -0x80;
    w->h.vel_y = 0;
    if (arg->h.grav_rot != 0) {
        w->h.vel_x = (s16)((w->h.vel_x * obj_sin((u16)(a << 8))) >> 12);
        w->h.vel_y = (s16)((w->h.vel_y * obj_cos((u16)(a << 8))) >> 12);
    }
    if (w->h.disp_flags & 1) {
        x = (s16)-x;
        w->h.vel_x = (s16)-w->h.vel_x;
    }
    dcp_rotate_vec(&x, &y, (u8)(arg->h.angle + arg->h.grav_rot));
    w->h.x += x << 8;
    w->h.y += y << 8;
    w->h.move_flags |= 0x100;
    w->h.obj_flags |= 2;
    eff_set_move_until_anim_end(w);
    return w;
}
