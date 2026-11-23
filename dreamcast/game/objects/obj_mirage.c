/*
 * Mirage Road (zone 3) objects, enemies, decorations and effects, copied
 * from the decompilation and adapted like obj_water.c.
 * See docs/dreamcast/translation/OBJECTS.md (section 5c).
 */
#include <string.h>
#include "obj_internal.h"

static void set_prio(Obj *o, u32 prio) { o->spr.flags = (o->spr.flags & ~0x60000u) | (prio << 17); }
static Obj *eff_alloc(int bac) {
    Obj *o = obj_alloc(OBJ_PRIO_EFFECT, obj_stageobj_dtor);
    if (o == NULL)
        return NULL;
    if (bac >= 0)
        obj_load_bac(o, bac);
    o->kind = OBJK_EFFECT;
    return o;
}
static void eff_set_parent(Obj *w, DcPlayer *arg) {
    if (obj_player_ctx(arg) != NULL)
        w->ppar = arg;
    else
        w->parent = (Obj *)arg;
}
/* Effect_State_FollowParent (also for a player parent) */
static void eff_state_follow_parent(Obj *w) {
    const DcPlayer *p = w->parent ? &w->parent->h : w->ppar;
    if (p != NULL) {
        w->h.x = p->x;
        w->h.y = p->y;
        w->h.z = p->z;
        if (w->h.disp_flags & 1)
            w->h.x -= w->h.vel_x * 0x100;
        else
            w->h.x += w->h.vel_x * 0x100;
        w->h.y += w->h.vel_y * 0x100;
    }
    if ((w->h.disp_flags & 4) || (w->h.disp_flags & 8)) {
        if (w->wC == 0)
            w->h.obj_flags |= 4;
        else
            w->wC--;
    }
}

/* EffSand_Create (sand kicked up, MapObj40 0xa9 area) */
void EffSand_Create(DcPlayer *arg, s16 x, s16 y) {
    Obj *w = eff_alloc(OBJ_BAC_EFF_SAND);
    if (w == NULL)
        return;
    w->spr.order = 4;
    set_prio(w, 1);
    if (arg->disp_flags & 1)
        w->h.disp_flags |= 1;
    w->h.x = arg->x + (x << 8);
    w->h.y = arg->y + (y << 8);
    eff_set_parent(w, arg);
    w->h.vel_x = x;
    w->h.vel_y = y;
    w->h.move_flags |= 0x2100;
    w->h.obj_flags |= 2;
    w->state = eff_state_follow_parent;
}
/* EffSandSlide_Create (sand spray of the sand slide) */
void EffSandSlide_Create(DcPlayer *arg, s16 x, s16 y) {
    Obj *w = eff_alloc(OBJ_BAC_EFF_SAND);
    if (w == NULL)
        return;
    obj_set_anim(w, 1);
    w->spr.order = 4;
    set_prio(w, 0);
    w->h.vel_x = x;
    w->h.vel_y = y;
    if (arg->disp_flags & 1) {
        w->h.disp_flags |= 1;
        x = (s16)-x;
    }
    w->h.x = arg->x + (x << 8);
    w->h.y = arg->y + (y << 8);
    eff_set_parent(w, arg);
    w->h.move_flags |= 0x2100;
    w->h.obj_flags |= 2;
    w->state = eff_state_follow_parent;
}

/* ============================================================ helpers */
#define CAM0 g_obj_camera[0]
static DcPlayer *player_n(int i) { return (i >= 0 && i < g_obj.n_players) ? g_obj.players[i].p : NULL; }
static Obj *mapobj(ObjParam *p, s32 x, s32 y, int bac, u16 prio, ObjFn dtor) {
    Obj *o = obj_alloc(prio, dtor);
    if (o == NULL)
        return NULL;
    obj_mapobject_init(o, p, x, y);
    o->kind = OBJK_GIMMICK;
    if (bac >= 0)
        obj_load_bac(o, bac);
    return o;
}
static void map_hitbox(Obj *w, s8 l, s8 t, s8 r, s8 b) {
    w->h.box_l = l;
    w->h.box_t = t;
    w->h.box_r = r;
    w->h.box_b = b;
}
/* the collision map pointers @0x1B8 (object) / @0x1C8 (tiles): NULL disables it */
static void cmap_set(Obj *w, const void *map, u16 cw, u16 ch, s16 ox, s16 oy) {
    w->cmap.obj = w;
    w->cmap.map = (const u8(*)[8])map;
    w->cmap.w = cw;
    w->cmap.h = ch;
    w->cmap.ox = ox;
    w->cmap.oy = oy;
    w->has_cmap = map != NULL;
}
static void count(const char *ev, Obj *o, DcPlayer *p) {
    g_obj.stats.gimmicks++;
    obj_log(ev, o && o->prm ? o->prm->type : 0, p ? p->x >> 8 : (o ? o->h.x >> 8 : 0));
}
static void mtx_store(Obj *w, const ObjMtx33 *m) { memcpy(w->mtx, m->m, sizeof(w->mtx)); }
static Obj *s_gmk_table;               /* data_ov000_0236a2fc: the GmkTable alive */
static Obj *s_enemy_room[8];           /* gEnemyRoomTable */
static u8 s_enemy_room_active = 0xff;  /* gCamera.unk10C */
void mirage_reset(void) {
    s_gmk_table = NULL;
    memset(s_enemy_room, 0, sizeof(s_enemy_room));
    s_enemy_room_active = 0xff;
}

/* ====================================================== EffShadow */
static void EffShadow_State_Main(Obj *w) {
    const DcpObjRef *r = w->h.ride;
    const DcPlayer *o = w->parent ? &w->parent->h : w->ppar;
    s32 v, bx, ax, d;
    s16 dx, dy, sn, t;
    u8 ang;
    if (s_gmk_table == NULL || r != &s_gmk_table->ref) {
        w->h.obj_flags |= 4;
        return;
    }
    if (o == NULL)
        return;
    w->h.z = o->z - 0x800;
    v = -0x1200;
    w->h.x = r->x;
    w->h.y = r->y;
    w->h.angle = s_gmk_table->h.angle;
    ax = r->x;
    bx = o->x;
    d = bx - ax;
    ang = w->h.angle;
    dx = (s16)(d >> 8);
    dy = (s16)((o->y + ((o->box_b + 2) << 8) - r->y) >> 8);
    if (ang != 0) {
        sn = obj_sin((u16)(ang << 8));
        t = (s16)((sn * 0xa400) >> 12);
        v = ((t * (d >> 7)) >> 8) - 0x1200;
        if (ang > 0x80)
            v += ((s16)(sn << 1) * -((bx - 0x8000 - ax) >> 8)) >> 8;
        else
            v += ((s16)(sn << 1) * -((bx + 0x8000 - ax) >> 8)) >> 8;
    }
    if ((v >> 8) > dy)
        dy = (s16)(v >> 8);
    w->h.x += dx << 8;
    w->h.y += dy << 8;
}
/* EffShadow_Create: the shadow of an object on the GmkTable (StageObj_Load3dBAC ac_eff_shadow:
 * drawn as a 3D sprite, Obj.bac3d) */
void EffShadow_Create(DcPlayer *arg) {
    Obj *w = eff_alloc(OBJ_BAC_EFF_SHADOW);
    if (w == NULL)
        return;
    w->bac3d = 1;
    w->h.disp_flags |= 0x200;
    w->spr.order = 0x16;
    eff_set_parent(w, arg);
    w->h.x = arg->x;
    w->h.z = arg->z - 0x800;
    w->h.ride = s_gmk_table ? &s_gmk_table->ref : NULL;
    if (s_gmk_table != NULL) {
        w->h.y = s_gmk_table->h.y - 0x1200;
        w->h.angle = s_gmk_table->h.angle;
    }
    w->h.move_flags |= 0x100;
    w->h.obj_flags |= 2;
    w->h.obj_flags |= 0x10;
    w->h.disp_flags |= 0x100;
    w->state = EffShadow_State_Main;
}

/* ====================================================== Cargo (0x89) */
static void Cargo_State_Stopped(Obj *p);
static void Cargo_State_Move(Obj *w);
static void Cargo_Action_Stop(Obj *w) {
    obj_set_anim(w, 0);
    w->state = Cargo_State_Stopped;
    w->h.vel_x = 0;
    w->h.vel_y = 0;
    w->h.move_x = 0;
    w->h.move_y = 0;
    w->wC = 0;
    w->h.obj_flags &= ~2u;
    w->h.move_flags |= 0x2000;
}
static void Cargo_Action_StartMove(Obj *w, s16 v) {
    w->state = Cargo_State_Move;
    w->h.obj_flags |= 2;
    w->h.move_flags &= ~0x2000u;
    w->h.gnd_vel = v;
    w->wC = 0;
    w->f1F4 &= ~3u;
}
static void Cargo_State_Stopped(Obj *p) {
    if (!(p->f1F4 & 4))
        return;
    p->wC++;
    if (p->wC <= 0x78)
        return;
    Cargo_Action_StartMove(p, 0);
}
static void Cargo_State_Move(Obj *w) {
    u32 f, flip;
    s32 c, x, y;
    u16 a;
    if (w->f1F4 & 2) {
        Cargo_Action_Stop(w);
        return;
    }
    f = w->prm->flags;
    flip = w->f1F4 & 4;
    if (flip)
        f ^= 1;
    w->wC += w->h.gnd_vel >> 2;
    if (w->f1F4 & 4) {
        if (f & 2)
            w->h.gnd_vel = (s16)dcp_spd_up(w->h.gnd_vel, -0x30, 0xc00);
        else
            w->h.gnd_vel = (s16)dcp_spd_up(w->h.gnd_vel, 0x30, 0xc00);
    } else if (f & 1) {
        w->h.gnd_vel = (s16)dcp_spd_down(w->h.gnd_vel, 0xd);
        if (w->h.gnd_vel == 0)
            w->f1F4 |= 1;
    }
    if (w->f1F4 & 1) {
        if (f & 2) {
            w->h.gnd_vel = (s16)dcp_spd_up(w->h.gnd_vel, 0x30, 0xc00);
            if (w->wC > 0) {
                w->wC = 0;
                w->f1F4 |= 2;
                w->h.gnd_vel = (s16)-w->h.gnd_vel;
            }
        } else {
            w->h.gnd_vel = (s16)dcp_spd_up(w->h.gnd_vel, -0x30, 0xc00);
            if (w->wC < 0) {
                w->wC = 0;
                w->f1F4 |= 2;
                w->h.gnd_vel = (s16)-w->h.gnd_vel;
            }
        }
    } else {
        if (w->wC > 0x8000) {
            w->wC = 0x8000;
            w->f1F4 |= 2;
            w->f1F4 ^= 4;
        }
        if (w->wC < -0x8000) {
            w->wC = -0x8000;
            w->f1F4 |= 2;
            w->f1F4 ^= 4;
        }
    }
    a = (u16)w->wC;
    c = (obj_cos(a) * -0xc000) >> 12;
    x = (w->home_x << 8) + ((obj_sin(a) * 0xc000) >> 12);
    if (f & 1) {
        if (flip)
            y = (w->home_y << 8) + 0xc000 - c;
        else
            y = (w->home_y << 8) - 0xc000 - c;
    } else {
        if (flip)
            y = (w->home_y << 8) - 0xc000 + c;
        else
            y = (w->home_y << 8) + 0xc000 + c;
    }
    w->h.vel_x = (s16)(x - w->h.x);
    w->h.vel_y = (s16)(y - w->h.y);
}
/* Cargo_Draw: the cart, the five rope links from the pivot, the hook at the pivot */
static void Cargo_Draw(Obj *w) {
    s16 dx, dy, i;
    s32 ax = 0, ay = 0;
    obj_draw2d(w);
    dx = (s16)(FX_DivS32((w->k.cargo.u32C << 8) - w->h.x, 5) >> 8);
    dy = (s16)(FX_DivS32((w->k.cargo.u32E << 8) - w->h.y, 5) >> 8);
    for (i = 0; i < 5; i++) {
        w->k.cargo.pts[i][0] = (s16)(w->k.cargo.u32C - ax);
        w->k.cargo.pts[i][1] = (s16)(w->k.cargo.u32E - ay);
        ax += dx;
        ay += dy;
    }
    for (i = 0; i < 5; i++)
        obj_draw2d_ex(w->k.cargo.pts[i][0] << 8, w->k.cargo.pts[i][1] << 8, NULL, &w->k.cargo.a);
    w->k.cargo.a.multi = (const s16(*)[2])w->k.cargo.pts;
    w->k.cargo.a.n_multi = 5;
    obj_draw2d_ex(w->k.cargo.pts[0][0] << 8, w->k.cargo.pts[0][1] << 8, w, &w->k.cargo.c);
}
static void Cargo_OnHit(ObjBox *a, ObjBox *b) {
    Obj *w = (Obj *)a->owner;
    DcPlayer *p = obj_box_player(b);
    s32 v;
    if (w == NULL || p == NULL || p->type != 1)
        return;
    v = p->move_x;
    if ((v < 0 ? -v : v) < p->run_thr[1])
        return;
    if ((w->prm->flags & 2) || v <= 0) {
        if (!(w->prm->flags & 2))
            return;
        if (v >= 0)
            return;
    }
    Cargo_Action_StartMove(w, (s16)v); /* (called without a speed: r1 still holds the player's @0x28, checked in the ROM code) */
    if (w->prm->flags & 2)
        Player_Action_Cargo(p, w, 0x18, 0);
    else
        Player_Action_Cargo(p, w, -0x18, 0);
    g_obj.stats.cargo_rides++;
    count("cargo", w, p);
}
Obj *Cargo_Create(ObjParam *p, s32 b, s32 c) {
    Obj *w = mapobj(p, b, c, OBJ_BAC_CARGO, 0xf6, obj_mapobject_dtor);
    const u8 *map;
    u16 a;
    if (w == NULL)
        return NULL;
    w->spr.order = 8;
    set_prio(w, 0);
    w->fn_draw = Cargo_Draw;
    w->out_ofs += 0x18;
    spr_init(&w->k.cargo.a, &g_obj.res[OBJ_BAC_CARGO], 1, w->spr.flags);
    w->k.cargo.a.order = 7;
    spr_init(&w->k.cargo.c, &g_obj.res[OBJ_BAC_CARGO], 2, w->spr.flags);
    w->k.cargo.c.order = 0x14;
    w->xspr[0] = &w->k.cargo.a;
    w->xspr[1] = &w->k.cargo.c;
    w->n_xspr = 2;
    map = obj_df("gmk_cargo.df", NULL);
    cmap_set(w, map, 8, 10, -0x30, -0x28);
    rect_set_atkdef(&w->coll[0], 0, (u32)-16);
    w->coll[0].cb = Cargo_OnHit;
    w->coll[0].flags |= 0x800;
    w->h.move_flags |= 0x100;
    w->h.move_flags |= 0x2000;
    if (!(w->prm->flags & 2))
        w->h.disp_flags &= ~1;
    a = (u16)w->wC;
    w->k.cargo.u32C = (s16)(((w->home_x << 8) + ((obj_sin(a) * 0xc000) >> 12)) >> 8);
    if (w->h.disp_flags & 1)
        w->k.cargo.u32C += 0x20;
    else
        w->k.cargo.u32C -= 0x20;
    if (w->prm->flags & 1)
        w->k.cargo.u32E = (s16)(((w->home_y << 8) + ((obj_cos(a) * -0xc000) >> 12)) >> 8);
    else
        w->k.cargo.u32E = (s16)(((w->home_y << 8) - ((obj_cos(a) * -0xc000) >> 12)) >> 8);
    Cargo_Action_Stop(w);
    return w;
}

/* ======================================================= Hand2d (0x8a) */
static void Hand2d_State_Main(Obj *w) { /* Hand2d_State_Unk_ov000_0230b6a0 */
    ObjMtx33 m, r;
    DcPlayer *t, *p;
    u32 f;
    u16 a;
    if (w->f1F4 & 2) {
        w->h.move_flags |= 0x2000;
        w->h.vel_x = 0;
        w->h.vel_y = 0;
        w->h.move_x = 0;
        w->h.move_y = 0;
        w->h.move_z = 0;
        w->has_cmap = 1; /* unk1B8 = w */
        w->wC++;
        if (w->wC > 0x78) {
            w->f1F4 &= ~2u;
            if (w->f1F4 & 4) {
                w->f1F4 |= 1;
                w->h.move_flags &= ~0x2000u;
                w->wC = 0x8000;
            } else {
                w->wC = 0;
            }
        }
    } else if (w->target != NULL) {
        t = (DcPlayer *)w->target;
        if (t->gimmick != w)
            w->target = NULL;
        else
            w->w8 = (u16)((t->rot3 & 0xff) << 8);
    } else if (w->has_cmap && w->cmap.rider != NULL && w->cmap.rider->ride == &w->ref) {
        g_obj_camera[2].flags |= 0x40000;
        w->h.shake_y = 2; /* @0x7 */
        p = w->cmap.rider;
        if (p->type == 1) {
            w->target = p;
            Player_Action_Hand2D(p, w, 0, 0);
            w->f1F4 |= 1;
            w->h.move_flags &= ~0x2000u;
            w->has_cmap = 0; /* unk1B8 = NULL */
            count("hand2d", w, p);
        }
    } else {
        g_obj_camera[2].flags &= ~0x40000u;
        if (w->f1F4 & 1) {
            w->w8 = w->w8 - ((w->h.z >> 4) + 0x370);
            w->w8 &= 0xffff;
            w->has_cmap = 0;
        }
    }
    f = w->f1F4;
    if (!(f & 1))
        return;
    if (!(f & 4)) {
        w->wC += (w->h.z >> 8) + 0x200;
        if (w->wC > 0x8000) {
            w->wC = 0x8000;
            w->w8 = 0;
            w->f1F4 &= ~1u;
        }
    } else {
        w->wC -= (w->h.z >> 8) + 0x200;
        if (w->wC < 0) {
            w->wC = 0;
            w->w8 = 0;
            w->f1F4 &= ~1u;
        }
    }
    a = (u16)(-w->w8);
    MTX_Identity33_(&m);
    MTX_RotY33_(&r, obj_sin(a), obj_cos(a));
    MTX_Concat33(&m, &r, &m);
    mtx_store(w, &m);
    a = (u16)w->wC;
    w->h.vel_x = (s16)((w->home_x << 8) + 0x8000 + ((obj_cos(a) * -0x8000) >> 12) - w->h.x);
    w->h.vel_z = (s16)(((obj_sin(a) << 15) >> 12) - w->h.z);
    if (w->f1F4 & 1)
        return;
    w->wC = 0;
    w->h.z = 0;
    w->f1F4 ^= 4;
    w->f1F4 |= 2;
}
static void Hand2d_Destructor(Obj *p) {
    g_obj_camera[2].flags &= ~0x40000u;
    obj_mapobject_dtor(p);
}
Obj *Hand2d_Create(ObjParam *p, s32 b, s32 c) {
    Obj *w = mapobj(p, b, c, OBJ_BAC_HAND2D, OBJ_PRIO_MAPOBJ, Hand2d_Destructor);
    if (w == NULL)
        return NULL;
    w->spr.order = 0x14;
    /* nine StageObj_LoadModelSimple blocks of mod/gmk_hand.nsbmd: one model view */
    w->model = "mod__gmk_hand__gmk_hand";
    MTX_Identity33_((ObjMtx33 *)w->mtx);
    w->h.disp_flags |= 0x200;
    w->mscale[0] = w->mscale[1] = w->mscale[2] = 0x34cc;
    rect_set_box2d(&w->coll[0], -0xa4, -0x50, -0x80, 0x10);
    cmap_set(w, g_obj.data.solid, 6, 3, -0x18, 2);
    w->cmap.attr = g_obj.data.col_attr;
    w->h.move_flags |= 0x2100;
    w->target = NULL;
    /* Hand2d_Action_Init */
    w->state = Hand2d_State_Main;
    w->wC = 0;
    w->w8 = 0;
    w->h.vel_x = 0;
    w->h.vel_y = 0;
    w->h.z = 0;
    w->f1F4 &= ~7u;
    return w;
}

/* ===================================================== GmkTable (0x8b) */
static void GmkTable_State_Released(Obj *w);
static void GmkTable_Action_Release(Obj *w) {
    obj_sfx_own_stop(w, 0);   /* NNS_SndPlayerStopSeq(w->unk1DC, 0) */
    w->h.move_flags &= ~0x80u;
    w->h.vel_y = 0;
    w->h.y = CAM0.water << 8;
    w->state = GmkTable_State_Released;
    w->h.gnd_vel = 0x100;
    w->h.vel_x = 0;
    w->h.angle = 0;
    w->h.move_flags &= ~0x820000u;
    w->h.move_flags |= 0x100;
    w->k.tbl.u428 = 0x100;
    obj_log("table_release", 0x8b, w->h.x >> 8);
}
static void GmkTable_UpdateBob(Obj *w) {
    if (w->k.tbl.u428 > 0)
        w->k.tbl.u42A = (s16)dcp_spd_up(w->k.tbl.u42A, -1, 0xc0);
    else
        w->k.tbl.u42A = (s16)dcp_spd_up(w->k.tbl.u42A, 1, 0xc0);
    w->k.tbl.u428 = (s16)(w->k.tbl.u428 + w->k.tbl.u42A);
    if ((u32)w->w8 > 0x1a00u)
        w->k.tbl.u424 = (s16)dcp_spd_up(w->k.tbl.u424, -256, 0x30);
    else
        w->k.tbl.u424 = (s16)dcp_spd_up(w->k.tbl.u424, 0x100, 0x30);
    w->w8 += w->k.tbl.u424;
}
static void table_rot(Obj *w, u16 rx, u16 rz) {
    ObjMtx33 m, r;
    MTX_Identity33_(&m);
    MTX_RotX33_(&r, obj_sin(rx), obj_cos(rx));
    MTX_Concat33(&m, &r, &m);
    MTX_RotZ33_(&r, obj_sin(rz), obj_cos(rz));
    MTX_Concat33(&m, &r, &m);
    mtx_store(w, &m);
}
static int water_on(void) { return CAM0.water != 0 && CAM0.water != 0xffff; }
static void GmkTable_State_Main(Obj *w) {
    ObjMtx33 m, r;
    DcPlayer *ring, *p0 = player_n(0);
    Obj *eff;
    w->h.rot2++;
    ring = w->cmap.rider;
    if (ring != NULL && ring->ride == &w->ref && ring->type == 1) {
        w->h.move_flags &= ~0x2000u;
        w->target = ring;
        if (water_on())
            Player_AttachGimmickTableNoInput(ring, w);
        else
            Player_AttachGimmickTable(ring, w);
    }
    ring = (DcPlayer *)w->target;
    if (ring != NULL && ring->type == 1) {
        if (w->h.move_flags & 1) {
            ring->ride = &w->ref;
        } else if (water_on()) {
            ring->ride = &w->ref;
            Player_AttachGimmickTableNoInput(ring, w);
        } else if (!(w->k.tbl.u364 & 0x80) && (ring->ride == NULL || ring->ride != &w->ref)) {
            Player_DetachGimmick(ring, w);
            w->target = NULL;
        }
    } else if (water_on()) {
        if (ring != NULL)
            ring->ride = &w->ref; /* (the original writes through a possibly NULL pointer) */
        if (p0 != NULL)
            Player_AttachGimmickTableNoInput(p0, w);
    }
    if (w->h.gnd_vel < 0x100)
        w->h.gnd_vel = 0x100;
    table_rot(w, (u16)w->w8, (u16)-(w->h.angle << 8));
    MTX_Identity33_(&m);
    MTX_RotZ33_(&r, obj_sin(w->k.tbl.u41C), obj_cos(w->k.tbl.u41C));
    MTX_Concat33(&m, &r, &m);
    memcpy(w->k.tbl.mtx2, m.m, sizeof(w->k.tbl.mtx2));
    if (!(w->h.move_flags & 0x2000))
        w->k.tbl.u41C = (u16)(w->k.tbl.u41C - w->h.gnd_vel);
    w->k.tbl.u41E = w->k.tbl.u41C;
    if (!(w->h.move_flags & 0x2000))
        w->k.tbl.u420 += w->h.gnd_vel;
    if (water_on()) {
        if (!(w->k.tbl.u364 & 0x80)) {
            eff = EffZakoBomb_Create(&w->h, 0, 0);
            if (eff != NULL)
                eff->spr.flags = (eff->spr.flags & ~0x60000u) | (1u << 17);
            obj_sfx(100);
            w->k.tbl.u364 |= 0x80;
            w->h.vel_y = -0x400;
            w->h.move_flags |= 0x10;
        } else if (!(W.frame & 7)) {
            eff = EffZakoBomb_Create(&w->h, 0, 0);
            if (eff != NULL)
                eff->spr.flags = (eff->spr.flags & ~0x60000u) | (1u << 17);
            obj_sfx(100);
        }
    }
    if (w->k.tbl.u364 & 0x80) {
        w->h.angle = (u8)dcp_spd_down(w->h.angle, 1);
        w->w8 += 0x80;
        if (w->w8 > 0x1880)
            w->w8 = 0x1880;
        w->h.z += 0x180;
        if (w->h.z > 0xf00)
            w->h.z = 0xf00;
    }
    if (CAM0.water != 0 && w->h.y >= (CAM0.water << 8)) {
        GmkTable_Action_Release(w);
        return;
    }
    /* the engine loop: restarted on the table's handle whenever no 0x5e is playing */
    if (!(w->h.move_flags & 0x2000) && obj_sfx_count(0x5e) == 0)
        obj_sfx_own(w, 0x5e, 0x20);   /* Sound_PlaySeqArcBankPrio(w->unk1DC, 0, 0x5e, 0x20, -1) */
    if (p0 != NULL && (p0->pf & 0x400))
        w->f1F4 |= 1;
    if (w->f1F4 & 1) {
        if (p0 != NULL && !(p0->pf & 0x400))
            w->h.obj_flags |= 4;
    }
}
static void GmkTable_State_Released(Obj *w) {
    DcPlayer *ring, *p0 = player_n(0);
    GmkTable_UpdateBob(w);
    if ((w->h.x >> 8) >= g_obj.map_w - 0x180)
        w->h.gnd_vel = 0;
    w->k.tbl.u426++;
    ring = w->cmap.rider;
    if (ring != NULL && ring->ride == &w->ref && ring->type == 1) {
        w->h.move_flags &= ~0x2000u;
        w->target = ring;
        Player_AttachGimmickTableReleased(ring, w);
    }
    if (w->target != NULL) {
        ring = (DcPlayer *)w->target;
        ring->ride = &w->ref;
        if (ring->type == 1 && (w->cmap.rider == NULL || w->cmap.rider->ride != &w->ref) && !(W.objmgr & 0x100))
            Player_AttachGimmickTableNoInput(ring, w);
    }
    table_rot(w, (u16)w->w8, (u16)w->k.tbl.u428);
    if (p0 != NULL && (p0->pf & 0x400))
        w->f1F4 |= 1;
    if (w->f1F4 & 1) {
        if (p0 != NULL && !(p0->pf & 0x400))
            w->h.obj_flags |= 4;
    }
}
static void GmkTable_Destructor(Obj *p) {
    if (s_gmk_table == p)
        s_gmk_table = NULL;
    obj_mapobject_dtor(p);
}
Obj *GmkTable_Create(ObjParam *p, s32 b, s32 c) {
    Obj *w = mapobj(p, b, c, -1, OBJ_PRIO_MAPOBJ, GmkTable_Destructor);
    if (w == NULL)
        return NULL;
    s_gmk_table = w;
    /* three StageObj_LoadModelSimple blocks of mod/gmk_table.nsbmd: the hull
     * (matrix @0x128) is the model view; block 1 (@0x2BC, k.tbl.mtx2) turns */
    w->model = "mod__gmk_table__gmk_table";
    MTX_Identity33_((ObjMtx33 *)w->mtx);
    w->h.disp_flags |= 0x200;
    w->mscale[0] = w->mscale[1] = w->mscale[2] = 0x34cc;
    w->spr.order = 0x14;
    map_hitbox(w, -8, -10, 8, 0xc);
    cmap_set(w, g_obj.data.solid, 0x20, 1, -0x80, -0x14);
    w->cmap.flags |= 1;
    w->h.move_flags |= 0x2040;
    w->h.move_flags |= 0x820080;
    w->h.disp_flags &= ~1;
    w->h.move_flags |= 0x400;
    w->h.obj_flags &= ~1u;
    w->h.slope_lim = 0;
    w->h.slope_acc = 0x40;
    w->h.gnd_max = 0x300;
    w->target = NULL;
    w->snd = 1;
    w->state = GmkTable_State_Main; /* GmkTable_Action_Init */
    w->h.gnd_vel = 0x100;
    return w;
}

/* ============================================= enemy rooms (0x8c..0x8f) */
/* EnemyRoom_* (gEnemyRoomTable, gCamera.unk10C) and Gimmick_* helpers */
static void EnemyRoom_Register(Obj *v, u8 i) { s_enemy_room[i & 7] = v; }
static void EnemyRoom_Unregister(u8 i) { s_enemy_room[i & 7] = NULL; }
static Obj *EnemyRoom_Get(u8 i) { return s_enemy_room[i & 7]; }
static void EnemyRoom_SetActive(u8 v) { s_enemy_room_active = v; }
static void EnemyRoom_ClearActive(u8 v) {
    if (s_enemy_room_active == v)
        s_enemy_room_active = 0xff;
}
static u8 Gimmick_GetRoomIndex(const Obj *s) { return (u8)((s->prm->flags & 0xe0) >> 5); }
static dcp_bool Gimmick_CheckRoomResetOnDeath(Obj *t, s32 inc) {
    DcPlayer *p;
    u32 v;
    if ((g_obj_camera[2].flags & 1) || t == NULL)
        return DCP_FALSE;
    p = player_n(t->k.room.pidx[0]);
    if (p == NULL)
        return DCP_FALSE;
    v = p->pf & 0x400;
    if ((t->k.room.f20C & 0x10000000) && v == 0)
        return DCP_TRUE;
    if (v != 0) {
        if (inc != 0)
            t->k.room.u230++;
        t->k.room.f20C |= 0x10000000;
    }
    return DCP_FALSE;
}
static void cam_lock(Obj *w, int on) { /* gCamera[i].unk34 bit 0x10000 */
    int i;
    for (i = 0; i < 2; i++) {
        if ((g_obj_camera[2].flags & 1) || (w->k.room.f20C & (2u << i))) {
            if (on)
                g_obj_camera[i].flags34 |= 0x10000;
            else
                g_obj_camera[i].flags34 &= ~0x10000u;
        }
    }
}
/* MapObj8C: the room controller */
static void MapObj8C_State_Idle(Obj *w) { (void)w; }
static void MapObj8C_State_FinishRoom(Obj *p) {
    if (EnemyRoom_Get(p->k.room.room) == NULL)
        return;
    EnemyRoom_ClearActive(p->k.room.room);
    p->state = MapObj8C_State_Idle;
}
static void MapObj8C_Action_RoomCleared(Obj *w) {
    w->k.room.f20C &= ~8u;
    w->k.room.f20C |= 0x2000;
    w->k.room.f20C |= 0x40;
    cam_lock(w, 0);
    if (!(g_obj_camera[2].flags & 1))
        EnemyRoom_ClearActive(w->k.room.room);
    w->f1F4 |= 0x10;
    w->state = MapObj8C_State_FinishRoom;
    g_obj.stats.enemy_rooms++;
    if (g_obj_hooks.effect)
        g_obj_hooks.effect(g_obj_hooks.ctx, NULL, OBJ_FX_ENEMY_ROOM, 2, w->k.room.got);
    obj_log("enemy_room_cleared", w->k.room.room, w->k.room.got);
}
static void MapObj8C_State_RoomActive(Obj *w) {
    u8 i;
    dcp_bool all;
    u32 f;
    if (Gimmick_CheckRoomResetOnDeath(w, 1)) {
        cam_lock(w, 0);
        EnemyRoom_ClearActive(w->k.room.room);
        w->f1F4 &= ~0x10u;
        w->h.obj_flags |= 4;
        return;
    }
    f = w->k.room.f20C;
    if (!(f & 0x1000000)) {
        all = DCP_TRUE;
        for (i = 0; i < 2; i = (u8)(i + 1)) {
            if ((f & (2u << i)) && (0x550000u << i) != (f & (0x550000u << i))) {
                all = DCP_FALSE;
                break;
            }
        }
        if (all)
            w->k.room.f20C |= 0x1000000;
    }
    if ((w->k.room.f20C & 0x1000020) == 0x1000020)
        w->k.room.f20C |= 0x100;
    if (w->k.room.got < w->k.room.need)
        return;
    MapObj8C_Action_RoomCleared(w);
}
static void MapObj8C_Action_StartRoom(Obj *w) {
    u8 i;
    DcPlayer *p;
    s32 px, py;
    EnemyRoom_SetActive(w->k.room.room);
    w->k.room.f20C |= 8;
    if (!(g_obj_camera[2].flags & 1)) {
        /* StageEvent_SpawnInRect(x +- 0x300, y +- 0x300): spawn the room's
         * objects now (native: the view-based spawner, objects_spawn_rect) */
        objects_spawn_rect((w->h.x >> 8) - 0x300, (w->h.y >> 8) - 0x300, (w->h.x >> 8) + 0x300, (w->h.y >> 8) + 0x300);
        w->k.room.room = Gimmick_GetRoomIndex(w);
        EnemyRoom_Register(w, w->k.room.room);
    }
    w->k.room.f20C |= 0x1000;
    w->k.room.f20C |= 0x10;
    for (i = 0; i < 2; i = (u8)(i + 1)) {
        if (w->k.room.pidx[i] >= 0 && (p = player_n(w->k.room.pidx[i])) != NULL) {
            px = p->x >> 8;
            if (w->k.room.b216 <= px && px <= w->k.room.b21A) {
                py = p->y >> 8;
                if (w->k.room.b218 <= py && py <= w->k.room.b21C)
                    w->k.room.f20C |= 2u << i;
            }
        }
    }
    if ((w->k.room.f20C & 2) && g_obj_hooks.effect) /* EnemyRoomHud_Create */
        g_obj_hooks.effect(g_obj_hooks.ctx, NULL, OBJ_FX_ENEMY_ROOM, 0, w->k.room.need);
    cam_lock(w, 1);
    w->state = MapObj8C_State_RoomActive;
    obj_log("enemy_room_start", w->k.room.room, w->k.room.need);
}
static void MapObj8C_State_WaitPlayerEnter(Obj *w) {
    DcPlayer *p;
    u8 i;
    s32 x, y;
    if (!(w->k.room.f20C & 0x200) && w->k.room.b216 != 0xffff && w->k.room.b218 != 0xffff &&
        w->k.room.b21A != 0xffff && w->k.room.b21C != 0xffff)
        w->k.room.f20C |= 0x200;
    if (!(w->k.room.f20C & 0x200))
        return;
    for (i = 0; i < 2; i = (u8)(i + 1)) {
        if (w->k.room.pidx[i] < 0 || (p = player_n(w->k.room.pidx[i])) == NULL)
            continue;
        if (p->z != 0)
            continue;
        x = p->x;
        y = p->y;
        if (w->k.room.b216 << 8 <= x && x <= w->k.room.b21A << 8 && w->k.room.b218 << 8 <= y &&
            y <= w->k.room.b21C << 8 && ((g_obj_camera[2].flags & 1) || !(p->pf & 0x400))) {
            if (x < w->h.x) {
                if (w->k.room.f20C & 1) {
                    MapObj8C_Action_StartRoom(w);
                    return;
                }
            } else if (!(w->k.room.f20C & 1)) {
                MapObj8C_Action_StartRoom(w);
                return;
            }
        }
    }
}
static void MapObj8C_Destructor(Obj *s) {
    EnemyRoom_Unregister(s->k.room.room);
    obj_mapobject_dtor(s);
}
static Obj *room_obj(ObjParam *p, s32 b, s32 c, ObjFn dtor) {
    Obj *w = mapobj(p, b, c, -1, OBJ_PRIO_MAPOBJ, dtor);
    if (w == NULL)
        return NULL;
    w->kind = OBJK_TRIGGER;
    rect_set_atkdef(&w->coll[0], 0, (u32)-16);
    w->coll[0].cb = NULL;
    w->coll[0].flags |= 0x800;
    w->out_ofs = 0x300;
    w->h.move_flags |= 0x2100;
    return w;
}
static Obj *MapObj8C_Create(ObjParam *p, s32 b, s32 c) {
    Obj *w = obj_alloc(0x4f6, MapObj8C_Destructor);
    if (w == NULL)
        return NULL;
    obj_mapobject_init(w, p, b, c);
    w->kind = OBJK_TRIGGER;
    rect_set_atkdef(&w->coll[0], 0, (u32)-16);
    w->coll[0].cb = NULL;
    w->coll[0].flags |= 0x800;
    w->out_ofs = 0x300;
    w->h.move_flags |= 0x2100;
    w->h.disp_flags |= 0x20;
    w->k.room.b216 = 0xffff;
    w->k.room.b218 = 0xffff;
    w->k.room.b21A = 0xffff;
    w->k.room.b21C = 0xffff;
    w->k.room.need = w->prm->p6;
    if (w->prm->flags & 1)
        w->k.room.f20C |= 1;
    w->k.room.pidx[0] = (s8)g_obj_camera[0].player;
    w->k.room.pidx[1] = -1;
    w->k.room.room = Gimmick_GetRoomIndex(w);
    EnemyRoom_Register(w, w->k.room.room);
    w->state = MapObj8C_State_WaitPlayerEnter;
    return w;
}
/* MapObj8D: an enemy spawner of the room */
static void MapObj8D_State_Done(Obj *p) {
    Obj *t1 = EnemyRoom_Get(Gimmick_GetRoomIndex(p));
    if (t1 == NULL) {
        p->h.obj_flags |= 4;
        return;
    }
    if (Gimmick_CheckRoomResetOnDeath(t1, 0)) {
        p->f1F4 &= ~0x10u;
        p->h.obj_flags |= 4;
        return;
    }
    if (t1->k.room.f20C & 8)
        return;
    p->f1F4 |= 0x10;
    p->h.obj_flags |= 4;
}
static void MapObj8D_State_WaitEnemyDefeated(Obj *p) {
    Obj *t0 = (Obj *)p->target, *t2 = EnemyRoom_Get(Gimmick_GetRoomIndex(p));
    if (t2 == NULL) {
        p->h.obj_flags |= 4;
        return;
    }
    if (Gimmick_CheckRoomResetOnDeath(t2, 0)) {
        p->f1F4 &= ~0x10u;
        p->h.obj_flags |= 4;
        if (t0 != NULL && t0->used)
            t0->h.obj_flags |= 4;
        return;
    }
    if (t0 != NULL && t0->used && !(t0->h.obj_flags & 4))
        return;
    t2->k.room.got++;
    if (g_obj_hooks.effect) /* EnemyRoomHud count */
        g_obj_hooks.effect(g_obj_hooks.ctx, NULL, OBJ_FX_ENEMY_ROOM, 1, t2->k.room.got);
    p->target = NULL;
    p->state = MapObj8D_State_Done;
}
static void MapObj8D_State_WaitSpawn(Obj *p) {
    Obj *t1 = EnemyRoom_Get(Gimmick_GetRoomIndex(p)), *e;
    if (Gimmick_CheckRoomResetOnDeath(t1, 0)) {
        p->f1F4 &= ~0x10u;
        p->h.obj_flags |= 4;
        return;
    }
    if (t1 == NULL)
        return;
    if (!(t1->k.room.f20C & 0x100))
        return;
    if (t1->k.room.got < p->prm->p9)
        return;
    p->wC--;
    if (p->wC != 0)
        return;
    /* MapObj8D_Action_SpawnEnemy */
    e = obj_mapobject_spawn((u8)p->prm->p7, p->h.x, p->h.y, (s16)(u16)(p->prm->flags | 0x40), p->prm->p6, 0, p->prm->p8, 0);
    p->target = e;
    if (e != NULL)
        e->h.obj_flags |= 0x10;
    else
        obj_log("room_spawn_untranslated", (u8)p->prm->p7, p->h.x >> 8);
    p->state = MapObj8D_State_WaitEnemyDefeated;
}
static Obj *MapObj8D_Create(ObjParam *p, s32 b, s32 c) {
    Obj *w = room_obj(p, b, c, obj_mapobject_dtor);
    if (w == NULL)
        return NULL;
    w->spr.order = 0x14;
    w->h.disp_flags |= 0x20;
    w->wC = 0xf;
    w->state = MapObj8D_State_WaitSpawn;
    return w;
}
/* GmkWall (0x8e): the room gate */
static void GmkWall_State_Opened(Obj *p) {
    if (!Gimmick_CheckRoomResetOnDeath(EnemyRoom_Get(Gimmick_GetRoomIndex(p)), 0))
        return;
    p->f1F4 &= ~0x10u;
    p->h.obj_flags |= 4;
}
static void GmkWall_State_MoveUp(Obj *s) {
    if (Gimmick_CheckRoomResetOnDeath(EnemyRoom_Get(Gimmick_GetRoomIndex(s)), 0)) {
        s->f1F4 &= ~0x10u;
        s->h.obj_flags |= 4;
        return;
    }
    s->wC--;
    if (!obj_gravity_flipped(s->h.x, s->h.y))
        s->h.y = s->w8 + (s->wC << 14) / 60;
    else
        s->h.y = s->w8 - (s->wC << 14) / 60;
    if (s->wC == 0)
        s->state = GmkWall_State_Opened; /* GmkWall_Action_Opened */
}
static void GmkWall_State_Closed(Obj *s) {
    Obj *p = EnemyRoom_Get(Gimmick_GetRoomIndex(s));
    if (Gimmick_CheckRoomResetOnDeath(p, 0)) {
        s->f1F4 &= ~0x10u;
        s->h.obj_flags |= 4;
        return;
    }
    if (p == NULL) {
        s->h.obj_flags |= 4;
        return;
    }
    if (!(p->k.room.f20C & 0x40))
        return;
    /* GmkWall_Action_MoveUp: the collision map goes away (@0x1B8 = @0x1C8 = 0) */
    s->has_cmap = 0;
    s->cmap.map = NULL;
    s->wC = 0x3c;
    s->state = GmkWall_State_MoveUp;
}
static void GmkWall_Action_Closed(Obj *p) {
    Obj *o = EnemyRoom_Get(Gimmick_GetRoomIndex(p));
    if (o == NULL) {
        p->h.obj_flags |= 4;
    } else {
        o->k.room.f20C |= 0x20;
        p->state = GmkWall_State_Closed;
    }
}
static void GmkWall_State_MoveDown(Obj *s) {
    if (Gimmick_CheckRoomResetOnDeath(EnemyRoom_Get(Gimmick_GetRoomIndex(s)), 0)) {
        s->f1F4 &= ~0x10u;
        s->h.obj_flags |= 4;
        return;
    }
    s->wC--;
    if (!obj_gravity_flipped(s->h.x, s->h.y)) {
        s->h.y = s->w8 + ((0x28 - s->wC) << 14) / 40;
        s->cmap.oy = (s8)((s->wC << 6) / 40 - 0x20);
    } else {
        s->h.y = s->w8 - ((0x28 - s->wC) << 14) / 40;
        s->cmap.oy = (s8)(-0x20 - (s->wC << 6) / 40);
    }
    if (s->wC == 0)
        GmkWall_Action_Closed(s);
}
static void GmkWall_State_WaitRoomStart(Obj *p) {
    Obj *t1 = EnemyRoom_Get(Gimmick_GetRoomIndex(p));
    if (Gimmick_CheckRoomResetOnDeath(t1, 0)) {
        p->f1F4 &= ~0x10u;
        p->h.obj_flags |= 4;
        return;
    }
    if (t1 == NULL)
        return;
    if (t1->k.room.f20C & 0x40)
        return;
    if (!(t1->k.room.f20C & 0x10))
        return;
    /* GmkWall_Action_MoveDown */
    p->wC = 0x28;
    if (!obj_gravity_flipped(p->h.x, p->h.y))
        p->cmap.oy = 0x20;
    else
        p->cmap.oy = -0x60;
    p->state = GmkWall_State_MoveDown;
}
static Obj *GmkWall_Create(ObjParam *p, s32 b, s32 c) {
    Obj *w = room_obj(p, b, c, obj_mapobject_dtor);
    if (w == NULL)
        return NULL;
    obj_load_bac(w, OBJ_BAC_GMK_WALL);
    w->kind = OBJK_GIMMICK;
    w->spr.order = 0x14;
    cmap_set(w, g_obj.data.solid, 4, 8, -0x10, -0x20);
    w->w8 = w->h.y;
    obj_set_anim(w, 0); /* GmkWall_Action_Init */
    w->state = GmkWall_State_WaitRoomStart;
    return w;
}
/* MapObj8F: room bounds (flags & 1 clear) / camera bounds (flags & 1 set) */
static void MapObj8F_State_RoomBoundDone(Obj *p) {
    Obj *t1 = EnemyRoom_Get(Gimmick_GetRoomIndex(p));
    if (t1 == NULL) {
        p->h.obj_flags |= 4;
        return;
    }
    if (Gimmick_CheckRoomResetOnDeath(t1, 0)) {
        p->f1F4 &= ~0x10u;
        p->h.obj_flags |= 4;
        return;
    }
    if (t1->k.room.f20C & 8)
        return;
    p->f1F4 |= 0x10;
    p->h.obj_flags |= 4;
}
static void MapObj8F_State_SetRoomBound(Obj *s) {
    Obj *t = EnemyRoom_Get(Gimmick_GetRoomIndex(s));
    if (Gimmick_CheckRoomResetOnDeath(t, 0)) {
        s->f1F4 &= ~0x10u;
        s->h.obj_flags |= 4;
        return;
    }
    if (t == NULL)
        return;
    if (!(s->prm->flags & 2)) {
        t->k.room.b216 = (u16)(s->h.x >> 8);
        t->k.room.b218 = (u16)(s->h.y >> 8);
    } else {
        t->k.room.b21A = (u16)(s->h.x >> 8);
        t->k.room.b21C = (u16)(s->h.y >> 8);
    }
    if (t->k.room.f20C & 8)
        s->state = MapObj8F_State_RoomBoundDone; /* MapObj8F_Action_RoomBoundDone */
}
/* Camera_GetTrackedPlayerOffset: (player - camera) / view size, FX_Div >> 4 */
static void Camera_GetTrackedPlayerOffset(int i, s32 *ox, s32 *oy) {
    ObjCamera *c = &g_obj_camera[i];
    DcPlayer *p = player_n(c->player);
    s64 a, b;
    if (p == NULL || c->w == 0 || c->h == 0) {
        *ox = *oy = 0;
        return;
    }
    a = ((s64)(p->x - c->x) << 12) / c->w;
    b = ((s64)(p->y - c->y) << 12) / c->h;
    *ox = (s32)a >> 4;
    *oy = (s32)b >> 4;
}
static void MapObj8F_State_CameraRelease(Obj *s) {
    Obj *o = EnemyRoom_Get(Gimmick_GetRoomIndex(s));
    ObjCamera *pp;
    s32 x;
    u8 i;
    if (o == NULL) {
        s->h.obj_flags |= 4;
        return;
    }
    if (Gimmick_CheckRoomResetOnDeath(o, 0)) {
        s->h.obj_flags |= 4;
        return;
    }
    s->wC++;
    /* (single-screen branch, gCamera[2].flags & 1, not used on the DC) */
    for (i = 0; i < 2; i++) {
        if (s->k.mo8f.u224 & (1u << i)) {
            pp = &g_obj_camera[i];
            x = s->k.mo8f.pos[i][0];
            pp->ofs_x = x + (pp->def_ofs_x - x) * s->wC / 20;
            x = s->k.mo8f.pos[i][1];
            pp->ofs_y = x + (pp->def_ofs_y - x) * s->wC / 20;
        }
    }
    if (s->wC < 20)
        return;
    if (s->k.mo8f.u224 & 1)
        g_obj_camera[0].flags &= ~0x2000u;
    if (s->k.mo8f.u224 & 2)
        g_obj_camera[1].flags &= ~0x2000u;
    s->f1F4 |= 0x10;
    s->h.obj_flags |= 4;
}
/* MapObj8F_Action_StartCameraRelease (src/asm_fallback, translated from the ARM listing) */
static void MapObj8F_Action_StartCameraRelease(Obj *s) {
    Obj *o = EnemyRoom_Get(Gimmick_GetRoomIndex(s));
    ObjCamera *c;
    u8 i;
    s->f1F4 |= 0x10;
    if (o == NULL) {
        s->h.obj_flags |= 4;
        return;
    }
    if (s->prm->flags & 2) {
        s->h.obj_flags |= 4;
        return;
    }
    for (i = 0; i < 2; i++) {
        if (!(o->k.room.f20C & (2u << i)))
            continue;
        s->k.mo8f.u224 |= 1u << i;
        c = &g_obj_camera[i];
        if (g_obj_camera[2].flags & 1) {
            c->flags |= 0x100000;
            c->flags |= 0x200000;
            c->flags &= ~0x400000u;
            c->flags &= ~0x800000u;
            g_obj_camera[2].flags &= ~8u;
            g_obj_camera[2].flags |= 4;
            c->flags |= 0x200;
        } else {
            c->flags |= 0x2000;
            Camera_GetTrackedPlayerOffset(i, &s->k.mo8f.pos[i][0], &s->k.mo8f.pos[i][1]);
            c->ofs_x = s->k.mo8f.pos[i][0];
            c->ofs_y = s->k.mo8f.pos[i][1];
        }
        c->bound[0] = g_obj_camera[2].bound[0];
        c->bound[1] = g_obj_camera[2].bound[1];
        c->bound[2] = g_obj_camera[2].bound[2];
        c->bound[3] = g_obj_camera[2].bound[3];
    }
    s->state = MapObj8F_State_CameraRelease;
}
static void MapObj8F_State_CameraLocked(Obj *s) {
    Obj *t = EnemyRoom_Get(Gimmick_GetRoomIndex(s));
    if (t == NULL) {
        s->h.obj_flags |= 4;
        return;
    }
    if (Gimmick_CheckRoomResetOnDeath(t, 0)) {
        s->f1F4 &= ~0x10u;
        s->h.obj_flags |= 4;
        return;
    }
    if (t->k.room.f20C & 0x2000)
        MapObj8F_Action_StartCameraRelease(s);
}
static void MapObj8F_State_CameraLock(Obj *s) {
    Obj *o = EnemyRoom_Get(Gimmick_GetRoomIndex(s));
    ObjCamera *p;
    u8 i;
    u32 f, m;
    s32 v;
    if (o == NULL) {
        s->h.obj_flags |= 4;
        return;
    }
    if (Gimmick_CheckRoomResetOnDeath(o, 0)) {
        s->f1F4 &= ~0x10u;
        s->h.obj_flags |= 4;
        return;
    }
    for (i = 0; i < 2; i++) {
        f = o->k.room.f20C;
        if (!(f & (2u << i)))
            continue;
        p = &g_obj_camera[i];
        if (!(s->prm->flags & 2)) {
            m = 0x10000u << i;
            if (!(f & m)) {
                if (p->x + 0x1000 >= s->k.mo8f.u20C) {
                    p->bound[0] = s->k.mo8f.u20C;
                    o->k.room.f20C |= m;
                } else {
                    p->bound[0] = p->x + 0x1000;
                }
            }
            m = 0x40000u << i;
            if (!(o->k.room.f20C & m)) {
                v = p->y + 0x1000;
                if (v < s->h.y) {
                    p->bound[1] = v;
                } else {
                    p->bound[1] = s->k.mo8f.u210;
                    o->k.room.f20C |= m;
                }
            }
        } else {
            m = 0x100000u << i;
            if (!(f & m)) {
                v = p->x + (p->w << 8) - 0x1000;
                if (v <= s->k.mo8f.u20C) {
                    p->bound[2] = s->k.mo8f.u20C;
                    o->k.room.f20C |= m;
                } else {
                    p->bound[2] = v;
                }
            }
            m = 0x400000u << i;
            if (!(o->k.room.f20C & m)) {
                v = p->y + p->h * 0xc0 - 0x1000;
                if (v <= s->h.y) {
                    p->bound[3] = s->k.mo8f.u210;
                    o->k.room.f20C |= m;
                } else {
                    p->bound[3] = v;
                }
            }
        }
    }
    if (o->k.room.f20C & 0x1000000)
        s->state = MapObj8F_State_CameraLocked; /* MapObj8F_Action_CameraLocked (dual-screen branch) */
}
static void MapObj8F_State_SetCameraBound(Obj *s) {
    Obj *t = EnemyRoom_Get(Gimmick_GetRoomIndex(s));
    if (t != NULL) {
        if (!(s->prm->flags & 2)) {
            t->k.room.c220 = s->k.mo8f.u20C;
            t->k.room.c224 = s->k.mo8f.u210;
        } else {
            t->k.room.c228 = s->k.mo8f.u20C;
            t->k.room.c22C = s->k.mo8f.u210;
        }
    }
    if (t == NULL)
        return;
    if (!(t->k.room.f20C & 0x1000))
        return;
    s->state = MapObj8F_State_CameraLock; /* MapObj8F_Action_StartCameraLock (dual-screen branch) */
}
static Obj *MapObj8F_Create(ObjParam *p, s32 b, s32 c) {
    Obj *w = room_obj(p, b, c, obj_mapobject_dtor);
    if (w == NULL)
        return NULL;
    w->spr.order = 0x14;
    w->h.disp_flags |= 0x20;
    if (w->prm->flags & 1) {
        /* MapObj8F_Action_InitCameraBound */
        w->k.mo8f.u20C = w->h.x;
        w->k.mo8f.u210 = w->h.y;
        w->state = MapObj8F_State_SetCameraBound;
    } else {
        w->state = MapObj8F_State_SetRoomBound;
    }
    return w;
}
Obj *MapObj8C_CreateByType(ObjParam *p, s32 a, s32 b) {
    switch (p->type) {
    case 0x8c: return MapObj8C_Create(p, a, b);
    case 0x8d: return MapObj8D_Create(p, a, b);
    case 0x8e: return GmkWall_Create(p, a, b);
    case 0x8f: return MapObj8F_Create(p, a, b);
    }
    return NULL;
}

/* ================================================== EffEntrance (src/game/Effects) */
static void EffEntrance_State_ScaleInOut(Obj *w) {
    if (w->parent != NULL) {
        w->scale_x = (s16)ObjShift_Set(w->scale_x, 0x100, 2, 0, 8);
        w->scale_y = (s16)ObjShift_Set(w->scale_y, 0x100, 2, 0, 8);
        if (w->scale_x == 0x100 && !obj_pawn_in_entrance(w->parent))
            w->parent = NULL;
    }
    if (w->parent != NULL)
        return;
    w->scale_x = (s16)ObjShift_Set(w->scale_x, 0, 2, 0, 8);
    w->scale_y = (s16)ObjShift_Set(w->scale_y, 0x200, 2, 0, 8);
    if (w->scale_x == 0)
        w->h.obj_flags |= 4;
}
void EffEntrance_Create(Obj *par, s32 dx, s32 dy) {
    Obj *w;
    if (W.flags & 0x20)
        return;
    w = eff_alloc(OBJ_BAC_EFF_ENTRANCE);
    if (w == NULL)
        return;
    w->spr.order = 0x15;
    w->spr.flags |= 0x200;
    w->parent = par;
    w->h.x = par->h.x + (dx << 8);
    w->h.y = par->h.y + (dy << 8);
    w->scale_x = 1; /* (0 means unscaled natively: the smallest visible size) */
    w->scale_y = 0x200;
    w->h.disp_flags |= 4;
    w->h.move_flags |= 0x2100;
    w->h.obj_flags |= 0x202;
    w->state = EffEntrance_State_ScaleInOut;
}

/* ================================================ SandSlider (0x90, 0x91) */
static void SandSlider_State_Idle(Obj *w) { (void)w; }
static void SandSliderPart_State_Idle(Obj *w) { (void)w; }
static void SandSlider_CreatePart(Obj *arg) {
    Obj *w = eff_alloc(OBJ_BAC_SAND_SLIDER);
    if (w == NULL)
        return;
    obj_set_anim(w, 1);
    w->spr.order = 0x15;
    w->h.disp_flags |= 1;
    w->parent = arg;
    w->h.x = w->parent->h.x;
    w->h.y = w->parent->h.y + 0x2c00;
    w->h.move_flags |= 0x2100;
    w->h.obj_flags |= 0x12;
    w->state = SandSliderPart_State_Idle;
}
static void SandSlider_OnHit(ObjBox *a, ObjBox *b) {
    Obj *t0 = (Obj *)a->owner;
    DcPlayer *t1 = obj_box_player(b);
    if (t0 == NULL || t1 == NULL)
        return;
    if (t0->prm->type == 0x90)
        Player_Action_SandSlider90(t1, t0);
    else
        Player_Action_SandSlider(t1, t0);
    g_obj.stats.sand_slides++;
    count("sand_slider", t0, t1);
}
Obj *SandSlider_Create(ObjParam *a, s32 b, s32 c) {
    Obj *w = mapobj(a, b, c, OBJ_BAC_SAND_SLIDER, OBJ_PRIO_MAPOBJ, obj_mapobject_dtor);
    if (w == NULL)
        return NULL;
    w->spr.order = 9;
    set_prio(w, 1);
    if (a->type == 0x90) {
        cmap_set(w, g_obj.data.solid, 6, 5, -0x18, -0x14);
        SandSlider_CreatePart(w);
    } else {
        w->h.z = 0x4000;
    }
    rect_set_atkdef(&w->coll[0], 0, (u32)-16);
    w->coll[0].cb = SandSlider_OnHit;
    w->coll[0].flags |= 0x800;
    w->h.move_flags |= 0x2100;
    w->state = SandSlider_State_Idle; /* SandSlider_Action_Idle */
    return w;
}

/* ======================================================== Rock (0x92) */
static void Rock_State_Fall(Obj *w) {
    if (w->h.move_flags & 0x100)
        return;
    if (!(w->h.move_flags & 1))
        return;
    obj_sfx(0x5f);
    w->h.hitstop |= 0x44;
    w->h.move_flags |= 0x100;
    w->h.move_flags &= ~1u;
    w->h.vel_x = (s16)((obj_sin((u16)(w->h.angle << 8)) * 0x280) >> 12);
    w->h.vel_y = -0x480;
    w->out_ofs = 0x20;
    g_obj.stats.rocks_fallen++;
    obj_log("rock_land", 0x92, w->h.x >> 8);
}
static void Rock_State_Wait(Obj *w) {
    if (w->wC & 0x10)
        w->h.vel_y = (s16)dcp_spd_up(w->h.vel_y, -16, 0x40);
    else
        w->h.vel_y = (s16)dcp_spd_up(w->h.vel_y, 0x10, 0x40);
    if (w->wC != 0) {
        w->wC--;
        if (w->wC <= 0xc)
            w->h.vel_y = 0;
        if (w->wC == 8)
            w->h.hitstop |= 0x80;
        return;
    }
    /* Rock_Action_Fall */
    w->state = Rock_State_Fall;
    w->h.vel_y = 0;
    w->h.move_flags &= ~0x2000u;
    w->h.move_flags |= 0x80;
    w->h.move_flags |= 0x40;
    w->h.move_flags &= ~0x100u;
}
Obj *Rock_Create(ObjParam *p, s32 b, s32 c) {
    Obj *w;
    if (p->flags & 0x80) {
        if (g_obj_stage.save_flags & 1)
            return NULL;
    }
    if ((b >> 8) < W.map_left + 0xc4)
        return NULL;
    w = mapobj(p, b, c, OBJ_BAC_ROCK, OBJ_PRIO_MAPOBJ, obj_mapobject_dtor);
    if (w == NULL)
        return NULL;
    w->kind = OBJK_ENEMY;
    w->out_t = -0x100;
    /* the original hides this 2D sprite (0x80000) and draws ac_gmk_rock_3.bac as a 3D
     * BAC at depth @0x18: Obj.bac3d 2 (obj_draw.c draws the 3D BAC's frame) */
    w->bac3d = 2;
    map_hitbox(w, -8, 0, 8, 0xc);
    if (w->prm->p7 < 0x28)
        w->h.z = -(w->prm->p7 << 8);
    w->h.move_flags |= 0x2100;
    w->h.move_flags &= ~0x200u;
    w->snd = 1;
    /* Rock_Action_Wait */
    w->h.obj_flags &= ~1u;
    w->h.move_flags &= ~0x2000u;
    w->h.move_flags &= ~0x80u;
    w->h.move_flags |= 0x1500;
    w->wC = w->prm->p9;
    w->state = Rock_State_Wait;
    EffShadow_Create(&w->h);
    return w;
}

/* ===================================================== GmkRail (0x88) */
static void GmkRail_StartFromPlayerSpeed(Obj *w, DcPlayer *p) {
    s32 v;
    w->h.gnd_vel = p->move_x;
    v = w->h.gnd_vel;
    if (v < 0)
        v = -v;
    if (p->run_thr[2] <= v)
        w->f1F4 |= 1;
    else
        w->f1F4 &= ~1u;
    if (w->prm->flags & 2) {
        if (w->h.gnd_vel < 0x800)
            w->h.gnd_vel = 0x800;
        if (w->prm->p6 < 0)
            w->h.gnd_vel = (s16)-w->h.gnd_vel;
        if (w->w8 == w->h.x)
            w->h.gnd_vel = (s16)-w->h.gnd_vel;
        w->f1F4 |= 1;
    }
    w->coll[0].owner = NULL;
    w->wC &= ~0xff;
}
static void GmkRail_State_Move(Obj *w) {
    s32 xmin, xmax, ymin, ymax, v;
    s16 old;
    ObjParam *p;
    if (w->h.gnd_vel == 0) {
        if (w->coll[0].owner != NULL)
            return;
        w->wC++;
        if (!(w->wC & 1))
            return;
        w->wC &= ~0xff;
        w->h.obj_flags &= ~2u;
        w->coll[0].owner = w;
        return;
    }
    if (!(w->prm->flags & 1) && !(w->f1F4 & 1))
        w->h.gnd_vel = (s16)dcp_spd_down(w->h.gnd_vel, 10);
    w->h.vel_x = (s16)((w->h.gnd_vel * obj_cos((u16)(w->wC >> 8))) >> 12);
    w->h.vel_y = (s16)((w->h.gnd_vel * obj_sin((u16)(w->wC >> 8))) >> 12);
    if (w->prm->p6 >= 0) {
        xmax = w->w8;
        xmin = w->home_x << 8;
    } else {
        w->h.vel_y = (s16)-w->h.vel_y;
        w->h.vel_x = (s16)-w->h.vel_x;
        xmin = w->w8;
        xmax = w->home_x << 8;
    }
    p = w->prm;
    if (p->p7 >= 0) {
        ymin = w->home_y << 8;
        ymax = (w->home_y + (p->p9 + (p->p7 << 6))) << 8;
    } else {
        ymin = (w->home_y + (p->p9 + (p->p7 << 6))) << 8;
        ymax = w->home_y << 8;
    }
    if (p->flags & 1) {
        if (w->h.vel_y < 0) {
            old = w->h.gnd_vel;
            w->h.gnd_vel = (s16)dcp_spd_down(old, 0x10);
            v = w->h.gnd_vel;
            if (v < 0)
                v = -v;
            if (v < 0x40)
                w->h.gnd_vel = (s16)-old;
        } else if (w->h.gnd_vel > 0) {
            w->h.gnd_vel = (s16)dcp_spd_up(w->h.gnd_vel, 0x10, 0xf00);
        } else {
            w->h.gnd_vel = (s16)dcp_spd_up(w->h.gnd_vel, -0x10, 0xf00);
        }
    }
    if ((w->h.vel_x > 0 && w->h.x + w->h.vel_x > xmax) || (w->h.vel_x < 0 && w->h.x + w->h.vel_x < xmin))
        w->h.obj_flags |= 2;
    if ((w->h.vel_x > 0 && w->h.x >= xmax) || (w->h.vel_x < 0 && w->h.x <= xmin)) {
        if (w->h.vel_x > 0)
            w->h.x = xmax;
        else
            w->h.x = xmin;
        w->h.vel_x = 0;
    }
    if ((w->h.vel_y > 0 && w->h.y >= ymax) || (w->h.vel_y < 0 && w->h.y <= ymin)) {
        if (w->h.vel_y > 0)
            w->h.y = ymax;
        else
            w->h.y = ymin;
        w->h.vel_y = 0;
    }
    if (w->h.vel_x == 0 && w->h.vel_y == 0)
        w->h.gnd_vel = 0;
}
static void GmkRail_OnHit(ObjBox *a, ObjBox *b) {
    Obj *t0 = (Obj *)a->owner;
    DcPlayer *t1 = obj_box_player(b);
    s32 v;
    if (t0 == NULL || t1 == NULL || t1->type != 1)
        return;
    v = t1->move_x;
    if (v < 0)
        v = -v;
    if (v <= t1->run_thr[0]) {
        if (t0->prm->p7 == 0)
            return;
    }
    GmkRail_StartFromPlayerSpeed(t0, t1);
    Player_Action_AttachToObject(t1, t0, 0, 0x28);
    if (g_dcp_hooks.sfx)
        g_dcp_hooks.sfx(g_dcp_hooks.ctx, t1, DCP_CH_BOOST, 0x5a);
    t1->pf |= 1;
    g_obj.stats.rails++;
    count("gmk_rail", t0, t1);
}
Obj *GmkRail_Create(ObjParam *p, s32 b, s32 c) {
    Obj *w = mapobj(p, b, c, W.zone == 5 ? OBJ_BAC_GMK_RAIL6 : OBJ_BAC_GMK_RAIL, OBJ_PRIO_MAPOBJ, obj_mapobject_dtor);
    if (w == NULL)
        return NULL;
    if (W.zone == 5)
        obj_set_anim(w, 4);
    else
        obj_set_anim(w, 1);
    w->spr.order = 0x14;
    rect_set_box2d(&w->coll[0], 4, -8, 0x10, 0);
    rect_set_atkdef(&w->coll[0], 0, (u32)-0x10);
    w->coll[0].cb = GmkRail_OnHit;
    w->coll[0].flags |= 0x800;
    w->coll[0].owner = w;
    w->coll[0].owner_kind = OBJ_OWNER_OBJECT;
    w->h.move_flags |= 0x100;
    w->w8 = (w->home_x + (p->p8 + (p->p6 << 6))) << 8;
    w->wC = dcp_atan2(p->p9 + (p->p7 << 6), p->p8 + (p->p6 << 6)) << 8;
    w->state = GmkRail_State_Move; /* GmkRail_Action_Init */
    return w;
}

/* ===================================================== EggBishop (0xe4) */
static void EggBishop_State_Spawned(Obj *p) {
    if (p->wC == 0)
        return;
    p->wC--;
    if (p->wC != 0)
        return;
    p->spr.order = 0xa;
}
Obj *EggBishop_Create(ObjParam *p, s32 b, s32 c) {
    Obj *w = mapobj(p, b, c, OBJ_BAC_EGG_BISHOP, OBJ_PRIO_MAPOBJ, obj_mapobject_dtor);
    s32 k;
    if (w == NULL)
        return NULL;
    w->kind = OBJK_ENEMY;
    w->spr.order = 0x15;
    if (!(w->prm->flags & 1))
        w->h.disp_flags &= ~1;
    w->h.vel_x = 0x300;
    if (w->h.disp_flags & 1)
        w->h.vel_x = (s16)-w->h.vel_x;
    k = w->prm->flags & 0xc;
    if (k != 0)
        w->h.vel_x = (s16)((w->h.vel_x * (((k >> 2) << 7) + 0x100)) >> 8);
    w->h.move_flags |= 0x10;
    w->h.move_flags |= 0x100;
    w->h.disp_flags |= 0x100;
    /* EggBishop_Action_Spawned */
    obj_disable_colliders_set_anim(w, 4);
    w->h.disp_flags |= 4;
    w->state = EggBishop_State_Spawned;
    w->wC = 0x10;
    return w;
}

/* ===================================================== DecGroup45 (zone 3 decorations) */
Obj *DecGroup45_Create(ObjParam *p, s32 a, s32 b) {
    Obj *w = obj_alloc(OBJ_PRIO_MAPOBJ, Decoration_Destructor);
    if (w == NULL)
        return NULL;
    Decoration_InitFromMap(w, p, a, b);
    switch (p->type) {
    case 0x54:
    case 0x55:
        obj_load_bac(w, OBJ_BAC_DEC_STATUE);
        if (p->type & 1)
            w->h.disp_flags ^= 1;
        break;
    case 0x5f:
    case 0x60:
        obj_load_bac(w, OBJ_BAC_DEC_STATUE);
        obj_set_anim(w, 1);
        w->spr.order = 6;
        set_prio(w, 1);
        if (!(p->type & 1))
            w->h.disp_flags ^= 1;
        break;
    case 0x64:
    case 0x65:
        obj_load_bac(w, OBJ_BAC_GMK_HAND);
        if (p->type & 1)
            w->h.disp_flags ^= 1;
        break;
    case 0x66:
        obj_load_bac(w, OBJ_BAC_GMK_HAND);
        obj_set_anim(w, 1);
        w->spr.order = 0x15;
        break;
    case 0x56:
    case 0x57:
        obj_load_bac(w, OBJ_BAC_DEC_RELIEF);
        if (p->type & 1)
            w->h.disp_flags ^= 1;
        break;
    case 0x58:
    case 0x59:
        obj_load_bac(w, OBJ_BAC_DEC_BED);
        if (p->type & 1)
            w->h.disp_flags ^= 1;
        break;
    case 0x5a:
    case 0x5b:
        obj_load_bac(w, OBJ_BAC_DEC_CANOPS);
        if (p->type & 1)
            w->h.disp_flags ^= 1;
        break;
    case 0x5c:
    case 0x5d:
        obj_load_bac(w, OBJ_BAC_DEC_CANOPS);
        obj_set_anim(w, 1);
        if (p->type & 1)
            w->h.disp_flags ^= 1;
        break;
    case 0x5e:
        obj_load_bac(w, OBJ_BAC_DEC_BOX);
        break;
    case 0x61:
        obj_load_bac(w, OBJ_BAC_DEC_TORCH);
        w->h.disp_flags |= 4;
        break;
    case 0x62:
        obj_load_bac(w, OBJ_BAC_DEC_DECORA);
        w->spr.order = 6;
        set_prio(w, 0);
        break;
    case 0x63:
        obj_load_bac(w, OBJ_BAC_DEC_DECORA);
        obj_set_anim(w, 1);
        w->spr.order = 6;
        set_prio(w, 0);
        break;
    case 0x45:
    case 0x46:
    case 0x47:
    case 0x48:
    case 0x49:
        obj_load_bac(w, OBJ_BAC_GMK_RAIL);
        switch (p->type) {
        case 0x45:
            obj_set_anim(w, 0);
            break;
        case 0x49:
            w->h.disp_flags ^= 1;
            /* fall through */
        case 0x46:
            obj_set_anim(w, 2);
            break;
        case 0x47:
            obj_set_anim(w, 3);
            break;
        case 0x48:
            obj_set_anim(w, 4);
            break;
        }
        w->spr.order = 0x16;
        break;
    case 0x4a:
    case 0x4b:
    case 0x4c: {
        Obj *sh = NULL;
        obj_load_bac(w, OBJ_BAC_DEC_SAND);
        switch (p->type) {
        case 0x4a:
            sh = dec_shared_create(OBJ_BAC_DEC_SAND, 0, 0);
            obj_set_anim(w, 0);
            break;
        case 0x4b:
            sh = dec_shared_create(OBJ_BAC_DEC_SAND, 1, 1);
            obj_set_anim(w, 1);
            break;
        case 0x4c:
            sh = dec_shared_create(OBJ_BAC_DEC_SAND, 2, 2);
            obj_set_anim(w, 2);
            break;
        }
        w->parent = sh;
        if (sh != NULL)
            w->spr.vram = &sh->spr;
        w->spr.order = 0x16;
        w->spr.flags |= 8;
        w->h.obj_flags |= 0x200;
        w->state = dec_shared_ping;
        break;
    }
    case 0x53: {
        Obj *sh;
        obj_load_bac(w, OBJ_BAC_DEC_TABLE_WATER);
        sh = dec_shared_create(OBJ_BAC_DEC_TABLE_WATER, 0, 3);
        w->parent = sh;
        if (sh != NULL)
            w->spr.vram = &sh->spr;
        w->spr.order = 0x16;
        w->spr.flags |= 8;
        w->h.obj_flags |= 0x200;
        w->state = dec_shared_ping;
        break;
    }
    }
    if (w->spr.order == 0)
        w->spr.order = 0x16;
    w->h.move_flags |= 0x2100;
    w->h.obj_flags |= 2;
    return w;
}
