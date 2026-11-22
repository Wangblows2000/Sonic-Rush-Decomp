/*
 * Dead Line (zone 7) objects and decorations, copied from the decompilation
 * and adapted like obj_water.c / obj_mirage.c:
 *   src/game/Zone/Dead_Line/Objects/ Cannon_*, Duct_*, GmkAir_*, MapObj9C_*,
 *     Rocket_*, Swing_*
 *   src/game/Zone/Dead_Line/Decorations/ DecArrow_Create
 * The Dead_Line/Effects 3D effects (EffBoostA*_3D, EffFire01_3D) and
 * EffDebrisSpawner belong to the zone 7 boss (Boss7PartD5) and are not placed
 * in the act maps.  See docs/dreamcast/translation/OBJECTS.md (section 5d).
 */
#include "obj_zone.h"

/* ===================================================== Cannon (0x95) */
static void Cannon_RotateBarrel(Obj *w) {
    u16 base = 0;
    s16 spd = 0x100;
    s32 t;
    if (w->prm->flags & 8)
        spd = (s16)(spd << 1);
    if (w->prm->flags & 4) {
        w->w8 += spd;
        w->w8 &= 0xffff;
        return;
    }
    base = (u16)(base + ((w->prm->flags & 3) << 14));
    if (w->f1F4 & 2) {
        t = base + 0x2000;
        w->w8 = ObjAngle_MoveToward16((u16)w->w8, (u16)t, (u16)spd);
        if (w->w8 == (u16)t)
            w->f1F4 ^= 2;
    } else {
        t = base - 0x2000;
        w->w8 = ObjAngle_MoveToward16((u16)w->w8, (u16)t, (u16)spd);
        if (w->w8 == (u16)t)
            w->f1F4 ^= 2;
    }
}
static void Cannon_State_Aim(Obj *w) {
    DcPlayer *l = (DcPlayer *)w->target;
    if (l != NULL) {
        if (w->f1F4 & 1) {
            Cannon_RotateBarrel(w);
            w->h.angle = (u8)((u32)w->w8 >> 8);
        } else if (l->timer == 0) {
            w->f1F4 |= 1;
            w->h.hitstop = 0x80;
            z_spr_prio(&w->k.cannon.a, 1); /* sprA/sprB (@0x20C) */
            z_set_prio(w, 1);
        }
        if (!(l->move_flags & 0x2000)) {
            w->f1F4 &= ~3u;
            w->target = NULL;
            w->wC = 8;
        }
        return;
    }
    if (w->wC == 0)
        return;
    w->wC--;
    w->h.shake_x = (s8)-((w->wC * 8 * ZCOS((u16)(w->h.angle << 8))) >> 12);
    w->h.shake_y = (s8)-((w->wC * 8 * ZSIN((u16)(w->h.angle << 8))) >> 12);
    if (w->wC == 0) {
        z_spr_prio(&w->k.cannon.a, 2);
        z_set_prio(w, 2);
    }
}
static void Cannon_OnHit(ObjBox *a, ObjBox *b) {
    Obj *t0 = z_box_obj(a);
    DcPlayer *t1 = z_box_player(b);
    if (t0 == NULL || t1 == NULL)
        return;
    if (t1->type != 1)
        return;
    if (t1->state != Player_State_Cannon)
        z_count("cannon", t0, t1);
    t0->target = t1;
    Player_Action_Cannon(t1, t0);
}
static void Cannon_Draw(Obj *t0) {
    obj_draw2d(t0);
    obj_draw2d_ex(t0->h.x, t0->h.y, t0, &t0->k.cannon.a);
    t0->h.disp_flags |= 0x100;
    obj_draw2d_ex(t0->h.x, t0->h.y, t0, &t0->k.cannon.c);
    t0->h.disp_flags &= ~0x100;
}
static void cannon_sprite(ObjSprite *s, int anim, u16 order) {
    obj_res_ensure(OBJ_BAC_CANNON);
    spr_init(s, &g_obj.res[OBJ_BAC_CANNON], (u16)anim, 0x40000);
    s->order = order;
}
Obj *Cannon_Create(ObjParam *p, s32 b, s32 c) {
    Obj *w = z_mapobj(p, b, c, OBJ_BAC_CANNON, OBJ_PRIO_MAPOBJ, obj_mapobject_dtor); /* Cannon_Destructor */
    if (w == NULL)
        return NULL;
    obj_set_anim(w, 0);
    w->spr.order = 0x14;
    cannon_sprite(&w->k.cannon.c, 1, 0x15); /* sprC/sprD (@0x29C): the barrel */
    w->k.cannon.c.flags |= 0x200;
    cannon_sprite(&w->k.cannon.a, 2, 0x16); /* sprA/sprB (@0x20C) */
    w->xspr[0] = &w->k.cannon.a;
    w->xspr[1] = &w->k.cannon.c;
    w->n_xspr = 2;
    w->fn_draw = Cannon_Draw;
    rect_set_atkdef(&w->coll[0], 0, (u32)-16);
    w->coll[0].cb = Cannon_OnHit;
    w->coll[0].flags |= 0x800;
    w->h.move_flags |= 0x2100;
    /* Cannon_Action_Init */
    obj_set_anim(w, 0);
    w->state = Cannon_State_Aim;
    w->w8 = (w->prm->flags & 3) << 0xe;
    w->h.angle = (u8)((u32)w->w8 >> 8);
    return w;
}

/* ===================================================== Duct (0x97) */
static void Duct_State_CheckPlayer(Obj *p0) {
    if (RingManager_SuckScatteredRings(&p0->h)) {
        if (p0->h.disp_flags & 0x10) {
            obj_sfx(0x65);
            z_count("duct", p0, NULL);
        }
        /* Sound_SetPositional: dropped */
        p0->h.disp_flags &= ~0x10;
        return;
    }
    if (p0->h.disp_flags & 8)
        p0->h.disp_flags |= 0x10;
}
Obj *Duct_Create(ObjParam *a, s32 b, s32 c) {
    Obj *w = z_mapobj(a, b, c, OBJ_BAC_DUCT, OBJ_PRIO_MAPOBJ, obj_mapobject_dtor);
    if (w == NULL)
        return NULL;
    w->spr.order = 0x14;
    w->h.move_flags |= 0x2100;
    w->snd = 1;
    /* Duct_Action_Init */
    obj_set_anim(w, 0);
    w->h.disp_flags |= 0x10;
    w->h.disp_flags |= 4;
    w->state = Duct_State_CheckPlayer;
    return w;
}

/* ===================================================== GmkAir (0x94) */
static void GmkAir_OnHit(ObjBox *a, ObjBox *b) {
    Obj *w = z_box_obj(a);
    DcPlayer *p = z_box_player(b);
    s32 dy;
    if (w == NULL || p == NULL)
        return;
    if (!rect_box_check(a, p->x >> 8, p->y >> 8, -6, -0x10, 6, 0x10))
        return;
    if (p->type != 1)
        return;
    dy = w->prm->p7 << 8;
    if (w->h.disp_flags & 2)
        dy = -dy;
    if (p->state != Player_State_Air)
        z_count("air", w, p);
    Player_Action_Air(p, w, w->h.x + ((w->prm->p6 - 6) << 8), w->h.y + dy,
                      w->h.x + ((w->prm->p6 + w->prm->p8 + 6) << 8));
}
Obj *GmkAir_Create(ObjParam *p, s32 b, s32 c) {
    Obj *w = z_mapobj(p, b, c, OBJ_BAC_AIR, OBJ_PRIO_MAPOBJ, obj_mapobject_dtor);
    if (w == NULL)
        return NULL;
    w->spr.order = 0x14;
    rect_set_box2d(&w->coll[0], p->p6, (s16)(p->p7 - 0x60), (s16)(p->p8 + p->p6), (s16)(p->p9 + p->p7));
    rect_set_atkdef(&w->coll[0], 0, (u32)-16);
    w->coll[0].flags |= 0xc0;
    w->coll[0].cb = GmkAir_OnHit;
    z_box_owner(w, &w->coll[0]);
    w->h.move_flags |= 0x2100;
    if (obj_gravity_flipped(w->h.x, w->h.y))
        w->h.disp_flags |= 2;
    return w;
}

/* ===================================================== MapObj9C (0x9c): depth lane */
static void MapObj9C_OnHit(ObjBox *a, ObjBox *b) {
    Obj *t0 = z_box_obj(a);
    DcPlayer *t1 = z_box_player(b);
    if (t0 == NULL || t1 == NULL)
        return;
    if (t1->type != 1)
        return;
    if (!rect_point_check(a, t1->x >> 8, t1->y >> 8))
        return;
    Player_Unk_ov000_02350adc(t1, t0);
}
Obj *MapObj9C_Create(ObjParam *p, s32 b, s32 c) {
    Obj *w = z_mapobj(p, b, c, -1, OBJ_PRIO_MAPOBJ, obj_mapobject_dtor);
    ObjParam *q;
    if (w == NULL)
        return NULL;
    w->kind = OBJK_TRIGGER;
    q = w->prm;
    if (q->p8 == 0 && q->p9 == 0)
        rect_set_box3d(&w->coll[0], -32, -32, 32, 32, -0x100, 0x100);
    else
        rect_set_box3d(&w->coll[0], q->p6, q->p7, (s16)(q->p8 + q->p6), (s16)(q->p9 + q->p7), -0x100, 0x100);
    w->h.z = -0x4000;
    if (w->prm->flags & 1)
        w->h.z = -w->h.z;
    rect_set_atkdef(&w->coll[0], 0, (u32)-16);
    w->coll[0].flags |= 0xc0;
    w->coll[0].cb = MapObj9C_OnHit;
    z_box_owner(w, &w->coll[0]);
    w->h.move_flags |= 0x2120;
    return w;
}

/* ===================================================== Rocket (0x93) */
static void Rocket_State_Fly(Obj *w);
static void Rocket_State_Idle(Obj *w) { (void)w; }
static void Rocket_Explode(Obj *p0) {
    Obj *t0;
    if (p0->h.disp_flags & 0x20)
        return;
    /* NNS_SndPlayerStopSeq: dropped */
    obj_sfx(0x64);
    EffZakoBomb_Create(&p0->h, 0, 0);
    eff_zako_debris(p0, 0, -24, 0, -0x200, 4);
    eff_zako_debris(p0, 0, -24, 0, -0x200, 5);
    eff_zako_debris(p0, 0, -24, 0, -0x200, 6);
    eff_zako_debris(p0, 0, -24, 0, -0x200, 7);
    obj_screen_shake(4);
    z_fxhook(NULL, OBJ_FX_SCREEN_FLASH, 0, 0);
    t0 = AttackHitbox_Create(&p0->h, -48, -48, 0x30, 0x30, 8);
    if (t0 != NULL)
        t0->coll[1].mask = 0x21;
    p0->h.obj_flags |= 8;
    p0->h.disp_flags |= 0x20;
    z_count("rocket_explode", p0, NULL);
}
static void Rocket_Action_Launch(Obj *w, DcPlayer *a) {
    w->state = Rocket_State_Fly;
    w->h.move_flags |= 0x40;
    w->h.move_flags &= ~0x2100u;
    w->h.move_flags &= ~0x200u;
    w->h.gnd_vel = -0x800;
    if (w->h.disp_flags & 1)
        w->h.gnd_vel = 0x800;
    w->target = a;
    obj_sfx(99);
}
static void Rocket_State_Fly(Obj *w) {
    DcPlayer *t = (DcPlayer *)w->target;
    s16 x, y;
    Obj *e;
    if (t != NULL) {
        if (t->gimmick == (void *)w)
            w->h.angle = t->angle;
        else
            w->target = NULL;
    }
    /* (else Sound_SetPositional: dropped) */
    if (!(W.frame & 3)) {
        x = (s16)(0x33 - (RAND() & 6));
        y = (s16)(3 - (RAND() & 6));
        dcp_rotate_vec(&x, &y, w->h.angle);
        e = EffZakoBombPuff_Create(w, x, y);
        if (e != NULL)
            e->spr.order = 0x15; /* @0xDA / @0x122 */
    }
    if ((w->h.move_flags & 0xf) || w->h.gnd_vel == 0)
        Rocket_Explode(w);
    if (w->h.disp_flags & 1)
        w->h.gnd_vel = (s16)dcp_spd_up(w->h.gnd_vel, 0x10, 0xc00);
    else
        w->h.gnd_vel = (s16)dcp_spd_up(w->h.gnd_vel, -0x10, 0xc00);
}
static void Rocket_OnHit(ObjBox *a, ObjBox *b) {
    Obj *t0 = z_box_obj(a);
    DcPlayer *t1 = z_box_player(b);
    if (t0 == NULL || t1 == NULL)
        return;
    if (t1->type != 1)
        return;
    obj_sfx(0x6d);
    Rocket_Action_Launch(t0, t1);
    z_count("rocket", t0, t1);
    if (t0->prm->flags & 1) {
        Player_Action_Rocket(t1, t0, 0xc, 0);
        return;
    }
    Player_Action_Rocket(t1, t0, -12, 0);
}
static void Rocket_OnHit_Box1(ObjBox *a, ObjBox *b) {
    Obj *t0 = z_box_obj(a);
    if (t0 == NULL || b->owner == NULL)
        return;
    Rocket_Explode(t0);
}
Obj *Rocket_Create(ObjParam *a, s32 b, s32 c) {
    Obj *w = z_mapobj(a, b, c, OBJ_BAC_ROCKET, 0xf6, obj_mapobject_dtor);
    if (w == NULL)
        return NULL;
    w->spr.order = 0x14;
    w->spr.flags |= 0x200;
    z_map_hitbox(w, -0xc, -0xc, 0xc, 0xc);
    rect_set_atkdef(&w->coll[0], 0, (u32)-0x10);
    w->coll[0].cb = Rocket_OnHit;
    w->coll[0].flags |= 0x800;
    rect_set_atkdef(&w->coll[1], 0, (u32)-0x10);
    w->coll[1].cb = Rocket_OnHit_Box1;
    w->coll[1].flags |= 0x800;
    w->coll[1].mask = 0x21;
    if (w->prm->flags & 1)
        w->h.disp_flags ^= 1;
    w->h.move_flags |= 0x2110;
    w->h.move_flags |= 0x80000;
    w->h.move_flags &= ~0x80u;
    w->h.disp_flags |= 0x100;
    if (obj_gravity_flipped(w->h.x, w->h.y))
        w->h.grav_rot = 0x80;
    w->snd = 1;
    obj_set_anim(w, 0); /* Rocket_Action_Idle */
    w->state = Rocket_State_Idle;
    return w;
}

/* ===================================================== Swing (0x96) */
static void Swing_State_Main(Obj *w) {
    DcPlayer *p = (DcPlayer *)w->target;
    s32 v;
    if (p != NULL) {
        if (!(p->move_flags & 0x2000)) {
            w->wC = 8;
            if (p->move_flags & 1) {
                w->h.angle = 0;
                w->wC = 0;
            } else {
                w->wC = (s8)w->h.angle << 8;
            }
            w->h.gnd_vel = p->gnd_vel;
            w->wC -= w->h.gnd_vel;
            w->target = NULL;
            return;
        }
        w->h.angle = p->angle;
        w->h.gnd_vel = p->gnd_vel;
        return;
    }
    if (!(w->h.obj_flags & 2))
        return;
    v = w->wC;
    if ((v < 0 ? -v : v) < 0x100 && (w->h.gnd_vel < 0 ? -w->h.gnd_vel : w->h.gnd_vel) < 0x80) {
        w->wC = 0;
        w->h.gnd_vel = 0;
    } else if (v < 0) {
        if (w->h.gnd_vel > 0)
            w->h.gnd_vel = (s16)dcp_spd_down(w->h.gnd_vel, 0x20);
        w->h.gnd_vel = (s16)dcp_spd_up(w->h.gnd_vel, -0x20, 0x300);
    } else {
        if (w->h.gnd_vel < 0)
            w->h.gnd_vel = (s16)dcp_spd_down(w->h.gnd_vel, 0x20);
        w->h.gnd_vel = (s16)dcp_spd_up(w->h.gnd_vel, 0x20, 0x300);
    }
    w->wC -= w->h.gnd_vel;
    w->h.angle = (u8)((w->wC & 0xffff) >> 8);
    if (w->h.gnd_vel == 0 && w->h.angle == 0)
        w->h.obj_flags &= ~2u;
}
static void Swing_OnHit(ObjBox *a, ObjBox *b) {
    Obj *t0 = z_box_obj(a);
    DcPlayer *t1 = z_box_player(b);
    if (t0 == NULL || t1 == NULL)
        return;
    if (t1->gnd_vel == 0)
        return;
    if ((t1->move_flags & 1) == 0)
        return;
    t0->target = t1;
    t0->h.obj_flags |= 2;
    if (t1->state != Player_State_Swing)
        z_count("swing", t0, t1);
    Player_Action_Swing(t1, t0, 0, 0x34);
}
/* Swing_Draw: the pole, then the two handle sprites twice (the second
 * time mirrored on both axes) */
static void Swing_Draw(Obj *t0) {
    u8 t2 = t0->h.angle;
    t0->h.angle = 0;
    obj_draw2d(t0);
    t0->h.angle = t2;
    obj_draw2d_ex(t0->h.x, t0->h.y, t0, &t0->k.swing.s0);
    obj_draw2d_ex(t0->h.x, t0->h.y, t0, &t0->k.swing.s2);
    t0->k.swing.m0 = t0->k.swing.s0;
    t0->k.swing.m2 = t0->k.swing.s2;
    t0->h.disp_flags ^= 1;
    t0->h.disp_flags ^= 2;
    obj_draw2d_ex(t0->h.x, t0->h.y, t0, &t0->k.swing.s0);
    obj_draw2d_ex(t0->h.x, t0->h.y, t0, &t0->k.swing.s2);
    t0->h.disp_flags ^= 1;
    t0->h.disp_flags ^= 2;
}
static void swing_sprite(ObjSprite *s, int anim, u16 order) {
    obj_res_ensure(OBJ_BAC_SWING);
    spr_init(s, &g_obj.res[OBJ_BAC_SWING], (u16)anim, 0x40000);
    s->order = order;
}
Obj *Swing_Create(ObjParam *a, s32 b, s32 c) {
    Obj *w = z_mapobj(a, b, c, OBJ_BAC_SWING, OBJ_PRIO_MAPOBJ, obj_mapobject_dtor); /* Swing_Destructor */
    if (w == NULL)
        return NULL;
    w->spr.order = 0x14;
    w->fn_draw = Swing_Draw;
    swing_sprite(&w->k.swing.s0, 1, 9);
    z_spr_prio(&w->k.swing.s0, 1);
    w->k.swing.s0.flags |= 0x200;
    swing_sprite(&w->k.swing.s2, 2, 0x15);
    w->k.swing.s2.flags |= 0x200;
    w->k.swing.m0 = w->k.swing.s0;
    w->k.swing.m2 = w->k.swing.s2;
    w->xspr[0] = &w->k.swing.m0;
    w->xspr[1] = &w->k.swing.m2;
    w->xspr[2] = &w->k.swing.s0;
    w->xspr[3] = &w->k.swing.s2;
    w->n_xspr = 4;
    rect_set_box2d(&w->coll[0], -4, 0x30, 4, 0x38);
    rect_set_atkdef(&w->coll[0], 0, (u32)-0x10);
    w->coll[0].cb = Swing_OnHit;
    w->coll[0].flags |= 0x800;
    z_box_owner(w, &w->coll[0]);
    rect_set_box2d(&w->coll[1], -4, -0x38, 4, -0x30);
    rect_set_atkdef(&w->coll[1], 0, (u32)-0x10);
    w->coll[1].cb = Swing_OnHit;
    w->coll[1].flags |= 0x800;
    z_box_owner(w, &w->coll[1]);
    w->h.move_flags |= 0x2100;
    w->h.disp_flags |= 0x100;
    if (obj_gravity_flipped(w->h.x, w->h.y))
        w->h.grav_rot = 0x80;
    w->state = Swing_State_Main; /* Swing_Action_Init */
    return w;
}

/* ===================================================== DecArrow (0x88..0x8f) */
Obj *DecArrow_Create(ObjParam *p, s32 a, s32 b) {
    Obj *w = obj_alloc(OBJ_PRIO_MAPOBJ, Decoration_Destructor);
    if (w == NULL)
        return NULL;
    Decoration_InitFromMap(w, p, a, b);
    obj_load_bac(w, OBJ_BAC_DEC_ARROW);
    if (p->type <= 0x89)
        obj_set_anim(w, 0); /* (shared VRAM data_ov000_0236a790 / _7a0 / _7b0: own frame natively) */
    else if (p->type <= 0x8b)
        obj_set_anim(w, 1);
    else
        obj_set_anim(w, 2);
    if (p->type == 0x89 || p->type == 0x8d || p->type == 0x8f)
        w->h.disp_flags ^= 1;
    if (p->type == 0x8b || p->type >= 0x8e)
        w->h.disp_flags ^= 2;
    w->spr.order = 0x16;
    z_fxhook(NULL, OBJ_FX_PALETTE_ANIM, 0x0236457c, p->type); /* PaletteAnim_Create */
    w->h.obj_flags |= 0x800;
    if (w->spr.order == 0)
        w->spr.order = 0x16;
    w->h.move_flags |= 0x2100;
    w->h.obj_flags |= 2;
    return w;
}
