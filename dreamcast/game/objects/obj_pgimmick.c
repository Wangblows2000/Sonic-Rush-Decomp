/*
 * Player gimmick states and Player_In parts that the stage objects need and
 * the player module (dreamcast/game/player) has not translated.  They work
 * on the player through its documented entry points (DcPlayer fields with
 * their original offsets, dcp_* actions); player work the module does not
 * keep (@0x3EE..@0x3FC) lives in ObjPlayerCtx.
 *
 * Copied from src/game/Player/ and adapted (offset structs -> DcPlayer
 * fields, Sound_PlaySeqArcEx(w->unk384/388/390, ...) -> g_dcp_hooks.sfx on
 * channel VOICE/BOOST/TRICK, Eff*_Create -> g_obj_hooks.effect):
 *   Player_Action_Rope, Player_State_Rope, Player_Action_RopeC,
 *   Player_State_RopeC, Player_Action_BendStick, Player_State_BendStick,
 *   Player_Action_BendStickLaunch, Player_Action_CorkscrewPath,
 *   Player_State_CorkscrewPath, Player_MoveAlongCorkscrew,
 *   Player_MoveAlongCorkscrewPath, Player_Action_GmkSpecial,
 *   Player_State_GmkSpecial, Player_Action_GmkSpecialEnter,
 *   Player_State_GmkSpecialEnter, Player_Action_PedestalLaunch,
 *   Player_Action_Grind, Player_State_Grind, Player_Action_GrindTrick,
 *   Player_CheckStartGrind, Player_FindHomingTarget, Player_GiveMagnet.
 * Field map: unk1 angle, unk2/unk3 rot2/rot3, unk5 grav_rot, unk6/7 shake,
 * unk8 trick_input, unkC timer, unk1C/20/24 px/py/pz, unk28/2A/2C move_*,
 * unk2E/30/32 vel_*, unk34 gnd_vel, unk54 obj_flags, unk58 move_flags,
 * unk5C disp_flags, unk5E hit, unk62 hitstop, unk67 box_b, unk68 state,
 * unk2CC attack box, unk324 chara, unk328 anim, unk32A charge, unk32C pf,
 * unk330 pf2, unk334 act_idle, unk340 act_jump, unk350 tension, unk352
 * dash_timer, unk366/368 boost_acc/top, unk370 jump, unk378.. run_thr[],
 * unk3CA unk3CA, unk3CF trick_cnt, unk3D0 trick_lvl, unk3DC homing,
 * unk3E8 gimmick, unk410/411 layer_ovr/layer_set, unk420/422 held/pressed.
 */
#include <string.h>
#include "obj_internal.h"

#define H g_dcp_hooks
#define ABS(x) ((x) < 0 ? -(x) : (x))

static ObjPlayerCtx s_dummy;
static ObjPlayerCtx *ctx(DcPlayer *w) {
    ObjPlayerCtx *c = obj_player_ctx(w);
    return c ? c : &s_dummy;
}
static void psfx(DcPlayer *p, s32 ch, s32 id) {
    if (H.sfx)
        H.sfx(H.ctx, p, ch, id);
}
static void psfx_stop(DcPlayer *p, s32 ch) {
    if (H.sfx_stop)
        H.sfx_stop(H.ctx, p, ch);
}
static void pfx(DcPlayer *p, s32 kind, s32 a, s32 b) {
    if (g_obj_hooks.effect)
        g_obj_hooks.effect(g_obj_hooks.ctx, p, kind, a, b);
}
static void patkdef(DcPlayer *p, s32 box, s32 a, s32 d) {
    if (H.atkdef)
        H.atkdef(H.ctx, p, box, a, d);
}
/* Player_SetAnimSpeedFromVelocity (same code as the player module's) */
static void anim_speed(DcPlayer *p, s32 v) {
    s32 t = (v >> 5) + (v >> 6);
    s16 s = (s16)ABS(t);
    if (s <= 0x10)
        s = 0x10;
    if (s >= 0x80)
        s = 0x80;
    if (H.anim_speed)
        H.anim_speed(H.ctx, p, s);
}
static void gimmick_count(const char *ev, DcPlayer *p, s32 a) {
    g_obj.stats.gimmicks++;
    obj_log(ev, a, p->x >> 8);
}

/* ================================================================ Rope */
void Player_State_Rope(DcPlayer *w);
void Player_Action_Rope(DcPlayer *w, Obj *parent, s32 tx, s32 ty) {
    ObjPlayerCtx *c = ctx(w);
    s16 spd = w->gnd_vel;
    s32 t;
    dcp_reset_gimmick_state(w);
    w->gimmick = parent;
    dcp_set_anim(w, 0x24);
    w->disp_flags |= 4;
    w->state = Player_State_Rope;
    w->move_flags |= 0x10;
    w->move_flags |= 0x8000;
    w->move_flags |= 0x4000;
    w->move_flags &= ~0x80u;
    w->move_flags &= ~0x40u;
    w->pf |= 0x100000;
    if (w->angle == 0x40 || w->angle == 0xc0) {
        if (w->gnd_vel > 0x900)
            w->gnd_vel = 0x900;
        if (w->gnd_vel < -0x900)
            w->gnd_vel = -0x900;
    }
    w->vel_x = (s16)((w->gnd_vel * obj_cos((u16)(w->angle << 8))) >> 12);
    w->vel_y = (s16)((w->gnd_vel * obj_sin((u16)(w->angle << 8))) >> 12);
    t = (w->gnd_vel << 4) >> 8;
    c->g3F0 = (t * t) << 1;
    if (c->g3F0 > 0x8000)
        c->g3F0 = 0x8000;
    if (w->angle == 0x40 || w->angle == 0xc0) {
        c->g3F4 = w->vel_y & 0xffff;
        w->vel_x = 0;
        w->gnd_vel = 0;
    } else {
        c->g3F4 = (w->vel_x << 16) | (w->vel_y & 0xffff);
    }
    if (spd < 0)
        w->angle += 0x40;
    else
        w->angle -= 0x40;
    c->g3F8 = tx;
    c->g3FC = ty;
    w->trick_input = 0;
    w->timer = 0;
    psfx(w, DCP_CH_VOICE, 0x4d);
    gimmick_count("rope", w, 0);
}
void Player_State_Rope(DcPlayer *w) {
    ObjPlayerCtx *c = ctx(w);
    void *p;
    s32 dx, dy;
    if (w->trick_input != 0) {
        w->trick_input--;
        w->vel_x = (s16)(w->vel_x + c->g3F0);
        w->vel_y = (s16)(w->vel_y + c->g3F4);
        if (w->trick_input != 0)
            return;
        p = w->gimmick;
        w->angle = 0;
        w->gimmick = NULL;
        w->move_flags |= 0xc0;
        w->move_flags &= ~0x4000u;
        w->pf &= ~0x100000u;
        dcp_act_spring(w, w->vel_x, w->vel_y);
        if (p != NULL)
            obj_player_set_source(w, (Obj *)p);
        return;
    }
    if (w->timer != 0) {
        w->shake_x = (s8)((((w->timer << 1) + 8) & 7) - 4);
        w->shake_y = (s8)((((w->timer << 1) + 8) & 7) - 4);
        w->timer--;
        if (w->timer == 0) {
            c->g3F0 = (s16)-(c->g3F4 >> 20);
            c->g3F4 = (s16)-((s16)c->g3F4 >> 4);
            w->trick_input = 0x1c;
            return;
        }
    } else {
        if (w->gimmick != NULL) {
            dy = (c->g3FC - w->y) >> 8;
            dx = (c->g3F8 - w->x) >> 8;
            if (dx * dx + dy * dy > c->g3F0) {
                w->timer = 8;
                w->vel_x = 0;
                w->vel_y = 0;
            }
        }
        if (w->move_x == 0 && w->move_y == 0 && w->act_idle != NULL) {
            w->angle = 0;
            w->gimmick = NULL;
            w->move_flags |= 0xc0;
            w->move_flags &= ~0x8000u;
            w->move_flags &= ~0x4000u;
            w->pf &= ~0x100000u;
            w->act_idle(w);
            return;
        }
    }
    if (!(w->pressed & 3))
        return;
    if (w->act_jump == NULL)
        return;
    w->angle = 0;
    w->gimmick = NULL;
    w->move_flags |= 0xc0;
    w->move_flags &= ~0x4000u;
    w->pf &= ~0x100000u;
    w->act_jump(w);
}

/* =============================================================== RopeC */
void Player_State_RopeC(DcPlayer *w);
void Player_Action_RopeC(DcPlayer *w, Obj *parent, s32 len) {
    ObjPlayerCtx *c = ctx(w);
    obj_init_state(w);
    w->gimmick = parent;
    dcp_set_anim(w, 0x25);
    w->disp_flags |= 4;
    w->state = Player_State_RopeC;
    w->obj_flags |= 2;
    w->move_flags |= 0x10;
    w->move_flags |= 0x2000;
    w->move_flags |= 0x100;
    w->disp_flags &= ~1;
    w->disp_flags &= ~0x200;
    w->pf |= 0x100000;
    w->vel_x = 0;
    w->vel_y = 0;
    w->vel_z = 0;
    w->gnd_vel = 0x60;
    w->angle = 0;
    w->trick_input = 0xf000;
    c->g3F4 = 0;
    w->timer = len;
    w->pf |= 0x2000;
    w->pf2 |= 0x30;
    psfx(w, DCP_CH_VOICE, 0x42);
    gimmick_count("rope_c", w, len);
}
static void ropec_release(DcPlayer *w) {
    w->gimmick = NULL;
    w->obj_flags &= ~2u;
    w->move_flags &= ~0x2000u;
    w->move_flags &= ~0x100u;
    w->pf &= ~0x2000u;
    w->pf2 &= ~0x30u;
    w->pf &= ~0x100000u;
    w->disp_flags |= 0x200;
    w->rot3 = 0;
    w->rot2 = 0;
    w->angle = 0;
    if (w->act_jump)
        w->act_jump(w);
}
void Player_State_RopeC(DcPlayer *w) {
    ObjPlayerCtx *c = ctx(w);
    ObjMtx33 m, r;
    ObjVec v;
    Obj *o = (Obj *)w->gimmick;
    u8 prev;
    if (o == NULL || (w->pressed & 3)) {
        ropec_release(w);
        return;
    }
    prev = w->rot3;
    w->gnd_vel = (s16)dcp_spd_up(w->gnd_vel, 6, 0x280);
    c->g3F4 -= w->gnd_vel * 6;
    w->rot3 = (u8)((c->g3F4 & 0xffff) >> 8);
    if (w->rot3 > prev)
        psfx(w, DCP_CH_VOICE, 0x4f);
    if (w->rot3 < 0x80 && prev >= 0x80)
        psfx(w, DCP_CH_VOICE, 0x4f);
    w->timer -= w->gnd_vel;
    if ((u32)w->trick_input != 0 && (u32)w->trick_input < 0xffff) {
        w->trick_input += 0x40;
        if ((u32)w->trick_input > 0xffff)
            w->trick_input = 0;
    } else {
        w->trick_input = 0;
    }
    w->px = w->x;
    w->py = w->y;
    w->pz = w->z;
    MTX_Identity33_(&m);
    MTX_RotZ33_(&r, obj_sin((u16)w->trick_input), obj_cos((u16)w->trick_input));
    MTX_Concat33(&m, &r, &m);
    MTX_RotY33_(&r, obj_sin((u16)-(w->rot3 << 8)), obj_cos((u16)-(w->rot3 << 8)));
    MTX_Concat33(&m, &r, &m);
    v.x = w->timer << 4;
    v.y = 0;
    v.z = 0;
    MTX_MultVec33(&v, &m, &v);
    w->x = o->h.x - (v.x >> 4);
    w->y = o->h.y - (v.y >> 4);
    w->z = o->h.z - (v.z >> 4);
    w->move_x = (s16)(w->x - w->px);
    w->move_y = (s16)(w->y - w->py);
    w->move_z = (s16)(w->z - w->pz);
    if (w->timer < 0x600) {
        w->gnd_vel = 0;
        ropec_release(w);
    }
}

/* ========================================================== Bend stick */
static void Player_Action_BendStickLaunch(DcPlayer *w) {
    ObjPlayerCtx *c = ctx(w);
    dcp_set_anim(w, 0x30);
    w->disp_flags |= 4;
    w->move_flags |= 0x10;
    w->move_flags |= 0x8000;
    w->state = dcp_state_in_air;
    w->vel_x = (s16)(c->g3F4 >> 0x11);
    w->vel_y = (s16)c->g3F4;
    patkdef(w, 1, 0x13, 0);
    w->pf &= ~0xfu;
    w->pf |= 1;
    w->pf |= 2;
    w->timer = 0;
    w->trick_input = 0;
    w->charge = 0;
    w->trick_cnt = 0;
    psfx(w, DCP_CH_VOICE, 0x4e);
}
static void Player_State_BendStick(DcPlayer *s) {
    if (s->timer != 0) {
        s->timer--;
        return;
    }
    Player_Action_BendStickLaunch(s);
}
void Player_Action_BendStick(DcPlayer *w, s32 a, s32 b) {
    ObjPlayerCtx *c = ctx(w);
    dcp_reset_gimmick_state(w);
    dcp_set_anim(w, 0x31);
    w->disp_flags |= 4;
    w->move_flags &= ~0x10u;
    w->move_flags |= 0x8000;
    w->state = Player_State_BendStick;
    c->g3F4 = (w->vel_x << 16) | (a & 0xffff);
    w->timer = b;
    w->vel_x = 0;
    w->vel_y = 0;
    w->angle = 0;
    patkdef(w, 1, 0x13, 0);
    if (c != &s_dummy) {
        c->atk.cb = NULL;      /* w->unk2E8 = 0 (attack box callback) */
        c->atk.flags &= ~8;    /* w->unk2D6 &= ~8 */
    }
    gimmick_count("bend_stick", w, a);
}

/* =========================================================== Corkscrew */
static void Player_MoveAlongCorkscrew(DcPlayer *w, s32 len, s32 rx, s16 ry) {
    ObjPlayerCtx *c = ctx(w);
    s32 t, n;
    s8 q;
    s16 cs;
    w->timer += ABS(w->gnd_vel);
    t = w->timer;
    q = (s8)(t / len);
    n = ((t - len * q) << 8) / len;
    w->rot2 = (u8)n;
    if ((u32)w->trick_input & 1)
        w->rot2 = (u8)-w->rot2;
    if (w->rot2 < 0x40)
        w->angle = w->rot2;
    else if (w->rot2 < 0x80)
        w->angle = (u8)(0x80 - w->rot2);
    else if (w->rot2 < 0xc0)
        w->angle = (u8)(w->rot2 - 0x80);
    else
        w->angle = (u8)(0x100 - w->rot2);
    w->angle = w->angle >> 1;
    if (w->rot2 >= 0x80)
        w->angle = (u8)-w->angle;
    w->angle = (u8)-w->angle;
    if (c->g3F8 == 2)
        w->rot2 = (u8)-w->rot2;
    cs = obj_cos((u16)(n << 8));
    ry = (s16)(ry - w->box_b);
    if ((u32)w->trick_input & 2)
        ry = (s16)-ry;
    w->px = w->x;
    w->py = w->y;
    if ((u32)w->trick_input & 1)
        w->x = c->g3F0 - ((q * rx) << 8) - rx * n;
    else
        w->x = c->g3F0 + ((q * rx) << 8) + rx * n;
    w->y = c->g3F4 + (ry << 8) - ((cs * ry) >> 4);
    w->move_x = (s16)(w->x - w->px);
    w->move_y = (s16)(w->y - w->py);
}
static void Player_MoveAlongCorkscrewPath(DcPlayer *w, s32 len, s16 ox, s16 oy, s32 dist, s16 sx, s16 sy) {
    ObjPlayerCtx *c = ctx(w);
    u32 f8 = (u32)w->trick_input;
    s32 t, quarter, half, n;
    s8 q;
    s16 cs;
    w->timer += ABS(w->gnd_vel);
    if (ABS(w->timer) >= dist) {
        if (!(w->pf & 1)) {
            w->pf |= 1;
            if (f8 & 1) {
                c->g3F0 += -((sx - ox) << 8);
                if (!(f8 & 2))
                    c->g3F0 -= w->box_b << 8;
            } else {
                c->g3F0 += (sx - ox) << 8;
            }
            if (!(f8 & 1))
                c->g3F0 += w->box_b << 8;
            if (f8 & 2)
                c->g3F4 += ((oy >> 2) - sy) << 8;
            else
                c->g3F4 -= ((oy >> 2) - sy) << 8;
        }
        quarter = len >> 2;
        t = w->timer - dist + quarter;
        half = len >> 1;
        q = (s8)FX_DivS32(quarter + ABS(t), half);
        if ((u32)(s32)q >= (f8 >> 2)) { /* s8 vs u32: unsigned compare, as in the original */
            if (!(w->pf & 2)) {
                w->pf |= 2;
                if (f8 & 1)
                    c->g3F0 -= (ox - w->box_b) << 8;
                else
                    c->g3F0 += (ox - w->box_b) << 8;
                if (f8 & 2)
                    c->g3F4 -= ((q - 1) * (oy >> 1) + (oy >> 2)) << 8;
                else
                    c->g3F4 += ((q - 1) * (oy >> 1) + (oy >> 2)) << 8;
            }
            n = FX_DivS32((quarter + (t - half * (s32)(f8 >> 2))) << 8, dist);
            w->rot2 = (u8)(((n << 6) >> 8) + 0xc0);
            if (f8 & 2)
                sy = (s16)-sy;
            if (f8 & 1)
                sx = (s16)-sx;
            if (!((f8 >> 2) & 1))
                sx = (s16)-sx;
            w->px = w->x;
            w->py = w->y;
            w->x = c->g3F0 + sx * n;
            w->y = c->g3F4 + sy * n;
            w->move_x = (s16)(w->x - w->px);
            w->move_y = (s16)(w->y - w->py);
            return;
        }
        q = (s8)FX_DivS32(t, len);
        n = FX_DivS32((t - len * q) << 8, len);
        w->rot3 = (u8)(n + 0xc0);
        w->rot2 = 0xc0;
        if (f8 & 1)
            w->rot3 = (u8)-w->rot3;
        cs = obj_cos((u16)(n << 8));
        ox = (s16)(ox - w->box_b);
        if (f8 & 2)
            oy = (s16)-oy;
        w->px = w->x;
        w->py = w->y;
        if (f8 & 1)
            w->x = c->g3F0 - (ox << 8) + ((cs * ox) >> 4);
        else
            w->x = c->g3F0 + (ox << 8) - ((cs * ox) >> 4);
        w->y = c->g3F4 + ((q * oy) << 8) + oy * n;
        w->move_x = (s16)(w->x - w->px);
        w->move_y = (s16)(w->y - w->py);
        return;
    }
    n = FX_DivS32(w->timer << 8, dist);
    w->rot2 = (u8)(0x100 - ((n << 6) >> 8));
    if ((f8 & 1) && (f8 & 2))
        sx = (s16)(sx - w->box_b);
    if (f8 & 2)
        sy = (s16)-sy;
    if (f8 & 1)
        sx = (s16)-sx;
    w->px = w->x;
    w->py = w->y;
    w->x = c->g3F0 + sx * n;
    w->y = c->g3F4 + sy * n;
    w->move_x = (s16)(w->x - w->px);
    w->move_y = (s16)(w->y - w->py);
}
static void Player_State_CorkscrewPath(DcPlayer *w) {
    ObjPlayerCtx *c = ctx(w);
    if (c->g3F8 != 2 && ABS(w->gnd_vel) < w->run_thr[1] && dcp_check_fall_off(w)) {
        w->angle = 0;
        w->rot2 = 0;
        w->rot3 = 0;
        w->move_flags &= ~0x2000u;
        dcp_act_fall(w);
        return;
    }
    if ((w->pressed & 3) && w->act_jump != NULL) {
        if (c->g3F8 == 2)
            w->obj_flags |= 1;
        w->angle = 0;
        w->rot2 = 0;
        w->rot3 = 0;
        w->move_flags &= ~0x2000u;
        w->act_jump(w);
        return;
    }
    if (w->charge != 0) {
        w->charge--;
    } else {
        if ((u32)w->trick_input & 2)
            w->move_flags &= ~0x10u;
        if (w->move_flags & 0xd) {
            w->angle = 0;
            w->rot2 = 0;
            w->rot3 = 0;
            w->move_flags &= ~0x2000u;
            w->vel_x = w->gnd_vel;
            dcp_act_land(w, 0);
            if (c->g3F8 == 2) {
                Player_Action_Grind(w);
                return;
            }
            if (w->act_idle)
                w->act_idle(w);
            return;
        }
    }
    if (c->g3F8 != 2) {
        anim_speed(w, w->gnd_vel);
        if (w->anim != 0x31)
            dcp_update_run_anim(w);
    }
    if (c->g3F8 != 2)
        dcp_apply_ground_input(w);
    switch (c->g3F8) {
    case 0:
    default:
        Player_MoveAlongCorkscrew(w, 0x1759d, 0x120, 0x26);
        break;
    case 1:
        Player_MoveAlongCorkscrewPath(w, 0x21e52, 0x54, 0x80, 0x83f0, 0x80, 0x20);
        break;
    case 2:
        Player_MoveAlongCorkscrew(w, 0x13d9d, 0x100, 0x1e);
        break;
    }
}
void Player_Action_CorkscrewPath(DcPlayer *w, s32 x, s32 y, s32 flags) {
    ObjPlayerCtx *c = ctx(w);
    u8 save = w->layer_ovr;
    s32 kind;
    if (w->state == Player_State_CorkscrewPath)
        return;
    w->layer_ovr = 0;
    dcp_reset_gimmick_state(w);
    kind = flags >> 12;
    w->layer_ovr = save;
    if (kind != 2 && w->anim != 0x31)
        dcp_set_run_anim_for_speed(w);
    w->disp_flags |= 4;
    w->move_flags |= 0x10;
    w->move_flags |= 0x2000;
    w->pf &= ~3u;
    c->g3F0 = x;
    c->g3F4 = y;
    c->g3F8 = kind;
    w->trick_input = flags & 0xfff;
    w->timer = 0;
    if (w->trick_input & 1) {
        if (c->g3F0 > w->x)
            w->timer = c->g3F0 - w->x;
    } else {
        if (c->g3F0 < w->x)
            w->timer = w->x - c->g3F0;
    }
    c->g3F4 -= w->box_b << 8;
    if (c->g3F8 == 1) {
        if (!(w->trick_input & 2))
            c->g3F4 -= 0x400;
    } else {
        w->trick_input |= 2;
    }
    w->state = Player_State_CorkscrewPath;
    w->charge = 0x10;
    gimmick_count("corkscrew", w, kind);
}

/* ======================================================== Special ring */
static void Player_State_GmkSpecialEnter(DcPlayer *w);
static void Player_Action_GmkSpecialEnter(DcPlayer *w) {
    ObjPlayerCtx *c = ctx(w);
    s32 v;
    w->state = Player_State_GmkSpecialEnter;
    v = w->gnd_vel;
    if (v < 0)
        v = -v;
    w->vel_y = (s16)-v;
    if (w->vel_y < -0xf00)
        w->vel_y = -0xf00;
    else if (w->vel_y > 0xf00)
        w->vel_y = 0xf00;
    w->pf2 &= ~0xc0u;
    w->pf2 &= ~8u;
    w->gnd_vel = 0;
    c->g3F0 = w->x;
    w->move_flags &= ~0x2000u;
    c->g3EE = 0;
    w->timer = 0x40;
    w->unk3CA = 0;
    psfx(w, DCP_CH_VOICE, 0x40);
}
static void Player_State_GmkSpecialEnter(DcPlayer *w) {
    ObjPlayerCtx *c = ctx(w);
    Obj *o = (Obj *)w->gimmick;
    s32 t = 0x50, py, p2;
    if (o == NULL)
        return;
    w->vel_x = (s16)(ObjShift_SetFromRef(w->x, o->h.x, c->g3F0, 1, 0xf00, 0x400) - w->x);
    w->angle = dcp_angle_toward8(w->angle, 0xc0, 0x10);
    if (w->grav_rot != 0) {
        t = -0x50;
        w->vel_x = (s16)-w->vel_x;
    }
    w->pf2 &= ~0xc0u;
    c->g3EE = (s16)ObjShift_SetFromRef(c->g3EE, t, 0, 4, 0xf00, 0x80);
    if (!(W.frame & 1)) {
        u16 r1 = RAND(); /* MWCC evaluates call arguments right to left */
        u16 r2 = RAND();
        pfx(w, OBJ_FX_TRICK_AT, 0xf - (r2 & 0x1e), 0xf - (r1 & 0x1e));
    }
    py = o->h.y;
    if (((w->y < py - 0x3000 && w->grav_rot == 0) || (w->y > py + 0x3000 && w->grav_rot != 0)) &&
        !(w->disp_flags & 0x20)) {
        dcp_stop_highspeed(w);
        dcp_stop_boost(w);
        w->disp_flags |= 0x20;
        w->pf |= 0x200000;
    }
    p2 = o->h.y;
    if ((w->y < p2 - 0xe800 && w->grav_rot == 0) || (w->y > p2 + 0xe800 && w->grav_rot != 0))
        w->vel_y = 0;
    if (w->timer == 0) {
        if (!(w->pf & 0x40000000)) {
            /* Player_Last starts the special stage (not in the player module) */
            if (g_obj_hooks.stage_exit)
                g_obj_hooks.stage_exit(g_obj_hooks.ctx, OBJ_EXIT_SPECIAL_STAGE);
            obj_log("special_stage", w->rings, w->x >> 8);
        }
        w->pf |= 0x40000000;
        return;
    }
    w->timer--;
    if (w->timer == 0x10 && g_obj_hooks.fade)
        g_obj_hooks.fade(g_obj_hooks.ctx, 5, 0x100);
}
static void Player_State_GmkSpecial(DcPlayer *w) {
    ObjPlayerCtx *c = ctx(w);
    Obj *o = (Obj *)w->gimmick;
    s32 a, r;
    if (o != NULL) {
        w->disp_flags &= ~1;
        if (w->pf & 0x80) {
            c->g3F4++;
            if (w->timer == 0) {
                psfx_stop(w, DCP_CH_BOOST);
                psfx(w, DCP_CH_BOOST, 0x47);
                w->timer++;
                pfx(w, OBJ_FX_SPECIAL, 0, 0);
            }
            if (c->g3F4 > 0x60) {
                if (!(W.frame & 3)) {
                    u16 r1 = RAND(); /* arguments right to left */
                    u16 r2 = RAND();
                    pfx(w, OBJ_FX_TRICK_AT, 0xf - (r2 & 0x1e), 0xf - (r1 & 0x1e));
                }
                if (w->timer == 1) {
                    psfx_stop(w, DCP_CH_BOOST);
                    psfx(w, DCP_CH_BOOST, 0x48);
                    w->timer++;
                }
            }
            if (c->g3F4 > 0xc0) {
                if (w->timer == 2) {
                    psfx_stop(w, DCP_CH_BOOST);
                    psfx(w, DCP_CH_BOOST, 0x49);
                    w->timer++;
                }
                Player_Action_GmkSpecialEnter(w);
                return;
            }
            if (w->gnd_vel > 0)
                w->gnd_vel = (s16)dcp_spd_up(w->gnd_vel, w->boost_acc, w->boost_top << 1);
            else
                w->gnd_vel = (s16)dcp_spd_up(w->gnd_vel, -w->boost_acc, w->boost_top << 1);
        } else {
            w->timer = 0;
            c->g3F4 = 0;
            if (c->g3F8 < 0x4000)
                c->g3F8 += 0x10000;
            if (c->g3F8 > 0x14000)
                c->g3F8 -= 0x10000;
            if (c->g3F8 < 0xc000) {
                if (w->gnd_vel > 0)
                    w->gnd_vel = (s16)dcp_spd_down(w->gnd_vel, 0x20);
                w->gnd_vel = (s16)dcp_spd_up(w->gnd_vel, -0x20, 0xf00);
            } else {
                if (w->gnd_vel < 0)
                    w->gnd_vel = (s16)dcp_spd_down(w->gnd_vel, 0x20);
                w->gnd_vel = (s16)dcp_spd_up(w->gnd_vel, 0x20, 0xf00);
            }
            a = 0xc000 - c->g3F8;
            if (a < 0)
                a = -a;
            if (a < 0x100) {
                a = w->gnd_vel;
                if (a < 0)
                    a = -a;
                if (a < 0x80) {
                    c->g3F8 = 0xc000;
                    w->gnd_vel = 0;
                }
            }
        }
        c->g3F8 -= w->gnd_vel;
        w->angle = (u8)(c->g3F8 >> 8);
        w->px = w->x;
        w->py = w->y;
        a = (u16)((w->angle + w->grav_rot) << 8);
        r = c->g3F0;
        w->x = o->h.x + (s16)((r * obj_cos((u16)a)) >> 12);
        w->y = o->h.y + (s16)((r * obj_sin((u16)a)) >> 12);
        w->move_x = (s16)(w->x - w->px);
        w->move_y = (s16)(w->y - w->py);
    }
    if (w->gimmick != NULL && !(w->pressed & 3))
        return;
    if (w->act_jump == NULL)
        return;
    if (w->act_idle == NULL)
        return;
    psfx_stop(w, DCP_CH_BOOST);
    w->gnd_vel = 0;
    w->gimmick = NULL;
    w->move_flags |= 0xc0;
    w->move_flags &= ~0x2000u;
    w->move_flags &= ~0x4000u;
    w->move_flags &= ~0x100u;
    w->obj_flags &= ~2u;
    w->pf &= ~0x2000u;
    w->pf2 &= ~8u;
    w->angle = 0;
    if (w->pressed & 3) {
        w->act_jump(w);
        return;
    }
    dcp_act_land(w, 0);
    w->act_idle(w);
}
void Player_Action_GmkSpecial(DcPlayer *w, Obj *o) {
    ObjPlayerCtx *c = ctx(w);
    dcp_reset_gimmick_state(w);
    dcp_player_load_physics(w);
    w->gimmick = o;
    dcp_set_anim(w, 0x25);
    w->disp_flags |= 4;
    w->state = Player_State_GmkSpecial;
    w->disp_flags &= ~1;
    w->move_flags &= ~0x10u;
    w->move_flags |= 0x8000;
    w->move_flags |= 0x2000;
    w->move_flags |= 0x100;
    w->move_flags &= ~0x80u;
    w->move_flags &= ~0x40u;
    w->obj_flags |= 2;
    w->pf |= 0x2000;
    w->pf2 |= 0x30;
    w->pf2 |= 8;
    w->gnd_vel = (s16)(w->vel_x + w->vel_y);
    w->vel_x = 0;
    w->vel_y = 0;
    c->g3F8 = dcp_atan2(o->h.y - w->y, o->h.x - w->x);
    c->g3F8 += w->grav_rot << 8;
    if (c->g3F8 < 0x4000)
        c->g3F8 += 0x10000;
    w->angle = (u8)(c->g3F8 >> 8);
    w->trick_input = 0;
    w->timer = 0;
    c->g3F0 = -0x2000;
    c->g3F4 = 0;
    c->g3EE = 0;
    psfx(w, DCP_CH_VOICE, 0x6d);
    gimmick_count("special_ring", w, 0);
}

/* ===================================================== Jump pedestal */
void Player_Action_PedestalLaunch(DcPlayer *w, s16 x, s16 y) {
    dcp_rotate_vec(&x, &y, w->grav_rot);
    Player_Action_GimmickLaunch(w, x, y);
    psfx(w, DCP_CH_VOICE, 0x3F);
    gimmick_count("pedestal", w, x);
}

/* ===================================================== Grind rails */
static void Player_Action_GrindTrick(DcPlayer *w) {
    s16 v;
    w->trick_lvl = 0;
    if (w->anim == 0x1c) {
        dcp_set_anim(w, 0x1d);
        dcp_add_tension(w, 0xa0);
        if (w->chara != 1) {
            psfx(w, DCP_CH_VOICE, (RAND() & 1) ? 0x1e : 0x25);
            psfx(w, DCP_CH_VOICE, 0xc);
        } else {
            psfx(w, DCP_CH_VOICE, (RAND() & 1) ? 0x27 : 0x2e);
            psfx(w, DCP_CH_VOICE, 0x19);
        }
        psfx(w, DCP_CH_VOICE, 0x30);
    } else if (w->anim == 0x1d) {
        dcp_set_anim(w, 0x1e);
        dcp_add_tension(w, 0x140);
        if (w->chara != 1) {
            psfx(w, DCP_CH_VOICE, (RAND() & 1) ? 0x1f : 0x20);
            psfx(w, DCP_CH_VOICE, 0xd);
        } else {
            psfx(w, DCP_CH_VOICE, (RAND() & 1) ? 0x28 : 0x29);
            psfx(w, DCP_CH_VOICE, 0x1a);
        }
        psfx_stop(w, DCP_CH_TRICK);
        psfx(w, DCP_CH_TRICK, 0x31);
        if (w->disp_flags & 1)
            w->gnd_vel = (s16)(w->gnd_vel - 0x200);
        else
            w->gnd_vel = (s16)(w->gnd_vel + 0x200);
        w->trick_input = 0;
        dcp_add_trick_score(w, 200);
    } else {
        dcp_set_anim(w, 0x1c);
        dcp_add_tension(w, 0xa0);
        if (w->chara != 1) {
            psfx(w, DCP_CH_VOICE, 0x1d);
            psfx(w, DCP_CH_VOICE, 0xc);
        } else {
            psfx(w, DCP_CH_VOICE, 0x26);
            psfx(w, DCP_CH_VOICE, 0x19);
        }
        psfx(w, DCP_CH_VOICE, 0x2f);
    }
    w->pf |= 0x8000;
    v = w->tension;
    if (v < 0)
        v = 0;
    else if (v > 0x12c0)
        v = 0x12c0;
    w->tension = v;
    pfx(w, OBJ_FX_TRICK_SPARKLE, w->trick_lvl, 0);
    dcp_add_trick_score(w, 100);
    w->timer = 0;
    w->charge = 0;
}
static void Player_State_Grind(DcPlayer *w) {
    u8 ang = w->angle;
    s32 d;
    if (w->move_flags & 4)
        w->disp_flags ^= 1;
    if (w->gnd_vel == 0 && (w->angle == 0 || w->angle == 0x80)) {
        w->gnd_vel = 0x100;
        if (w->disp_flags & 1)
            w->gnd_vel = (s16)-w->gnd_vel;
    }
    if (!(w->move_flags & 1)) {
        psfx_stop(w, DCP_CH_BOOST);
        psfx_stop(w, DCP_CH_TRICK);
        w->obj_flags |= 1;
        w->layer_ovr = 0;
        Player_Action_GimmickLaunch(w, w->gnd_vel, -0x500);
        psfx(w, DCP_CH_VOICE, w->chara != 1 ? 2 : 0x13);
        return;
    }
    w->angle = 0;
    if (dcp_check_fall_off(w)) {
        psfx_stop(w, DCP_CH_BOOST);
        psfx_stop(w, DCP_CH_TRICK);
        w->obj_flags |= 1;
        w->layer_ovr = 0;
        dcp_act_fall(w);
        return;
    }
    w->angle = ang;
    if (w->pressed & 3) {
        psfx_stop(w, DCP_CH_BOOST);
        psfx_stop(w, DCP_CH_TRICK);
        if (w->hit & 0x10) {
            w->obj_flags |= 1;
            w->layer_ovr = 0;
            Player_Action_GimmickLaunch(w, w->gnd_vel, -0x700);
            obj_player_start_launch_trick(w);
            return;
        }
        if (w->act_jump != NULL) {
            w->obj_flags |= 1;
            w->act_jump(w);
            return;
        }
    }
    if (!(w->hit & 8) && w->act_idle != NULL) {
        psfx_stop(w, DCP_CH_BOOST);
        psfx_stop(w, DCP_CH_TRICK);
        w->obj_flags |= 1;
        w->layer_ovr = 0;
        w->act_idle(w);
        return;
    }
    d = ((obj_sin((u16)(w->angle << 8)) * 0x28) << 4) >> 16;
    if ((w->disp_flags & 1) && d > 0) {
        d = (s16)(d >> 2);
        if (w->dash_timer != 0)
            d = 0;
    }
    if (!(w->disp_flags & 1) && d < 0) {
        d = (s16)(d >> 2);
        if (w->dash_timer != 0)
            d = 0;
    }
    w->gnd_vel = (s16)(w->gnd_vel + d);
    if (w->gnd_vel > 0)
        w->disp_flags &= ~1;
    else if (w->gnd_vel < 0)
        w->disp_flags |= 1;
    if (w->held & 0x20) {
        if (w->gnd_vel < 0 || w->gnd_vel > 0x200)
            w->gnd_vel = (s16)(w->gnd_vel - 0x10);
    } else if (w->held & 0x10) {
        if (w->gnd_vel > 0 || w->gnd_vel < -0x200)
            w->gnd_vel = (s16)(w->gnd_vel + 0x10);
    }
    switch (w->anim) {
    case 0x1e:
        if (w->disp_flags & 8) {
            dcp_set_anim(w, 0x1f);
            w->disp_flags |= 4;
        }
        break;
    case 0x1f:
        w->trick_input++;
        if (w->trick_input >= 0x20 && (w->disp_flags & 8)) {
            dcp_set_anim(w, 0x20);
            w->trick_input = 0;
        }
        break;
    case 0x20:
        if (w->disp_flags & 8) {
            psfx_stop(w, DCP_CH_TRICK);
            dcp_set_anim(w, 0x1a);
            w->disp_flags |= 4;
        }
        break;
    }
    if (w->anim >= 0x1e)
        return;
    if (w->anim >= 0x1c) {
        w->charge++;
        if (w->charge > 0xff)
            w->charge = 0xff;
    }
    if (w->pressed & 0x100)
        w->timer = 1;
    if (w->anim != 0x1a && (w->anim != 0x1c || w->charge <= 0x18)) {
        if (w->anim != 0x1d)
            return;
        if (w->charge <= 0x10)
            return;
    }
    if (w->timer == 0)
        return;
    Player_Action_GrindTrick(w);
}
void Player_Action_Grind(DcPlayer *w) {
    s32 a, b;
    s16 v;
    w->layer_ovr &= ~0x80;
    dcp_reset_gimmick_state(w);
    dcp_set_anim(w, 0x1A);
    w->disp_flags |= 4;
    w->move_flags &= ~0x10u;
    w->pf &= ~0xC000u;
    w->layer_ovr |= 0x80;
    w->layer_set = 0;
    w->state = Player_State_Grind;
    if (w->move_flags & 1) {
        a = w->gnd_vel;
        b = w->vel_x;
        a = a < 0 ? -a : a;
        b = b < 0 ? -b : b;
        if (b < a)
            w->vel_x = w->gnd_vel;
    }
    dcp_act_land(w, 0);
    v = w->gnd_vel;
    if (v > -0x200 && v < 0x200) {
        if (v < 0)
            w->gnd_vel = -0x200;
        else if (v > 0)
            w->gnd_vel = 0x200;
    }
    psfx_stop(w, DCP_CH_BOOST);
    psfx(w, DCP_CH_BOOST, 0x3E);
    w->timer = 0;
    w->trick_input = 0;
    w->trick_cnt = 0;
    pfx(w, OBJ_FX_GRIND, 0, 0x12);
    g_obj.stats.grinds++;
    obj_log("grind", w->x >> 8, w->y >> 8);
}
/* Player_CheckStartGrind (Player_In) */
dcp_bool Player_CheckStartGrind(DcPlayer *w) {
    if ((w->move_flags & 1) && (w->hit & 8)) {
        if (w->anim < 0x1a || w->anim > 0x20)
            Player_Action_Grind(w);
        return DCP_TRUE;
    }
    return DCP_FALSE;
}

/* ======================================== Player_FindHomingTarget (Player_In) */
void Player_FindHomingTarget(DcPlayer *w) {
    ObjBox *t;
    Obj *o, *best = NULL;
    s32 dx, dy, d, bestd = 0x7fffffff;
    u8 i;
    if (w->homing != NULL && (w->homing->obj_flags & 4))
        w->homing = NULL;
    if (w->pf & 0x40)
        return;
    if (W.zone >= 9)
        return;
    if (W.act >= 2)
        return;
    i = 0;
    for (;;) {
        t = (ObjBox *)rect_regist_get(2, i);
        if (t == NULL)
            break;
        if (!(t->flags & 0x1000) && t->owner_kind == OBJ_OWNER_OBJECT && (o = (Obj *)t->owner) != NULL &&
            o->h.type == 2) {
            dy = (o->h.y - w->y) >> 8;
            dx = (o->h.x - w->x) >> 8;
            d = dx * dx + dy * dy;
            if (bestd > d) {
                bestd = d;
                best = o;
            }
        }
        i++;
    }
    w->homing = NULL;
    if (best != NULL) {
        w->homing = &best->ref;
        if (w->state == dcp_state_homing)
            g_obj.stats.homing_targets++;
    }
}

/* Player_GiveMagnet (item box kind 7); Magnet_Create (the model) -> item hook */
void Player_GiveMagnet(DcPlayer *w) {
    if (!(w->pf & 0x20000000)) {
        obj_sfx(0x35);
        if (g_obj_hooks.item)
            g_obj_hooks.item(g_obj_hooks.ctx, w, OBJ_ITEM_MAGNET);
    }
    w->pf |= 0x30000000;
}
