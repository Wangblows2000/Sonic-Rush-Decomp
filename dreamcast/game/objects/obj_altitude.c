/*
 * Altitude Limit (zone 6) objects and decorations, copied from the
 * decompilation and adapted like obj_water.c / obj_mirage.c:
 *   src/game/Zone/Altitude_Limit/Objects/ Glider_*, NeedleG_*, Parachute_*,
 *     ParachuteReleased_*, Saucer_*, MapObj13_*, MapObj13Part_*
 *   src/game/Zone/Altitude_Limit/Decorations/ DecCloudSpawner_Create,
 *     DecCloudW_*
 *   src/game/Decorations/DecGroup95_Create (zones 5..7)
 * 3D BACs (StageObj_Load3dBAC: MapObj13 clouds, 3D flappers, glider rings)
 * are the same converted BAC drawn as a 2D sprite (Obj.bac3d, deviation 23);
 * their depth is kept in z.  See docs/dreamcast/translation/OBJECTS.md.
 */
#include "obj_zone.h"

/* ===================================================== Glider (0xb5) */
static void Glider_State_Carry(Obj *w) {
    DcPlayer *p = (DcPlayer *)w->target;
    u16 ay = 0x4000, ax = 0;
    ObjMtx33 m, r;
    if (p != NULL) {
        if ((w->f1F4 & 1) && !(p->pf & 0x400)) {
            w->h.obj_flags |= 4;
            return;
        }
        if (p->type == 1 && (p->pf & 0x400)) {
            w->h.z -= 0x200;
            w->f1F4 |= 1;
            ay = (u16)(ay - 0xc000);
        } else {
            w->h.x = p->x;
            w->h.y = p->y + 0xc00;
            w->h.z = p->z;
            ay = (u16)(ay - (p->rot3 << 8));
            ax = (u16)-(p->rot2 << 8);
            if (p->disp_flags & 0x200) {
                w->h.vel_x = 0x800;
                w->h.vel_z = 0x40;
                w->h.move_flags &= ~0x2000u;
                w->target = NULL;
            }
        }
    }
    MTX_Identity33_(&m);
    MTX_RotY33_(&r, ZSIN(ay), ZCOS(ay));
    MTX_Concat33(&m, &r, &m);
    MTX_RotX33_(&r, ZSIN(ax), ZCOS(ax));
    MTX_Concat33(&m, &r, &m);
    z_mtx_store(w, &m);
}
static void Glider_OnHit(ObjBox *a, ObjBox *b) {
    Obj *t0 = z_box_obj(a);
    DcPlayer *t1 = z_box_player(b);
    if (t0 == NULL || t1 == NULL)
        return; /* (deviation 29) */
    if (t0->target != NULL)
        return;
    t0->target = t1;
    t0->h.obj_flags |= 2;
    t0->h.disp_flags &= ~0x200;
    Player_Action_Glider(t1, t0, 0, -4);
    z_count("glider", t0, t1);
}
Obj *Glider_Create(ObjParam *p, s32 b, s32 c) {
    Obj *w = z_mapobj(p, b, c, OBJ_BAC_GLIDER, 0x800, obj_mapobject_dtor);
    if (w == NULL)
        return NULL;
    w->spr.order = 0x14;
    w->spr.flags |= 0x80000;
    /* five StageObj_LoadModelSimple blocks of mod/gmk_glider.nsbmd (material
     * data_ov000_023669b0[i], shape i), all shown */
    z_model(w, "mod__gmk_glider__gmk_glider", 0x34cc);
    w->shapes = 0x1f;
    w->h.disp_flags |= 0x200;
    z_trigger(w, 0, -16, -36, 16, 32, Glider_OnHit, 0x800);
    w->h.move_flags |= 0x2100;
    w->target = NULL;
    w->state = Glider_State_Carry; /* Glider_Action_Carry */
    return w;
}

/* ===================================================== NeedleG (0xb1, 0xb2) */
static void NeedleG_State_Idle(Obj *w) { (void)w; }
Obj *NeedleG_Create(ObjParam *a, s32 b, s32 c) {
    Obj *w = z_mapobj(a, b, c, OBJ_BAC_NEEDLE_G, OBJ_PRIO_MAPOBJ, obj_mapobject_dtor);
    if (w == NULL)
        return NULL;
    if (w->prm->type != 0xb1)
        obj_set_anim(w, 1); /* (shared VRAM data_ov000_0236a0f4 / _104: own frame natively) */
    w->spr.order = 0x16;
    rect_set_atkdef(&w->coll[1], 0x13, (u32)-1);
    w->coll[1].cb = NULL;
    if (w->prm->flags & 1)
        w->h.disp_flags ^= 1;
    w->f1F4 |= 0x20;
    w->state = NeedleG_State_Idle;
    return w;
}

/* ===================================================== Parachute (0xb4) */
static void ParachuteReleased_State_Main(Obj *w) {
    DcPlayer *p = w->ppar;
    s16 x, y;
    if (p != NULL) {
        x = 0;
        y = -0x3000;
        if (p->disp_flags & 1)
            w->h.disp_flags |= 1;
        else
            w->h.disp_flags &= ~1;
        w->h.angle = p->angle;
        dcp_rotate_vec(&x, &y, w->h.angle);
        w->h.x = p->x + x;
        w->h.y = p->y + y;
        if ((p->move_flags & 1) || (p->move_flags & 0x2000) || (p->move_flags & 0x100)) {
            w->ppar = NULL;
            w->h.move_flags &= ~0x2000u;
            return;
        }
        if (w->wC != 0)
            return;
        if (p->type != 1)
            return;
        if (p->anim != 0x15)
            return;
        obj_set_anim(w, 1);
        w->wC = 1;
        w->h.hitstop = 0x60;
        return;
    }
    if (w->h.obj_flags & 0x10)
        obj_set_anim(w, 2);
    w->h.obj_flags &= ~0x10u;
    w->w8++;
    if ((u32)w->w8 > 0x20) {
        if (w->w8 & 2)
            w->h.disp_flags |= 0x20;
        else
            w->h.disp_flags &= ~0x20;
    }
    w->h.angle = (u8)dcp_spd_down(w->h.angle, 1);
    w->h.vel_x = (s16)dcp_spd_up(w->h.vel_x, 8, 0x180);
    w->h.vel_y = (s16)dcp_spd_up(w->h.vel_y, -4, 0x200);
    if ((u32)w->w8 > 0x40)
        w->h.obj_flags |= 4;
}
void ParachuteReleased_Create(DcPlayer *arg) {
    Obj *w = z_effobj(OBJ_BAC_PARACHUTE, OBJ_PRIO_EFFECT);
    if (w == NULL)
        return;
    w->spr.order = 0xa;
    w->spr.flags |= 0x200;
    w->ppar = arg;
    w->h.x = arg->x;
    w->h.y = arg->y;
    w->wC = 0;
    if (arg->disp_flags & 1)
        w->h.disp_flags |= 1;
    else
        w->h.disp_flags &= ~1;
    w->h.disp_flags |= 4;
    w->h.disp_flags |= 0x100;
    w->h.move_flags |= 0x2100;
    w->h.obj_flags |= 2;
    w->h.obj_flags |= 0x10;
    w->state = ParachuteReleased_State_Main;
}
static void Parachute_State_Idle(Obj *w) { (void)w; }
static void Parachute_OnHit(ObjBox *a, ObjBox *b) {
    Obj *t0 = z_box_obj(a);
    DcPlayer *t1 = z_box_player(b);
    if (t0 == NULL || t1 == NULL)
        return; /* (deviation 29) */
    if (t1->state != Player_State_Parachute && t1->state != Player_State_ParachuteRelease && t1->gimmick != (void *)t0)
        z_count("parachute", t0, t1);
    Player_Action_Parachute(t1, t0, t0->prm->p6);
}
Obj *Parachute_Create(ObjParam *a, s32 b, s32 c) {
    Obj *w = z_mapobj(a, b, c, OBJ_BAC_PARACHUTE, OBJ_PRIO_MAPOBJ, obj_mapobject_dtor);
    if (w == NULL)
        return NULL;
    obj_set_anim(w, 3);
    w->spr.order = 6;
    z_set_prio(w, 1);
    rect_set_atkdef(&w->coll[0], 0, (u32)-0x10);
    w->coll[0].cb = Parachute_OnHit;
    w->coll[0].flags |= 0x800;
    w->coll[1].owner = NULL;
    if (w->prm->flags & 1)
        w->h.disp_flags ^= 1;
    w->h.move_flags |= 0x2100;
    w->state = Parachute_State_Idle;
    return w;
}

/* ===================================================== Saucer (0xb3) */
static void Saucer_State_Idle(Obj *w);
static void Saucer_State_Ridden(Obj *w);
static void Saucer_Action_Idle(Obj *s) {
    s->state = Saucer_State_Idle;
    s->h.move_flags |= 0x10;
}
static void Saucer_Explode(Obj *p0) {
    if (p0->h.disp_flags & 0x20)
        return;
    obj_sfx(0x64);
    EffZakoBomb_Create(&p0->h, 0, 0);
    eff_zako_debris(p0, 0, -24, 0, -0x200, 4);
    eff_zako_debris(p0, 0, -24, 0, -0x200, 5);
    eff_zako_debris(p0, 0, -24, 0, -0x200, 6);
    eff_zako_debris(p0, 0, -24, 0, -0x200, 7);
    obj_screen_shake(4);
    z_fxhook(NULL, OBJ_FX_SCREEN_FLASH, 0, 0);
    p0->h.obj_flags |= 8;
    p0->h.disp_flags |= 0x20;
    z_count("saucer_explode", p0, NULL);
}
static void Saucer_Action_Ridden(Obj *p0) {
    obj_set_anim(p0, 1);
    p0->h.disp_flags |= 4;
    p0->state = Saucer_State_Ridden;
    p0->h.move_flags &= ~0x2000u;
    p0->wC = 0;
    p0->h.gnd_vel = 0;
    p0->h.gnd_vel = 0;
    p0->w8 = 0x100;
    p0->h.hitstop = 0x60;
    obj_sfx(0x6c);
}
static void Saucer_State_Idle(Obj *w) {
    DcPlayer *r;
    s16 acc = -8, max = 0x1c0;
    if (w->h.move_flags & 0xf) {
        /* NNS_SndPlayerStopSeq: dropped */
        Saucer_Explode(w);
        return;
    }
    if (w->prm->flags & 1) {
        acc = (s16)(acc << 1);
        max = (s16)(max << 1);
    }
    w->h.vel_x = (s16)dcp_spd_down(w->h.vel_x, 0x10);
    w->h.vel_y = (s16)dcp_spd_up(w->h.vel_y, acc, max);
    w->h.angle = dcp_angle_toward8(w->h.angle, 0, 1);
    r = w->cmap.rider;
    if (r != NULL && r->ride == &w->ref && r->type == 1) {
        w->target = r;
        Player_Action_Saucer(r, w);
        Saucer_Action_Ridden(w);
        z_count("saucer", w, r);
        return;
    }
    if (w->h.move_flags & 0x2000)
        return;
    if (!(W.frame & 7))
        obj_sfx(0x6c); /* (positional sound: dropped) */
}
static void Saucer_State_Ridden(Obj *w) {
    s16 acc = 8, max = 0x1c0;
    DcPlayer *p;
    u16 a;
    if (w->prm->flags & 1) {
        acc = (s16)(acc << 1);
        max = (s16)(max << 1);
    }
    if (w->h.move_flags & 0xf) {
        Saucer_Explode(w);
        return;
    }
    p = (DcPlayer *)w->target;
    if (p != NULL) {
        if (p->held & 0x20)
            w->wC = dcp_spd_up(w->wC, -0x100, 0x1800);
        else if (p->held & 0x10)
            w->wC = dcp_spd_up(w->wC, 0x100, 0x1800);
        else
            w->wC = dcp_spd_down(w->wC, 0x300);
        if (!(W.frame & 7))
            obj_sfx(0x6c);
        if (p->gimmick != (void *)w) {
            w->target = NULL;
            w->h.vel_y = 0x200;
            Saucer_Action_Idle(w);
            return;
        }
    }
    w->h.gnd_vel = (s16)dcp_spd_up(w->h.gnd_vel, acc, max);
    a = (u16)((u8)(w->h.angle - 0x40) << 8);
    w->h.vel_x = (s16)(((w->h.gnd_vel * ZCOS(a)) >> 12) << 1);
    w->h.vel_y = (s16)((w->h.gnd_vel * ZSIN(a)) >> 12);
    w->h.vel_y = (s16)(w->h.vel_y + w->w8);
    w->w8 = dcp_spd_down(w->w8, 4);
    if (w->target != NULL && (((DcPlayer *)w->target)->move_flags & 2))
        w->h.vel_y = 0;
    w->h.angle = (u8)(w->wC >> 8);
}
Obj *Saucer_Create(ObjParam *a, s32 b, s32 c) {
    Obj *w = z_mapobj(a, b, c, OBJ_BAC_SAUCER, 0xf6, obj_mapobject_dtor); /* Saucer_Destructor */
    if (w == NULL)
        return NULL;
    w->spr.order = 0x14;
    w->spr.flags |= 0x200;
    z_cmap(w, g_obj.data.solid, 5, 2, -0x13, 0xa);
    w->cmap.attr = g_obj.data.col_attr;
    w->cmap.flags |= 1;
    z_map_hitbox(w, -0x15, -0x17, 0x15, 0x14);
    w->h.move_flags |= 0x2000;
    w->h.move_flags |= 0x40;
    w->h.move_flags |= 0x8000;
    w->h.move_flags |= 0x4000;
    w->h.obj_flags |= 2;
    w->h.disp_flags |= 0x100;
    w->snd = 1;
    Saucer_Action_Idle(w);
    return w;
}

/* ============================================ MapObj13 (0x13, 0xb8..0xba) */
static Obj *s_mo13_part[2]; /* data_ov000_02369f70 (3D flapper driver), data_ov000_02369f6c (ring driver) */
static void MapObj13Part_State_DestroyOnAnimEnd(Obj *s) {
    if (s->h.disp_flags & 8)
        s->h.obj_flags |= 4;
}
static void MapObj13Part_Destructor_ov000_0230bdf4(Obj *p) {
    if (s_mo13_part[0] == p)
        s_mo13_part[0] = NULL;
    obj_stageobj_dtor(p);
}
static void MapObj13Part_Destructor_ov000_0230bfdc(Obj *p) {
    if (s_mo13_part[1] == p)
        s_mo13_part[1] = NULL;
    obj_stageobj_dtor(p);
}
static void MapObj13Part_State_DestroyWhenIdle(Obj *s) {
    if (s->wC == 0) {
        s->h.obj_flags |= 4;
        s_mo13_part[0] = NULL;
    }
    s->wC = 0;
}
static void MapObj13Part_State_DestroyWhenIdle2(Obj *s) {
    if (s->wC == 0) {
        s->h.obj_flags |= 4;
        s_mo13_part[1] = NULL;
    }
    s->wC = 0;
}
/* ring sparkle when a glider-sequence ring is collected */
static Obj *MapObj13_CreatePart_ov000_0230bc50(Obj *par) {
    Obj *w = z_effobj(OBJ_BAC_RING, OBJ_PRIO_EFFECT);
    if (w == NULL)
        return NULL;
    obj_set_anim(w, 1);
    w->spr.order = 9;
    z_set_prio(w, 1);
    w->h.disp_flags |= 1;
    w->h.x = par->h.x;
    w->h.y = par->h.y;
    w->h.z = par->h.z;
    w->bac3d = 1;
    w->h.move_flags |= 0x100;
    w->h.obj_flags |= 2;
    w->state = MapObj13Part_State_DestroyOnAnimEnd;
    return w;
}
/* the shared 3D-BAC animation drivers (one per kind, task 0x3000) */
static Obj *mo13_driver(int i, int bac, ObjFn dtor, ObjFn st) {
    Obj *w;
    if (s_mo13_part[i] != NULL && s_mo13_part[i]->used)
        return s_mo13_part[i];
    w = obj_alloc(OBJ_PRIO_RING, dtor);
    if (w == NULL)
        return NULL;
    s_mo13_part[i] = w;
    obj_load_bac(w, bac);
    w->kind = OBJK_EFFECT;
    w->h.move_flags |= 0x2100;
    w->h.disp_flags |= 0x20;
    w->h.disp_flags |= 4;
    w->h.obj_flags |= 0x10;
    w->wC = 1;
    w->state = st;
    return w;
}
static Obj *MapObj13_CreatePart_ov000_0230be44(void) {
    return mo13_driver(0, OBJ_BAC_FLAPPER3D, MapObj13Part_Destructor_ov000_0230bdf4, MapObj13Part_State_DestroyWhenIdle);
}
static Obj *MapObj13_CreatePart_ov000_0230c02c(void) {
    return mo13_driver(1, OBJ_BAC_RING_GRAIDER, MapObj13Part_Destructor_ov000_0230bfdc, MapObj13Part_State_DestroyWhenIdle2);
}
void altitude_reset(void) { memset(s_mo13_part, 0, sizeof(s_mo13_part)); }
static void MapObj13_State_Unk_ov000_0230c1c8(Obj *p0) {
    s32 t1;
    if (p0->parent != NULL)
        p0->parent->wC++;
    t1 = ObjShift_Set(p0->w8, 0, 4, 0, 0);
    p0->h.push_x = (s16)(p0->w8 - t1);
    p0->w8 = t1;
    if (p0->h.z <= 0x8000)
        return;
    if (W.wind_x == 0)
        p0->h.obj_flags |= 4;
    if (!(p0->prm->flags & 1)) {
        p0->h.obj_flags |= 4;
        return;
    }
    p0->h.disp_flags |= 0x20;
    p0->h.obj_flags |= 2;
    p0->h.vel_z = 0;
}
static void MapObj13_Action_Unk_ov000_0230c290(Obj *p0) {
    p0->state = MapObj13_State_Unk_ov000_0230c1c8;
    p0->h.disp_flags &= ~0x20;
    p0->h.move_flags &= ~0x8000000u;
    p0->h.vel_x = (s16)(p0->prm->p6 << 6);
    p0->h.vel_z = 0x400;
}
static void MapObj13_State_Unk_ov000_0230c2d0(Obj *w) {
    s32 x = 0;
    if (w->parent != NULL)
        w->parent->wC++;
    if (g_obj_camera[1].flags34 & 1)
        x = g_obj_camera[1].x;
    if (g_obj_camera[0].flags34 & 1)
        x = g_obj_camera[0].x;
    if (w->h.x - 0x2000 < x) {
        w->w8 = 0x8000 - (w->h.x - x);
        w->h.vel_x = 0;
        MapObj13_Action_Unk_ov000_0230c290(w);
        return;
    }
    if (W.wind_x == 0)
        w->h.obj_flags |= 4;
}
static void MapObj13_OnHit(ObjBox *a, ObjBox *b) {
    Obj *t0 = z_box_obj(a);
    DcPlayer *t1 = z_box_player(b);
    if (t0 == NULL || t1 == NULL)
        return; /* (deviation 29) */
    if (t0->h.obj_flags & 2)
        return;
    dcp_add_rings(t1, 1);
    t0->h.obj_flags |= 8;
    t0->h.obj_flags |= 2;
    t0->h.disp_flags |= 0x20;
    MapObj13_CreatePart_ov000_0230bc50(t0);
    z_count("glider_ring", t0, t1);
}
Obj *MapObj13_Create(ObjParam *p, s32 b, s32 c) {
    Obj *w, *sh;
    if ((b >> 8) < W.map_left + 0xc8 || W.wind_x == 0)
        return NULL;
    w = z_mapobj(p, b, c, -1, OBJ_PRIO_MAPOBJ, obj_mapobject_dtor);
    if (w == NULL)
        return NULL;
    w->bac3d = 1;
    switch (w->prm->type) {
    case 0xb9:
        obj_load_bac(w, OBJ_BAC_CLOUD_3); /* 3D BAC ac_gmk_cloud_3 (shared texture data_ov000_02369fac) */
        w->h.obj_flags |= 2;
        break;
    case 0xba:
        obj_load_bac(w, OBJ_BAC_CLOUD_3);
        obj_set_anim(w, 1);
        w->h.obj_flags |= 2;
        break;
    case 0x13:
        /* 2D ac_ene_flapper (hidden) + 3D ac_ene_flapper3d animated by the shared driver */
        obj_load_bac(w, OBJ_BAC_FLAPPER3D);
        sh = MapObj13_CreatePart_ov000_0230be44();
        w->parent = sh;
        if (sh != NULL)
            w->spr.vram = &sh->spr;
        w->h.obj_flags |= 0x200;
        break;
    case 0xb8:
        /* 2D ac_itm_ring (act_com, hidden) + 3D ac_itm_ring_graider (shared driver) */
        obj_load_bac(w, OBJ_BAC_RING_GRAIDER);
        sh = MapObj13_CreatePart_ov000_0230c02c();
        w->parent = sh;
        if (sh != NULL)
            w->spr.vram = &sh->spr;
        w->h.obj_flags |= 0x200;
        rect_set_box3d(&w->coll[0], -6, -6, 6, 6, -10, 10);
        rect_set_atkdef(&w->coll[0], 0, (u32)-16);
        w->coll[0].cb = MapObj13_OnHit;
        z_box_owner(w, &w->coll[0]);
        break;
    }
    w->h.z = -0xc400;
    w->h.move_flags |= 0x100;
    w->h.move_flags |= 0x8000000;
    if (w->prm->flags & 1)
        w->h.obj_flags |= 0x10;
    /* MapObj13_Action_Unk_ov000_0230c370 */
    w->h.disp_flags |= 0x20;
    w->state = MapObj13_State_Unk_ov000_0230c2d0;
    w->h.vel_x = -0x800;
    return w;
}

/* ============================================ DecCloudSpawner (0xc7), DecCloudW */
static void DecCloudW_State_Main(Obj *w) {
    w->spr.flags &= ~0x80000u;
    w->h.x += w->h.vel_x;
    if (w->h.x < -0x4000)
        w->h.x += 0x20000;
    /* gCamera[0].y < gCamera[1].y: hidden on the main screen (spr0), else on the
     * other DS screen (spr1, not kept); the DC shows view 0 */
    if (g_obj.view[0].y < g_obj.view[g_obj.n_views > 1 ? 1 : 0].y)
        w->spr.flags |= 0x80000;
    if (W.flags & 1)
        w->h.disp_flags &= ~0x20;
}
static Obj *DecCloudW_Create(u32 a) {
    Obj *w;
    u32 i;
    if (W.flags & 0x20)
        return NULL;
    w = z_effobj(OBJ_BAC_DEC_CLOUD_W, OBJ_PRIO_EFFECT);
    if (w == NULL)
        return NULL;
    w->kind = OBJK_DECOR;
    w->h.x = (s32)((a & 7) << 14);
    i = a & 3;
    obj_set_anim(w, (u16)i); /* (shared VRAM data_ov000_0236a790[i + 0x14]: own frame) */
    if (a > 3)
        w->spr.flags |= 8;
    if (a & 8) {
        w->h.y = 0xc400;
        w->h.vel_x = -0x40;
        z_set_prio(w, 3);
        w->spr.order = 0x18;
    } else {
        w->h.y = 0xe400;
        w->h.vel_x = -0x80;
        z_set_prio(w, 0);
        w->spr.order = 5;
    }
    w->h.disp_flags |= 0x20;
    w->h.disp_flags |= 1;
    w->h.disp_flags |= 0x80;
    w->h.move_flags |= 0x2100;
    w->h.obj_flags |= 0x12;
    w->state = DecCloudW_State_Main;
    return w;
}
/* DecCloudSpawner_Create: a task that never ends, returns NULL by design */
Obj *DecCloudSpawner_Create(ObjParam *p, s32 x, s32 y) {
    Obj *w = obj_alloc(OBJ_PRIO_MAPOBJ, obj_mapobject_dtor);
    u16 i;
    if (w == NULL)
        return NULL; /* (the original does not test the allocation) */
    w->kind = OBJK_DECOR;
    w->h.type = 3;
    w->home_x = x >> 8;
    w->home_y = y >> 8;
    w->prm = p;
    w->saved_x = w->prm->x0;
    w->prm->x0 = 0xff;
    w->h.move_flags |= 0x2100;
    w->h.obj_flags |= 0x10;
    if (W.zone == 5) {
        for (i = 0; i < 0x10; i++)
            DecCloudW_Create(i);
    }
    obj_log("cloud_spawner", W.zone, x >> 8);
    return NULL;
}

/* ===================================================== DecGroup95 */
Obj *DecGroup95_Create(ObjParam *p, s32 a, s32 b) {
    Obj *w = obj_alloc(OBJ_PRIO_MAPOBJ, Decoration_Destructor), *sh;
    if (w == NULL)
        return NULL;
    Decoration_InitFromMap(w, p, a, b);
    w->h.move_flags |= 0x2100;
    w->h.obj_flags |= 2;
    switch (p->type) {
    case 0x96:
    default:
        obj_load_bac(w, OBJ_BAC_DEC_CLOUD);
        if (p->type != 0x96)
            w->spr.order = (u16)(w->prm->type - 0xc7);
        else
            w->spr.order = 0x16;
        obj_set_anim(w, 0);
        break;
    case 0x97:
        obj_load_bac(w, OBJ_BAC_DEC_CLOUD);
        w->spr.order = 6;
        z_set_prio(w, 1);
        obj_set_anim(w, 1);
        w->h.move_flags |= 0x8000000;
        break;
    case 0x98:
        obj_load_bac(w, OBJ_BAC_DEC_CLOUD);
        w->spr.order = 0x16;
        obj_set_anim(w, 2);
        w->h.move_flags |= 0x8000000;
        break;
    case 0x99:
        obj_load_bac(w, OBJ_BAC_DEC_CLOUD);
        w->spr.order = 6;
        z_set_prio(w, 1);
        obj_set_anim(w, 3);
        w->h.move_flags |= 0x8000000;
        break;
    case 0x9a:
        obj_load_bac(w, OBJ_BAC_DEC_CLOUD);
        w->spr.order = 0x16;
        obj_set_anim(w, 4);
        w->h.move_flags |= 0x8000000;
        break;
    case 0x9b:
        obj_load_bac(w, OBJ_BAC_DEC_CLOUD);
        w->spr.order = 6;
        z_set_prio(w, 1);
        obj_set_anim(w, 5);
        w->h.move_flags |= 0x8000000;
        break;
    case 0x95:
        obj_load_bac(w, OBJ_BAC_DEC_INNER_PILLAR);
        sh = dec_shared_create(OBJ_BAC_DEC_INNER_PILLAR, 0, 0);
        w->parent = sh;
        if (sh != NULL)
            w->spr.vram = &sh->spr;
        w->spr.order = 0x16;
        w->spr.flags |= 8;
        w->h.obj_flags |= 0x200;
        w->state = dec_shared_ping;
        w->h.move_flags &= ~0x2000u;
        break;
    case 0x9c:
        obj_load_bac(w, OBJ_BAC_DEC_PROPELLER);
        w->h.disp_flags |= 4;
        break;
    case 0xc5:
        obj_load_bac(w, OBJ_BAC_DEC_JET);
        w->spr.order = 0x16;
        w->h.disp_flags |= 4;
        break;
    case 0xc6: case 0xc7: case 0xc8: case 0xc9: case 0xca: case 0xcb:
    case 0xcc: case 0xcd: case 0xce: case 0xd6: case 0xd7:
        obj_load_bac(w, OBJ_BAC_GMK_RAIL6);
        switch (p->type) {
        case 0xd6:
            obj_set_anim(w, 0);
            break;
        case 0xc7:
            w->h.disp_flags ^= 1;
            /* fall through */
        case 0xc6:
            obj_set_anim(w, 1);
            break;
        case 0xc9:
            w->h.disp_flags ^= 1;
            /* fall through */
        case 0xc8:
            obj_set_anim(w, 2);
            break;
        case 0xcb:
            w->h.disp_flags ^= 1;
            /* fall through */
        case 0xca:
            obj_set_anim(w, 3);
            break;
        case 0xce:
            w->h.disp_flags ^= 1;
            /* fall through */
        case 0xcc:
            obj_set_anim(w, 5);
            break;
        case 0xd7:
            w->h.disp_flags ^= 1;
            /* fall through */
        case 0xcd:
            obj_set_anim(w, 6);
            break;
        }
        w->spr.order = 0x16;
        break;
    case 0xd0: case 0xd1: case 0xd2: case 0xd3: case 0xd4: case 0xd5:
        obj_load_bac(w, OBJ_BAC_DEC_LIGHT);
        switch (p->type) {
        case 0xd1:
            w->h.disp_flags ^= 1;
            /* fall through */
        case 0xd0:
            obj_set_anim(w, 0);
            break;
        case 0xd3:
            w->h.disp_flags ^= 1;
            /* fall through */
        case 0xd2:
            obj_set_anim(w, 1);
            break;
        case 0xd5:
            w->h.disp_flags ^= 1;
            /* fall through */
        case 0xd4:
            obj_set_anim(w, 2);
            break;
        }
        w->spr.order = 0x16;
        z_fxhook(NULL, OBJ_FX_PALETTE_ANIM, 0x0236459c, p->type); /* PaletteAnim_Create */
        w->h.obj_flags |= 0x800;
        break;
    case 0xe6:
        w->h.disp_flags ^= 1;
        /* fall through */
    case 0xe5:
        obj_load_bac(w, OBJ_BAC_DEC_LASER);
        w->spr.order = 0x16;
        obj_set_anim(w, 1);
        break;
    case 0xe7:
        obj_load_bac(w, OBJ_BAC_DEC_LASER);
        sh = dec_shared_create(OBJ_BAC_DEC_LASER, 0, 3);
        w->parent = sh;
        if (sh != NULL)
            w->spr.vram = &sh->spr;
        obj_set_anim(w, 0);
        w->spr.order = 0x17;
        w->spr.flags |= 8;
        w->h.obj_flags |= 0x200;
        w->state = dec_shared_ping;
        break;
    }
    if (w->spr.order == 0)
        w->spr.order = 0x16;
    return w;
}
