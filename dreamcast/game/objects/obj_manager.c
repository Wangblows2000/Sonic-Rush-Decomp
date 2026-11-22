/*
 * Object manager: faithful translation of
 *   src/game/Stage/StageEvent_*.c           (region spawning in view)
 *   src/game/Stage/ObjectSystem/MapObject_* (spawn entries, init, destructor,
 *                                            default In / Collide)
 *   src/game/Stage/ObjectSystem/StageObj_Main, _ViewOutCheck, _PauseCheck
 *   the task list order of Task_Create (sorted by id, FIFO within an id)
 * plus the public API (objects.h).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "obj_internal.h"

ObjGlobals g_obj;
ObjHooks g_obj_hooks;
ObjStage g_obj_stage;

void rect_forget(ObjBox *b);
void colmap_forget(ObjColMap *e);

/* ------------------------------------------------------- helpers */
void obj_sfx(s32 id) {
    if (g_obj_hooks.sfx)
        g_obj_hooks.sfx(g_obj_hooks.ctx, id);
}
void obj_log(const char *ev, s32 a, s32 b) {
    if (g_obj_hooks.log)
        g_obj_hooks.log(g_obj_hooks.ctx, ev, a, b);
}
dcp_bool obj_gravity_flipped(s32 x, s32 y) {
    return g_dcp_hooks.gravity_flipped ? g_dcp_hooks.gravity_flipped(g_dcp_hooks.ctx, x, y) : DCP_FALSE;
}
dcp_bool obj_pause_check(u32 v) { return (W.objmgr & 1) && !(v & 0x20); }

/* ------------------------------------------------------ task list */
Obj *obj_alloc(u16 prio, ObjFn dtor) {
    Obj *o = NULL, **pp;
    int i;
    for (i = 0; i < OBJ_MAX; i++)
        if (!g_obj.pool[i].used) {
            o = &g_obj.pool[i];
            break;
        }
    if (o == NULL)
        return NULL; /* task == OS_GetArenaLo(0) */
    memset(o, 0, sizeof(*o));
    o->used = 1;
    o->prio = prio;
    o->dtor = dtor;
    /* Task_Create: insert after every task with id <= prio */
    pp = &g_obj.head;
    while (*pp && (*pp)->prio <= prio)
        pp = &(*pp)->next;
    o->next = *pp;
    *pp = o;
    g_obj.stats.live++;
    return o;
}

void obj_free_player_refs(Obj *o) {
    int i;
    for (i = 0; i < g_obj.n_players; i++) {
        DcPlayer *p = g_obj.players[i].p;
        if (p->ride == &o->ref)
            p->ride = NULL;
        if (p->touch == &o->ref)
            p->touch = NULL;
        if (p->homing == &o->ref)
            p->homing = NULL;
    }
}

/* Task_Destroy: destructor, then unlink (the caller keeps iterating via next) */
static void obj_destroy(Obj *o) {
    Obj **pp;
    if (o->dtor)
        o->dtor(o);
    obj_free_player_refs(o);
    rect_forget(&o->coll[0]);
    rect_forget(&o->coll[1]);
    rect_forget(&o->coll[2]);
    rect_forget(&o->detect);
    colmap_forget(&o->cmap);
    for (pp = &g_obj.head; *pp; pp = &(*pp)->next)
        if (*pp == o) {
            *pp = o->next;
            break;
        }
    o->used = 0;
    g_obj.stats.live--;
}

/* StageObj_Destructor (sprite VRAM release: nothing to do natively) */
void obj_stageobj_dtor(Obj *o) { (void)o; }

/* StageObj_Load2dBAC: the sprite pair (spr1 = other DS screen is not kept) */
void obj_load_bac(Obj *o, int bac) {
    spr_init(&o->spr, &g_obj.res[bac], 0, 0x40000);
    o->spr.order = 0;
    o->has_spr = 1;
}

/* ---------------------------------------- MapObject_Alloc/FreeSpawnEntry */
static u8 spawn_entry_alloc(void) {
    u8 i;
    for (i = 0; i < OBJ_SPAWN_ENTRIES; i++) {
        if (i < 0x20) {
            if (!(g_obj.spawn_mask_lo & (1u << i))) {
                g_obj.spawn_mask_lo |= 1u << i;
                return i;
            }
        } else if (!(g_obj.spawn_mask_hi & (1u << (i - 0x20)))) {
            g_obj.spawn_mask_hi |= 1u << (i - 0x20);
            return i;
        }
    }
    return i;
}
static void spawn_entry_free(ObjParam *p) {
    u8 i;
    for (i = 0; i < OBJ_SPAWN_ENTRIES; i++)
        if (&g_obj.spawn_entries[i] == p) {
            if (i < 32)
                g_obj.spawn_mask_lo &= ~(1u << i);
            else
                g_obj.spawn_mask_hi &= ~(1u << (i - 32));
            return;
        }
}

/* MapObject_Spawn */
Obj *obj_mapobject_spawn(s32 type, s32 x, s32 y, s16 d, s8 e, s8 f, u8 g, u8 h) {
    Obj *o = NULL;
    s16 idx = spawn_entry_alloc();
    ObjParam *ent;
    ObjCreateFn fn;
    if (idx < OBJ_SPAWN_ENTRIES) {
        ent = &g_obj.spawn_entries[idx];
        memset(ent, 0, sizeof(*ent));
        ent->x0 = 0xff;
        ent->y0 = 0xff;
        ent->type = (u16)type;
        ent->flags = (u16)d;
        ent->p6 = e;
        ent->p7 = f;
        ent->p8 = g;
        ent->p9 = h;
        ent->ax = (u16)(x >> 8);
        ent->ay = (u16)(y >> 8);
        fn = obj_create_fn((u16)type);
        o = fn ? fn(ent, x, y) : NULL;
        if (o == NULL)
            spawn_entry_free(ent);
    }
    return o;
}

/* ------------------------------------------------- MapObject_In_Default */
static void mapobject_in_default(Obj *w) {
    if (w->target != NULL && (((DcPlayer *)w->target)->obj_flags & 4))
        w->target = NULL;
    /* MapObject_NetReceiveHits: no network play */
    if (obj_pause_check(w->h.obj_flags))
        return;
    if (w->flash == 0)
        return;
    w->flash--;
    if (w->flash & 2)
        w->h.disp_flags |= 0x20;
    else
        w->h.disp_flags &= ~0x20;
    w->coll[0].def |= 5;  /* @0x158 */
    w->coll[1].atk &= ~7u; /* @0x180 */
    if (w->flash != 0)
        return;
    w->h.disp_flags &= ~0x20;
    w->coll[0].def = 0xa;
    w->coll[1].atk |= 3;
}

/* MapObject_Collide_Default */
void obj_mapobject_collide_default(Obj *o) {
    if (o->h.obj_flags & 0xc)
        return;
    if (o->coll[0].owner != NULL)
        obj_handle_collider(&o->h, o, OBJ_OWNER_OBJECT, &o->coll[0], o->has_spr);
    if (o->coll[1].owner != NULL)
        obj_handle_collider(&o->h, o, OBJ_OWNER_OBJECT, &o->coll[1], o->has_spr);
    if (o->coll[2].owner != NULL)
        obj_handle_collider(&o->h, o, OBJ_OWNER_OBJECT, &o->coll[2], o->has_spr);
    if (!o->has_cmap)
        return;
    if (obj_view_out(o->h.x, o->h.y, 0x20, 0, 0, 0, 0))
        return;
    colmap_register(&o->cmap);
}

/* ------------------------------------------------------ MapObject_Init */
void obj_mapobject_init(Obj *s, ObjParam *prm, s32 x, s32 y) {
    int i;
    s->fn_in = mapobject_in_default;
    s->h.obj_flags |= 0x40;
    s->spr_cb = obj_sprite_cb_default;
    s->fn_collide = obj_mapobject_collide_default;
    s->home_x = x >> 8;
    s->home_y = y >> 8;
    s->h.x = x;
    s->h.y = y;
    s->h.type = prm->type < 0x40 ? 2 : 3;
    s->h.gravity = 0x2a;
    s->h.fall_max = 0xf00;
    s->h.obj_flags |= 1;
    s->h.disp_flags |= 1;
    s->h.move_flags |= 0x200;
    for (i = 0; i < 3; i++)
        s->coll[i].mask = 0x12;
    s->coll[0].cb = mapobject_on_hit_default;
    rect_set_atkdef(&s->coll[0], 9, 0xa);
    rect_set_atkdef(&s->coll[1], 0x13, 0x13);
    rect_set_atkdef(&s->coll[2], 0x13, 0x13);
    for (i = 0; i < 3; i++)
        s->coll[i].flags &= ~4;
    if (prm != NULL) {
        s->prm = prm;
        s->saved_x = prm->x0;
        prm->x0 = 0xff;
    }
    s->out_ofs = (s16)(0x108 - g_obj.data.margin[prm->type]);
    g_obj.stats.spawned++;
}

/* ------------------------------------------------ MapObject_Destructor */
void obj_mapobject_dtor(Obj *w) {
    /* sound handle release: nothing to do natively */
    if (w->prm != NULL && w->prm->x0 == 0xff && w->prm->y0 == 0xff)
        spawn_entry_free(w->prm);
    else if (!(w->f1F4 & 0x10) && w->prm != NULL)
        w->prm->x0 = w->saved_x;
    if (w->f1F4 & 0x10)
        g_obj.stats.destroyed++;
    else
        g_obj.stats.despawned++;
    obj_stageobj_dtor(w);
}

/* Enemy_DisableCollidersSetAnim */
void obj_disable_colliders_set_anim(Obj *o, u16 anim) {
    o->coll[0].flags &= ~4;
    o->coll[1].flags &= ~4;
    o->coll[2].flags &= ~4;
    obj_set_anim(o, anim);
}

/* -------------------------------------------------- StageObj_ViewOutCheck */
dcp_bool obj_view_out(s32 x, s32 y, s16 offset, s16 sl, s16 st, s16 sr, s16 sb) {
    s32 tl, tt, tr, tb;
    s16 sw = 0x100, sh = 0xc0; /* data_0207cf80 zoom = 0x100 */
    int i;
    if (offset == 0)
        offset = 0x40;
    if ((W.objmgr & 4) == 0)
        return DCP_FALSE;
    /* (gObjManagerFlags & 0x200: combined dual-screen box; not used on DC) */
    for (i = 0; i < g_obj.n_views; i++) {
        tl = (g_obj.view[i].x >> 8) - offset;
        tt = (g_obj.view[i].y >> 8) - offset;
        tr = sw + (offset << 1);
        tb = sh + (offset << 1);
        tl += sl;
        tt += st;
        tb += -st + sb;
        tr += -sl + sr;
        if (tl <= (x >> 8) && tl + tr >= (x >> 8) && tt <= (y >> 8) && tt + tb >= (y >> 8))
            return DCP_FALSE;
    }
    return DCP_TRUE;
}

/* ------------------------------------------------------ StageObj_Main */
static void obj_update_ref(Obj *o) {
    o->ref.x = o->h.x;
    o->ref.y = o->h.y;
    o->ref.z = o->h.z;
    o->ref.px = o->h.px;
    o->ref.py = o->h.py;
    o->ref.pz = o->h.pz;
    o->ref.vx = o->h.move_x;
    o->ref.vy = o->h.move_y;
    o->ref.vz = o->h.move_z;
    o->ref.max = o->h.ride_max;
    o->ref.obj_flags = o->h.obj_flags;
    o->ref.move_flags = o->h.move_flags;
}

static void obj_main(Obj *o) {
    u16 saved = 0;
    int restore = 0;
    if (o->h.obj_flags & 4) {
        obj_destroy(o);
        return;
    }
    if (o->h.obj_flags & 8)
        o->h.obj_flags |= 4;
    if ((o->h.obj_flags & 0x10) == 0) {
        if (obj_view_out(o->h.x, o->h.y, o->out_ofs, o->out_l, o->out_t, o->out_r, o->out_b))
            o->h.obj_flags |= 4;
    }
    if (o->parent != NULL && (!o->parent->used || (o->parent->h.obj_flags & 4))) {
        if ((o->h.obj_flags & 0x200) == 0)
            o->h.obj_flags |= 4;
        o->parent = NULL;
    }
    dcp_handle_ride(&o->h);
    if ((!obj_pause_check(o->h.obj_flags) || (o->h.obj_flags & 0x40)) && o->fn_in != NULL)
        o->fn_in(o);
    if (!obj_pause_check(o->h.obj_flags)) {
        if (o->h.hitstop != 0) {
            if (o->h.hitstop & 0xf)
                o->h.hitstop--;
            if (o->h.hitstop & 0xf0)
                o->h.hitstop -= 0x10;
        }
        if (!obj_pause_check(o->h.obj_flags)) {
            if ((o->h.hitstop & 0xf) == 0 && (o->h.obj_flags & 0x80) == 0 && o->state != NULL)
                o->state(o);
            if (o->used && (o->h.move_flags & 0x2000) == 0)
                dcp_obj_move(&o->h);
            if ((W.objmgr & 8) && (o->h.move_flags & 0x100) == 0) {
                o->h.ride = NULL;
                o->h.touch = NULL;
                dcp_obj_collision(&o->h);
            }
        }
    }
    if ((o->h.hitstop & 0xf) || obj_pause_check(o->h.obj_flags)) {
        saved = o->h.disp_flags;
        o->h.disp_flags = saved | 0x10;
        restore = 1;
    }
    if (o->h.hitstop & 0xf0) {
        o->h.shake_x += g_obj.data.shake[(o->h.hitstop & 0xf0) >> 5];
        o->h.shake_y += g_obj.data.shake[((o->h.hitstop & 0xf0) >> 5) + 1];
    }
    if ((o->h.obj_flags & 4) == 0) {
        if (o->fn_draw != NULL)
            o->fn_draw(o);
        if (o->fn_draw == NULL || (o->h.disp_flags & 0x40)) {
            if (o->has_spr)
                obj_draw2d(o);
        }
    }
    if (restore)
        o->h.disp_flags = saved;
    obj_update_ref(o);
    if (obj_pause_check(o->h.obj_flags))
        return;
    if ((o->h.hitstop & 0xf) == 0 && (W.objmgr & 0x10) && o->fn_collide != NULL)
        o->fn_collide(o);
    o->h.push_x = 0;
    o->h.push_y = 0;
    o->h.push_z = 0;
    o->h.shake_x = 0;
    o->h.shake_y = 0;
    if (o->fn_last != NULL)
        o->fn_last(o);
}

/* ------------------------------------------------ gMapObjectCreateTable */
ObjCreateFn obj_create_fn(u16 t) {
    if (t <= 0x01 || t == 0x03 || (t >= 0x07 && t <= 0x0a) || (t >= 0x0d && t <= 0x0f) || t == 0x14 || t == 0x15)
        return pawn_create;          /* EnemyPawnGroup_Create */
    if (t >= 0x2b && t <= 0x31)
        return itembox_create;       /* ItemBox_Create */
    if ((t >= 0x43 && t <= 0x4c) || t == 0xe0)
        return spring_create;        /* Spring_Create */
    if ((t >= 0x4d && t <= 0x50) || t == 0x5d || t == 0x5e)
        return needle_create;        /* Needle_Create */
    if (t == 0x5c || (t >= 0x5f && t <= 0x62) || (t >= 0x98 && t <= 0x9b))
        return dashp_create;         /* DashP_Create */
    if ((t >= 0x63 && t <= 0x66) || (t >= 0x6a && t <= 0x6d))
        return dashc_create_by_type; /* DashC_CreateByType */
    if (t == 0x6e)
        return marker_create;        /* Marker_Create */
    if (t == 0x71)
        return goalring_create;      /* GoalRing_Create */
    return NULL;                     /* not translated (OBJECTS.md) */
}

/* ------------------------------------------------- StageEvent spawners */
typedef struct { u32 l, t, r, b; } URect;

/* StageEvent_SpawnMapObjectsInBlock */
static void spawn_objects_in_block(u16 x, u16 y, const URect *rc, const URect *o, u32 f) {
    const rlvl_header_t *h = g_obj.hdr;
    u32 ri = (u32)(u16)(y * (((g_obj.map_w - 1) >> 8) + 1) + x), i, i0, i1;
    s32 bx = x << 8, by = y << 8, d;
    u32 px, py, ml, mt, mr, mb;
    const u8 *tbl = g_obj.data.margin;
    ObjParam *e;
    ObjCreateFn fn;
    if (ri >= (u32)h->region_cols * h->region_rows)
        return;
    i0 = g_obj.lv_obj_region[ri];
    i1 = g_obj.lv_obj_region[ri + 1];
    for (i = i0; i < i1; i++) {
        e = &g_obj.prm[i];
        if ((f & 8) && !(e->flags & 0x4000))
            continue;
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
            if (px < rc->l + (u16)ml || px > rc->r - (u16)mr || py < rc->t + (u16)mt || py > rc->b - (u16)mb)
                continue;
        }
        if (f & 4) {
            d = (s32)(0x100 - tbl[e->type]) / 3; /* data_ov000_0236aa78 = 0x100 */
            if (px > o->l - d && px < o->r + d && py > o->t - d && py < o->b + d)
                continue;
        }
        fn = obj_create_fn(e->type);
        if (fn == NULL)
            continue;
        fn(e, (bx + e->x0) << 8, (by + e->y0) << 8);
    }
}

/* StageEvent_SpawnRingsInBlock */
static void spawn_rings_in_block(u16 x, u16 y, const URect *rc) {
    const rlvl_header_t *h = g_obj.hdr;
    u32 ri = (u32)(u16)(y * (((g_obj.map_w - 1) >> 8) + 1) + x), i;
    s32 bx = x << 8, by = y << 8;
    u32 px, py;
    if (ri >= (u32)h->region_cols * h->region_rows)
        return;
    for (i = g_obj.lv_ring_region[ri]; i < g_obj.lv_ring_region[ri + 1]; i++) {
        u8 *e = &g_obj.ring_state[i];
        if (e[0] != 0xff) {
            u8 ly = (u8)(g_obj.lv_ring[i].y & 0xff);
            px = (u32)e[0] + bx;
            py = (u32)ly + by;
            if (px >= rc->l && px <= rc->r && py >= rc->t && py <= rc->b)
                ring_place(e, (bx + e[0]) << 8, (by + ly) << 8);
        }
    }
}

static void spawner(int i, u16 x, u16 y, const URect *rc, const URect *o, u32 f) {
    if (i == 0)
        spawn_objects_in_block(x, y, rc, o, f);
    else if (i == 1)
        spawn_rings_in_block(x, y, rc);
    /* i == 2: StageEvent_SpawnDecorationsInBlock (decorations, not in this module) */
}

/* StageEvent_SpawnInView (dual-screen variant: one view per DS screen) */
static void spawn_in_view(int idx, u32 flags) {
    static const u16 margin[3] = { 0x100, 0x10, 0x100 }; /* data_ov000_0236aa78/7c/80 */
    const ObjView *v = &g_obj.view[idx];
    s32 maxx = g_obj.map_w - 1, maxy = g_obj.map_h - 1;
    s32 l, t, r, b;
    URect rc, base;
    u32 f;
    u16 xx, yy;
    int i;
    base.l = (u32)(v->x >> 8);
    base.t = (u32)(v->y >> 8);
    base.r = (u32)((v->x + (v->w << 8)) >> 8);
    base.b = (u32)((v->y + v->h * 0xc0) >> 8);
    for (i = 0; i < 3; i++) {
        u16 rr = margin[i];
        f = flags;
        l = (v->x >> 8) - rr;
        t = (v->y >> 8) - rr;
        r = rr + ((v->x + (v->w << 8)) >> 8);
        b = rr + ((v->y + v->h * 0xc0) >> 8);
        if (l <= 0) { l = 0; f |= 0x10; }
        if (t <= 0) { t = 0; f |= 0x20; }
        if (r >= maxx) { r = maxx; f |= 0x40; }
        if (b >= maxy) { b = maxy; f |= 0x80; }
        rc.l = (u32)l; rc.t = (u32)t; rc.r = (u32)r; rc.b = (u32)b;
        for (yy = (u16)(t >> 8); yy <= (u16)(b >> 8); yy++)
            for (xx = (u16)(l >> 8); xx <= (u16)(r >> 8); xx++)
                spawner(i, xx, yy, &rc, &base, f);
    }
}

/* StageEvent_SpawnAll (boss stages) */
static void spawn_all(void) {
    URect rc;
    s32 maxx = g_obj.map_w - 1, maxy = g_obj.map_h - 1;
    u16 xx, yy;
    int i;
    for (i = 0; i < 3; i++) {
        rc.l = 0; rc.t = 0; rc.r = (u32)maxx; rc.b = (u32)maxy;
        for (yy = 0; yy <= (u16)(maxy >> 8); yy++)
            for (xx = 0; xx <= (u16)(maxx >> 8); xx++)
                spawner(i, xx, yy, &rc, NULL, 0);
    }
}

/* StageEvent_SpawnAllMapObjects (Stage_Create: entries flagged 0x4000) */
static void spawn_all_map_objects(void) {
    URect rc;
    s32 maxx = g_obj.map_w - 1, maxy = g_obj.map_h - 1;
    u16 cols = (u16)((maxx >> 8) + 1), rows = (u16)((maxy >> 8) + 1), xx, yy;
    rc.l = 0; rc.r = (u32)maxx; rc.t = 0; rc.b = (u32)maxy;
    for (yy = 0; yy < rows; yy++)
        for (xx = 0; xx < cols; xx++)
            spawn_objects_in_block(xx, yy, &rc, NULL, 8);
}

/* StageEvent_Main: InitialSpawn, then UpdateDualScreen / UpdateBoss */
static void stage_event_main(void) {
    int i;
    if (g_obj.event_state == 0) {
        if (!(W.zone < 9 && W.act < 2)) {
            spawn_all();
            g_obj.event_state = 2; /* StageEvent_UpdateBoss */
        } else {
            for (i = 0; i < g_obj.n_views; i++)
                spawn_in_view(i, 1);
            g_obj.event_state = 1;
        }
    } else if (g_obj.event_state == 1) {
        for (i = 0; i < g_obj.n_views; i++)
            if (g_obj.view[i].x != g_obj.ev_last[i].x || g_obj.view[i].y != g_obj.ev_last[i].y)
                spawn_in_view(i, 5);
    }
    for (i = 0; i < g_obj.n_views; i++)
        g_obj.ev_last[i] = g_obj.view[i];
}

/* ------------------------------------------------------------- API */
const ObjStats *objects_stats(void) { return &g_obj.stats; }
int objects_ring_count(void) { return ring_count(); }
u8 *objects_ring_scatter_level(void) { return ring_scatter_level(); }
void objects_set_player_anim_done(int on) { g_obj.anim_done_flag = on; }

void objects_set_paths(const char *obj_dir, const char *sprite_dir) {
    strncpy(g_obj.obj_dir, obj_dir, sizeof(g_obj.obj_dir) - 1);
    strncpy(g_obj.spr_dir, sprite_dir, sizeof(g_obj.spr_dir) - 1);
}

void objects_set_view(int idx, s32 x, s32 y) {
    if (idx < 0 || idx >= OBJ_MAX_VIEWS)
        return;
    g_obj.view[idx].x = x;
    g_obj.view[idx].y = y;
    g_obj.view[idx].w = 0x100;
    g_obj.view[idx].h = 0x100;
}
void objects_set_view_count(int n) { g_obj.n_views = n < 1 ? 1 : (n > OBJ_MAX_VIEWS ? OBJ_MAX_VIEWS : n); }

static int load_objdata(int stage) {
    char path[160];
    u8 *b;
    u32 size, off = 12, nslots, nstages, idlen;
    ObjData *d = &g_obj.data;
    int s;
#define TAKE(dst, n) do { memcpy((dst), b + off, (n)); off += (n); off = (off + 3) & ~3u; } while (0)
    snprintf(path, sizeof(path), "%s/objdata.bin", g_obj.obj_dir);
    if (obj_load_file(path, &b, &size) != 0)
        return -1;
    if (size < 516 || memcmp(b, "ROBJ", 4) || (b[4] | b[5] << 8) != 1) {
        free(b);
        return -1;
    }
    nslots = b[6] | b[7] << 8;
    nstages = b[8] | b[9] << 8;
    idlen = b[10] | b[11] << 8;
    TAKE(d->margin, 0xf2);
    TAKE(&d->ring_gravity, 2);
    TAKE(d->itembox_rings, 8);
    TAKE(d->itembox_vs, 8);
    TAKE(d->player_spr_anim, 0x68);
    TAKE(d->needle_shape, 96);
    TAKE(d->animal, 24);
    TAKE(d->shake, 16);
#undef TAKE
    if (nslots != OBJ_BAC_COUNT || idlen != 48 || (u32)stage >= nstages || size != off + nstages * nslots * idlen) {
        free(b);
        return -1;
    }
    for (s = 0; s < OBJ_BAC_COUNT; s++) {
        memcpy(d->ids[s], b + off + (stage * nslots + s) * idlen, 48);
        d->ids[s][47] = 0;
    }
    free(b);
    return 0;
}

int objects_init(const rlvl_map_t *map, const uint8_t *level_data) {
    const rlvl_header_t *h = (const rlvl_header_t *)level_data;
    char od[64], sd[64];
    u32 i;
    int s, missing = 0;
    DcPlayer *keep[2];
    int nkeep = g_obj.n_players;
    for (s = 0; s < nkeep && s < 2; s++)
        keep[s] = g_obj.players[s].p;
    objects_shutdown();
    if (!g_obj.obj_dir[0])
        objects_set_paths("/cd/objects", "/cd/sprites");
    memcpy(od, g_obj.obj_dir, sizeof(od));
    memcpy(sd, g_obj.spr_dir, sizeof(sd));
    memset(&g_obj, 0, sizeof(g_obj));
    memcpy(g_obj.obj_dir, od, sizeof(od));
    memcpy(g_obj.spr_dir, sd, sizeof(sd));
    memset(&g_obj_stage, 0, sizeof(g_obj_stage));
    if (h->magic != RLVL_MAGIC)
        return -1;
    g_obj.map = map;
    g_obj.level = level_data;
    g_obj.hdr = h;
    g_obj.map_w = h->width_blocks * RLVL_BLOCK_PX;
    g_obj.map_h = h->height_blocks * RLVL_BLOCK_PX;
    g_obj.lv_obj = (const rlvl_object_t *)rlvl_section(level_data, RLVL_SEC_OBJECTS, NULL);
    g_obj.lv_obj_region = (const uint32_t *)rlvl_section(level_data, RLVL_SEC_OBJ_REGION, NULL);
    g_obj.lv_ring = (const rlvl_ring_t *)rlvl_section(level_data, RLVL_SEC_RINGS, NULL);
    g_obj.lv_ring_region = (const uint32_t *)rlvl_section(level_data, RLVL_SEC_RING_REGION, NULL);
    if (load_objdata(h->stage) != 0)
        return -2;
    for (s = 0; s < OBJ_BAC_COUNT; s++)
        if (obj_res_load(&g_obj.res[s], g_obj.spr_dir, g_obj.obj_dir, g_obj.data.ids[s]) != 0)
            missing++;
    g_obj.prm = (ObjParam *)calloc(h->num_objects ? h->num_objects : 1, sizeof(ObjParam));
    g_obj.ring_state = (u8 *)calloc(h->num_rings ? h->num_rings : 1, 1);
    if (!g_obj.prm || !g_obj.ring_state)
        return -3;
    for (i = 0; i < h->num_objects; i++) {
        const rlvl_object_t *e = &g_obj.lv_obj[i];
        ObjParam *p = &g_obj.prm[i];
        p->x0 = (u8)(e->x & 0xff);
        p->y0 = (u8)(e->y & 0xff);
        p->type = e->type;
        p->flags = e->flags;
        p->p6 = e->p0;
        p->p7 = e->p1;
        p->p8 = e->p2;
        p->p9 = e->p3;
        p->ax = e->x;
        p->ay = e->y;
    }
    for (i = 0; i < h->num_rings; i++)
        g_obj.ring_state[i] = (u8)(g_obj.lv_ring[i].x & 0xff);
    if (g_obj.n_views == 0)
        g_obj.n_views = 1;
    for (s = 0; s < OBJ_MAX_VIEWS; s++)
        objects_set_view(s, 0, 0);
    W.objmgr |= 0x1c; /* view-out, map collision and hit boxes on (Stage_Create) */
    rect_check_init();
    colmap_reset();
    for (s = 0; s < nkeep && s < 2; s++)
        objects_add_player(keep[s]);
    obj_player_install_hooks();
    spawn_all_map_objects();
    ring_init();
    obj_log("init_missing_sprites", missing, h->stage);
    return 0;
}

void objects_shutdown(void) {
    int s;
    while (g_obj.head) {
        Obj *o = g_obj.head;
        g_obj.head = o->next;
        o->used = 0;
    }
    for (s = 0; s < OBJ_BAC_COUNT; s++)
        if (g_obj.res[s].loaded)
            obj_res_free(&g_obj.res[s]);
    free(g_obj.prm);
    free(g_obj.ring_state);
    g_obj.prm = NULL;
    g_obj.ring_state = NULL;
    if (g_dcp_col.obj_probe != NULL && g_obj.hdr != NULL) {
        g_dcp_col.obj_probe = NULL;
        g_dcp_hooks = g_obj.prev_hooks;
    }
    g_obj.hdr = NULL;
}

/* Stage_Main (hit boxes, solid objects), then StageEvent_Main */
void objects_frame_begin(void) {
    if (!g_obj.hdr)
        return;
    rect_check_all();
    colmap_commit();
    stage_event_main();
}

/* everything that runs after the player task */
void objects_update(DcPlayer *p) {
    Obj *o, *nxt;
    int i;
    (void)p;
    if (!g_obj.hdr)
        return;
    for (i = 0; i < g_obj.n_players; i++)
        obj_player_collide(&g_obj.players[i]); /* Player_Collide (player task, id 0x100) */
    for (o = g_obj.head; o; o = nxt) {
        if (o->prio >= OBJ_PRIO_RING)
            break;
        obj_main(o);
        nxt = o->next; /* read after the main: tasks created by it run this frame */
    }
    ring_main(); /* RingManager (0x3000) and RingEffect2D tasks */
    for (; o; o = nxt) {
        obj_main(o);
        nxt = o->next;
    }
}
