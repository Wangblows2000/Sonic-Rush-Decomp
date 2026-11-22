/*
 * Night Carnival (zone 4) objects and decorations, copied from the
 * decompilation and adapted like obj_water.c / obj_mirage.c (task/work
 * struct -> Obj with named fields, VRAM/palette calls dropped,
 * Sound_PlaySeqArcEx -> obj_sfx, sound handles / positional sound / tempo
 * dropped, ScreenShake_Start -> obj_screen_shake, ZoneFx and PaletteAnim ->
 * effect hook):
 *   src/game/Zone/Night_Carnival/Objects/ CraneSlide_*, NeonRoute_*,
 *     Shooter_*, ShooterPart1_*, SwingSky_*, Switch_*, SwitchTimer_*
 *   src/game/Zone/Night_Carnival/Decorations/ DecGroupAF_*,
 *     DecGroupAFChild_*, DecSonicNeon_*, DecSonicNeonPart_*
 * Work offsets: @0x130 -> cmap.rider (+4) / cmap.toucher (+8), @0x1B8 !=
 * NULL -> has_cmap, @0x20C (NeonRoute) -> cmap2.
 * See docs/dreamcast/translation/OBJECTS.md (section 5d).
 */
#include "obj_zone.h"

static s16 s_switch_timers[32]; /* gSwitchTimers (bss data_ov000_0236a2bc, 0x40 bytes) */
void carnival_reset(void) { memset(s_switch_timers, 0, sizeof(s_switch_timers)); }
s16 *obj_switch_timers(void) { return s_switch_timers; }

/* ObjSpd_AccelToward */
s32 obj_spd_accel_toward(DcPlayer *o, s32 v, s32 accel, s32 decel, s16 max) {
    if (ZABS(v) < 0x100 && ZABS(o->gnd_vel) < 0x80) {
        v = 0;
        o->gnd_vel = (s16)v;
    } else {
        if (v < 0) {
            if (o->gnd_vel > 0)
                o->gnd_vel = (s16)dcp_spd_down(o->gnd_vel, decel);
            o->gnd_vel = (s16)dcp_spd_up(o->gnd_vel, -accel, max);
        } else {
            if (o->gnd_vel < 0)
                o->gnd_vel = (s16)dcp_spd_down(o->gnd_vel, decel);
            o->gnd_vel = (s16)dcp_spd_up(o->gnd_vel, accel, max);
        }
        v -= o->gnd_vel;
        o->angle = (u8)((v & 0xffff) >> 8);
    }
    return v;
}

/* ===================================================== CraneSlide (0xae) */
/* blocks 0..4 / 5..9 of mod/gmk_slide.nsbmd (@0xA8 & 0x80 = hidden) */
static void crane_slide_blocks(Obj *w, int second) { w->shapes = second ? 0x3e0u : 0x01fu; }
static void CraneSlide_State_Unk_ov000_023064d8(Obj *w) {
    DcPlayer *tgt = (DcPlayer *)w->target;
    ObjMtx33 m, r;
    s32 v, a;
    u16 idx;
    if (tgt != NULL) {
        if ((w->f1F4 & 2) && !(w->f1F4 & 1)) {
            w->wC = obj_spd_accel_toward(&w->h, w->wC, 0x20, 0x20, 0x400);
            w->h.vel_x = (s16)-w->h.gnd_vel;
        } else {
            w->h.gnd_vel = (s16)dcp_spd_down(w->h.gnd_vel, 10);
            v = w->h.gnd_vel;
            if (v < -0x800)
                v = -0x800;
            else if (v > 0x800)
                v = 0x800;
            w->h.vel_x = (s16)v;
            w->wC = (w->h.x - (w->home_x << 8)) & 0xffff;
            if (w->h.vel_x == 0 && !(w->f1F4 & 1)) {
                w->f1F4 |= 2;
                if (w->wC > 0x7fff)
                    w->wC -= 0x10000;
            }
        }
        if (tgt->gimmick != (void *)w) {
            if (tgt->y > w->h.y + 0x6400)
                w->has_cmap = 1;
            if (tgt->y < w->h.y - 0x7400)
                w->has_cmap = 1;
            w->h.vel_x = 0;
            w->h.gnd_vel = w->h.vel_x;
            w->h.disp_flags |= 0x200;
            w->target = NULL;
            w->w8 = 0x10;
            if (w->wC > 0x7fff)
                w->wC -= 0x10000;
        }
    } else if (w->wC == 0 || (w->f1F4 & 1)) {
        if (w->h.disp_flags & 2)
            w->has_cmap = 1;
        w->h.vel_x = 0;
        w->h.gnd_vel = w->h.vel_x;
        if (w->w8 == 0) {
            w->has_cmap = 1;
            if (!(w->h.obj_flags & 0x2000))
                w->h.obj_flags &= ~2u;
        } else {
            w->w8--;
        }
    } else {
        w->wC = obj_spd_accel_toward(&w->h, w->wC, 0x20, 0x20, 0x400);
        w->h.vel_x = (s16)-w->h.gnd_vel;
    }
    if (w->prm->p6 >= 0) {
        if (w->h.x + w->h.vel_x < (w->home_x << 8)) {
            w->h.x = w->home_x << 8;
            w->h.vel_x = 0;
            w->h.gnd_vel = w->h.vel_x;
        }
        if (w->h.x + w->h.vel_x > (w->home_x + (w->prm->p6 << 6)) << 8) {
            if (!(w->f1F4 & 1)) {
                w->f1F4 |= 1;
                w->h.hitstop = 0x44;
                obj_screen_shake(2);
                obj_sfx(0x6d);
            }
            w->h.x = (w->home_x + (w->prm->p6 << 6)) << 8;
            w->h.gnd_vel = 0;
            w->h.obj_flags |= 0x2000;
            w->h.move_flags |= 0x2000;
        }
    } else {
        if (w->h.x + w->h.vel_x > (w->home_x << 8)) {
            w->h.x = w->home_x << 8;
            w->h.vel_x = 0;
            w->h.gnd_vel = w->h.vel_x;
        }
        if (w->h.x + w->h.vel_x < (w->home_x + (w->prm->p6 << 6)) << 8) {
            if (!(w->f1F4 & 1)) {
                w->f1F4 |= 1;
                w->h.hitstop = 0x44;
                obj_screen_shake(2);
                obj_sfx(0x6d);
            }
            w->h.x = (w->home_x + (w->prm->p6 << 6)) << 8;
            w->h.gnd_vel = 0;
            w->h.obj_flags |= 0x2000;
            w->h.move_flags |= 0x2000;
        }
    }
    if (w->h.gnd_vel != 0) {
        crane_slide_blocks(w, (W.frame & 8) != 0);
    } else {
        crane_slide_blocks(w, 0);
        if (w->h.hitstop == 0) {
            w->h.vel_x = 0;
            w->h.move_x = 0;
        }
    }
    MTX_Identity33_(&m);
    idx = (u16)w->wC;
    MTX_RotX33_(&r, ZSIN(idx), ZCOS(idx));
    MTX_Concat33(&m, &r, &m);
    z_mtx_store(w, &m);
    v = w->wC & 0xffff;
    if (v < 0xc000 && v < 0x4000)
        w->h.disp_flags &= ~2;
    else
        w->h.disp_flags |= 2;
    /* spr1 (second DS screen sprite) 0x80000 toggling: not kept;
     * NNS_SndPlayerSetTempoRatio / StopSeq of the slide loop: dropped */
    v = w->h.vel_x;
    a = v < 0 ? -v : v;
    (void)a;
}
static void CraneSlide_OnHit(ObjBox *a, ObjBox *b) {
    Obj *t0 = z_box_obj(a);
    DcPlayer *t1 = z_box_player(b);
    if (t0 == NULL)
        return;
    if (t1 == NULL)
        return; /* (the matched code tests the type only when unk18 == NULL: deviation 29) */
    if (t0->target != NULL)
        return;
    t0->target = t1;
    t0->has_cmap = 0;
    t0->h.obj_flags |= 2;
    t0->h.disp_flags &= ~0x200;
    t0->f1F4 &= ~3u;
    t0->h.move_flags &= ~0x2000u;
    t0->h.obj_flags &= ~0x2000u;
    t0->h.gnd_vel = t1->move_x;
    t0->h.vel_x = t0->h.gnd_vel;
    if (t0->h.gnd_vel != 0)
        obj_sfx(0x71);
    Player_Action_GrabCraneSlide(t1, t0, 0, 0x70);
    z_count("crane_slide", t0, t1);
}
Obj *CraneSlide_Create(ObjParam *p, s32 b, s32 c) {
    Obj *w = z_mapobj(p, b, c, OBJ_BAC_CRANE_SLIDE, 0xf6, obj_mapobject_dtor);
    if (w == NULL)
        return NULL;
    w->spr.order = 0x14;
    obj_set_anim(w, 1);
    w->spr.flags |= 0x80000;
    /* ten StageObj_LoadModelSimple blocks of mod/gmk_slide.nsbmd (material
     * data_ov000_02366894[i], shape i); blocks 5..9 start hidden */
    z_model(w, "mod__gmk_slide__gmk_slide", 0x34cc);
    crane_slide_blocks(w, 0);
    w->h.disp_flags |= 0x200;
    w->spr.order = 0x14;
    z_trigger(w, 0, -4, 0x60, 4, 0x68, CraneSlide_OnHit, 0x800);
    z_cmap(w, g_obj.data.solid, 3, 10, -12, 0);
    w->h.move_flags |= 0x100;
    w->target = NULL;
    w->snd = 1;
    w->state = CraneSlide_State_Unk_ov000_023064d8; /* CraneSlide_Action_Init */
    return w;
}

/* ===================================================== NeonRoute (0xc0, 0xc1) */
static void NeonRoute_State_WaitSwitch(Obj *w);
static void NeonRoute_State_Move(Obj *w);
static void NeonRoute_Action_StartMove(Obj *p0) {
    ObjParam *t = p0->prm;
    p0->state = NeonRoute_State_Move;
    p0->cmap2.obj = p0;
    p0->has_cmap2 = 1;
    p0->has_cmap = 1;
    if (t->type == 0xc0) {
        p0->h.vel_x = (s16)(t->p7 << 5);
        if (!(t->flags & 1))
            return;
        if (p0->h.vel_x == 0 || (t->p8 * 0xc) / p0->h.vel_x == 0)
            return; /* (division by zero on the DS divider: never placed) */
        p0->h.x = (s32)((u32)W.frame % (u32)(p0->h.vel_x * ((t->p8 * 0xc) / p0->h.vel_x)) + (u32)(p0->home_x << 8));
        p0->w8 = (s32)((u32)p0->h.x % (u32)(((p0->w8 + 1) * 0xc + p0->home_x) << 8));
        return;
    }
    p0->h.vel_y = (s16)(t->p7 << 5);
    if (!(t->flags & 1))
        return;
    if (p0->h.vel_y == 0 || (t->p8 * 0xc) / p0->h.vel_y == 0)
        return;
    p0->h.y = (s32)((u32)W.frame % (u32)(p0->h.vel_y * ((t->p8 * 0xc) / p0->h.vel_y)) + (u32)(p0->home_y << 8));
    p0->w8 = (s32)((u32)p0->h.y % (u32)(((p0->w8 + 1) * 0xc + p0->home_y) << 8));
}
static void NeonRoute_Action_Stop(Obj *p0) {
    p0->state = NeonRoute_State_WaitSwitch;
    p0->h.disp_flags |= 0x20;
    p0->h.vel_x = 0;
    p0->h.vel_y = 0;
    p0->cmap2.obj = NULL;
    p0->has_cmap2 = 0;
    p0->has_cmap = 0;
    p0->w8 = 0;
    p0->h.x = p0->home_x << 8;
    p0->h.y = p0->home_y << 8;
}
static void NeonRoute_State_WaitSwitch(Obj *p0) {
    if (s_switch_timers[p0->prm->p6 & 0x1f] == 0)
        return;
    NeonRoute_Action_StartMove(p0);
}
static void NeonRoute_State_Move(Obj *w) {
    ObjParam *p = w->prm;
    u16 c;
    w->h.disp_flags &= ~0x20;
    if (p->type == 0xc0) {
        if (p->p7 > 0) {
            if ((u32)w->h.x > ((((u32)w->w8 + 1) * 12 + (u32)w->home_x) << 8)) {
                w->w8++;
                obj_sfx(0x70);
            }
            if (w->h.x > (p->p8 * 12 + w->home_x) << 8) {
                w->h.x = w->home_x << 8;
                w->w8 = 0;
            }
        } else {
            if ((u32)w->h.x < (((u32)w->home_x - ((u32)w->w8 + 1) * 12) << 8)) {
                w->w8++;
                obj_sfx(0x70);
            }
            if (w->h.x < (w->home_x - p->p8 * 12) << 8) {
                w->h.x = w->home_x << 8;
                w->w8 = 0;
            }
        }
    } else {
        if (p->p7 > 0) {
            if ((u32)w->h.y > ((((u32)w->w8 + 1) * 12 + (u32)w->home_y) << 8)) {
                w->w8++;
                obj_sfx(0x70);
            }
            if (w->h.y > (p->p8 * 12 + w->home_y) << 8) {
                w->h.y = w->home_y << 8;
                w->w8 = 0;
            }
        } else {
            if ((u32)w->h.y < (((u32)w->home_y - ((u32)w->w8 + 1) * 12) << 8)) {
                w->w8++;
                obj_sfx(0x70);
            }
            if (w->h.y < (w->home_y - p->p8 * 12) << 8) {
                w->h.y = w->home_y << 8;
                w->w8 = 0;
            }
        }
    }
    /* Sound_SetPositional: dropped */
    if (p->p6 != 0 && s_switch_timers[p->p6 & 0x1f] == 0) {
        NeonRoute_Action_Stop(w);
        return;
    }
    if (p->type == 0xc0) {
        c = (u16)(p->p9 * 2);
        if (p->p7 > 0) {
            if ((u32)w->w8 < (u32)(p->p9 * 2))
                c = (u16)(w->w8 + 1);
            w->cmap.w = (u16)((c * 12) >> 3);
            w->cmap2.w = (u16)(((p->p9 * 2 - c) * 12) >> 3);
            w->cmap2.ox = (s16)(w->cmap.ox - p->p8 * 12 + w->cmap.w * 8);
        } else {
            if ((u32)w->w8 < (u32)(p->p9 * 2))
                c = (u16)(w->w8 + 1);
            w->cmap.w = (u16)((c * 12) >> 3);
            w->cmap.ox = (s16)-(w->cmap.w << 3);
            w->cmap2.w = (u16)(((p->p9 * 2 - c) * 12) >> 3);
            w->cmap2.ox = (s16)(p->p8 * 12 + w->cmap.ox - (w->cmap2.w << 3));
        }
    }
    if (p->type != 0xc0 && p->p7 <= 0)
        return;
    if (w->has_cmap2) {
        colmap_register(&w->cmap2);
        if ((u32)w->w8 < (u32)(p->p9 * 2 - 1))
            w->cmap2.h = 2;
        else
            w->cmap2.h = 0;
    }
    if (p->type == 0xc0)
        return;
    if ((u32)w->w8 < (u32)(p->p9 * 2 - 1))
        w->cmap.h = 0;
    else
        w->cmap.h = 2;
}
/* NeonRoute_Draw: the sprite drawn p9 * 2 times along the route (each
 * StageObj_Draw2D call advances the animation, as on the DS) */
static void NeonRoute_Draw(Obj *w) {
    ObjParam *p = w->prm;
    s32 ox = w->h.x, oy = w->h.y;
    u16 i, n = 0;
    if (p->type == 0xc0) {
        if (p->p7 > 0)
            w->h.x = (w->w8 * 12 + w->home_x) << 8;
        else
            w->h.x = (w->home_x - w->w8 * 12) << 8;
    } else {
        if (p->p7 > 0)
            w->h.y = (w->w8 * 12 + w->home_y) << 8;
        else
            w->h.y = (w->home_y - w->w8 * 12) << 8;
    }
    for (i = 0; i < p->p9 * 2; i++) {
        obj_draw2d(w);
        if (n < 64) {
            w->k.neon.pts[n][0] = w->spr.x;
            w->k.neon.pts[n][1] = w->spr.y;
            n++;
        }
        if (p->type == 0xc0) {
            if (p->p7 > 0) {
                w->h.x -= 0xc00;
                if (w->h.x < w->home_x << 8)
                    w->h.x += (p->p8 * 12) << 8;
            } else {
                w->h.x += 0xc00;
                if (w->h.x > w->home_x << 8)
                    w->h.x -= (p->p8 * 12) << 8;
            }
        } else {
            if (p->p7 > 0) {
                w->h.y -= 0xc00;
                if (w->h.y < w->home_y << 8)
                    w->h.y += (p->p8 * 12) << 8;
            } else {
                w->h.y += 0xc00;
                if (w->h.y > w->home_y << 8)
                    w->h.y -= (p->p8 * 12) << 8;
            }
        }
    }
    w->spr.multi = (const s16(*)[2])w->k.neon.pts;
    w->spr.n_multi = (u8)n;
    w->h.x = ox;
    w->h.y = oy;
}
Obj *NeonRoute_Create(ObjParam *a, s32 b, s32 c) {
    Obj *w = z_mapobj(a, b, c, -1, 0xf6, obj_mapobject_dtor);
    ObjParam *p;
    if (w == NULL)
        return NULL;
    if (w->prm->p9 == 0)
        w->prm->p9 = 2;
    if (w->prm->p8 == 0)
        w->prm->p8 = 0x10;
    w->out_ofs = (s16)(w->out_ofs + w->prm->p8 * 0xc);
    obj_load_bac(w, OBJ_BAC_NEON_ROUTE);
    /* StageObj_SetSharedSpriteVram (data_ov000_0236a11c..4c): one VRAM block per kind;
     * natively every route animates its own sprite (same animation) */
    if (w->prm->p6 == 0)
        obj_set_anim(w, w->prm->type == 0xc0 ? 0 : 2);
    else
        obj_set_anim(w, w->prm->type == 0xc0 ? 1 : 3);
    w->spr.order = 0x16;
    w->fn_draw = NeonRoute_Draw;
    w->cmap.obj = w;
    w->has_cmap = 1;
    w->cmap.map = (const u8(*)[8])g_obj.data.solid;
    w->cmap.attr = g_obj.data.col_attr;
    w->cmap2.map = (const u8(*)[8])g_obj.data.solid;
    w->cmap2.attr = g_obj.data.col_attr;
    w->cmap2.obj = w;
    if (w->prm->type == 0xc0) {
        w->cmap.w = (u16)((w->prm->p9 * 0x18) >> 3);
        w->cmap.h = 2;
        if (w->prm->p7 > 0)
            w->cmap.ox = 0;
        else
            w->cmap.ox = (s16)-(w->cmap.w << 3);
        w->cmap.oy = -0x18;
        w->cmap2.w = w->cmap.w;
        w->cmap2.h = w->cmap.h;
        p = w->prm;
        if (p->p7 > 0)
            w->cmap2.ox = (s16)(w->cmap.ox - p->p8 * 0xc);
        else
            w->cmap2.ox = (s16)(w->cmap.ox + p->p8 * 0xc);
        w->cmap2.oy = w->cmap.oy;
    } else {
        w->cmap.w = 6;
        w->cmap.h = 2;
        w->cmap.ox = -0x18;
        if (w->prm->p7 <= 0)
            w->cmap.oy = 4;
        else
            w->cmap.oy = (s16)-(((w->prm->p9 * 0x18) >> 3) << 3);
        w->cmap2.w = w->cmap.w;
        w->cmap2.h = w->cmap.h;
        w->cmap2.ox = w->cmap.ox;
        p = w->prm;
        if (p->p7 > 0)
            w->cmap2.oy = (s16)(w->cmap.oy + p->p8 * 0xc);
        else
            w->cmap2.oy = (s16)(w->cmap.oy - p->p8 * 0xc);
    }
    w->h.move_flags |= 0x100;
    w->h.obj_flags |= 2;
    w->snd = 1;
    NeonRoute_Action_StartMove(w);
    return w;
}

/* ===================================================== Shooter (0xbb, 0xbc) */
static void ShooterPart1_State_Main(Obj *w) {
    Obj *p = w->parent;
    if (p == NULL)
        return;
    switch (w->wC) {
    case 0:
        if (p->wC == 0)
            return;
        obj_set_anim(w, (w->h.obj_flags & 0x1000) ? 8 : 3);
        w->wC++;
        w->h.disp_flags |= 4;
        break;
    case 1:
        if (p->w8 != 0)
            return;
        obj_set_anim(w, (w->h.obj_flags & 0x1000) ? 9 : 4);
        w->wC++;
        w->h.disp_flags |= 4;
        break;
    case 2:
        w->w8++;
        if ((u32)w->w8 <= 0x10)
            return;
        obj_set_anim(w, (w->h.obj_flags & 0x1000) ? 7 : 2);
        w->w8 = 0;
        w->wC = 0;
        w->h.disp_flags |= 4;
        break;
    }
}
static Obj *Shooter_CreatePart1(Obj *arg, s32 n) {
    Obj *w = z_effobj(OBJ_BAC_SHOOTER, OBJ_PRIO_EFFECT);
    if (w == NULL)
        return NULL;
    w->spr.order = 0x12;
    w->h.disp_flags |= arg->h.disp_flags & 1;
    w->h.disp_flags ^= 1;
    w->h.x = arg->h.x;
    w->h.y = arg->h.y;
    w->parent = arg;
    w->w8 = 0;
    if (n)
        w->h.obj_flags |= 0x1000;
    obj_set_anim(w, (w->h.obj_flags & 0x1000) ? 7 : 2);
    w->h.disp_flags |= 4;
    w->h.move_flags |= 0x2100;
    w->h.obj_flags |= 2;
    w->h.obj_flags |= 0x10;
    w->state = ShooterPart1_State_Main;
    return w;
}
static Obj *Shooter_CreatePart2(Obj *arg, s32 type) {
    Obj *w = z_effobj(OBJ_BAC_SHOOTER, OBJ_PRIO_EFFECT);
    if (w == NULL)
        return NULL;
    w->spr.order = 0x13;
    w->h.disp_flags |= arg->h.disp_flags & 1;
    w->h.x = arg->h.x;
    w->h.y = arg->h.y;
    w->parent = arg;
    obj_set_anim(w, type != 0 ? 5 : 0);
    w->h.disp_flags |= 4;
    w->h.move_flags |= 0x2100;
    w->h.obj_flags |= 2;
    w->h.obj_flags |= 0x10;
    return w;
}
static void Shooter_State_Main(Obj *w) {
    s32 flag = 0, old, v;
    DcPlayer *p;
    s16 s;
    if (w->has_cmap && (p = w->cmap.toucher) != NULL && p->type == 1) {
        if (w->prm->type == 0xbb) {
            CAMS.flags |= 0x40000;
            if (w->cmap.rider == p) {
                if (p->held & 0x80) {
                    if (w->w8 == 0)
                        obj_sfx(0x73);
                    flag = 1;
                    w->w8++;
                    if ((u32)w->w8 > 0x40)
                        w->w8 = 0x40;
                    w->h.vel_y = 0x100;
                    if (w->wC > 0x400)
                        w->h.vel_y = 0;
                } else {
                    if ((u32)w->w8 > 8) {
                        s = (s16)-(s16)(w->w8 << 6);
                        w->h.vel_y = s;
                        dcp_act_spring(p, 0, s);
                        obj_player_set_source(p, w);
                        w->h.hitstop = 0x40;
                        w->w8 = 0;
                        w->has_cmap = 0;
                        z_count("shooter", w, p);
                    }
                    w->w8 = 0;
                }
            }
        } else if (w->cmap.rider != p) {
            if ((p->move_flags & 0x2000000) &&
                ((!(w->h.disp_flags & 1) && !(p->disp_flags & 1)) || ((w->h.disp_flags & 1) && (p->disp_flags & 1)))) {
                if (w->w8 == 0)
                    obj_sfx(0x73);
                flag = 1;
                w->w8++;
                if ((u32)w->w8 > 0x40)
                    w->w8 = 0x40;
                w->h.move_flags |= 0x4000000;
                v = w->wC;
                if (v < 0)
                    v = -v;
                if (v > 0x400)
                    w->h.ride_max = 0;
                else
                    w->h.ride_max = 0x100;
            } else {
                w->h.vel_x = 0;
                if ((u32)w->w8 > 8) {
                    s = (s16)(w->w8 << 6);
                    if (!(w->h.disp_flags & 1))
                        s = (s16)-s;
                    w->h.vel_x = s;
                    dcp_act_spring(p, s, 0);
                    obj_player_set_source(p, w);
                    w->h.hitstop = 0x40;
                    w->w8 = 0;
                    w->has_cmap = 0;
                    z_count("shooter", w, p);
                }
                w->w8 = 0;
            }
        }
    } else {
        if (w->prm->type == 0xbb)
            CAMS.flags &= ~0x40000u;
        w->w8 = 0;
    }
    if (!flag) {
        old = w->wC;
        /* NNS_SndPlayerStopSeq (charge loop): dropped */
        w->wC = ObjShift_Set(w->wC, 0, 2, 0x200, 0x80);
        if (w->wC == 0)
            w->has_cmap = 1;
        if (w->prm->type == 0xbb)
            w->h.vel_y = (s16)(w->wC - old);
        else
            w->h.vel_x = (s16)(w->wC - old);
    } else if (w->prm->type == 0xbb) {
        w->wC += w->h.vel_y;
    } else {
        w->wC += w->h.vel_x + w->h.push_x;
    }
}
Obj *Shooter_Create(ObjParam *a, s32 b, s32 c) {
    Obj *w = z_mapobj(a, b, c, OBJ_BAC_SHOOTER, 0xf6, obj_mapobject_dtor);
    if (w == NULL)
        return NULL;
    obj_set_anim(w, w->prm->type == 0xbb ? 1 : 6);
    w->spr.order = 0x14;
    if (w->prm->type == 0xbb) {
        z_cmap(w, g_obj.data.solid, 4, 8, -0xf, -0x38);
        Shooter_CreatePart2(w, 0);
        Shooter_CreatePart1(w, 0);
    } else {
        z_cmap(w, g_obj.data.solid, 7, 3, -0x22, -0x1c);
        Shooter_CreatePart2(w, 1);
        Shooter_CreatePart1(w, 1);
    }
    w->h.ride_max = 0x100;
    if (!(w->prm->flags & 1))
        w->h.disp_flags ^= 1;
    w->h.move_flags |= 0x100;
    w->h.obj_flags |= 2;
    w->snd = 1;
    w->state = Shooter_State_Main; /* Shooter_Action_Init */
    w->wC = 0;
    w->w8 = 0;
    w->h.disp_flags |= 4;
    return w;
}

/* ===================================================== SwingSky (0xbd) */
static void SwingSky_State_Main(Obj *w) {
    DcPlayer *p;
    s32 old = w->wC, v;
    s16 a = 0x2a, b = 10;
    if (!(w->f1F4 & 1))
        return;
    p = (DcPlayer *)w->target;
    if (p != NULL && p->type == 1) {
        if (w->h.gnd_vel != 0)
            w->k.sky.u4A2 = 0x400;
        if (p->gimmick != (void *)w) {
            w->target = NULL;
            w->w8 = 0;
        } else {
            if (p->held & 0x30) {
                if ((w->h.gnd_vel < 0 && (p->held & 0x10)) || (w->h.gnd_vel > 0 && (p->held & 0x20))) {
                    b = 0;
                    if (w->wC > 0 && w->h.gnd_vel > 0)
                        a += 8;
                    if (w->wC < 0 && w->h.gnd_vel < 0)
                        a += 8;
                }
                if ((w->h.gnd_vel > 0 && (p->held & 0x10)) || (w->h.gnd_vel < 0 && (p->held & 0x20)))
                    b += 8;
            }
            v = w->h.gnd_vel;
            if (v < 0)
                v = -v;
            if (v < 0x1c0) {
                v = w->wC;
                if (v < 0)
                    v = -v;
                if (v < 0x1000) {
                    if (p->pressed & 0x10)
                        w->h.gnd_vel = -0x300;
                    else if (p->pressed & 0x20)
                        w->h.gnd_vel = 0x300;
                }
            }
        }
    } else if (w->h.obj_flags & 2) {
        w->w8++;
        if ((u32)w->w8 > 0x18) {
            w->h.obj_flags &= ~2u;
            w->w8 = 0;
        }
    }
    w->wC = obj_spd_accel_toward(&w->h, w->wC, a, b, 0xf00);
    if ((old > 0 && w->wC <= 0) || (old < 0 && w->wC >= 0))
        w->h.move_y = w->h.gnd_vel;
    w->h.move_x = w->h.gnd_vel;
    if (w->prm->p8 > 0x40 && w->prm->p8 < 0xc0)
        return;
    if (w->wC > 0x4800)
        w->wC = 0x4800;
    if (w->wC < -0x4800)
        w->wC = -0x4800;
}
static void SwingSky_OnHit(ObjBox *a, ObjBox *b) {
    Obj *s = z_box_obj(a);
    DcPlayer *t = z_box_player(b);
    if (s == NULL)
        return;
    if (t == NULL)
        return; /* (NULL toucher: the matched code reads byte 0 of NULL; deviation 29) */
    s->f1F4 |= 1;
    s->h.obj_flags |= 2;
    s->target = t;
    Player_Action_SwingSky(t, s, s->prm->p8 > 0x80 ? 0 : 1);
    z_count("swing_sky", s, t);
}
static void SwingSky_Draw(Obj *w) {
    s32 pos[20][2];
    s32 n, bx, by, len, p;
    s16 i, j, k = 0x60;
    u8 n0 = 0, n2 = 0;
    n = w->prm->p6;
    if (n > 10)
        k = (s16)(k + (n - 10) * 4);
    for (i = 0; i <= n; i++) {
        if (i != 0) {
            bx = pos[i - 1][0];
            by = pos[i - 1][1];
            len = 0x800;
            w->k.sky.chain[i] = (s16)(w->k.sky.chain[i] + ((k * (w->k.sky.chain[i - 1] - w->k.sky.chain[i])) >> 8));
        } else {
            bx = w->h.x;
            by = w->h.y;
            len = 0x2000;
            w->k.sky.chain[i] = (s16)w->wC;
        }
        pos[i][0] = bx + ((len * ZSIN(w->k.sky.chain[i])) >> 12);
        pos[i][1] = by + ((len * ZCOS(w->k.sky.chain[i])) >> 12);
    }
    w->k.sky.u4A0 = (u16)(w->k.sky.u4A0 + w->k.sky.u4A2);
    w->k.sky.u4A2 = (s16)dcp_spd_down(w->k.sky.u4A2, 2);
    w->h.angle = (u8)(w->k.sky.u4A0 >> 8);
    w->h.disp_flags |= 0x100;
    obj_draw2d(w);
    w->h.disp_flags &= ~0x100;
    w->h.angle = 0;
    w->h.disp_flags |= 4;
    obj_draw2d_ex(w->h.x, w->h.y, w, &w->k.sky.s6);
    w->h.disp_flags &= ~4;
    for (j = 0; j <= n; j++) {
        if (j == n) {
            w->h.disp_flags |= 0x100;
            w->h.angle = (u8)-(w->k.sky.chain[j] >> 8);
            obj_draw2d_ex(pos[j][0], pos[j][1], w, &w->k.sky.s4);
        } else if (w->k.sky.u4A4 & (1u << j)) {
            obj_draw2d_ex(pos[j][0], pos[j][1], NULL, &w->k.sky.s0);
            w->k.sky.p0[n0][0] = w->k.sky.s0.x;
            w->k.sky.p0[n0][1] = w->k.sky.s0.y;
            n0++;
        } else {
            obj_draw2d_ex(pos[j][0], pos[j][1], NULL, &w->k.sky.s2);
            w->k.sky.p2[n2][0] = w->k.sky.s2.x;
            w->k.sky.p2[n2][1] = w->k.sky.s2.y;
            n2++;
        }
    }
    w->k.sky.s0.multi = (const s16(*)[2])w->k.sky.p0;
    w->k.sky.s0.n_multi = n0;
    w->k.sky.s2.multi = (const s16(*)[2])w->k.sky.p2;
    w->k.sky.s2.n_multi = n2;
    w->h.disp_flags &= ~0x100;
    p = n - 1;
    pos[n][0] = pos[p][0] + ((ZSIN(w->k.sky.chain[n]) * 0x1a00) >> 12);
    pos[n][1] = pos[p][1] + ((ZCOS(w->k.sky.chain[n]) * 0x1a00) >> 12);
    if (!(w->h.obj_flags & 2)) {
        obj_handle_collider(&w->h, w, OBJ_OWNER_OBJECT, &w->detect, 1);
        w->detect.x = pos[n][0] >> 8;
        w->detect.y = pos[n][1] >> 8;
    }
    pos[n][0] = pos[p][0] + ((ZSIN(w->k.sky.chain[n]) * 0x2400) >> 12);
    pos[n][1] = pos[p][1] + ((ZCOS(w->k.sky.chain[n]) * 0x2400) >> 12);
    w->h.px = pos[n][0];
    w->h.py = pos[n][1];
    if (!(W.frame & 3))
        w->k.sky.u4A4 = (w->k.sky.u4A4 << 1) | ((w->k.sky.u4A4 & 0x20000000) >> 29);
}
static void sky_sprite(ObjSprite *s, int anim, u16 order) {
    obj_res_ensure(OBJ_BAC_SWING_SKY);
    spr_init(s, &g_obj.res[OBJ_BAC_SWING_SKY], (u16)anim, 0x40000);
    s->order = order;
}
Obj *SwingSky_Create(ObjParam *a, s32 b, s32 c) {
    Obj *w = z_mapobj(a, b, c, OBJ_BAC_SWING_SKY, 0xf6, obj_mapobject_dtor); /* SwingSky_Destructor */
    u16 i;
    if (w == NULL)
        return NULL;
    w->spr.flags |= 0x200;
    w->spr.order = 6;
    z_set_prio(w, 1);
    w->fn_draw = SwingSky_Draw;
    sky_sprite(&w->k.sky.s0, 3, 0x14);
    sky_sprite(&w->k.sky.s2, 2, 0x14);
    sky_sprite(&w->k.sky.s4, 4, 0x13);
    w->k.sky.s4.flags |= 0x200;
    sky_sprite(&w->k.sky.s6, 1, 5);
    z_spr_prio(&w->k.sky.s6, 1);
    w->xspr[0] = &w->k.sky.s6;
    w->xspr[1] = &w->k.sky.s0;
    w->xspr[2] = &w->k.sky.s2;
    w->xspr[3] = &w->k.sky.s4;
    w->n_xspr = 4;
    rect_set_box2d(&w->detect, -4, -4, 4, 4);
    rect_set_atkdef(&w->detect, 0, (u32)-0x10);
    w->detect.mask = 0x12;
    w->detect.cb = SwingSky_OnHit;
    w->detect.flags |= 0x800;
    z_box_owner(w, &w->detect);
    w->h.move_flags |= 0x2100;
    if (w->prm->p6 == 0)
        w->prm->p6 = 9;
    if (w->prm->p6 <= 2)
        w->prm->p6 = 3;
    if (w->prm->p6 >= 0x14)
        w->prm->p6 = 0x13;
    if (w->prm->p8 == 0)
        w->prm->p8 = 0xd0;
    w->k.sky.u4A4 = 0x38e38e38;
    /* SwingSky_Action_Init */
    w->state = SwingSky_State_Main;
    w->wC = w->prm->p8 << 8;
    if (w->wC > 0x8000)
        w->wC -= 0x10000;
    for (i = 0; i < 20; i++)
        w->k.sky.chain[i] = (s16)w->wC;
    return w;
}

/* ===================================================== Switch (0xbe, 0xbf) */
static void SwitchTimer_Main(Obj *w);
static void SwitchTimer_Destructor(Obj *w) {
    DcPlayer *p0 = z_player0();
    /* NNS_SndPlayerStopSeq / handle release: dropped */
    if (w->k.swt.kind != 0) {
        if (p0 != NULL && (p0->pf2 & 0x8000))
            z_fxhook(p0, OBJ_FX_ZONE4_BLDY, 1, (3 << 8) | 8); /* ZoneFx_Zone4_StartBldyRampUp(3, 8) */
        if (p0 != NULL)
            p0->pf2 &= ~0x4000u;
    }
    s_switch_timers[w->k.swt.idx & 0x1f] = 0;
}
/* SwitchTimer_Create (a plain task, id 0xa00) */
static void SwitchTimer_Create(s16 a, u16 b) {
    Obj *w = obj_alloc(OBJ_PRIO_EFFECT, SwitchTimer_Destructor);
    DcPlayer *p0 = z_player0();
    if (w == NULL)
        return;
    w->kind = OBJK_TRIGGER;
    w->h.obj_flags |= 0x10 | 0x40; /* (not a StageObj: no view-out, runs during pause) */
    w->h.move_flags |= 0x2100;
    w->state = SwitchTimer_Main;
    w->k.swt.idx = a;
    w->k.swt.kind = b;
    obj_sfx(0x6e);
    if (s_switch_timers[w->k.swt.idx & 0x1f] != 0x3fc)
        obj_sfx(0x6f); /* the ticking loop (sound handle @0x8) */
    w->k.swt.u4 = 0;
    obj_log("switch_timer", a, b);
    if (w->k.swt.kind == 0)
        return;
    if (p0 == NULL || !(p0->pf2 & 0x8000))
        return;
    z_fxhook(p0, OBJ_FX_ZONE4_BLDY, 0, 3); /* ZoneFx_Zone4_StartBldyFlash(3) */
    p0->pf2 |= 0x4000;
}
static void SwitchTimer_Main(Obj *w) {
    DcPlayer *p0 = z_player0();
    s16 *t = &s_switch_timers[w->k.swt.idx & 0x1f];
    if (*t == 0x3fc)
        return;
    if (*t != 0) {
        (*t)--;
    } else {
        w->h.obj_flags |= 4; /* Task_Destroy */
        return;
    }
    if (p0 != NULL && (p0->pf & 0x400))
        w->k.swt.u6 = 1;
    if (w->k.swt.u6 != 0) {
        if (p0 == NULL || !(p0->pf & 0x400)) {
            w->h.obj_flags |= 4;
            return;
        }
    }
    if (*t < 0x78 && w->k.swt.u4 == 0)
        w->k.swt.u4 = 1;
    if (w->k.swt.u4 == 0)
        return;
    if (*t == 0x3c) {
        w->k.swt.u4 = 2; /* NNS_SndPlayerSetTempoRatio(loop, 0x200): dropped */
        return;
    }
    if (*t <= 0x3c)
        return;
    if (w->k.swt.u4 != 2)
        return;
    w->k.swt.u4 = 1; /* tempo 0x100 */
}
static Obj *Switch_CreatePart(Obj *arg, s32 n) {
    Obj *w = z_effobj(OBJ_BAC_SWITCH, OBJ_PRIO_EFFECT);
    if (w == NULL)
        return NULL;
    w->spr.order = 0x13;
    z_set_prio(w, 1);
    w->h.disp_flags |= arg->h.disp_flags & 1;
    w->h.disp_flags |= arg->h.disp_flags & 2;
    w->h.x = arg->h.x;
    w->h.y = arg->h.y;
    w->parent = arg;
    if (n) {
        obj_set_anim(w, 2);
        if (w->h.disp_flags & 1)
            w->h.x -= 0x400;
        else
            w->h.x += 0x400;
    } else {
        obj_set_anim(w, 0);
        if (w->h.disp_flags & 2)
            w->h.y -= 0x400;
        else
            w->h.y += 0x400;
    }
    w->h.move_flags |= 0x2100;
    w->h.obj_flags |= 2;
    w->h.obj_flags |= 0x10;
    return w;
}
static void switch_press(Obj *w) {
    if (s_switch_timers[w->prm->p6 & 0x1f] == 0)
        SwitchTimer_Create(w->prm->p6, w->prm->flags & 2);
    s_switch_timers[w->prm->p6 & 0x1f] = (s16)((w->prm->p8 + w->prm->p9) * 2);
}
static void Switch_State_Horizontal(Obj *w) {
    s32 flag = 0, d, old;
    DcPlayer *p;
    w->h.vel_x = 0;
    p = w->cmap.toucher;
    if (p != NULL && p->type == 1 && w->cmap.rider != p && (p->move_flags & 4) &&
        (((w->h.disp_flags & 1) && (p->disp_flags & 1)) || (!(w->h.disp_flags & 1) && !(p->disp_flags & 1)))) {
        if (s_switch_timers[w->prm->p6 & 0x1f] == 0)
            z_count("switch", w, p);
        switch_press(w);
        flag = 1;
        w->f1F4 |= 2;
    }
    if (w->f1F4 & 1) {
        w->h.vel_x = -0x100;
        if (w->h.disp_flags & 1)
            w->h.vel_x = (s16)-w->h.vel_x;
        d = w->wC;
        if (d < 0)
            d = -d;
        if (d <= 0x200)
            w->h.vel_x = 0;
    } else if (w->f1F4 & 2) {
        w->h.vel_x = 0x80;
        if (w->h.disp_flags & 1)
            w->h.vel_x = (s16)-w->h.vel_x;
        d = w->wC;
        if (d < 0)
            d = -d;
        if (d >= 0x400) {
            w->h.vel_x = 0;
            w->f1F4 |= 1;
            w->f1F4 &= ~2u;
        }
    }
    if (!flag && s_switch_timers[w->prm->p6 & 0x1f] == 0) {
        old = w->wC;
        w->f1F4 &= ~1u;
        w->wC = ObjShift_Set(w->wC, 0, 2, 0x200, 0x80);
        w->h.vel_x = (s16)(w->wC - old);
        w->h.move_flags &= ~0x4000000u;
        return;
    }
    w->wC += w->h.vel_x + w->h.push_x;
}
static void Switch_State_Vertical(Obj *w) {
    s32 flag = 0, d, old;
    DcPlayer *p;
    w->h.vel_y = 0;
    p = w->cmap.toucher;
    if (p != NULL && p->type == 1) {
        if (w->cmap.rider == p || ((p->move_flags & 2) && (w->h.disp_flags & 2))) {
            d = (w->h.y >> 8) - ((p->y >> 8) + p->box_l);
            if (d < 0)
                d = -d;
            if (d < 14)
                goto hit;
        }
        d = (w->h.y >> 8) - (p->box_r + (p->y >> 8));
        if (d < 0)
            d = -d;
        if (d < 14) {
        hit:
            if (s_switch_timers[w->prm->p6 & 0x1f] == 0)
                z_count("switch", w, p);
            switch_press(w);
            flag = 1;
            w->f1F4 |= 2;
            w->f1F4 &= ~1u;
        }
    }
    if (w->f1F4 & 1) {
        w->h.vel_y = -0x100;
        if (w->h.disp_flags & 2)
            w->h.vel_y = (s16)-w->h.vel_y;
        if (w->wC <= 0x200)
            w->h.vel_y = 0;
    } else if (w->f1F4 & 2) {
        w->h.vel_y = 0x80;
        if (w->h.disp_flags & 2)
            w->h.vel_y = (s16)-w->h.vel_y;
        if (w->wC >= 0x400) {
            w->h.vel_y = 0;
            w->f1F4 |= 1;
            w->f1F4 &= ~2u;
        }
    }
    if (!flag && s_switch_timers[w->prm->p6 & 0x1f] == 0) {
        old = w->wC;
        w->f1F4 &= ~1u;
        w->wC = ObjShift_Set(w->wC, 0, 2, 0x200, 0x80);
        w->h.vel_y = (s16)(w->wC - old);
        if (w->h.disp_flags & 2)
            w->h.vel_y = (s16)-w->h.vel_y;
        return;
    }
    if (w->h.disp_flags & 2)
        w->wC -= w->h.vel_y;
    else
        w->wC += w->h.vel_y;
}
Obj *Switch_Create(ObjParam *a, s32 b, s32 c) {
    Obj *w = z_mapobj(a, b, c, OBJ_BAC_SWITCH, OBJ_PRIO_MAPOBJ, obj_mapobject_dtor); /* Switch_Destructor */
    const u8 *map;
    if (w == NULL)
        return NULL;
    w->spr.order = 0x14;
    if (w->prm->type == 0xbf)
        obj_set_anim(w, 2);
    if (w->prm->type == 0xbe) {
        map = obj_df("gmk_switch.df", NULL);
        z_cmap(w, map, 4, 2, -0xf, -7);
        obj_set_anim(w, 1);
        if (w->prm->flags & 1)
            w->h.disp_flags ^= 2;
        Switch_CreatePart(w, 0);
    } else {
        map = obj_df("gmk_switch_tate.df", NULL);
        z_cmap(w, map, 2, 4, -8, -0x10);
        obj_set_anim(w, 3);
        if (w->prm->flags & 1)
            w->h.disp_flags ^= 1;
        Switch_CreatePart(w, 1);
    }
    w->h.move_flags |= 0x100;
    /* Switch_Action_Init */
    if (w->prm->type == 0xbe) {
        if (s_switch_timers[w->prm->p6 & 0x1f] != 0) {
            w->wC = 0x200;
            if (w->h.disp_flags & 2)
                w->h.y -= 0x200;
            else
                w->h.y += 0x200;
        }
        w->state = Switch_State_Vertical;
        return w;
    }
    if (s_switch_timers[w->prm->p6 & 0x1f] != 0) {
        w->wC = 0x200;
        if (!(w->h.disp_flags & 1)) {
            w->h.x += 0x200;
        } else {
            w->h.x -= 0x200;
            w->wC = -w->wC;
        }
    }
    w->state = Switch_State_Horizontal;
    return w;
}

/* ================================================ DecGroupAF (decorations) */
static void DecGroupAF_State_SwitchVisible(Obj *s) {
    if (s_switch_timers[0] != 0) /* (reads gSwitchTimers[0]) */
        s->h.disp_flags &= ~0x20;
    else
        s->h.disp_flags |= 0x20;
}
static void DecGroupAF_OnHit(ObjBox *a, ObjBox *b) {
    Obj *s = z_box_obj(a);
    (void)b;
    if (s != NULL)
        s->wC = 1;
}
static void DecGroupAF_State_ClearTouched(Obj *s) { s->wC = 0; }
static void DecGroupAFChild_State_FollowParent(Obj *w) {
    if (w->parent == NULL)
        return;
    w->h.x = w->parent->h.x;
    w->h.y = w->parent->h.y;
    if (w->parent->wC != 0) {
        if (W.frame & 1)
            w->h.disp_flags &= ~0x20;
        else
            w->h.disp_flags |= 0x20;
        if (w->spr.anim == 1)
            w->h.disp_flags ^= 0x20;
        return;
    }
    w->h.disp_flags |= 0x20;
}
/* DecGroupAFChild_Draw: the sprite twice, the second time mirrored */
static void DecGroupAFChild_Draw(Obj *t0) {
    u16 t1;
    obj_draw2d(t0);
    t0->k.afc.mirror = t0->spr;
    t1 = t0->h.disp_flags;
    t0->h.disp_flags = t1 ^ 1;
    obj_draw2d(t0);
    t0->h.disp_flags = t1;
    t0->xspr[0] = &t0->k.afc.mirror;
    t0->n_xspr = 1;
}
/* the light4 children: the ROM names ac_dec_cloud_w.bac, but ObjData_Load
 * returns the resource already held in data_ov000_0236a720 (light4, loaded by
 * the parent first), so they show ac_dec_light4.bac */
static Obj *DecGroupAF_CreateChildBack(Obj *arg) {
    Obj *w = z_effobj(OBJ_BAC_DEC_LIGHT4, 0x4ff);
    if (w == NULL)
        return NULL;
    z_set_prio(w, 1);
    w->spr.order = 5;
    obj_set_anim(w, 2);
    w->h.x = arg->h.x;
    w->h.y = arg->h.y;
    w->parent = arg;
    w->h.disp_flags |= 1;
    w->h.move_flags |= 0x2100;
    w->h.obj_flags |= 0x12;
    w->state = DecGroupAFChild_State_FollowParent;
    w->fn_draw = DecGroupAFChild_Draw;
    return w;
}
static Obj *DecGroupAF_CreateChildFront(Obj *arg) {
    Obj *w = z_effobj(OBJ_BAC_DEC_LIGHT4, 0x4ff);
    if (w == NULL)
        return NULL;
    w->spr.order = 0x15;
    obj_set_anim(w, 1);
    w->h.x = arg->h.x;
    w->h.y = arg->h.y;
    w->parent = arg;
    w->h.disp_flags |= 1;
    w->h.move_flags |= 0x2100;
    w->h.obj_flags |= 0x12;
    w->state = DecGroupAFChild_State_FollowParent;
    return w;
}
static void DecSonicNeonPart_State_FollowParentMask(Obj *w) {
    Obj *p = w->parent;
    if (p != NULL) {
        if ((u32)p->w8 & (1u << (w->w8 & 31)))
            w->h.disp_flags &= ~0x20;
        else
            w->h.disp_flags |= 0x20;
        return;
    }
    w->h.disp_flags |= 0x20;
}
static Obj *DecSonicNeonPart_Create(Obj *arg, u16 idx) {
    Obj *w = z_effobj(OBJ_BAC_DEC_SONIC_NEON, 0x4ff);
    if (w == NULL)
        return NULL;
    /* ObjVram_Alloc of one frame per part: nothing natively */
    w->spr.order = 0x16;
    obj_set_anim(w, idx);
    w->h.x = arg->h.x;
    w->h.y = arg->h.y;
    if (idx >= 0xd) {
        idx -= 0xd;
        w->w8 = idx + 0x10;
        if (!(W.frame & 1))
            obj_draw2d(w);
    } else {
        w->w8 = idx;
        if (W.frame & 1)
            obj_draw2d(w);
    }
    w->h.y = arg->h.y + (idx << 11) - 0x3000;
    w->parent = arg;
    w->h.disp_flags |= 1;
    w->h.move_flags |= 0x2100;
    w->h.obj_flags |= 0x12;
    w->state = DecSonicNeonPart_State_FollowParentMask;
    return w;
}
static void DecSonicNeon_State_BlinkFull(Obj *w) {
    s32 i;
    u32 v, t;
    if (!(w->h.obj_flags & 0x1000)) {
        w->h.obj_flags |= 0x1000;
        for (i = 0; i < 0xc; i++)
            DecSonicNeonPart_Create(w, (u16)i);
        for (i = 0xd; i < 0x19; i++)
            DecSonicNeonPart_Create(w, (u16)i);
    }
    v = (u32)w->w8 & 0xffff;
    t = (u32)w->wC;
    if (t & 0x100) {
        if (!(t & 7)) {
            v <<= 1;
            if (!(v & 0x8000) || ((u32)w->wC & 0xff) == 0xf8)
                v |= 1;
        }
        if (!((u32)w->wC & 0xff))
            v = 0xffff;
    } else if (!(t & 0x80)) {
        if (!(t & 7))
            v >>= 1;
        if (!((u32)w->wC & 0x7f))
            v = 0xfff;
    } else if (!(t & 0x40)) {
        if (!(t & 3)) {
            v <<= 1;
            if (!(v & 2))
                v |= 1;
        }
    } else if (!(t & 3)) {
        v <<= 1;
        v |= 1;
    }
    v &= 0xffff;
    v |= ~v << 16;
    w->w8 = (s32)v;
    w->wC++;
}
static void DecSonicNeon_State_BlinkPair(Obj *w) {
    s32 i;
    u32 fl, v;
    if (w->wC == 0) {
        w->wC = 1;
        if (w->prm->type == 0xec) {
            for (i = 0; i < 0xc; i++)
                DecSonicNeonPart_Create(w, (u16)i);
        } else {
            for (i = 0xd; i < 0x19; i++)
                DecSonicNeonPart_Create(w, (u16)i);
        }
    }
    fl = W.frame;
    v = (u32)w->w8;
    if (fl & 0x100) {
        if (!(fl & 0xff)) {
            v = 0xb6db6db6u;
            if (w->prm->type == 0xee)
                v = ~v;
        }
        if (!(fl & 0x80)) {
            if (!(fl & 0xf))
                v <<= 1;
        } else {
            if (!(fl & 3))
                v <<= 1;
        }
        if (!(v & 2) || !(v & 4))
            v |= 1;
    } else {
        if (!(fl & 0x20))
            v = 0xffffffffu;
        else
            v = 0;
        if (w->prm->type == 0xee)
            v = ~v;
    }
    w->w8 = (s32)v;
}
Obj *DecGroupAF_Create(ObjParam *p, s32 a, s32 b) {
    Obj *w = obj_alloc(OBJ_PRIO_MAPOBJ, Decoration_Destructor);
    s32 idx = 0;
    if (w == NULL)
        return NULL;
    Decoration_InitFromMap(w, p, a, b);
    switch (p->type) {
    default:
        obj_load_bac(w, OBJ_BAC_DEC_NEON_FLOOR);
        if (p->type < 0xb3) {
            if (p->type == 0xb1)
                idx = 1;
            else if (p->type == 0xb2)
                idx = 2;
            else if (!(p->type & 1))
                w->h.disp_flags ^= 1;
        } else {
            idx = ((p->type - 0xb3) >> 1) + 3;
            if (!(p->type & 1))
                w->h.disp_flags ^= 1;
        }
        obj_set_anim(w, (u16)idx); /* (shared VRAM data_ov000_0236a790[idx]: own frame natively) */
        w->spr.order = 6;
        z_set_prio(w, 0);
        w->state = DecGroupAF_State_SwitchVisible;
        break;
    case 0xe4:
        w->h.disp_flags ^= 1;
        /* fall through */
    case 0xe3:
        obj_load_bac(w, OBJ_BAC_DEC_NEON_LAND);
        w->spr.order = 6;
        z_set_prio(w, 0);
        w->state = DecGroupAF_State_SwitchVisible;
        break;
    case 0xe2:
        w->h.disp_flags ^= 2;
        /* fall through */
    case 0xd9:
        obj_load_bac(w, OBJ_BAC_DEC_SWING_SKY_HANG);
        break;
    case 0xdb:
        w->h.disp_flags ^= 1;
        /* fall through */
    case 0xda:
        obj_load_bac(w, OBJ_BAC_DEC_SWING_SKY_HANG);
        obj_set_anim(w, 1);
        break;
    case 0xc3:
        obj_load_bac(w, OBJ_BAC_DEC_NEON_BALL);
        w->spr.order = 6;
        z_set_prio(w, 0);
        obj_palanim_start(w, 0x02364568, p->type); /* PaletteAnim_Create */
        w->h.obj_flags |= 0x800;
        break;
    case 0xc4:
        obj_load_bac(w, OBJ_BAC_DEC_NEON_PILLAR);
        w->spr.order = 6;
        z_set_prio(w, 0);
        obj_palanim_start(w, 0x0236454c, p->type);
        w->h.obj_flags |= 0x800;
        break;
    case 0xd8:
        obj_load_bac(w, OBJ_BAC_DEC_LIGHT4);
        obj_set_anim(w, 0);
        DecGroupAF_CreateChildBack(w);
        DecGroupAF_CreateChildFront(w);
        w->spr.order = 0x16;
        rect_set_box2d(&w->coll[0], -0x30, -8, 0x30, 0x40);
        rect_set_atkdef(&w->coll[0], 0, (u32)-0x10);
        w->coll[0].flags |= 0xc0;
        w->coll[0].cb = DecGroupAF_OnHit;
        z_box_owner(w, &w->coll[0]);
        w->h.obj_flags &= ~2u;
        w->h.move_flags |= 0x2100;
        w->state = DecGroupAF_State_ClearTouched;
        return w;
    case 0xe8:
        obj_load_bac(w, OBJ_BAC_DEC_NEON_BALL2);
        w->spr.order = 0x16;
        w->h.disp_flags |= 4;
        break;
    case 0xe9:
    case 0xea:
        obj_load_bac(w, OBJ_BAC_DEC_NEON_STAR);
        if (p->type == 0xea)
            obj_set_anim(w, 3);
        w->spr.order = 0x16;
        obj_palanim_start(w, 0x02364558, p->type);
        w->h.obj_flags |= 0x800;
        break;
    case 0xeb:
        obj_load_bac(w, OBJ_BAC_DEC_SONIC_NEON);
        obj_set_anim(w, 0xc);
        break;
    case 0xed:
        obj_load_bac(w, OBJ_BAC_DEC_SONIC_NEON);
        obj_set_anim(w, 0x19);
        break;
    case 0xec:
    case 0xee:
        w->state = DecSonicNeon_State_BlinkPair;
        w->h.disp_flags |= 0x20;
        break;
    case 0xef:
        w->w8 = 0xffff;
        w->state = DecSonicNeon_State_BlinkFull;
        w->h.disp_flags |= 0x20;
        break;
    }
    if (w->spr.order == 0)
        w->spr.order = 0x16;
    w->h.move_flags |= 0x2100;
    w->h.obj_flags |= 2;
    return w;
}
