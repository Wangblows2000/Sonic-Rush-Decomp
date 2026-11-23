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
#ifdef _arch_dreamcast
#include "../engine/pak.h"   /* per-stage resource pack */
#endif

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
s32 obj_sfx_count(s32 id) { return g_obj_hooks.sfx_count ? g_obj_hooks.sfx_count(g_obj_hooks.ctx, id) : 0; }
void obj_sfx_own(const void *owner, s32 id, s32 prio) {
    if (g_obj_hooks.sfx_own)
        g_obj_hooks.sfx_own(g_obj_hooks.ctx, owner, id, prio);
    else
        obj_sfx(id);
}
void obj_sfx_own_stop(const void *owner, s32 fade) {
    if (g_obj_hooks.sfx_own_stop)
        g_obj_hooks.sfx_own_stop(g_obj_hooks.ctx, owner, fade);
}
void obj_sfx_own_pos(const void *owner, s32 x, s32 y, s32 px, s32 py, s32 volume) {
    if (g_obj_hooks.sfx_own_pos)
        g_obj_hooks.sfx_own_pos(g_obj_hooks.ctx, owner, x, y, px, py, volume);
}
void obj_sfx_own_tempo(const void *owner, s32 ratio) {
    if (g_obj_hooks.sfx_own_tempo)
        g_obj_hooks.sfx_own_tempo(g_obj_hooks.ctx, owner, ratio);
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
        if (p->gimmick == (void *)o)
            p->gimmick = NULL; /* (Player_In clears it when the object is destroyed) */
    }
}

/* Task_Destroy: destructor, then unlink (the caller keeps iterating via next) */
static void obj_destroy(Obj *o) {
    Obj **pp;
    if (o->dtor)
        o->dtor(o);
    obj_sfx_own_stop(o, -1);   /* MapObject_Destructor: NNS_SndHandleReleaseSeq + Sound_FreeHandle(unk1DC) */
    if (o->spr.palanim) /* StageObj_Destructor: flags 0x800 -> PaletteAnim_Release */
        obj_palanim_release(o);
    obj_free_player_refs(o);
    rect_forget(&o->coll[0]);
    rect_forget(&o->coll[1]);
    rect_forget(&o->coll[2]);
    rect_forget(&o->detect);
    colmap_forget(&o->cmap);
    colmap_forget(&o->cmap2);
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
    obj_res_ensure(bac);
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

/* StageObj_HandleRide, first part (objdata v4): the object's collision map
 * link @0x130 (= &@0x1B8 since MapObject_Init).  A toucher that is not the
 * rider and carries move flag 0x2000000 pushes an object with move flag
 * 0x4000000 by its displacement (clamped by both @0x48).  The player
 * module's dcp_handle_ride has only the ride/touch part. */
static void obj_handle_ride_link(Obj *o) {
    DcPlayer *t;
    s32 v;
    if (o->prm == NULL || o->is_dec)
        return; /* @0x130 is set by MapObject_Init only */
    if (o->cmap.rider != NULL && (o->cmap.rider->obj_flags & 4))
        o->cmap.rider = NULL;
    t = o->cmap.toucher;
    if (t != NULL && t != o->cmap.rider) {
        if ((t->move_flags & 0x2000000) && (o->h.move_flags & 0x4000000)) {
            v = (t->move_x < -t->ride_max) ? -t->ride_max : ((t->move_x > t->ride_max) ? t->ride_max : t->move_x);
            v = ((s16)v < -o->h.ride_max) ? -o->h.ride_max : (((s16)v > o->h.ride_max) ? o->h.ride_max : (s16)v);
            o->h.push_x += (s16)v;
        }
        if (o->cmap.toucher->obj_flags & 4)
            o->cmap.toucher = NULL;
    }
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
    obj_handle_ride_link(o);
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
    if (t == 0x02 || t == 0x04 || t == 0x0b || t == 0x0c || (t >= 0x10 && t <= 0x12))
        return EnemyFlapperGroup_Create;
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
    if ((t >= 0x40 && t <= 0x42) || (t >= 0x59 && t <= 0x5b) || t == 0x7e || t == 0x9d || t == 0x9f ||
        t == 0xa9 || t == 0xb0 || t == 0xb6 || t == 0xb7 || (t >= 0xc3 && t <= 0xc6))
        return mapobj40_create;      /* MapObj40_Create */
    switch (t) {
    case 0x51: case 0x52: case 0x53: case 0x54: case 0x76: case 0xaf: case 0xef:
        return GmkLand_Create;
    case 0x55: return Trampoline_Create;
    case 0x56: case 0x57: return BendStick_Create;
    case 0x58: return Rope_Create;
    case 0x67: return JumpPedestal_Create;
    case 0x68: case 0x69: return CorkscrewTrigger_Create;
    case 0x6f: return RockBreak_Create;
    case 0x72: return PlayerStart_Create;
    case 0x73: return MapObj73_Create;
    case 0x75: return MapObj75_Create;
    case 0x77: return WallBreak_Create;
    case 0x78: case 0x79: return IronBall_Create;
    case 0x7a: return RopeC_Create;
    case 0x9e: return GmkSpecial_Create;
    case 0xdf: return EggPawnM_Create;
    case 0xe1: return RopeCSpring_Create;
    case 0xe2: return RopeCLand_Create;
    case 0xaa: return GmkLandAA_Create;
    case 0x7c: return WaterLevelTrigger_Create;
    case 0x70: return MapObj70_Create;
    /* objdata v3: Water Palace */
    case 0x05: case 0x06: return Kuragen_Create;
    case 0x7b: return WaterRunTrigger_Create;
    case 0x7d: return MapObj7D_Create;
    case 0x7f: return Mine_Create;
    case 0x80: return MapObj80_Create;
    case 0x81: return LandUnrest_Create;
    case 0x82: return Wheel2D_Create;
    case 0x83: return LaunchZone_Create;
    case 0x84: return BubbleSpawner_Create;
    case 0x85: case 0x86: case 0x87: return WaterJet_Create;
    case 0xe3: return KuragenBomb_Create;
    case 0xe8: case 0xe9: case 0xea: return Bubble_Create;
    /* objdata v3: Mirage Road */
    case 0x88: return GmkRail_Create;
    case 0x89: return Cargo_Create;
    case 0x8a: return Hand2d_Create;
    case 0x8b: return GmkTable_Create;
    case 0x8c: case 0x8d: case 0x8e: case 0x8f: return MapObj8C_CreateByType;
    case 0x90: case 0x91: return SandSlider_Create;
    case 0x92: return Rock_Create;
    case 0xe4: return EggBishop_Create;
    /* objdata v4: Night Carnival */
    case 0xae: return CraneSlide_Create;
    case 0xbb: case 0xbc: return Shooter_Create;
    case 0xbd: return SwingSky_Create;
    case 0xbe: case 0xbf: return Switch_Create;
    case 0xc0: case 0xc1: return NeonRoute_Create;
    /* objdata v4: Altitude Limit */
    case 0xb5: return Glider_Create;
    case 0xb1: case 0xb2: return NeedleG_Create;
    case 0xb4: return Parachute_Create;
    case 0xb3: return Saucer_Create;
    case 0x13: case 0xb8: case 0xb9: case 0xba: return MapObj13_Create;
    case 0xc7: return DecCloudSpawner_Create;
    /* objdata v4: Huge Crisis */
    case 0xa0: case 0xa1: return Piston_Create;
    case 0xa2: case 0xa3: case 0xa4: case 0xa5: return Valve_Create;
    case 0xa6: case 0xab: case 0xac: case 0xad: return GmkPipe_Create;
    case 0xa7: return JetCatapult_Create;
    case 0xa8: return Crane_Create;
    case 0xed: return CranePartED_Create;
    case 0xee: return CranePartEE_Create;
    case 0xf1: return Steam_Create;
    /* objdata v4: Dead Line */
    case 0x93: return Rocket_Create;
    case 0x94: return GmkAir_Create;
    case 0x95: return Cannon_Create;
    case 0x96: return Swing_Create;
    case 0x97: return Duct_Create;
    case 0x9c: return MapObj9C_Create;
    /* objdata v4: enemy shots */
    case 0xe6: return EggSf_Create;
    case 0xe7: return EggFlapperLaser_Create;
    case 0xeb: return EggBombHawk_Create;
    case 0xec: return GunHunter_Create;
    case 0xf0: return FlapperCannon_Create;
    }
    return NULL;                     /* not translated (OBJECTS.md) */
}

/* BAC slots a placed object type loads (for the preload at objects_init;
 * anything missed is loaded on first use, see obj_res_ensure) */
static int type_slots(u16 t, u8 *o) {
    int n = 0;
    ObjCreateFn f = obj_create_fn(t);
    if (f == NULL)
        return 0;
    if (f == pawn_create) {
        o[n++] = t == 3 ? OBJ_BAC_EGG_LANCER : (t == 7 || t == 0x15) ? OBJ_BAC_EGG_BISHOP : t == 8 ? OBJ_BAC_EGG_HAMMER :
                 t == 9 ? OBJ_BAC_EGG_SF : t == 0xa ? OBJ_BAC_EGG_SOLID : t == 0xd ? OBJ_BAC_GUN_HUNTER :
                 t == 0xe ? OBJ_BAC_RHINO : t == 0xf ? OBJ_BAC_EGG_BUNNY : OBJ_BAC_EGGPAWN;
        if (t == 0xe) o[n++] = OBJ_BAC_EFF_BRAKE;
        o[n++] = t == 1 ? OBJ_BAC_EGGPAWN_M : OBJ_BAC_EFF_FOUND;
        if (t == 0x14 || t == 0x15 || t == 7) o[n++] = OBJ_BAC_EFF_SHADOW;
    }
    else if (f == EnemyFlapperGroup_Create) {
        o[n++] = t == 4 ? OBJ_BAC_EGG_DIVER : t == 0xb ? OBJ_BAC_EGG_FLAPPER_LASER : t == 0xc ? OBJ_BAC_BOMB_HAWK :
                 t == 0x10 ? OBJ_BAC_FLAPPER_BAT : t == 0x11 ? OBJ_BAC_FLAPPER_CANNON : t == 0x12 ? OBJ_BAC_FALCO : OBJ_BAC_FLAPPER;
        o[n++] = OBJ_BAC_EFF_FOUND;
        if (t == 0xc) { o[n++] = OBJ_BAC_ZAKO_BOMB; o[n++] = OBJ_BAC_HAHEN; }
    }
    else if (f == itembox_create) o[n++] = OBJ_BAC_ITEMBOX;
    else if (f == spring_create) o[n++] = OBJ_BAC_SPRING;
    else if (f == needle_create) o[n++] = OBJ_BAC_NEEDLE;
    else if (f == dashp_create) o[n++] = OBJ_BAC_DASH_P;
    else if (f == dashc_create_by_type) { o[n++] = OBJ_BAC_DASH_C; o[n++] = OBJ_BAC_DASH_CT; }
    else if (f == marker_create) o[n++] = OBJ_BAC_MARKER;
    else if (f == goalring_create) o[n++] = OBJ_BAC_GOALRING;
    else if (f == GmkLand_Create) o[n++] = t == 0x76 ? OBJ_BAC_FLOOR_BREAK : OBJ_BAC_LAND;
    else if (f == Trampoline_Create) o[n++] = OBJ_BAC_TRAMPOLINE;
    else if (f == BendStick_Create) o[n++] = OBJ_BAC_BEND_STICK;
    else if (f == Rope_Create) o[n++] = OBJ_BAC_ROPE;
    else if (f == JumpPedestal_Create) o[n++] = OBJ_BAC_JUMP_PEDESTAL;
    else if (f == RockBreak_Create) o[n++] = OBJ_BAC_ROCK_BREAK;
    else if (f == WallBreak_Create) o[n++] = OBJ_BAC_WALL_BREAK;
    else if (f == IronBall_Create) o[n++] = OBJ_BAC_IRON_BALL;
    else if (f == RopeC_Create) { o[n++] = OBJ_BAC_ROPE_C; o[n++] = OBJ_BAC_ROPE_C_LAND; o[n++] = OBJ_BAC_ROPE_C_SPRING; o[n++] = OBJ_BAC_SPRING; }
    else if (f == GmkSpecial_Create) { o[n++] = OBJ_BAC_SPECIAL; o[n++] = OBJ_BAC_CARGO; }
    else if (f == EggPawnM_Create) o[n++] = OBJ_BAC_EGGPAWN_M;
    else if (f == GmkLandAA_Create) o[n++] = OBJ_BAC_LAND;
    /* objdata v3: Water Palace */
    else if (f == Kuragen_Create) { o[n++] = OBJ_BAC_KURAGEN_BOMB; o[n++] = OBJ_BAC_KURAGEN_BLITZ; }
    else if (f == MapObj7D_Create || f == MapObj80_Create) o[n++] = OBJ_BAC_EFF_WATER_GMK;
    else if (f == Mine_Create) { o[n++] = OBJ_BAC_MINE; o[n++] = OBJ_BAC_FIX_MOJI; }
    else if (f == LandUnrest_Create) o[n++] = OBJ_BAC_LAND_UNREST;
    else if (f == Wheel2D_Create) { o[n++] = OBJ_BAC_WHEEL2D; o[n++] = OBJ_BAC_BUBBLE; }
    else if (f == BubbleSpawner_Create || f == Bubble_Create) o[n++] = OBJ_BAC_BUBBLE;
    else if (f == WaterJet_Create) o[n++] = OBJ_BAC_WATER_JET;
    else if (f == WaterRunTrigger_Create) o[n++] = OBJ_BAC_EFF_WATER;
    /* objdata v3: Mirage Road */
    else if (f == GmkRail_Create) o[n++] = W.zone == 5 ? OBJ_BAC_GMK_RAIL6 : OBJ_BAC_GMK_RAIL;
    else if (f == Cargo_Create) o[n++] = OBJ_BAC_CARGO;
    else if (f == Hand2d_Create) o[n++] = OBJ_BAC_HAND2D;
    else if (f == GmkTable_Create) { o[n++] = OBJ_BAC_EFF_SHADOW; o[n++] = OBJ_BAC_ZAKO_BOMB; }
    else if (f == MapObj8C_CreateByType) { if (t == 0x8e) o[n++] = OBJ_BAC_GMK_WALL; if (t == 0x8d) { o[n++] = OBJ_BAC_EFF_ENTRANCE; o[n++] = OBJ_BAC_EGGPAWN; o[n++] = OBJ_BAC_EGG_HAMMER; } }
    else if (f == SandSlider_Create) { o[n++] = OBJ_BAC_SAND_SLIDER; o[n++] = OBJ_BAC_EFF_SAND; }
    else if (f == Rock_Create) { o[n++] = OBJ_BAC_ROCK; o[n++] = OBJ_BAC_EFF_SHADOW; }
    else if (f == EggBishop_Create) o[n++] = OBJ_BAC_EGG_BISHOP;
    else if (f == mapobj40_create && t == 0xa9) o[n++] = OBJ_BAC_EFF_SAND;
    /* objdata v4: Night Carnival */
    else if (f == CraneSlide_Create) o[n++] = OBJ_BAC_CRANE_SLIDE;
    else if (f == Shooter_Create) o[n++] = OBJ_BAC_SHOOTER;
    else if (f == SwingSky_Create) { o[n++] = OBJ_BAC_SWING_SKY; o[n++] = OBJ_BAC_FIX_INFORMATION; }
    else if (f == Switch_Create) o[n++] = OBJ_BAC_SWITCH;
    else if (f == NeonRoute_Create) o[n++] = OBJ_BAC_NEON_ROUTE;
    /* objdata v4: Altitude Limit */
    else if (f == Glider_Create) o[n++] = OBJ_BAC_GLIDER;
    else if (f == NeedleG_Create) o[n++] = OBJ_BAC_NEEDLE_G;
    else if (f == Parachute_Create) o[n++] = OBJ_BAC_PARACHUTE;
    else if (f == Saucer_Create) { o[n++] = OBJ_BAC_SAUCER; o[n++] = OBJ_BAC_ZAKO_BOMB; o[n++] = OBJ_BAC_HAHEN; }
    else if (f == MapObj13_Create) o[n++] = t == 0x13 ? OBJ_BAC_FLAPPER3D : t == 0xb8 ? OBJ_BAC_RING_GRAIDER : OBJ_BAC_CLOUD_3;
    else if (f == DecCloudSpawner_Create) o[n++] = OBJ_BAC_DEC_CLOUD_W;
    /* objdata v4: Huge Crisis */
    else if (f == Piston_Create) o[n++] = OBJ_BAC_PISTON;
    else if (f == Valve_Create) { o[n++] = OBJ_BAC_VALVE; o[n++] = OBJ_BAC_EFF_STEAM; }
    else if (f == GmkPipe_Create) o[n++] = OBJ_BAC_PIPE;
    else if (f == JetCatapult_Create) { o[n++] = OBJ_BAC_JET_CATAPULT; o[n++] = OBJ_BAC_EFF_JET; o[n++] = OBJ_BAC_HAHEN; }
    else if (f == Crane_Create || f == CranePartED_Create || f == CranePartEE_Create) o[n++] = OBJ_BAC_CRANE;
    else if (f == Steam_Create) o[n++] = OBJ_BAC_EFF_STEAM;
    /* objdata v4: Dead Line */
    else if (f == Rocket_Create) { o[n++] = OBJ_BAC_ROCKET; o[n++] = OBJ_BAC_ZAKO_BOMB; o[n++] = OBJ_BAC_HAHEN; }
    else if (f == GmkAir_Create) o[n++] = OBJ_BAC_AIR;
    else if (f == Cannon_Create) { o[n++] = OBJ_BAC_CANNON; o[n++] = OBJ_BAC_ZAKO_BOMB; }
    else if (f == Swing_Create) { o[n++] = OBJ_BAC_SWING; o[n++] = OBJ_BAC_FIX_INFORMATION; }
    else if (f == Duct_Create) o[n++] = OBJ_BAC_DUCT;
    return n;
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
    else if (g_obj.lv_dec != NULL)
        spawn_decorations_in_block(x, y, rc, o, f); /* StageEvent_SpawnDecorationsInBlock */
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

/* StageEvent_SpawnInRect (src/asm_fallback, translated from the ARM listing):
 * every block of the rect, all three spawners, flags 0xf0 (no rect / view test) */
void objects_spawn_rect(s32 l, s32 t, s32 r, s32 b) {
    s32 mw = g_obj.map_w - 1, mh = g_obj.map_h - 1;
    URect rc;
    u16 xx, yy;
    int i;
    if (!g_obj.hdr)
        return;
    if (l < 0) l = 0; /* (the room objects clamp to 0 before the call) */
    if (t < 0) t = 0;
    if (l > mw) l = mw;
    if (t > mh) t = mh;
    if (r > mw) r = mw;
    if (b > mh) b = mh;
    rc.l = (u32)l; rc.t = (u32)t; rc.r = (u32)r; rc.b = (u32)b;
    for (i = 0; i < 3; i++)
        for (yy = (u16)(((u32)t << 8) >> 16); yy <= (u16)(b >> 8); yy++)
            for (xx = (u16)(((u32)l << 8) >> 16); xx <= (u16)(r >> 8); xx++)
                spawner(i, xx, yy, &rc, NULL, 0xf0);
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

static u32 rd32(const u8 *p) { return (u32)p[0] | (u32)p[1] << 8 | (u32)p[2] << 16 | (u32)p[3] << 24; }
void dec_preload(const rlvl_header_t *h);
int dec_init_level(const uint8_t *level_data);
void camera_init(void);

/* ------------------------------------------------------------- API */
const ObjStats *objects_stats(void) { return &g_obj.stats; }
int objects_ring_count(void) { return ring_count(); }
u8 *objects_ring_scatter_level(void) { return ring_scatter_level(); }
void objects_set_player_anim_done(int on) { g_obj.anim_done_flag = on; }
void objects_set_grind_check(int on) { g_obj.no_grind_check = !on; }
void objects_set_underwater_check(int on) { g_obj.no_underwater = !on; }

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
    if (idx < 2) {   /* view 2 (the DC window) has no gCamera mirror */
        g_obj_camera[idx].x = x;
        g_obj_camera[idx].y = y;
    }
    if (idx == 0 && g_obj.n_views == 1) { /* one DC view: camera 1 follows camera 0 */
        g_obj_camera[1].x = x;
        g_obj_camera[1].y = y;
    }
}
/* StageEvent_InitialSpawn again on the next objects_frame_begin (after a
 * respawn / teleport the DS restarts the stage, which does the same) */
void objects_respawn_view(void) {
    if (g_obj.event_state == 1)
        g_obj.event_state = 0;
}
void objects_set_view_count(int n) { g_obj.n_views = n < 1 ? 1 : (n > OBJ_MAX_VIEWS ? OBJ_MAX_VIEWS : n); }

/* objdata.bin "ROBJ" v2 (tools/objdata.py) */
static int load_objdata(int stage) {
    char path[160];
    u8 *b;
    u32 size, off = 12, nslots, nstages, idlen, i, n, dsz, pos;
    ObjData *d = &g_obj.data;
    int s;
#define TAKE(dst, n) do { memcpy((dst), b + off, (n)); off += (n); off = (off + 3) & ~3u; } while (0)
    snprintf(path, sizeof(path), "%s/objdata.bin", g_obj.obj_dir);
    if (obj_load_file(path, &b, &size) != 0)
        return -1;
    if (size < 1024 || memcmp(b, "ROBJ", 4) || (b[4] | b[5] << 8) != 4) {
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
    TAKE(d->solid, 384);
    TAKE(d->animal, 24);
    TAKE(d->shake, 16);
    TAKE(d->col_attr, 4);
    TAKE(d->bend_wobble, 28);
    TAKE(d->pedestal, 48);
    TAKE(d->dec_margin, 0xf2);
    TAKE(d->valve_ofs, 16);
    TAKE(d->valve_box, 32);
    TAKE(d->valve_puff, 32);
    TAKE(d->valve_anim, 48);
#undef TAKE
    if (nslots != OBJ_BAC_COUNT || idlen != 48 || (u32)stage >= nstages || size < off + nstages * nslots * idlen) {
        free(b);
        return -1;
    }
    for (s = 0; s < OBJ_BAC_COUNT; s++) {
        memcpy(d->ids[s], b + off + (stage * nslots + s) * idlen, 48);
        d->ids[s][47] = 0;
    }
    off += nstages * nslots * idlen;
    /* per stage .df blobs: u32 offset table [stages + 1], then {char name[24]; u32 size; data (4-aligned)} */
    if (size >= off + (nstages + 1) * 4) {
        u32 o0 = rd32(b + off + stage * 4), o1 = rd32(b + off + (stage + 1) * 4);
        if (o1 >= o0 && o1 <= size) {
            d->df_buf = (u8 *)malloc(o1 - o0 + 1);
            if (d->df_buf) {
                memcpy(d->df_buf, b + o0, o1 - o0);
                for (pos = 0, i = 0; pos + 28 <= o1 - o0 && i < 8; i++) {
                    memcpy(d->df[i].name, d->df_buf + pos, 24);
                    d->df[i].name[23] = 0;
                    dsz = rd32(d->df_buf + pos + 24);
                    d->df[i].size = dsz;
                    d->df[i].data = d->df_buf + pos + 28;
                    n = (28 + dsz + 3) & ~3u;
                    pos += n;
                }
                d->n_df = (int)i;
            }
        }
    }
    free(b);
    return 0;
}

/* ObjData_Load of a /dat/gmk/<name> file (resolved offline per stage) */
const u8 *obj_df(const char *name, u32 *size) {
    int i;
    for (i = 0; i < g_obj.data.n_df; i++)
        if (!strcmp(g_obj.data.df[i].name, name)) {
            if (size)
                *size = g_obj.data.df[i].size;
            return g_obj.data.df[i].data;
        }
    return NULL;
}

/* resources are loaded at objects_init for the placed types (preload) or on
 * first use; a slot that does not resolve on this stage stays empty */
int obj_res_ensure(int bac) {
    ObjRes *r;
    if (bac < 0 || bac >= OBJ_BAC_COUNT)
        return -1;
    r = &g_obj.res[bac];
    if (r->loaded)
        return 0;
    if (r->id[0] == 1) /* tried before */
        return -1;
    if (!g_obj.data.ids[bac][0] || obj_res_load(r, g_obj.spr_dir, g_obj.obj_dir, g_obj.data.ids[bac]) != 0) {
        memset(r, 0, sizeof(*r));
        r->id[0] = 1;
        return -1;
    }
    if (g_obj.preloaded)
        g_obj.lazy_loads++;
    return 0;
}

static void preload_resources(const rlvl_header_t *h) {
    static const u8 always[] = { OBJ_BAC_RING, OBJ_BAC_ZAKO_BOMB, OBJ_BAC_HAHEN, OBJ_BAC_PLAYER, OBJ_BAC_EFF_FOUND };
    u8 seen[0x100], sl[8];
    u32 i;
    int k, n;
    memset(seen, 0, sizeof(seen));
    for (i = 0; i < sizeof(always); i++)
        obj_res_ensure(always[i]);
    if (h->zone < 9 && h->act < 2) {
        /* the player's effects (obj_peffect.c): created mid-run, so no disc read then */
        obj_res_ensure(OBJ_BAC_EFF_BOOST);
        obj_res_ensure(OBJ_BAC_EFF_SPIN1);
        obj_res_ensure(OBJ_BAC_EFF_SPIN2);
        obj_res_ensure(OBJ_BAC_EFF_BRAKE);
        obj_res_ensure(OBJ_BAC_EFF_TRICK);
    }
    for (i = 0; i < h->num_objects; i++) {
        u16 t = g_obj.lv_obj[i].type;
        if (t >= 0x100 || seen[t])
            continue;
        seen[t] = 1;
        n = type_slots(t, sl);
        for (k = 0; k < n; k++)
            obj_res_ensure(sl[k]);
    }
    dec_preload(h);
    if (W.zone == 1) { /* Player_UpdateUnderwater effects (water entry splash, air bubbles, countdown) */
        obj_res_ensure(OBJ_BAC_EFF_WATER);
        obj_res_ensure(OBJ_BAC_BUBBLE);
    }
    if (W.zone < 8) /* freed animals of this zone (MapObject_SpawnFreedAnimal) */
        for (k = 0; k < 3; k++)
            obj_res_ensure(OBJ_BAC_ANIMAL01 + g_obj.data.animal[W.zone][k]);
    g_obj.preloaded = 1;
}

/* ------------------------------------- Stage_Save/RestoreEventState (unk18 & 0x40) */
/* data_022c4520 (buf0 / size0: map-object list, buf1 / size1: ring list) and
 * the data_022c4590 restart camera unk58 / unk5A / unk5C: static, they
 * outlive the stage like the DS globals */
static struct {
    u8 valid, pending, cam_set;
    u8 stage;
    u32 n_obj, n_ring;
    ObjParam *prm;
    u8 *ring;
    ObjStage vars;
    s16 cam_x, cam_y; /* unk58 / unk5A (px) */
    u32 cam_f;        /* unk5C: gCamera[2] flags & 2 */
} s_ev;
static void destroy_tasks(void) { /* Stage_DestroyTasks: object tasks and the ring manager */
    while (g_obj.head)
        obj_destroy(g_obj.head);
    ring_destroy();
}
void objects_save_event_state(void) {
    const rlvl_header_t *h = g_obj.hdr;
    void *p;
    if (h == NULL || g_obj.prm == NULL)
        return;
    destroy_tasks();
    p = realloc(s_ev.prm, (h->num_objects ? h->num_objects : 1) * sizeof(ObjParam));
    if (p == NULL)
        return;
    s_ev.prm = (ObjParam *)p;
    p = realloc(s_ev.ring, h->num_rings ? h->num_rings : 1);
    if (p == NULL)
        return;
    s_ev.ring = (u8 *)p;
    s_ev.n_obj = h->num_objects;
    s_ev.n_ring = h->num_rings;
    s_ev.stage = h->stage;
    memcpy(s_ev.prm, g_obj.prm, h->num_objects * sizeof(ObjParam)); /* Stage_SaveEventState */
    memcpy(s_ev.ring, g_obj.ring_state, h->num_rings);
    s_ev.vars = g_obj_stage;
    s_ev.cam_x = (s16)(g_obj_camera[0].x >> 8); /* Stage_SaveRestartCamera */
    s_ev.cam_y = (s16)(g_obj_camera[0].y >> 8);
    s_ev.cam_f = g_obj_camera[2].flags & 2;
    s_ev.valid = 1;
    s_ev.pending = 0;
    obj_log("save_event_state", (s32)s_ev.n_obj, (s32)s_ev.n_ring);
}
/* Stage_RestoreEventState + Stage_RestoreRestartCamera */
static int apply_event_state(void) {
    const rlvl_header_t *h = g_obj.hdr;
    s32 x, y;
    s_ev.pending = 0;
    if (!s_ev.valid || h == NULL || h->stage != s_ev.stage || h->num_objects != s_ev.n_obj || h->num_rings != s_ev.n_ring)
        return 0;
    s_ev.valid = 0;
    memcpy(g_obj.prm, s_ev.prm, s_ev.n_obj * sizeof(ObjParam));
    memcpy(g_obj.ring_state, s_ev.ring, s_ev.n_ring);
    g_obj_stage = s_ev.vars;
    y = s_ev.cam_y << 8;
    g_obj_camera[2].flags |= s_ev.cam_f;
    x = s_ev.cam_x << 8;
    g_obj_camera[0].x = x;
    g_obj_camera[0].y = y;
    g_obj_camera[1].x = x;
    if (!(g_obj_camera[2].flags & 2))
        g_obj_camera[1].y = g_obj_camera[0].y + 0x11000;
    else
        g_obj_camera[1].y = g_obj_camera[0].y - 0x11000;
    s_ev.cam_set = 1;
    obj_log("restore_event_state", (s32)s_ev.n_obj, (s32)s_ev.n_ring);
    return 1;
}
static void spawn_all_map_objects(void);
void objects_restore_event_state(void) {
    if (!s_ev.valid)
        return;
    if (g_obj.hdr == NULL) { /* before objects_init: applied at the SetupResources point */
        s_ev.pending = 1;
        return;
    }
    /* after objects_init: the objects StageStart_State_CreateStage made from
     * the fresh list go, the list is restored, and they are made again */
    if (g_obj.hdr->stage != s_ev.stage)
        return;
    destroy_tasks();
    if (!apply_event_state())
        return;
    ring_init();
    obj_vs_reset();
    spawn_all_map_objects();
    g_obj.event_state = 0;
}
int objects_restart_camera(s32 *x, s32 *y, u32 *flags) {
    if (x)
        *x = g_obj_camera[0].x;
    if (y)
        *y = g_obj_camera[0].y;
    if (flags)
        *flags = g_obj_camera[2].flags & 2;
    return s_ev.cam_set;
}

int objects_init(const rlvl_map_t *map, const uint8_t *level_data) {
    const rlvl_header_t *h = (const rlvl_header_t *)level_data;
    char od[64], sd[64];
    u32 i;
    int s, loaded = 0;
    DcPlayer *keep[2];
    int nkeep = g_obj.n_players;
    for (s = 0; s < nkeep && s < 2; s++)
        keep[s] = g_obj.players[s].p;
    objects_shutdown();
    if (!g_obj.obj_dir[0])
        objects_set_paths("/cd/objects", "/cd/sprites");
    memcpy(od, g_obj.obj_dir, sizeof(od));
    memcpy(sd, g_obj.spr_dir, sizeof(sd));
    s = g_obj.no_grind_check | (g_obj.no_underwater << 1) | (g_obj.anim_done_flag << 2) |
        (g_obj.n_views << 4); /* settings survive the reset */
    memset(&g_obj, 0, sizeof(g_obj));
    g_obj.n_views = (s >> 4) & 7;
    g_obj.no_grind_check = s & 1;
    g_obj.no_underwater = (s >> 1) & 1;
    g_obj.anim_done_flag = (s >> 2) & 1;
    memcpy(g_obj.obj_dir, od, sizeof(od));
    memcpy(g_obj.spr_dir, sd, sizeof(sd));
    memset(&g_obj_stage, 0, sizeof(g_obj_stage));
    if (h->magic != RLVL_MAGIC)
        return -1;
    obj_palanim_load(g_obj.obj_dir);
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
    if (dec_init_level(level_data) != 0)
        return -3;
#ifdef _arch_dreamcast
    {   /* the stage's resources in one read (objects/stageNN.pak, tools/dreamcast/mkpak.py) */
        char pk[96];
        snprintf(pk, sizeof(pk), "%s/stage%02d.pak", g_obj.obj_dir, h->stage);
        pak_stage_begin(pk);
    }
#endif
    preload_resources(h);
#ifdef _arch_dreamcast
    pak_stage_end();
#endif
    for (s = 0; s < OBJ_BAC_COUNT; s++)
        if (g_obj.res[s].loaded)
            loaded++;
    if (g_obj.n_views == 0)
        g_obj.n_views = 1;
    camera_init();
    dec_shared_reset();
    mirage_reset();
    carnival_reset();
    altitude_reset();
    crisis_reset();
    for (s = 0; s < OBJ_MAX_VIEWS; s++)
        objects_set_view(s, 0, 0);
    W.objmgr |= 0x1c; /* view-out, map collision and hit boxes on (Stage_Create) */
    rect_check_init();
    colmap_reset();
    for (s = 0; s < nkeep && s < 2; s++)
        objects_add_player(keep[s]);
    obj_player_install_hooks();
    s_ev.cam_set = 0;
    if (s_ev.pending) /* StageStart_State_SetupResources: unk18 & 0x40 */
        apply_event_state();
    spawn_all_map_objects();
    ring_init();
    obj_vs_reset();
    obj_log("init_resources", loaded, h->stage);
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
    obj_palanim_reset();
    free(g_obj.prm);
    free(g_obj.ring_state);
    free(g_obj.dprm);
    free(g_obj.data.df_buf);
    g_obj.prm = NULL;
    g_obj.ring_state = NULL;
    g_obj.dprm = NULL;
    g_obj.data.df_buf = NULL;
    g_obj.data.n_df = 0;
    if (g_dcp_col.obj_probe != NULL && g_obj.hdr != NULL) {
        g_dcp_col.obj_probe = NULL;
        g_dcp_hooks = g_obj.prev_hooks;
    }
    g_obj.hdr = NULL;
}

/* Stage_Main (hit boxes, solid objects), then StageEvent_Main */
void objects_frame_begin(void) {
    int i;
    if (!g_obj.hdr)
        return;
    Obj *o, *nxt;
    rect_check_all();
    colmap_commit();
    stage_event_main();
    /* tasks that run before the player (id < 0x100: GmkLand, GmkLandAA,
     * WallBreak use 0xf6, so platforms move before the player rides them) */
    for (o = g_obj.head; o && o->prio < OBJ_PRIO_PLAYER; o = nxt) {
        obj_main(o);
        nxt = o->next;
    }
    /* Player_In parts the player module leaves to the objects (they read
     * the hit box lists just swapped by Stage_Main): Player_FindHomingTarget
     * and Player_CheckStartGrind (see OBJECTS.md, player-module changes) */
    for (i = 0; i < g_obj.n_players; i++) {
        DcPlayer *p = g_obj.players[i].p;
        obj_player_check_gimmick(p);
        if (!g_obj.no_underwater)
            Player_UpdateUnderwater(p);
        Player_FindHomingTarget(p);
        if (!g_obj.no_grind_check)
            Player_CheckStartGrind(p);
    }
}

/* everything that runs after the player task */
void objects_update(DcPlayer *p) {
    Obj *o, *nxt;
    int i;
    (void)p;
    if (!g_obj.hdr)
        return;
    for (i = 0; i < g_obj.n_players; i++) {
        DcPlayer *pl = g_obj.players[i].p;
        obj_player_collide(&g_obj.players[i]); /* Player_Collide (player task, id 0x100) */
        if (pl->pf & 0x20000000)
            RingManager_AttractRings(pl); /* Player_Last: magnet barrier */
    }
    for (o = g_obj.head; o && o->prio < OBJ_PRIO_PLAYER; o = o->next)
        ; /* ran in objects_frame_begin */
    for (; o; o = nxt) {
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
    obj_palanim_main(); /* PaletteAnim tasks (id 0x3e00) */
}
