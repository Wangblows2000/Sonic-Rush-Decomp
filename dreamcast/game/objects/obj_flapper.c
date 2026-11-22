/*
 * Flying enemies: src/game/Enemies/EnemyFlapperGroup_* copied and adapted
 * (task/work struct -> Obj, Rand -> RAND, sound handle dropped):
 *   EnemyFlapperGroup_Create, _Action_Hover, _State_Hover, _Action_Patrol,
 *   _State_Patrol, _Action_PatrolTurn, _State_PatrolTurn, _UpdateHover.
 * Types 0x02 (Flapper, Leaf Storm) and 0x04 (Egg Diver) fly their hover /
 * patrol paths.  objdata v4 adds the shooting / swooping types of zones
 * 4..7: 0x0b (Egg Flapper Laser), 0x0c (Bomb Hawk), 0x10 (Flapper Bat),
 * 0x11 (Flapper Cannon), 0x12 (Falco): EnemyFlapperGroup_OnHit_Box2,
 * _FireShot, _Action/_State_FireShot_ov000_022ffd68 / _02300438 /
 * _02300578, _Action_StartSwoop, _State_SwoopOut, _Action/_State_SwoopReturn,
 * and the shots EggFlapperLaser_Create (0xe7), EggBombHawk_* (0xeb),
 * FlapperCannon_Create (0xf0).
 */
#include "obj_internal.h"

/* Math_LerpFx32 */
static s32 Math_LerpFx32(s32 a, s32 b, s32 t) {
    if (t == 0)
        return b;
    if (t != 0x1000)
        a = b + (((a - b) * t) >> 12);
    return a;
}

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

/* ------------------------------------------- objdata v4: shooting / swooping */
static void flapper_resume(Obj *p0) {
    if (p0->prm->p6 == 0 && p0->prm->p8 == 0) {
        EnemyFlapperGroup_Action_Hover(p0);
        return;
    }
    EnemyFlapperGroup_Action_Patrol(p0);
}
static dcp_bool easy_slow(void) { return (g_obj_stage.save_flags & 1) && W.mode != 2 && W.mode != 3; }
/* EnemyFlapperGroup_FireShot */
static void EnemyFlapperGroup_FireShot(Obj *w, s16 dx, s16 dy) {
    u16 f = 0;
    s32 type = 0xe7;
    Obj *o;
    if (w->h.disp_flags & 1)
        f = w->h.disp_flags & 1;
    f |= w->prm->flags;
    switch (w->prm->type) {
    case 0xb:
        type = 0xe7;
        break;
    case 0xc:
        type = 0xeb;
        break;
    case 0x11:
        type = 0xf0;
        break;
    }
    o = obj_mapobject_spawn(type, w->h.x, w->h.y, (s16)f, 0, 0, 0, 0);
    if (o == NULL)
        return;
    o->h.grav_rot = w->h.grav_rot;
    dcp_rotate_vec(&dx, &dy, w->h.grav_rot);
    if (o->h.disp_flags & 1)
        o->h.x -= dx << 8;
    else
        o->h.x += dx << 8;
    o->h.y += dy << 8;
    obj_log("flapper_shot", type, w->h.x >> 8);
}
static void EnemyFlapperGroup_State_FireShot_ov000_022ffcac(Obj *p0) {
    if ((p0->spr.frame_t >> 4) == 0xf) {
        if ((p0->f1F4 & 1) == 0) {
            obj_sfx(0x78);
            EnemyFlapperGroup_FireShot(p0, 0, 0xa);
        }
        p0->f1F4 |= 1;
    }
    if ((p0->h.disp_flags & 8) == 0)
        return;
    p0->w8 = 0x78;
    flapper_resume(p0);
}
static void EnemyFlapperGroup_State_FireShot_ov000_02300338(Obj *p0) {
    if ((p0->spr.frame_t >> 4) == 0xc) {
        if ((p0->f1F4 & 1) == 0) {
            obj_sfx(0x78);
            EnemyFlapperGroup_FireShot(p0, 0, 0);
        }
        p0->f1F4 |= 1;
    }
    if (p0->w8 == 0) {
        p0->w8 = 0x78;
        if (easy_slow())
            p0->w8 = (s32)((u32)p0->w8 + ((u32)p0->w8 >> 1));
        flapper_resume(p0);
        return;
    }
    p0->w8--;
}
static void EnemyFlapperGroup_State_FireShot_ov000_02300484(Obj *p0) {
    if ((p0->spr.frame_t >> 4) == 0x16) {
        if ((p0->f1F4 & 1) == 0) {
            obj_sfx(0x77);
            EnemyFlapperGroup_FireShot(p0, 0, 0);
        }
        p0->f1F4 |= 1;
    }
    if ((p0->h.disp_flags & 8) == 0)
        return;
    p0->w8 = 0x78;
    if (easy_slow())
        p0->w8 = (s32)((u32)p0->w8 + ((u32)p0->w8 >> 1));
    flapper_resume(p0);
}
static void flapper_fire_action(Obj *p0, ObjFn st) {
    obj_set_anim(p0, 3);
    p0->h.disp_flags |= 4;
    p0->state = st;
    p0->f1F4 &= ~1u;
    p0->h.gnd_vel = 0;
}
static void EnemyFlapperGroup_State_SwoopReturn(Obj *p0) {
    s32 t0 = p0->home_x << 8, t1 = p0->home_y << 8;
    if (p0->prm->type != 0x12)
        p0->h.vel_x = (s16)ObjShift_SetFromRef(p0->h.vel_x, 0x1000, 0, 4, 0x280, 0x80);
    else
        p0->h.vel_x = (s16)ObjShift_SetFromRef(p0->h.vel_x, 0x1000, 0, 2, 0x600, 0x100);
    p0->h.px = p0->h.x;
    p0->h.py = p0->h.y;
    p0->h.x = Math_LerpFx32(t0, p0->u1FC, (u16)p0->h.vel_x);
    p0->h.y = Math_LerpFx32(t1, p0->u200, (u16)p0->h.vel_x);
    if (p0->h.x != t0)
        return;
    if (p0->h.y != t1)
        return;
    p0->h.move_flags &= ~0x2000u;
    p0->wC = 0;
    p0->w8 = 0x60;
    p0->h.vel_x = 0;
    p0->h.vel_y = 0;
    p0->h.gnd_vel = 0;
    flapper_resume(p0);
}
static void EnemyFlapperGroup_Action_SwoopReturn(Obj *p0) {
    p0->state = EnemyFlapperGroup_State_SwoopReturn;
    p0->h.disp_flags ^= 1;
    p0->f1F4 &= ~1u;
    p0->h.gnd_vel = 0;
    p0->h.vel_x = 0;
    p0->h.vel_y = 0;
    p0->w8 = 0;
    p0->u1FC = p0->h.x;
    p0->u200 = p0->h.y;
}
static void EnemyFlapperGroup_State_SwoopOut(Obj *p0) {
    u32 v = (u32)p0->w8;
    s32 t0 = (s32)((v & 0xffff) << 8), t1 = (s32)(((v >> 16) & 0xffff) << 8);
    if (p0->h.vel_y == 0) {
        if (p0->prm->type != 0x12)
            p0->h.vel_x = (s16)ObjShift_SetFromRef(p0->h.vel_x, 0x1000, 0, 4, 0x280, 0x80);
        else
            p0->h.vel_x = (s16)ObjShift_SetFromRef(p0->h.vel_x, 0x1000, 0, 2, 0x600, 0x100);
        p0->h.px = p0->h.x;
        p0->h.py = p0->h.y;
        p0->h.x = Math_LerpFx32(t0, p0->u1FC, (u16)p0->h.vel_x);
        p0->h.y = Math_LerpFx32(t1, p0->u200, (u16)p0->h.vel_x);
    } else {
        p0->h.vel_y--;
        if (p0->h.vel_y == 0)
            obj_sfx(0x91);
    }
    if (p0->h.x != t0)
        return;
    if (p0->h.y != t1)
        return;
    if (!(p0->f1F4 & 1)) {
        p0->h.hitstop |= 0x80;
        p0->f1F4 |= 1;
        return;
    }
    if (p0->h.gnd_vel != 0) {
        p0->h.gnd_vel--;
        return;
    }
    EnemyFlapperGroup_Action_SwoopReturn(p0);
}
static void EnemyFlapperGroup_Action_StartSwoop(Obj *w) {
    DcPlayer *t = (DcPlayer *)w->target;
    if (w->prm->type != 0x12)
        obj_set_anim(w, 2);
    else
        obj_set_anim(w, 0);
    w->h.disp_flags |= 4;
    w->state = EnemyFlapperGroup_State_SwoopOut;
    w->f1F4 &= ~1u;
    w->h.gnd_vel = 0;
    w->h.vel_x = 0;
    w->h.vel_y = 0;
    w->w8 = 0;
    w->h.move_flags |= 0x2000;
    if (t != NULL) {
        if (w->h.x > t->x)
            w->h.disp_flags |= 1;
        else
            w->h.disp_flags &= ~1;
        w->w8 = (s32)((u32)((t->x >> 8) & 0xffff) | ((u32)(t->y >> 8) << 16));
        w->target = NULL;
    }
    w->u1FC = w->h.x;
    w->u200 = w->h.y;
    if (w->prm->type != 0x12) {
        w->h.vel_y = 0x10;
        w->h.gnd_vel = 0x18;
    } else {
        w->h.vel_y = 0x20;
        w->h.gnd_vel = 8;
    }
    if (!easy_slow())
        return;
    w->h.vel_y = (s16)(w->h.vel_y + (w->h.vel_y >> 1));
    w->h.gnd_vel = (s16)(w->h.gnd_vel + (w->h.gnd_vel >> 1));
}
static void EnemyFlapperGroup_OnHit_Box2(ObjBox *a, ObjBox *b) {
    Obj *w = (a && a->owner_kind == OBJ_OWNER_OBJECT) ? (Obj *)a->owner : NULL;
    DcPlayer *o = obj_box_player(b);
    u16 ang;
    if (w == NULL || o == NULL)
        return;
    switch (w->prm->type) {
    case 0xb:
        w->coll[2].flags &= ~4;
        flapper_fire_action(w, EnemyFlapperGroup_State_FireShot_ov000_02300484); /* _Action_FireShot_ov000_02300578 */
        break;
    case 0xc:
        w->coll[2].flags &= ~4;
        flapper_fire_action(w, EnemyFlapperGroup_State_FireShot_ov000_02300338); /* _Action_FireShot_ov000_02300438 */
        w->w8 = 0x60;
        break;
    case 0x10:
        w->coll[2].flags &= ~4;
        w->target = o;
        EnemyFlapperGroup_Action_StartSwoop(w);
        break;
    case 0x11:
        ang = (u16)dcp_atan2((o->y - w->h.y) >> 8, (o->x - w->h.x) >> 8);
        if ((ang > 0x5000 && ang < 0x7000) || (ang > 0x1000 && ang < 0x3000)) {
            w->coll[2].flags &= ~4;
            flapper_fire_action(w, EnemyFlapperGroup_State_FireShot_ov000_022ffcac); /* _Action_FireShot_ov000_022ffd68 */
            break;
        }
        return;
    case 0x12:
        w->coll[2].flags &= ~4;
        w->target = o;
        EnemyFlapperGroup_Action_StartSwoop(w);
        break;
    }
    obj_log("flapper_detect", w->prm->type, w->h.x >> 8);
    EffFound_Create(w, 0, -0x20);
}
static void flapper_box2(Obj *w, s16 l, s16 t, s16 r, s16 b) {
    rect_set_box2d(&w->coll[2], l, t, r, b);
    w->coll[2].flags |= 0xc0;
    rect_set_atkdef(&w->coll[2], 0, (u32)-16);
    w->coll[2].mask = 0x12;
    w->coll[2].cb = EnemyFlapperGroup_OnHit_Box2;
    w->coll[2].owner = w;
    w->coll[2].owner_kind = OBJ_OWNER_OBJECT;
}

/* the shots */
static Obj *shot_obj(ObjParam *p, s32 b, s32 c, int bac) {
    Obj *w = obj_alloc(OBJ_PRIO_MAPOBJ, obj_mapobject_dtor);
    if (w == NULL)
        return NULL;
    obj_mapobject_init(w, p, b, c);
    w->kind = OBJK_ENEMY;
    obj_load_bac(w, bac);
    return w;
}
/* EggFlapperLaser_Create (0xe7) */
Obj *EggFlapperLaser_Create(ObjParam *p, s32 b, s32 c) {
    Obj *w = shot_obj(p, b, c, OBJ_BAC_EGG_FLAPPER_LASER);
    if (w == NULL)
        return NULL;
    obj_set_anim(w, 4);
    w->h.disp_flags |= 4;
    w->spr.order = 0x15;
    if (!(w->prm->flags & 1))
        w->h.disp_flags &= ~1;
    w->h.vel_y = 0x600;
    w->h.move_flags |= 0x110;
    w->h.disp_flags |= 0x100;
    return w;
}
/* FlapperCannon_Create (0xf0) */
Obj *FlapperCannon_Create(ObjParam *p, s32 b, s32 c) {
    Obj *w = shot_obj(p, b, c, OBJ_BAC_FLAPPER_CANNON);
    if (w == NULL)
        return NULL;
    obj_set_anim(w, 4);
    w->h.disp_flags |= 4;
    w->spr.order = 0x15;
    w->h.vel_x = -0x300;
    w->h.vel_y = 0x300;
    if (!(w->prm->flags & 1)) {
        w->h.disp_flags &= ~1;
        w->h.vel_x = (s16)-w->h.vel_x;
    }
    w->h.move_flags |= 0x110;
    w->h.disp_flags |= 0x100;
    w->h.disp_flags |= 4;
    return w;
}
/* EggBombHawk_* (0xeb): the bomb dropped by the Bomb Hawk.  The ROM names
 * ac_ene_egg_bomb_hawk.bac, which no archive holds; ObjData_Load returns
 * the resource already in data_ov000_02369e04 (ac_ene_bomb_hawk.bac, the
 * dropping flapper's), so the bomb shows that BAC */
static void EggBombHawk_Explode(Obj *p0) {
    if (p0->h.disp_flags & 0x20)
        return;
    obj_sfx(0x59);
    EffZakoBomb_Create(&p0->h, 0, 0);
    eff_zako_debris(p0, 0, -24, 0, -0x200, 4);
    eff_zako_debris(p0, 0, -24, 0, -0x200, 5);
    obj_screen_shake(3);
    if (g_obj_hooks.effect)
        g_obj_hooks.effect(g_obj_hooks.ctx, NULL, OBJ_FX_SCREEN_FLASH, 0, 0);
    AttackHitbox_Create(&p0->h, -24, -24, 0x18, 0x18, 8);
    p0->h.obj_flags |= 8;
    p0->h.disp_flags |= 0x20;
    p0->h.move_flags |= 0x100;
    obj_log("bomb_hawk_explode", 0xeb, p0->h.x >> 8);
}
static void EggBombHawk_State_ExplodeOnContact(Obj *p0) {
    if (p0->w8 != 0) {
        p0->w8--;
        if (p0->w8 == 0)
            p0->spr.order = 0xa;
    }
    if (p0->h.move_flags & 0x100)
        return;
    if ((p0->h.move_flags & 0xf) == 0) {
        if ((p0->coll[1].flags & 0x200) == 0)
            return;
    }
    EggBombHawk_Explode(p0);
}
Obj *EggBombHawk_Create(ObjParam *p, s32 b, s32 c) {
    Obj *w = shot_obj(p, b, c, OBJ_BAC_BOMB_HAWK);
    if (w == NULL)
        return NULL;
    obj_set_anim(w, 4);
    w->h.disp_flags |= 4;
    w->spr.order = 0x15;
    if (!(w->prm->flags & 1))
        w->h.disp_flags &= ~1;
    w->h.move_flags |= 0x90;
    w->h.disp_flags |= 0x100;
    w->snd = 1;
    w->state = EggBombHawk_State_ExplodeOnContact;
    return w;
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
    /* objdata v4 */
    case 11:
        obj_load_bac(w, OBJ_BAC_EGG_FLAPPER_LASER);
        flapper_box2(w, -45, 0, 45, 400);
        break;
    case 12:
        obj_load_bac(w, OBJ_BAC_BOMB_HAWK);
        flapper_box2(w, -45, 0, 45, 400);
        w->f1F4 |= 0x20;
        break;
    case 16:
        obj_load_bac(w, OBJ_BAC_FLAPPER_BAT);
        if (w->prm->p9 == 0)
            flapper_box2(w, -45, -45, 45, 45);
        else
            flapper_box2(w, -45, p->p7, 45, (s16)(p->p9 + p->p7));
        break;
    case 17:
        obj_load_bac(w, OBJ_BAC_FLAPPER_CANNON);
        flapper_box2(w, -100, 0, 0, 100);
        break;
    case 18:
        obj_load_bac(w, OBJ_BAC_FALCO);
        flapper_box2(w, -60, 10, 0, 80);
        if (w->prm->p6 == 0 && w->prm->p8 == 0) {
            w->prm->p6 = -0x28;
            w->prm->p8 = 0x50;
        }
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
        EffEntrance_Create(w, 0, -16); /* (objdata v4: translated since v3, was an effect hook) */
    }
    if (w->prm->p6 == 0 && w->prm->p8 == 0)
        EnemyFlapperGroup_Action_Hover(w);
    else
        EnemyFlapperGroup_Action_Patrol(w);
    w->snd = 1;
    return w;
}
