/*
 * Player creation, per-frame driver, input, timers, tension, rings.
 * Translations of Player_Create (physics part), Player_InitState,
 * Player_LoadPhysicsParams, Player_In, Player_Last, Player_RemapButtons,
 * Player_ReadInput, StageObj_Main (player path), Player_HandleSurfaceLanding,
 * Player_UpdateGravityFlip, Player_CheckCrushed, Player_Unk_ov000_023460ec,
 * Player_AddTension(+Combo), Player_DrainTension, Player_AddRings,
 * RingManager_ScatterRings (velocity pattern), Player_AddTrickScore,
 * Player_ResetGimmickState, Player_Respawn (physics part), Player_SetAnimation
 * (state part).
 */
#include "dcp_player.h"
#include "dcp_collision.h"

#define H g_dcp_hooks
#define W g_dcp_world

static void sfx(DcPlayer *p, s32 ch, s32 id) { if (H.sfx) H.sfx(H.ctx, p, ch, id); }
static void sfx_stop(DcPlayer *p, s32 ch) { if (H.sfx_stop) H.sfx_stop(H.ctx, p, ch); }

/* Player_SetAnimation: the physics-relevant part (asm_fallback/Player_SetAnimation.c):
 * store the raw id, clear ANIM_DONE (8) and LOOP (4); the model/sprite switch
 * goes to the hook, after which the blend request bit 0x400 is consumed. */
void dcp_set_anim(DcPlayer *p, s32 anim) {
    p->anim = (s16)anim;
    p->disp_flags &= ~8;
    p->disp_flags &= ~4;
    if (H.anim)
        H.anim(H.ctx, p, anim);
    p->disp_flags &= ~0x400;
}

/* ----------------------------------------------- Player_LoadPhysicsParams */
void dcp_player_load_physics(DcPlayer *w) {
    const DcpPhysics *t = dcp_physics(w->chara);
    w->acc = t->acc;
    if (W.zone >= 9 || W.act >= 2)
        w->top = t->top / 2;
    else
        w->top = t->top;
    w->dec = t->dec;
    w->sd_base = t->sd_base;
    w->sd_add = t->sd_add;
    w->sd_max = t->sd_max;
    w->roll_dec = t->roll_dec;
    w->hs_top = t->hs_top;
    w->boost_acc = t->boost_acc;
    w->boost_top = t->boost_top;
    w->boost_dec = t->boost_dec;
    w->hs_min = t->hs_min;
    w->slope_bonus = t->slope_bonus;
    w->jump = t->jump;
    w->p374 = t->p1a;
    w->invuln_len = t->invuln_len;
    w->fall_grace = (u8)t->fall_grace;
    /* the original computes (s16)(top * 0.15 / 0.3 / 0.5 / 0.7 / 0.9) in
     * double precision; dcp_mul_double reproduces that bit-exactly with
     * integers (SH-4 KOS builds use -m4-single-only, where double == float) */
    w->run_thr[0] = (s16)dcp_mul_double(w->top, DCP_D_0_15);
    w->run_thr[1] = (s16)dcp_mul_double(w->top, DCP_D_0_3);
    w->run_thr[2] = (s16)dcp_mul_double(w->top, DCP_D_0_5);
    w->run_thr[3] = (s16)dcp_mul_double(w->top, DCP_D_0_7);
    w->run_thr[4] = (s16)dcp_mul_double(w->top, DCP_D_0_9);
    w->slope_lim = 0x1c;
    w->slope_acc = t->slope_acc;
    w->gnd_max = t->gnd_max;
    w->gravity = t->gravity;
    w->fall_max = t->fall_max;
    w->ride_max = t->ride_max;
    if (w->pf & 0x4000000) {
        w->jump = (w->jump >> 1) + (w->jump >> 2);
        w->gravity = w->gravity >> 1;
    }
}

/* -------------------------------------------------------- Player_InitState */
void dcp_player_init_state(DcPlayer *w) {
    w->box_l = -7;
    w->box_r = 7;
    w->box_t = -0xc;
    w->box_b = 0xd;
    if (H.atkdef && w->invuln == 0)
        H.atkdef(H.ctx, w, 0, 0xa, 2);
    if (H.atkdef)
        H.atkdef(H.ctx, w, 1, 0, 0);
    dcp_player_load_physics(w);
    w->angle = 0;
    w->rot2 = 0;
    w->rot3 = 0;
    w->gnd_vel = 0;
    w->vel_x = 0;
    w->vel_y = 0;
    w->vel_z = 0;
    w->air_lock = 0;
    if (!(w->pf2 & 0x100))
        w->z = 0;
    w->ride = NULL;
    if (!(w->pf2 & 0x600)) {
        w->gimmick = NULL;
        w->pf2 &= ~0x38;
        w->pf2 &= ~0xc0;
    }
    w->still_cnt = 0;
    w->boost_cool = 0;
    w->trick_cnt = 0;
    w->hover_timer = 0x78;
    w->land_speed = 0;
    w->jump_y = 0;
    if (w->layer_ovr & 0x80)
        w->obj_flags |= 1;
    w->pf2 &= ~0x20003;
    w->pf2 &= ~0x40000;
    w->pf2 &= ~0x400000;
    w->pf &= ~0x80000;
    w->pf &= ~0xf;
    w->pf &= ~0xc040;
    w->pf &= ~0x102000;
    w->pf &= ~0x400000;
    w->pf &= ~0x400;
    w->obj_flags &= ~2;
    w->obj_flags |= 0x10;
    w->disp_flags &= ~0x20;
    if (W.zone < 9 && W.act < 2)
        w->disp_flags |= 0x200;
    w->disp_flags |= 0x100;
    w->move_flags |= 0x80;
    w->move_flags |= 0x40;
    w->move_flags |= 0x20000;
    w->move_flags |= 0x40000;
    w->move_flags |= 0x80000;
    w->move_flags |= 0x200000;
    w->move_flags &= ~0xf;
    w->move_flags &= ~0x100;
    w->move_flags &= ~0x200;
    w->move_flags &= ~0x10;
    w->move_flags &= ~0x2000;
    w->move_flags &= ~0x8000;
    w->move_flags &= ~0x4000;
    w->move_flags &= ~0x10000000;
    sfx_stop(w, DCP_CH_VOICE);
    sfx_stop(w, DCP_CH_TRICK);
}

/* ---------------------------------------------- Player_Create (physics) */
void dcp_player_create(DcPlayer *w, u8 chara, u8 pid, s32 x_px, s32 y_px) {
    u8 *b = (u8 *)w;
    u32 i;
    for (i = 0; i < sizeof(*w); i++)
        b[i] = 0;
    w->pid = pid;
    w->cam = pid;
    w->chara = chara;
    w->type = 1;
    w->obj_flags |= 0x10;
    w->obj_flags |= 1;
    w->disp_flags |= 0x800;
    w->map_right = 0x10;
    w->map_left = 0x20;
    w->map_up = 0x40;
    w->map_down = 0x80;
    w->map_a = 1;
    w->map_b = 2;
    w->disp_flags |= 0x200;
    w->act_idle = dcp_act_ground_idle;
    w->act_move = dcp_act_ground_move;
    w->act_crouch = dcp_act_crouch;
    w->act_jump = dcp_act_jump;
    w->act_air = chara == 1 ? dcp_act_hover : dcp_act_homing;
    w->act_348 = NULL;
    w->unk3CA = 0;
    w->trick_obj = NULL;
    dcp_player_init_state(w);
    w->x = x_px << 8;
    w->y = y_px << 8;
    if (W.zone < 9 && W.act < 2)
        w->tension = 0x640;
    if (H.gravity_flipped && H.gravity_flipped(H.ctx, w->x, w->y))
        w->disp_flags ^= 1;
    w->act_idle(w);
    w->disp_flags |= 0x40;
}

/* ------------------------------------------------------ input remapping */
u16 dcp_remap_buttons(DcPlayer *w, u16 in) {
    u16 out = in & ~0xF3;
    if (in & 0x10) out |= w->map_right;
    if (in & 0x20) out |= w->map_left;
    if (in & 0x40) out |= w->map_up;
    if (in & 0x80) out |= w->map_down;
    if (in & 1) out |= w->map_a;
    if (in & 2) out |= w->map_b;
    return out;
}

/* -------------------------------------------- Player_UpdateGravityFlip */
void dcp_update_gravity_flip(DcPlayer *p) {
    dcp_bool f = H.gravity_flipped ? H.gravity_flipped(H.ctx, p->x, p->y) : DCP_FALSE;
    if (f) {
        if (p->grav_rot == 0) {
            p->vel_x = -p->vel_x;
            p->vel_y = -p->vel_y;
            p->angle = (u8)(p->angle + 0x80);
            p->map_left = 0x10;
            p->map_right = 0x20;
        }
        p->grav_rot = 0x80;
        return;
    }
    if (p->grav_rot == 0x80) {
        p->vel_x = -p->vel_x;
        p->vel_y = -p->vel_y;
        p->angle = (u8)(p->angle + 0x80);
        p->map_left = 0x20;
        p->map_right = 0x10;
    }
    p->grav_rot = 0;
}

/* ------------------------------------------ Player_HandleSurfaceLanding */
void dcp_handle_surface_landing(DcPlayer *w) {
    u8 ang;
    u32 fl;
    if (!(w->pf2 & 1))
        return;
    ang = 0;
    fl = w->move_flags;
    if (!(fl & 0xf))
        return;
    if (fl & 1) {
        dcp_act_land(w, ang);
    } else if (fl & 2) {
        dcp_act_land(w, (w->disp_flags & 1) ? 0x60 : 0xa0);
        ang = w->angle;
        dcp_probe_tile(w->x >> 8, (w->y >> 8) + w->box_t - 4, w->obj_flags & 1, 3, &ang, NULL);
        w->angle = ang;
    } else if (fl & 4) {
        dcp_act_land(w, (w->disp_flags & 1) ? 0x40 : 0xc0);
    } else if (fl & 8) {
        dcp_act_land(w, (w->disp_flags & 1) ? 0xc0 : 0x40);
    }
    if (w->pf2 & 2) {
        w->disp_flags ^= 1;
        w->gnd_vel = -w->gnd_vel;
    }
    w->move_flags |= 1;
    w->pf2 &= ~3;
    w->act_idle(w);
}

/* --------------------------------------------------- Player_CheckCrushed */
void dcp_check_crushed(DcPlayer *w) {
    if ((w->pf & 0x800) && w->touch == NULL && (w->move_flags & 4) && (w->move_flags & 8))
        dcp_act_die(w);
    if (w->touch != NULL && !(w->pf & 0x1000)) {
        if (((w->move_flags & 1) && (w->move_flags & 2)) || ((w->move_flags & 4) && (w->move_flags & 8))) {
            dcp_act_die(w);
            return;
        }
        w->crush = 0;
        return;
    }
    w->crush = 0;
}

/* ------------------------------------------- Player_Unk_ov000_023460ec */
void dcp_update_ride_max(DcPlayer *w) {
    const DcpPhysics *t = dcp_physics(w->chara);
    if (w->anim == 0xc || (w->pf & 0x80)) {
        w->move_flags |= 0x2000000;
        if (w->pf & 0x80) {
            w->ride_max = w->boost_top >> 1;
            return;
        }
        w->ride_max = t->ride_max;
        return;
    }
    w->ride_max = t->ride_max;
    w->move_flags &= ~0x2000000;
}

/* ------------------------------------------------------------- Player_In */
/* Player_UpdateGimmickStageBounds: the horizontal map collision limits follow a gimmick that holds
 * the player (pf2 0x200: +-0x6c px around it), else 0 .. map width. (The vertical pair, unk20 /
 * unk28 with pf2 0x400, has no consumer in the port.) */
static void dcp_update_gimmick_stage_bounds(DcPlayer *w) {
    s32 width = g_dcp_col.map ? g_dcp_col.map->right : W.map_right;
    if (w->pid != 0 || W.zone >= 9 || W.act >= 2)
        return;
    if (w->pf2 & 0x200) {
        if (w->gimmick != NULL) {
            W.map_left = (((const DcPlayer *)w->gimmick)->x >> 8) - 0x6c;
            W.map_right = (((const DcPlayer *)w->gimmick)->x >> 8) + 0x6c;
        } else {
            w->pf2 &= ~0x200;
            W.map_left = 0;
            W.map_right = width;
        }
    } else {
        W.map_left = 0;
        W.map_right = width;
    }
    if ((w->pf2 & 0x400) && w->gimmick == NULL)
        w->pf2 &= ~0x400;
}

void dcp_player_in(DcPlayer *w) {
    dcp_update_gimmick_stage_bounds(w);
    dcp_update_gravity_flip(w);
    /* Player_ReadInput */
    if (!(w->pf & 0x200000) && w->pid == 0) {
        w->held = dcp_remap_buttons(w, w->pad_raw[0]);
        w->pressed = dcp_remap_buttons(w, w->pad_raw[1]);
        w->released = dcp_remap_buttons(w, w->pad_raw[2]);
    }
    if (w->layer_ovr != 0) {
        if ((w->anim < 0x1a || w->anim > 0x20) && !(w->pf2 & 4) && (w->layer_set & 2)) {
            w->obj_flags &= ~1;
            w->obj_flags |= w->layer_set & 1;
            w->layer_set = 0;
            if (!(w->layer_ovr & 0x80))
                w->layer_ovr = 0;
        }
        w->pf2 &= ~4;
    }
    if (w->dash_timer != 0)
        w->dash_timer--;
    if (w->vs_timer != 0)
        w->vs_timer--;
    if (w->combo_timer != 0)
        w->combo_timer--;
    if (w->confuse != 0) {
        /* gPlayerDpadRemapTable = {0x10, 0x20, 0x40, 0x80} (code constant of the remap) */
        static const u16 col[4] = { 0x10, 0x20, 0x40, 0x80 };
        if (!(w->confuse & 0x7f)) {
            u16 r = dcp_rand() & 3;
            w->map_up = col[(u16)(r + 1) & 3];
            w->map_down = col[(u16)(r + 2) & 3];
            w->map_left = col[(u16)(r + 3) & 3];
            w->map_right = col[(u16)(r + 4) & 3];
        }
        w->confuse--;
        if (w->confuse == 0) {
            w->map_up = 0x40;
            w->map_down = 0x80;
            if (H.gravity_flipped && H.gravity_flipped(H.ctx, w->x, w->y)) {
                w->map_left = 0x10;
                w->map_right = 0x20;
            } else {
                w->map_left = 0x20;
                w->map_right = 0x10;
            }
        }
    }
    if (w->slowdown != 0) {
        const DcpPhysics *t = dcp_physics(w->chara);
        w->slowdown--;
        w->top = t->top >> 2;
        w->boost_top = t->boost_top >> 1;
        w->gnd_max = t->gnd_max >> 2;
        w->hs_min = t->hs_min >> 2;
        if (w->slowdown == 0) {
            w->top = t->top;
            w->boost_top = t->boost_top;
            w->gnd_max = t->gnd_max;
            w->hs_min = t->hs_min;
        }
    }
    if (w->respawn_inv != 0)
        w->respawn_inv--;
    if (w->invuln != 0) {
        w->invuln--;
        if (w->invuln & 2)
            w->disp_flags |= 0x20;
        else
            w->disp_flags &= ~0x20;
        if (w->invuln == 0)
            w->disp_flags &= ~0x20;
    }
    if (w->tension_max != 0) {
        if (W.flags & 1)
            w->tension_max--;
        if (w->tension_max == 0 && !(w->pf & 0x1000000))
            sfx(w, DCP_CH_GLOBAL, 0x3c);
    }
    if (w->invinc != 0) {
        w->unk3CA = 0;
        w->invinc--;
        if (w->invinc == 0)
            w->disp_flags &= ~0x20;
    }
    if (!(w->move_flags & 0x100) && !(w->pf & 0x1000000) && (W.zone != 6 || W.act != 0)) {
        if (W.death_y - 0x1a <= (w->y >> 8))
            dcp_act_die(w);
        if (W.zone == 6 && W.act == 1 && (w->y >> 8) <= 0x1a)
            dcp_act_die(w);
    }
    /* Player_UpdateUnderwater / Player_FindHomingTarget: stage/object side,
     * the object module sets w->homing before the frame. */
    dcp_handle_surface_landing(w);
    dcp_update_boost(w);
    /* Player_CheckStartGrind: gimmick, not translated */
    dcp_drain_boost_tension(w);
    dcp_update_ride_max(w);
    dcp_check_crushed(w);
    /* Player_CheckTimeOver: stage timer, handled by the stage module */
}

/* ----------------------------------------------------------- Player_Last */
void dcp_player_last(DcPlayer *w) {
    u32 f;
    w->pf &= ~0x10000;
    if (w->tension_max == 0 && (w->pf & 0x40000)) {
        dcp_add_tension(w, -0x640);
        w->pf &= ~0x40000;
    }
    /* RingManager_AttractRings, Player_UpdateCamera(+Lead): other modules */
    if ((w->move_flags & 4) && !(w->move_flags & 0x4000))
        w->dash_timer = 0;
    if (w->gimmick == NULL || (w->pf2 & 8))
        w->move_flags &= ~0x4000;
    f = w->pf2;
    if (f & 0x1000) {
        w->pf2 = f & ~0x1000;
        w->pf2 |= 0x80000;
        w->move_flags |= 0x80;
        w->move_flags &= ~0x100;
    } else if (f & 0x80000) {
        w->pf2 = f & ~0x100000;
        if (!(w->move_flags & 0x10)) {
            w->z = dcp_spd_down(w->z, 0x200);
            w->vel_z = (s16)dcp_spd_down(w->vel_z, 0x80);
        }
    }
    if (w->pf & 0x2000000) {
        const DcpPhysics *t = dcp_physics(w->chara);
        w->gravity = t->gravity;
        w->fall_max = t->fall_max;
        w->pf &= ~0x2000000;
        if (w->pf & 0x4000000)
            w->gravity >>= 1;
    }
    if (w->pf2 & 0x2000) {
        w->pf2 &= ~0x2000;
        w->obj_flags |= 1;
    }
    if (w->pf2 & 0x10000) {
        w->pf2 &= ~0x10000;
        w->obj_flags |= 1;
    }
    w->pf &= ~0x20000;
}

/* ------------------------------------------ StageObj_Main (player task) */
void dcp_player_frame(DcPlayer *o, u16 held, u16 pressed, u16 released) {
    dcp_bool paused;
    if (o->obj_flags & 4)
        return;
    if (o->obj_flags & 8)
        o->obj_flags |= 4;
    dcp_handle_ride(o);
    paused = (W.objmgr & 1) && !(o->obj_flags & 0x20);
    if (!paused || (o->obj_flags & 0x40)) {
        o->pad_raw[0] = held;
        o->pad_raw[1] = pressed;
        o->pad_raw[2] = released;
        dcp_player_in(o);
    }
    if (!paused) {
        if (o->hitstop != 0) {
            if (o->hitstop & 0xf)
                o->hitstop--;
            if (o->hitstop & 0xf0)
                o->hitstop -= 0x10;
        }
        if ((o->hitstop & 0xf) == 0 && (o->obj_flags & 0x80) == 0 && o->state != NULL)
            o->state(o);
        if ((o->move_flags & 0x2000) == 0)
            dcp_obj_move(o);
        if ((W.objmgr & 8) && (o->move_flags & 0x100) == 0) {
            o->ride = NULL;
            o->touch = NULL;
            dcp_obj_collision(o);
        }
    }
    if (paused)
        return;
    /* fn78 = Player_Collide: hit-box registration, object module */
    o->push_x = 0;
    o->push_y = 0;
    o->push_z = 0;
    o->shake_x = 0;
    o->shake_y = 0;
    dcp_player_last(o);
    W.frame++;
}

/* ------------------------------------------------------- tension gauge */
void dcp_add_tension(DcPlayer *w, s16 v) {
    s16 t;
    if ((W.flags & 0x20) && v == 0x12c0 && H.vs_tension)   /* Battle Play: EffVsTensionMax */
        H.vs_tension(H.ctx, w, 1);
    if (w->pf & 0x400000)
        return;
    if (v < 0) {
        if (w->tension_max != 0)
            sfx(w, DCP_CH_GLOBAL, 0x3c);
        w->tension_max = 0;
    }
    w->tension += v;
    if (w->tension_max == 0) {
        if (w->tension >= 0x12c0) {
            w->tension_max = 0x140;
            sfx(w, DCP_CH_GLOBAL, 0x3a);
        }
    } else if (v > 0) {
        w->tension_max = 0x140;
        sfx(w, DCP_CH_GLOBAL, 0x3b);
    }
    t = w->tension;
    if (t < 0)
        t = 0;
    else if (t > 0x12c0)
        t = 0x12c0;
    w->tension = t;
}

void dcp_add_tension_combo(DcPlayer *w, s16 v) {
    s32 t;
    if (w->combo_timer == 0)
        w->combo = 0;
    t = (s16)((v >> 1) * w->combo + v);
    if (t < 0)
        t = 0;
    else if (t > 0x12bf)
        t = 0x12bf;
    dcp_add_tension(w, (s16)t);
    w->combo_timer = 0x60;
    w->combo++;
}

void dcp_drain_tension(DcPlayer *w) {
    if (H.vs_tension)   /* TensionSound_Create(0x32, 0x80) + EffVsTensionDrain_Create */
        H.vs_tension(H.ctx, w, 0);
    dcp_add_tension(w, -0x12c0);
    if (H.tension_hud)
        H.tension_hud(H.ctx, w, (u32)(w->tension << 12) >> 16);
}

/* ------------------------------------------------------------- rings */
void dcp_add_rings(DcPlayer *w, s16 d) {
    s16 old = w->rings;
    s16 t;
    s32 v;
    w->rings = old + d;
    v = w->rings;
    w->rings = (s16)(v < 0 ? 0 : (v > 999 ? 999 : v));
    w->rings_total += d;
    v = w->rings_total;
    w->rings_total = (s16)(v < 0 ? 0 : (v > 9999 ? 9999 : v));
    sfx(w, DCP_CH_GLOBAL, 4);
    if (W.mode - 1U <= 1)
        return;
    for (t = 100; t <= 900; t += 100) {
        if (old < t && w->rings >= t && H.add_life)
            H.add_life(H.ctx, w, 1);
    }
}

/* RingManager_ScatterRings: the scatter velocity pattern. level is the ring
 * manager's unk9D5 (scatter count, raises the spread each hit). */
void dcp_scatter_rings(DcPlayer *w, u8 *level) {
    u8 i, n, cnt = 0;
    s16 vx = 0, vy = 0;
    s32 ang = 0x488, sh;
    u16 flags = 0x1000;
    n = (u8)w->rings;
    w->rings = 0;
    if (n > 0x40)
        n = 0x40;
    if ((w->obj_flags & 1) && !(w->pf2 & 0x40000))
        flags |= 0x10;
    ang += *level << 8;
    for (i = 0; i < 0x40; i++) {
        if (ang >= 0) {
            sh = ang >> 8;
            if (sh >= 6)
                sh = 9 - sh;
            vx = (s16)(dcp_sin((u16)((ang & 0xff) << 8)) >> sh);
            vy = (s16)(dcp_cos((u16)((ang & 0xff) << 8)) >> sh);
            vx -= vx / 4;
            vy -= vy / 4;
            ang = (ang + 0x10) | 0x80;
        }
        if (H.spawn_ring)
            H.spawn_ring(H.ctx, w, w->x, w->y, vx, vy, flags);
        ang = -ang;
        vx = -vx;
        cnt++;
        if (cnt >= n) {
            if (*level < 4) /* (3 when the save option bit 0 is set) */
                (*level)++;
            return;
        }
    }
}

/* ---------------------------------------------------------- score */
void dcp_add_trick_score(DcPlayer *w, u32 b) {
    u32 a;
    switch (w->trick_cnt) {
    case 0: a = 0; break;
    case 1: a = 100; break;
    case 2: a = 200; break;
    default: a = 300; break;
    }
    a >>= w->trick_lvl;
    b >>= w->trick_lvl;
    if (H.score) {
        H.score(H.ctx, w, (s32)a);
        H.score(H.ctx, w, (s32)b);
    }
    w->trick_cnt++;
}

/* ------------------------------------------------ Player_ResetGimmickState */
void dcp_reset_gimmick_state(DcPlayer *w) {
    w->gimmick = NULL;
    w->trick_cnt = 0;
    w->air_lock = 0;
    if (w->layer_ovr & 0x80) {
        w->obj_flags |= 1;
        w->layer_ovr = 0;
    }
    w->pf &= ~0x80000;
    w->pf &= ~0xf;
    w->pf &= ~0xc000;
    w->pf2 &= ~0x40000;
    w->pf2 &= 0xfffdff0f;
    w->pf &= 0xffefdfff;
    w->disp_flags |= 0x100;
    w->move_flags |= 0x80;
    w->move_flags |= 0x40;
    w->move_flags &= ~0x2000;
    w->move_flags &= ~0x100;
    w->move_flags &= ~0x200;
    w->move_flags &= ~0x8000;
    w->move_flags &= ~0x4000;
    w->move_flags |= 0x40000;
    w->move_flags |= 0x20000;
    w->move_flags |= 0x200000;
    w->move_flags &= ~0x10000000;
    w->disp_flags &= ~0x20;
    w->rot3 = 0;
    w->rot2 = 0;
    if (W.zone < 9 && W.act < 2)
        w->disp_flags |= 0x200;
    sfx_stop(w, DCP_CH_VOICE);
}

/* ------------------------------------------------ Player_Respawn (physics) */
void dcp_respawn(DcPlayer *w) {
    if (W.mode != 1 && !(w->pf2 & 0x400000)) {
        if (W.lives != 0 && W.mode != 2) {
            W.lives--;
        } else {
            w->pf |= 0x800000; /* game over: Player_Last starts the exit */
            return;
        }
    }
    if (H.restart)
        H.restart(H.ctx, w);
    w->x = W.check_x << 8;
    w->y = W.check_y << 8;
    if (w->pf2 & 0x400000) {   /* Player_State_Warp (Battle Play Attract): at the rival */
        w->x = w->warp_x;
        w->y = w->warp_y;
        w->z = 0;
        w->warp_x = 0;
        w->warp_y = 0;
        w->invuln = w->invuln_len;
        w->respawn_inv = w->invuln;
        w->pf2 &= ~0x400000u;
        goto common;
    }
    w->slowdown = 0;
    w->grav_rot = 0;
    w->confuse = 0;
    w->map_up = 0x40;
    w->map_down = 0x80;
    w->map_left = 0x20;
    w->map_right = 0x10;
    w->pf &= ~0x30000000;
    w->obj_flags |= 1;
    w->tension_max = 0;
    w->vs_timer = 0;
    w->invinc = 0;
    w->invuln = 0;
    w->respawn_inv = 0;
    w->rings = 0;
    if (W.flags & 0x20)
        w->rings = 1;
common:
    W.objmgr &= ~0x100;
    W.wind_x = 0;
    W.wind_y = 0;
    w->pf &= ~0x1800;
    w->pf &= ~0x180;
    w->pf2 &= ~(0x2000 | 0x200 | 0x400 | 0x100 | 0x1000 | 0x80000 | 0x100000 | 0x8000 | 0x4000 | 0xa0 | 0x50 | 4);
    w->combo_timer = 0;
    w->combo = 0;
    w->layer_ovr = 0;
    w->layer_set = 0;
    w->hs_timer = 0;
    w->unk44 = 0;
    w->unk46 = 0;
    dcp_player_init_state(w);
}
