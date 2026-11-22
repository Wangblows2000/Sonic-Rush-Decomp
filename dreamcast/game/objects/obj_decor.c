/*
 * Decorations: the StageEvent decoration spawner and the Decoration
 * framework with the Leaf Storm decorations and the freed animals, copied
 * from the decompilation and adapted (task/work struct -> Obj, VRAM and
 * palette calls dropped):
 *   src/game/Stage/StageEvent_SpawnDecorationsInBlock,
 *   src/game/Decorations/Decoration_* (AllocSlot, FreeSlot, Spawn,
 *     InitFromMap, In_Default, SpriteCallback_Default, Collide_Default,
 *     Destructor), DecGroup35_*, DecAnimal_*,
 *   src/game/Zone/Leaf_Storm/Decorations/ DecGroup0F_Create, DecVine_*,
 *     DecBird_*, DecGroup35Piece_*.
 * Decoration work: @0x134 coll0 = coll[0], @0x160 collision map = cmap,
 * @0x184 entry = prm, @0x188/@0x18C home = home_x/home_y, @0x190 player =
 * target, @0x194 flags = f1F4, @0x198 DecAnimal touch count = k.ani.touch.
 */
#include <stdlib.h>
#include <string.h>
#include "obj_internal.h"

typedef struct { u32 l, t, r, b; } DRect;

/* the box owner's StageObj header (players and objects both start with it) */
static DcPlayer *box_hdr(const ObjBox *b) { return b ? (DcPlayer *)b->owner : NULL; }
static void set_prio(Obj *o, u32 prio) { o->spr.flags = (o->spr.flags & ~0x60000u) | (prio << 17); }

/* ------------------------------------------------------ level binding */
int dec_init_level(const uint8_t *level_data) {
    const rlvl_header_t *h = (const rlvl_header_t *)level_data;
    u32 i;
    g_obj.lv_dec = (const rlvl_decor_t *)rlvl_section(level_data, RLVL_SEC_DECOR, NULL);
    g_obj.lv_dec_region = (const uint32_t *)rlvl_section(level_data, RLVL_SEC_DECOR_REGION, NULL);
    if (h->num_decor == 0 || g_obj.lv_dec == NULL || g_obj.lv_dec_region == NULL) {
        g_obj.lv_dec = NULL;
        return 0;
    }
    g_obj.dprm = (ObjParam *)calloc(h->num_decor, sizeof(ObjParam));
    if (g_obj.dprm == NULL)
        return -1;
    for (i = 0; i < h->num_decor; i++) {
        const rlvl_decor_t *e = &g_obj.lv_dec[i];
        g_obj.dprm[i].x0 = (u8)(e->x & 0xff);
        g_obj.dprm[i].y0 = (u8)(e->y & 0xff);
        g_obj.dprm[i].type = e->type;
        g_obj.dprm[i].ax = e->x;
        g_obj.dprm[i].ay = e->y;
    }
    return 0;
}

/* ---------------------------------------- Decoration_AllocSlot / FreeSlot */
static s32 Decoration_AllocSlot(void) {
    u8 i;
    for (i = 0; i < 32; i++) {
        if (!(g_obj.dec_mask & (1u << i))) {
            g_obj.dec_mask |= 1u << i;
            return i;
        }
    }
    return i;
}
static void Decoration_FreeSlot(ObjParam *e) {
    u8 i;
    for (i = 0; i < 32; i++)
        if (&g_obj.dec_slots[i] == e) {
            g_obj.dec_mask &= ~(1u << i);
            return;
        }
}
/* Decoration_Spawn */
Obj *Decoration_Spawn(s32 type, s32 x, s32 y) {
    Obj *r = NULL;
    s32 i = Decoration_AllocSlot();
    ObjDecCreateFn fn;
    ObjParam *e;
    if (i < 32) {
        fn = obj_dec_create_fn((u16)type);
        e = &g_obj.dec_slots[i];
        memset(e, 0, sizeof(*e));
        e->x0 = 0xff;
        e->y0 = 0xff;
        e->type = (u16)type;
        r = fn ? fn(e, x, y) : NULL;
        if (r == NULL)
            Decoration_FreeSlot(e);
    }
    return r;
}

/* ------------------------------------------------- Decoration framework */
static void Decoration_In_Default(Obj *p) {
    if (p->cmap.rider == NULL)
        return;
    if (p->cmap.rider->obj_flags & 4)
        p->cmap.rider = NULL;
}
static void Decoration_SpriteCallback_Default(void *owner, const HboxEvent *ev) {
    Obj *o = (Obj *)owner;
    if (ev->slot != 0) /* (command 7 only: every .hbx event is one) */
        return;
    o->coll[0].owner = o;
    o->coll[0].owner_kind = OBJ_OWNER_OBJECT;
    rect_set_box2d(&o->coll[0], ev->l, ev->t, ev->r, ev->b);
}
static void Decoration_Collide_Default(Obj *t0) {
    if (t0->h.obj_flags & 0xc)
        return;
    if (t0->coll[0].owner != NULL)
        obj_handle_collider(&t0->h, t0, OBJ_OWNER_OBJECT, &t0->coll[0], t0->has_spr);
    if (!t0->has_cmap)
        return;
    colmap_register(&t0->cmap);
}
void Decoration_Destructor(Obj *w) {
    ObjParam *e = w->prm;
    if (e != NULL && e->x0 == 0xff && e->y0 == 0xff)
        Decoration_FreeSlot(e);
    else if (!(w->f1F4 & 0x10) && e != NULL)
        e->x0 = (u8)w->home_x;
    g_obj.stats.dec_despawned++;
    obj_stageobj_dtor(w);
}
void Decoration_InitFromMap(Obj *w, ObjParam *p, s32 x, s32 y) {
    w->is_dec = 1;
    w->kind = OBJK_DECOR;
    w->fn_in = Decoration_In_Default;
    w->h.obj_flags |= 0x40;
    w->spr_cb = Decoration_SpriteCallback_Default;
    w->fn_collide = Decoration_Collide_Default;
    w->home_x = x >> 8;
    w->home_y = y >> 8;
    w->h.x = x;
    w->h.y = y;
    w->h.type = 4;
    w->h.gravity = 0x2a;
    w->h.fall_max = 0xf00;
    w->h.obj_flags |= 1;
    w->h.move_flags |= 0x200;
    w->h.box_l = -2; /* StageObj_SetMapHitbox(w, -2, -6, 2, 0) */
    w->h.box_t = -6;
    w->h.box_r = 2;
    w->h.box_b = 0;
    w->h.disp_flags |= 1;
    w->coll[0].mask = 0x12;
    rect_set_atkdef(&w->coll[0], 0, 10);
    w->coll[0].flags &= ~4;
    if (p != NULL) {
        w->prm = p;
        w->prm->x0 = 0xff;
    }
    w->out_ofs = (s16)(0x108 - g_obj.data.dec_margin[p->type]);
    g_obj.stats.dec_spawned++;
}
static Obj *dec_alloc(ObjFn dtor) { return obj_alloc(OBJ_PRIO_MAPOBJ, dtor); }

/* ============================================================ DecGroup0F */
static Obj *DecGroup0F_Create(ObjParam *p, s32 a, s32 b) {
    Obj *w = dec_alloc(Decoration_Destructor);
    u16 idx;
    s32 t;
    if (w == NULL)
        return NULL;
    Decoration_InitFromMap(w, p, a, b);
    idx = (u16)(p->type - 0xf);
    if (p->type >= 0xf && p->type <= 0x18) {
        if (p->type >= 0x14)
            idx -= 5;
        obj_load_bac(w, OBJ_BAC_DEC_FLOWER);
        obj_set_anim(w, idx);
        w->spr.order = 6;
        set_prio(w, 0);
        if (p->type >= 0x14)
            w->h.disp_flags &= ~1;
    } else if (p->type >= 0x19 && p->type <= 0x34) {
        obj_load_bac(w, OBJ_BAC_DEC_VINE);
        switch (p->type) {
        default:
        case 0x19: case 0x1a: case 0x1b: case 0x1c: obj_set_anim(w, 0); break;
        case 0x1d: case 0x1e: case 0x1f: case 0x20: obj_set_anim(w, 3); break;
        case 0x21: case 0x22: case 0x23: case 0x24: obj_set_anim(w, 2); break;
        case 0x25: case 0x26: case 0x27: case 0x28: obj_set_anim(w, 1); break;
        case 0x29: case 0x2a: case 0x2b: case 0x2c: obj_set_anim(w, 5); break;
        case 0x2d: case 0x2e: case 0x2f: case 0x30: obj_set_anim(w, 4); break;
        }
        if (p->type & 1)
            w->h.disp_flags |= 1;
        else
            w->h.disp_flags &= ~1;
        t = p->type & 3;
        if (t == 0 || t == 3) {
            w->spr.order = 6;
            set_prio(w, 1);
        }
    } else if (p->type == 0x3a) {
        obj_load_bac(w, OBJ_BAC_DEC_BIRD_NEST);
        w->spr.order = 0x16;
    } else if (p->type >= 0x3b && p->type <= 0x40) {
        obj_load_bac(w, OBJ_BAC_DEC_VINE);
        switch (p->type) {
        default:
        case 0x3c:
            w->h.disp_flags ^= 1;
            /* fall through */
        case 0x3b:
            obj_set_anim(w, 8);
            break;
        case 0x3e:
            w->h.disp_flags ^= 1;
            /* fall through */
        case 0x3d:
            obj_set_anim(w, 9);
            break;
        case 0x40:
            w->h.disp_flags ^= 1;
            /* fall through */
        case 0x3f:
            obj_set_anim(w, 0xa);
            break;
        }
        set_prio(w, 0);
    }
    if (w->spr.order == 0)
        w->spr.order = 0x16;
    w->h.move_flags |= 0x2100;
    w->h.obj_flags |= 2;
    return w;
}

/* =============================================================== DecVine */
static void DecVine_OnHit(ObjBox *a, ObjBox *b) {
    Obj *o = (Obj *)a->owner;
    DcPlayer *p = box_hdr(b);
    s32 v;
    if (o == NULL || p == NULL)
        return;
    if (!(p->move_flags & 1))
        return;
    v = p->move_x;
    if (v < 0)
        v = -v;
    if (v <= 0x200)
        return;
    o->h.shake_y = (s8)(RAND() & 1);
}
static Obj *DecVine_Create(ObjParam *p, s32 a, s32 b) {
    Obj *w = dec_alloc(Decoration_Destructor);
    if (w == NULL)
        return NULL;
    Decoration_InitFromMap(w, p, a, b);
    rect_set_box2d(&w->coll[0], -8, -0x60, 8, -0x20);
    rect_set_atkdef(&w->coll[0], 0, (u32)-0x10);
    w->coll[0].flags |= 0xc0;
    w->coll[0].cb = DecVine_OnHit;
    w->coll[0].owner = w;
    w->coll[0].owner_kind = OBJ_OWNER_OBJECT;
    obj_load_bac(w, OBJ_BAC_DEC_VINE);
    if (p->type < 0x33) {
        obj_set_anim(w, 6);
        set_prio(w, 3);
    } else {
        obj_set_anim(w, 7);
        set_prio(w, 2);
    }
    if (!(p->type & 1)) {
        w->spr.order = 6;
        w->spr.flags -= 0x40000;
    }
    w->h.move_flags |= 0x2100;
    return w;
}

/* =============================================================== DecBird */
static void DecBird_State_FlyAway(Obj *p0) {
    if (p0->h.disp_flags & 1)
        p0->h.vel_x = (s16)dcp_spd_up(p0->h.vel_x, -4, 0x180);
    else
        p0->h.vel_x = (s16)dcp_spd_up(p0->h.vel_x, 4, 0x180);
    p0->h.vel_y = (s16)dcp_spd_up(p0->h.vel_y, -4, 0x240);
}
static void DecBird_Action_FlyAway(Obj *w, DcPlayer *p) {
    s16 s;
    obj_set_anim(w, 1);
    w->h.disp_flags |= 4;
    w->state = DecBird_State_FlyAway;
    w->h.move_flags &= ~0x2000u;
    w->h.obj_flags |= 2;
    w->h.vel_x = 0xa0;
    w->h.vel_y = -0x120;
    if (p != NULL) {
        s = 0;
        s = (s16)(s + (s16)((p->move_x < 0 ? -p->move_x : p->move_x) + (p->move_y < 0 ? -p->move_y : p->move_y)));
        s >>= 3;
        w->h.vel_x = (s16)(w->h.vel_x + s);
        w->h.vel_y = (s16)(w->h.vel_y - s);
    }
    if (w->h.disp_flags & 1)
        w->h.vel_x = (s16)-w->h.vel_x;
    w->f1F4 |= 0x10;
    obj_log("bird_fly", w->prm->type, w->h.x >> 8);
}
static void DecBird_OnHit(ObjBox *a, ObjBox *b) { DecBird_Action_FlyAway((Obj *)a->owner, box_hdr(b)); }
static Obj *DecBird_Create(ObjParam *p, s32 a, s32 b) {
    Obj *w = dec_alloc(Decoration_Destructor);
    if (w == NULL)
        return NULL;
    Decoration_InitFromMap(w, p, a, b);
    rect_set_box2d(&w->coll[0], -0x28, -0x18, 0x28, 0x30);
    rect_set_atkdef(&w->coll[0], 0, (u32)-0x10);
    w->coll[0].flags |= 0xc0;
    w->coll[0].cb = DecBird_OnHit;
    w->coll[0].owner = w;
    w->coll[0].owner_kind = OBJ_OWNER_OBJECT;
    obj_load_bac(w, OBJ_BAC_DEC_BIRD);
    w->spr.order = 0x16;
    set_prio(w, 2);
    w->h.move_flags |= 0x2100;
    if (p->type == 0x37)
        w->h.obj_flags |= 2;
    if (p->type == 0x39)
        w->h.disp_flags ^= 1;
    return w;
}

/* ========================================================== DecGroup35 */
static void DecGroup35Piece_State_Flutter(Obj *w) {
    if (!(RAND() & 0x1f)) {
        w->h.vel_x = (s16)-w->h.vel_x;
        if (!(RAND() & 1))
            w->h.disp_flags ^= 1;
    }
    if (!(RAND() & 0x1f))
        w->h.vel_y = 0;
}
static Obj *leaf_alloc(u16 anim, s32 x, s32 y, s16 vx, s16 vy) {
    Obj *w = obj_alloc(OBJ_PRIO_EFFECT, obj_stageobj_dtor);
    if (w == NULL)
        return NULL;
    w->kind = OBJK_EFFECT;
    obj_load_bac(w, OBJ_BAC_DEC_LEAF);
    obj_set_anim(w, anim);
    if (RAND() & 1)
        w->h.disp_flags |= 1;
    if (RAND() & 1)
        w->h.disp_flags |= 2;
    w->h.x = x;
    w->h.y = y;
    w->h.vel_x = vx;
    w->h.vel_y = vy;
    return w;
}
static void DecGroup35Piece_Create(s32 x, s32 y, s16 vx, s16 vy, u16 n) {
    Obj *w = obj_alloc(OBJ_PRIO_EFFECT, obj_stageobj_dtor);
    if (w == NULL)
        return;
    w->kind = OBJK_EFFECT;
    obj_load_bac(w, OBJ_BAC_DEC_LEAF);
    obj_set_anim(w, (u16)(n + 2));
    w->spr.order = 9;
    if (RAND() & 1)
        w->h.disp_flags |= 1;
    if (RAND() & 1)
        w->h.disp_flags |= 2;
    w->h.x = x;
    w->h.y = y;
    w->h.vel_x = vx;
    w->h.vel_y = vy;
    w->h.gravity = 0x15;
    w->h.fall_max = 0x780;
    w->h.move_flags |= 0x180;
    w->h.obj_flags |= 2;
    w->state = DecGroup35Piece_State_Flutter;
}
static void DecGroup35_CreateFallingDebris(s32 x, s32 y, s16 vx, s16 vy, u16 n) {
    Obj *w = leaf_alloc(n, x, y, vx, vy);
    if (w == NULL)
        return;
    w->spr.order = 0x16;
    set_prio(w, 3);
    w->h.gravity = 0x2a;
    w->h.fall_max = 0xf00;
    w->h.move_flags |= 0x180;
    w->h.obj_flags |= 2;
}
/* DecGroup35_SpawnPieceBurst: the call arguments are evaluated right to left
 * (checked in the matched object code), so each call draws vy, vx, y, x */
static void DecGroup35_SpawnPieceBurst(s32 x, s32 y) {
    static const s16 bvx[4] = { 0xff, 0xff, 0x7f, 0x17f }, bvy[4] = { 0xff, -1, 0xff, -0x81 };
    u16 r_vy, r_vx, r_y, r_x;
    int i;
    for (i = 0; i < 4; i++) {
        r_vy = RAND();
        r_vx = RAND();
        r_y = RAND();
        r_x = RAND();
        DecGroup35Piece_Create(x + (0xfff - (r_x & 0x1ffe)), y + (0x7ff - (r_y & 0xffe)), (s16)(bvx[i] - (r_vx & 0x1fe)),
                               (s16)(bvy[i] - (r_vy & 0x1fe)), (u16)i);
    }
}
static void DecGroup35_State_Main(Obj *p0) {
    if ((p0->h.obj_flags & 2) == 0)
        return;
    p0->wC++;
    if ((u32)p0->wC > (u32)p0->w8) {
        p0->h.obj_flags &= ~2u;
        p0->wC = 0;
    }
}
static void DecGroup35_OnHit(ObjBox *a, ObjBox *b) {
    Obj *w = (Obj *)a->owner;
    DcPlayer *o = box_hdr(b);
    s32 v;
    if (w == NULL || o == NULL)
        return;
    switch (w->prm->type) {
    case 0x35:
        if (!(o->move_flags & 1))
            return;
        v = o->move_x;
        if (v < 0)
            v = -v;
        if (v <= 0x100)
            return;
        if (!(RAND() & 1))
            DecGroup35_SpawnPieceBurst(w->h.x + (0x1fff - (RAND() & 0x3ffe)), w->h.y);
        w->h.obj_flags |= 2;
        obj_log("leaf_burst", w->prm->type, w->h.x >> 8);
        break;
    case 0x36:
        if (!(o->move_flags & 1))
            return;
        v = o->move_x;
        if (v < 0)
            v = -v;
        if (v <= 0x180)
            return;
        if (!(RAND() & 1))
            DecGroup35_CreateFallingDebris(w->h.x + (0x1fff - (RAND() & 0x3ffe)), w->h.y, 0, 0, 0);
        if (!(RAND() & 1))
            DecGroup35_CreateFallingDebris(w->h.x + (0x1fff - (RAND() & 0x3ffe)), w->h.y, 0, 0, 1);
        w->h.obj_flags |= 2;
        obj_log("leaf_burst", w->prm->type, w->h.x >> 8);
        break;
    case 0x51:
    case 0x52:
        if (!(o->move_flags & 0x400001))
            return;
        v = o->move_x;
        if (v < 0)
            v = -v;
        if (v <= 0x80 && o->move_y == 0)
            return;
        /* EffWaterGmk_CreateSplash(o, 0, 0x10) (zone 2 effect: hook) */
        if (g_obj_hooks.effect)
            g_obj_hooks.effect(g_obj_hooks.ctx, o, OBJ_FX_WATER_SPLASH, 0, 0x10 | (w->prm->type == 0x52 ? 0x100 : 0));
        w->h.obj_flags |= 2;
        break;
    }
}
static Obj *DecGroup35_Create(ObjParam *p, s32 a, s32 b) {
    Obj *w = dec_alloc(Decoration_Destructor);
    if (w == NULL)
        return NULL;
    Decoration_InitFromMap(w, p, a, b);
    if (p->type == 0x36)
        rect_set_box2d(&w->coll[0], -0x20, -0x30, 0x20, 0);
    else
        rect_set_box2d(&w->coll[0], -0x20, -0x18, 0x20, 0);
    rect_set_atkdef(&w->coll[0], 0, (u32)-0x10);
    w->coll[0].flags |= 0xc0;
    w->coll[0].cb = DecGroup35_OnHit;
    w->coll[0].owner = w;
    w->coll[0].owner_kind = OBJ_OWNER_OBJECT;
    w->w8 = 0x20;
    if ((u16)(p->type + 0xffaf) <= 1)
        w->w8 = 4;
    w->h.move_flags |= 0x2100;
    w->state = DecGroup35_State_Main;
    return w;
}

/* ========================================================== DecAnimal */
static void DecAnimal_State_Idle(Obj *w);
static void DecAnimal_State_Move(Obj *w);
static void DecAnimal_State_Leave(Obj *w);
static void DecAnimal_Action_Idle(Obj *w) {
    if (w->state != DecAnimal_State_Idle || (RAND() & 1))
        obj_set_anim(w, 0);
    w->h.disp_flags |= 4;
    w->state = DecAnimal_State_Idle;
    w->wC = RAND() & 0x1f;
    switch (w->prm->type) {
    case 3:
    case 8:
    case 13:
        w->h.vel_y = 0;
        break;
    }
}
static void DecAnimal_Action_Move(Obj *w) {
    if (w->state != DecAnimal_State_Move)
        obj_set_anim(w, 2);
    w->h.disp_flags |= 4;
    w->state = DecAnimal_State_Move;
}
static void DecAnimal_Action_Leave(Obj *w) {
    Obj *e;
    DcPlayer *t = (DcPlayer *)w->target;
    if (w->state == DecAnimal_State_Leave)
        return;
    w->state = DecAnimal_State_Leave;
    obj_set_anim(w, 2);
    if (w->prm->type != 5)
        w->h.disp_flags |= 4;
    else
        w->h.move_flags |= 0x2000;
    if (t != NULL && t->x < w->h.x)
        w->h.disp_flags &= ~1;
    else
        w->h.disp_flags |= 1;
    if (w->prm->type == 0xb) {
        e = EffBrakeSmoke_Create(w, 0, -6);
        if (e != NULL) {
            e->h.vel_x = 0;
            e->h.vel_y = 0x60;
            e->wC = 0xfffe;
            e->spr.speed >>= 2; /* unkBE (spr0) and unk106 (spr1) */
        }
    }
    w->wC = 0;
}
static void DecAnimal_UpdateLeaveTimer(Obj *s) {
    if (s->w8 != 0) {
        s->w8--;
        return;
    }
    DecAnimal_Action_Leave(s);
}
/* StageObj_IsTouched: the DS touch screen; the DC has none, so never */
static dcp_bool StageObj_IsTouched(Obj *o, ObjBox *b) {
    (void)o;
    (void)b;
    return DCP_FALSE;
}
static void DecAnimal_CheckTouch(Obj *w) {
    if (!(w->h.move_flags & 1)) {
        if (w->prm->type != 6 && w->prm->type != 0xc)
            return;
        if (w->h.vel_y < -0x80)
            return;
    }
    if (!StageObj_IsTouched(w, &w->coll[0]))
        return;
    if (w->k.ani.touch > 10 || !(RAND() & (0xfff >> w->k.ani.touch))) {
        DecAnimal_Action_Leave(w);
        return;
    }
    obj_set_anim(w, 0);
    w->h.disp_flags |= 4;
    w->h.vel_y = -0x200;
    w->h.move_flags |= 0x10;
    switch (w->prm->type) {
    case 0xc:
        if (RAND() & 1)
            w->h.disp_flags &= ~1;
        else
            w->h.disp_flags |= 1;
        w->h.vel_x = 0x220;
        if (w->h.disp_flags & 1)
            w->h.vel_x = (s16)-w->h.vel_x;
        /* fall through */
    case 6:
    case 10:
        obj_set_anim(w, 1);
        w->h.disp_flags |= 4;
        break;
    }
    if (w->k.ani.touch <= 4 && (RAND() & (0x1f >> w->k.ani.touch))) {
    } else {
        if (w->k.ani.touch <= 4)
            w->k.ani.touch = 5;
        switch (w->prm->type) {
        default:
            w->h.vel_y >>= 1;
            break;
        case 2:
        case 4:
        case 0xe:
            w->h.vel_y = 0;
            w->h.hitstop = 0x22;
            break;
        case 10:
        case 0x87:
            break;
        }
        obj_set_anim(w, 1);
        w->h.disp_flags |= 4;
    }
    w->k.ani.touch++;
}
static void DecAnimal_ChooseNextAction(Obj *w) {
    s32 dx;
    s16 r, a, b, d, c = 0;
    DcPlayer *t = (DcPlayer *)w->target;
    r = (s16)(RAND() & 0xf);
    dx = (t ? t->x : w->h.x) - w->h.x;
    if (dx < 0)
        dx = -dx;
    b = 2;
    d = 2;
    a = 12;
    if (w->f1F4 & 0x40) {
        if (!(RAND() & 4)) {
            switch (RAND() & 3) {
            case 0:
                w->f1F4 &= ~0x10u;
                w->f1F4 &= ~0x20u;
                break;
            case 1:
                w->f1F4 |= 0x10;
                w->f1F4 &= ~0x20u;
                break;
            case 2:
                w->f1F4 &= ~0x10u;
                w->f1F4 |= 0x20;
                break;
            }
        }
    }
    if (w->f1F4 & 0x20) {
        if (dx > 0x4000) {
            a = 1;
            b = 10;
            d = 5;
        }
        if (t != NULL && t->x < w->h.x)
            w->h.disp_flags |= 1;
        else
            w->h.disp_flags &= ~1;
    } else {
        if (RAND() & 1)
            w->h.disp_flags |= 1;
        else
            w->h.disp_flags &= ~1;
    }
    r -= a;
    if (r < 0) {
        DecAnimal_Action_Idle(w);
        return;
    }
    r -= b;
    if (r < 0) {
        w->wC = (RAND() & 0xf) + 8;
        DecAnimal_Action_Move(w);
        return;
    }
    r -= c;
    if (r < 0) {
        w->wC = (RAND() & 0xf) + 0x1e;
        DecAnimal_Action_Move(w);
        return;
    }
    r -= d;
    if (r < 0) {
        w->wC = (RAND() & 0xf) + 0x10;
        DecAnimal_Action_Move(w);
        return;
    }
}
static void DecAnimal_UpdateMove(Obj *w) {
    s16 acc = 0x24, max = 0x200;
    s32 v;
    switch (w->prm->type) {
    case 10:
        max = 0x280;
        acc = 0x30;
        break;
    case 4:
        max = 0x2a0;
        acc = 0x2a;
        break;
    case 14:
        max = 0x180;
        break;
    case 3:
    case 8:
    case 13:
        acc = 0x80;
        if (w->h.move_flags & 1) {
            if (w->prm->type == 8) {
                w->h.vel_y = -0x300;
                max = 0x240;
            } else if (w->prm->type == 13) {
                w->h.vel_y = -0x180;
                max = 0x280;
            } else {
                w->h.vel_y = -0x240;
            }
            w->h.move_flags |= 0x10;
        }
        break;
    case 12:
        max = 0x400;
        break;
    case 7:
        if (w->h.disp_flags & 0x10) {
            w->h.vel_x = 0;
            if (w->h.hitstop == 0 && !(W.frame & 0x1f))
                w->h.disp_flags &= ~0x10;
        } else if (w->h.angle != 0) {
            w->h.angle = dcp_angle_toward8(w->h.angle, 0, 4);
            w->h.vel_x = 0;
        } else {
            w->h.move_flags &= ~0x2100u;
            if (!(W.frame & 0xf) && (w->h.move_flags & 1) && !(RAND() & 0xf)) {
                w->h.angle = 0x40;
                if (w->h.disp_flags & 1)
                    w->h.angle += 0x80;
                w->h.vel_x = 0;
                w->h.disp_flags |= 0x10;
                w->h.move_flags |= 0x2100;
                w->h.hitstop = 0x40;
            }
        }
        v = (s8)w->h.angle;
        if (v < 0)
            v = -v;
        w->h.shake_y = (s8)-(v >> 4);
        break;
    }
    if (w->h.move_flags & 4)
        w->h.disp_flags ^= 1;
    if (w->state != DecAnimal_State_Leave)
        max = (s16)(max - (max >> 2));
    if (w->h.disp_flags & 1)
        w->h.vel_x = (s16)dcp_spd_up(w->h.vel_x, -acc, max);
    else
        w->h.vel_x = (s16)dcp_spd_up(w->h.vel_x, acc, max);
}
static void DecAnimal_State_Idle(Obj *w) {
    if (w->h.move_flags & 1) {
        if ((w->h.move_flags & 0x10) && w->prm->type == 10) {
            obj_set_anim(w, 0);
            w->h.disp_flags |= 4;
        }
        w->h.move_flags &= ~0x10u;
        if (w->f1F4 & 0x30) {
            if (w->wC == 0) {
                if (!(W.frame & 7)) {
                    DecAnimal_ChooseNextAction(w);
                    return;
                }
            } else {
                w->wC--;
            }
        }
    }
    w->h.vel_x = (s16)dcp_spd_down(w->h.vel_x, 0x28);
    DecAnimal_CheckTouch(w);
    DecAnimal_UpdateLeaveTimer(w);
}
static void DecAnimal_State_Move(Obj *w) {
    DecAnimal_UpdateMove(w);
    if (w->wC == 0) {
        if ((w->h.move_flags & 1) && !(w->h.move_flags & 0x2000)) {
            DecAnimal_ChooseNextAction(w);
            return;
        }
    } else {
        w->wC--;
    }
    DecAnimal_CheckTouch(w);
    DecAnimal_UpdateLeaveTimer(w);
}
static void DecAnimal_State_Leave(Obj *w) {
    switch (w->prm->type) {
    case 5:
        if (w->h.disp_flags & 8)
            w->h.obj_flags |= 4;
        break;
    case 6:
    case 12:
        w->h.move_flags &= ~0x80u;
        w->h.move_flags |= 0x110;
        w->h.vel_y = (s16)dcp_spd_up(w->h.vel_y, -16, 0x200);
        break;
    }
    DecAnimal_UpdateMove(w);
}
static void DecAnimal_Destructor(Obj *p) {
    g_obj.dec_animal_count--;
    Decoration_Destructor(p);
}
static Obj *DecAnimal_Create(ObjParam *p, s32 a, s32 b) {
    Obj *w = dec_alloc(DecAnimal_Destructor);
    s32 n;
    ObjParam *q;
    if (w == NULL)
        return NULL;
    g_obj.dec_animal_count++;
    Decoration_InitFromMap(w, p, a, b);
    n = w->prm->type;
    if (n == 0x87)
        n = 0xf;
    obj_load_bac(w, OBJ_BAC_ANIMAL01 + n); /* sprintf("/act/ac_dec_animal%02d.bac", n + 1) */
    w->spr.order = 0x12;
    w->h.box_l = -4; /* StageObj_SetMapHitbox(w, -4, -8, 4, -1) */
    w->h.box_t = -8;
    w->h.box_r = 4;
    w->h.box_b = -1;
    rect_set_box2d(&w->coll[0], -0xc, -0x14, 0xc, 6);
    w->h.move_flags |= 0x80;
    w->h.obj_flags |= 2;
    w->h.disp_flags |= 0x100;
    if (obj_gravity_flipped(w->h.x, w->h.y))
        w->h.grav_rot = 0x80;
    w->target = g_obj.n_players ? g_obj.players[0].p : NULL;
    switch (w->prm->type) {
    case 0xa:
        w->f1F4 |= 0x40;
        if (RAND() & 1)
            w->f1F4 |= 0x10;
        break;
    case 0x87:
        if (RAND() & 1)
            w->f1F4 |= 0x10;
        else
            w->f1F4 |= 0x20;
        break;
    case 2:
    case 0xe:
        if (!(RAND() & 7))
            w->f1F4 |= 0x10;
        else if (!(RAND() & 7))
            w->f1F4 |= 0x20;
        break;
    case 4:
    case 8:
    case 0xd:
        if (RAND() & 1)
            w->f1F4 |= 0x10;
        else if (RAND() & 1)
            w->f1F4 |= 0x20;
        break;
    case 7:
        if (RAND() & 7)
            w->f1F4 |= 0x20;
        else
            w->f1F4 |= 0x10;
        break;
    case 9:
        if (RAND() & 3)
            w->f1F4 |= 0x20;
        break;
    case 5:
        break;
    default:
        if (RAND() & 1) {
            if (RAND() & 1)
                w->f1F4 |= 0x10;
            else
                w->f1F4 |= 0x20;
        }
        if (!(RAND() & 7))
            w->f1F4 |= 0x40;
        break;
    }
    q = w->prm;
    if (q != NULL && q->x0 == 0xff && q->y0 == 0xff)
        w->h.vel_y = -0x400;
    w->w8 = (RAND() & 0xff) + 0x168;
    if (g_obj.dec_animal_count > 5)
        w->w8 >>= 1;
    if (g_obj.dec_animal_count > 6)
        w->w8 >>= 1;
    if (g_obj.dec_animal_count > 7)
        w->w8 = 1;
    DecAnimal_Action_Idle(w);
    return w;
}

/* ------------------------------------------------ gDecorationCreateTable */
ObjDecCreateFn obj_dec_create_fn(u16 t) {
    if (t <= 0x0e || t == 0x87)
        return DecAnimal_Create;
    if ((t >= 0x0f && t <= 0x30) || (t >= 0x3a && t <= 0x40))
        return DecGroup0F_Create;
    if (t >= 0x31 && t <= 0x34)
        return DecVine_Create;
    if (t == 0x35 || t == 0x36 || t == 0x51 || t == 0x52)
        return DecGroup35_Create;
    if (t >= 0x37 && t <= 0x39)
        return DecBird_Create;
    /* objdata v3: Water Palace */
    if ((t >= 0x41 && t <= 0x44) || t == 0x4f || t == 0x50 || t == 0x90 || (t >= 0x9d && t <= 0xae) || t == 0xf1)
        return DecGroup41_Create;
    if (t == 0x4d || t == 0x4e || t == 0xcf)
        return DecGroup4D_Create;
    /* objdata v3: Mirage Road */
    if ((t >= 0x45 && t <= 0x4c) || (t >= 0x53 && t <= 0x66))
        return DecGroup45_Create;
    /* objdata v4: Dead Line */
    if (t >= 0x88 && t <= 0x8f)
        return DecArrow_Create;
    /* objdata v4: Huge Crisis */
    if ((t >= 0x67 && t <= 0x86) || (t >= 0x91 && t <= 0x94))
        return DecGroup67_Create;
    /* objdata v4: DecGroup95 (zones 5..7) */
    if ((t >= 0x95 && t <= 0x9c) || (t >= 0xc5 && t <= 0xce) || (t >= 0xd0 && t <= 0xd7) || (t >= 0xdc && t <= 0xe1) ||
        (t >= 0xe5 && t <= 0xe7))
        return DecGroup95_Create;
    /* objdata v4: Night Carnival */
    if ((t >= 0xaf && t <= 0xc4) || (t >= 0xd8 && t <= 0xdb) || (t >= 0xe2 && t <= 0xe4) || (t >= 0xe8 && t <= 0xef))
        return DecGroupAF_Create;
    return NULL; /* not translated (OBJECTS.md) */
}

static int dec_type_slots(u16 t, u8 *o) {
    if (t <= 0x0e || t == 0x87) {
        o[0] = (u8)(OBJ_BAC_ANIMAL01 + (t == 0x87 ? 0xf : t));
        return 1;
    }
    if (t >= 0x0f && t <= 0x18) { o[0] = OBJ_BAC_DEC_FLOWER; return 1; }
    if ((t >= 0x19 && t <= 0x34) || (t >= 0x3b && t <= 0x40)) { o[0] = OBJ_BAC_DEC_VINE; return 1; }
    if (t == 0x3a) { o[0] = OBJ_BAC_DEC_BIRD_NEST; return 1; }
    if (t == 0x35 || t == 0x36) { o[0] = OBJ_BAC_DEC_LEAF; return 1; }
    if (t >= 0x37 && t <= 0x39) { o[0] = OBJ_BAC_DEC_BIRD; return 1; }
    if (t == 0x41) { o[0] = OBJ_BAC_DEC_FOUNTAIN; return 1; }
    if ((t >= 0x42 && t <= 0x44) || t == 0xf1) { o[0] = OBJ_BAC_WATER_FALL; return 1; }
    if (t == 0x4d || t == 0x4e) { o[0] = OBJ_BAC_GMK_WHEEL; return 1; }
    if (t == 0x4f || t == 0x50 || t == 0x90) { o[0] = OBJ_BAC_EFF_WATER_GMK; return 1; }
    if (t >= 0x9d && t <= 0xa0) { o[0] = OBJ_BAC_DEC_ALGAE; return 1; }
    if (t >= 0xa1 && t <= 0xa4) { o[0] = OBJ_BAC_DEC_CORAL; return 1; }
    if (t >= 0xa5 && t <= 0xae) { o[0] = OBJ_BAC_DEC_FISH; return 1; }
    if (t == 0x54 || t == 0x55 || t == 0x5f || t == 0x60) { o[0] = OBJ_BAC_DEC_STATUE; return 1; }
    if (t >= 0x64 && t <= 0x66) { o[0] = OBJ_BAC_GMK_HAND; return 1; }
    if (t == 0x56 || t == 0x57) { o[0] = OBJ_BAC_DEC_RELIEF; return 1; }
    if (t == 0x58 || t == 0x59) { o[0] = OBJ_BAC_DEC_BED; return 1; }
    if (t >= 0x5a && t <= 0x5d) { o[0] = OBJ_BAC_DEC_CANOPS; return 1; }
    if (t == 0x5e) { o[0] = OBJ_BAC_DEC_BOX; return 1; }
    if (t == 0x61) { o[0] = OBJ_BAC_DEC_TORCH; return 1; }
    if (t == 0x62 || t == 0x63) { o[0] = OBJ_BAC_DEC_DECORA; return 1; }
    if (t >= 0x45 && t <= 0x49) { o[0] = OBJ_BAC_GMK_RAIL; return 1; }
    if (t >= 0x4a && t <= 0x4c) { o[0] = OBJ_BAC_DEC_SAND; return 1; }
    if (t == 0x53) { o[0] = OBJ_BAC_DEC_TABLE_WATER; return 1; }
    /* objdata v4: Night Carnival (DecGroupAF) */
    if (t >= 0xaf && t <= 0xc2) { o[0] = OBJ_BAC_DEC_NEON_FLOOR; return 1; }
    if (t == 0xc3) { o[0] = OBJ_BAC_DEC_NEON_BALL; return 1; }
    if (t == 0xc4) { o[0] = OBJ_BAC_DEC_NEON_PILLAR; return 1; }
    if (t == 0xd8) { o[0] = OBJ_BAC_DEC_LIGHT4; return 1; }
    if ((t >= 0xd9 && t <= 0xdb) || t == 0xe2) { o[0] = OBJ_BAC_DEC_SWING_SKY_HANG; return 1; }
    if (t == 0xe3 || t == 0xe4) { o[0] = OBJ_BAC_DEC_NEON_LAND; return 1; }
    if (t == 0xe8) { o[0] = OBJ_BAC_DEC_NEON_BALL2; return 1; }
    if (t == 0xe9 || t == 0xea) { o[0] = OBJ_BAC_DEC_NEON_STAR; return 1; }
    if (t >= 0xeb && t <= 0xef) { o[0] = OBJ_BAC_DEC_SONIC_NEON; return 1; }
    if (t == 0xcf) { o[0] = OBJ_BAC_CRANE_SLIDE; return 1; }
    /* objdata v4: DecGroup67 / DecGroup95 */
    if (t >= 0x67 && t <= 0x86) { o[0] = OBJ_BAC_DEC_PIPE; return 1; }
    if (t >= 0x88 && t <= 0x8f) { o[0] = OBJ_BAC_DEC_ARROW; return 1; }
    if (t >= 0x91 && t <= 0x94) { o[0] = OBJ_BAC_DEC_PLANE; return 1; }
    if ((t >= 0x96 && t <= 0x9b) || (t >= 0xdc && t <= 0xe1)) { o[0] = OBJ_BAC_DEC_CLOUD; return 1; }
    if (t == 0x95) { o[0] = OBJ_BAC_DEC_INNER_PILLAR; return 1; }
    if (t == 0x9c) { o[0] = OBJ_BAC_DEC_PROPELLER; return 1; }
    if (t == 0xc5) { o[0] = OBJ_BAC_DEC_JET; return 1; }
    if ((t >= 0xc6 && t <= 0xce) || t == 0xd6 || t == 0xd7) { o[0] = OBJ_BAC_GMK_RAIL6; return 1; }
    if (t >= 0xd0 && t <= 0xd5) { o[0] = OBJ_BAC_DEC_LIGHT; return 1; }
    if (t >= 0xe5 && t <= 0xe7) { o[0] = OBJ_BAC_DEC_LASER; return 1; }
    return 0;
}
void dec_preload(const rlvl_header_t *h) {
    u8 seen[0x100], sl[4];
    u32 i;
    int k, n;
    if (g_obj.lv_dec == NULL)
        return;
    memset(seen, 0, sizeof(seen));
    for (i = 0; i < h->num_decor; i++) {
        u16 t = g_obj.lv_dec[i].type;
        if (t >= 0x100 || seen[t])
            continue;
        seen[t] = 1;
        n = dec_type_slots(t, sl);
        for (k = 0; k < n; k++)
            obj_res_ensure(sl[k]);
    }
    obj_res_ensure(OBJ_BAC_EFF_BRAKE);
}

/* StageEvent_SpawnDecorationsInBlock */
void spawn_decorations_in_block(u16 x, u16 y, const void *rcv, const void *ov, u32 f) {
    const DRect *rc = (const DRect *)rcv, *o = (const DRect *)ov;
    const rlvl_header_t *h = g_obj.hdr;
    u32 ri = (u32)(u16)(y * (((g_obj.map_w - 1) >> 8) + 1) + x), i;
    ObjParam *e;
    s32 bx = x << 8, by = y << 8;
    u32 px, py;
    u16 ml, mt, mr, mb;
    const u8 *tbl = g_obj.data.dec_margin;
    ObjDecCreateFn fn;
    if (ri >= (u32)h->region_cols * h->region_rows)
        return;
    for (i = g_obj.lv_dec_region[ri]; i < g_obj.lv_dec_region[ri + 1]; i++) {
        e = &g_obj.dprm[i];
        if (e->x0 == 0xff)
            continue;
        if (e->type >= 0xf2)
            continue; /* (the original reads tbl[] before this test) */
        px = (u32)e->x0 + bx;
        py = (u32)e->y0 + by;
        ml = (f & 0x10) ? 0 : tbl[e->type];
        mt = (f & 0x20) ? 0 : tbl[e->type];
        mr = (f & 0x40) ? 0 : tbl[e->type];
        mb = (f & 0x80) ? 0 : tbl[e->type];
        if (f & 1) {
            if (px < rc->l + ml || px > rc->r - mr || py < rc->t + mt || py > rc->b - mb)
                continue;
        }
        if (f & 4) {
            if (px > o->l && px < o->r && py > o->t && py < o->b)
                continue;
        }
        fn = obj_dec_create_fn(e->type);
        if (fn == NULL)
            continue;
        fn(e, (bx + e->x0) << 8, (by + e->y0) << 8);
    }
}
