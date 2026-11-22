/*
 * Huge Crisis (zone 5) objects, decorations and effects, copied from the
 * decompilation and adapted like obj_water.c / obj_mirage.c:
 *   src/game/Zone/Huge_Crisis/Objects/ Crane_*, CranePartED_*, CranePartEE_*,
 *     GmkPipe_*, JetCatapult_*, JetCatapultPart_*, Piston_*, Steam_*, Valve_*
 *   src/game/Zone/Huge_Crisis/Decorations/ DecGroup67_Create
 *   src/game/Zone/Huge_Crisis/Effects/ EffJet_*
 * The crane and its parts are NSBMD blocks of mod/gmk_crane.nsbmd: one
 * model view each with the shapes of its blocks (Obj.shapes).
 * See docs/dreamcast/translation/OBJECTS.md (section 5d).
 */
#include "obj_zone.h"

/* ===================================================== Crane (0xa8) */
static void Crane_State_Unk_ov000_02305ea0(Obj *p) {
    ObjMtx33 m, r;
    s32 lim, v;
    p->wC += p->h.gnd_vel;
    lim = 0x8000;
    if (p->h.obj_flags & 0x1000) {
        if (p->prm->flags & 2)
            p->h.gnd_vel = (s16)dcp_spd_up(p->h.gnd_vel, 0x12, 0xd00);
        else
            p->h.gnd_vel = (s16)dcp_spd_up(p->h.gnd_vel, -0x12, 0xd00);
        if (p->prm->flags & 2) {
            if (p->wC > p->w8) {
                p->wC = p->w8;
                p->h.gnd_vel = 0;
                p->h.obj_flags &= ~0x1000u;
            }
        } else {
            if (p->wC < p->w8) {
                p->wC = p->w8;
                p->h.gnd_vel = 0;
                p->h.obj_flags &= ~0x1000u;
            }
        }
    }
    if (p->f1F4 & 1) {
        if (p->prm->flags & 4)
            lim += 0x10000;
        if (p->prm->flags & 2)
            p->h.gnd_vel = (s16)dcp_spd_up(p->h.gnd_vel, -0x12, 0xd00);
        else
            p->h.gnd_vel = (s16)dcp_spd_up(p->h.gnd_vel, 0x12, 0xd00);
        if (p->prm->flags & 2) {
            if (p->wC < p->w8 - lim) {
                p->wC = p->w8 - 0x8000;
                p->h.disp_flags |= 0x200;
                p->h.obj_flags |= 0x1000;
                p->f1F4 &= ~1u;
                p->h.gnd_vel = 0;
                if (p->prm->flags & 4)
                    obj_sfx(0x40);
            }
        } else {
            if ((u32)p->wC > (u32)(p->w8 + lim)) {
                p->wC = p->w8 + 0x8000;
                p->h.disp_flags |= 0x200;
                p->h.obj_flags |= 0x1000;
                p->f1F4 &= ~1u;
                p->h.gnd_vel = 0;
                if (p->prm->flags & 4)
                    obj_sfx(0x40);
            }
        }
    }
    MTX_Identity33_(&m);
    MTX_RotY33_(&r, ZSIN(p->wC), ZCOS(p->wC));
    MTX_Concat33(&m, &r, &m);
    z_mtx_store(p, &m);
    v = p->wC & 0xffff;
    if (v < 0xc000 && v < 0x4000) {
        p->h.disp_flags |= 1;
        return;
    }
    p->h.disp_flags &= ~1;
}
Obj *Crane_Create(ObjParam *p, s32 b, s32 c) {
    Obj *w = z_mapobj(p, b, c, OBJ_BAC_CRANE, 0xf6, obj_mapobject_dtor), *o;
    if (w == NULL)
        return NULL;
    w->spr.order = 0x14;
    obj_set_anim(w, 1);
    w->spr.flags |= 0x80000;
    /* StageObj_LoadModelSimple blocks 0..10 except 2 (material data_ov000_023667d8[i], shape i) */
    z_model(w, "mod__gmk_crane__gmk_crane", 0x34cc);
    w->shapes = 0x7ffu & ~(1u << 2);
    w->h.disp_flags |= 0x200;
    w->h.move_flags |= 0x2100;
    if (w->prm->flags & 1) {
        w->wC = 0x8000;
        w->h.obj_flags ^= 1;
    } else {
        w->wC = 0;
    }
    w->w8 = w->wC;
    w->state = Crane_State_Unk_ov000_02305ea0; /* Crane_Action_Init */
    o = obj_mapobject_spawn(0xed, w->h.x, w->h.y, (s16)w->prm->flags, 0, w->prm->p7, w->prm->p8, w->prm->p9);
    if (o != NULL) {
        o->parent = w;
        o->w8 = w->w8;
    }
    o = obj_mapobject_spawn(0xee, w->h.x, w->h.y, 0, w->prm->p6, 0, 0, 0);
    if (o != NULL)
        o->parent = w;
    w->snd = 1;
    return w;
}

/* crane hook (0xed) */
static void CranePartED_NotifyParentArrived(Obj *p0) {
    if (p0 == NULL)
        return;
    if (p0->prm->flags & 8)
        return;
    if (p0->h.obj_flags & 0x1000)
        return;
    p0->f1F4 |= 1;
    p0->h.disp_flags &= ~0x200;
    obj_sfx(0x6a);
}
static void CranePartED_State_Unk_ov000_023056fc(Obj *w) {
    DcPlayer *o;
    ObjMtx33 m, r;
    if (w->parent != NULL) {
        w->w8 = w->parent->wC;
        if (w->w8 != w->parent->w8)
            w->h.obj_flags |= 2;
        else
            w->h.obj_flags &= ~2u;
        if (w->parent->h.obj_flags & 0x1000)
            w->h.obj_flags |= 0x1000;
        else
            w->h.obj_flags &= ~0x1000u;
    }
    o = (DcPlayer *)w->target;
    if (o != NULL) {
        if (w->h.y > w->home_y << 8) {
            w->h.vel_y = (s16)dcp_spd_up(w->h.vel_y, -0x18, 0x800);
        } else {
            if (w->h.vel_y != 0)
                w->h.hitstop = 0x80; /* (NNS_SndPlayerStopSeq: dropped) */
            w->h.vel_y = 0;
            w->h.y = w->home_y << 8;
            if (w->h.hitstop == 0 && (w->h.disp_flags & 0x200)) {
                CranePartED_NotifyParentArrived(w->parent);
                w->h.disp_flags &= ~0x200;
            }
        }
        if (o->gimmick != (void *)w) {
            w->h.disp_flags |= 0x200;
            w->target = NULL;
            if (w->parent != NULL)
                w->parent->h.disp_flags |= 0x200;
        }
    } else {
        w->h.vel_y = 0;
        w->h.py = w->h.y;
        if (!(w->h.obj_flags & 2))
            w->h.y = ObjShift_Set(w->h.y, (w->home_y + w->prm->p7 + w->prm->p8 + w->prm->p9) << 8, 4, 0x400, 0x20);
    }
    MTX_Identity33_(&m);
    MTX_RotY33_(&r, ZSIN(w->w8), ZCOS(w->w8));
    MTX_Concat33(&m, &r, &m);
    z_mtx_store(w, &m);
}
static void CranePartED_OnHit(ObjBox *a, ObjBox *b) {
    Obj *t0 = z_box_obj(a);
    DcPlayer *t1 = z_box_player(b);
    if (t0 == NULL || t1 == NULL)
        return; /* (deviation 29) */
    if (t0->target == NULL) {
        t0->h.vel_y = 0x100;
        obj_sfx(0x69);
        z_count("crane", t0, t1);
    }
    t0->target = t1;
    Player_Action_GrabCranePart(t1, t0, -116, 0x12);
}
static void CranePartED_Draw(Obj *w) {
    s32 x, y, r = 0x7400;
    u16 n = 0;
    /* StageObj_Draw3D(w, model): the model view */
    y = w->h.y - 0x200;
    if (!(w->h.disp_flags & 1))
        r -= 0x300;
    x = w->h.x - (s16)((r * ZCOS(w->w8)) >> 12);
    obj_draw2d_ex(x, y - 0x1200, w, &w->spr);
    w->k.crane.a.n_multi = 0;
    if (w->h.obj_flags & 2)
        return;
    if (!(w->h.disp_flags & 1))
        x = w->h.x - (s16)(((r + 0x100) * ZCOS(w->w8)) >> 12);
    for (;;) {
        y -= 0x800;
        if (y < (w->home_y - 0x18) << 8)
            break;
        obj_draw2d_ex(x, y, NULL, &w->k.crane.a);
        if (n < 48) {
            w->k.crane.pts[n][0] = w->k.crane.a.x;
            w->k.crane.pts[n][1] = w->k.crane.a.y;
            n++;
        }
    }
    w->k.crane.a.multi = (const s16(*)[2])w->k.crane.pts;
    w->k.crane.a.n_multi = (u8)n;
}
Obj *CranePartED_Create(ObjParam *p, s32 b, s32 c) {
    Obj *w = z_mapobj(p, b, c, OBJ_BAC_CRANE, 0xf7, obj_mapobject_dtor); /* CranePartED_Destructor */
    if (w == NULL)
        return NULL;
    w->spr.order = 0x14;
    obj_set_anim(w, 2);
    w->spr.flags |= 0x80000;
    /* blocks: shape 0xb (material 0xa), shape 0xc (material 9) */
    z_model(w, "mod__gmk_crane__gmk_crane", 0x34cc);
    w->shapes = (1u << 0xb) | (1u << 0xc);
    w->h.disp_flags |= 0x200;
    obj_res_ensure(OBJ_BAC_CRANE);
    spr_init(&w->k.crane.a, &g_obj.res[OBJ_BAC_CRANE], 0, 0x40000);
    w->k.crane.a.order = 0x14;
    w->k.crane.a.flags |= 0x10;
    w->xspr[0] = &w->k.crane.a;
    w->n_xspr = 1;
    w->fn_draw = CranePartED_Draw;
    if (w->prm->flags & 1)
        w->h.disp_flags ^= 1;
    z_trigger(w, 0, -0x78, 0, -0x70, 8, CranePartED_OnHit, 0x800);
    w->prm->p7 = 0x40;
    w->h.y += (w->prm->p7 + w->prm->p8 + w->prm->p9) << 8;
    if (w->prm->flags & 4)
        w->h.gnd_vel = 0xd00;
    w->h.move_flags |= 0x100;
    w->h.obj_flags |= 0x10;
    w->snd = 1;
    w->state = CranePartED_State_Unk_ov000_023056fc; /* CranePartED_Action_Init */
    return w;
}
/* crane mast (0xee): the model block and sprite drawn p6 + 1 times, 0x40 px apart */
static void CranePartEE_Draw(Obj *p) {
    s32 x = p->h.x, y = p->h.y;
    s16 i;
    u16 n = 0;
    for (i = 0; i < p->prm->p6 + 1; i++) {
        obj_draw2d(p); /* (StageObj_Draw3D of the block: model view reps) */
        if (n < 48) {
            p->k.crane.pts[n][0] = p->spr.x;
            p->k.crane.pts[n][1] = p->spr.y;
            n++;
        }
        p->h.y += 0x4000;
    }
    p->spr.multi = (const s16(*)[2])p->k.crane.pts;
    p->spr.n_multi = (u8)n;
    p->mreps = (u8)(p->prm->p6 + 1 > 0 ? p->prm->p6 + 1 : 0);
    p->h.y = y;
    p->h.x = x;
}
Obj *CranePartEE_Create(ObjParam *p, s32 b, s32 c) {
    Obj *w = z_mapobj(p, b, c, OBJ_BAC_CRANE, OBJ_PRIO_MAPOBJ, obj_mapobject_dtor);
    if (w == NULL)
        return NULL;
    w->spr.order = 0x14;
    obj_set_anim(w, 3);
    w->spr.flags |= 0x80000;
    z_model(w, "mod__gmk_crane__gmk_crane", 0x34cc); /* shape 2, material 0xb */
    w->shapes = 1u << 2;
    w->mrep_dy = 0x4000;
    w->mreps = (u8)(w->prm->p6 + 1);
    w->h.disp_flags |= 0x200;
    w->fn_draw = CranePartEE_Draw;
    w->h.move_flags |= 0x2100;
    w->h.obj_flags |= 0x10;
    return w;
}

/* ===================================================== GmkPipe (0xa6, 0xab..0xad) */
static void GmkPipe_State_Cooldown(Obj *s) {
    if (s->wC == 0)
        return;
    s->wC--;
    if (s->wC == 0)
        s->h.obj_flags &= ~2u;
}
static void GmkPipe_OnHit(ObjBox *a, ObjBox *b) {
    Obj *o = z_box_obj(a);
    DcPlayer *p = z_box_player(b);
    u8 dir = 0xc0;
    s16 v = 0;
    u16 f;
    if (o == NULL || p == NULL)
        return; /* (deviation 29) */
    f = o->prm->flags;
    if ((f & 4) || (p->gimmick != (void *)o && (p->move_flags & 0x200))) {
        if (o->prm->p6 != 0)
            v = (s16)(o->prm->p6 * 0x180 + 0x780);
        Player_Action_PipeExit(p, v);
        return;
    }
    if (f & 1)
        dir = (u8)(dir + 0x40);
    if (f & 2)
        dir = (u8)(dir + 0x80);
    if (p->gimmick != (void *)o)
        z_count("pipe", o, p);
    Player_Action_Pipe(p, o, dir);
    o->wC = 0x60;
    o->h.obj_flags |= 2;
}
static void GmkPipe_OnHit_Box1(ObjBox *a, ObjBox *b) {
    DcPlayer *pb = z_box_player(b);
    if (a->owner == NULL || pb == NULL)
        return;
    pb->hitstop = 0xf;
}
Obj *GmkPipe_Create(ObjParam *p, s32 b, s32 c) {
    Obj *w = z_mapobj(p, b, c, OBJ_BAC_PIPE, OBJ_PRIO_MAPOBJ, obj_mapobject_dtor);
    if (w == NULL)
        return NULL;
    w->spr.order = 6;
    w->spr.flags &= ~0x60000u;
    if (w->prm->flags & 8)
        w->prm->flags &= ~4;
    switch (p->type) {
    case 0xab:
        w->prm->flags &= ~1;
        w->prm->flags |= 2;
        break;
    case 0xac:
        w->prm->flags |= 1;
        w->prm->flags &= ~2;
        break;
    case 0xad:
        w->prm->flags |= 1;
        w->prm->flags |= 2;
        break;
    }
    if (w->prm->flags & 1) {
        if (w->prm->flags & 4)
            obj_set_anim(w, 3);
        else
            obj_set_anim(w, 2);
    } else if (w->prm->flags & 4) {
        obj_set_anim(w, 1);
    }
    if (w->prm->flags & 2) {
        if (w->prm->flags & 1)
            w->h.disp_flags ^= 1;
        else
            w->h.disp_flags ^= 2;
    }
    rect_set_atkdef(&w->coll[0], 0, (u32)-0x10);
    w->coll[0].cb = GmkPipe_OnHit;
    w->coll[0].flags |= 0x800;
    w->coll[1].owner = NULL;
    rect_set_box2d(&w->coll[1], -2, -2, 2, 2);
    w->coll[1].flags |= 0x800;
    rect_set_atkdef(&w->coll[1], 0, (u32)-0x10);
    w->coll[1].mask = 0x12;
    w->coll[1].cb = GmkPipe_OnHit_Box1;
    z_box_owner(w, &w->coll[1]);
    if (w->prm->flags & 1)
        z_cmap(w, g_obj.data.solid, 3, 5, 0xd, -0x12);
    else
        z_cmap(w, g_obj.data.solid, 5, 4, -0x14, -8);
    w->h.move_flags |= 0x2100;
    w->state = GmkPipe_State_Cooldown; /* GmkPipe_Action_Init */
    return w;
}

/* ===================================================== JetCatapult (0xa7) */
static void JetCatapult_State_Countdown(Obj *p0);
static void JetCatapult_State_Blastoff(Obj *p0);
static void JetCatapult_State_Fly(Obj *w);
static void JetCatapult_State_Idle(Obj *s) {
    if (s->h.move_flags & 1)
        s->h.vel_y = 0;
}
static void EffJet_State_FollowParent(Obj *w) {
    Obj *p = w->parent;
    if (p == NULL)
        return;
    if (p->h.disp_flags & 1)
        w->h.disp_flags |= 1;
    else
        w->h.disp_flags &= ~1;
    w->h.angle = p->h.angle;
    w->h.x = p->h.x;
    w->h.y = p->h.y;
    if (w->wC != 0)
        return;
    if (p->state != JetCatapult_State_Blastoff)
        return;
    w->wC = 1;
    obj_set_anim(w, 1);
    w->h.disp_flags |= 4;
}
static void EffJet_Create(Obj *par) {
    Obj *w = z_effobj(OBJ_BAC_EFF_JET, OBJ_PRIO_EFFECT);
    if (w == NULL)
        return;
    w->spr.order = 0x14;
    w->parent = par;
    w->h.x = par->h.x;
    w->h.y = par->h.y;
    w->wC = 0;
    if (par->h.disp_flags & 1)
        w->h.disp_flags |= 1;
    else
        w->h.disp_flags &= ~1;
    w->h.disp_flags |= 4;
    w->h.disp_flags |= 0x100;
    w->h.move_flags |= 0x40;
    w->h.move_flags |= 0x2100;
    w->h.obj_flags |= 2;
    w->h.obj_flags |= 0x10;
    w->state = EffJet_State_FollowParent;
}
static void JetCatapultPart_State_Countdown(Obj *s) {
    Obj *p = s->parent;
    if (p == NULL)
        return;
    if (p->state != JetCatapult_State_Countdown)
        return;
    if (s->wC > 0xb5 && p->wC <= 0xb5) {
        obj_set_anim(s, 2);
        obj_sfx(0x6b);
    }
    if (s->wC > 0x79 && p->wC <= 0x79) {
        obj_set_anim(s, 3);
        obj_sfx(0x6b);
    }
    if (s->wC > 0x3d && p->wC <= 0x3d) {
        obj_set_anim(s, 4);
        obj_sfx(0x6b);
    }
    if (s->wC > 1 && p->wC <= 1) {
        obj_set_anim(s, 5);
        obj_sfx(0xeb);
    }
    s->wC = p->wC;
}
static void JetCatapult_CreatePart_ov000_0230e354(Obj *par, s32 x, s32 y) {
    Obj *w = z_effobj(OBJ_BAC_JET_CATAPULT, OBJ_PRIO_EFFECT);
    if (w == NULL)
        return;
    obj_set_anim(w, 1);
    w->spr.order = 0x16;
    w->h.disp_flags |= 1;
    w->parent = par;
    w->h.x = x;
    w->h.y = y;
    w->wC = 0xb6;
    w->h.move_flags |= 0x2100;
    w->h.obj_flags |= 2;
    w->h.obj_flags |= 0x10;
    w->state = JetCatapultPart_State_Countdown;
}
static void JetCatapult_Action_Launch(Obj *w) {
    w->state = JetCatapult_State_Fly;
    w->h.move_flags |= 0x100;
    w->h.move_flags &= ~1u;
    w->h.move_flags |= 0x10;
    w->h.move_flags &= ~0x40u;
    w->h.disp_flags |= 0x100;
    w->has_cmap = 0;
    w->h.vel_y = -0x280;
    w->h.vel_x = (s16)(w->h.gnd_vel >> 1);
    w->h.gnd_vel = 0;
    obj_screen_shake(4);
    w->h.hitstop = 0x44;
    eff_zako_debris(w, 0, 0, 0, -0x200, 0);
    eff_zako_debris(w, 0, 0, 0, -0x200, 1);
    EffZakoBombPuff_Create(w, 0x14, 0);
    obj_sfx(0x64);
    z_count("jet_catapult_launch", w, NULL);
}
static void JetCatapult_State_Fly(Obj *w) {
    s16 dy, dx;
    w->h.angle = (u8)(w->h.angle + 8);
    if ((W.frame & 7) == 0) {
        dy = (s16)(7 - (RAND() & 0xe)); /* (arguments drawn right to left, deviation 21) */
        dx = (s16)(15 - (RAND() & 0x1e));
        EffZakoBombPuff_Create(w, dx, dy);
    }
    if (w->h.move_flags & 4) {
        w->h.gnd_vel = 0;
        return;
    }
    w->h.gnd_vel = (s16)dcp_spd_up(w->h.gnd_vel, 0x18, 0xf00);
}
static void JetCatapult_State_Blastoff(Obj *p0) {
    DcPlayer *t = (DcPlayer *)p0->target;
    if (W.frame & 2) {
        p0->h.shake_y = g_obj.data.shake[W.frame & 3];
        if (t != NULL)
            t->shake_y = p0->h.shake_y;
    }
    if (p0->h.move_flags & 4) {
        JetCatapult_Action_Launch(p0);
        return;
    }
    p0->h.gnd_vel = (s16)dcp_spd_up(p0->h.gnd_vel, 0x18, 0xf00);
    if (p0->h.move_flags & 1)
        p0->h.vel_y = 0;
}
static void JetCatapult_Action_Blastoff(Obj *p0) {
    p0->state = JetCatapult_State_Blastoff;
    p0->h.move_flags &= ~0x100u;
    p0->h.move_flags &= ~0x10u;
    p0->h.move_flags |= 0x40;
    p0->h.disp_flags |= 0x100;
    p0->h.vel_y = 0;
    p0->h.gnd_vel = 0xa00;
    obj_screen_shake(4);
    p0->h.hitstop = 0x80;
    obj_sfx(0x6c);
}
static void JetCatapult_State_Countdown(Obj *p0) {
    DcPlayer *t = (DcPlayer *)p0->target;
    p0->h.shake_y = g_obj.data.shake[W.frame & 3];
    if (t != NULL)
        t->shake_y = p0->h.shake_y;
    p0->wC--;
    if (p0->wC == 0) {
        JetCatapult_Action_Blastoff(p0);
        return;
    }
    if (p0->h.move_flags & 1)
        p0->h.vel_y = 0;
}
static void JetCatapult_Action_Ignite(Obj *s) {
    s->state = JetCatapult_State_Countdown;
    s->h.obj_flags |= 2;
    s->wC = 0xb6;
    s->h.vel_y = 0x100;
    EffJet_Create(s);
}
static void JetCatapult_OnHit(ObjBox *a, ObjBox *b) {
    Obj *t0 = z_box_obj(a);
    DcPlayer *t1 = z_box_player(b);
    if (t0 == NULL || t1 == NULL)
        return; /* (deviation 29) */
    if (t0->h.x - 0xb00 >= t1->x)
        return;
    if (t0->h.x - 0x80 <= t1->x)
        return;
    t0->target = t1;
    JetCatapult_Action_Ignite(t0);
    Player_Action_JetCatapult(t1, t0);
    z_count("jet_catapult", t0, t1);
}
int obj_jet_catapult_flying(const Obj *o) { return o->state == JetCatapult_State_Fly; }
Obj *JetCatapult_Create(ObjParam *p, s32 b, s32 c) {
    Obj *w = z_mapobj(p, b, c, OBJ_BAC_JET_CATAPULT, 0xf6, obj_mapobject_dtor); /* JetCatapult_Destructor */
    if (w == NULL)
        return NULL;
    w->spr.order = 0x14;
    w->spr.flags |= 0x200;
    rect_set_atkdef(&w->coll[0], 0, (u32)-16);
    w->coll[0].cb = JetCatapult_OnHit;
    w->coll[0].flags |= 0x80;
    z_map_hitbox(w, -20, -5, 0x1c, 9);
    z_cmap(w, obj_df("gmk_jet.df", NULL), 10, 3, -0x2a, -0xc); /* ObjData_Load(/dat/gmk/gmk_jet.df) */
    w->cmap.flags |= 1;
    w->h.disp_flags ^= 1;
    w->h.move_flags |= 0x4080;
    w->state = JetCatapult_State_Idle; /* JetCatapult_Action_Idle */
    w->snd = 1;
    JetCatapult_CreatePart_ov000_0230e354(w, w->h.x + (w->prm->p6 << 8), w->h.y + (w->prm->p7 << 8));
    return w;
}

/* ===================================================== Piston (0xa0, 0xa1) */
static u8 s_piston_loop; /* NNS_SndPlayerCountPlayingSeqBySeqArcIdx(0, 0x67) == 0 */
void crisis_reset(void) { s_piston_loop = 0; }
static void Piston_UpdateOscillation(Obj *w) {
    u16 f = w->prm->flags;
    u8 spd = f & 7;
    u16 t = (u16)w->wC;
    s32 y = w->h.py; /* @0x20: oscillation centre */
    s32 amp = (w->prm->p9 << 15) >> 16;
    if (spd != 0)
        t = (u16)((W.frame * spd + t) & 0x3ff); /* gGameState.unk24 (deviation 19) */
    else
        t = (u16)(((W.frame + (t >> 2)) & 0xff) << 2);
    if (f & 8)
        y = (y + (((s16)amp * ZSIN((u16)((t << 6) + 0x8000))) >> 12)) << 8;
    else
        y = (y + (((s16)amp * ZSIN((u16)(t << 6))) >> 12)) << 8;
    w->h.move_y = (s16)(y - w->h.y);
    w->h.y = y;
}
static void Piston_DrawShaft(Obj *w) {
    s32 c = w->prm->p6, x, y, dy;
    u8 n, i;
    u16 k = 0;
    if (c < 0)
        c = 0;
    n = (u8)c;
    if (w->prm->type == 0xa0) {
        x = w->h.x;
        y = w->h.y - 0x3c00;
        dy = -0x4000;
    } else {
        x = w->h.x;
        y = w->h.y + 0x3c00;
        dy = 0x4000;
    }
    for (i = 0; i < n; i++) {
        obj_draw2d_ex(x, y, w, &w->k.crane.a);
        if (k < 48) {
            w->k.crane.pts[k][0] = w->k.crane.a.x;
            w->k.crane.pts[k][1] = w->k.crane.a.y;
            k++;
        }
        y += dy;
    }
    w->k.crane.a.multi = (const s16(*)[2])w->k.crane.pts;
    w->k.crane.a.n_multi = (u8)k;
}
static void Piston_State_Move(Obj *p0) {
    Piston_UpdateOscillation(p0);
    Piston_DrawShaft(p0);
    if (!s_piston_loop) {
        s_piston_loop = 1;
        obj_sfx(0x67); /* Sound_PlaySeqArcBankPrio(.., 0x67): one loop for all pistons */
    }
}
static void Piston_InitOscillationPhase(Obj *w) {
    u16 t;
    s32 amp, y, base;
    w->h.py = (w->h.y >> 8) + w->prm->p7 + (w->prm->p9 >> 1);
    if (w->prm->p9 == 0)
        return;
    amp = w->prm->p9 >> 1;
    base = w->h.py;
    y = w->h.y >> 8;
    t = 0x300;
    do {
        if (base + ((amp * ZSIN((u16)(t << 6))) >> 12) > y)
            break;
        t -= 4;
    } while (t > 0x100);
    w->wC = t;
    w->wC -= (s16)(((w->prm->flags & 0x30) >> 4) << 8);
    w->wC &= 0x3fff;
}
Obj *Piston_Create(ObjParam *a, s32 b, s32 c) {
    Obj *w = z_mapobj(a, b, c, OBJ_BAC_PISTON, 0xf6, obj_mapobject_dtor); /* Piston_Destructor */
    if (w == NULL)
        return NULL;
    w->spr.order = 0x14;
    spr_init(&w->k.crane.a, &g_obj.res[OBJ_BAC_PISTON], 0, 0);
    spr_set_anim(&w->k.crane.a, 1);
    w->k.crane.a.order = 0x14;
    w->k.crane.a.flags &= ~0x60000u;
    w->k.crane.a.flags |= 0x40000;
    w->k.crane.a.flags |= 0x10;
    w->xspr[0] = &w->k.crane.a;
    w->n_xspr = 1;
    z_cmap(w, g_obj.data.solid, 6, 7, -0x18, -0x1c);
    w->h.move_flags |= 0x2100;
    w->h.obj_flags |= 2;
    w->snd = 1;
    Piston_InitOscillationPhase(w);
    obj_set_anim(w, 0); /* Piston_Action_Init */
    w->state = Piston_State_Move;
    return w;
}

/* ===================================================== Steam (0xf1) */
static void Steam_State_Rise(Obj *w) {
    w->k.steam.u2A8++;
    w->h.vel_x = (s16)(((ZSIN((u16)(w->k.steam.u2A8 << 11)) << 7) >> 12) / 3);
    if (w->h.disp_flags & 8)
        w->h.obj_flags |= 4;
}
Obj *Steam_Create(ObjParam *a, s32 b, s32 c) {
    Obj *w = z_mapobj(a, b, c, OBJ_BAC_EFF_STEAM, OBJ_PRIO_MAPOBJ, obj_mapobject_dtor);
    if (w == NULL)
        return NULL;
    w->spr.order = 0x12;
    rect_set_atkdef(&w->coll[0], 0, (u32)-0x10);
    w->h.move_flags |= 0x100;
    z_map_hitbox(w, -2, -2, 2, 2);
    w->coll[0].cb = NULL;
    w->spr.flags &= ~0x60000u;
    w->snd = 1;
    obj_sfx(0x68);
    /* Steam_Action_Rise */
    obj_set_anim(w, 4);
    w->h.vel_y = (s16)(-0x61 - (RAND() & 0x7e));
    w->state = Steam_State_Rise;
    return w;
}

/* ===================================================== Valve (0xa2..0xa5) */
static void Valve_State_Wait(Obj *s);
static void Valve_State_Puff(Obj *s);
static void Valve_State_Active(Obj *s);
static void Valve_State_End(Obj *s);
static int Valve_DrawSprites(Obj *w) {
    ObjSprite *s = &w->k.valve.s;
    s32 px = w->h.x >> 8, py = w->h.y >> 8;
    s16 ox = g_obj.data.valve_ofs[w->k.valve.u2A4 & 3][0], oy = g_obj.data.valve_ofs[w->k.valve.u2A4 & 3][1];
    /* the sprite pair is drawn per DS screen relative to gCamera[i]: the DC
     * keeps the main screen one in world coordinates */
    s->x = (s16)(ox + px);
    s->y = (s16)(oy + py);
    spr_process(s, NULL, NULL);
    s->hidden = 0;
    return (s->flags & 2) != 0;
}
static void Valve_SpawnSteamPuff(Obj *w, s32 type, s32 x, s32 y) {
    (void)w;
    obj_mapobject_spawn(type, x, y, 0, 0, 0, 0, 0);
}
static void Valve_SetSpriteAnimActive(Obj *w) {
    spr_set_anim(&w->k.valve.s, (u16)g_obj.data.valve_anim[w->k.valve.u2A4 & 3][0]);
    w->k.valve.s.flags |= 4;
}
static void Valve_SetSpriteAnimEnd(Obj *w) {
    spr_set_anim(&w->k.valve.s, (u16)g_obj.data.valve_anim[w->k.valve.u2A4 & 3][1]);
    w->k.valve.s.flags &= ~4u;
}
static void Valve_Action_Wait(Obj *p0) {
    obj_set_anim(p0, 0);
    p0->wC = (u16)((u8)p0->prm->p6 * 0x64 + (u8)p0->prm->p7);
    p0->state = Valve_State_Wait;
}
static void Valve_Action_Puff(Obj *s) {
    s->wC = 0x3c;
    s->state = Valve_State_Puff;
}
static void Valve_Action_Active(Obj *p0) {
    Valve_SetSpriteAnimActive(p0);
    p0->h.obj_flags &= ~2u;
    p0->wC = (u16)(p0->prm->p8 * 0x64 + p0->prm->p9);
    p0->state = Valve_State_Active;
    z_count("valve", p0, NULL);
}
static void Valve_Action_End(Obj *s) {
    s->h.obj_flags |= 2;
    Valve_SetSpriteAnimEnd(s);
    s->state = Valve_State_End;
}
static void Valve_State_Wait(Obj *s) {
    s->k.valve.s.hidden = 1;
    s->wC--;
    if (s->wC != 0)
        return;
    Valve_Action_Puff(s);
}
static void Valve_State_Puff(Obj *w) {
    s32 i, x, y, tx, ty, py, px;
    w->k.valve.s.hidden = 1;
    if (!(RAND() & 0x1f) || w->wC == 0x3c) {
        i = w->k.valve.u2A4 & 3;
        x = w->h.x;
        y = w->h.y;
        tx = g_obj.data.valve_ofs[i][0];
        ty = g_obj.data.valve_ofs[i][1];
        py = (ty + (y >> 8) + (3 - (RAND() & 6))) << 8; /* (right to left, deviation 21) */
        px = (tx + (x >> 8) + (3 - (RAND() & 6))) << 8;
        Valve_SpawnSteamPuff(w, 0xf1, px, py);
    }
    w->wC--;
    if (w->wC == 0)
        Valve_Action_Active(w);
}
static void Valve_State_Active(Obj *w) {
    s32 i, x, y, px, py;
    w->k.valve.s.hidden = 1;
    Valve_DrawSprites(w);
    if (!(RAND() & 0xf)) {
        i = w->k.valve.u2A4 & 3;
        x = w->h.x >> 8;
        y = w->h.y >> 8;
        py = (y + g_obj.data.valve_puff[i][1] + RAND() % g_obj.data.valve_puff[i][3]) << 8;
        px = (x + g_obj.data.valve_puff[i][0] + RAND() % g_obj.data.valve_puff[i][2]) << 8;
        Valve_SpawnSteamPuff(w, 0xf1, px, py);
    }
    w->wC--;
    if (w->wC == 0)
        Valve_Action_End(w);
}
static void Valve_State_End(Obj *p) {
    p->k.valve.s.hidden = 1;
    if (Valve_DrawSprites(p) == 0)
        return;
    Valve_Action_Wait(p);
}
static void Valve_InitSteamSprites(Obj *w) {
    u16 idx = (u16)g_obj.data.valve_anim[w->k.valve.u2A4 & 3][0];
    u32 fl = g_obj.data.valve_anim[w->k.valve.u2A4 & 3][2] | 0x4004;
    ObjSprite *spr = &w->k.valve.s;
    /* "obj:/act/ac_eff_steam.bac" from the zone archive, else the loose file */
    obj_res_ensure(OBJ_BAC_EFF_STEAM);
    spr_init(spr, &g_obj.res[OBJ_BAC_EFF_STEAM], idx, fl);
    spr->order = 0x13;
    spr_process(spr, NULL, NULL);
    spr->flags |= 0x10;
    spr->hidden = 1;
    w->xspr[0] = spr;
    w->n_xspr = 1;
}
Obj *Valve_Create(ObjParam *a, s32 b, s32 c) {
    Obj *w = z_mapobj(a, b, c, OBJ_BAC_VALVE, OBJ_PRIO_MAPOBJ, obj_mapobject_dtor); /* Valve_Destructor */
    s32 k;
    if (w == NULL)
        return NULL;
    w->spr.order = 0x14;
    rect_set_atkdef(&w->coll[0], 0, (u32)-0x10);
    w->coll[0].cb = NULL;
    w->coll[0].flags |= 0x800;
    w->h.move_flags |= 0x2100;
    switch (w->prm->type) {
    default:
    case 0xa2:
        w->k.valve.u2A4 = 0;
        w->h.disp_flags &= ~2;
        break;
    case 0xa3:
        w->k.valve.u2A4 = 1;
        w->h.disp_flags |= 2;
        break;
    case 0xa4:
        w->k.valve.u2A4 = 2;
        w->h.disp_flags |= 1;
        w->h.disp_flags |= 0x100;
        w->h.angle = 0xc0;
        break;
    case 0xa5:
        w->k.valve.u2A4 = 3;
        w->h.disp_flags &= ~1;
        w->h.disp_flags |= 0x100;
        w->h.angle = 0x40;
        break;
    }
    obj_set_anim(w, 0);
    w->h.disp_flags |= 4;
    k = w->k.valve.u2A4;
    rect_set_box2d(&w->coll[1], g_obj.data.valve_box[k][0], g_obj.data.valve_box[k][1], g_obj.data.valve_box[k][2],
                   g_obj.data.valve_box[k][3]);
    rect_set_atkdef(&w->coll[1], 0x13, 0x13);
    w->coll[1].cb = NULL;
    z_box_owner(w, &w->coll[1]);
    w->h.obj_flags |= 2;
    Valve_InitSteamSprites(w);
    Valve_Action_Wait(w);
    return w;
}

/* ===================================================== DecGroup67 */
Obj *DecGroup67_Create(ObjParam *p, s32 a, s32 b) {
    Obj *w = obj_alloc(OBJ_PRIO_MAPOBJ, Decoration_Destructor);
    s32 i;
    if (w == NULL)
        return NULL;
    Decoration_InitFromMap(w, p, a, b);
    switch (p->type) {
    default:
        obj_load_bac(w, OBJ_BAC_DEC_PIPE);
        if (p->type < 0x83) {
            i = p->type - 0x67;
            i -= FX_DivS32(i, 7) * 7;
            obj_set_anim(w, (u16)i); /* (shared VRAM data_ov000_0236a790[i]: own frame natively) */
            if (p->type >= 0x6e)
                w->h.disp_flags ^= 1;
            if (p->type >= 0x75)
                w->h.disp_flags ^= 2;
            if (p->type >= 0x75 && p->type <= 0x7b)
                w->h.disp_flags ^= 1;
        } else {
            i = p->type - 0x83;
            obj_set_anim(w, (u16)(i + 7));
        }
        if (p->type == 0x84 || p->type == 0x86)
            w->spr.order = 5;
        else
            w->spr.order = 6;
        z_set_prio(w, 0);
        break;
    case 0x91:
    case 0x92:
    case 0x93:
    case 0x94:
        obj_load_bac(w, OBJ_BAC_DEC_PLANE); /* (palette row 1 for 0x93 / 0x94: deviation 16) */
        if (!(p->type & 1))
            w->h.disp_flags ^= 1;
        w->spr.order = 0x16;
        break;
    }
    if (w->spr.order == 0)
        w->spr.order = 0x16;
    w->h.move_flags |= 0x2100;
    w->h.obj_flags |= 2;
    return w;
}
