/*
 * Leaf Storm objects, copied from src/game/Zone/Leaf_Storm/ and adapted
 * (task/work struct -> Obj with named fields, VRAM/palette calls dropped,
 * Sound_PlaySeqArcEx -> obj_sfx, ScreenShake_Start -> obj_screen_shake):
 *   Trampoline_*, BendStick_*, CorkscrewTrigger_*, RockBreak_*,
 *   RockBreakPart_*, RopeC_*, RopeCLand_*, RopeCSpring_*, EggPawnM_Create,
 *   Enemy_Action_StartPopOnContact, Enemy_State_PopOnContact.
 * Collision map fields: @0x1B8 cmap.obj, @0x1BC cmap.rider, @0x1C8 map,
 * @0x1D0 attr, @0x1D4/@0x1D6 ox/oy, @0x1D8/@0x1DA w/h.
 */
#include <string.h>
#include "obj_internal.h"

static Obj *mapobj(ObjParam *p, s32 x, s32 y, int bac) {
    Obj *o = obj_alloc(OBJ_PRIO_MAPOBJ, obj_mapobject_dtor);
    if (o == NULL)
        return NULL;
    obj_mapobject_init(o, p, x, y);
    o->kind = OBJK_GIMMICK;
    if (bac >= 0)
        obj_load_bac(o, bac);
    return o;
}
static void cmap_solid(Obj *w, u16 cw, u16 ch, s16 ox, s16 oy) {
    w->has_cmap = 1;
    w->cmap.obj = w;
    w->cmap.map = (const u8(*)[8])g_obj.data.solid;
    w->cmap.w = cw;
    w->cmap.h = ch;
    w->cmap.ox = ox;
    w->cmap.oy = oy;
}
static void count(const char *ev, Obj *o, DcPlayer *p) {
    g_obj.stats.gimmicks++;
    obj_log(ev, o->prm ? o->prm->type : 0, p ? p->x >> 8 : o->h.x >> 8);
}

/* ========================================================== Trampoline */
static void Trampoline_State_Idle(Obj *s) { (void)s; }
static void Trampoline_Action_Idle(Obj *s) {
    obj_set_anim(s, 0);
    s->state = Trampoline_State_Idle;
}
static void Trampoline_State_Bounce(Obj *s);
static void Trampoline_Action_Bounce(Obj *s) {
    obj_set_anim(s, 1);
    s->state = Trampoline_State_Bounce;
}
static void Trampoline_State_Bounce(Obj *s) {
    if (!(s->h.disp_flags & 8))
        return;
    Trampoline_Action_Idle(s);
}
static void Trampoline_OnHit(ObjBox *a, ObjBox *b) {
    Obj *s = (Obj *)a->owner;
    DcPlayer *t = obj_box_player(b);
    s16 v;
    Trampoline_Action_Bounce(s);
    s->coll[0].flags &= ~0x100;
    if (t == NULL) /* (the original reads t->unk2A before this test) */
        return;
    v = (s16)-(t->move_y + (t->move_y >> 3) + (t->move_y >> 4));
    if (v < -0xc00)
        v = -0xc00;
    else if (v > -0x780)
        v = -0x780;
    dcp_act_spring(t, 0, v);
    obj_player_set_source(t, s);
    count("trampoline", s, t);
}
Obj *Trampoline_Create(ObjParam *a, s32 b, s32 c) {
    Obj *w = mapobj(a, b, c, OBJ_BAC_TRAMPOLINE);
    if (w == NULL)
        return NULL;
    w->spr.order = 0x14;
    rect_set_atkdef(&w->coll[0], 0, (u32)-0x10);
    w->coll[0].cb = Trampoline_OnHit;
    w->coll[0].flags |= 0x800;
    w->h.move_flags |= 0x2100;
    Trampoline_Action_Idle(w);
    return w;
}

/* ========================================================== Bend stick */
static void BendStick_State_Wobble(Obj *w) {
    const s8 *t4 = &g_obj.data.bend_wobble[0], *t8 = &g_obj.data.bend_wobble[4], *tc = &g_obj.data.bend_wobble[8];
    if (w->spr.anim != 0) {
        s32 t = w->wC;
        if (t != 0) {
            switch (w->w8) {
            default:
                w->cmap.oy = t4[(10 - t) / 5];
                w->h.move_y = (s16)((w->cmap.oy << 8) / 5);
                break;
            case 1:
                w->cmap.oy = t8[(15 - t) / 5];
                w->h.move_y = (s16)((w->cmap.oy << 8) / 5);
                break;
            case 2:
                w->cmap.oy = tc[20 - t];
                w->h.move_y = (s16)((w->cmap.oy << 8) / 5);
                break;
            }
            w->wC--;
        } else {
            w->cmap.oy = 0;
            w->h.move_y = 0;
        }
        if (w->h.disp_flags & 8)
            obj_set_anim(w, 0);
    } else {
        w->cmap.oy = 0;
        w->h.move_y = 0;
    }
}
static void BendStick_Action_Init(Obj *s) {
    obj_set_anim(s, 0);
    s->state = BendStick_State_Wobble;
}
static void BendStick_OnHit(ObjBox *a, ObjBox *b) {
    Obj *w = (Obj *)a->owner;
    DcPlayer *p = w->cmap.rider;
    s32 t, d, v, dx;
    if (p != NULL && p->type == 1 && b->owner == p && w->cmap.oy == 0) {
        dx = p->x - w->h.x;
        if (dx < 0)
            dx = -dx;
        d = dx >> 8;
        if (d < 0x10) {
            t = 10;
            obj_set_anim(w, 3);
            v = -0x500;
            w->w8 = 0;
        } else if (d < 0x20) {
            t = 15;
            obj_set_anim(w, 2);
            v = -0x580;
            w->w8 = 1;
        } else {
            t = 20;
            obj_set_anim(w, 1);
            v = -0x600;
            w->w8 = 2;
        }
        w->wC = t;
        Player_Action_BendStick(p, (s16)(v - d * 0x18), t);
        obj_player_set_source(p, w);
    } else {
        rect_no_hit(a, b);
    }
}
Obj *BendStick_Create(ObjParam *p, s32 b, s32 c) {
    Obj *w = mapobj(p, b, c, OBJ_BAC_BEND_STICK);
    if (w == NULL)
        return NULL;
    w->spr.order = 0x14;
    rect_set_atkdef(&w->coll[0], 0, (u32)-16);
    w->coll[0].cb = BendStick_OnHit;
    w->coll[0].flags |= 0x800;
    cmap_solid(w, 5, 1, 2, 0);
    w->cmap.attr = g_obj.data.col_attr;
    w->h.move_flags |= 0x2100;
    if (w->prm->type == 0x57)
        w->h.disp_flags &= ~1;
    BendStick_Action_Init(w);
    return w;
}

/* =================================================== Corkscrew trigger */
static void CorkscrewTrigger_OnHit(ObjBox *a, ObjBox *b) {
    Obj *o = (Obj *)a->owner;
    ObjParam *prm = o->prm;
    u16 f = prm->flags;
    u16 g = f & ~0x80;
    DcPlayer *p;
    s32 lim;
    if (prm->type == 0x69)
        g |= prm->p6 << 2;
    p = obj_box_player(b);
    if (p == NULL)
        return;
    if (p->type != 1)
        return;
    lim = p->run_thr[2]; /* @0x37C */
    if (f & 0x80)
        lim = 0;
    if ((p->gnd_vel >= lim && !(f & 1)) || (p->gnd_vel <= -lim && (f & 1))) {
        if (prm->type == 0x69)
            g |= 0x1000;
        if (f & 0x80)
            g |= 0x2000;
        if (f & 0x80) {
            if (p->anim >= 0x1a && p->anim <= 0x20)
                Player_Action_CorkscrewPath(p, o->h.x, o->h.y, g);
        } else {
            Player_Action_CorkscrewPath(p, o->h.x, o->h.y, g);
        }
    }
}
Obj *CorkscrewTrigger_Create(ObjParam *a, s32 b, s32 c) {
    Obj *w = mapobj(a, b, c, -1);
    if (w == NULL)
        return NULL;
    w->kind = OBJK_TRIGGER;
    if (w->prm->flags & 1)
        rect_set_box2d(&w->coll[0], -0x10, -8, -4, 0);
    else
        rect_set_box2d(&w->coll[0], 4, -8, 0x10, 0);
    rect_set_atkdef(&w->coll[0], 0, (u32)-0x10);
    w->coll[0].flags |= 0xc0;
    w->coll[0].cb = CorkscrewTrigger_OnHit;
    w->coll[0].owner = w;
    w->coll[0].owner_kind = OBJ_OWNER_OBJECT;
    w->h.move_flags |= 0x2120;
    return w;
}

/* ========================================================== Rock break */
static void RockBreakPart_State_Fly(Obj *s) { s->h.vel_x = (s16)dcp_spd_down(s->h.vel_x, 10); }
static void RockBreak_CreatePart_ov000_023136ec(Obj *arg, s16 x, s16 y, s32 type) {
    Obj *w = obj_alloc(OBJ_PRIO_MAPOBJ, obj_stageobj_dtor);
    if (w == NULL)
        return;
    w->kind = OBJK_EFFECT;
    obj_load_bac(w, OBJ_BAC_ROCK_BREAK);
    obj_set_anim(w, type != 0 ? 2 : 1);
    w->spr.order = 0x14;
    w->h.x = arg->h.x;
    w->h.y = arg->h.y;
    w->h.vel_x = x;
    w->h.vel_y = y;
    w->h.gravity = 0x2a;
    w->h.fall_max = 0xf00;
    w->h.obj_flags |= 1;
    w->h.move_flags |= 0x180;
    w->state = RockBreakPart_State_Fly; /* RockBreakPart_Action_Fly */
}
static void RockBreak_State_Idle(Obj *s) { (void)s; }
static void RockBreak_OnHit(ObjBox *a, ObjBox *b) {
    Obj *t0 = (Obj *)a->owner;
    DcPlayer *t1 = obj_box_player(b);
    if (t1 == NULL)
        return;
    if (!(t1->move_flags & 0x10))
        return;
    obj_player_attack_bounce(t1);
    obj_screen_shake(3);
    t0->h.obj_flags |= 8;
    t0->h.obj_flags |= 2;
    t0->f1F4 |= 0x10;
    t0->coll[0].flags |= 0x1000;
    t0->coll[1].flags |= 0x1000;
    RockBreak_CreatePart_ov000_023136ec(t0, 0x300, -0x200, 0);
    RockBreak_CreatePart_ov000_023136ec(t0, -0x200, -0x280, 0);
    RockBreak_CreatePart_ov000_023136ec(t0, -0x330, -0x2e0, 1);
    RockBreak_CreatePart_ov000_023136ec(t0, 0x240, -0x320, 1);
    obj_sfx(0x4c);
    g_obj.stats.rocks_broken++;
    count("rock_break", t0, t1);
}
Obj *RockBreak_Create(ObjParam *a, s32 b, s32 c) {
    Obj *w = mapobj(a, b, c, OBJ_BAC_ROCK_BREAK);
    if (w == NULL)
        return NULL;
    w->spr.order = 0x12;
    rect_set_atkdef(&w->coll[0], 0, 8);
    w->coll[0].cb = RockBreak_OnHit;
    w->h.move_flags |= 0x2100;
    cmap_solid(w, 5, 4, -0x12, -0x10);
    obj_set_anim(w, 0); /* RockBreak_Action_Idle */
    w->state = RockBreak_State_Idle;
    return w;
}

/* =============================================================== RopeC */
static void RopeCLand_State_Idle(Obj *p0) {
    Obj *t0 = p0->parent;
    if (t0 == NULL)
        return;
    p0->spr.flags &= ~0x60000u;
    if (t0->f1F4 & 1) {
        p0->spr.flags |= 0x20000;
        return;
    }
    p0->spr.flags |= 0x40000;
}
Obj *RopeCLand_Create(ObjParam *a, s32 b, s32 c) {
    Obj *w = mapobj(a, b, c, OBJ_BAC_ROPE_C_LAND);
    if (w == NULL)
        return NULL;
    w->spr.order = 0x14;
    w->h.obj_flags |= 2;
    w->h.move_flags |= 0x2100;
    obj_set_anim(w, 0); /* RopeCLand_Action_Idle */
    w->state = RopeCLand_State_Idle;
    return w;
}
static void RopeCSpring_State_Idle(Obj *p0);
static void RopeCSpring_State_Active(Obj *p0);
static void RopeCSpring_Action_Idle(Obj *s) {
    obj_set_anim(s, 0);
    s->state = RopeCSpring_State_Idle;
}
static void RopeCSpring_Action_Active(Obj *p0) {
    obj_set_anim(p0, 0);
    p0->state = RopeCSpring_State_Active;
    p0->h.hitstop = 0x80;
    p0->wC = 0x10;
    p0->scale_x = 0x140;
    p0->scale_y = p0->scale_x;
}
static void RopeCSpring_State_Active(Obj *p0) {
    s32 t0 = p0->scale_x;
    Obj *t1;
    if (t0 > 0x100) {
        p0->scale_x = (s16)(t0 - 0x20);
        p0->scale_y = p0->scale_x;
    }
    t1 = p0->parent;
    if (t1 == NULL)
        return;
    if (t1->h.obj_flags & 2)
        return;
    p0->scale_x = 0;
    p0->scale_y = p0->scale_x;
    RopeCSpring_Action_Idle(p0);
}
static void RopeCSpring_State_Idle(Obj *p0) {
    Obj *t0 = p0->parent;
    if (t0 == NULL)
        return;
    if (!(t0->h.obj_flags & 2))
        return;
    RopeCSpring_Action_Active(p0);
}
Obj *RopeCSpring_Create(ObjParam *a, s32 b, s32 c) {
    Obj *w = mapobj(a, b, c, OBJ_BAC_ROPE_C_SPRING);
    if (w == NULL)
        return NULL;
    w->spr.flags |= 0x200;
    w->spr.order = 0x14;
    w->h.obj_flags |= 2;
    w->h.obj_flags |= 0x10;
    w->h.move_flags |= 0x2100;
    RopeCSpring_Action_Idle(w);
    return w;
}
/* Model_SetShapeResource(model, shape): RopeC switches between the two shapes */
static void ropec_shape(Obj *s, int shape) { s->k.mdl.shape = (u8)shape; }
static void RopeC_State_Main(Obj *w) {
    DcPlayer *p = (DcPlayer *)w->target;
    ObjMtx33 m, mtx;
    s32 idx;
    if (p != NULL) {
        if (p->gimmick != w) {
            w->h.disp_flags |= 0x200;
            w->h.obj_flags &= ~2u;
            w->target = NULL;
            ropec_shape(w, 1);
            return;
        }
        MTX_Identity33_(&m);
        idx = (u16)((u32)p->trick_input & 0xffff);
        MTX_RotX33_(&mtx, obj_sin((u16)idx), obj_cos((u16)idx));
        MTX_Concat33(&m, &mtx, &m);
        idx = (u16)(-((u8)(p->rot3 - 0x40) << 8));
        MTX_RotY33_(&mtx, obj_sin((u16)idx), obj_cos((u16)idx));
        MTX_Concat33(&m, &mtx, &m);
        memcpy(w->k.mdl.mtx, m.m, sizeof(m.m));
        w->k.mdl.scale[0] = 0x34cc;
        w->k.mdl.scale[1] = 0x34cc;
        w->k.mdl.scale[2] = (((p->timer << 8) / 0xa000) * 0x34cc) >> 8;
        if (p->rot3 < 0x80)
            w->f1F4 |= 1;
        else
            w->f1F4 &= ~1u;
    } else {
        w->f1F4 &= ~1u;
        MTX_Identity33_(&m);
        MTX_RotX33_(&mtx, obj_sin(0xf000), obj_cos(0xf000)); /* data_02072070 = data_0206e470 + 0x3c00: angle 0xf000 */
        MTX_Concat33(&m, &mtx, &m);
        MTX_RotY33_(&mtx, obj_sin(0x4000), obj_cos(0x4000)); /* data_0206f470 = data_0206e470 + 0x1000: angle 0x4000 */
        MTX_Concat33(&m, &mtx, &m);
        memcpy(w->k.mdl.mtx, m.m, sizeof(m.m));
        w->k.mdl.scale[0] = 0x34cc;
        w->k.mdl.scale[1] = 0x34cc;
        w->k.mdl.scale[2] = 0x34cc;
    }
}
static void RopeC_OnHit(ObjBox *a, ObjBox *b) {
    Obj *s = (Obj *)a->owner;
    DcPlayer *t;
    if (s->target != NULL)
        return;
    if (b->owner == NULL)
        return;
    t = obj_box_player(b);
    if (t == NULL) /* only players have type 1 here */
        return;
    s->target = t;
    s->h.obj_flags |= 2;
    ropec_shape(s, 0);
    if (t->type != 1)
        return;
    s->h.disp_flags &= ~0x200;
    Player_Action_RopeC(t, s, 0xa000);
    obj_player_set_source(t, s);
}
static void RopeC_SpawnParts(Obj *p0) {
    Obj *t;
    t = obj_mapobject_spawn(0xe0, p0->h.x, p0->h.y, 0, p0->prm->p6, 0, 0, 0);
    if (t != NULL)
        t->parent = p0;
    t = obj_mapobject_spawn(0xe1, p0->h.x - 0x9000, p0->h.y + 0x4000, 0, 0, 0, 0, 0);
    if (t != NULL)
        t->parent = p0;
    t = obj_mapobject_spawn(0xe2, p0->h.x, p0->h.y + 0x1800, 0, 0, 0, 0, 0);
    if (t != NULL)
        t->parent = p0;
}
Obj *RopeC_Create(ObjParam *a, s32 b, s32 c) {
    Obj *w = mapobj(a, b, c, OBJ_BAC_ROPE_C);
    if (w == NULL)
        return NULL;
    w->spr.order = 0x14;
    w->spr.flags |= 0x80000;
    /* StageObj_LoadModelSimple(mod/gmk_rope_c.nsbmd): the model view (objects_model_get) */
    w->h.disp_flags |= 0x200;
    w->k.mdl.scale[0] = w->k.mdl.scale[1] = w->k.mdl.scale[2] = 0x34cc;
    ropec_shape(w, 1);
    if (w->prm->flags & 1)
        rect_set_box2d(&w->coll[0], 0x84, 0x40, 0x8c, 0x48);
    else
        rect_set_box2d(&w->coll[0], -0x88, 0x40, -0x80, 0x48);
    rect_set_atkdef(&w->coll[0], 0, (u32)-0x10);
    w->coll[0].flags |= 0xc0;
    w->coll[0].cb = RopeC_OnHit;
    w->coll[0].owner = w;
    w->coll[0].owner_kind = OBJ_OWNER_OBJECT;
    w->h.move_flags |= 0x2100;
    w->target = NULL;
    RopeC_SpawnParts(w);
    w->state = RopeC_State_Main; /* RopeC_Action_Init */
    RopeC_State_Main(w);
    return w;
}

/* =============================================== Egg Pawn M (type 0xdf) */
static void Enemy_State_PopOnContact(Obj *p0) {
    if (p0->wC != 0) {
        p0->wC--;
        if (p0->wC == 0)
            p0->spr.order = 0xa;
    }
    if (p0->h.move_flags & 0x100)
        return;
    if ((p0->h.move_flags & 0xf) == 0) {
        if ((p0->coll[1].flags & 0x200) == 0)
            return;
    }
    EffZakoBombPuff_Create(p0, 0, 0);
    p0->h.obj_flags |= 4;
}
void Enemy_Action_StartPopOnContact(Obj *s) {
    obj_disable_colliders_set_anim(s, 3);
    s->h.disp_flags |= 4;
    s->state = Enemy_State_PopOnContact;
    s->wC = 0x10;
}
Obj *EggPawnM_Create(ObjParam *p, s32 b, s32 c) {
    Obj *w = mapobj(p, b, c, OBJ_BAC_EGGPAWN_M);
    s32 k;
    if (w == NULL)
        return NULL;
    w->kind = OBJK_ENEMY;
    w->spr.order = 0x15;
    if (!(w->prm->flags & 1))
        w->h.disp_flags &= ~1;
    w->h.vel_x = 0x380;
    if (w->h.disp_flags & 1)
        w->h.vel_x = (s16)-w->h.vel_x;
    k = w->prm->flags & 0xc;
    if (k != 0)
        w->h.vel_x = (s16)((w->h.vel_x * (((k >> 2) << 7) + 0x100)) >> 8);
    w->h.move_flags |= 0x10;
    w->h.disp_flags |= 0x100;
    Enemy_Action_StartPopOnContact(w);
    obj_log("eggpawn_m_child", w->h.x >> 8, w->h.y >> 8);
    return w;
}
