/*
 * Enemy projectiles of zones 5 and 7 that EnemyPawnGroup throws, copied
 * from the decompilation and adapted like obj_enemy.c:
 *   src/game/Zone/Dead_Line/Enemies/EggSf_Create (0xe6, thrown by type 9)
 *   src/game/Zone/Huge_Crisis/Enemies/GunHunter_Create, _State_CheckGround
 *     (0xec, the Gun Hunter's shot, type 0xd)
 * The flapper shots (0xe7, 0xeb, 0xf0) are in obj_flapper.c.
 */
#include "obj_zone.h"

/* EggSf_Create: the holder data_ov000_02369dbc is the type 9 pawn's, so it
 * shows ac_ene_egg_sf.bac */
Obj *EggSf_Create(ObjParam *p, s32 b, s32 c) {
    Obj *w = obj_alloc(OBJ_PRIO_MAPOBJ, obj_mapobject_dtor);
    s32 k;
    if (w == NULL)
        return NULL;
    obj_mapobject_init(w, p, b, c);
    w->kind = OBJK_ENEMY;
    obj_load_bac(w, OBJ_BAC_EGG_SF);
    w->spr.order = 0x15;
    if (!(w->prm->flags & 1))
        w->h.disp_flags &= ~1;
    w->h.vel_x = 0xa00;
    if (w->h.disp_flags & 1)
        w->h.vel_x = (s16)-w->h.vel_x;
    k = w->prm->flags & 0xc;
    if (k != 0)
        w->h.vel_x = (s16)((w->h.vel_x * (((k >> 2) << 7) + 0x100)) >> 8);
    w->h.move_flags |= 0x110;
    w->h.disp_flags |= 0x100;
    Enemy_Action_StartPopOnContact(w);
    return w;
}

/* GunHunter_Create / _State_CheckGround: the shot flies until it is over
 * open air or its box hit something */
static void GunHunter_State_CheckGround(Obj *p0) {
    s32 t0 = rlvl_probe_distance(g_obj.map, p0->h.x >> 8, p0->h.y >> 8, 1, 2, NULL, NULL);
    if (t0 >= 0) {
        if ((p0->coll[1].flags & 0x200) == 0)
            return;
    }
    p0->h.obj_flags |= 4;
}
Obj *GunHunter_Create(ObjParam *p, s32 b, s32 c) {
    Obj *w = obj_alloc(OBJ_PRIO_MAPOBJ, obj_mapobject_dtor);
    s32 k;
    if (w == NULL)
        return NULL;
    obj_mapobject_init(w, p, b, c);
    w->kind = OBJK_ENEMY;
    obj_load_bac(w, OBJ_BAC_GUN_HUNTER);
    obj_disable_colliders_set_anim(w, 4);
    w->spr.order = 9;
    if (!(w->prm->flags & 1))
        w->h.disp_flags &= ~1;
    w->h.vel_x = (s16)((z_frame_rand() & 0xff) + 0x500);
    if (w->h.disp_flags & 1)
        w->h.vel_x = (s16)-w->h.vel_x;
    k = w->prm->flags & 0xc;
    if (k != 0)
        w->h.vel_x = (s16)((w->h.vel_x * (((k >> 2) << 7) + 0x100)) >> 8);
    w->h.move_flags |= 0x110;
    w->h.disp_flags |= 0x100;
    w->h.disp_flags |= 4;
    w->state = GunHunter_State_CheckGround;
    return w;
}
