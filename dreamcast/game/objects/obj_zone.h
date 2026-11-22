/*
 * Helpers shared by the objdata v4 zone files (obj_carnival.c, obj_crisis.c,
 * obj_altitude.c, obj_deadline.c, obj_enemy2.c, obj_decor2.c and the
 * obj_p*.c player states).  Each one stands for a DS call sequence that the
 * creates repeat (Task_Create + MapObject_Init + StageObj_Load2dBAC, ObjRect
 * set-up, StageObj_SetMapHitbox, ...).  Static inline: no new linkage.
 */
#ifndef OBJ_ZONE_H
#define OBJ_ZONE_H

#include <string.h>
#include "obj_internal.h"

#define ZABS(x) ((x) < 0 ? -(x) : (x))
#define CAMS g_obj_camera[2] /* gCamera.unkC8 = gCamera[2] @0x00 */

/* StageObj_SetOAMPriority */
static inline void z_set_prio(Obj *o, u32 prio) { o->spr.flags = (o->spr.flags & ~0x60000u) | (prio << 17); }
static inline void z_spr_prio(ObjSprite *s, u32 prio) { s->flags = (s->flags & ~0x60000u) | (prio << 17); }
/* StageObj_SetMapHitbox */
static inline void z_map_hitbox(Obj *w, s8 l, s8 t, s8 r, s8 b) {
    w->h.box_l = l;
    w->h.box_t = t;
    w->h.box_r = r;
    w->h.box_b = b;
}
static inline void z_fxhook(DcPlayer *p, s32 kind, s32 a, s32 b) {
    if (g_obj_hooks.effect)
        g_obj_hooks.effect(g_obj_hooks.ctx, p, kind, a, b);
}
/* Task_Create(StageObj_Main, dtor, 0, prio, ..) + MapObject_Init + StageObj_Load2dBAC */
static inline Obj *z_mapobj(ObjParam *p, s32 x, s32 y, int bac, u16 prio, ObjFn dtor) {
    Obj *o = obj_alloc(prio, dtor);
    if (o == NULL)
        return NULL;
    obj_mapobject_init(o, p, x, y);
    o->kind = OBJK_GIMMICK;
    if (bac >= 0)
        obj_load_bac(o, bac);
    return o;
}
/* Task_Create(StageObj_Main, StageObj_Destructor, 0, prio, ..) + StageObj_Load2dBAC */
static inline Obj *z_effobj(int bac, u16 prio) {
    Obj *o = obj_alloc(prio, obj_stageobj_dtor);
    if (o == NULL)
        return NULL;
    if (bac >= 0)
        obj_load_bac(o, bac);
    o->kind = OBJK_EFFECT;
    return o;
}
/* ObjRect_SetBox2D + ObjRect_SetAtkDefFlags(0, -0x10) + callback + unk18 = w */
static inline void z_trigger(Obj *w, int i, s16 l, s16 t, s16 r, s16 b, void (*cb)(ObjBox *, ObjBox *), u16 fl) {
    rect_set_box2d(&w->coll[i], l, t, r, b);
    rect_set_atkdef(&w->coll[i], 0, (u32)-0x10);
    w->coll[i].flags |= fl;
    w->coll[i].cb = cb;
    w->coll[i].owner = w;
    w->coll[i].owner_kind = OBJ_OWNER_OBJECT;
}
static inline void z_box_owner(Obj *w, ObjBox *b) {
    b->owner = w;
    b->owner_kind = OBJ_OWNER_OBJECT;
}
/* the collision map @0x1B8: obj, tiles @0x1C8, attr @0x1D0, w/h @0x1D8, offset @0x1D4 */
static inline void z_cmap(Obj *w, const void *map, u16 cw, u16 ch, s16 ox, s16 oy) {
    w->cmap.obj = w;
    w->cmap.map = (const u8(*)[8])map;
    w->cmap.w = cw;
    w->cmap.h = ch;
    w->cmap.ox = ox;
    w->cmap.oy = oy;
    w->has_cmap = map != NULL;
}
static inline void z_count(const char *ev, Obj *o, DcPlayer *p) {
    g_obj.stats.gimmicks++;
    obj_log(ev, o && o->prm ? o->prm->type : 0, p ? p->x >> 8 : (o ? o->h.x >> 8 : 0));
}
static inline DcPlayer *z_player(int i) { return (i >= 0 && i < g_obj.n_players) ? g_obj.players[i].p : NULL; }
/* gPlayers[0] (NULL-safe stand-in) */
static inline DcPlayer *z_player0(void) { return g_obj.n_players > 0 ? g_obj.players[0].p : NULL; }
/* box unk18: the owner when it is a player (type == 1), else NULL */
static inline DcPlayer *z_box_player(const ObjBox *b) { return obj_box_player(b); }
static inline Obj *z_box_obj(const ObjBox *b) {
    return (b != NULL && b->owner_kind == OBJ_OWNER_OBJECT) ? (Obj *)b->owner : NULL;
}
/* data_0206e470 sine table by 16-bit angle */
#define ZSIN(a) obj_sin((u16)(a))
#define ZCOS(a) obj_cos((u16)(a))
static inline void z_mtx_store(Obj *w, const ObjMtx33 *m) { memcpy(w->mtx, m->m, sizeof(w->mtx)); }
static inline void z_mtx_load(const Obj *w, ObjMtx33 *m) { memcpy(m->m, w->mtx, sizeof(w->mtx)); }
/* StageObj_LoadModelSimple: model id + identity matrix + default scale */
static inline void z_model(Obj *w, const char *id, s32 sc) {
    ObjMtx33 m;
    w->model = id;
    MTX_Identity33_(&m);
    z_mtx_store(w, &m);
    w->mscale[0] = w->mscale[1] = w->mscale[2] = sc;
}
/* the player work the player module does not keep (@0x3EC..@0x3FC, ...) */
static inline ObjPlayerCtx *z_ctx(DcPlayer *w) {
    static ObjPlayerCtx dummy;
    ObjPlayerCtx *c = obj_player_ctx(w);
    return c ? c : &dummy;
}
static inline void z_psfx(DcPlayer *p, s32 ch, s32 id) {
    if (g_dcp_hooks.sfx)
        g_dcp_hooks.sfx(g_dcp_hooks.ctx, p, ch, id);
}
static inline void z_psfx_stop(DcPlayer *p, s32 ch) {
    if (g_dcp_hooks.sfx_stop)
        g_dcp_hooks.sfx_stop(g_dcp_hooks.ctx, p, ch);
}
static inline void z_call(DcpFn f, DcPlayer *w) {
    if (f != NULL)
        f(w);
}
/* Player_SetAnimSpeedFromVelocity (same code as the player module's) */
static inline void z_anim_speed(DcPlayer *p, s32 v) {
    s32 t = (v >> 5) + (v >> 6);
    s16 s = (s16)ZABS(t);
    if (s <= 0x10)
        s = 0x10;
    if (s >= 0x80)
        s = 0x80;
    if (g_dcp_hooks.anim_speed)
        g_dcp_hooks.anim_speed(g_dcp_hooks.ctx, p, s);
}
/* "FrameRand": the LCG step applied to the stage frame counter */
static inline u16 z_frame_rand(void) { return (u16)((((u32)W.frame << 16) * 0x196225u + 0x3c6ef35fu) >> 16); }

#endif
