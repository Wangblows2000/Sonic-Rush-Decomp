/*
 * Shared gimmicks, copied from src/game/Objects/, src/game/Stage/ and
 * src/game/Camera/ and adapted (task/work struct -> Obj, VRAM/palette and
 * sound-handle calls dropped or sent to hooks, gCamera -> g_obj_camera):
 *   Rope_*, JumpPedestal_*, PlayerStart_Create, WallBreak_*, IronBall_*,
 *   IronBallPart_*, GmkSpecial_*, GmkLand_*, MapObj73_*, MapObj75_*,
 *   Camera_CheckPosInView, Camera_AddAreaObject, Camera_RemoveAreaObject.
 */
#include <string.h>
#include "obj_internal.h"

ObjCamera g_obj_camera[3];

static Obj *mapobj_prio(ObjParam *p, s32 x, s32 y, int bac, u16 prio, ObjFn dtor) {
    Obj *o = obj_alloc(prio, dtor);
    if (o == NULL)
        return NULL;
    obj_mapobject_init(o, p, x, y);
    o->kind = OBJK_GIMMICK;
    if (bac >= 0)
        obj_load_bac(o, bac);
    return o;
}
static Obj *mapobj(ObjParam *p, s32 x, s32 y, int bac) { return mapobj_prio(p, x, y, bac, OBJ_PRIO_MAPOBJ, obj_mapobject_dtor); }
static void count(const char *ev, Obj *o, DcPlayer *p) {
    g_obj.stats.gimmicks++;
    obj_log(ev, o->prm ? o->prm->type : 0, p ? p->x >> 8 : o->h.x >> 8);
}
static DcPlayer *player0(void) { return g_obj.n_players ? g_obj.players[0].p : NULL; }

/* ScreenShake_Start */
void obj_screen_shake(s32 s) {
    if (g_obj_hooks.shake)
        g_obj_hooks.shake(g_obj_hooks.ctx, s);
    else if (g_dcp_hooks.shake)
        g_dcp_hooks.shake(g_dcp_hooks.ctx, s);
}

/* ================================================================ camera */
void camera_init(void) {
    int i;
    memset(g_obj_camera, 0, sizeof(g_obj_camera));
    for (i = 0; i < 3; i++) {
        g_obj_camera[i].w = 0x100;
        g_obj_camera[i].h = 0xc0;
        g_obj_camera[i].bound[0] = 0;
        g_obj_camera[i].bound[1] = 0;
        g_obj_camera[i].bound[2] = g_obj.map_w << 8;
        g_obj_camera[i].bound[3] = g_obj.map_h << 8;
    }
}
/* Camera_CheckPosInView (dual-screen branch: gCamera[2].flags & 1 is 0 on DC) */
u8 Camera_CheckPosInView(s32 x, s32 y) {
    ObjCamera *p;
    u8 res = 0, i;
    dcp_bool hit;
    for (i = 0; i < 2; i++) {
        p = &g_obj_camera[i];
        hit = x >= p->x - 0x10000 && x <= p->x + (p->w << 8) + 0x10000 && y >= p->y - 0x10000 &&
              y <= p->y + p->h * 0xc0 + 0x10000;
        if (hit) {
            p->flags |= 0x4000000;
            res |= 1 << i;
        }
    }
    return res;
}
void Camera_AddAreaObject(void) {
    if (g_obj_camera[2].flags & 1) {
        g_obj_camera[0].area_objs++;
        return;
    }
    g_obj_camera[0].area_objs++;
    g_obj_camera[1].area_objs++;
}
void Camera_RemoveAreaObject(void) {
    if (g_obj_camera[2].flags & 1) {
        g_obj_camera[0].area_objs--;
        return;
    }
    g_obj_camera[0].area_objs--;
    g_obj_camera[1].area_objs--;
}

/* ================================================================== Rope */
static void Rope_State_Attached(Obj *s) {
    DcPlayer *p = (DcPlayer *)s->target;
    if (p == NULL)
        return;
    s->wC = p->timer;
    if (p->gimmick != s) {
        s->h.obj_flags &= ~2u;
        s->target = NULL;
    }
}
/* Rope_Draw: StageObj_Draw2DEx(pt, 0, NULL, spr0, spr1) per rope point (the
 * sprite is processed once per call, as on the DS) */
static void Rope_Draw(Obj *w) {
    u8 n = 1, i;
    s16 sx, sy;
    DcPlayer *t = (DcPlayer *)w->target;
    memset(w->k.rope.pts, 0, sizeof(w->k.rope.pts));
    if (t != NULL) {
        sx = (s16)(((t->x - w->h.x) / 8) >> 8);
        sy = (s16)(((t->y - w->h.y) / 8) >> 8);
        for (i = 0; i < 8; i++) {
            w->k.rope.pts[i][0] = (s16)(sx * i + (w->h.x >> 8));
            w->k.rope.pts[i][1] = (s16)(sy * i + (w->h.y >> 8));
        }
        if (w->wC != 0) {
            for (i = 1; i < 8; i++) {
                w->k.rope.pts[i][0] += ((i + (w->wC << 1)) & 7) - 4;
                w->k.rope.pts[i][1] += ((i + (w->wC << 1)) & 7) - 4;
            }
        }
        n = 8;
    } else {
        w->k.rope.pts[0][0] = (s16)(w->h.x >> 8);
        w->k.rope.pts[0][1] = (s16)(w->h.y >> 8);
    }
    for (i = 0; i < n; i++)
        obj_draw2d_ex(w->k.rope.pts[i][0] << 8, w->k.rope.pts[i][1] << 8, NULL, &w->spr);
    w->spr.multi = (const s16(*)[2])w->k.rope.pts;
    w->spr.n_multi = n;
}
static void Rope_OnHit(ObjBox *a, ObjBox *b) {
    Obj *t2 = (Obj *)a->owner;
    DcPlayer *t3 = obj_box_player(b);
    s32 t0, v4;
    if (t2->target != NULL)
        return;
    if (t3 == NULL || t3->type != 1)
        return;
    if (t3->move_flags & 0x10)
        return;
    t0 = t3->run_thr[2]; /* @0x37C */
    v4 = t3->gnd_vel;
    if (v4 < 0)
        v4 = -v4;
    if (v4 < t0)
        return;
    t2->target = t3;
    t2->h.obj_flags |= 2;
    /* ObjRect_CenterX / CenterY of coll0 */
    Player_Action_Rope(t3, t2, (t2->coll[0].x + ((t2->coll[0].l + t2->coll[0].r) >> 1)) << 8,
                       (t2->coll[0].y + ((t2->coll[0].t + t2->coll[0].b) >> 1)) << 8);
}
Obj *Rope_Create(ObjParam *p, s32 b, s32 c) {
    Obj *w = mapobj(p, b, c, OBJ_BAC_ROPE);
    if (w == NULL)
        return NULL;
    w->spr.order = 0x14;
    w->fn_draw = Rope_Draw;
    obj_set_anim(w, 0);
    rect_set_box2d(&w->coll[0], p->p6, p->p7, (s16)(p->p8 + p->p6), (s16)(p->p9 + p->p7));
    rect_set_atkdef(&w->coll[0], 0, (u32)-0x10);
    w->coll[0].flags |= 0xc0;
    w->coll[0].cb = Rope_OnHit;
    w->coll[0].owner = w;
    w->coll[0].owner_kind = OBJ_OWNER_OBJECT;
    w->h.move_flags |= 0x2100;
    w->h.disp_flags |= 1;
    w->target = NULL;
    w->state = Rope_State_Attached; /* Rope_Action_Init */
    return w;
}

/* ========================================================= Jump pedestal */
static void JumpPedestal_CheckLaunch(Obj *s, DcPlayer *p) {
    u8 k;
    s32 px, x, lim;
    u32 f = 0, fl;
    s16 spd;
    const s16 *tb;
    fl = s->h.disp_flags;
    spd = p->gnd_vel;
    if (fl & 2)
        spd = (s16)-p->gnd_vel;
    if (s->w8 != 0) {
        lim = p->run_thr[2]; /* @0x37C */
        if ((spd >= lim && !(f = fl & 1)) || (spd <= -lim && (f = fl & 1))) {
            x = s->h.x;
            px = p->x;
            k = s->prm->flags & 3;
            tb = g_obj.data.pedestal[k];
            if ((x + 0x1c00 < px && !f) || (x - 0x1c00 > px && f)) {
                Player_Action_PedestalLaunch(p, (s16)(p->gnd_vel + tb[0]), (s16)(tb[1] - p->jump));
                obj_player_set_source(p, s);
            } else if ((x - 0x400 < px && !f) || (x + 0x400 > px && f)) {
                if (p->pressed & 3) {
                    Player_Action_PedestalLaunch(p, (s16)(p->gnd_vel + tb[2]), (s16)(tb[3] - p->jump));
                    obj_player_set_source(p, s);
                    obj_player_start_launch_trick(p);
                }
            }
        }
    }
    s->w8 = (s32)(p->move_flags & 1);
}
static void JumpPedestal_State_WaitPlayer(Obj *p0) {
    DcPlayer *t0 = p0->cmap.rider;
    if (t0 == NULL)
        return;
    if (t0->type != 1)
        return;
    JumpPedestal_CheckLaunch(p0, t0);
}
Obj *JumpPedestal_Create(ObjParam *p, s32 b, s32 c) {
    static const char *const df[4] = { "gmk_jump30.df", "gmk_jump45.df", "gmk_jump30.df", "gmk_jump30.df" };
    Obj *w = mapobj(p, b, c, OBJ_BAC_JUMP_PEDESTAL); /* JumpPedestal_Destructor: ObjData_Release only */
    const u8 *map;
    if (w == NULL)
        return NULL;
    w->spr.order = 0x14;
    w->h.move_flags |= 0x2100;
    w->h.obj_flags |= 2;
    if (!(w->prm->flags & 4))
        w->h.disp_flags &= ~1;
    map = obj_df(df[w->prm->flags & 3], NULL);
    if (map != NULL) {
        w->has_cmap = 1;
        w->cmap.map = (const u8(*)[8])map;
    }
    w->cmap.obj = w;
    w->cmap.w = 6;
    w->cmap.h = 4;
    w->cmap.ox = -0x18;
    w->cmap.oy = -0x14;
    obj_set_anim(w, (u8)(w->prm->flags & 3)); /* JumpPedestal_Action_Init */
    w->state = JumpPedestal_State_WaitPlayer;
    if (obj_gravity_flipped(w->h.x, w->h.y)) {
        w->h.disp_flags |= 2;
        w->h.grav_rot = 0x80;
    }
    return w;
}

/* =========================================================== PlayerStart */
Obj *PlayerStart_Create(ObjParam *e, s32 x, s32 y) {
    if (!(W.flags & 2) && (!(W.flags & 8) || W.zone >= 7)) {
        g_obj_stage.check_x = x >> 8;
        g_obj_stage.check_y = y >> 8;
        W.check_x = g_obj_stage.check_x;
        W.check_y = g_obj_stage.check_y;
    }
    e->x0 = 0xff;
    return NULL;
}

/* ============================================================ Wall break */
static void WallBreak_State_Idle(Obj *w) { (void)w; }
static void WallBreak_State_Broken(Obj *w) {
    u8 i, j;
    s32 acc = 0x2a, k, v;
    w->wC++;
    if (w->h.grav_rot != 0)
        acc = -0x2a;
    for (i = 0; i < 8; i++) {
        j = 2;
        while (j-- != 0) {
            k = i + j * 8;
            w->k.brk.pos[k][0] += w->k.brk.vel[k][0];
            w->k.brk.pos[k][1] += w->k.brk.vel[k][1];
            v = w->k.brk.pos[k][0];
            w->k.brk.pos[k][0] = v < -0xfff00 ? -0xfff00 : (v > 0xfff00 ? 0xfff00 : v);
            v = w->k.brk.pos[k][1];
            w->k.brk.pos[k][1] = v < -0xfff00 ? -0xfff00 : (v > 0xfff00 ? 0xfff00 : v);
            w->k.brk.vel[k][0] = (s16)dcp_spd_down(w->k.brk.vel[k][0], 0x18);
            w->k.brk.vel[k][1] = (s16)dcp_spd_up(w->k.brk.vel[k][1], acc, 0x3000);
        }
    }
}
/* WallBreak_DrawBroken / GmkLand_Draw: StageObj_Draw2DEx with the piece offsets */
static void draw_pieces(Obj *w) {
    w->spr.ppos = (const s32(*)[2])w->k.brk.pos;
    obj_draw2d_ex(w->h.x, w->h.y, w, &w->spr);
}
static void WallBreak_Action_Break(Obj *w, s32 a) {
    u8 i;
    obj_screen_shake(3);
    w->state = WallBreak_State_Broken;
    w->fn_draw = draw_pieces;
    w->wC = 0;
    w->h.obj_flags |= 2;
    w->has_cmap = 0;
    for (i = 0; i < 16; i++) {
        if (((a << 15) >> 16) > 0)
            w->k.brk.vel[i][0] = (s16)(((a << 15) >> 16) - i * 16);
        else
            w->k.brk.vel[i][0] = (s16)(((a << 15) >> 16) + i * 16);
        w->k.brk.vel[i][1] = (s16)(i * 0x80 - 0x500);
    }
    obj_sfx(0x4b);
}
static void WallBreak_OnHit(ObjBox *a, ObjBox *b) {
    Obj *o = (Obj *)a->owner;
    DcPlayer *p = obj_box_player(b);
    s16 s, v, h;
    s32 mode;
    if (p == NULL || p->type != 1)
        return;
    v = p->gnd_vel;
    s = (s16)(v < 0 ? -v : v);
    h = p->vel_x;
    if (s < (h < 0 ? -h : h)) {
        s = (s16)(h < 0 ? -h : h);
        v = h;
    }
    mode = o->prm->flags & 3;
    if (mode == 0 || (mode == 1 && s > p->run_thr[2]) || (mode == 2 && s > p->run_thr[4])) {
        WallBreak_Action_Break(o, v);
        p->hitstop = 4;
        g_obj.stats.walls_broken++;
        count("wall_break", o, p);
    } else {
        rect_no_hit(a, b);
    }
}
Obj *WallBreak_Create(ObjParam *a, s32 b, s32 c) {
    Obj *w = mapobj_prio(a, b, c, OBJ_BAC_WALL_BREAK, 0xf6, obj_mapobject_dtor);
    if (w == NULL)
        return NULL;
    /* (zone 4: PaletteAnim_Create of the wall colours; palette animation not drawn) */
    w->spr.order = 0x14;
    rect_set_atkdef(&w->coll[0], 0, (u32)-0x10);
    w->coll[0].cb = WallBreak_OnHit;
    w->coll[0].flags |= 0x800;
    w->has_cmap = 1;
    w->cmap.obj = w;
    w->cmap.map = (const u8(*)[8])g_obj.data.solid;
    w->cmap.h = 6;
    w->cmap.oy = -0x1c;
    switch (w->prm->flags & 3) {
    case 0:
        w->cmap.w = 1;
        w->cmap.ox = -3;
        w->has_cmap = 0;
        break;
    default:
        w->cmap.w = 4;
        w->cmap.ox = -0x10;
        break;
    }
    if (obj_gravity_flipped(w->h.x, w->h.y))
        w->h.grav_rot = 0x80;
    w->h.move_flags |= 0x2100;
    switch (w->prm->flags & 3) { /* WallBreak_Action_Idle */
    case 0: obj_set_anim(w, 0); break;
    case 1: obj_set_anim(w, 1); break;
    default: obj_set_anim(w, 2); break;
    }
    w->state = WallBreak_State_Idle;
    return w;
}

/* ============================================================= Iron ball */
static void IronBallPart_State_FollowParent(Obj *p0) {
    Obj *t0 = p0->parent;
    if (t0 == NULL) {
        p0->h.obj_flags |= 4;
        return;
    }
    p0->h.x = t0->h.x;
    p0->h.y = t0->h.y;
    p0->wC += t0->h.gnd_vel;
    p0->h.angle = (u8)(p0->wC >> 8);
}
static void IronBall_CreatePart_ov000_0230cd00(Obj *par) {
    Obj *w = obj_alloc(OBJ_PRIO_MAPOBJ, obj_stageobj_dtor);
    if (w == NULL)
        return;
    w->kind = OBJK_EFFECT;
    obj_load_bac(w, OBJ_BAC_IRON_BALL);
    w->spr.order = 0x13;
    w->parent = par;
    w->h.x = par->h.x;
    w->h.y = par->h.y;
    w->h.move_flags |= 0x2100;
    w->h.disp_flags |= 0x100;
    w->h.obj_flags |= 2;
    w->h.obj_flags |= 0x10;
    obj_set_anim(w, 1); /* IronBallPart_Action_Follow */
    w->state = IronBallPart_State_FollowParent;
    w->wC = 0;
}
static void IronBall_State_Idle(Obj *w) { (void)w; }
static void IronBall_State_Roll(Obj *w) {
    DcPlayer *p = player0();
    s16 spd, d;
    u32 f;
    s32 dx;
    if (p == NULL)
        return;
    if ((w->h.obj_flags & 0x10) && (w->h.move_flags & 1)) {
        if ((p->gnd_vel < 0 ? -p->gnd_vel : p->gnd_vel) > (p->vel_x < 0 ? -p->vel_x : p->vel_x))
            spd = (s16)(p->gnd_vel < 0 ? -p->gnd_vel : p->gnd_vel);
        else
            spd = (s16)(p->vel_x < 0 ? -p->vel_x : p->vel_x);
        if (spd < 0x900) {
            w->h.gnd_max = 0xa00;
        } else {
            dx = w->h.x - p->x;
            d = (s16)((dx < 0 ? -dx : dx) >> 8);
            if (d > 0x80) {
                w->h.slope_acc = 0x140;
                w->h.gnd_max = (s16)(spd + 0x800);
            } else if (d <= 0x40) {
                w->h.slope_acc = 0xc;
                w->h.gnd_max = (s16)(spd - 0x300);
            } else {
                w->h.slope_acc = 0xc;
                w->h.gnd_max = (s16)(spd + ((FX_DivS32((d - 0x40) << 8, 0x40) * 0x280) >> 8));
            }
        }
    }
    if (g_obj.ironball_flag != 0 || (p->pf & 0x400)) {
        w->h.obj_flags &= ~0x10u;
        w->h.slope_acc = 0xc;
        w->h.gnd_max = 0xa00;
    }
    if (!(w->f1F4 & 1)) {
        f = w->h.move_flags;
        if ((f & 0x10) && (f & 1)) {
            obj_screen_shake(4);
            w->h.move_flags |= 0x10;
            w->h.vel_y = -0x280;
            w->w8 = (w->w8 & 0xffff) | (w->h.gnd_vel << 16);
            w->h.gnd_vel = 0;
            w->h.hitstop = 4;
            w->f1F4 |= 1;
            w->wC = 0x100;
        } else if (f & 1) {
            w->h.move_flags &= ~0x10u;
        } else {
            w->h.move_flags |= 0x10;
        }
    } else {
        if (w->wC != 0) {
            w->wC--;
            if (w->wC == 0)
                w->f1F4 &= ~1u;
        }
        if (w->h.move_flags & 1) {
            w->h.move_flags &= ~0x10u;
            if (w->f1F4 & 1) {
                w->h.hitstop = 4;
                obj_screen_shake(4);
                w->h.gnd_vel = (s16)((u32)w->w8 >> 16);
                w->wC = 0;
            }
            w->f1F4 &= ~1u;
        } else {
            w->h.move_flags |= 0x10;
        }
    }
    /* rolling sound 0x50 (looped while grounded; positional pan dropped) */
    if (w->h.move_flags & 1) {
        if (!w->k.snd.playing) {
            obj_sfx(0x50);
            w->k.snd.playing = 1;
        }
    } else {
        w->k.snd.playing = 0;
    }
    w->w8 = (s32)(((u32)w->w8 & 0xffff0000u) | w->h.angle);
}
static void IronBall_OnHitStartRoll(ObjBox *a, ObjBox *b) {
    Obj *t0 = (Obj *)a->owner;
    DcPlayer *t1 = obj_box_player(b);
    if (t1 == NULL || t1->type != 1)
        return;
    t0->h.hitstop = 0x88;
    /* IronBall_Action_StartRoll */
    obj_set_anim(t0, 0);
    t0->coll[0].flags &= ~4;
    t0->coll[1].flags &= ~4;
    t0->h.disp_flags |= 4;
    t0->state = IronBall_State_Roll;
    t0->wC = 0;
    t0->w8 = 0;
    t0->h.move_flags &= ~0x2100u;
    t0->h.move_flags |= 0x10;
    t0->h.obj_flags |= 0x10;
    rect_set_atkdef(&t0->coll[1], 0x37, (u32)-1);
    t0->coll[1].cb = NULL;
    t0->coll[1].flags &= ~0x1000;
    t0->coll[0].flags |= 0x1000;
    g_obj.ironball_flag = 0;
    count("iron_ball_roll", t0, t1);
}
static void IronBall_OnHit_Unk_ov000_0230ce24(ObjBox *a, ObjBox *b) {
    DcPlayer *p = obj_box_player(b);
    (void)a;
    if (p == NULL)
        return;
    if (p->type == 1)
        g_obj.ironball_flag = 1;
}
Obj *IronBall_Create(ObjParam *p, s32 b, s32 c) {
    Obj *w = mapobj(p, b, c, -1); /* IronBall_Destructor: stop the rolling sound */
    if (w == NULL)
        return NULL;
    if (p->type == 0x79) {
        w->kind = OBJK_TRIGGER;
        rect_set_box2d(&w->coll[0], p->p6, p->p7, (s16)(p->p8 + p->p6), (s16)(p->p9 + p->p7));
        rect_set_atkdef(&w->coll[0], 0, (u32)-16);
        w->coll[0].flags |= 0xc0;
        w->coll[0].cb = IronBall_OnHit_Unk_ov000_0230ce24;
        w->coll[0].owner = w;
        w->coll[0].owner_kind = OBJ_OWNER_OBJECT;
        w->h.move_flags |= 0x2120;
        return w;
    }
    obj_load_bac(w, OBJ_BAC_IRON_BALL);
    w->spr.order = 0x14;
    rect_set_box2d(&w->coll[0], p->p6, p->p7, (s16)(p->p8 + p->p6), (s16)(p->p9 + p->p7));
    rect_set_atkdef(&w->coll[0], 0, (u32)-16);
    w->coll[0].flags |= 0xc0;
    w->coll[0].cb = IronBall_OnHitStartRoll;
    w->coll[0].owner = w;
    w->coll[0].owner_kind = OBJ_OWNER_OBJECT;
    w->coll[1].flags |= 0x1000;
    if (p->flags & 1)
        w->coll[0].flags |= 0x1000;
    w->h.box_l = -0x28;
    w->h.box_r = 0x28;
    w->h.box_t = -0x28;
    w->h.box_b = 0x30;
    w->h.move_flags |= 0x2100;
    w->h.move_flags |= 0x200c0;
    w->h.move_flags |= 0x4000;
    w->h.slope_lim = 0;
    w->h.slope_acc = 0xc;
    w->h.gnd_max = 0xa00;
    w->h.gravity = 0xa8;
    w->h.fall_max = 0x1e00;
    g_obj.ironball_flag = 0;
    w->state = IronBall_State_Idle; /* IronBall_Action_Idle */
    IronBall_CreatePart_ov000_0230cd00(w);
    return w;
}

/* ===================================================== Special ring */
static void GmkSpecial_State_Main(Obj *w) {
    DcPlayer *p = (DcPlayer *)w->target;
    s32 v;
    if (p != NULL) {
        w->f1F4 &= ~2u;
        w->h.disp_flags &= ~0x10;
        if (p->pf & 0x80) {
            if (w->spr.anim == 2) {
                obj_set_anim(w, 0);
                w->h.disp_flags |= 4;
            }
        } else if (w->spr.anim == 0) {
            obj_set_anim(w, 2);
            w->h.disp_flags |= 4;
        }
        w->f1F4 &= ~1u;
        if (p->gimmick != w || !(p->move_flags & 0x2000)) {
            w->target = NULL;
            w->wC = w->h.angle << 8;
            w->f1F4 |= 2;
            return;
        }
        w->h.angle = p->angle;
    } else {
        if (w->h.obj_flags & 2) {
            w->w8++;
            if ((u32)w->w8 > 0x10) {
                w->w8 = 0;
                w->h.obj_flags &= ~2u;
            }
        }
        v = w->wC;
        if ((v < 0 ? -v : v) < 0x100 && (w->h.gnd_vel < 0 ? -w->h.gnd_vel : w->h.gnd_vel) < 0x80) {
            w->wC = 0;
            w->h.gnd_vel = 0;
            if (!(w->f1F4 & 1))
                w->h.hitstop = 0x20;
            w->f1F4 |= 1;
            goto end;
        }
        if (v < 0) {
            if (w->h.gnd_vel > 0)
                w->h.gnd_vel = (s16)dcp_spd_down(w->h.gnd_vel, 0x40);
            w->h.gnd_vel = (s16)dcp_spd_up(w->h.gnd_vel, -0x40, 0xf00);
        } else {
            if (w->h.gnd_vel < 0)
                w->h.gnd_vel = (s16)dcp_spd_down(w->h.gnd_vel, 0x40);
            w->h.gnd_vel = (s16)dcp_spd_up(w->h.gnd_vel, 0x40, 0xf00);
        }
        w->wC -= w->h.gnd_vel;
        w->h.angle = (u8)((w->wC & 0xffff) >> 8);
    }
end:
    if (!(w->f1F4 & 2))
        return;
    w->f1F4 &= ~2u;
    obj_set_anim(w, 0);
    w->h.disp_flags |= 0x10;
    w->h.disp_flags |= 4;
}
static void GmkSpecial_Draw(Obj *t0) {
    t0->h.disp_flags |= 0x100;
    obj_draw2d_ex(t0->h.x, t0->h.y, t0, &t0->sprA);
    t0->h.disp_flags &= ~0x100;
    obj_draw2d_ex(t0->h.x, t0->h.y, t0, &t0->spr);
}
static void GmkSpecial_OnHit(ObjBox *a, ObjBox *b) {
    Obj *t0 = (Obj *)a->owner;
    DcPlayer *t1 = obj_box_player(b);
    if (t0 == NULL || t1 == NULL)
        return;
    t0->target = t1;
    t0->h.obj_flags |= 2;
    Player_Action_GmkSpecial(t1, t0);
}
Obj *GmkSpecial_Create(ObjParam *p, s32 b, s32 c) {
    Obj *w;
    if (W.mode == 2 || W.mode == 1 || W.mode == 4) { /* (|| gGameState.unk0 == 1) */
        p->x0 = 0xff;
        return NULL;
    }
    w = mapobj(p, b, c, OBJ_BAC_SPECIAL); /* GmkSpecial_Destructor: VRAM release only */
    if (w == NULL)
        return NULL;
    w->spr.order = 0x14;
    w->fn_draw = GmkSpecial_Draw;
    /* ObjAction_Load2dBAC(&s0, &s1, ac_gmk_cargo.bac): the sprite pair @0x20C */
    obj_res_ensure(OBJ_BAC_CARGO);
    spr_init(&w->sprA, &g_obj.res[OBJ_BAC_CARGO], 0, 0x40000);
    w->has_sprA = 1;
    spr_set_anim(&w->sprA, 1);
    w->sprA.order = 0x13;
    rect_set_atkdef(&w->coll[0], 0, (u32)-0x10);
    w->coll[0].cb = GmkSpecial_OnHit;
    w->coll[0].flags |= 0x800;
    w->h.move_flags |= 0x2100;
    if (obj_gravity_flipped(w->h.x, w->h.y)) {
        w->h.disp_flags |= 2;
        w->h.grav_rot = 0x80;
    }
    obj_set_anim(w, 0); /* GmkSpecial_Action_Init */
    w->h.disp_flags |= 0x10;
    w->h.disp_flags |= 4;
    w->state = GmkSpecial_State_Main;
    return w;
}

/* ============================================================ Platforms */
static void GmkLand_UpdateOscillation(Obj *s) {
    ObjParam *prm = s->prm;
    u16 f = prm->flags;
    u8 k = f & 7;
    u16 c = (u16)s->wC;
    s32 bx = s->h.px, by = s->h.py; /* @0x1C / @0x20: oscillation centre (px) */
    s16 rx = (s16)((prm->p8 * 0x800) >> 12);
    s16 ry = (s16)((prm->p9 * 0x800) >> 12);
    u16 ang;
    s32 x, y;
    if (k)
        ang = (u16)((W.frame * k + c) & 0x3ff); /* gGameState.unk24 */
    else
        ang = (u16)(((W.frame + (c >> 2)) & 0xff) << 2);
    if (f & 0x20) {
        x = (bx + ((rx * obj_sin((u16)((ang << 6) + 0x8000))) >> 12)) << 8;
        y = (by + ((ry * obj_sin((u16)(ang << 6))) >> 12)) << 8;
    } else {
        s16 sn = obj_sin((u16)(ang << 6));
        x = (bx + ((rx * sn) >> 12)) << 8;
        y = (by + ((ry * sn) >> 12)) << 8;
    }
    s->h.move_x = (s16)(x - s->h.x);
    s->h.move_y = (s16)(y - s->h.y);
    s->h.x = x;
    s->h.y = y;
}
static void GmkLand_InitOscillation(Obj *s) {
    s32 w, pos, base;
    u16 ang;
    s->h.px = (s->h.x >> 8) + s->prm->p6 + (s->prm->p8 >> 1);
    s->h.py = (s->h.y >> 8) + s->prm->p7 + (s->prm->p9 >> 1);
    if (s->prm->p9 == 0 && s->prm->p8 == 0)
        return;
    if (s->prm->p9 < s->prm->p8) {
        w = s->prm->p8 >> 1;
        pos = s->h.x >> 8;
        base = s->h.px;
    } else {
        w = s->prm->p9 >> 1;
        pos = s->h.y >> 8;
        base = s->h.py;
    }
    ang = 0x300;
    do {
        if (base + ((s32)(w * obj_sin((u16)(ang << 6))) >> 12) > pos)
            break;
        ang -= 4;
    } while (ang > 0x100);
    s->wC = ang;
    s->wC -= (s16)(((s->prm->flags & 0x18) >> 3) << 8);
    s->wC &= 0x3fff;
}
static dcp_bool ridden(Obj *w) { return w->cmap.rider != NULL && w->cmap.rider->ride == &w->ref; }
static void GmkLand_State_Crumble(Obj *p) {
    s32 delay = 10, step = 8, n, k, t, idx, v;
    ObjParam *r = p->prm;
    u8 y, x, cnt;
    p->wC++;
    if (r->type == 0x76) {
        if (r->p7 != 0)
            step = r->p7;
        if (r->p8 != 0)
            delay = (s8)r->p8;
    }
    if (ridden(p))
        p->h.shake_y = 2;
    n = 3;
    y = (u8)n;
    k = 0x3000;
    cnt = 0;
    while (y-- != 0) {
        x = (u8)n;
        while (x-- != 0) {
            t = cnt * step - (3 - y) * delay;
            if (t < p->wC) {
                idx = y + x * 4;
                p->k.brk.pos[idx][0] += p->k.brk.vel[idx][0];
                p->k.brk.pos[idx][1] += p->k.brk.vel[idx][1];
                v = p->k.brk.pos[idx][0];
                p->k.brk.pos[idx][0] = v < -0x100000 ? -0x100000 : (v > 0x100000 ? 0x100000 : v);
                v = p->k.brk.pos[idx][1];
                p->k.brk.pos[idx][1] = v < -0x100000 ? -0x100000 : (v > 0x100000 ? 0x100000 : v);
                p->k.brk.vel[idx][1] = (s16)dcp_spd_up(p->k.brk.vel[idx][1], (p->wC - t) * 2 + 8, k);
            }
            cnt++;
        }
    }
    if (p->k.brk.pos[2][1] != 0) {
        p->cmap.w = 4;
        p->cmap.ox = -3;
    }
    if (p->k.brk.pos[1][1] != 0) {
        p->cmap.w = 2;
        p->cmap.ox = 0xd;
    }
    if (p->k.brk.pos[0][1] != 0)
        p->has_cmap = 0;
}
static void GmkLand_Action_StartCrumble(Obj *p0) {
    p0->state = GmkLand_State_Crumble;
    p0->fn_draw = draw_pieces;
    p0->wC = 0;
    obj_sfx(0x4b); /* positional */
    count("land_crumble", p0, NULL);
}
static void GmkLand_State_Oscillate(Obj *w) {
    if ((u32)w->w8 < 0x1e)
        GmkLand_UpdateOscillation(w);
    if (ridden(w)) {
        if (w->h.disp_flags & 2)
            w->h.shake_y = -2;
        else
            w->h.shake_y = 2;
    }
    if (!(w->prm->flags & 0x40) && w->prm->type != 0x54)
        return;
    if (ridden(w))
        w->f1F4 |= 1;
    if (!(w->f1F4 & 1))
        return;
    w->w8++;
    if (w->w8 != 0x1e)
        return;
    if (w->prm->type == 0x54) {
        GmkLand_Action_StartCrumble(w);
        return;
    }
    w->h.move_flags &= ~0x2000u;
    w->h.move_flags |= 0x80;
    w->h.px = w->h.x;
    w->h.py = w->h.y;
}
static void GmkLand_State_WaitCrumble(Obj *w) {
    s16 n = 0x1e;
    if (w->prm->p6 > 0)
        n = (s16)(w->prm->p6 << 2);
    if (ridden(w))
        w->h.shake_y = 2;
    if (w->prm->flags & 1)
        w->f1F4 &= ~1u;
    if (ridden(w))
        w->f1F4 |= 1;
    if (w->f1F4 & 1) {
        w->w8++;
        if (w->w8 != n)
            return;
        GmkLand_Action_StartCrumble(w);
        return;
    }
    w->w8 = 0;
}
static void land_anim(Obj *p0) {
    u16 t0 = p0->prm->type;
    if (t0 == 0x53)
        obj_set_anim(p0, 2);
    else if (t0 == 0x52)
        obj_set_anim(p0, 1);
    else
        obj_set_anim(p0, 0);
}
static void GmkLand_State_SpawnPeriodic(Obj *w) {
    s32 t = (w->prm->p8 + 1) << 6;
    if ((s32)(W.frame - (u32)w->wC) < t)
        return;
    w->wC += t;
    obj_mapobject_spawn(0xef, w->h.x, w->h.y, 0, 0, w->prm->p7, 0, 0);
}
static void GmkLand_Action_InitSpawner(Obj *w) {
    u32 n, i;
    s32 x, d, per, sum;
    s16 v;
    ObjParam *p;
    w->wC = (w->prm->p6 & 0x3f) * (w->prm->p8 + 1) + g_obj.land_base;
    w->h.fall_max >>= 3;
    w->h.fall_max += w->prm->p7 << 6;
    x = w->h.x;
    if ((x < g_obj_camera[0].x - 0x1000 || x > g_obj_camera[0].x - 0x11000) &&
        (x < g_obj_camera[1].x - 0x1000 || x > g_obj_camera[1].x - 0x11000)) {
        sum = 0;
        v = 0;
        d = (s32)(W.frame - (u32)w->wC);
        if (d > 0) {
            per = (w->prm->p8 + 1) << 6;
            w->wC += per * ((u32)d / per);
        }
        p = w->prm;
        n = (u32)(W.frame - (u32)w->wC) % (u32)((p->p8 + 1) << 6);
        for (i = 0; i < n; i++) {
            v += w->h.gravity;
            if (v > w->h.fall_max)
                v = w->h.fall_max;
            sum += v;
        }
        obj_mapobject_spawn(0xef, w->h.x, w->h.y + sum, 0, 0, p->p7, 0, 0);
    }
    w->has_cmap = 0;
    w->h.disp_flags |= 0x20;
    w->state = GmkLand_State_SpawnPeriodic;
}
static void GmkLand_State_Unk_ov000_0230f4c8(Obj *s) {
    if (!ridden(s))
        return;
    if (s->h.disp_flags & 2)
        s->h.shake_y = -2;
    else
        s->h.shake_y = 2;
}
static void GmkLand_Action_Unk_ov000_0230f4fc(Obj *w) {
    s32 i;
    w->h.fall_max >>= 3;
    w->h.fall_max += w->prm->p7 << 6;
    w->h.move_flags &= ~0x2000u;
    w->h.move_flags |= 0x80;
    if (w->prm->p8 != 0)
        for (i = 0; i < w->prm->p8; i++)
            dcp_obj_move(&w->h);
    w->state = GmkLand_State_Unk_ov000_0230f4c8;
}
static void GmkLand_Destructor(Obj *p0) {
    if (p0->prm != NULL && p0->prm->type == 0xaf) {
        if (g_obj.land_spawners != 0)
            g_obj.land_spawners--;
        if (g_obj.land_spawners == 0)
            g_obj.land_base = 0;
    }
    obj_mapobject_dtor(p0);
}
Obj *GmkLand_Create(ObjParam *p, s32 b, s32 c) {
    Obj *w = obj_alloc(0xf6, GmkLand_Destructor);
    u16 k;
    if (w == NULL)
        return NULL;
    obj_mapobject_init(w, p, b, c);
    w->kind = OBJK_GIMMICK;
    if (w->prm->type == 0xaf) {
        if (g_obj.land_spawners == 0 && g_obj.land_base == 0)
            g_obj.land_base = (s32)W.frame;
        g_obj.land_spawners++;
    }
    obj_load_bac(w, p->type == 0x76 ? OBJ_BAC_FLOOR_BREAK : OBJ_BAC_LAND);
    /* (zone 4, not 0x76: PaletteAnim_Create; palette animation not drawn) */
    w->spr.order = 0x14;
    w->has_cmap = 1;
    w->cmap.obj = w;
    w->cmap.map = (const u8(*)[8])g_obj.data.solid;
    if (!(w->prm->flags & 0x80) && w->prm->type != 0x53)
        w->cmap.attr = g_obj.data.col_attr;
    k = w->prm->type;
    if (k == 0x52) {
        w->cmap.w = 0xc;
        w->cmap.h = 3;
        w->cmap.ox = -0x2f;
        w->cmap.oy = -0xb;
    } else if (k == 0x53) {
        w->cmap.w = 6;
        w->cmap.h = 6;
        w->cmap.ox = -0x17;
        w->cmap.oy = -0x17;
    } else {
        w->cmap.w = 6;
        w->cmap.h = 3;
        w->cmap.ox = -0x17;
        w->cmap.oy = -0xb;
        if (w->cmap.attr != NULL) {
            w->cmap.h = 1;
            w->cmap.oy += 4;
        }
    }
    w->h.move_flags |= 0x2100;
    w->h.obj_flags |= 2;
    switch (w->prm->type) {
    default:
        GmkLand_InitOscillation(w);
        land_anim(w); /* GmkLand_Action_InitOscillating */
        w->state = GmkLand_State_Oscillate;
        break;
    case 0x76:
        land_anim(w); /* GmkLand_Action_InitCrumbling */
        w->state = GmkLand_State_WaitCrumble;
        break;
    case 0xaf:
        GmkLand_Action_InitSpawner(w);
        w->out_ofs += 0x100;
        break;
    case 0xef:
        GmkLand_Action_Unk_ov000_0230f4fc(w);
        w->out_ofs += 0x200;
        break;
    }
    if (obj_gravity_flipped(w->h.x, w->h.y)) {
        w->h.disp_flags |= 2;
        w->cmap.oy -= 2;
    }
    return w;
}

/* ===================================================== Camera areas */
typedef void (*Mo75Fn)(Obj *, s32);
static void MapObj75_PlayerState_WaitEnter(Obj *w, s32 i);
static void MapObj75_PlayerState_WaitExit(Obj *w, s32 i);
static dcp_bool MapObj75_IsPlayerInArea(Obj *s, s32 i) {
    DcPlayer *p;
    dcp_bool res = DCP_FALSE, in = DCP_FALSE, r, a, b2;
    s32 lo, v;
    int idx = s->k.mo75.pidx[i];
    if (idx < 0 || idx >= g_obj.n_players)
        return DCP_FALSE;
    p = g_obj.players[idx].p;
    v = p->x;
    r = DCP_FALSE;
    lo = s->h.x + (s->prm->p6 << 8);
    if (lo <= v && lo + (s->prm->p8 << 8) >= v)
        r = DCP_TRUE;
    if (r) {
        v = p->y;
        lo = s->h.y + (s->prm->p7 << 8);
        if (lo <= v && lo + (s->prm->p9 << 8) >= v)
            in = DCP_TRUE;
    }
    if (in) {
        a = DCP_TRUE;
        b2 = DCP_FALSE;
        if ((p->obj_flags & 1) && (s->prm->flags & 0x20))
            b2 = a;
        if (!b2 && ((p->obj_flags & 1) || !(s->prm->flags & 0x10)))
            a = DCP_FALSE;
        if (a)
            res = DCP_TRUE;
    }
    return res;
}
static void MapObj75_ApplyCameraBounds(Obj *w, s32 i) {
    ObjCamera *c = &g_obj_camera[i];
    u16 f = w->prm->flags;
    s32 v;
    u32 g;
    w->k.mo75.f20C[i] |= 2;
    if (f & 1) {
        c->bound[0] = w->k.mo75.r214[i][0];
        w->k.mo75.f20C[i] |= 4;
        c->lock[0]++;
    }
    if (f & 4) {
        c->bound[2] = w->k.mo75.r214[i][2];
        w->k.mo75.f20C[i] |= 0x10;
        c->lock[2]++;
    }
    g = g_obj_camera[2].flags;
    if (g & 1) {
        if (g & 2) {
            if (f & 2) {
                v = w->k.mo75.r214[i][1];
                if (c->y - 0x11000 >= v) {
                    c->bound[1] = v + 0x11000;
                } else {
                    c->flags |= 0x20;
                    w->k.mo75.f20C[i] |= 0x80;
                }
                w->k.mo75.f20C[i] |= 8;
                c->lock[1]++;
            }
            if (f & 8) {
                v = w->k.mo75.r214[i][3];
                if (c->y + 0xc000 <= v) {
                    c->bound[3] = v;
                } else {
                    c->flags |= 0x20;
                    w->k.mo75.f20C[i] |= 0x80;
                }
                w->k.mo75.f20C[i] |= 0x20;
                c->lock[3]++;
            }
        } else {
            if (f & 2) {
                v = w->k.mo75.r214[i][1];
                if (c->y >= v) {
                    c->bound[1] = v;
                } else {
                    c->flags |= 0x20;
                    w->k.mo75.f20C[i] |= 0x80;
                }
                w->k.mo75.f20C[i] |= 8;
                c->lock[1]++;
            }
            if (f & 8) {
                v = w->k.mo75.r214[i][3];
                if (c->y + 0x1d000 <= v) {
                    c->bound[3] = v - 0x11000;
                } else {
                    c->flags |= 0x20;
                    w->k.mo75.f20C[i] |= 0x80;
                }
                w->k.mo75.f20C[i] |= 0x20;
                c->lock[3]++;
            }
        }
    } else {
        if (f & 2) {
            c->bound[1] = w->k.mo75.r214[i][1];
            w->k.mo75.f20C[i] |= 8;
            c->lock[1]++;
        }
        if (f & 8) {
            c->bound[3] = w->k.mo75.r214[i][3];
            w->k.mo75.f20C[i] |= 0x20;
            c->lock[3]++;
        }
    }
    w->k.mo75.pstate[i] = MapObj75_PlayerState_WaitExit;
    obj_log("camera_bounds", i, w->prm->flags & 0xf);
}
static void MapObj75_PlayerState_ReleaseBounds(Obj *w, s32 i) {
    ObjCamera *c = &g_obj_camera[i];
    if (w->k.mo75.f20C[i] & 4) {
        if (--c->lock[0] == 0) {
            c->bound[0] = g_obj_camera[2].bound[0];
            c->flags &= ~0x10u;
            w->k.mo75.f20C[i] &= ~4u;
        }
    }
    if (w->k.mo75.f20C[i] & 8) {
        if (--c->lock[1] == 0) {
            c->bound[1] = g_obj_camera[2].bound[1];
            c->flags &= ~0x20u;
            w->k.mo75.f20C[i] &= ~8u;
        }
    }
    if (w->k.mo75.f20C[i] & 0x10) {
        if (--c->lock[2] == 0) {
            c->bound[2] = g_obj_camera[2].bound[2];
            c->flags &= ~0x10u;
            w->k.mo75.f20C[i] &= ~0x10u;
        }
    }
    if (w->k.mo75.f20C[i] & 0x20) {
        if (--c->lock[3] == 0) {
            c->bound[3] = g_obj_camera[2].bound[3];
            c->flags &= ~0x20u;
            w->k.mo75.f20C[i] &= ~0x20u;
        }
    }
    w->k.mo75.f20C[i] &= ~2u;
    w->k.mo75.pstate[i] = MapObj75_PlayerState_WaitEnter;
}
static void MapObj75_PlayerState_WaitEnter(Obj *w, s32 i) {
    if (!MapObj75_IsPlayerInArea(w, i))
        return;
    MapObj75_ApplyCameraBounds(w, i);
}
static void MapObj75_PlayerState_WaitExit(Obj *w, s32 i) {
    if (MapObj75_IsPlayerInArea(w, i))
        return;
    MapObj75_PlayerState_ReleaseBounds(w, i);
}
static void MapObj75_State_Main(Obj *p0) {
    Camera_CheckPosInView(p0->h.x, p0->h.y);
    if (p0->k.mo75.pstate[0] != NULL)
        p0->k.mo75.pstate[0](p0, 0);
    if (p0->k.mo75.pstate[1] == NULL)
        return;
    p0->k.mo75.pstate[1](p0, 1);
}
static void MapObj75_Action_Init(Obj *s) {
    ObjCamera *p;
    u8 i;
    ObjParam *q = s->prm;
    if (g_obj_camera[2].flags & 1) {
        s->k.mo75.pidx[0] = g_obj_camera[0].player;
        s->k.mo75.pidx[1] = -1;
        s->k.mo75.pstate[0] = MapObj75_PlayerState_ReleaseBounds;
        s->k.mo75.pstate[1] = NULL;
        s->k.mo75.r214[0][0] = s->h.x + q->p6 * g_obj_camera[0].w;
        s->k.mo75.r214[0][1] = s->h.y + q->p7 * g_obj_camera[0].h;
        s->k.mo75.r214[0][2] = s->h.x + g_obj_camera[0].w * (q->p6 + q->p8);
        s->k.mo75.r214[0][3] = s->h.y + g_obj_camera[0].h * (q->p7 + q->p9);
    } else {
        for (i = 0; i < 2; i++) {
            p = &g_obj_camera[i];
            s->k.mo75.pidx[i] = p->player;
            s->k.mo75.pstate[i] = MapObj75_PlayerState_ReleaseBounds;
            s->k.mo75.pstate[1] = MapObj75_PlayerState_ReleaseBounds;
            s->k.mo75.r214[i][0] = s->h.x + q->p6 * p->w;
            s->k.mo75.r214[i][1] = s->h.y + q->p7 * p->h;
            s->k.mo75.r214[i][2] = s->h.x + p->w * (q->p6 + q->p8);
            s->k.mo75.r214[i][3] = s->h.y + p->h * (q->p7 + q->p9);
        }
    }
    s->state = MapObj75_State_Main;
}
static void MapObj75_Destructor(Obj *p) {
    Camera_RemoveAreaObject();
    obj_mapobject_dtor(p);
}
Obj *MapObj75_Create(ObjParam *p, s32 b, s32 c) {
    Obj *w = mapobj_prio(p, b, c, -1, OBJ_PRIO_MAPOBJ, MapObj75_Destructor);
    if (w == NULL)
        return NULL;
    w->kind = OBJK_CAMERA;
    w->h.move_flags |= 0x2100;
    w->h.disp_flags |= 0x20;
    MapObj75_Action_Init(w);
    Camera_AddAreaObject();
    return w;
}

/* MapObj73_Unk_*: camera offset blends (four directions) */
static DcPlayer *mo73_player(Obj *s, int i) {
    int idx = s->k.mo73.pidx[i];
    return (idx >= 0 && idx < g_obj.n_players) ? g_obj.players[idx].p : NULL;
}
static void mo73_dir(Obj *s, int vert, int far_side) {
    ObjCamera *P;
    DcPlayer *pl;
    u8 i;
    s32 y, x, num, den, lo, hi, a, b2;
    ObjParam *q = s->prm;
    for (i = 0; i < 2; i++) {
        P = &g_obj_camera[i];
        pl = mo73_player(s, i);
        if (pl == NULL)
            continue;
        if (vert) {
            a = pl->y;
            lo = s->h.y + (s16)(q->p7 << 8);
            hi = s->h.y + ((q->p7 + q->p9) << 8);
        } else {
            a = pl->x;
            lo = s->h.x + (s16)(q->p6 << 8);
            hi = s->h.x + ((q->p6 + q->p8) << 8);
        }
        if (far_side ? a > hi : a < lo) {
            P->ofs_x = s->k.mo73.u210;
            P->ofs_y = s->k.mo73.u214;
            P->flags &= ~0x2000u;
            P->flags |= 0x1000;
        } else if (far_side ? a < lo : a > hi) {
            P->flags &= ~0x2000u;
            P->flags &= ~0x1000u;
        } else {
            P->flags &= ~0x1000u;
            if (vert) {
                b2 = pl->x;
                lo = s->h.x + (s16)(q->p6 << 8);
                hi = s->h.x + ((q->p6 + q->p8) << 8);
            } else {
                b2 = pl->y;
                lo = s->h.y + (s16)(q->p7 << 8);
                hi = s->h.y + ((q->p7 + q->p9) << 8);
            }
            if (b2 >= lo && b2 <= hi) {
                P->flags |= 0x2000;
                if (vert) {
                    num = far_side ? (pl->y >> 8) - ((s->h.y >> 8) + q->p7)
                                   : (q->p7 + q->p9 + (s->h.y >> 8)) - (pl->y >> 8);
                    den = q->p9;
                } else {
                    num = far_side ? (pl->x >> 8) - ((s->h.x >> 8) + q->p6)
                                   : (q->p6 + q->p8 + (s->h.x >> 8)) - (pl->x >> 8);
                    den = q->p8;
                }
                if (den == 0)
                    den = 1; /* (the DS divider returns -1 for a zero divisor) */
                y = P->def_ofs_y + (s->k.mo73.u214 - P->def_ofs_y) * num / den;
                x = P->def_ofs_x + (s->k.mo73.u210 - P->def_ofs_x) * num / den;
                P->ofs_x = x;
                P->ofs_y = y;
            }
        }
    }
}
static void MapObj73_State_Main(Obj *p0) {
    Camera_CheckPosInView(p0->h.x, p0->h.y);
    switch (p0->k.mo73.u218 & 0xff) { /* data_ov000_023643f0 */
    case 0: mo73_dir(p0, 0, 1); break; /* MapObj73_Unk_ov000_02303c58 */
    case 1: mo73_dir(p0, 1, 1); break; /* MapObj73_Unk_ov000_02303ac4 */
    case 2: mo73_dir(p0, 0, 0); break; /* MapObj73_Unk_ov000_02303930 */
    default: mo73_dir(p0, 1, 0); break; /* MapObj73_Unk_ov000_0230379c */
    }
}
static void MapObj73_OnHit(ObjBox *a, ObjBox *b) { (void)a; (void)b; }
static void MapObj73_Destructor(Obj *p) {
    Camera_RemoveAreaObject();
    obj_mapobject_dtor(p);
}
Obj *MapObj73_Create(ObjParam *p, s32 b, s32 c) {
    Obj *w;
    u32 k = p->flags & 0xf;
    if (k != 1 && k != 2 && k != 4 && k != 8)
        p->flags |= 1;
    w = mapobj_prio(p, b, c, -1, OBJ_PRIO_MAPOBJ, MapObj73_Destructor);
    if (w == NULL)
        return NULL;
    w->kind = OBJK_CAMERA;
    rect_set_box2d(&w->coll[0], p->p6, p->p7, (s16)(p->p8 + p->p6), (s16)(p->p9 + p->p7));
    rect_set_atkdef(&w->coll[0], 0, (u32)-16);
    w->coll[0].flags |= 0xc0;
    w->coll[0].cb = MapObj73_OnHit;
    w->coll[0].owner = w;
    w->coll[0].owner_kind = OBJ_OWNER_OBJECT;
    if (g_obj_camera[2].flags & 1) {
        w->k.mo73.pidx[0] = g_obj_camera[0].player;
        w->k.mo73.n = 1;
    } else {
        w->k.mo73.pidx[0] = g_obj_camera[0].player;
        w->k.mo73.pidx[1] = g_obj_camera[1].player;
        w->k.mo73.n = 2;
    }
    w->h.move_flags |= 0x2100;
    w->h.disp_flags |= 0x20;
    switch (p->flags & 0xf) {
    case 1: w->k.mo73.u218 = 0; break;
    case 2: w->k.mo73.u218 = 1; break;
    case 4: w->k.mo73.u218 = 2; break;
    case 8: w->k.mo73.u218 = 3; break;
    }
    w->k.mo73.u210 = -p->p6 << 9;
    w->k.mo73.u214 = -p->p7 << 9;
    w->state = MapObj73_State_Main; /* MapObj73_Action_Init */
    Camera_AddAreaObject();
    return w;
}

/* ================================================ model views (RopeC) */
static int is_model_obj(const Obj *o) {
    if (!o->used || (o->h.obj_flags & 4))
        return 0;
    if (o->model != NULL)
        return 1; /* v3: Wheel2D, Hand2d, GmkTable (StageObj_LoadModelSimple) */
    return o->kind == OBJK_GIMMICK && o->prm && o->prm->type == 0x7a;
}
int objects_model_count(void) {
    Obj *o;
    int n = 0;
    for (o = g_obj.head; o; o = o->next)
        if (is_model_obj(o))
            n++;
    return n;
}
int objects_model_get(int i, ObjModelView *out) {
    Obj *o;
    for (o = g_obj.head; o; o = o->next) {
        if (!is_model_obj(o))
            continue;
        if (i-- != 0)
            continue;
        out->x = o->h.x;
        out->y = o->h.y;
        out->z = o->h.z;
        if (o->model != NULL) {
            out->model = o->model;
            out->mtx = o->mtx;
            out->scale = o->mscale;
        } else {
            out->model = o->k.mdl.shape ? "mod__gmk_rope_c__gmk_rope_c02" : "mod__gmk_rope_c__gmk_rope_c01";
            out->mtx = &o->k.mdl.mtx[0];
            out->scale = o->k.mdl.scale;
        }
        out->visible = (o->h.disp_flags & 0x20) == 0;
        out->persp = (o->h.disp_flags & 0x200) != 0;
        out->shapes = o->shapes != 0 ? o->shapes : 0xffffffffu;
        out->reps = o->mreps != 0 ? o->mreps : 1;
        out->rep_dy = o->mrep_dy;
        return 1;
    }
    return 0;
}

/* ======================================== pushable block (GmkLandAA) */
static void GmkLandAA_State_Decelerate(Obj *w) {
    w->h.gnd_vel = (s16)dcp_spd_down(w->h.gnd_vel, 0x80);
    w->h.vel_x = (s16)dcp_spd_down(w->h.vel_x, 0x80);
    if (w->h.angle != 0)
        w->h.move_flags &= ~0x4000000u;
    else
        w->h.move_flags |= 0x4000000;
}
Obj *GmkLandAA_Create(ObjParam *p, s32 b, s32 c) {
    Obj *w = mapobj_prio(p, b, c, OBJ_BAC_LAND, 0xf6, obj_mapobject_dtor);
    if (w == NULL)
        return NULL;
    w->spr.order = 0x15;
    w->has_cmap = 1;
    w->cmap.obj = w;
    w->cmap.map = (const u8(*)[8])g_obj.data.solid;
    w->cmap.w = 6;
    w->cmap.h = 6;
    w->cmap.ox = -0x17;
    w->cmap.oy = -0x17;
    w->h.box_l = -0x15; /* StageObj_SetMapHitbox(w, -0x15, -0x17, 0x15, 0x17) */
    w->h.box_t = -0x17;
    w->h.box_r = 0x15;
    w->h.box_b = 0x17;
    w->h.move_flags |= 0x4000080;
    w->h.obj_flags |= 2;
    w->h.move_flags |= 0x20040;
    /* (zone 4: PaletteAnim_Create; palette animation not drawn) */
    obj_set_anim(w, 2); /* GmkLandAA_Action_Init */
    w->state = GmkLandAA_State_Decelerate;
    if (obj_gravity_flipped(w->h.x, w->h.y)) {
        w->h.disp_flags |= 2;
        w->h.grav_rot = 0x80;
        w->cmap.oy -= 2;
    }
    w->h.slope_lim = 0;
    w->h.slope_acc = 0xa0;
    w->h.gnd_max = 0xa00;
    w->h.ride_max = 0x1000;
    return w;
}

/* =============================================== water level trigger */
static DcPlayer *player_n(int idx) { return (idx >= 0 && idx < g_obj.n_players) ? g_obj.players[idx].p : NULL; }
static void WaterLevelTrigger_State_Idle(Obj *w) { (void)w; }
static void WaterLevelTrigger_State_ApplyOnce(Obj *w) {
    u8 i;
    DcPlayer *pl;
    if (g_obj_camera[2].flags & 1) {
        w->k.cam.pidx[0] = g_obj_camera[0].player;
        w->k.cam.pidx[1] = g_obj_camera[0].player;
    } else {
        w->k.cam.pidx[0] = g_obj_camera[0].player;
        w->k.cam.pidx[1] = g_obj_camera[1].player;
    }
    for (i = 0; i < 2; i++) {
        pl = player_n(w->k.cam.pidx[i]);
        if (pl != NULL && w->h.x >= pl->x - 0x4000 && w->h.x <= pl->x + 0x4000) {
            g_obj_camera[i].flags |= 0x1000000;
            g_obj_camera[i].water = w->k.cam.value;
        }
    }
    w->state = WaterLevelTrigger_State_Idle;
}
static void WaterLevelTrigger_State_SetBySide(Obj *w) {
    u8 i;
    DcPlayer *pl;
    ObjCamera *e;
    dcp_bool f;
    for (i = 0; i < 2; i++) {
        pl = player_n(w->k.cam.pidx[i]);
        if (pl == NULL)
            continue;
        e = &g_obj_camera[i];
        if (pl->x < w->h.x)
            f = (w->prm->flags & 1) ? DCP_FALSE : DCP_TRUE;
        else
            f = (w->prm->flags & 1) ? DCP_TRUE : DCP_FALSE;
        if (f) {
            e->flags |= 0x1000000;
            e->water = w->k.cam.value;
            if (w->prm->flags & 8)
                e->flags |= 0x2000000;
        } else {
            e->flags &= ~0x1000000u;
            e->water = 0xffff;
            e->flags &= ~0x2000000u;
        }
    }
}
Obj *WaterLevelTrigger_Create(ObjParam *p, s32 b, s32 c) {
    Obj *w;
    if (p->flags & 4) {
        g_obj_camera[0].water = g_obj_camera[1].water = (u16)((u8)p->p6 * 10000 + (u8)p->p7 * 100 + p->p8);
        g_obj_camera[0].flags |= 0x1000000;
        g_obj_camera[1].flags |= 0x1000000;
        p->x0 = 0xff;
        return NULL;
    }
    w = mapobj(p, b, c, -1);
    if (w == NULL)
        return NULL;
    w->kind = OBJK_CAMERA;
    w->h.move_flags |= 0x2100;
    w->h.disp_flags |= 0x20;
    w->k.cam.value = (u16)((u8)p->p6 * 10000 + (u8)p->p7 * 100 + p->p8);
    if (p->flags & 0x10) {
        WaterLevelTrigger_State_ApplyOnce(w);
    } else { /* WaterLevelTrigger_Action_InitBySide */
        if (g_obj_camera[2].flags & 1) {
            w->k.cam.pidx[0] = g_obj_camera[0].player;
            w->k.cam.pidx[1] = g_obj_camera[0].player;
        } else {
            w->k.cam.pidx[0] = g_obj_camera[0].player;
            w->k.cam.pidx[1] = g_obj_camera[1].player;
        }
        w->state = WaterLevelTrigger_State_SetBySide;
    }
    return w;
}

/* ============================================ camera limit (MapObj70) */
/* Camera_CheckPosInViewLatched (dual-screen branch) */
static u8 Camera_CheckPosInViewLatched(s32 x, s32 y) {
    ObjCamera *p;
    u8 res = 0, i;
    for (i = 0; i < 2; i++) {
        p = &g_obj_camera[i];
        if (x >= p->x - 0x10000 && x <= p->x + (p->w << 8) + 0x10000 && y >= p->y - 0x10000 &&
            y <= p->y + p->h * 0xc0 + 0x10000) {
            p->flags34 |= 0x20;
            res |= 1 << i;
        }
    }
    return res;
}
static void MapObj70_State_Idle(Obj *w) { (void)w; }
static void MapObj70_State_SetCameraLimitBySide(Obj *w) {
    u8 i;
    ObjCamera *p;
    DcPlayer *pl;
    u16 f;
    for (i = 0; i < 2; i = (u8)(i + 1)) {
        if (w->k.cam.pidx[i] < 0)
            continue;
        if (!((1 << i) & Camera_CheckPosInViewLatched(w->h.x, w->h.y)))
            continue;
        pl = player_n(w->k.cam.pidx[i]);
        if (pl == NULL)
            continue;
        p = &g_obj_camera[i];
        f = w->prm->flags;
        if (f & 3) {
            if (pl->x < w->h.x) {
                if (f & 1) {
                    if (!(f & 0x20))
                        p->flags34 &= ~0x10u;
                } else {
                    p->flags34 |= 0x10;
                    p->limit = w->k.cam.value;
                }
            } else {
                if (f & 1) {
                    p->flags34 |= 0x10;
                    p->limit = w->k.cam.value;
                } else if (!(f & 0x20)) {
                    p->flags34 &= ~0x10u;
                }
            }
        } else if (f & 0xc) {
            if (pl->y < w->h.y) {
                if (f & 4) {
                    if (!(f & 0x20))
                        p->flags34 &= ~0x10u;
                } else {
                    p->flags34 |= 0x10;
                    p->limit = w->k.cam.value;
                }
            } else {
                if (f & 8) {
                    p->flags34 |= 0x10;
                    p->limit = w->k.cam.value;
                } else if (!(f & 0x20)) {
                    p->flags34 &= ~0x10u;
                }
            }
        }
    }
}
static void MapObj70_Destructor(Obj *p) {
    Camera_RemoveAreaObject();
    obj_mapobject_dtor(p);
}
Obj *MapObj70_Create(ObjParam *p, s32 b, s32 c) {
    Obj *w;
    u8 i;
    DcPlayer *pl;
    if (p->flags & 0x10) {
        g_obj_camera[0].limit = g_obj_camera[1].limit = (u16)((u8)p->p6 * 10000 + (u8)p->p7 * 100 + p->p8);
        g_obj_camera[0].flags34 |= 0x10;
        g_obj_camera[1].flags34 |= 0x10;
        p->x0 = 0xff;
        return NULL;
    }
    w = mapobj_prio(p, b, c, -1, OBJ_PRIO_MAPOBJ, MapObj70_Destructor);
    if (w == NULL)
        return NULL;
    w->kind = OBJK_CAMERA;
    w->spr.order = 0x14;
    rect_set_atkdef(&w->coll[0], 0, (u32)-16);
    w->coll[0].cb = NULL;
    w->coll[0].flags |= 0x800;
    w->h.move_flags |= 0x2100;
    w->h.disp_flags |= 0x20;
    w->k.cam.value = (u16)((u8)p->p6 * 10000 + (u8)p->p7 * 100 + p->p8);
    if (g_obj_camera[2].flags & 1) {
        w->k.cam.pidx[0] = g_obj_camera[0].player;
        w->k.cam.pidx[1] = g_obj_camera[0].player;
    } else {
        w->k.cam.pidx[0] = g_obj_camera[0].player;
        w->k.cam.pidx[1] = g_obj_camera[1].player;
    }
    if (p->flags & 0x40) { /* MapObj70_Action_ApplyInitialCameraLimit */
        for (i = 0; i < 2; i++) {
            pl = player_n(w->k.cam.pidx[i]);
            if (pl != NULL && w->h.x >= pl->x - 0x4000 && w->h.x <= pl->x + 0x4000) {
                g_obj_camera[i].flags34 |= 0x10;
                g_obj_camera[i].limit = w->k.cam.value;
            }
        }
        w->state = MapObj70_State_Idle;
    } else {
        w->state = MapObj70_State_SetCameraLimitBySide; /* MapObj70_Action_Init */
    }
    Camera_AddAreaObject();
    return w;
}
