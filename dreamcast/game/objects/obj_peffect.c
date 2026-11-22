/*
 * Player effects (src/game/Effects/), translated onto the object system's
 * effect objects (task/work struct -> Obj, VRAM / palette calls dropped,
 * spr1 (second DS screen) not kept):
 *   EffBoost_Create, EffBoost_SetFollowState, EffBoost_State_FollowPlayer,
 *   EffBoostAura_Create, EffBoostBurst_Create (+ _Draw: the vertically
 *   mirrored second draw is a second object), EffHighSpeedStart_Create,
 *   EffBlazeBoostFlame_Create, EffBoostJet_Spawn / _Create,
 *   EffSpinSmoke1_Spawn / _Create / _State_FollowParent,
 *   EffSpinSmoke2_Spawn / _Create / _Action_Init / _State_Main,
 *   EffBrakeSmoke_Spawn.
 * The player module raises them through g_dcp_hooks.effect (DCP_FX_*);
 * obj_player.c dispatches to obj_player_effect().
 * AnimatorSprite flag 0x400 (OAM semi-transparent) is drawn in the
 * translucent list by obj_draw.c.  The 3D variants (zone >= 9 or the boss
 * acts: EffBoostJet3D, EffSpinSmoke*_3D, EffBrakeSmoke3D) are not created.
 */
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
    case DCP_FX_BLAZE_FLAME: /* a = 4: Player_UpdateBoost EffBlazeBoostFlame_Create(w); the run / spin /
                              * jump / trick flames (a = 0..3) are not translated */
        if (a == 4)
            EffBlazeBoostFlame_Create(p);
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
