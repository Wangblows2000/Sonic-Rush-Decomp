/*
 * Player effects (src/game/Effects/), translated onto the object system's
 * effect objects (task/work struct -> Obj, VRAM / palette calls dropped,
 * spr1 (second DS screen) not kept):
 *   EffBoost_Create, EffBoost_SetFollowState, EffBoost_State_FollowPlayer,
 *   EffBoostAura_Create, EffBoostBurst_Create (+ _Draw: the vertically
 *   mirrored second draw is a second object), EffHighSpeedStart_Create,
 *   EffBlazeBoostFlame_Create, EffBlazeRunFlame_Spawn / _Create,
 *   EffBlazeSpinFlame_Spawn / _Create / _State_Main, EffBlazeTrickFlame_Create,
 *   EffTrail_Create / _State_Init / _State_Wait / _State_Follow / _SetNodeFromOwner / _Draw
 *   (the quads; obj_draw.c draws them), EffBoostJet_Spawn / _Create,
 *   EffSpinSmoke1_Spawn / _Create / _State_FollowParent,
 *   EffSpinSmoke2_Spawn / _Create / _Action_Init / _State_Main,
 *   EffBrakeSmoke_Spawn.
 * The player module raises them through g_dcp_hooks.effect (DCP_FX_*);
 * obj_player.c dispatches to obj_player_effect().
 * AnimatorSprite flag 0x400 (OAM semi-transparent) is drawn in the
 * translucent list by obj_draw.c.  The 3D variants (zone >= 9 or the boss
 * acts: EffBoostJet3D, EffSpinSmoke*_3D, EffBrakeSmoke3D) are not created.
 */
#include <string.h>
#include "obj_internal.h"

#define SPR_SEMITRANS 0x400u /* attr0 OBJ mode 1 */

static int stage_2d(void) { return W.zone < 9 && W.act < 2; }

static Obj *peff_new(int bac) {
    Obj *o = obj_alloc(OBJ_PRIO_EFFECT, obj_stageobj_dtor);
    if (o == NULL)
        return NULL;
    obj_load_bac(o, bac);
    o->kind = OBJK_EFFECT;
    return o;
}

/* StageObj_SetOAMOrder(w, 9); StageObj_SetOAMPriority(w, prio) */
static void peff_oam(Obj *w, int prio) {
    w->spr.order = 9;
    if (prio >= 0)
        w->spr.flags = (w->spr.flags & ~0x60000u) | ((u32)prio << 17);
}

/* ------------------------------------------- EffBoost_State_FollowPlayer */
static void EffBoost_State_FollowPlayer(Obj *w) {
    DcPlayer *p = w->ppar;
    if (p == NULL)
        return;
    w->h.x = p->x;
    w->h.y = p->y;
    w->h.z = p->z;
    if (p->state == Player_State_Rocket) {
        w->h.disp_flags &= ~1;
        if (p->disp_flags & 1)
            w->h.angle = (u8)(p->angle + 0x80);
        else
            w->h.angle = p->angle;
        w->h.angle = (u8)(w->h.angle + p->grav_rot);
    } else {
        if (!(p->hitstop & 0xf)) {
            if (p->move_y == 0 && p->move_x == 0)
                w->h.angle = p->angle;
            else
                w->h.angle = (u8)(dcp_atan2(p->move_y, p->move_x) >> 8);
            if (w->h.angle == 0 && p->grav_rot == 0 && p->angle == 0 && (p->disp_flags & 1))
                w->h.angle = (u8)(w->h.angle + 0x80);
        }
        w->h.disp_flags &= ~1;
    }
    w->h.x += w->h.vel_x << 8;
    w->h.y += w->h.vel_y << 8;
    if (p->pf2 & 0x20000)
        w->h.disp_flags |= 0x20;
    else
        w->h.disp_flags &= ~0x20;
    if (w->wC != 0) {
        if (!(p->move_flags & 1) && w->wC > 8)
            w->wC = 8;
        w->wC--;
        if ((p->pf & 0x100) && w->wC != 0)
            return;
        w->h.obj_flags |= 4;
    } else {
        if (p->pf & 0x80)
            return;
        w->h.obj_flags |= 4;
    }
}

/* common part of EffBoostAura / EffBoostBurst / EffHighSpeedStart */
static Obj *boost_part(DcPlayer *p, u16 anim_s, u16 anim_b, s32 x, s32 y) {
    Obj *w = peff_new(OBJ_BAC_EFF_BOOST);
    if (w == NULL)
        return NULL;
    obj_set_anim(w, p->chara != 1 ? anim_s : anim_b); /* gGameState.unk0: the character */
    peff_oam(w, 1);
    w->spr.flags |= SPR_SEMITRANS | 0x200;
    if (p->disp_flags & 1)
        w->h.disp_flags |= 1;
    w->h.x = p->x;
    w->h.y = p->y;
    w->h.vel_x = 0;
    w->h.vel_y = 0;
    w->h.angle = (u8)(p->angle + p->grav_rot);
    if (!(w->h.disp_flags & 1)) {
        w->h.x += x << 8;
    } else {
        w->h.x -= x << 8;
        w->h.vel_x = (s16)-w->h.vel_x;
    }
    w->h.y += y << 8;
    w->h.move_flags |= 0x100;
    w->h.disp_flags |= 0x100;
    w->h.obj_flags |= 2;
    return w;
}

/* EffBoost_Create: the dust puffs behind the player in high-speed mode */
Obj *EffBoost_Create(DcPlayer *p, s16 x, s16 y) {
    Obj *w;
    u8 n;
    if (W.objmgr & 0x100)
        return NULL;
    if (p->pid != 0 || !stage_2d())
        return NULL;
    w = peff_new(OBJ_BAC_EFF_BOOST);
    if (w == NULL)
        return NULL;
    obj_set_anim(w, p->chara != 1 ? 0 : 1);
    peff_oam(w, -1);
    if (p->disp_flags & 1)
        w->h.disp_flags |= 1;
    w->h.x = p->x;
    w->h.y = p->y;
    w->h.vel_x = 0;
    w->h.vel_y = 0;
    n = (u8)(p->angle + p->grav_rot);
    if (w->h.disp_flags & 1)
        x = (s16)-x;
    dcp_rotate_vec(&x, &y, n);
    w->h.x += x << 8;
    w->h.y += y << 8;
    w->h.move_flags |= 0x100;
    w->h.obj_flags |= 2;
    eff_set_move_until_anim_end(w);
    return w;
}

/* EffBoostAura_Create: the aura around the player while boosting */
Obj *EffBoostAura_Create(DcPlayer *p, s32 x, s32 y) {
    Obj *w;
    if (p->pid != 0)
        return NULL;
    w = boost_part(p, 4, 7, x, y);
    if (w == NULL)
        return NULL;
    w->ppar = p;
    w->h.disp_flags |= 4;
    w->h.obj_flags |= 0x10;
    w->state = EffBoost_State_FollowPlayer; /* EffBoost_SetFollowState */
    return w;
}

/* EffBoostBurst_Create: the flash at the start of a boost; EffBoostBurst_Draw draws the
 * frame a second time mirrored vertically (5C ^ 2) without animating it (0x1000) */
Obj *EffBoostBurst_Create(DcPlayer *p, s32 x, s32 y) {
    Obj *w, *m;
    if (p->pid != 0)
        return NULL;
    w = boost_part(p, 5, 8, x, y);
    if (w == NULL)
        return NULL;
    eff_set_move_until_anim_end(w);
    m = boost_part(p, 5, 8, x, y);
    if (m != NULL) {
        m->h.disp_flags |= 2;
        eff_set_move_until_anim_end(m);
    }
    return w;
}

/* EffHighSpeedStart_Create: the burst when high-speed mode starts */
Obj *EffHighSpeedStart_Create(DcPlayer *p, s32 x, s32 y) {
    Obj *w;
    if (p->pid != 0)
        return NULL;
    w = boost_part(p, 3, 6, x, y);
    if (w == NULL)
        return NULL;
    w->ppar = p;
    w->h.disp_flags |= 4;
    w->wC = (p->move_flags & 1) ? 0x40 : 8;
    w->state = EffBoost_State_FollowPlayer;
    return w;
}

/* EffBlazeBoostFlame_Create: Blaze's flames while boosting (every 4 frames) */
Obj *EffBlazeBoostFlame_Create(DcPlayer *p) {
    Obj *w;
    s16 v8, vA, vC, vE;
    if (p->pid != 0)
        return NULL;
    w = peff_new(OBJ_BAC_EFF_BOOST);
    if (w == NULL)
        return NULL;
    obj_set_anim(w, 2);
    peff_oam(w, -1);
    w->spr.flags |= SPR_SEMITRANS;
    if (RAND() & 1)
        peff_oam(w, 1);
    if (p->disp_flags & 1)
        w->h.disp_flags |= 1;
    w->h.x = p->x;
    w->h.y = p->y;
    w->h.angle = (u8)(dcp_atan2(p->move_y, p->move_x) >> 8);
    if (DCP_ABS(p->move_x) > DCP_ABS(p->move_y))
        w->h.vel_x = (s16)((0x7f - (RAND() & 0xfe)) + DCP_ABS(p->move_x >> 1));
    else
        w->h.vel_x = (s16)((0x7f - (RAND() & 0xfe)) + DCP_ABS(p->move_y >> 1));
    w->h.vel_y = 0;
    dcp_rotate_vec(&w->h.vel_x, &w->h.vel_y, w->h.angle);
    v8 = (s16)(-0x1100 - (RAND() & 0xe00));
    vA = (s16)(0xf00 - (RAND() & 0x1e00));
    vC = (s16)(0x3f - (RAND() & 0x7e));
    vE = (s16)(-vA >> 9);
    dcp_rotate_vec(&vC, &vE, w->h.angle);
    w->wC = (s32)(((u32)(u16)vC << 16) | (u16)vE);
    dcp_rotate_vec(&v8, &vA, w->h.angle);
    w->h.x += v8;
    w->h.y += vA;
    w->h.move_flags |= 0x100;
    w->h.obj_flags |= 2;
    eff_set_move_until_anim_end(w);
    return w;
}

/* the flames Blaze trails (ac_eff_boost.bac anim 2, semi-transparent, OAM order 9): */
static Obj *blaze_flame_new(DcPlayer *p) {
    Obj *w = peff_new(OBJ_BAC_EFF_BOOST);
    if (w == NULL)
        return NULL;
    obj_set_anim(w, 2);
    peff_oam(w, -1);
    w->spr.flags |= SPR_SEMITRANS;
    (void)p;
    return w;
}

/* EffBlazeRunFlame_Spawn / EffBlazeRunFlame_Create: running faster than run_thr[1], every 4 frames */
static Obj *EffBlazeRunFlame_Create(DcPlayer *p, s16 x, s16 y) {
    Obj *w = blaze_flame_new(p);
    u8 n;
    if (w == NULL)
        return NULL;
    if (RAND() & 1)
        peff_oam(w, 1);
    if (p->disp_flags & 1)
        w->h.disp_flags |= 1;
    w->h.x = p->x;
    w->h.y = p->y;
    w->h.vel_x = (s16)((0x7f - (RAND() & 0xfe)) + (p->move_x >> 1));
    w->h.vel_y = (s16)(0x9f - (RAND() & 0x3e));
    w->wC = (s32)(((u32)(u16)(0x3f - (RAND() & 0x7e)) << 16) | (u16)(-0x11 - (RAND() & 0x1e)));
    n = (u8)(p->angle + p->grav_rot);
    if (w->h.disp_flags & 1)
        x = (s16)-x;
    dcp_rotate_vec(&x, &y, n);
    x = (s16)(x + 3 - (RAND() & 6));
    y = (s16)(y + 3 - (RAND() & 6));
    w->h.x += x << 8;
    w->h.y += y << 8;
    w->h.move_flags |= 0x100;
    w->h.obj_flags |= 2;
    eff_set_move_until_anim_end(w);
    return w;
}
static Obj *EffBlazeRunFlame_Spawn(DcPlayer *p) {
    if (p->pid != 0 || !stage_2d())
        return NULL;
    return EffBlazeRunFlame_Create(p, -8, (p->pf2 & 0x1000) ? 0 : 8);
}

/* EffBlazeSpinFlame_State_Main: circles the player (radius w8 growing 0x18 a frame, angle wC
 * -0x600 a frame), rising; OAM priority 2 behind / 1 in front */
static void EffBlazeSpinFlame_State_Main(Obj *w) {
    DcPlayer *p = w->ppar;
    if (p != NULL) {
        w->h.x = p->x + (w->h.move_x << 8);
        w->h.y = p->y + (w->h.move_y << 8);
        if (w->h.disp_flags & 1)
            w->h.x -= w->h.px;
        else
            w->h.x += w->h.px;
        if (w->h.disp_flags & 2)
            w->h.y -= w->h.py;
        else
            w->h.y += w->h.py;
        w->wC -= 0x600;
        w->h.vel_x = (s16)((w->w8 * dcp_cos((u16)w->wC)) >> 12);
        w->h.vel_y = (s16)(w->h.vel_y - 10);
        w->w8 += 0x18;
        w->h.px += w->h.vel_x;
        w->h.py += w->h.vel_y;
        peff_oam(w, w->h.vel_x < 0 ? 2 : 1);
    }
    if (w->h.disp_flags & 8)
        w->h.obj_flags |= 4;
}
/* EffBlazeSpinFlame_Spawn / _Create: rolling (every 8 frames) and jumping (anim 0x30, every 8) */
static Obj *EffBlazeSpinFlame_Create(DcPlayer *p, s16 x, s16 y) {
    Obj *w = blaze_flame_new(p);
    u8 n;
    if (w == NULL)
        return NULL;
    w->spr.speed >>= 1;
    if (p->disp_flags & 1)
        w->h.disp_flags |= 1;
    if (p->grav_rot != 0)
        w->h.disp_flags |= 2;
    w->ppar = p;
    w->h.x = p->x;
    w->h.y = p->y;
    w->h.px = 0;
    w->h.py = 0;
    w->h.vel_x = 0;
    w->h.vel_y = -0x80;
    n = (u8)(p->angle + p->grav_rot);
    if (w->h.disp_flags & 1)
        x = (s16)-x;
    dcp_rotate_vec(&x, &y, n);
    x = (s16)(x + 3 - (RAND() & 6));
    y = (s16)(y + 3 - (RAND() & 6));
    w->wC = (s32)((W.frame & 0xf) << 12);
    w->h.move_x = x;
    w->h.move_y = y;
    w->w8 = 0x200;
    w->h.move_flags |= 0x2100;
    w->h.obj_flags |= 2;
    w->state = EffBlazeSpinFlame_State_Main;
    return w;
}
static Obj *EffBlazeSpinFlame_Spawn(DcPlayer *p) {
    if (p->pid != 0 || !stage_2d() || (p->pf2 & 0x1000))
        return NULL;
    return EffBlazeSpinFlame_Create(p, 0, 8);
}

/* EffBlazeTrickFlame_Create: the trick spins (anim 0x35 / 0x36), every 4 frames; f = flame count */
static Obj *EffBlazeTrickFlame_Create(DcPlayer *p, u32 f) {
    Obj *w;
    s32 d;
    if (p->pid != 0)
        return NULL;
    w = blaze_flame_new(p);
    if (w == NULL)
        return NULL;
    if (f & 8)
        peff_oam(w, 1);
    if (p->disp_flags & 1)
        w->h.disp_flags |= 1;
    w->h.x = p->x;
    w->h.y = p->y;
    w->h.vel_y = (s16)(-0x61 - (RAND() & 0x3e));
    w->wC = (s32)(((u32)(u16)(0xf - (RAND() & 0x1e)) << 16) | (u16)(-1 - (RAND() & 0x3e)));
    d = ((dcp_sin((u16)(f << 12)) * 12) << 4) >> 16;
    if (w->h.disp_flags & 1)
        d = (s16)-d;
    w->h.x += d << 8;
    w->h.move_flags |= 0x100;
    w->h.obj_flags |= 2;
    eff_set_move_until_anim_end(w);
    return w;
}

/* Effect_State_FollowParent with the player as parent (@0x88) */
static void peff_follow_player(Obj *w) {
    DcPlayer *p = w->ppar;
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

/* EffBoostJet_Create (EffBoostJet_Spawn: 2D stages; the 3D variant is not created) */
Obj *EffBoostJet_Spawn(DcPlayer *p) {
    Obj *w;
    s16 x = -8, y = 6;
    if (p->pid != 0 || !stage_2d())
        return NULL;
    w = peff_new(OBJ_BAC_EFF_BOOST);
    if (w == NULL)
        return NULL;
    obj_set_anim(w, 9);
    peff_oam(w, -1);
    w->ppar = p;
    if (p->disp_flags & 1)
        x = (s16)-x;
    w->h.x = p->x;
    w->h.y = p->y;
    w->h.angle = (u8)(dcp_atan2(-0x800, p->vel_x) >> 8);
    w->h.angle = (u8)(w->h.angle + p->grav_rot + 0x40);
    dcp_rotate_vec(&x, &y, (u8)(p->angle + p->grav_rot));
    w->h.vel_x = x;
    w->h.vel_y = y;
    w->h.x += x << 8;
    w->h.y += y << 8;
    w->h.move_flags |= 0x2100;
    w->h.disp_flags |= 0x100;
    w->h.obj_flags |= 2;
    w->state = peff_follow_player; /* Effect_SetFollowParentState */
    return w;
}

/* ------------------------------------------------- spin dash smoke */
static void EffSpinSmoke1_State_FollowParent(Obj *w) {
    DcPlayer *p = w->ppar;
    if (p != NULL) {
        w->h.x = p->x;
        w->h.y = p->y;
        if (w->h.disp_flags & 1)
            w->h.x -= w->h.vel_x << 8;
        else
            w->h.x += w->h.vel_x << 8;
        w->h.y += w->h.vel_y << 8;
        if (p->gnd_vel != 0)
            w->ppar = NULL;
    }
    if (w->h.disp_flags & 8)
        w->h.obj_flags |= 4;
}

static Obj *spin_smoke(DcPlayer *p, int bac, u16 anim, s16 x, s16 y) {
    Obj *w = peff_new(bac);
    if (w == NULL)
        return NULL;
    obj_set_anim(w, anim);
    peff_oam(w, 1);
    if (p->disp_flags & 1)
        w->h.disp_flags |= 1;
    w->ppar = p;
    w->h.x = p->x;
    w->h.y = p->y;
    w->h.angle = p->grav_rot;
    dcp_rotate_vec(&x, &y, p->grav_rot);
    w->h.vel_x = x;
    w->h.vel_y = y;
    if (w->h.disp_flags & 1)
        w->h.x -= x << 8;
    else
        w->h.x += x << 8;
    w->h.y += y << 8;
    w->h.move_flags |= 0x100;
    w->h.disp_flags |= 0x100;
    w->h.obj_flags |= 2;
    return w;
}

/* EffSpinSmoke1_Spawn / EffSpinSmoke1_Create */
Obj *EffSpinSmoke1_Spawn(DcPlayer *p) {
    Obj *w;
    if (p->pid != 0 || !stage_2d() || (p->pf2 & 0x1000))
        return NULL;
    w = spin_smoke(p, OBJ_BAC_EFF_SPIN1, p->chara != 1 ? 0 : 1, -10, 0xe);
    if (w != NULL)
        w->state = EffSpinSmoke1_State_FollowParent;
    return w;
}

static void EffSpinSmoke2_State_Main(Obj *w) {
    DcPlayer *p = w->ppar;
    u16 a = w->spr.anim;
    if (p == NULL)
        return;
    if (a == 1 || a == 3) {
        if (w->h.disp_flags & 8)
            w->h.obj_flags |= 4;
        return;
    }
    w->h.x = p->x;
    w->h.y = p->y;
    if (w->h.disp_flags & 1)
        w->h.x -= w->h.vel_x << 8;
    else
        w->h.x += w->h.vel_x << 8;
    w->h.y += w->h.vel_y << 8;
    if (p->anim >= 0x32 && p->anim <= 0x33 && (p->pf & 1))
        return;
    obj_set_anim(w, a == 0 ? 1 : 3); /* also clears the loop bit _Action_Init set */
    if (p->anim >= 0x32 && p->anim <= 0x33)
        return;
    w->h.move_flags &= ~0x2000;
    w->h.vel_x = -0x100;
    w->h.vel_y = 0;
    if (w->h.disp_flags & 1)
        w->h.vel_x = (s16)-w->h.vel_x;
}

/* EffSpinSmoke2_Spawn / EffSpinSmoke2_Create / EffSpinSmoke2_Action_Init */
Obj *EffSpinSmoke2_Spawn(DcPlayer *p) {
    Obj *w;
    if (p->pid != 0 || !stage_2d() || (p->pf2 & 0x1000) || p->type != 1)
        return NULL;
    w = spin_smoke(p, OBJ_BAC_EFF_SPIN2, p->chara != 1 ? 0 : 2, -10, 0xe);
    if (w == NULL)
        return NULL;
    w->h.move_flags |= 0x2000;
    w->h.disp_flags |= 4;
    w->state = EffSpinSmoke2_State_Main;
    return w;
}

/* EffBrakeSmoke_Spawn (2D: EffBrakeSmoke_Create(p, 10, 0xe), (10, 0) with pf2 & 0x1000) */
static Obj *EffBrakeSmoke_Spawn(DcPlayer *p) {
    if (p->pid != 0 || !stage_2d())
        return NULL;
    /* EffBrakeSmoke_Create reads only the StageObj header of its argument */
    return EffBrakeSmoke_Create((Obj *)(void *)p, 10, (p->pf2 & 0x1000) ? 0 : 0xe);
}

/* ------------------------------------------------------------- EffTrail */
/* Sonic's boost ribbon (Player_StartBoost: EffTrail_Create(w, 0x18, 0xa, 0x7ca5, 0x7ca5)): a ring of
 * 11 nodes, each the owner's position +-len/2 across its motion; one node a frame.  The DS draws it
 * in the 3D layer (EffTrail_Draw, flat quads, alpha 6 -> 0x12 -> 1 along it); obj_draw.c draws the
 * quads objects_trail_quads() returns. */
#define TRAIL_NODES 11
typedef struct {
    Obj *o;             /* the task (NULL: slot free) */
    DcPlayer *own;      /* @0x134 */
    s32 len;            /* @0x138 ribbon width (fx) */
    s32 cnt, max;       /* @0x2A0 nodes shown, @0x2A4 limit */
    u16 c0, c1;         /* @0x2A8 / @0x2AA BGR555 colours */
    s32 a0, a1, a2;     /* @0x2AC / @0x2B0 / @0x2B4 alpha: head, peak, tail */
    s32 kdiv, kmul;     /* @0x2B8 / @0x2BC: the peak is at cnt * kmul / kdiv */
    int cur;            /* @0x29C newest node */
    s32 n[TRAIL_NODES][6]; /* a.x a.y a.z b.x b.y b.z */
} Trail;
static Trail s_trail[2];

static Trail *trail_of(Obj *w) {
    int i;
    for (i = 0; i < 2; i++)
        if (s_trail[i].o == w)
            return &s_trail[i];
    return NULL;
}
static void EffTrail_Destructor(Obj *w) {
    Trail *t = trail_of(w);
    if (t)
        t->o = NULL;
    obj_stageobj_dtor(w);
}
/* v * RotX(a) * RotY(b) * RotZ(c) (MTX_RotX33_ / _RotY33_ / _RotZ33_, MTX_Concat33, MTX_MultVec33) */
static void trail_rot(s32 v[3], u16 a, u16 b, u16 c) {
    s32 x = v[0], y = v[1], z = v[2], t;
    s32 s = dcp_sin(a), k = dcp_cos(a);
    t = (s32)(((s64)y * k - (s64)z * s) >> 12); z = (s32)(((s64)y * s + (s64)z * k) >> 12); y = t;
    s = dcp_sin(b), k = dcp_cos(b);
    t = (s32)(((s64)x * k + (s64)z * s) >> 12); z = (s32)(((s64)z * k - (s64)x * s) >> 12); x = t;
    s = dcp_sin(c), k = dcp_cos(c);
    t = (s32)(((s64)x * k - (s64)y * s) >> 12); y = (s32)(((s64)x * s + (s64)y * k) >> 12); x = t;
    v[0] = x, v[1] = y, v[2] = z;
}
/* EffTrail_SetNodeFromOwner: b = position + (0, -len/2, 0) R, a = position + (0, len/2, 0) R;
 * R = RotX(rot2) RotY(rot3) RotZ(angle + direction of motion in the air) */
static void EffTrail_SetNodeFromOwner(s32 *out, const DcPlayer *w, s32 len) {
    s32 v1[3] = { 0, -len / 2, 0 }, v2[3] = { 0, len / 2, 0 };
    u16 ang = 0;
    if (w->move_flags & 0x10)
        ang = (u16)dcp_atan2((w->y - w->py) >> 8, (w->x - w->px) >> 8);
    trail_rot(v1, (u16)(w->rot2 << 8), (u16)(w->rot3 << 8), (u16)(ang + (w->angle << 8)));
    trail_rot(v2, (u16)(w->rot2 << 8), (u16)(w->rot3 << 8), (u16)(ang + (w->angle << 8)));
    out[3] = w->x + v1[0], out[4] = w->y + v1[1], out[5] = w->z + v1[2];
    out[0] = w->x + v2[0], out[1] = w->y + v2[1], out[2] = w->z + v2[2];
}
static void trail_push(Trail *t) {
    t->cur = (t->cur + 1) % TRAIL_NODES;
    EffTrail_SetNodeFromOwner(t->n[t->cur], t->own, t->len);
}
static void EffTrail_End(Obj *w) {
    w->h.disp_flags |= 0x20;
    w->h.obj_flags |= 4;
}
static void EffTrail_State_Follow(Obj *w) {
    Trail *t = trail_of(w);
    if (t == NULL)
        return;
    if (t->own->type == 1 && (t->own->pf & 0x80)) {
        EffTrail_End(w); /* boosting again: Player_StartBoost made a new one */
        return;
    }
    trail_push(t);
    if (--t->cnt <= 0)
        EffTrail_End(w);
}
static void EffTrail_State_Wait(Obj *w) {
    Trail *t = trail_of(w);
    if (t == NULL)
        return;
    if (t->own->type == 1 && !(t->own->pf & 0x80)) {
        w->state = EffTrail_State_Follow;
        EffTrail_State_Follow(w);
        return;
    }
    trail_push(t);
}
static void EffTrail_State_Init(Obj *w) {
    Trail *t = trail_of(w);
    if (t == NULL)
        return;
    if (t->own->type == 1 && !(t->own->pf & 0x80)) {
        w->state = EffTrail_State_Follow;
        EffTrail_State_Follow(w);
        return;
    }
    trail_push(t);
    if (++t->cnt >= t->max) {
        t->cnt = t->max;
        w->state = EffTrail_State_Wait;
    }
    w->h.disp_flags &= ~0x20; /* (@0x70 = EffTrail_Draw) */
}
/* EffTrail_Create(arg, len px, nodes, colour, colour) */
static Obj *EffTrail_Create(DcPlayer *p, s32 a, s32 b, u16 c, u16 d) {
    Obj *w;
    Trail *t = NULL;
    int i;
    for (i = 0; i < 2 && t == NULL; i++)
        if (s_trail[i].o == NULL || !s_trail[i].o->used)
            t = &s_trail[i];
    if (t == NULL)
        return NULL;
    w = obj_alloc(OBJ_PRIO_EFFECT, EffTrail_Destructor);
    if (w == NULL)
        return NULL;
    memset(t, 0, sizeof(*t));
    t->o = w;
    t->own = p;
    t->len = a << 8;
    t->max = b;
    t->cur = 0;
    EffTrail_SetNodeFromOwner(t->n[0], p, t->len);
    t->c0 = c, t->c1 = d;
    t->a0 = 6, t->a1 = 0x12, t->a2 = 1;
    t->kdiv = 0xa, t->kmul = 3;
    w->kind = OBJK_EFFECT;
    w->h.type = 5;
    w->h.obj_flags |= 0x12;
    w->h.move_flags |= 0x2300;
    w->h.disp_flags |= 0x20;
    w->ppar = p;
    w->state = EffTrail_State_Init;
    return w;
}

/* EffTrail_Draw: segment i (0 = newest) from node i to node i + 1, colour Math_LerpRGB555(c0, c1,
 * cnt, i), alpha a0 -> a1 over the first cnt * 3 / 10 segments, then a1 -> a2 */
int objects_trail_quads(ObjTrailQuad *q, int max) {
    int i, k, n = 0;
    for (k = 0; k < 2; k++) {
        Trail *t = &s_trail[k];
        int cnt, mid, rem, node;
        if (t->o == NULL || !t->o->used || (t->o->h.obj_flags & 4) || (t->o->h.disp_flags & 0x20))
            continue;
        if (t->own->type == 1 && (t->own->pf2 & 0x20000))
            continue;
        cnt = t->cnt;
        mid = cnt * t->kmul / t->kdiv;
        rem = cnt - mid;
        node = t->cur;
        for (i = 0; i < cnt && n < max; i++) {
            int next = (node + TRAIL_NODES - 1) % TRAIL_NODES, a, c;
            u16 col;
            if (i <= mid)
                a = mid ? t->a0 - i * (t->a0 - t->a1) / mid : t->a0;
            else
                a = t->a1 - (t->a1 - t->a2) * (i - mid) / rem;
            /* Math_LerpRGB555(&col, c0, c1, cnt, i) */
            col = 0;
            for (c = 0; c < 15; c += 5) {
                s32 x0 = (t->c0 >> c) & 0x1f, x1 = (t->c1 >> c) & 0x1f;
                col |= (u16)(((x0 + (x1 - x0) * i / (cnt ? cnt : 1)) & 0x1f) << c);
            }
            q[n].x[0] = t->n[node][3], q[n].y[0] = t->n[node][4];
            q[n].x[1] = t->n[node][0], q[n].y[1] = t->n[node][1];
            q[n].x[2] = t->n[next][3], q[n].y[2] = t->n[next][4];
            q[n].x[3] = t->n[next][0], q[n].y[3] = t->n[next][1];
            q[n].bgr555 = col;
            q[n].alpha = (u8)(a & 0x1f);
            n++;
            node = next;
        }
    }
    return n;
}

/* g_dcp_hooks.effect -> the effect the original creates at that point */
void obj_player_effect(DcPlayer *p, s32 fx, s32 a, s32 b) {
    switch (fx) {
    case DCP_FX_BOOST:       /* Player_UpdateBoost: EffBoost_Create(w, -0xd, 0xd or 0) */
        EffBoost_Create(p, (s16)a, (s16)b);
        break;
    case DCP_FX_BOOST_BURST: /* Player_StartBoost: EffBoostBurst_Create(w, 0, 0); EffBoostAura_Create(w, 0, 0) */
        EffBoostBurst_Create(p, 0, 0);
        EffBoostAura_Create(p, 0, 0);
        break;
    case DCP_FX_HIGHSPEED:   /* Player_StartHighSpeed: EffHighSpeedStart_Create(w, 0, 0) */
        EffHighSpeedStart_Create(p, 0, 0);
        break;
    case DCP_FX_BLAZE_FLAME: /* a: 0 Player_State_GroundMove EffBlazeRunFlame_Spawn, 1 Player_State_Roll and
                              * 2 Player_State_InAir (anim 0x30) EffBlazeSpinFlame_Spawn, 3 the trick
                              * spins EffBlazeTrickFlame_Create(w, b), 4 Player_UpdateBoost EffBlazeBoostFlame_Create */
        switch (a) {
        case 0: EffBlazeRunFlame_Spawn(p); break;
        case 1:
        case 2: EffBlazeSpinFlame_Spawn(p); break;
        case 3: EffBlazeTrickFlame_Create(p, (u32)b); break;
        case 4: EffBlazeBoostFlame_Create(p); break;
        }
        break;
    case DCP_FX_TRAIL:       /* Player_StartBoost: EffTrail_Create(w, 0x18, 0xa, 0x7ca5, 0x7ca5) */
        EffTrail_Create(p, a, b, 0x7ca5, 0x7ca5);
        break;
    case DCP_FX_BOOST_JET:   /* EffBoostJet_Spawn */
        EffBoostJet_Spawn(p);
        break;
    case DCP_FX_SPIN_SMOKE1:
        EffSpinSmoke1_Spawn(p);
        break;
    case DCP_FX_SPIN_SMOKE2:
        EffSpinSmoke2_Spawn(p);
        break;
    case DCP_FX_BRAKE_SMOKE:
        EffBrakeSmoke_Spawn(p);
        break;
    default:
        break;
    }
}
