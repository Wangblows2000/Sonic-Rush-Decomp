/*
 * Water Palace (zone 2) objects, enemies, decorations and effects, copied
 * from the decompilation and adapted (task/work struct -> Obj with named
 * fields, VRAM/palette calls dropped, Sound_PlaySeqArcEx -> obj_sfx,
 * positional sound / sound handles dropped, ScreenShake_Start ->
 * obj_screen_shake, ScreenFlash_Create -> effect hook):
 *   src/game/Zone/Water_Palace/Objects/  WaterRunTrigger_*, MapObj7D_*,
 *     MapObj80_*, LaunchZone_*, LandUnrest_*, Wheel2D_*, Mine_*, MineDigit_*,
 *     BubbleSpawner_*, Bubble_*, WaterJet_*
 *   src/game/Zone/Water_Palace/Enemies/  Kuragen_*, KuragenBomb_*
 *   src/game/Zone/Water_Palace/Effects/  EffAirBubble_*, EffDrownCount_*,
 *     EffGmkBubble_*, EffWaterGmk_*, EffWaterRunSplash_Create,
 *     EffWaterSplash_*
 *   src/game/Zone/Water_Palace/Decorations/ DecGroup41_*, DecFishPart_*
 *   src/game/Decorations/ DecGroup4D_*, DecShared_*
 *   src/game/Effects/ EffZakoBomb_Create
 * The 3D variants used on the boss stages (Bubble3D, EffSplash3D,
 * EffRipple3D, EffDrownCount_Spawn3D / FixMoji3D) are not translated: the
 * 2D stages never take those branches (zone < 9 and act < 2).
 * See docs/dreamcast/translation/OBJECTS.md (section 5c).
 */
#include <string.h>
#include "obj_internal.h"

#define CAM0 g_obj_camera[0] /* gCamera[0] */

static void set_prio(Obj *o, u32 prio) { o->spr.flags = (o->spr.flags & ~0x60000u) | (prio << 17); }
static DcPlayer *box_hdr(const ObjBox *b) { return b ? (DcPlayer *)b->owner : NULL; }
static s32 probe(s32 x, s32 y, u32 flags, u32 dir) { return rlvl_probe_distance(g_obj.map, x, y, flags, dir, NULL, NULL); }
/* "FrameRand": the LCG step applied to the stage frame counter (no seed update) */
static u16 frame_rand(void) { return (u16)((((u32)W.frame << 16) * 0x196225u + 0x3c6ef35fu) >> 16); }
static void fxhook(DcPlayer *p, s32 kind, s32 a, s32 b) {
    if (g_obj_hooks.effect)
        g_obj_hooks.effect(g_obj_hooks.ctx, p, kind, a, b);
}
static void map_hitbox(Obj *w, s8 l, s8 t, s8 r, s8 b) { /* StageObj_SetMapHitbox */
    w->h.box_l = l;
    w->h.box_t = t;
    w->h.box_r = r;
    w->h.box_b = b;
}
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
static void trigger_box(Obj *w, s16 l, s16 t, s16 r, s16 b, void (*cb)(ObjBox *, ObjBox *), u16 fl) {
    rect_set_box2d(&w->coll[0], l, t, r, b);
    rect_set_atkdef(&w->coll[0], 0, (u32)-0x10);
    w->coll[0].flags |= fl;
    w->coll[0].cb = cb;
    w->coll[0].owner = w;
    w->coll[0].owner_kind = OBJ_OWNER_OBJECT;
}
static void count(const char *ev, Obj *o, DcPlayer *p) {
    g_obj.stats.gimmicks++;
    obj_log(ev, o && o->prm ? o->prm->type : 0, p ? p->x >> 8 : (o ? o->h.x >> 8 : 0));
}
static Obj *eff_alloc(int bac) {
    Obj *o = obj_alloc(OBJ_PRIO_EFFECT, obj_stageobj_dtor);
    if (o == NULL)
        return NULL;
    if (bac >= 0)
        obj_load_bac(o, bac);
    o->kind = OBJK_EFFECT;
    return o;
}
static const DcPlayer *eff_parent(const Obj *w) { return w->parent ? &w->parent->h : w->ppar; }
static void eff_set_parent(Obj *w, DcPlayer *arg) {
    if (obj_player_ctx(arg) != NULL)
        w->ppar = arg; /* a player: no Obj around it */
    else
        w->parent = (Obj *)arg;
}

/* =================================================== shared effects */
/* Effect_State_FollowParent (src/game/Effects), with a player parent too */
static void eff_state_follow_parent(Obj *w) {
    const DcPlayer *p = eff_parent(w);
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

/* EffZakoBomb_Create (src/game/Effects): explosion cloud, act_com BAC anim 2 */
Obj *EffZakoBomb_Create(DcPlayer *arg, s16 x, s16 y) {
    Obj *w = eff_alloc(OBJ_BAC_ZAKO_BOMB);
    if (w == NULL)
        return NULL;
    obj_set_anim(w, 2);
    w->spr.order = 9;
    if (arg->grav_rot != 0)
        w->spr.flags |= 0x200;
    if (arg->disp_flags & 1)
        w->h.disp_flags |= 1;
    w->h.x = arg->x;
    w->h.y = arg->y;
    w->h.angle = arg->grav_rot;
    dcp_rotate_vec(&x, &y, arg->grav_rot);
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

/* EffAirBubble_Create / _State_Rise */
static void EffAirBubble_State_Rise(Obj *w) {
    s32 r;
    w->h.vel_y = (s16)dcp_spd_up(w->h.vel_y, -13, w->w8);
    w->h.vel_x = (s16)(w->h.vel_y >> 1);
    if (W.frame & 2)
        w->h.vel_x = (s16)-w->h.vel_x;
    r = probe(w->h.x >> 8, w->h.y >> 8, 1, 2);
    if (w->wC > (w->h.y >> 8) || r < 0)
        w->h.obj_flags |= 4;
}
void EffAirBubble_Create(DcPlayer *arg, s16 x, s16 y, s32 type, s32 t) {
    Obj *w = eff_alloc(OBJ_BAC_BUBBLE);
    if (w == NULL)
        return;
    obj_set_anim(w, (u16)(type + 2));
    w->spr.order = 9;
    if (arg->disp_flags & 1)
        w->h.disp_flags |= 1;
    w->h.x = arg->x + (x << 8) + ((3 - (RAND() & 6)) << 8);
    w->h.y = arg->y + (y << 8);
    w->h.vel_y = (s16)(0xff - (RAND() & 0xfe));
    w->wC = t + 6;
    w->w8 = 0x2bf - (RAND() & 0xfe);
    w->h.move_flags |= 0x100;
    w->h.obj_flags |= 2;
    w->state = EffAirBubble_State_Rise;
}
/* EffAirBubble_Spawn: 2D branch (the boss-stage branch creates Bubble3D) */
void EffAirBubble_Spawn(DcPlayer *a, s32 x, s32 y, s32 c) {
    u16 r = RAND() & 3;
    if (W.zone >= 9 || W.act >= 2)
        return; /* Bubble3D (boss arenas): not translated */
    switch (r) {
    case 0:
        EffAirBubble_Create(a, (s16)x, (s16)y, 0, c);
        break;
    case 1:
        EffAirBubble_Create(a, (s16)(x + 4), (s16)(y + 8), 1, c);
        EffAirBubble_Create(a, (s16)(x + 1), (s16)(y - 3), 1, c);
        break;
    case 2:
        EffAirBubble_Create(a, (s16)x, (s16)y, 0, c);
        EffAirBubble_Create(a, (s16)(x + 2), (s16)(y + 6), 1, c);
        break;
    case 3:
        EffAirBubble_Create(a, (s16)x, (s16)y, 0, c);
        EffAirBubble_Create(a, (s16)(x - 2), (s16)(y + 6), 1, c);
        EffAirBubble_Create(a, (s16)(x + 4), (s16)(y + 9), 1, c);
        break;
    }
}

/* EffDrownCount_Create / _State_FollowPlayer / _Spawn (countdown digit over the player) */
static void EffDrownCount_State_FollowPlayer(Obj *w) {
    const DcPlayer *p = eff_parent(w);
    if (p != NULL) {
        w->h.x = p->x + w->h.vel_x;
        w->h.y = p->y + w->h.vel_y;
    }
    if (w->wC > 0x10)
        w->h.vel_y = (s16)(w->h.vel_y - 0xc0);
    w->wC++;
    if (w->wC > 0x30)
        w->h.obj_flags |= 4;
}
void EffDrownCount_Create(DcPlayer *arg, s32 x, s32 y, s32 idx) {
    Obj *w = eff_alloc(OBJ_BAC_EFF_WATER);
    if (w == NULL)
        return;
    obj_set_anim(w, (u16)(idx + 4));
    w->spr.flags |= 0x10;
    w->spr.order = 3;
    set_prio(w, 0);
    w->h.obj_flags |= 0x10;
    w->h.disp_flags |= 1;
    w->h.vel_x = (s16)(x << 8);
    w->h.vel_y = (s16)(y << 8);
    w->h.x = arg->x + w->h.vel_x;
    w->h.y = arg->y + w->h.vel_y;
    eff_set_parent(w, arg);
    w->state = EffDrownCount_State_FollowPlayer;
    w->wC = 0;
    w->w8 = 0;
    w->h.move_flags |= 0x2100;
    w->h.obj_flags |= 2;
}
void EffDrownCount_Spawn(DcPlayer *a, s32 b, s32 c, s32 d) {
    if (!(W.zone < 9 && W.act < 2))
        return; /* EffDrownCount_Spawn3D (FixMoji3D): boss arenas, not translated */
    EffDrownCount_Create(a, b, c, d);
}

/* EffGmkBubble_Create / _State_Main (bubbles stirred up by the Wheel2D) */
static void EffGmkBubble_State_Main(Obj *w) {
    const DcPlayer *p = eff_parent(w);
    s32 r;
    if (p != NULL)
        w->h.gnd_vel = (s16)(p->gnd_vel << 3); /* @0x34 */
    w->wC += w->h.gnd_vel;
    w->h.x = w->h.px + 0x5400 - ((obj_cos((u16)((u8)FX_DivS32(w->wC << 8, 0x21e52) << 8)) * 0x54) >> 4);
    w->h.y += w->h.vel_y;
    r = probe(w->h.x >> 8, w->h.y >> 8, 1, 2);
    if ((u32)w->w8 > (u32)(w->h.y >> 8) || r < 0)
        w->h.obj_flags |= 4;
}
void EffGmkBubble_Create(DcPlayer *arg, s32 x, s32 y, s32 dx, s32 unused, s32 t) {
    Obj *w;
    const DcPlayer *p;
    u16 r = RAND() & 1;
    (void)unused;
    w = eff_alloc(OBJ_BAC_BUBBLE);
    if (w == NULL)
        return;
    obj_set_anim(w, (u16)(r + 2));
    w->spr.order = 9;
    set_prio(w, 1);
    if (arg->disp_flags & 1)
        w->h.disp_flags |= 1;
    w->h.x = x + (dx << 8);
    w->h.y = y + (0xfff - (RAND() & 0x1ffe));
    w->h.vel_y = (s16)(0x22f - (RAND() & 0x1fe));
    w->w8 = t + 6;
    eff_set_parent(w, arg);
    w->wC = RAND() & 0x1ffff;
    w->h.move_flags |= 0x2100;
    w->h.obj_flags |= 2;
    w->h.px = (x - 0x5400) + (0xfff - (RAND() & 0x1ffe)); /* @0x1C: circle centre x */
    w->state = EffGmkBubble_State_Main;
    p = eff_parent(w);
    w->h.vel_y = (s16)(p->gnd_vel << 2);
    if (!(p->pf & 1))
        w->h.vel_y = (s16)-w->h.vel_y;
}

/* EffWaterGmk_Create (spray following the parent) / _CreateSplash */
void EffWaterGmk_Create(DcPlayer *arg, s16 x, s16 y) {
    Obj *w = eff_alloc(OBJ_BAC_EFF_WATER_GMK);
    if (w == NULL)
        return;
    obj_set_anim(w, 2);
    w->spr.order = 4;
    set_prio(w, 1);
    if (arg->disp_flags & 1) {
        w->h.disp_flags |= 1;
        x = (s16)-x;
    }
    w->h.x = arg->x + (x << 8);
    w->h.y = arg->y + (y << 8);
    eff_set_parent(w, arg);
    w->h.vel_x = x;
    w->h.vel_y = y;
    w->h.move_flags |= 0x2100;
    w->h.obj_flags |= 2;
    w->state = eff_state_follow_parent; /* Effect_SetFollowParentState */
}
Obj *EffWaterGmk_CreateSplash(DcPlayer *arg, s32 x, s32 y) {
    Obj *w = eff_alloc(OBJ_BAC_EFF_WATER_GMK);
    if (w == NULL)
        return NULL;
    if ((arg->move_x < 0 ? -arg->move_x : arg->move_x) > 0x400 || arg->move_y != 0)
        obj_set_anim(w, 1);
    w->spr.order = 0x12;
    if (arg->disp_flags & 1)
        w->h.disp_flags |= 1;
    w->h.disp_flags ^= 1;
    w->h.x = arg->x + (x << 8);
    w->h.y = arg->y + (y << 8);
    w->h.move_flags |= 0x2100;
    w->h.obj_flags |= 2;
    eff_set_move_until_anim_end(w);
    return w;
}
/* EffWaterRunSplash_Create (spray of the water run, at the surface) */
void EffWaterRunSplash_Create(DcPlayer *arg, s32 x, s32 y, s32 base) {
    Obj *w = eff_alloc(OBJ_BAC_EFF_WATER);
    if (w == NULL)
        return;
    obj_set_anim(w, 2);
    w->spr.order = 9;
    set_prio(w, 1);
    if (arg->disp_flags & 1)
        w->h.disp_flags |= 1;
    w->h.x = arg->x + (x << 8);
    w->h.y = (base << 8) + y * 256;
    w->h.move_flags |= 0x2100;
    w->h.obj_flags |= 2;
    eff_set_move_until_anim_end(w);
}
/* EffWaterSplash_Create / _Spawn (entering / leaving the water) */
void EffWaterSplash_Create(DcPlayer *arg, s32 x, s32 y, s32 base) {
    Obj *w = eff_alloc(OBJ_BAC_EFF_WATER);
    if (w == NULL)
        return;
    if ((arg->vel_y < 0 ? -arg->vel_y : arg->vel_y) > 0x600)
        obj_set_anim(w, 1);
    w->spr.order = 9;
    set_prio(w, 1);
    if (arg->disp_flags & 1)
        w->h.disp_flags |= 1;
    w->h.x = arg->x + (x << 8);
    w->h.y = (base << 8) + y * 256;
    w->h.move_flags |= 0x2100;
    w->h.obj_flags |= 2;
    eff_set_move_until_anim_end(w);
    g_obj.stats.water_splashes++;
}
void EffWaterSplash_Spawn(DcPlayer *p) {
    if (p->pid != 0)
        return;
    if (W.zone >= 9 || W.act >= 2)
        return; /* EffSplash3D_Spawn (boss arenas): not translated */
    EffWaterSplash_Create(p, 0, 0, CAM0.water);
}

/* ============================================================ objects */
/* ------------------------------------------- WaterRunTrigger (0x7b) */
static void WaterRunTrigger_OnHit(ObjBox *a, ObjBox *b) {
    Obj *o = (Obj *)a->owner;
    DcPlayer *p = box_hdr(b);
    if (p == NULL || p->type != 1)
        return;
    if (p->gnd_vel < p->run_thr[2] || (o->prm->flags & 1)) {
        if (p->gnd_vel > -p->run_thr[2])
            return;
        if (!(o->prm->flags & 1))
            return;
    }
    Player_Action_WaterRun(p);
    count("water_run", o, p);
}
Obj *WaterRunTrigger_Create(ObjParam *p, s32 b, s32 c) {
    Obj *w = mapobj(p, b, c, -1, OBJ_PRIO_MAPOBJ, obj_mapobject_dtor);
    if (w == NULL)
        return NULL;
    w->kind = OBJK_TRIGGER;
    if (w->prm->flags & 1)
        trigger_box(w, -0x10, -8, -4, 0, WaterRunTrigger_OnHit, 0x800);
    else
        trigger_box(w, 4, -8, 0x10, 0, WaterRunTrigger_OnHit, 0x800);
    w->h.move_flags |= 0x2120;
    return w;
}

/* ------------------------------------- MapObj7D (0x7d): water current */
static void MapObj7D_OnHit(ObjBox *a, ObjBox *b) {
    Obj *o = (Obj *)a->owner;
    DcPlayer *p = box_hdr(b);
    if (p == NULL)
        return;
    if (o->prm->flags & 1)
        p->gravity = 0xa8;
    else
        p->gravity = 0x54;
    p->pf |= 0x2000000;
    if (!(W.frame & 7))
        EffWaterGmk_Create(p, 0, -12);
}
Obj *MapObj7D_Create(ObjParam *p, s32 b, s32 c) {
    Obj *w = mapobj(p, b, c, -1, OBJ_PRIO_MAPOBJ, obj_mapobject_dtor);
    if (w == NULL)
        return NULL;
    w->kind = OBJK_TRIGGER;
    trigger_box(w, p->p6, p->p7, (s16)(p->p8 + p->p6), (s16)(p->p9 + p->p7), MapObj7D_OnHit, 0xc0);
    w->h.move_flags |= 0x2120;
    return w;
}

/* --------------------------------- MapObj80 (0x80): water slide entry */
static void MapObj80_OnHit(ObjBox *a, ObjBox *b) {
    Obj *o = (Obj *)a->owner;
    DcPlayer *p = box_hdr(b);
    if (p == NULL || p->type != 1)
        return;
    if (p->anim != 0x27) {
        if (!(p->move_flags & 1))
            return;
        if ((p->gnd_vel >= 0 && !(o->prm->flags & 1)) || (p->gnd_vel < 0 && (o->prm->flags & 1))) {
            Player_Action_MapObj80(p, o);
            count("mapobj80", o, p);
        }
    } else {
        if (!(o->prm->flags & 2))
            return;
        Player_Action_MapObj80(p, o);
    }
}
Obj *MapObj80_Create(ObjParam *p, s32 b, s32 c) {
    Obj *w = mapobj(p, b, c, -1, OBJ_PRIO_MAPOBJ, obj_mapobject_dtor);
    if (w == NULL)
        return NULL;
    w->kind = OBJK_TRIGGER;
    if (w->prm->flags & 2) {
        if (w->prm->flags & 1)
            trigger_box(w, -10, 0, 0x10, 0x20, MapObj80_OnHit, 0xc0);
        else
            trigger_box(w, 0x20, 0, 0x38, 0x20, MapObj80_OnHit, 0xc0);
    } else {
        if (w->prm->flags & 1)
            trigger_box(w, -0x10, 0, 0, 0x40, MapObj80_OnHit, 0xc0);
        else
            trigger_box(w, 0, 0, 0x10, 0x40, MapObj80_OnHit, 0xc0);
    }
    w->h.move_flags |= 0x2120;
    return w;
}

/* --------------------------------------- LaunchZone (0x83): water jet area */
static void LaunchZone_OnHit(ObjBox *a, ObjBox *b) {
    Obj *s = (Obj *)a->owner;
    DcPlayer *t = box_hdr(b);
    s16 vx = 0, vy = 0;
    u16 fl;
    if (t == NULL)
        return;
    fl = s->prm->flags;
    if (!(fl & 8)) {
        if (fl & 1)
            vx = -0xa00;
        else
            vx = 0xa00;
    }
    if (fl & 2)
        vy = -0xa00;
    if (fl & 4)
        vy = 0xa00;
    if (((fl & 2) | 4) && !(fl & 8)) { /* ((fl & 2) | 4) is always true, as in the original */
        vx = (s16)((vx * 0xb5) >> 8);
        vy = (s16)((vy * 0xb5) >> 8);
    }
    if (t->type != 1)
        return; /* (the original calls Player_Action_ZoneLaunch on any toucher; only players reach list 1) */
    Player_Action_ZoneLaunch(t, vx, vy);
    count("zone_launch", s, t);
}
Obj *LaunchZone_Create(ObjParam *p, s32 b, s32 c) {
    Obj *w = mapobj(p, b, c, -1, OBJ_PRIO_MAPOBJ, obj_mapobject_dtor);
    if (w == NULL)
        return NULL;
    w->kind = OBJK_TRIGGER;
    trigger_box(w, p->p6, p->p7, (s16)(p->p8 + p->p6), (s16)(p->p9 + p->p7), LaunchZone_OnHit, 0xc0);
    w->h.move_flags |= 0x2120;
    return w;
}

/* ---------------------------------------- LandUnrest (0x81): tilting raft */
static void LandUnrest_State_Tilt(Obj *s) {
    u8 a;
    s8 b;
    DcPlayer *r = s->cmap.rider;
    s->h.rot2 = s->h.angle;
    s->h.vel_y = 0;
    if (r != NULL) {
        if (r->ride == &s->ref) {
            s->h.vel_y = 0x100;
            s->h.rot3 = (u8)((r->x - s->h.x) >> 8);
            s->h.angle = dcp_angle_toward8(s->h.angle, s->h.rot3, 2);
            dcp_apply_slope_force(r, (const u8 *)&s->h);
            if (s->h.angle != 0) {
                if (s->h.angle > 0x80)
                    s->w8 = (0x100 - s->h.angle) >> 1;
                else
                    s->w8 = 0x100 - (s->h.angle >> 1);
                if ((u32)s->w8 < 4 || (u32)s->w8 > 0xfc)
                    s->w8 = 0;
            } else {
                s->w8 = 0;
            }
        }
    } else {
        a = s->h.angle;
        if (a != 0) {
            if (s->w8 == 0) {
                if (a < 0x80) {
                    s->h.angle = (u8)(a - 2);
                    if (s->h.angle > 0x80)
                        s->h.angle = 0;
                }
                if (s->h.angle > 0x80) {
                    s->h.angle += 2;
                    if (s->h.angle < 0x80)
                        s->h.angle = 0;
                }
            } else {
                if ((u32)s->w8 > 0x80) {
                    s->h.angle = (u8)(a - 2);
                    if ((u32)s->w8 < s->h.angle)
                        s->h.angle = (u8)s->w8;
                }
                if ((u32)s->w8 < 0x80) {
                    s->h.angle += 2;
                    if ((u32)s->w8 > s->h.angle)
                        s->h.angle = (u8)s->w8;
                }
            }
            if ((u32)s->w8 == s->h.angle) {
                if ((u32)s->w8 < 4 || (u32)s->w8 > 0xfc) {
                    s->w8 = 0;
                    s->h.angle = 0;
                } else if (s->h.angle > 0x80) {
                    s->w8 = (0x100 - s->h.angle) >> 1;
                } else {
                    s->w8 = 0x100 - (s->h.angle >> 1);
                }
            }
        }
        if ((s->h.y >> 8) != s->home_y) {
            s->h.vel_y = -0x100;
            s->wC = 0;
        }
    }
    if (s->h.vel_y > 0) {
        if (s->wC >= 8)
            s->h.vel_y = 0;
        else
            s->wC++;
    } else if (s->h.vel_y < 0) {
        if ((s->h.y >> 8) <= s->home_y) {
            s->h.vel_y = 0;
            s->wC = 0;
            s->h.y = s->home_y << 8;
        }
    }
    if (s->h.angle == 0)
        return;
    if (s->h.rot2 < 0x80)
        b = (s8)-(s->h.rot2 >> 1);
    else
        b = (s8)-((0x100 - s->h.rot2) >> 1);
    if (s->h.angle < 0x80)
        s->h.shake_y = (s8)-(s->h.angle >> 1);
    else
        s->h.shake_y = (s8)-((0x100 - s->h.angle) >> 1);
    if (b != s->h.shake_y)
        s->h.vel_y = (s16)(s->h.vel_y + ((b - s->h.shake_y) << 8));
    if ((s->h.y >> 8) < s->home_y)
        s->h.vel_y = 0;
}
static void LandUnrest_Action_Init(Obj *s) {
    s->state = LandUnrest_State_Tilt;
    s->w8 = 0;
    s->wC = 0;
}
Obj *LandUnrest_Create(ObjParam *p, s32 b, s32 c) {
    Obj *w = mapobj(p, b, c, OBJ_BAC_LAND_UNREST, 0xf6, obj_mapobject_dtor);
    if (w == NULL)
        return NULL;
    w->spr.order = 0x14;
    w->spr.flags |= 0x200;
    w->has_cmap = 1;
    w->cmap.obj = w;
    w->cmap.map = (const u8(*)[8])g_obj.data.solid;
    w->cmap.attr = g_obj.data.col_attr;
    w->cmap.w = 8;
    w->cmap.h = 2;
    w->cmap.ox = -0x21;
    w->cmap.oy = -0x1a;
    w->h.move_flags |= 0x100;
    w->h.disp_flags |= 0x100;
    w->h.obj_flags |= 2;
    LandUnrest_Action_Init(w);
    return w;
}

/* ------------------------------------------------- Wheel2D (0x82) */
static void wheel_rot(Obj *w) { /* MTX_Identity33_ + MTX_RotY33_(-unk8) + MTX_Concat33 on the model matrix */
    ObjMtx33 m, r;
    u16 a = (u16)(-w->w8);
    MTX_Identity33_(&m);
    MTX_RotY33_(&r, obj_sin(a), obj_cos(a));
    MTX_Concat33(&m, &r, &m);
    memcpy(w->mtx, m.m, sizeof(w->mtx));
}
static void Wheel2D_State_Main(Obj *w) {
    DcPlayer *p = (DcPlayer *)w->target;
    s32 old;
    if (p != NULL) {
        if (p->gimmick != w) {
            w->h.disp_flags |= 0x200;
            w->h.obj_flags &= ~2u;
            w->target = NULL;
            return;
        }
        if (p->pf & 2) {
            w->w8 -= w->wC;
            w->w8 &= 0xffff;
            w->wC = dcp_spd_down(w->wC, 6);
        } else {
            old = w->w8;
            w->w8 = (u16)((u8)(p->rot3 - 0x10) << 8);
            w->wC = (old - w->w8) & 0xffff;
        }
        wheel_rot(w);
        if (p->rot3 < 0x80)
            w->f1F4 |= 1;
        else
            w->f1F4 &= ~1u;
    } else {
        w->w8 -= w->wC;
        w->w8 &= 0xffff;
        w->wC = dcp_spd_down(w->wC, 6);
        wheel_rot(w);
    }
}
static void Wheel2D_OnHit(ObjBox *a, ObjBox *b) {
    Obj *w = (Obj *)a->owner;
    DcPlayer *p = box_hdr(b);
    if (w == NULL || p == NULL)
        return;
    if (p->type != 1)
        return; /* (only players are in list 1) */
    w->target = p;
    Player_Action_Wheel2D(p, w);
    count("wheel2d", w, p);
}
Obj *Wheel2D_Create(ObjParam *p, s32 b, s32 c) {
    Obj *w = mapobj(p, b, c, OBJ_BAC_WHEEL2D, OBJ_PRIO_MAPOBJ, obj_mapobject_dtor);
    if (w == NULL)
        return NULL;
    w->spr.order = 0x14;
    w->spr.flags |= 0x80000;
    /* six StageObj_LoadModelSimple blocks of mod/gmk_wheel.nsbmd share the
     * model matrix @0x128: one model view here */
    w->model = "mod__gmk_wheel__gmk_wheel";
    MTX_Identity33_((ObjMtx33 *)w->mtx);
    w->h.disp_flags |= 0x200;
    w->mscale[0] = w->mscale[1] = w->mscale[2] = 0x34cc;
    trigger_box(w, -0x7c, -0x28, -0x60, 0x18, Wheel2D_OnHit, 0xc0);
    w->h.move_flags |= 0x2100;
    w->target = NULL;
    w->state = Wheel2D_State_Main; /* Wheel2D_Action_Init */
    return w;
}

/* ------------------------------------------------- Mine (0x7f) */
static void MineDigit_State_FollowParent(Obj *w) {
    if (w->parent != NULL) {
        w->h.x = w->parent->h.x + w->h.vel_x;
        w->h.y = w->parent->h.y + w->h.vel_y;
    }
    w->wC++;
    if (w->wC > 0x20)
        w->h.obj_flags |= 4;
}
static Obj *MineDigit_Create(Obj *arg, s32 x, s32 y, u32 idx) {
    Obj *w;
    if (idx >= 10)
        return NULL;
    w = eff_alloc(OBJ_BAC_FIX_MOJI);
    if (w == NULL)
        return NULL;
    /* spr.unk34 = Hud_GetDigitGlyphVram(n) + idx * 0x80: the HUD digit glyphs
     * are ac_fix_moji.bac animations 0..9 (Hud_Create InitDigits) */
    obj_set_anim(w, (u16)idx);
    w->spr.flags |= 0x18;
    w->spr.order = 3;
    set_prio(w, 0);
    w->h.obj_flags |= 0x10;
    w->h.disp_flags |= 0x1000;
    w->h.disp_flags |= 1;
    spr_process(&w->spr, NULL, NULL);
    w->h.vel_x = (s16)(x << 8);
    w->h.vel_y = (s16)(y << 8);
    w->h.x = arg->h.x + w->h.vel_x;
    w->h.y = arg->h.y + w->h.vel_y;
    w->parent = arg;
    w->state = MineDigit_State_FollowParent;
    w->wC = 0;
    w->w8 = 0;
    w->h.move_flags |= 0x2100;
    w->h.obj_flags |= 2;
    return w;
}
static void Mine_Explode(Obj *w) {
    Obj *t;
    if (w->h.obj_flags & 2)
        return;
    obj_sfx(0x59);
    EffZakoBomb_Create(&w->h, 0, 0);
    eff_zako_debris(w, 0, -24, 0, -0x200, 4);
    eff_zako_debris(w, 0, -24, 0, -0x200, 5);
    eff_zako_debris(w, 0, -24, 0, -0x200, 6);
    eff_zako_debris(w, 0, -24, 0, -0x200, 7);
    obj_screen_shake(4);
    fxhook(NULL, OBJ_FX_SCREEN_FLASH, 0, 0);
    t = AttackHitbox_Create(&w->h, -48, -48, 0x30, 0x30, 8);
    if (t != NULL)
        t->coll[1].mask |= 0x20;
    w->h.obj_flags |= 2;
    w->h.disp_flags |= 0x20;
    w->h.obj_flags |= 0x1000;
    g_obj.stats.mines_exploded++;
    obj_log("mine_explode", w->prm ? w->prm->type : 0, w->h.x >> 8);
}
static void Mine_State_Drop(Obj *w) {
    s16 i;
    s32 flag = 1, t;
    DcPlayer *pl = (DcPlayer *)w->target;
    if (pl != NULL && w->coll[0].owner == NULL && pl->gimmick != w) {
        w->wC++;
        if (w->wC > 0x18) {
            w->coll[0].owner = w;
            w->coll[0].flags &= ~0x1000;
            w->wC = 0;
        }
    }
    w->h.vel_y = (s16)dcp_spd_up(w->h.vel_y, -6, 0x100);
    w->k.mine.u32C += 4;
    for (i = 0; i < 16; i++) {
        if (i == 3) {
            w->k.mine.pts[i][0] = w->h.x;
            w->k.mine.pts[i][1] = w->k.mine.pts[i - 1][1];
        } else {
            if (w->k.mine.pts[i][0] == 0)
                break;
            if (i >= 4) {
                if (w->k.mine.cnt[i] == 0) {
                    w->k.mine.pts[i][1] += w->k.mine.u32C;
                    w->k.mine.pts[i][0] += w->k.mine.u32C >> 1;
                    flag = 0;
                    if (probe(w->k.mine.pts[i][0] >> 8, w->k.mine.pts[i][1] >> 8, 1, 2) < 0)
                        w->k.mine.cnt[i]++;
                } else {
                    w->k.mine.cnt[i]++;
                    flag = 0;
                    if (w->k.mine.cnt[i] > 0x60)
                        w->k.mine.pts[i][0] = 0;
                }
            } else if (i != 0) {
                w->k.mine.pts[i][0] = w->h.x;
                w->k.mine.pts[i][1] = w->k.mine.pts[i - 1][1] + 0xa00;
            } else {
                w->k.mine.pts[i][0] = w->h.x;
                w->k.mine.pts[i][1] = w->h.y + 0x1800;
            }
        }
    }
    if (w->prm->p6 != 0) {
        t = w->prm->p6 * 10 - w->w8;
        switch (t) {
        case 60: MineDigit_Create(w, 0, -0x18, 1); break;
        case 120: MineDigit_Create(w, 0, -0x18, 2); break;
        case 180: MineDigit_Create(w, 0, -0x18, 3); break;
        case 240: MineDigit_Create(w, 0, -0x18, 4); break;
        case 300: MineDigit_Create(w, 0, -0x18, 5); break;
        case 360: MineDigit_Create(w, 0, -0x18, 6); break;
        case 420: MineDigit_Create(w, 0, -0x18, 7); break;
        case 480: MineDigit_Create(w, 0, -0x18, 8); break;
        case 540: MineDigit_Create(w, 0, -0x18, 9); break;
        }
        if (t < 0)
            Mine_Explode(w);
    }
    w->w8++;
    if (probe(w->h.x >> 8, w->h.y >> 8, 1, 3) < 0)
        Mine_Explode(w);
    if (!(w->h.obj_flags & 2))
        return;
    if (flag)
        w->h.obj_flags |= 4;
}
static void Mine_Action_Drop(Obj *s) {
    s->state = Mine_State_Drop;
    s->h.vel_y = 0x60;
    s->h.move_flags &= ~0x2000u;
    s->wC = 0;
}
static void Mine_State_Main(Obj *w) {
    s32 dist = 0;
    s16 i, idx;
    w->wC++;
    idx = (s16)(((w->wC >> 4) - 1) & 7);
    w->h.shake_x = (s8)((0x9515 >> (idx * 2)) & 3); /* @0x6 */
    for (i = 0; i < 16; i++) {
        w->k.mine.pts[i][0] = w->h.x;
        idx = (s16)((i + (w->wC >> 4)) & 7);
        idx = (s16)((0x9515 >> (idx * 2)) & 3);
        w->k.mine.pts[i][0] += idx << 8;
        if (i == 3) {
            w->k.mine.pts[i][1] = w->k.mine.pts[i - 1][1];
            continue;
        }
        if (dist < 0) {
            w->k.mine.pts[i][0] = 0;
            continue;
        }
        if (i == 4)
            w->k.mine.pts[i][1] = w->k.mine.pts[i - 1][1] + 0x1e00;
        else if (i != 0)
            w->k.mine.pts[i][1] = w->k.mine.pts[i - 1][1] + 0xa00;
        else
            w->k.mine.pts[i][1] = w->h.y + 0x1800;
        dist = probe(w->k.mine.pts[i][0] >> 8, w->k.mine.pts[i][1] >> 8, 1, 2);
        if (dist < 0)
            w->k.mine.pts[i][0] = 0;
    }
    if (w->h.obj_flags & 2)
        w->h.obj_flags |= 4;
}
/* Mine_Draw: the mine, then the chain links (@0x20C) and the anchor (@0x29C)
 * at the recorded points; each StageObj_Draw2DEx call advances that sprite */
static void Mine_Draw(Obj *w) {
    s16 i;
    obj_draw2d(w);
    w->k.mine.n = 0;
    w->k.mine.s2.hidden = 1;
    for (i = 0; i < 16; i++) {
        if (i == 3) {
            if (!(w->h.disp_flags & 0x20))
                obj_draw2d_ex(w->k.mine.pts[i][0], w->k.mine.pts[i][1], NULL, &w->k.mine.s2);
        } else if (w->k.mine.pts[i][0] != 0 && (i > 3 || !(w->h.disp_flags & 0x20)) &&
                   (w->k.mine.cnt[i] < 0x3c || (w->k.mine.cnt[i] & 2))) {
            obj_draw2d_ex(w->k.mine.pts[i][0], w->k.mine.pts[i][1], NULL, &w->k.mine.s0);
            w->k.mine.mpts[w->k.mine.n][0] = w->k.mine.s0.x;
            w->k.mine.mpts[w->k.mine.n][1] = w->k.mine.s0.y;
            w->k.mine.n++;
        }
    }
    w->k.mine.s0.multi = (const s16(*)[2])w->k.mine.mpts;
    w->k.mine.s0.n_multi = w->k.mine.n;
}
static void Mine_OnHit(ObjBox *a, ObjBox *b) {
    Obj *s = (Obj *)a->owner;
    DcPlayer *t = obj_box_player(b);
    if (s == NULL || t == NULL || t->type != 1)
        return;
    Mine_Action_Drop(s);
    s->target = t;
    Player_Action_AttachToObject(t, s, 0, 0x50);
    t->pf |= 0x2000;
    t->pf2 |= 0x30;
    a->owner = NULL;
    a->flags |= 0x1000;
    count("mine_grab", s, t);
}
static void Mine_OnHitExplode(ObjBox *a, ObjBox *b) {
    Obj *s = (Obj *)a->owner;
    if (s == NULL || b->owner == NULL)
        return;
    Mine_Explode(s);
}
Obj *Mine_Create(ObjParam *p, s32 b, s32 c) {
    Obj *w = mapobj(p, b, c, OBJ_BAC_MINE, 0xf6, obj_mapobject_dtor);
    if (w == NULL)
        return NULL;
    w->kind = OBJK_ENEMY;
    w->spr.order = 0x14;
    w->fn_draw = Mine_Draw;
    obj_set_anim(w, 0);
    spr_init(&w->k.mine.s0, &g_obj.res[OBJ_BAC_MINE], 3, w->spr.flags);
    w->k.mine.s0.order = 0x14;
    spr_init(&w->k.mine.s2, &g_obj.res[OBJ_BAC_MINE], 2, w->spr.flags);
    w->k.mine.s2.order = 0x13;
    w->xspr[0] = &w->k.mine.s0;
    w->xspr[1] = &w->k.mine.s2;
    w->n_xspr = 2;
    rect_set_atkdef(&w->coll[0], 0, (u32)-0x10);
    w->coll[0].cb = Mine_OnHit;
    w->coll[0].flags |= 0x800;
    rect_set_atkdef(&w->coll[1], 0, (u32)-0x10);
    w->coll[1].cb = Mine_OnHitExplode;
    w->coll[1].flags |= 0x800;
    w->coll[1].mask = 0x33;
    map_hitbox(w, -0xc, -0xc, 0xc, 0xc);
    w->h.move_flags |= 0x2100;
    w->state = Mine_State_Main; /* Mine_Action_Init */
    return w;
}

/* --------------------------------------- Bubble (0xe8..0xea) */
static u8 Bubble_ProbeCorners(Obj *w) {
    s32 r = w->k.bub.u216;
    u8 hit = 0, ret = 0;
    s16 y = (s16)((w->h.y >> 8) - 3), x = (s16)(w->h.x >> 8);
    if (probe(x - r, y - r, 0, 3) <= 0 || probe(x - r, y - r, 1, 3) <= 0)
        hit |= 1;
    if (probe(x + r, y - r, 0, 3) <= 0 || probe(x + r, y - r, 1, 3) <= 0)
        hit |= 2;
    if (probe(x - r, y + r, 0, 3) <= 0 || probe(x - r, y + r, 1, 3) <= 0)
        hit |= 4;
    if (probe(x + r, y + r, 0, 3) <= 0 || probe(x + r, y + r, 1, 3) <= 0)
        hit |= 8;
    if (hit == 5)
        ret |= 1;
    if (hit == 3)
        ret |= 2;
    if (hit == 0xa)
        ret |= 4;
    if (hit == 0xc)
        ret |= 8;
    return ret;
}
static void Bubble_State_Float(Obj *w) {
    s32 v;
    u32 r;
    w->k.bub.u210++;
    switch (w->prm->type) {
    case 0xe8:
        if ((w->h.obj_flags & 2) && w->k.bub.u210 >= 10)
            w->h.obj_flags &= ~2u;
        if (!(frame_rand() & 0x1f))
            w->h.vel_x = (s16)(w->h.vel_x + (s16)(0xf - (RAND() & 0x1e)));
        v = w->h.vel_y;
        if (v < 0)
            v = -v;
        if (v < 0xd0)
            w->h.vel_y -= 2;
        if (w->k.bub.u210 >= 300)
            w->h.vel_y -= 1;
        break;
    case 0xe9:
        w->h.vel_x = (s16)(((obj_sin((u16)(w->k.bub.u210 << 11)) << 7) >> 12) / 3);
        break;
    case 0xea:
        w->h.vel_x = (s16)((obj_sin((u16)(w->k.bub.u210 << 12)) << 6) >> 12);
        break;
    }
    if ((w->h.y >> 8) - w->k.bub.u216 <= w->k.bub.u214 + 3) {
        w->h.vel_y = 0;
        w->h.obj_flags |= 4;
        return;
    }
    r = Bubble_ProbeCorners(w);
    if (r & 2) {
        w->h.obj_flags |= 4;
        return;
    }
    if (r & 5)
        w->h.vel_x = (s16)-w->h.vel_x;
}
static void Bubble_Action_Float(Obj *w) {
    switch (w->prm->type) {
    case 0xe8:
        break;
    case 0xe9:
        obj_set_anim(w, 2);
        w->h.vel_y = (s16)(-0x101 - (RAND() & 0x7e));
        break;
    case 0xea:
        obj_set_anim(w, 3);
        w->h.vel_y = (s16)(-0x131 - (RAND() & 0x7e));
        break;
    }
    w->h.disp_flags |= 4;
    w->h.move_flags |= 0x10;
    w->state = Bubble_State_Float;
}
static void Bubble_State_Grow(Obj *w) {
    w->k.bub.u210++;
    w->scale_x = (s16)((w->k.bub.u210 << 8) / 32);
    w->scale_y = (s16)((w->k.bub.u210 << 8) / 32);
    if (w->k.bub.u210 < 0x20)
        return;
    w->k.bub.u210 = 0;
    w->scale_x = 0x100;
    w->scale_y = 0x100;
    w->h.vel_y = 0;
    Bubble_Action_Float(w);
}
static void Bubble_Action_Spawn(Obj *s) {
    obj_set_anim(s, 0);
    s->h.disp_flags |= 4;
    s->h.move_flags |= 0x10;
    s->scale_x = 0;
    s->scale_y = 0;
    s->h.vel_y = (s16)((-0xc1 - (RAND() & 0x1e)) >> 1);
    s->h.obj_flags |= 2;
    s->state = Bubble_State_Grow;
}
static void Bubble_OnHit(ObjBox *a, ObjBox *b) {
    Obj *o = (Obj *)a->owner;
    DcPlayer *p = obj_box_player(b);
    if (o == NULL)
        return;
    if (p == NULL)
        return; /* (the matched code reads byte 0 of a NULL toucher; only players reach list 1) */
    Player_Action_BreatheAirBubble(p);
    o->h.obj_flags |= 4;
    g_obj.stats.air_bubbles++;
    obj_log("air_bubble", o->prm->type, p->x >> 8);
}
Obj *Bubble_Create(ObjParam *p, s32 b, s32 c) {
    Obj *w = mapobj(p, b, c, OBJ_BAC_BUBBLE, OBJ_PRIO_MAPOBJ, obj_mapobject_dtor);
    if (w == NULL)
        return NULL;
    w->kind = OBJK_EFFECT;
    w->spr.order = 0x14;
    rect_set_atkdef(&w->coll[0], 0, (u32)-0x10);
    w->h.move_flags |= 0x100;
    switch (w->prm->type) {
    case 0xe8:
        map_hitbox(w, -10, -10, 10, 10);
        w->k.bub.u216 = 10;
        w->coll[0].cb = Bubble_OnHit;
        w->coll[0].flags |= 0x800;
        break;
    case 0xe9:
        map_hitbox(w, -2, -2, 2, 2);
        w->k.bub.u216 = 2;
        w->coll[0].cb = NULL;
        break;
    case 0xea:
        map_hitbox(w, -1, -1, 1, 1);
        w->k.bub.u216 = 1;
        w->coll[0].cb = NULL;
        break;
    }
    if (g_obj_camera[2].flags & 1)
        w->k.bub.u214 = g_obj_camera[0].water;
    else if (g_obj_camera[0].flags & 0x1000000)
        w->k.bub.u214 = g_obj_camera[0].water;
    else if (g_obj_camera[1].flags & 0x1000000)
        w->k.bub.u214 = g_obj_camera[1].water;
    else
        w->k.bub.u214 = 0;
    if (w->prm->type == 0xe8)
        Bubble_Action_Spawn(w);
    else
        Bubble_Action_Float(w);
    return w;
}

/* ------------------------------------ BubbleSpawner (0x84) */
static void BubbleSpawner_SpawnBubble(Obj *w, s32 type, s32 x, s32 y) {
    (void)w;
    if (type != 0xe8 && type != 0xe9 && type != 0xea)
        type = 0xea;
    obj_mapobject_spawn((s16)type, x, y, 0, 0, 0, 0, 0);
}
static void BubbleSpawner_State_Emit(Obj *w) {
    u32 f = w->k.bub.f20C;
    s16 rx, ry;
    s32 bx, by;
    if ((f & 2) && !(f & 1))
        w->k.bub.f20C = f & ~2u;
    else if (!(f & 2) && (f & 1))
        w->k.bub.f20C |= 2;
    w->k.bub.f20C &= ~1u;
    if (w->k.bub.u210 < 0xb2) {
        if ((frame_rand() & 0x3f) == 0) {
            ry = (s16)(7 - (RAND() & 0xe)); /* arguments drawn right to left (MWCC) */
            rx = (s16)(7 - (RAND() & 0xe));
            EffAirBubble_Create(&w->h, rx, ry, 0, w->k.bub.u214);
        }
        if (!((frame_rand() & 0x1f) - 1)) {
            ry = (s16)(7 - (RAND() & 0xe));
            rx = (s16)(7 - (RAND() & 0xe));
            EffAirBubble_Create(&w->h, rx, ry, 1, w->k.bub.u214);
        }
    }
    w->k.bub.u210--;
    if (!(w->k.bub.f20C & 2))
        return;
    if (w->k.bub.u210 < 0) {
        w->k.bub.u210 = 0xd2;
        by = w->h.y + (0x3ff - (RAND() & 0x7fe));
        bx = w->h.x + (0x3ff - (RAND() & 0x7fe));
        BubbleSpawner_SpawnBubble(w, 0xe8, bx, by);
        g_obj.stats.bubbles_spawned++;
    }
}
static void BubbleSpawner_OnHit_Box1(ObjBox *a, ObjBox *b) {
    (void)b;
    ((Obj *)a->owner)->k.bub.f20C |= 1;
}
Obj *BubbleSpawner_Create(ObjParam *p, s32 b, s32 c) {
    Obj *w = mapobj(p, b, c, OBJ_BAC_BUBBLE, OBJ_PRIO_MAPOBJ, obj_mapobject_dtor);
    if (w == NULL)
        return NULL;
    map_hitbox(w, -6, -6, 6, 6);
    w->spr.order = 0x14;
    w->coll[0].atk = 0;
    w->coll[0].def = 7;
    rect_set_box2d(&w->coll[1], -0x50, -0x180, 0x50, 0xc0);
    rect_set_atkdef(&w->coll[1], 0, (u32)-16);
    w->coll[1].flags |= 0xc0;
    w->coll[1].cb = BubbleSpawner_OnHit_Box1;
    w->coll[1].owner = w;
    w->coll[1].owner_kind = OBJ_OWNER_OBJECT;
    w->h.move_flags |= 0x100;
    /* BubbleSpawner_Action_Init */
    obj_set_anim(w, 1);
    w->h.disp_flags |= 4;
    w->h.move_flags |= 0x10;
    w->state = BubbleSpawner_State_Emit;
    return w;
}

/* ------------------------------------------ WaterJet (0x85..0x87) */
static void WaterJet_State_Wait(Obj *s);
static void WaterJet_State_Prepare(Obj *s);
static void WaterJet_State_Spout(Obj *s);
static void WaterJet_Action_Wait(Obj *w) {
    obj_set_anim(w, 0);
    w->h.disp_flags &= ~4;
    w->h.disp_flags |= 0x10;
    w->wC = (u16)(w->prm->p8 * 0x64 + w->prm->p9);
    w->state = WaterJet_State_Wait;
}
static void WaterJet_Action_Prepare(Obj *w) {
    w->h.disp_flags &= ~0x10;
    w->h.disp_flags |= 4;
    w->wC = 0x50;
    w->state = WaterJet_State_Prepare;
    obj_sfx(0x56);
}
static void WaterJet_Action_Spout(Obj *w) {
    obj_set_anim(w, 1);
    w->coll[1].flags &= ~0x80;
    w->wC = 0x28;
    w->k.bub.f20C |= 1;
    w->state = WaterJet_State_Spout;
    obj_sfx(0x57);
}
static void WaterJet_State_Wait(Obj *s) {
    s->wC--;
    if (s->wC != 0)
        return;
    WaterJet_Action_Prepare(s);
}
static void WaterJet_State_Prepare(Obj *s) {
    s->wC--;
    if (s->wC != 0)
        return;
    WaterJet_Action_Spout(s);
}
static void WaterJet_State_Spout(Obj *w) {
    w->wC--;
    if (w->wC == 0)
        w->k.bub.f20C &= ~1u;
    if (!(w->h.disp_flags & 8))
        return;
    WaterJet_Action_Wait(w);
}
static void WaterJet_OnHit(ObjBox *a, ObjBox *b) {
    Obj *o = (Obj *)a->owner;
    DcPlayer *p = box_hdr(b);
    s16 vx = 0, vy = 0, v;
    u16 id;
    if (o == NULL || p == NULL)
        return;
    if (!(o->k.bub.f20C & 1))
        return;
    v = -0x780;
    v = (s16)(v - o->prm->p6 * 0x180);
    id = o->prm->type;
    if (id == 0x85)
        vy = v;
    else if (id == 0x86)
        vx = v;
    else
        vx = (s16)-v;
    dcp_act_spring(p, vx, vy);
    obj_player_set_source(p, o);
    count("water_jet", o, p);
}
Obj *WaterJet_Create(ObjParam *p, s32 b, s32 c) {
    Obj *w = mapobj(p, b, c, OBJ_BAC_WATER_JET, OBJ_PRIO_MAPOBJ, obj_mapobject_dtor);
    if (w == NULL)
        return NULL;
    w->spr.order = 0x14;
    rect_set_atkdef(&w->coll[0], 0, (u32)-0x10);
    w->coll[0].cb = NULL;
    w->coll[0].flags |= 0x800;
    w->h.move_flags |= 0x2100;
    switch (w->prm->type) {
    case 0x85:
        rect_set_box2d(&w->coll[1], 0xc, -0x40, 0x24, 0);
        break;
    case 0x86:
        rect_set_box2d(&w->coll[1], -0x40, -0xc, 0, -0x24);
        w->h.disp_flags |= 1;
        w->h.disp_flags |= 0x100;
        w->h.angle = 0xc0;
        break;
    case 0x87:
        rect_set_box2d(&w->coll[1], -0x40, -0xc, 0, -0x24);
        w->h.disp_flags &= ~1;
        w->h.disp_flags |= 0x100;
        w->h.angle = 0x40;
        break;
    }
    rect_set_atkdef(&w->coll[1], 0, (u32)-0x10);
    w->coll[1].flags |= 0xc0;
    w->coll[1].flags |= 0x800;
    w->coll[1].cb = WaterJet_OnHit;
    w->coll[1].owner = w;
    w->coll[1].owner_kind = OBJ_OWNER_OBJECT;
    w->snd = 1;
    WaterJet_Action_Wait(w);
    return w;
}

/* ============================================================ Kuragen */
static void KuragenBomb_State_Exploded(Obj *w) { (void)w; }
static void KuragenBomb_Action_Explode(Obj *w) {
    if (w->h.obj_flags & 4)
        return;
    obj_sfx(0x59);
    obj_screen_shake(4);
    fxhook(NULL, OBJ_FX_SCREEN_FLASH, 0, 0);
    EffZakoBomb_Create(&w->h, 0, -32);
    AttackHitbox_Create(&w->h, -16, -48, 0x10, 0, 8);
    w->state = KuragenBomb_State_Exploded;
    w->h.obj_flags |= 4;
    obj_log("kuragen_bomb", 0xe3, w->h.x >> 8);
}
static void KuragenBomb_State_Fall(Obj *w) {
    if (probe(w->h.x >> 8, w->h.y >> 8, 1, 2) >= 0)
        return;
    KuragenBomb_Action_Explode(w);
}
static void KuragenBomb_Action_Drop(Obj *w) {
    s32 t;
    obj_set_anim(w, 1);
    w->h.vel_y = 0x280;
    t = w->prm->flags & 0xc;
    if (t != 0)
        w->h.vel_x = (s16)((w->h.vel_x * (((t >> 2) << 7) + 0x100)) >> 8);
    w->h.disp_flags |= 4;
    w->state = KuragenBomb_State_Fall;
}
static void KuragenBomb_State_FollowCarrier(Obj *w) {
    Obj *t = w->parent;
    if (t == NULL)
        return;
    w->h.x = t->h.x;
    w->h.y = t->h.y + 0xf00;
    if (!(t->k.kur.u238 & 0x20))
        return;
    KuragenBomb_Action_Drop(w);
    t->k.kur.u238 &= ~0x20u;
    w->h.obj_flags |= 0x200;
}
static void KuragenBomb_Action_FollowCarrier(Obj *w) {
    obj_set_anim(w, 1);
    w->h.disp_flags |= 4;
    if (w->parent == NULL) {
        w->h.x = 0;
        w->h.y = 0xf00;
    } else {
        w->h.x = w->parent->h.x;
        w->h.y = w->parent->h.y + 0xf00;
    }
    w->state = KuragenBomb_State_FollowCarrier;
}
static void KuragenBomb_State_Hold(Obj *w) {
    Obj *p = w->parent;
    w->wC--;
    if (w->wC == 0) {
        if (p != NULL)
            p->k.kur.u238 |= 0x100;
        KuragenBomb_Action_FollowCarrier(w);
        return;
    }
    if (p == NULL)
        return;
    w->h.x = p->h.x;
    w->h.y = p->h.y + (((0x3c - w->wC) * 0xf) << 8) / 60;
}
static void KuragenBomb_Action_Hold(Obj *w) {
    obj_set_anim(w, 1);
    w->h.disp_flags |= 4;
    w->wC = 0x3c;
    w->state = KuragenBomb_State_Hold;
}
static void KuragenBomb_Attach(Obj *p) {
    Obj *o = p->parent;
    obj_set_anim(p, 1);
    p->h.disp_flags |= 4;
    if (o->k.kur.u238 & 0x80) {
        o->k.kur.u238 &= ~0x80u;
        KuragenBomb_Action_Hold(p);
    } else {
        KuragenBomb_Action_FollowCarrier(p);
    }
}
static void KuragenBomb_OnHit_Box1(ObjBox *a, ObjBox *b) {
    if (b->owner == NULL)
        return;
    KuragenBomb_Action_Explode((Obj *)a->owner);
}
static void KuragenBomb_Destructor(Obj *s) {
    Obj *p = s->parent;
    if (p != NULL && p->used)
        p->k.kur.u238 |= 0x40;
    obj_mapobject_dtor(s);
}
Obj *KuragenBomb_Create(ObjParam *p, s32 b, s32 c) {
    Obj *w = mapobj(p, b, c, OBJ_BAC_KURAGEN_BOMB, 0x501, KuragenBomb_Destructor);
    if (w == NULL)
        return NULL;
    w->kind = OBJK_ENEMY;
    map_hitbox(w, -6, -4, 6, 4);
    w->spr.order = 0x15;
    rect_set_atkdef(&w->coll[1], 0x13, (u32)-16);
    w->coll[1].cb = KuragenBomb_OnHit_Box1;
    w->h.move_flags |= 0x100;
    w->snd = 1;
    return w;
}
static void Kuragen_SpawnBomb(Obj *p) {
    Obj *e;
    u16 f = 0;
    f |= p->prm->flags & 0xc;
    e = obj_mapobject_spawn(0xe3, p->h.x, p->h.y, (s16)f, 0, 0, 0, 0);
    if (e == NULL)
        return; /* (the original assumes the spawn succeeds) */
    e->parent = p;
    KuragenBomb_Attach(e);
}
static void Kuragen_UpdatePulse(Obj *s) {
    if (s->prm->type != 6)
        return;
    s->wC--;
    if (s->wC != 0)
        return;
    if (s->k.kur.u238 & 0x1000) {
        s->wC = 0xb4;
        obj_set_anim(s, 0);
        s->coll[0].cb = s->k.kur.cb;
    } else {
        s->wC = 0x5a;
        obj_set_anim(s, 1);
        s->coll[0].cb = NULL;
    }
    s->k.kur.u238 ^= 0x1000;
    s->h.disp_flags |= 4;
}
static void Kuragen_State_Float(Obj *w);
static void Kuragen_Action_Float(Obj *w) {
    if (w->k.kur.u238 & 0x10000)
        w->k.kur.u238 &= ~0x10000u;
    else
        obj_set_anim(w, 0);
    w->h.disp_flags |= 4;
    w->h.move_flags |= 0x10;
    w->state = Kuragen_State_Float;
}
static void Kuragen_State_FloatTurn(Obj *p) {
    s16 spd = 0x100;
    s32 v = p->prm->flags & 0xc;
    if (v)
        spd = (s16)(spd + ((v >> 2) << 7));
    p->h.vel_y = (s16)dcp_spd_down(p->h.vel_y, (spd << 5) >> 8);
    p->k.kur.u238 ^= 0x10;
    p->k.kur.u238 |= 0x10000;
    Kuragen_Action_Float(p);
}
static void Kuragen_State_Float(Obj *w) {
    s16 sp = 0x100;
    s32 f;
    Kuragen_UpdatePulse(w);
    f = w->prm->flags & 0xc;
    if (f)
        sp = (s16)(sp + ((f >> 2) << 7));
    if (w->k.kur.u238 & 0x10) {
        w->h.vel_y = (s16)dcp_spd_up(w->h.vel_y, -((sp * 5) >> 8), (sp * 0x60) >> 8);
        if (w->h.y < (w->home_y + w->prm->p7) << 8) {
            w->state = Kuragen_State_FloatTurn; /* Kuragen_Action_FloatTurn */
            return;
        }
    } else {
        w->h.vel_y = (s16)dcp_spd_up(w->h.vel_y, (sp * 5) >> 8, (sp * 0x60) >> 8);
        if (w->h.y > (w->home_y + w->prm->p7 + w->prm->p9) << 8) {
            w->state = Kuragen_State_FloatTurn;
            return;
        }
    }
    if (!(w->k.kur.u238 & 1)) {
        if (!(w->k.kur.u238 & 0x100))
            return;
        w->k.kur.u238 &= ~0x100u;
        w->k.kur.u238 |= 1;
        return;
    }
    obj_handle_collider(&w->h, w, OBJ_OWNER_OBJECT, &w->detect, 1);
}
static void Kuragen_State_BombDrop(Obj *w) {
    if (w->k.kur.u23C == 0) {
        if (w->spr.frame_no == 4) {
            w->h.disp_flags |= 0x10;
            w->wC = 1;
            w->k.kur.u23C = 1;
        }
    } else if (w->k.kur.u23C == 1) {
        w->wC--;
        if (w->wC == 0) {
            w->h.disp_flags &= ~0x10;
            w->spr.speed = (s16)(w->spr.speed << 1); /* @0xBE (spr1 @0x106: not kept) */
            w->k.kur.u238 |= 0x20;
            w->k.kur.u23C = 2;
        }
    } else if (w->k.kur.u23C == 2) {
        if (w->spr.frame_no == 2 && !(w->h.disp_flags & 0x10)) {
            w->h.disp_flags |= 0x10;
            w->spr.speed = (s16)(w->spr.speed >> 1);
        }
        if ((w->k.kur.u238 & 0x40) && w->spr.frame_no == 2) {
            w->k.kur.u238 &= ~0x40u;
            w->k.kur.u238 |= 2;
            w->wC = 0x3c;
            w->k.kur.u23C = 3;
        }
    } else if (w->k.kur.u23C == 3) {
        w->wC--;
        if (w->wC == 0) {
            w->h.disp_flags &= ~0x10;
            w->k.kur.u238 &= ~3u;
            w->k.kur.u238 |= 0x80;
            Kuragen_SpawnBomb(w);
            w->k.kur.u238 |= 0x10000;
            w->k.kur.u238 &= ~1u;
            Kuragen_Action_Float(w);
        }
    }
}
static void Kuragen_OnDetect(ObjBox *a, ObjBox *b) {
    Obj *o = (Obj *)a->owner;
    void *t = b->owner;
    if (t == NULL)
        return;
    if (o->prm->type != 5)
        return;
    EffFound_Create(o, 0, -0x20);
    o->target = t;
    /* Kuragen_Action_StartBombDrop */
    o->state = Kuragen_State_BombDrop;
    o->h.vel_y = 0;
    o->k.kur.u23C = 0;
    o->k.kur.u238 &= ~3u;
    g_obj.stats.kuragen_drops++;
    obj_log("kuragen_drop", 5, o->h.x >> 8);
}
Obj *Kuragen_Create(ObjParam *p, s32 b, s32 c) {
    Obj *w = mapobj(p, b, c, -1, OBJ_PRIO_MAPOBJ, obj_mapobject_dtor);
    if (w == NULL)
        return NULL;
    w->kind = OBJK_ENEMY;
    map_hitbox(w, 0xc, -12, 0xc, 0x14);
    switch (w->prm->type) {
    case 5:
        obj_load_bac(w, OBJ_BAC_KURAGEN_BOMB);
        break;
    case 6:
        obj_load_bac(w, OBJ_BAC_KURAGEN_BLITZ);
        break;
    }
    w->spr.order = 0x14;
    rect_set_box2d(&w->detect, -0x40, 0, 0x40, 0x170);
    w->detect.flags |= 0xc0;
    rect_set_atkdef(&w->detect, 0, (u32)-16);
    w->detect.mask = 0x12;
    w->detect.cb = Kuragen_OnDetect;
    w->detect.owner = w;
    w->detect.owner_kind = OBJ_OWNER_OBJECT;
    if (w->prm->type == 5) {
        w->k.kur.u238 |= 1;
        Kuragen_SpawnBomb(w);
    } else {
        w->wC = 0xb4;
        w->k.kur.cb = w->coll[0].cb;
    }
    w->h.move_flags |= 0x100;
    Kuragen_Action_Float(w);
    return w;
}

/* ========================================================== decorations */
/* DecShared_* (src/game/Decorations): one hidden animation driver per
 * shared sprite; the decorations using it show its frame (spr.vram) */
static Obj *s_dec_shared[16]; /* gDecSharedObjects */
static void DecShared_Destructor(Obj *s) {
    s32 i = s->w8;
    if (i >= 0 && i < 16 && s == s_dec_shared[i])
        s_dec_shared[i] = NULL;
    obj_stageobj_dtor(s);
}
static void DecShared_State_Main(Obj *p) {
    if (p->wC == 0) {
        p->h.obj_flags |= 4;
        s_dec_shared[p->w8] = NULL;
    }
    p->wC = 0;
}
static void DecShared_State_PingParent(Obj *s);
void dec_shared_ping(Obj *s) { /* DecShared_State_PingParent */
    DecShared_State_PingParent(s);
}
static void DecShared_State_PingParent(Obj *s) {
    if (s->parent != NULL)
        s->parent->wC++;
    else
        s->spr.vram = NULL; /* (native: the driver is gone, show the own frame) */
}
static Obj *DecShared_Create(int bac, s32 c, u16 e) {
    Obj *w;
    s32 i = e;
    if (s_dec_shared[i] != NULL && s_dec_shared[i]->used)
        return s_dec_shared[i];
    w = obj_alloc(OBJ_PRIO_RING, DecShared_Destructor);
    if (w == NULL)
        return NULL;
    s_dec_shared[i] = w;
    obj_load_bac(w, bac);
    w->kind = OBJK_DECOR;
    obj_set_anim(w, (u16)c);
    w->h.move_flags |= 0x2100;
    w->h.disp_flags |= 0x20;
    w->h.disp_flags |= 4;
    w->h.obj_flags |= 0x10;
    w->wC = 1;
    w->w8 = (u16)e;
    w->state = DecShared_State_Main;
    w->spr.flags |= 0x10;
    /* AnimatorSprite_AnimateManual(spr, (frame & 0xf) << 4): advance by that many ticks */
    w->spr.flags |= SPR_LOOP;
    w->spr.speed = (s16)((W.frame & 0xf) << 4);
    spr_process(&w->spr, NULL, NULL);
    w->spr.speed = 0x10;
    return w;
}
void dec_shared_reset(void) { memset(s_dec_shared, 0, sizeof(s_dec_shared)); }
Obj *dec_shared_create(int bac, s32 anim, u16 slot) { return DecShared_Create(bac, anim, slot); }

/* DecFishPart_* (fish of a school) */
static void DecFishPart_State_Swim(Obj *w);
static void DecFishPart_State_Flee(Obj *w);
static void DecFishPart_Action_Swim(Obj *w) {
    w->state = DecFishPart_State_Swim;
    w->h.disp_flags |= 4;
    w->h.move_flags |= 0x2000;
    w->h.move_flags |= 0x100;
    w->h.move_flags |= 0x80;
    w->h.obj_flags |= 2;
    w->h.obj_flags |= 0x200;
    w->w8 = (s32)(((((u32)((w->h.rot3 + (W.frame + w->h.rot2)) << 16) * 0x196225u + 0x3c6ef35fu) >> 16) & 0xff) + 0x80);
}
static void DecFishPart_Action_Flee(Obj *w, const DcPlayer *p) {
    u16 a;
    if (w->spr.anim & 1)
        obj_set_anim(w, (u16)(w->spr.anim - 1));
    w->state = DecFishPart_State_Flee;
    w->h.disp_flags |= 4;
    w->h.obj_flags |= 2;
    w->h.move_flags &= ~0x80u;
    w->h.move_flags &= ~0x2000u;
    a = dcp_atan2(w->h.y - p->y, w->h.x - p->x);
    w->h.vel_x = (s16)((obj_cos(a) << 10) >> 12);
    w->h.vel_y = (s16)((obj_sin(a) << 10) >> 12);
    if (w->h.vel_x > 0)
        w->h.disp_flags &= ~1;
    else
        w->h.disp_flags |= 1;
}
static void DecFishPart_State_Flee(Obj *w) {
    s32 t3, v;
    if (w->parent != NULL && w->parent->target == NULL) {
        w->h.vel_x = (s16)dcp_spd_down(w->h.vel_x, 0x20);
        w->h.vel_y = (s16)dcp_spd_down(w->h.vel_y, 0x20);
    }
    t3 = (CAM0.water + 8) << 8;
    if (w->h.y < t3)
        w->h.y = t3;
    v = w->h.vel_x < 0 ? -w->h.vel_x : w->h.vel_x;
    if (v >= 0x40)
        return;
    v = w->h.vel_y < 0 ? -w->h.vel_y : w->h.vel_y;
    if (v >= 0x40)
        return;
    DecFishPart_Action_Swim(w);
}
static void DecFishPart_State_Swim(Obj *w) {
    Obj *t = w->parent;
    s32 min;
    s8 v;
    if (t != NULL) {
        if (!(W.frame & 0xf)) {
            w->h.rot2 = (u8)(w->h.rot2 + 8 - (RAND() & 0xf));
            w->h.rot3 = (u8)(w->h.rot3 + 8 - (RAND() & 0xf));
            v = (s8)w->h.rot2;
            if (v > 0x20)
                w->h.rot2 = 0x20;
            if (v < -0x20)
                w->h.rot2 = (u8)-0x20;
            v = (s8)w->h.rot3;
            if (v > 0x20)
                w->h.rot3 = 0x20;
            if (v < -0x20)
                w->h.rot3 = (u8)-0x20;
        }
        w->h.px = w->h.x;
        w->h.py = w->h.y;
        w->h.x = ObjShift_Set(w->h.x, t->h.x + ((s8)w->h.rot2 << 8), 4, (u16)w->w8, 4);
        w->h.y = ObjShift_Set(w->h.y, t->h.y + ((s8)w->h.rot3 << 8), 4, (u16)w->w8, 4);
        min = (CAM0.water + 8) << 8;
        if (w->h.y < min)
            w->h.y = min;
        if (w->spr.anim & 1) {
            if (w->h.disp_flags & 8) {
                obj_set_anim(w, (u16)(w->spr.anim - 1));
                w->h.disp_flags |= 4;
                w->h.disp_flags ^= 1;
            }
        } else if (w->h.disp_flags & 1) {
            if (w->h.px < w->h.x - 0x60)
                obj_set_anim(w, (u16)(w->spr.anim + 1));
        } else {
            if (w->h.px > w->h.x + 0x60)
                obj_set_anim(w, (u16)(w->spr.anim + 1));
        }
        if (t->target != NULL)
            DecFishPart_Action_Flee(w, (const DcPlayer *)t->target);
    } else {
        if (g_obj.n_players)
            DecFishPart_Action_Flee(w, g_obj.players[0].p);
    }
}
static void DecFishPart_Create(Obj *arg, s32 x, s32 y) {
    Obj *w = eff_alloc(OBJ_BAC_DEC_FISH);
    if (w == NULL)
        return;
    w->kind = OBJK_DECOR;
    set_prio(w, 3);
    w->spr.order = 0x16;
    obj_set_anim(w, 4);
    w->h.x = arg->h.x;
    w->h.y = arg->h.y;
    w->parent = arg;
    w->h.x += x << 8;
    w->h.y += y << 8;
    w->h.rot2 = (u8)x;
    w->h.rot3 = (u8)y;
    w->h.disp_flags |= arg->h.disp_flags & 1;
    DecFishPart_Action_Swim(w);
}

/* DecGroup41_* (fountain, waterfalls, wheel, water spray, algae, coral, fish) */
static void DecGroup41_ChooseNextAction(Obj *w);
static void DecGroup41_State_Sink(Obj *w) {
    s32 v;
    w->h.vel_x = (s16)dcp_spd_down(w->h.vel_x, 8);
    if (w->wC == 0) {
        DecGroup41_ChooseNextAction(w);
        return;
    }
    w->wC--;
    if (w->h.y > w->home_y * 0x100 + 0x4000) {
        DecGroup41_ChooseNextAction(w);
        return;
    }
    v = (CAM0.water + 8) << 8;
    if (w->h.y < v)
        w->h.y = v;
}
static void DecGroup41_Action_Sink(Obj *w) {
    w->state = DecGroup41_State_Sink;
    w->h.disp_flags |= 4;
    w->h.move_flags &= ~0x2000u;
    w->h.move_flags |= 0x100;
    w->h.move_flags |= 0x80;
    w->h.gravity = 3;
    w->h.fall_max = 0x60;
    if (w->prm->type >= 0xab) {
        w->h.gravity = 2;
        w->h.fall_max >>= 1;
    }
    w->wC = 0x4f - (RAND() & 0x3e);
}
static void DecGroup41_State_Dart(Obj *w) {
    s32 v;
    if (w->wC != 0)
        w->wC--;
    else
        w->h.vel_x = (s16)dcp_spd_down(w->h.vel_x, 8);
    v = w->h.vel_x < 0 ? -w->h.vel_x : w->h.vel_x;
    if (v < 0x40) {
        DecGroup41_ChooseNextAction(w);
        return;
    }
    v = (CAM0.water + 8) << 8;
    if (w->h.y < v)
        w->h.y = v;
}
static void DecGroup41_Action_Dart(Obj *w, s16 x, s16 y, s32 c) {
    w->state = DecGroup41_State_Dart;
    w->h.disp_flags |= 4;
    w->h.vel_x = x;
    w->h.vel_y = y;
    w->wC = c;
    if (w->prm->type < 0xab || w->prm->type >= 0xad)
        return;
    w->h.vel_x = (s16)(w->h.vel_x >> 1);
    w->h.vel_y = (s16)(w->h.vel_y >> 1);
}
static void DecGroup41_State_Turn(Obj *w) {
    s16 a, b;
    s32 c;
    w->h.vel_x = (s16)dcp_spd_down(w->h.vel_x, 8);
    if (!(w->h.disp_flags & 8))
        return;
    obj_set_anim(w, (u16)(w->spr.anim - 1));
    w->h.disp_flags ^= 1;
    if (!(RAND() & 3)) {
        DecGroup41_Action_Sink(w);
        return;
    }
    c = RAND() & 7; /* arguments drawn right to left (MWCC) */
    b = (s16)(0xff - (RAND() & 0x1fe));
    if (w->h.disp_flags & 1)
        a = (s16)(-0x81 - (RAND() & 0xfe));
    else
        a = (s16)(0x17f - (RAND() & 0xfe));
    DecGroup41_Action_Dart(w, a, b, c);
}
static void DecGroup41_Action_Turn(Obj *s) {
    s->state = DecGroup41_State_Turn;
    obj_set_anim(s, (u16)(s->spr.anim + 1));
}
static void DecGroup41_ChooseNextAction(Obj *w) {
    s16 r, b = 6, c = 6, a = 2, d = 2, x, y;
    s32 n;
    r = (s16)(RAND() & 0xf);
    if (w->h.y > (w->home_y << 8) + 0x2000) {
        a += b;
        b = 0;
    }
    if ((w->h.x < (w->home_x << 8) - 0x3000 && (w->h.disp_flags & 1)) ||
        (w->h.x > (w->home_x << 8) + 0x3000 && !(w->h.disp_flags & 1))) {
        d += c + a;
        c = 0;
        a = 0;
    }
    if (w->h.y < (w->home_y << 8) - 0x2000) {
        b += c + a + d;
        d = 0;
        a = 0;
        c = 0;
    }
    r -= b;
    if (r < 0) {
        DecGroup41_Action_Sink(w);
        return;
    }
    r -= c;
    if (r < 0) {
        n = RAND() & 7;
        y = (s16)(0xff - (RAND() & 0x1fe));
        if (w->h.disp_flags & 1)
            x = (s16)(-0x81 - (RAND() & 0xfe));
        else
            x = (s16)(0x17f - (RAND() & 0xfe));
        DecGroup41_Action_Dart(w, x, y, n);
        return;
    }
    r -= a;
    if (r < 0) {
        n = RAND() & 7;
        y = (s16)(-0xc1 - (RAND() & 0x7e));
        if (w->h.disp_flags & 1)
            x = (s16)(-0x81 - (RAND() & 0x1fe));
        else
            x = (s16)(0x27f - (RAND() & 0x1fe));
        DecGroup41_Action_Dart(w, x, y, n);
        return;
    }
    r -= d;
    if (r < 0)
        DecGroup41_Action_Turn(w);
}
static void DecGroup41_State_Cruise(Obj *w) {
    s32 v;
    w->wC++;
    v = (CAM0.water + 8) << 8;
    if (w->h.y < v)
        w->h.y = v;
    if (w->wC & 0x1f)
        return;
    if (RAND() & 3)
        return;
    w->h.vel_y = (s16)(0xff - (RAND() & 0x1fe));
}
static void DecGroup41_Action_Cruise(Obj *w) {
    s32 min;
    w->state = DecGroup41_State_Cruise;
    w->h.disp_flags |= 4;
    w->h.move_flags |= 0x100;
    if (CAM0.x + 0x8000 > w->h.x)
        w->h.disp_flags &= ~1;
    else
        w->h.disp_flags |= 1;
    w->h.vel_x = 0x2a0;
    w->h.vel_y = (s16)(0xff - (RAND() & 0x1fe));
    if (w->h.disp_flags & 1)
        w->h.vel_x = (s16)-w->h.vel_x;
    min = (CAM0.water + 8) << 8;
    if (w->h.y < min)
        w->h.y = min;
}
static void DecGroup41_State_Flee(Obj *w) {
    s32 v;
    if (w->wC != 0) {
        w->wC--;
    } else {
        w->h.vel_x = (s16)dcp_spd_down(w->h.vel_x, 0x20);
        w->h.vel_y = (s16)dcp_spd_down(w->h.vel_y, 0x20);
        w->target = NULL;
    }
    v = (CAM0.water + 8) << 8;
    if (w->h.y < v)
        w->h.y = v;
    v = w->h.vel_x < 0 ? -w->h.vel_x : w->h.vel_x;
    if (v >= 0x40)
        return;
    v = w->h.vel_y < 0 ? -w->h.vel_y : w->h.vel_y;
    if (v >= 0x40)
        return;
    w->h.move_flags |= 0x80;
    w->h.obj_flags &= ~2u;
    DecGroup41_ChooseNextAction(w);
}
static void DecGroup41_Action_Flee(Obj *w, const DcPlayer *o) {
    u16 a;
    if (w->spr.anim & 1)
        obj_set_anim(w, (u16)(w->spr.anim - 1));
    w->state = DecGroup41_State_Flee;
    w->h.disp_flags |= 4;
    w->h.obj_flags |= 2;
    w->h.move_flags &= ~0x80u;
    a = dcp_atan2(w->h.y - o->y, w->h.x - o->x);
    w->h.vel_x = (s16)((obj_cos(a) * 0x400) >> 12);
    w->h.vel_y = (s16)((obj_sin(a) * 0x400) >> 12);
    if (w->h.vel_x > 0)
        w->h.disp_flags &= ~1;
    else
        w->h.disp_flags |= 1;
    w->wC = 0xe;
    if (w->prm->type >= 0xab && w->prm->type < 0xad) {
        w->h.vel_x = (s16)(w->h.vel_x >> 1);
        w->h.vel_y = (s16)(w->h.vel_y >> 1);
    }
}
static void DecGroup41_OnHit(ObjBox *a, ObjBox *b) {
    Obj *w = (Obj *)a->owner;
    DcPlayer *p = box_hdr(b);
    s32 v;
    if (p == NULL)
        return;
    v = p->move_x < 0 ? -p->move_x : p->move_x;
    if (v <= 0x300) {
        v = p->move_y < 0 ? -p->move_y : p->move_y;
        if (v <= 0x200)
            return;
    }
    DecGroup41_Action_Flee(w, p);
    w->target = p;
    g_obj.stats.fish_flee++;
}
static void DecGroup41_State_FollowParentPriority(Obj *w) {
    if (w->ppar == NULL)
        return;
    if (w->ppar->z <= 0) {
        set_prio(w, 1);
        return;
    }
    set_prio(w, 2);
}
Obj *DecGroup41_Create(ObjParam *p, s32 a, s32 b) {
    Obj *w = obj_alloc(OBJ_PRIO_MAPOBJ, Decoration_Destructor), *sh = NULL;
    if (w == NULL)
        return NULL;
    Decoration_InitFromMap(w, p, a, b);
    switch (p->type) {
    case 0x41:
        obj_load_bac(w, OBJ_BAC_DEC_FOUNTAIN);
        w->h.disp_flags |= 4;
        w->spr.order = 0x16;
        break;
    case 0x42:
    case 0x43:
    case 0x44:
    case 0xf1:
        obj_load_bac(w, OBJ_BAC_WATER_FALL);
        switch (p->type) {
        case 0x42:
            sh = DecShared_Create(OBJ_BAC_WATER_FALL, (u16)(p->type - 0x42), 1);
            break;
        case 0x43:
            sh = DecShared_Create(OBJ_BAC_WATER_FALL, (u16)(p->type - 0x42), 2);
            break;
        case 0xf1:
            w->h.disp_flags ^= 1;
            /* fall through */
        case 0x44:
            if (p->type == 0xf1)
                sh = DecShared_Create(OBJ_BAC_WATER_FALL, 2, 3);
            else
                sh = DecShared_Create(OBJ_BAC_WATER_FALL, (u16)(p->type - 0x42), 3);
            break;
        }
        w->parent = sh;
        if (sh != NULL)
            w->spr.vram = &sh->spr;
        if (p->type == 0xf1)
            obj_set_anim(w, 2);
        else
            obj_set_anim(w, (u16)(p->type - 0x42));
        w->h.disp_flags |= 4;
        w->spr.order = 6;
        set_prio(w, 1);
        w->spr.flags |= 8;
        w->h.obj_flags |= 0x200;
        w->state = DecShared_State_PingParent;
        break;
    case 0x4d:
        obj_load_bac(w, OBJ_BAC_GMK_WHEEL);
        obj_set_anim(w, 0);
        w->spr.order = 0x16;
        break;
    case 0x4e:
        obj_load_bac(w, OBJ_BAC_GMK_WHEEL);
        obj_set_anim(w, 1);
        w->spr.order = 0x16;
        w->ppar = g_obj.n_players ? g_obj.players[0].p : NULL;
        w->h.obj_flags |= 0x200;
        w->state = DecGroup41_State_FollowParentPriority;
        break;
    case 0x4f:
        obj_load_bac(w, OBJ_BAC_EFF_WATER_GMK);
        obj_set_anim(w, 3);
        w->h.disp_flags |= 4;
        w->spr.order = 5;
        set_prio(w, 1);
        break;
    case 0x50:
        obj_load_bac(w, OBJ_BAC_EFF_WATER_GMK);
        obj_set_anim(w, 4);
        w->h.disp_flags |= 4;
        set_prio(w, 1);
        w->spr.order = 5;
        break;
    case 0x90:
        obj_load_bac(w, OBJ_BAC_EFF_WATER_GMK);
        obj_set_anim(w, 6);
        w->h.disp_flags |= 4;
        set_prio(w, 1);
        w->spr.order = 5;
        break;
    case 0x9d:
    case 0x9e:
    case 0x9f:
    case 0xa0:
        obj_load_bac(w, OBJ_BAC_DEC_ALGAE);
        if (w->prm->type < 0x9f) {
            w->spr.order = 0x16;
        } else {
            set_prio(w, 1);
            w->spr.order = 5;
        }
        if (!(w->prm->type & 1))
            w->h.disp_flags ^= 1;
        break;
    case 0xa1:
    case 0xa2:
    case 0xa3:
    case 0xa4:
        obj_load_bac(w, OBJ_BAC_DEC_CORAL);
        if (w->prm->type < 0xa3) {
            set_prio(w, 3);
            w->spr.order = 0x16;
        } else {
            set_prio(w, 1);
            w->spr.order = 5;
        }
        if (!(w->prm->type & 1))
            w->h.disp_flags ^= 1;
        break;
    case 0xa5: case 0xa6: case 0xa7: case 0xa8: case 0xa9:
    case 0xaa: case 0xab: case 0xac: case 0xad: case 0xae:
        obj_load_bac(w, OBJ_BAC_DEC_FISH);
        set_prio(w, 3);
        w->spr.order = 0x16;
        if (w->prm->type >= 0xab) {
            w->spr.order = 0x18;
            obj_set_anim(w, 4);
        } else if (w->prm->type >= 0xa8) {
            obj_set_anim(w, 2);
        }
        if (w->prm->type >= 0xad) {
            w->h.disp_flags |= 0x20;
            DecFishPart_Create(w, 4, 4);
            DecFishPart_Create(w, -8, 2);
            DecFishPart_Create(w, 8, 0x10);
            DecFishPart_Create(w, 2, -8);
            DecFishPart_Create(w, -0xc, 6);
            DecFishPart_Create(w, 8, 8);
            DecFishPart_Create(w, -6, -4);
            DecFishPart_Create(w, -2, -10);
        }
        if (w->prm->type == 0xa7 || w->prm->type == 0xaa || w->prm->type == 0xac)
            DecGroup41_Action_Cruise(w);
        else
            DecGroup41_Action_Sink(w);
        w->h.obj_flags |= 2;
        if (w->prm->type == 0xa6 || w->prm->type == 0xa9 || w->prm->type == 0xae) {
            rect_set_box2d(&w->coll[0], -0x20, -0x20, 0x20, 0x20);
            rect_set_atkdef(&w->coll[0], 0, (u32)-0x10);
            w->coll[0].flags |= 0xc0;
            w->coll[0].cb = DecGroup41_OnHit;
            w->coll[0].owner = w;
            w->coll[0].owner_kind = OBJ_OWNER_OBJECT;
            w->h.obj_flags &= ~2u;
        }
        return w;
    }
    if (w->spr.order == 0)
        w->spr.order = 0x16;
    w->h.move_flags |= 0x2100;
    w->h.obj_flags |= 2;
    return w;
}

/* DecGroup4D_* (wheel / crane decorations with a 3D part): the 3D BAC part
 * (StageObj_Load3dBAC) is not drawn; the 2D sprite and the view test are kept */
static void DecGroup4D_State_Main(Obj *w) {
    s32 a, b;
    if (w->wC == 0) {
        w->wC = 1;
        return;
    }
    b = CAM0.x >> 8;
    a = w->h.x >> 8;
    if (b - 0x20 > a || b + 0x120 < a || (b = CAM0.y >> 8, a = w->h.y >> 8, b - 0x20 > a) || b + 0xe0 < a) {
        w->f1F4 |= 1;           /* @0x1A0 bit 0: 3D part off */
        w->spr.flags |= 8;      /* @0x258 */
    } else {
        w->f1F4 &= ~1u;
        w->spr.flags &= ~8u;
    }
}
Obj *DecGroup4D_Create(ObjParam *p, s32 a, s32 b) {
    Obj *w = obj_alloc(OBJ_PRIO_MAPOBJ, Decoration_Destructor);
    if (w == NULL)
        return NULL;
    Decoration_InitFromMap(w, p, a, b);
    switch (p->type) {
    case 0x4d:
        obj_load_bac(w, OBJ_BAC_GMK_WHEEL);
        w->spr.flags |= 0x80000;
        w->spr.order = 0x16;
        obj_set_anim(w, 0);
        w->state = DecGroup4D_State_Main;
        break;
    case 0x4e:
        obj_load_bac(w, OBJ_BAC_GMK_WHEEL);
        w->spr.flags |= 0x80000;
        w->spr.order = 0x16;
        obj_set_anim(w, 1);
        w->state = DecGroup4D_State_Main;
        break;
    }
    if (w->spr.order == 0)
        w->spr.order = 0x16;
    w->h.move_flags |= 0x2100;
    w->h.obj_flags |= 2;
    return w;
}
