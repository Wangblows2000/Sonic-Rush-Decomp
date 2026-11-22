/*
 * Flying enemies: src/game/Enemies/EnemyFlapperGroup_* copied and adapted
 * (task/work struct -> Obj, Rand -> RAND, sound handle dropped):
 *   EnemyFlapperGroup_Create, _Action_Hover, _State_Hover, _Action_Patrol,
 *   _State_Patrol, _Action_PatrolTurn, _State_PatrolTurn, _UpdateHover.
 * Types 0x02 (Flapper, Leaf Storm) and 0x04 (Egg Diver) fly their hover /
 * patrol paths.  The shooting / swooping types (0x0b, 0x0c, 0x10, 0x11,
 * 0x12: FireShot_*, StartSwoop, OnHit_Box2) are not translated yet; Create
 * returns NULL for them (OBJECTS.md).
 */
#include "obj_internal.h"

static void EnemyFlapperGroup_UpdateHover(Obj *w) {
    if ((W.frame & 3) == 0) {
        w->wC = RAND() & 0x1ff;
        if (RAND() & 1)
            w->wC = -w->wC;
        if (w->h.y > (w->home_y + 8) << 8 && w->h.vel_y > 0)
            w->h.vel_y = -0x1ff;
        if (w->h.y < (w->home_y - 8) << 8 && w->h.vel_y < 0)
            w->h.vel_y = 0x1ff;
    }
    w->h.vel_y = (s16)ObjShift_Set(w->h.vel_y, w->wC, 1, 0, 0x40);
}
static void EnemyFlapperGroup_State_Hover(Obj *p0) {
    if (p0->w8 != 0) {
        p0->w8--;
        if (p0->w8 == 0)
            p0->coll[2].flags |= 4;
    }
    if (p0->prm->type != 0x10)
        return;
    EnemyFlapperGroup_UpdateHover(p0);
}
static void EnemyFlapperGroup_Action_Hover(Obj *s) {
    obj_set_anim(s, 0);
    s->h.disp_flags |= 4;
    s->state = EnemyFlapperGroup_State_Hover;
}
static void EnemyFlapperGroup_Action_PatrolTurn(Obj *s);
static void EnemyFlapperGroup_State_Patrol(Obj *w) {
    s16 a = 10, m = 0xc0, sp = 0x100;
    u16 f;
    if (w->prm->type == 4) {
        a = 6;
        m = 0xb0;
    }
    f = w->prm->flags;
    if (f & 3) {
        sp = (s16)(sp + (((f & 0xc) >> 2) << 7));
        a = (s16)((a * sp) >> 8);
        m = (s16)((m * sp) >> 8);
    }
    if (w->h.disp_flags & 1)
        w->h.gnd_vel = (s16)dcp_spd_up(w->h.gnd_vel, -a, m);
    else
        w->h.gnd_vel = (s16)dcp_spd_up(w->h.gnd_vel, a, m);
    if (w->h.move_x > 0 && w->h.x > (w->home_x + w->prm->p6 + w->prm->p8) << 8) {
        EnemyFlapperGroup_Action_PatrolTurn(w);
        return;
    }
    if (w->h.move_x < 0 && w->h.x < (w->home_x + w->prm->p6) << 8) {
        EnemyFlapperGroup_Action_PatrolTurn(w);
        return;
    }
    if (w->w8 != 0) {
        w->w8--;
        if (w->w8 == 0)
            w->coll[2].flags |= 4;
    }
    if (w->prm->type == 0x10)
        EnemyFlapperGroup_UpdateHover(w);
}
static void EnemyFlapperGroup_Action_Patrol(Obj *p0) {
    obj_set_anim(p0, 2);
    p0->h.disp_flags |= 4;
    p0->h.move_flags &= ~0x10u;
    p0->state = EnemyFlapperGroup_State_Patrol;
}
static void EnemyFlapperGroup_State_PatrolTurn(Obj *w) {
    s16 a = 0x40, sp = 0x100;
    u16 f;
    if (w->prm->type == 4)
        a = 4;
    f = w->prm->flags;
    if (f & 3) {
        sp = (s16)(sp + (((f & 0xc) >> 2) << 7));
        a = (s16)((a * sp) >> 8);
    }
    w->h.gnd_vel = (s16)dcp_spd_down(w->h.gnd_vel, a);
    if (w->h.disp_flags & 8) {
        w->h.disp_flags ^= 1;
        EnemyFlapperGroup_Action_Patrol(w);
        return;
    }
    if (w->w8 != 0) {
        w->w8--;
        if (w->w8 == 0)
            w->coll[2].flags |= 4;
    }
    if (w->prm->type == 0x10)
        EnemyFlapperGroup_UpdateHover(w);
}
static void EnemyFlapperGroup_Action_PatrolTurn(Obj *s) {
    obj_set_anim(s, 1);
    s->h.move_flags &= ~0x10u;
    s->state = EnemyFlapperGroup_State_PatrolTurn;
}

Obj *EnemyFlapperGroup_Create(ObjParam *p, s32 b, s32 c) {
    Obj *w;
    if (p == NULL || p->x0 != 0xff || p->y0 != 0xff) {
        if (p->flags & 0x80) {
            if (g_obj_stage.save_flags & 1) {
                if (W.mode != 2 && W.mode != 3)
                    return NULL;
            }
        }
    }
    if (p->type != 2 && p->type != 4)
        return NULL; /* shooting / swooping types: not translated */
    w = obj_alloc(OBJ_PRIO_MAPOBJ, obj_mapobject_dtor);
    if (w == NULL)
        return NULL;
    obj_mapobject_init(w, p, b, c);
    w->kind = OBJK_ENEMY;
    switch (w->prm->type) {
    case 2:
        obj_load_bac(w, OBJ_BAC_FLAPPER);
        break;
    case 4:
        obj_load_bac(w, OBJ_BAC_EGG_DIVER);
        break;
    }
    w->spr.order = 0x14;
    if (w->prm->flags & 0x10)
        w->h.disp_flags &= ~1;
    w->h.move_flags |= 0x140;
    if (obj_gravity_flipped(w->h.x, w->h.y)) {
        w->h.grav_rot = 0x80;
        w->h.disp_flags |= 0x100;
    }
    if (p->flags & 0x40) {
        w->f1F4 |= 0x40;
        if (g_obj_hooks.effect) /* EffEntrance_Create(w, 0, -16) */
            g_obj_hooks.effect(g_obj_hooks.ctx, NULL, OBJ_FX_ENTRANCE, w->h.x >> 8, (w->h.y >> 8) - 16);
    }
    if (w->prm->p6 == 0 && w->prm->p8 == 0)
        EnemyFlapperGroup_Action_Hover(w);
    else
        EnemyFlapperGroup_Action_Patrol(w);
    w->snd = 1;
    return w;
}
