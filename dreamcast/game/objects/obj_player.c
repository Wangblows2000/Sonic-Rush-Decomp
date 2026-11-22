/*
 * Player side of the object system.
 *
 *  - the player's two ObjRects (@0x2A0 body/hurt box, @0x2CC attack box) and
 *    Player_Collide (StageObj_HandleCollider for both);
 *  - the son.bac sprite timing that drives Player_SpriteCallback (command 7
 *    hit boxes per animation frame, ObjRect_SetBox3D);
 *  - the player module hooks (g_dcp_hooks) that belong to the object system:
 *    atkdef (ObjRect_SetAtkDefFlags), anim (Player_SetAnimation sprite part),
 *    anim_speed (Player_SetAnimSpeedFromVelocity sprite part), spawn_ring
 *    (RingManager_SpawnRing), and g_dcp_col.obj_probe (ObjCollisionMap_ProbeForObj);
 *  - the parts of Player_In / Player_OnHurt that write the hurt box flags
 *    (@0x2C0 / @0x2C4), which the player module does not model;
 *  - player actions the objects call that the player module has not
 *    translated: Player_Action_AttackBounce, _AttackRecoil, _DashRing,
 *    _GimmickLaunch, Player_StartLaunchTrick, Player_SetSourceMapObject.
 */
#include <string.h>
#include "obj_internal.h"

#define H g_dcp_hooks

static void psfx(DcPlayer *p, s32 ch, s32 id) {
    if (H.sfx)
        H.sfx(H.ctx, p, ch, id);
}

ObjPlayerCtx *obj_player_ctx(const DcPlayer *p) {
    int i;
    for (i = 0; i < g_obj.n_players; i++)
        if (g_obj.players[i].p == p)
            return &g_obj.players[i];
    return NULL;
}

DcPlayer *obj_box_player(const ObjBox *b) {
    return (b && b->owner_kind == OBJ_OWNER_PLAYER) ? (DcPlayer *)b->owner : NULL;
}

/* Player_SetAnimation: sprite part (asm_fallback/Player_SetAnimation.c) */
static void player_sprite_anim(ObjPlayerCtx *pc, s32 anim) {
    DcPlayer *p = pc->p;
    s32 a = anim;
    if (p->chara != 0 && a >= 0x30)
        a = (s16)(p->chara * 0x1b + a);
    pc->body.flags &= ~4; /* @0x2AA */
    pc->atk.flags &= ~4;  /* @0x2D6 */
    pc->spr.speed = 0x10; /* @0xBE */
    spr_set_anim(&pc->spr, (u16)(a >= 0 && a < 0x68 ? g_obj.data.player_spr_anim[a] : 0));
}

/* --------------------------------------------------- player hooks */
static void hook_anim(void *ctx, DcPlayer *p, s32 anim) {
    ObjPlayerCtx *pc = obj_player_ctx(p);
    (void)ctx;
    if (pc)
        player_sprite_anim(pc, anim);
    if (g_obj.prev_hooks.anim)
        g_obj.prev_hooks.anim(g_obj.prev_hooks.ctx, p, anim);
}
static void hook_anim_speed(void *ctx, DcPlayer *p, s32 s) {
    ObjPlayerCtx *pc = obj_player_ctx(p);
    (void)ctx;
    if (pc)
        pc->spr.speed = (s16)s; /* @0xBE / @0x106 */
    if (g_obj.prev_hooks.anim_speed)
        g_obj.prev_hooks.anim_speed(g_obj.prev_hooks.ctx, p, s);
}
static void hook_atkdef(void *ctx, DcPlayer *p, s32 box, s32 atk, s32 def) {
    ObjPlayerCtx *pc = obj_player_ctx(p);
    (void)ctx;
    if (pc)
        rect_set_atkdef(box == 0 ? &pc->body : &pc->atk, (u32)atk, (u32)def);
    if (g_obj.prev_hooks.atkdef)
        g_obj.prev_hooks.atkdef(g_obj.prev_hooks.ctx, p, box, atk, def);
}
static void hook_spawn_ring(void *ctx, DcPlayer *p, s32 x, s32 y, s16 vx, s16 vy, u16 flags) {
    (void)ctx;
    if (flags & 0x1000)
        ring_mark_scatter(); /* RingManager_ScatterRings: mgr->unk0 |= 2 */
    ring_spawn(x, y, vx, vy, flags);
    g_obj.stats.rings_scattered++;
    if (g_obj.prev_hooks.spawn_ring)
        g_obj.prev_hooks.spawn_ring(g_obj.prev_hooks.ctx, p, x, y, vx, vy, flags);
}
/* ------------------------------------------- AttackHitbox (src/game/Player) */
static void AttackHitbox_State_Follow(Obj *o) {
    if (o->wC != 0) {
        o->wC--;
        if (o->wC == 0) {
            o->h.obj_flags |= 4;
            return;
        }
        obj_handle_collider(&o->h, o, OBJ_OWNER_OBJECT, &o->coll[1], 0);
        return;
    }
    if (o->k.atk.par != NULL) {
        o->h.x = o->k.atk.par->x;
        o->h.y = o->k.atk.par->y;
        o->h.disp_flags &= ~1;
        if (o->k.atk.par->disp_flags & 1)
            o->h.disp_flags |= 1;
        obj_handle_collider(&o->h, o, OBJ_OWNER_OBJECT, &o->coll[1], 0);
        return;
    }
    o->h.obj_flags |= 4;
}
static void AttackHitbox_State_Boost(Obj *o) {
    DcPlayer *p;
    if (o->coll[1].flags & 0x200) {
        obj_screen_shake(4);
        o->coll[1].flags &= ~0x200;
    }
    p = o->k.atk.par;
    if (p != NULL) {
        o->h.x = p->x;
        o->h.y = p->y;
        o->h.disp_flags &= ~1;
        if (p->disp_flags & 1)
            o->h.disp_flags |= 1;
        if (!(p->pf & 0x80))
            o->h.obj_flags |= 4;
        obj_handle_collider(&o->h, o, OBJ_OWNER_OBJECT, &o->coll[1], 0);
        return;
    }
    o->h.obj_flags |= 4;
}
static Obj *AttackHitbox_Create(DcPlayer *parent, s16 a, s16 b, s16 c, s16 d, s16 e) {
    Obj *w = obj_alloc(0x101, obj_stageobj_dtor);
    if (w == NULL)
        return NULL;
    w->kind = OBJK_ATTACK;
    w->h.type = 5;
    w->h.obj_flags |= 0x10;
    w->h.x = parent->x;
    w->h.y = parent->y;
    w->h.z = parent->z;
    w->k.atk.par = parent;
    w->h.move_flags |= 0x100;
    w->h.move_flags |= 0x2000;
    w->h.obj_flags |= 0x200;
    w->coll[1].mask = parent->type == 1 ? 0x21 : 0x12;
    w->coll[1].cb = NULL;
    w->coll[1].flags |= 0x20;
    rect_set_atkdef(&w->coll[1], 0x13, 0xffff);
    rect_set_box2d(&w->coll[1], a, b, c, d);
    if (parent->disp_flags & 1)
        w->h.disp_flags |= 1;
    w->state = AttackHitbox_State_Follow;
    w->wC = e;
    return w;
}
dcp_bool obj_is_attack_hitbox(const DcPlayer *h) { return h->type == 5; }

/* g_dcp_hooks.effect: the effects the object system owns */
static void hook_effect(void *ctx, DcPlayer *p, s32 fx, s32 a, s32 b) {
    Obj *o;
    (void)ctx;
    if (fx == DCP_FX_ATTACK_HITBOX_BOOST) {
        /* Player_StartBoost: AttackHitbox_Create(w, -0x20, -0x20, 0x20, 0x20, 0) + SetBoostState */
        o = AttackHitbox_Create(p, -0x20, -0x20, 0x20, 0x20, 0);
        if (o != NULL)
            o->state = AttackHitbox_State_Boost;
    }
    if (g_obj.prev_hooks.effect)
        g_obj.prev_hooks.effect(g_obj.prev_hooks.ctx, p, fx, a, b);
}

static s32 hook_obj_probe(void *ctx, DcPlayer *p, s32 x, s32 y, u32 flags, u32 dir, u8 *angle, u8 *attr) {
    (void)ctx;
    return colmap_probe_for_obj(p, x, y, flags, dir, angle, attr);
}

void obj_player_install_hooks(void) {
    g_obj.prev_hooks = H;
    H.anim = hook_anim;
    H.anim_speed = hook_anim_speed;
    H.atkdef = hook_atkdef;
    H.spawn_ring = hook_spawn_ring;
    H.effect = hook_effect;
    g_dcp_col.obj_ctx = NULL;
    g_dcp_col.obj_probe = hook_obj_probe;
}

/* Player_InitState (collision box part) for a newly registered player */
void objects_add_player(DcPlayer *p) {
    ObjPlayerCtx *pc;
    if (p == NULL || obj_player_ctx(p) || g_obj.n_players >= 2)
        return;
    pc = &g_obj.players[g_obj.n_players++];
    memset(pc, 0, sizeof(*pc));
    pc->p = p;
    pc->body.cb = obj_player_on_hurt; /* coll2A0.unk1C = Player_OnHurt */
    pc->body.mask = 0x21;
    pc->body.flags &= ~0x300;
    pc->body.flags |= 0x20;
    if (p->invuln == 0)
        rect_set_atkdef(&pc->body, 0xa, 2);
    pc->atk.mask = 0x21;
    pc->atk.cb = NULL;
    pc->atk.flags &= ~8;
    pc->atk.flags &= ~0x300;
    pc->atk.flags |= 0x20;
    rect_set_atkdef(&pc->atk, 0, 0);
    pc->body.owner = pc->atk.owner = p;
    pc->body.owner_kind = pc->atk.owner_kind = OBJ_OWNER_PLAYER;
    spr_init(&pc->spr, &g_obj.res[OBJ_BAC_PLAYER], 0, 0);
    player_sprite_anim(pc, p->anim);
}

/* Player_SpriteCallback: command 7 slots 0/1 -> coll2A0 / coll2CC */
static void player_sprite_cb(void *owner, const HboxEvent *ev) {
    ObjPlayerCtx *pc = (ObjPlayerCtx *)owner;
    ObjBox *b;
    if (ev->slot > 1)
        return;
    b = ev->slot == 0 ? &pc->body : &pc->atk;
    b->owner = pc->p;
    b->owner_kind = OBJ_OWNER_PLAYER;
    rect_set_box3d(b, ev->l, ev->t, ev->r, ev->b, -4, 4);
}

/* Per frame, after dcp_player_frame: the hurt box flags of Player_In,
 * the sprite step of the player's draw, then Player_Collide. */
void obj_player_collide(ObjPlayerCtx *pc) {
    DcPlayer *p = pc->p;
    s16 save;
    /* Player_In: invulnerability (@0x3B2) and invincibility (@0x3B4) */
    if (p->invuln != 0)
        pc->body.def |= 5;
    else if (pc->prev_invuln != 0)
        pc->body.def = 2;
    if (p->invinc != 0) {
        pc->body.def |= 5;
        pc->body.atk |= 0x15;
    } else if (pc->prev_invinc != 0) {
        pc->body.atk = 10;
        pc->body.def = 2;
    }
    pc->prev_invuln = p->invuln;
    pc->prev_invinc = p->invinc;
    /* sprite timing (draw step of StageObj_Main: speed 0 during hit-stop/pause) */
    if (p->disp_flags & 4)
        pc->spr.flags |= SPR_LOOP;
    else
        pc->spr.flags &= ~SPR_LOOP;
    save = pc->spr.speed;
    pc->spr.speed = (s16)((save * W.timescale) >> 8);
    if ((p->hitstop & 0xf) || obj_pause_check(p->obj_flags))
        pc->spr.speed = 0;
    spr_process(&pc->spr, player_sprite_cb, pc);
    pc->spr.speed = save;
    if (g_obj.anim_done_flag && (pc->spr.flags & SPR_ENDED))
        p->disp_flags |= DCP_DF_ANIM_DONE;
    if (obj_pause_check(p->obj_flags))
        return;
    if ((p->hitstop & 0xf) || !(W.objmgr & 0x10))
        return;
    /* Player_Collide (non-VS): attack box first, then the body */
    pc->atk.flags |= 0x20;
    obj_handle_collider(p, p, OBJ_OWNER_PLAYER, &pc->atk, 1);
    obj_handle_collider(p, p, OBJ_OWNER_PLAYER, &pc->body, 1);
}

/* Player_OnHurt as the coll2A0 callback (a = player body, b = attacker box) */
void obj_player_on_hurt(ObjBox *a, ObjBox *b) {
    DcPlayer *p = obj_box_player(a);
    ObjPlayerCtx *pc;
    if (p == NULL)
        return;
    pc = obj_player_ctx(p);
    dcp_on_hurt(p, b->atk);
    if (pc && p->state == dcp_state_hurt && p->invuln != 0) {
        pc->body.def |= 5; /* w->unk2C4 |= 5 */
        pc->prev_invuln = p->invuln;
    }
    g_obj.stats.hurts++;
    obj_log("hurt", p->x >> 8, p->rings);
}

void obj_player_add_tension_combo(DcPlayer *p, s16 v) { dcp_add_tension_combo(p, v); }

/* -------------------------------------------- Player_Action_AttackBounce */
void obj_player_attack_bounce(DcPlayer *w) {
    s16 vx, v;
    if (w->move_flags & 0x10) {
        vx = w->vel_x;
        v = w->gnd_vel;
        dcp_player_init_state(w);
        dcp_set_anim(w, 2);
        w->disp_flags |= 4;
        w->move_flags |= 0x10;
        w->move_flags |= 0x8000;
        w->state = dcp_state_in_air;
        if (H.atkdef)
            H.atkdef(H.ctx, w, 1, 0x13, 0);
        w->vel_y = -0x300;
        w->vel_x = vx;
        w->gnd_vel = v;
        w->pf &= ~0xfu;
        w->pf |= 1;
    }
    w->timer = 0;
    w->trick_input = 0;
    w->charge = 0;
}

/* ------------------------------------------- Player_Action_AttackRecoil */
void obj_player_attack_recoil(DcPlayer *w) {
    dcp_player_init_state(w);
    w->move_flags |= 0x10;
    w->vel_x = w->move_x > 0 ? -0x180 : 0x180;
    w->dash_timer = 0x18;
    obj_player_attack_bounce(w);
}

/* ------------------------------------------ Player_Action_GimmickLaunch */
void Player_Action_GimmickLaunch(DcPlayer *w, s16 vx, s16 vy) {
    dcp_reset_gimmick_state(w);
    if (w->move_flags & 1)
        w->vel_x = w->gnd_vel;
    dcp_act_land(w, 0);
    dcp_rotate_vec(&vx, &vy, w->grav_rot);
    if (!(w->move_flags & 0x10))
        w->jump_y = (s16)(w->y >> 8);
    w->move_flags |= 0x10;
    w->move_flags |= 0x8000;
    w->move_flags &= ~1u;
    w->state = dcp_state_in_air;
    if (vx != 0) {
        dcp_set_anim(w, 0x18);
        w->disp_flags |= 4;
        w->vel_x = vx;
        if (w->vel_x < 0) {
            if (w->gnd_vel > 0)
                w->gnd_vel = 0;
            w->disp_flags |= 1;
        } else {
            if (w->gnd_vel < 0)
                w->gnd_vel = 0;
            w->disp_flags &= ~1;
        }
    } else {
        dcp_set_anim(w, 0x13);
        w->disp_flags |= 4;
        w->vel_x = (s16)((w->gnd_vel * dcp_cos((u16)(w->angle << 8))) >> 12);
    }
    if (vy != 0)
        w->vel_y = vy;
    else
        w->vel_y = (s16)((w->gnd_vel * dcp_sin((u16)(w->angle << 8))) >> 12);
    if (H.atkdef)
        H.atkdef(H.ctx, w, 1, 0, 0);
    w->pf &= ~0xfu;
    w->pf |= 1;
    w->pf |= 2;
    w->dash_timer = 0x30;
    w->timer = 0;
    w->trick_input = 0;
    w->charge = 0;
    w->trick_cnt = 0;
    if (!(w->pf & 0x4000000))
        return;
    w->vel_x = (s16)((w->vel_x >> 1) + (w->vel_x >> 2));
    w->vel_y = (s16)((w->vel_y >> 1) + (w->vel_y >> 2));
    /* EffAirBubble_Spawn: effect, not translated */
}

/* -------------------------------------------- Player_Action_DashRing */
void obj_player_dash_ring(DcPlayer *w, s32 x, s32 y, s16 vx, s16 vy) {
    w->pf2 &= 0xffffffafu;
    w->pf2 &= 0xffffff5fu;
    /* Camera_UnlockX / Camera_UnlockY: camera module */
    w->angle = 0;
    if (x != 0)
        w->x = x;
    if (y != 0)
        w->y = y;
    Player_Action_GimmickLaunch(w, vx, vy);
    if (vx == 0)
        w->vel_x = 0;
    w->dash_timer = 0x1e;
    w->timer = 0x1e;
    w->move_flags &= ~0x80u;
    psfx(w, DCP_CH_VOICE, 0x41);
}

/* -------------------------------------------- Player_StartLaunchTrick */
void obj_player_start_launch_trick(DcPlayer *w) {
    s32 t = dcp_atan2(w->vel_y, w->vel_x);
    w->angle = (u8)((t >> 8) + 0x40);
    dcp_set_anim(w, 0x38);
    psfx(w, DCP_CH_VOICE, 0xe);
    dcp_add_trick_score(w, 0x190);
}

/* Player_SetSourceMapObject: @0x3E4 = map object parameter (byte +0xA is
 * the trick counter, ObjParam.pA) */
void obj_player_set_source(DcPlayer *p, Obj *o) { p->trick_obj = (u8 *)o->prm; }
