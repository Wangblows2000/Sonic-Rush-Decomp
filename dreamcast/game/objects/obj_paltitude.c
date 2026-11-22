/*
 * Player gimmick states of Altitude Limit, copied from src/game/Player/ and
 * adapted like obj_pwater.c / obj_pmirage.c:
 *   Player_Action_Glider, Player_State_Glider, Player_Action_ResumeGlider,
 *   Player_ClampToGliderArea, Player_Action_GliderSequenceStart,
 *   Player_State_GliderSequenceIntro, Player_State_GliderSequence,
 *   Player_UpdateGliderSequence, Player_Action_GliderSequenceEnd,
 *   Player_State_GliderSequenceExit, Player_OnHit_GliderSequence,
 *   Player_Action_Parachute, Player_State_Parachute,
 *   Player_Action_ParachuteRelease, Player_State_ParachuteRelease,
 *   Player_Action_Saucer, Player_State_Saucer.
 * gCamera[n] @0x00 / @0x34 bit 0 -> g_obj_camera[n].flags / flags34;
 * data_022c4ef8 (wind) -> g_dcp_world.wind_x; BrightnessFade_Create ->
 * g_obj_hooks.fade; the body box callback @0x2BC -> ObjPlayerCtx.body.cb.
 */
#include "obj_zone.h"

static void fade(s32 kind, s32 speed) {
    if (g_obj_hooks.fade)
        g_obj_hooks.fade(g_obj_hooks.ctx, kind, speed);
}

/* ------------------------------------------------------------ Glider */
void Player_Action_Glider(DcPlayer *p0, Obj *p1, s32 p2, s32 p3) {
    if (p0->gimmick == (void *)p1)
        return;
    dcp_reset_gimmick_state(p0);
    p0->gimmick = p1;
    dcp_set_anim(p0, 0x2e);
    p0->disp_flags |= 4;
    p0->state = Player_State_Glider;
    p0->move_flags |= 0x10;
    p0->move_flags |= 0x8000;
    p0->move_flags &= ~0x80u;
    p0->move_flags &= ~0x40u;
    dcp_stop_highspeed(p0);
    dcp_stop_boost(p0);
    p0->pf |= 0x100000;
    p0->pf &= ~0xfu;
    p0->disp_flags &= ~1;
    p0->disp_flags &= ~0x200;
    p0->x = p1->h.x;
    p0->vel_x = 0x800;
    p0->vel_y = 0;
    p0->gnd_vel = 0;
    p0->angle = 0;
    p0->trick_input = 0;
    p0->timer = 0;
    p0->px = p0->x + (p2 << 8);
    p0->py = p0->y + (p3 << 8);
    z_psfx(p0, DCP_CH_VOICE, 0x6d);
}
void Player_State_Glider(DcPlayer *w) {
    w->timer++;
    if (w->timer & 0x10)
        w->vel_y = (s16)dcp_spd_up(w->vel_y, -0x10, 0x80);
    else
        w->vel_y = (s16)dcp_spd_up(w->vel_y, 0x10, 0x80);
    if ((!(w->move_flags & 1) || w->timer <= 0xc) && w->timer <= 0x168)
        return;
    if (w->act_jump == NULL)
        return;
    if (w->act_idle == NULL)
        return;
    w->gimmick = NULL;
    w->disp_flags |= 0x200;
    w->move_flags |= 0xc0;
    w->move_flags &= ~0x2000u;
    w->move_flags &= ~0x4000u;
    w->pf &= ~0x100000u;
    w->angle = 0;
    dcp_act_land(w, 0);
    w->act_idle(w);
}
void Player_Action_ResumeGlider(DcPlayer *p0) {
    ObjPlayerCtx *c = z_ctx(p0);
    p0->state = Player_State_Glider;
    p0->move_flags |= 0x10;
    p0->move_flags |= 0x8000;
    p0->move_flags &= ~0x80u;
    p0->move_flags &= ~0x40u;
    p0->move_flags &= ~0x2000u;
    p0->pf |= 0x100000;
    p0->pf &= ~0xfu;
    p0->vel_x = 0x300;
    p0->vel_y = 0;
    p0->gnd_vel = 0;
    p0->angle = 0;
    p0->trick_input = 0;
    p0->timer = 0x10;
    p0->pf &= ~0x2000u;
    c->g3D2 = 0;
    c->g3D4 = 0;
}
void Player_ClampToGliderArea(DcPlayer *w) {
    Obj *o = (Obj *)w->gimmick;
    if (o == NULL)
        return;
    if (w->x + (w->box_l << 8) < o->h.x - 0x7000) {
        w->x = o->h.x - 0x7000 - (w->box_l << 8);
        if (w->vel_x < 0)
            w->vel_x = 0;
    }
    if (w->x + (w->box_r << 8) > o->h.x + 0x7000) {
        w->x = o->h.x + 0x7000 - (w->box_r << 8);
        if (w->vel_x > 0)
            w->vel_x = 0;
    }
    if (w->y + (w->box_b << 8) > o->h.y + 0x5000) {
        w->y = o->h.y + 0x5000 - (w->box_b << 8);
        if (w->vel_y > 0)
            w->vel_y = 0;
    }
    if (w->y + (w->box_t << 8) < o->h.y - 0x5000) {
        w->y = o->h.y - 0x5000 - (w->box_t << 8);
        if (w->vel_y < 0)
            w->vel_y = 0;
    }
}

/* -------------------------------------------- glider sequence (0xb6 .. 0xb7) */
void Player_OnHit_GliderSequence(ObjBox *a, ObjBox *b) {
    DcPlayer *t0 = (DcPlayer *)a->owner;
    ObjPlayerCtx *c = z_ctx(t0);
    if (t0 == NULL || (t0->hitstop & 0xf) != 0)
        return;
    if (!(t0->pf & 0x10000000)) {
        dcp_add_tension(t0, -0x4b0);
        if (g_dcp_hooks.tension_hud)
            g_dcp_hooks.tension_hud(g_dcp_hooks.ctx, t0, (u32)(t0->tension << 12) >> 16);
    }
    if (b->atk & 0x20) {
        if (t0->rings != 0) {
            z_psfx(t0, DCP_CH_VOICE, 5);
            dcp_scatter_rings(t0, ring_scatter_level());
        }
        dcp_act_die(t0);
        return;
    }
    if (!(t0->pf & 0x10000000)) {
        if (t0->rings == 0) {
            dcp_act_die(t0);
            return;
        }
    }
    if (!(t0->pf & 0x10000000)) {
        z_psfx(t0, DCP_CH_VOICE, 5);
        dcp_scatter_rings(t0, ring_scatter_level());
    }
    t0->pf &= ~0x30000000u;
    obj_screen_shake(3);
    t0->invuln = t0->invuln_len;
    c->body.def |= 5;
    t0->vel_x = 0;
    t0->vel_y = 0;
    t0->gnd_vel = 0;
    if (t0->chara != 1)
        z_psfx(t0, DCP_CH_VOICE, 0x21);
    else
        z_psfx(t0, DCP_CH_VOICE, 0x2a);
    t0->hitstop = 0x80;
    g_obj.stats.hurts++;
}
void Player_UpdateGliderSequence(DcPlayer *w) {
    g_obj_camera[w->cam & 1].flags &= ~1u;
    g_obj_camera[w->cam & 1].flags &= ~2u;
    if (w->held & 0x30) {
        if (w->held & 0x10) {
            if (w->vel_x < 0)
                w->vel_x = (s16)dcp_spd_down(w->vel_x, 0x48);
            if (!(w->move_flags & 4))
                w->vel_x = (s16)dcp_spd_up(w->vel_x, 0x28, 0x300);
        } else {
            if (w->vel_x > 0)
                w->vel_x = (s16)dcp_spd_down(w->vel_x, 0x48);
            if (!(w->move_flags & 8))
                w->vel_x = (s16)dcp_spd_up(w->vel_x, -0x28, 0x300);
        }
    } else {
        w->vel_x = (s16)dcp_spd_down(w->vel_x, 0x48);
    }
    if (w->held & 0xc0) {
        if (w->held & 0x80) {
            if (w->vel_y < 0)
                w->vel_y = (s16)dcp_spd_down(w->vel_y, 0x48);
            if (!(w->move_flags & 1))
                w->vel_y = (s16)dcp_spd_up(w->vel_y, 0x28, 0x300);
        } else {
            if (w->vel_y > 0)
                w->vel_y = (s16)dcp_spd_down(w->vel_y, 0x48);
            if (!(w->move_flags & 2))
                w->vel_y = (s16)dcp_spd_up(w->vel_y, -0x28, 0x300);
        }
    } else {
        w->vel_y = (s16)dcp_spd_down(w->vel_y, 0x48);
    }
    Player_ClampToGliderArea(w);
}
void Player_State_GliderSequence(DcPlayer *w) {
    ObjPlayerCtx *c = z_ctx(w);
    if (c->body.flags & 0x100) {
        w->timer++;
        if (w->timer > 0x10)
            c->body.flags &= ~0x100;
        else
            return;
    }
    Player_UpdateGliderSequence(w);
}
void Player_State_GliderSequenceIntro(DcPlayer *w) {
    w->pf2 |= 0xc0;
    if (w->gimmick != NULL)
        ((Obj *)w->gimmick)->h.x = w->x + 0x100;
    w->timer++;
    if (w->timer > 0x12) {
        w->timer = 0;
        W.wind_x = 0x100;
        w->vel_x = 0;
        fade(3, 0x20);
        w->state = Player_State_GliderSequence;
        dcp_set_anim(w, 0x2f);
    }
}
void Player_Action_GliderSequenceStart(DcPlayer *w, Obj *o) {
    ObjPlayerCtx *c = z_ctx(w);
    if (w->state != Player_State_Glider)
        return;
    w->move_flags |= 0x100;
    w->gimmick = o;
    fade(0x43, 0);
    fade(0x45, 0x2000);
    w->timer = 0;
    w->vel_x = 0x1000;
    w->vel_y = 0;
    g_obj_camera[0].flags34 |= 1;
    g_obj_camera[1].flags34 |= 1;
    w->pf2 |= 0x200;
    w->pf2 |= 0x400;
    w->pf2 |= 0x30;
    w->pf2 |= 0xc0;
    w->pf |= 0x2000;
    w->rot3 = 0xc0;
    w->state = Player_State_GliderSequenceIntro;
    c->body.cb = Player_OnHit_GliderSequence; /* coll2A0.unk1C (never restored, as on the DS) */
    z_count("glider_sequence", o, w);
}
void Player_State_GliderSequenceExit(DcPlayer *s) {
    ObjPlayerCtx *c = z_ctx(s);
    Obj *g = (Obj *)s->gimmick;
    s->px = s->x;
    s->py = s->y;
    s->pz = s->z;
    if (g == NULL && s->trick_input < 3)
        return; /* (the trigger is gone: the DS reads the freed task) */
    switch (s->trick_input) {
    case 0:
        s->x = ObjShift_SetFromRef(s->x, g->h.x, c->g3F0, 4, 0x800, 0x80);
        s->y = ObjShift_SetFromRef(s->y, g->h.y, c->g3F4, 4, 0x800, 0x80);
        s->z = ObjShift_SetFromRef(s->z, -0x6000, c->g3F8, 4, 0x800, 0x80);
        if (s->x != g->h.x)
            return;
        if (s->y != g->h.y)
            return;
        if (s->z != -0x6000)
            return;
        s->trick_input = 1;
        c->g3F0 = s->x;
        c->g3F4 = s->y;
        c->g3F8 = s->z;
        c->g3EC = -0x80;
        s->pf2 &= ~0x40u;
        g_obj_camera[0].flags34 &= ~1u;
        g_obj_camera[1].flags34 &= ~1u;
        break;
    case 1:
        s->x = ObjShift_SetFromRef(s->x, c->g3F0 + 0x5000, c->g3F0, 4, 0x200, 0x80);
        s->z = ObjShift_SetFromRef(s->z, 0x6000, c->g3F8, 2, 0x800, 0x80);
        if (s->rot3 != 0)
            s->rot3++;
        if (s->x != c->g3F0 + 0x5000)
            return;
        if (s->z != 0x6000)
            return;
        s->trick_input = 2;
        c->g3F0 = s->x;
        c->g3F4 = s->y;
        c->g3F8 = s->z;
        s->rot2--;
        break;
    case 2:
        s->x = ObjShift_SetFromRef(s->x, c->g3F0 + 0xe600, c->g3F0, 4, 0x800, 0x200);
        if (s->rot2 > 0xc0)
            s->rot2--;
        if (s->x != c->g3F0 + 0xe600)
            return;
        c->g3F0 = s->x;
        s->trick_input = 3;
        s->rot2 = 0;
        break;
    case 3:
        s->x = ObjShift_SetFromRef(s->x, c->g3F0 - 0xb600, c->g3F0, 4, 0x300, 0x80);
        s->z = ObjShift_SetFromRef(s->z, 0, c->g3F8, 4, 0x200, 0x80);
        if (s->x != c->g3F0 - 0xb600)
            return;
        s->gimmick = NULL;
        Player_Action_ResumeGlider(s);
        break;
    }
}
void Player_Action_GliderSequenceEnd(DcPlayer *p0, Obj *o) {
    ObjPlayerCtx *c = z_ctx(p0);
    (void)o;
    if (p0->state != Player_State_GliderSequence)
        return;
    p0->vel_x = 0;
    p0->vel_y = 0;
    p0->gnd_vel = 0;
    p0->move_x = 0;
    p0->move_y = 0;
    p0->move_flags &= ~0x100u;
    W.wind_x = 0;
    p0->rot3 = 0xc0;
    p0->state = Player_State_GliderSequenceExit;
    p0->timer = 0;
    p0->trick_input = 0;
    c->g3F0 = p0->x;
    c->g3F4 = p0->y;
    c->g3F8 = p0->z;
    p0->move_flags |= 0x2000;
    p0->disp_flags &= ~0x200;
    p0->pf2 &= ~0x200u;
    p0->pf2 &= ~0x400u;
    g_obj_camera[p0->cam & 1].flags |= 1;
    g_obj_camera[p0->cam & 1].flags |= 2;
    z_count("glider_sequence_end", o, p0);
}

/* --------------------------------------------------------- Parachute */
void Player_Action_ParachuteRelease(DcPlayer *w) {
    ObjPlayerCtx *c = z_ctx(w);
    dcp_set_anim(w, 0x13);
    w->disp_flags |= 4;
    w->state = Player_State_ParachuteRelease;
    w->move_flags |= 0x80;
    w->vel_y = -0x780;
    w->vel_y = (s16)(w->vel_y + c->g3F0 * -0x180);
    c->g3F0 = 0;
    c->g3F4 = 0;
    w->pf &= ~0xfu;
    w->trick_input = 0;
    w->timer = 0;
    z_psfx(w, DCP_CH_VOICE, 0x42);
    ParachuteReleased_Create(w);
}
void Player_State_Parachute(DcPlayer *w) {
    ObjPlayerCtx *c = z_ctx(w);
    Obj *g = (Obj *)w->gimmick;
    s32 d;
    switch (w->trick_input) {
    case 0:
        if (g != NULL) {
            w->px = w->x;
            w->py = w->y;
            w->x = ObjShift_SetFromRef(w->x, g->h.x, c->g3F4, 1, 0x400, 0x100);
            d = w->x - g->h.x;
            if (d < 0)
                d = -d;
            if (d < 0x800)
                w->y = ObjShift_SetFromRef(w->y, g->h.y, c->g3F8, 1, 0x400, 0x100);
            if (g->h.x == w->x && g->h.y == w->y)
                w->trick_input = 1;
        } else {
            w->trick_input = 1;
        }
        break;
    case 1:
        w->timer++;
        if (w->timer <= 0x10)
            return;
        Player_Action_ParachuteRelease(w);
        break;
    }
}
void Player_Action_Parachute(DcPlayer *w, Obj *other, s32 v) {
    ObjPlayerCtx *c = z_ctx(w);
    if (w->gimmick == (void *)other)
        return;
    if (w->state == Player_State_Parachute)
        return;
    if (w->state == Player_State_ParachuteRelease)
        return;
    obj_init_state(w);
    w->gimmick = other;
    dcp_set_anim(w, 0x31);
    w->disp_flags |= 4;
    w->state = Player_State_Parachute;
    w->disp_flags &= ~1;
    w->disp_flags |= other->h.disp_flags & 1;
    w->disp_flags ^= 1;
    w->move_flags |= 0x10;
    w->move_flags &= ~0x80u;
    w->pf &= ~1u;
    w->gnd_vel = 0;
    w->vel_x = 0;
    w->vel_y = 0;
    w->angle = 0;
    w->trick_input = 0;
    w->timer = 0;
    c->g3F0 = v;
    c->g3F4 = w->x;
    c->g3F8 = w->y;
    if (w->chara != 1)
        z_psfx(w, DCP_CH_VOICE, 0);
    else
        z_psfx(w, DCP_CH_VOICE, 0x11);
}
void Player_State_ParachuteRelease(DcPlayer *w) {
    ObjPlayerCtx *c = z_ctx(w);
    s32 lim = 0x400;
    switch (w->anim) {
    case 0x13:
    default:
        if (w->pressed & 3)
            w->trick_input = w->pressed;
        if (w->anim >= 0x39 && w->anim <= 0x3b) {
            w->charge++;
            if (w->charge > 0xff)
                w->charge = 0xff;
        }
        if (w->vel_y > -0x400) {
            if (w->anim != 0x13)
                w->disp_flags |= 0x400;
            w->trick_input = 0;
            w->charge = 0;
            dcp_set_anim(w, 0x14);
            z_psfx_stop(w, DCP_CH_TRICK); /* NNS_SndPlayerStopSeq(@0x390, 0x20) + release */
            return;
        }
        if (!(w->pf & 4) && (w->anim < 0x39 || w->charge > 0x18) && w->trick_input != 0)
            dcp_act_trick_combo(w);
        break;
    case 0x14:
        if (w->disp_flags & 8) {
            dcp_set_anim(w, 0x15);
            w->disp_flags |= 4;
            w->hitstop = 100;
            obj_sfx(0x90);
            w->gravity = 8;
            w->fall_max = 0x1e0;
            w->vel_y = 0;
        }
        break;
    case 0x15:
        if (w->pressed & 3) {
            w->trick_input = 0x10;
            w->hitstop = 0x20;
        }
        if (w->trick_input != 0) {
            w->trick_input--;
            c->g3F0 = dcp_spd_up(c->g3F0, 8, 0x80);
            lim <<= 2;
        } else {
            c->g3F0 = dcp_spd_down(c->g3F0, 0x10);
        }
        w->gravity = (s16)(((0x100 - c->g3F0) << 3) >> 8);
        w->fall_max = (s16)(((0x100 - c->g3F0) * 0x1e0) >> 8);
        if (w->pf & 1)
            c->g3F4 = dcp_spd_up(c->g3F4, -0x12, 0xa0);
        else
            c->g3F4 = dcp_spd_up(c->g3F4, 0x12, 0xa0);
        w->timer += c->g3F4;
        if (w->timer > lim)
            w->pf |= 1;
        if (w->timer < -lim)
            w->pf &= ~1u;
        w->angle = (u8)(w->timer >> 8);
        w->push_x = (s16)(w->push_x - (w->timer >> 4));
        dcp_apply_air_input(w);
        break;
    }
    if (!(w->move_flags & 1))
        return;
    w->gimmick = NULL;
    dcp_act_land(w, 0);
    z_call(w->act_idle, w);
}

/* ------------------------------------------------------------ Saucer */
void Player_Action_Saucer(DcPlayer *w, Obj *other) {
    ObjPlayerCtx *c = z_ctx(w);
    if (w->state == Player_State_Parachute)
        return;
    if (w->state == Player_State_ParachuteRelease)
        return;
    if (w->gimmick == (void *)other)
        return;
    obj_init_state(w);
    dcp_stop_highspeed(w);
    dcp_stop_boost(w);
    w->gimmick = other;
    dcp_set_anim(w, 0x3c);
    w->disp_flags |= 4;
    w->state = Player_State_Saucer;
    w->hitstop = 0x40;
    w->pf &= ~0xfu;
    w->pf |= 0x1000;
    w->pf |= 0x100000;
    w->disp_flags &= ~1;
    w->gnd_vel = 0;
    w->vel_x = 0;
    w->vel_y = 0;
    w->angle = 0;
    w->x = other->h.x;
    w->trick_input = 0;
    w->timer = 0;
    c->g3F0 = 0;
    z_psfx(w, DCP_CH_VOICE, 0x6d);
}
void Player_State_Saucer(DcPlayer *p0) {
    if (p0->gimmick != NULL) {
        if ((p0->pressed & 3) == 0)
            return;
    }
    p0->gnd_vel = 0;
    p0->gimmick = NULL;
    p0->pf &= 0xffefefffu;
    p0->angle = 0;
    if ((p0->pressed & 3) != 0) {
        z_call(p0->act_jump, p0);
        return;
    }
    dcp_act_land(p0, 0);
    z_call(p0->act_idle, p0);
}
