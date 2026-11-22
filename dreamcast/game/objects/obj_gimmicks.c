/*
 * Gimmick objects, faithful translations of src/game/Objects/:
 *   Spring_*, DashP_*, DashC_*, DashCT_Create, ItemBox_*, ItemBoxPart_*,
 *   Needle_*, GoalRing_*, GoalRingPart_*, Marker_*,
 *   src/game/Stage/StageClear_HandleGoalReached.c (score part).
 * Palette / VRAM allocation calls (StageObj_AllocSpritePalette,
 * StageObj_SetSharedSpriteVram, ObjVram*) have no native equivalent: the
 * converted atlases carry their colours.
 */
#include <string.h>
#include "obj_internal.h"

static Obj *mapobj_new(ObjParam *p, s32 x, s32 y, int bac) {
    Obj *o = obj_alloc(OBJ_PRIO_MAPOBJ, obj_mapobject_dtor);
    if (o == NULL)
        return NULL;
    obj_mapobject_init(o, p, x, y);
    obj_load_bac(o, bac);
    return o;
}
static Obj *effobj_new(int bac) {
    Obj *o = obj_alloc(OBJ_PRIO_EFFECT, obj_stageobj_dtor);
    if (o == NULL)
        return NULL;
    obj_load_bac(o, bac);
    o->kind = OBJK_EFFECT;
    return o;
}
static void spr_prio(Obj *o, u32 prio) { o->spr.flags = (o->spr.flags & ~0x60000u) | (prio << 17); }

/* ================================================================ Spring */
static void spring_state_idle(Obj *o);
static void spring_state_activated(Obj *o);

static void spring_action_idle(Obj *o) {
    obj_set_anim(o, (u16)o->w8);
    o->state = spring_state_idle;
    if (o->prm->flags & 5)
        o->h.disp_flags |= 0x20;
}
static void spring_state_idle(Obj *o) {
    Obj *par;
    if (o->prm->type != 0xe0)
        return;
    par = o->parent;
    if (par == NULL)
        return;
    o->spr.flags &= ~0x60000u;
    if (par->f1F4 & 1)
        o->spr.flags |= 0x20000;
    else
        o->spr.flags |= 0x40000;
}
static void spring_action_activate(Obj *o) {
    obj_set_anim(o, (u16)(o->w8 + 1));
    o->state = spring_state_activated;
    if (o->prm->flags & 4)
        o->h.disp_flags |= 0x20;
    else
        o->h.disp_flags &= ~0x20;
    o->spr.flags &= ~0x60000u;
    o->spr.flags |= 0x40000;
}
static void spring_state_activated(Obj *o) {
    if (!(o->h.disp_flags & 8))
        return;
    o->coll[0].flags &= ~0x100;
    spring_action_idle(o);
}
static void spring_on_hit(ObjBox *a, ObjBox *b) {
    Obj *o = (Obj *)a->owner;
    DcPlayer *p = obj_box_player(b);
    ObjParam *prm;
    u16 id;
    s16 vx, vy;
    if (o == NULL || p == NULL)
        return;
    if ((p->z & ~0x3ff) != (o->h.z & ~0x3ff))
        return;
    spring_action_activate(o);
    prm = o->prm;
    id = prm->type;
    vx = -0x780;
    vx = (s16)(vx - prm->p6 * 0x180);
    vy = vx;
    if (id <= 0x44 || id > 0x4c)
        vx = 0;
    else if (id <= 0x46)
        vy = 0;
    if (id >= 0x47 && id <= 0x4c) {
        vx = (s16)((vx * 0xb5) >> 8);
        vy = (s16)((vy * 0xb5) >> 8);
    }
    if (o->h.disp_flags & 2)
        vy = (s16)-vy;
    if (o->h.disp_flags & 1)
        vx = (s16)-vx;
    if ((id == 0x45 || id == 0x46) && !(prm->flags & 2))
        p->y = o->h.y + 0x200;
    if (W.zone == 3) {
        /* Player_Action_SpringLaunchAlt: Player_Action_Launch with the
         * velocity still in r1/r2, then sound 0x72 */
        dcp_act_launch(p, vx, vy);
        if (g_dcp_hooks.sfx)
            g_dcp_hooks.sfx(g_dcp_hooks.ctx, p, DCP_CH_VOICE, 0x72);
    } else {
        dcp_act_spring(p, vx, vy);
    }
    obj_player_set_source(p, o);
    g_obj.stats.springs++;
    obj_log("spring", id, (vx & 0xffff) | (vy << 16));
}

Obj *spring_create(ObjParam *a, s32 b, s32 c) {
    Obj *w = mapobj_new(a, b, c, OBJ_BAC_SPRING);
    u16 t;
    if (w == NULL)
        return NULL;
    w->kind = OBJK_SPRING;
    w->spr.order = 0x14;
    rect_set_atkdef(&w->coll[0], 0, (u32)-0x10);
    w->coll[0].cb = spring_on_hit;
    w->coll[0].flags |= 0x800;
    w->h.move_flags |= 0x2100;
    t = w->prm->type;
    if (t == 0x44 || (u16)(t - 0x49) <= 1)
        w->h.disp_flags |= 2;
    if (t != 0x46 && t != 0x48 && t != 0x4a && t != 0x4c)
        w->h.disp_flags &= ~1;
    if (t <= 0x4c) {
        if (t >= 0x4b)
            w->w8 = 6;
        else if (t >= 0x47)
            w->w8 = 4;
        else if (t >= 0x45)
            w->w8 = 2;
    }
    spring_action_idle(w);
    return w;
}

/* ============================================================ Dash panel */
static void dashp_state_idle(Obj *s) { s->coll[0].flags &= ~0x100; }
static void dashp_on_hit(ObjBox *a, ObjBox *b) {
    Obj *w = (Obj *)a->owner;
    DcPlayer *o = obj_box_player(b);
    s16 x = 0, y = 0;
    u16 type;
    if (o == NULL)
        return;
    type = w->prm->type;
    if (type != 0x5c && !(o->move_flags & 1)) {
        w->coll[0].flags &= ~0x100;
        return;
    }
    if (type == 0x5c && (o->anim < 0x1a || o->anim > 0x20)) {
        w->coll[0].flags &= ~0x100;
        return;
    }
    switch (type) {
    case 0x61:
    case 0x62:
        if (!(w->h.disp_flags & 2))
            y = (s16)-o->top;
        else
            y = o->top;
        break;
    case 0x98:
    case 0x99:
    case 0x9a:
    case 0x9b:
        x = o->top;
        if (type == 0x98 || type == 0x9a)
            x = (s16)-x;
        break;
    default:
        if (w->h.disp_flags & 1)
            x = (s16)-o->top;
        else
            x = o->top;
        break;
    }
    dcp_act_dash_panel(o, x, y);
    g_obj.stats.dash_panels++;
    obj_log("dash_panel", type, x);
}
Obj *dashp_create(ObjParam *p, s32 b, s32 c) {
    Obj *w = mapobj_new(p, b, c, OBJ_BAC_DASH_P);
    if (w == NULL)
        return NULL;
    w->kind = OBJK_DASHP;
    w->spr.order = 0x14;
    rect_set_atkdef(&w->coll[0], 0, (u32)-16);
    w->coll[0].cb = dashp_on_hit;
    w->h.move_flags |= 0x2100;
    switch (w->prm->type) {
    case 0x60:
        w->h.disp_flags |= 2;
        if (w->prm->flags & 1) w->h.disp_flags |= 1; else w->h.disp_flags &= ~1;
        break;
    case 0x61:
        w->h.disp_flags &= ~1;
        if (w->prm->flags & 1) w->h.disp_flags |= 2;
        break;
    case 0x62:
        w->h.disp_flags |= 1;
        if (w->prm->flags & 1) w->h.disp_flags |= 2;
        break;
    case 0x98:
    case 0x9a:
        w->h.disp_flags &= ~1;
        break;
    case 0x99:
    case 0x9b:
        w->h.disp_flags |= 1;
        break;
    default: /* 0x5c */
        if (w->prm->flags & 1) w->h.disp_flags |= 1; else w->h.disp_flags &= ~1;
        break;
    }
    switch (w->prm->type) {
    case 0x61:
    case 0x62: w->w8 = 1; break;
    case 0x5c:
        w->w8 = 2;
        if (obj_gravity_flipped(w->h.x, w->h.y))
            w->h.disp_flags ^= 2;
        break;
    case 0x98:
    case 0x99: w->w8 = 4; break;
    case 0x9a:
    case 0x9b: w->w8 = 3; break;
    default: w->w8 = 0; break;
    }
    /* DashP_Action_Init */
    obj_set_anim(w, (u16)w->w8);
    w->h.disp_flags |= 4;
    w->state = dashp_state_idle;
    return w;
}

/* ======================================================= Dash ring (DashC) */
static void dashc_state_idle(Obj *s) {
    obj_draw2d_ex(s->h.x, s->h.y, s, &s->sprA); /* StageObj_Draw2D(s, sprA, sprB) */
    s->coll[0].flags &= ~0x100;
}
static void dashc_on_hit(ObjBox *a, ObjBox *b) {
    Obj *w = (Obj *)a->owner;
    DcPlayer *p = obj_box_player(b);
    u16 ang = (u16)((u8)w->u2A4 << 13);
    s16 vx = (s16)((dcp_cos(ang) * 0x800) >> 12);
    s16 vy = (s16)((dcp_sin(ang) * 0x800) >> 12);
    if (p == NULL)
        return;
    obj_player_dash_ring(p, w->h.x, w->h.y, vx, vy);
    if (w->u2A0 & 1)
        obj_player_start_launch_trick(p);
    obj_player_set_source(p, w);
    g_obj.stats.dash_rings++;
    obj_log("dash_ring", w->prm->type, w->u2A4);
}
static void dashc_setup(Obj *w, int bac, u16 t) {
    int ct = (w->u2A0 & 1) != 0;
    (void)bac;
    w->kind = OBJK_DASHC;
    w->spr.order = 9;
    w->spr.flags &= ~0x60000u;
    spr_init(&w->sprA, w->spr.res, 0, 0x40000);
    w->has_sprA = 1;
    w->sprA.order = 0x14;
    w->sprA.flags |= 0x10;
    rect_set_atkdef(&w->coll[0], 0, (u32)-16);
    w->coll[0].cb = dashc_on_hit;
    w->coll[0].flags |= 0x800;
    w->coll[1].mask = 0;
    w->h.move_flags |= 0x2100;
    if (ct)
        w->h.disp_flags |= 4;
    switch (t) {
    case 0x63:
    case 0x6a:
        obj_set_anim(w, 2);
        spr_set_anim(&w->sprA, 3);
        if (!(w->prm->flags & 1)) {
            w->u2A4 = 6;
        } else {
            w->h.disp_flags |= 2;
            w->sprA.flags |= 0x100;
            w->u2A4 = 2;
        }
        break;
    case 0x64:
    case 0x6b:
        obj_set_anim(w, 0);
        spr_set_anim(&w->sprA, 1);
        if (ct ? !(w->prm->flags & 1) : !!(w->prm->flags & 1)) {
            if (ct) {
                w->h.disp_flags |= 1;
                w->sprA.flags |= 0x80;
                w->u2A4 = 0;
            } else {
                w->h.disp_flags &= ~1;
                w->u2A4 = 4;
            }
        } else {
            if (ct) {
                w->u2A4 = 4;
            } else {
                w->h.disp_flags |= 1;
                w->sprA.flags |= 0x80;
                w->u2A4 = 0;
            }
        }
        break;
    case 0x65:
    case 0x6c:
        obj_set_anim(w, 4);
        spr_set_anim(&w->sprA, 5);
        if (w->prm->flags & 1) {
            w->h.disp_flags &= ~1;
            w->h.disp_flags |= 2;
            w->sprA.flags |= 0x100;
            w->u2A4 = 1;
        } else {
            w->h.disp_flags |= 1;
            w->sprA.flags |= 0x80;
            w->u2A4 = 5;
        }
        break;
    case 0x66:
    case 0x6d:
        obj_set_anim(w, 4);
        spr_set_anim(&w->sprA, 5);
        if (w->prm->flags & 1) {
            w->h.disp_flags |= 3;
            w->sprA.flags |= 0x180;
            w->u2A4 = 3;
        } else {
            w->h.disp_flags &= ~1;
            w->u2A4 = 7;
        }
        break;
    }
    /* DashC_Action_Init */
    w->h.disp_flags |= 4;
    w->state = dashc_state_idle;
}
Obj *dashc_create_by_type(ObjParam *p, s32 b, s32 c) {
    Obj *w;
    switch (p->type) {
    case 0x6a: case 0x6b: case 0x6c: case 0x6d: /* DashC_Create */
        w = mapobj_new(p, b, c, OBJ_BAC_DASH_C);
        if (w == NULL)
            return NULL;
        dashc_setup(w, OBJ_BAC_DASH_C, p->type);
        return w;
    case 0x63: case 0x64: case 0x65: case 0x66: /* DashCT_Create */
        w = obj_alloc(OBJ_PRIO_MAPOBJ, obj_mapobject_dtor);
        if (w == NULL)
            return NULL;
        w->u2A0 |= 1;
        obj_mapobject_init(w, p, b, c);
        obj_load_bac(w, OBJ_BAC_DASH_CT);
        dashc_setup(w, OBJ_BAC_DASH_CT, p->type);
        return w;
    }
    return NULL;
}

/* ============================================================== Item box */
static Obj *s_itempart; /* data_ov000_0236a018 */

static void itempart_state_fadeout(Obj *p) {
    p->h.vel_y = (s16)dcp_spd_down(p->h.vel_y, 0x20);
    if (p->wC == 0)
        return;
    p->wC--;
    if (p->wC < 0x20) {
        if (p->wC & 2)
            p->h.disp_flags &= ~0x20;
        else
            p->h.disp_flags |= 0x20;
    }
    if (p->wC == 0)
        p->h.obj_flags |= 4;
}
static void itempart_dtor(Obj *o) {
    if (s_itempart == o)
        s_itempart = NULL;
}
/* ItemBox_CreatePart_ov000_0230d5d4: the item icon popup */
static void itembox_popup(u32 k) {
    Obj *w;
    if (s_itempart != NULL && s_itempart->used) {
        w = s_itempart;
    } else {
        w = obj_alloc(OBJ_PRIO_EFFECT, itempart_dtor);
        if (w == NULL)
            return;
        s_itempart = w;
        obj_load_bac(w, OBJ_BAC_ITEMBOX);
        w->kind = OBJK_ITEMPART;
        w->spr.order = 3;
    }
    obj_set_anim(w, (u16)(k + 2));
    w->h.x = 0x8000;
    w->h.y = 0xb800;
    w->h.vel_y = 0x200;
    spr_prio(w, 0);
    w->spr.order = 3;
    w->h.disp_flags |= 1;
    w->w8 = (s32)k;
    w->wC = 0x80;
    w->h.move_flags |= 0x100;
    w->h.obj_flags |= 0x12;
    w->h.disp_flags |= 0x80;
    w->state = itempart_state_fadeout;
}
static void itempart_state_follow(Obj *p) {
    if (p->parent == NULL)
        return;
    p->h.x = p->parent->h.x + (p->h.vel_x << 8);
    p->h.y = p->parent->h.y + (p->h.vel_y << 8);
    /* (VS mode item cycling not translated) */
}
/* ItemBox_CreatePart_ov000_0230d930: the icon inside the box */
static void itembox_icon(Obj *par, s16 vx, s16 vy, u32 k) {
    Obj *w = obj_alloc(OBJ_PRIO_EFFECT, obj_stageobj_dtor);
    if (w == NULL)
        return;
    obj_load_bac(w, OBJ_BAC_ITEMBOX);
    w->kind = OBJK_ITEMPART;
    obj_set_anim(w, (u16)(k + 2));
    w->spr.order = 0x15;
    w->h.disp_flags |= 1;
    w->h.disp_flags |= par->h.disp_flags & 2;
    w->parent = par;
    w->h.x = par->h.x;
    w->h.y = par->h.y;
    w->h.vel_x = vx;
    w->h.vel_y = vy;
    w->h.move_flags |= 0x2100;
    w->h.obj_flags |= 0x12;
    w->state = itempart_state_follow;
}
/* ItemBox_GiveItem */
static void itembox_give(Obj *a, DcPlayer *p, s32 k) {
    (void)a;
    switch (k) {
    case 1: dcp_add_rings(p, 5); break;
    case 3: dcp_add_tension(p, 0x12c0); break;
    case 4: dcp_add_tension(p, 0x640); break;
    case 2:
        if (g_dcp_hooks.add_life) g_dcp_hooks.add_life(g_dcp_hooks.ctx, p, 1);
        else W.lives++;
        break;
    case 5: case 6: case 7:
        if (g_obj_hooks.item) g_obj_hooks.item(g_obj_hooks.ctx, p, k);
        break;
    case 8: case 9: case 10: case 11:
        obj_sfx(0xdf);
        if (k == 10) dcp_add_tension(p, 0x12c0);
        if (g_obj_hooks.item) g_obj_hooks.item(g_obj_hooks.ctx, p, k);
        break;
    default: /* 0 */
        dcp_add_rings(p, g_obj.data.itembox_rings[(s16)(RAND() & 7)]);
        break;
    }
}
static void itembox_on_hit(ObjBox *a, ObjBox *b) {
    Obj *t0 = (Obj *)a->owner;
    DcPlayer *t1 = obj_box_player(b);
    u32 k;
    if (t0 == NULL || t1 == NULL)
        return;
    if (t1->respawn_inv != 0) { /* @0x3C8 */
        rect_no_hit(a, b);
        return;
    }
    obj_sfx(0x34);
    eff_zako_bomb_burst(t0, 0, 0);
    t0->h.obj_flags |= 8;
    t0->h.obj_flags |= 2;
    t0->f1F4 |= 0x10;
    if (t1->type == 1)
        obj_player_attack_bounce(t1);
    k = (u16)(s16)t0->prm->p6;
    itembox_give(t0, t1, (s32)k);
    itembox_popup(k);
    g_obj.stats.item_boxes++;
    obj_log("item_box", (s32)k, t1->rings);
}
static void itembox_state_cycle(Obj *o) {
    if ((W.flags & 0x20) == 0)
        return;
    o->wC++;
    o->prm->p6 = (s8)g_obj.data.itembox_vs[(o->wC >> 5) & 7];
}
Obj *itembox_create(ObjParam *p, s32 b, s32 c) {
    Obj *w = mapobj_new(p, b, c, OBJ_BAC_ITEMBOX);
    if (w == NULL)
        return NULL;
    w->kind = OBJK_ITEMBOX;
    w->spr.order = 0x16;
    rect_set_atkdef(&w->coll[0], 0, (u32)-16);
    w->coll[0].cb = itembox_on_hit;
    w->coll[0].flags |= 0x800;
    w->h.move_flags |= 0x2100;
    if (W.mode == 2 && w->prm->p6 == 2)
        w->prm->p6 = 3;
    if (w->prm->type < 0x30) {
        switch (0x30 - w->prm->type) {
        case 1: w->prm->p6 = 2; break;
        case 2: w->prm->p6 = 5; break;
        case 3: w->prm->p6 = 6; break;
        case 4: w->prm->p6 = 7; break;
        case 5: w->prm->p6 = 3; break;
        }
    }
    if (obj_gravity_flipped(w->h.x, w->h.y)) {
        w->h.disp_flags |= 2;
        w->h.grav_rot = 0x80;
    }
    if (w->prm->type <= 0x30) {
        itembox_icon(w, 0, 0, (u32)(u8)w->prm->p6);
    } else {
        obj_set_anim(w, 1);
        w->h.disp_flags |= 4;
    }
    w->state = itembox_state_cycle; /* ItemBox_Action_Init */
    return w;
}

/* ================================================================ Needle */
static void needle_state_idle(Obj *w);
static void needle_state_slide(Obj *w);
static void needle_state_extend_delay(Obj *w);

static void needle_action_idle(Obj *w) {
    obj_set_anim(w, w->anim1F8);
    w->h.disp_flags |= 4;
    w->state = needle_state_idle;
    w->wC = 0;
}
static void needle_action_slide(Obj *w) {
    obj_set_anim(w, w->anim1F8);
    w->state = needle_state_slide;
    w->wC = 0;
    w->w8 = 0;
    w->h.obj_flags |= 2;
    w->h.disp_flags &= ~0x20;
    w->h.disp_flags &= ~0x10;
    w->wC = w->anim1F8 == 1 ? -3 : 3;
}
static void needle_action_start_extend_delay(Obj *w) {
    obj_set_anim(w, w->anim1F8);
    w->state = needle_state_extend_delay;
    w->wC = 0;
    w->w8 = 0;
    w->h.obj_flags |= 2;
    w->h.disp_flags &= ~0x20;
    w->w8 = 0x18;
    w->h.disp_flags |= 0x10;
}
static void needle_state_extend_delay(Obj *w) {
    w->w8--;
    if (w->w8 != 0)
        return;
    needle_action_slide(w);
}
static void needle_state_slide(Obj *w) {
    w->w8++;
    w->cmap.oy = (s16)(w->cmap.oy + w->wC);
    if ((u32)w->w8 < 8)
        return;
    if (w->anim1F8 == 2) {
        w->anim1F8 = 0;
        w->h.disp_flags |= 0x20;
    } else if (w->anim1F8 == 1) {
        w->h.obj_flags &= ~2u;
        w->anim1F8 = 0;
    }
    needle_action_idle(w);
}
static void needle_state_idle(Obj *w) {
    DcPlayer *r;
    ObjPlayerCtx *pc;
    if (w->prm->type >= 0x5d) {
        w->wC++;
        if (w->wC > 0x78) {
            if (w->h.disp_flags & 0x20) {
                w->anim1F8 = 1;
                needle_action_start_extend_delay(w);
            } else {
                w->anim1F8 = 2;
                needle_action_slide(w);
            }
            return;
        }
    }
    if (w->prm->type != 0x4d && w->prm->type != 0x5d)
        return;
    r = w->cmap.rider;
    if (r == NULL || r->ride != &w->ref || r->type != 1)
        return;
    pc = obj_player_ctx(r);
    if (pc == NULL || !rect_flag_check(w->coll[0].atk, pc->body.def))
        return;
    obj_player_on_hurt(&pc->body, &w->coll[0]);
}
Obj *needle_create(ObjParam *a, s32 b, s32 c) {
    Obj *w = mapobj_new(a, b, c, OBJ_BAC_NEEDLE);
    if (w == NULL)
        return NULL;
    w->kind = OBJK_NEEDLE;
    w->spr.order = 0x14;
    rect_set_atkdef(&w->coll[0], 0x53, (u32)-1);
    w->coll[0].cb = NULL;
    w->has_cmap = 1;
    w->cmap.obj = w;
    w->cmap.map = (const u8(*)[8])g_obj.data.needle_shape;
    if ((u16)(w->prm->type - 0x4f) <= 1) {
        w->cmap.w = 3;
        w->cmap.h = 4;
        w->cmap.ox = -2;
        w->cmap.oy = -0x10;
        w->anim1F8 = 3;
    } else {
        w->cmap.w = 4;
        w->cmap.h = 3;
        w->cmap.ox = -0x10;
        w->cmap.oy = -0x14;
        if (!obj_gravity_flipped(w->h.x, w->h.y))
            w->cmap.angles_ff = 1; /* unk1CC = -1 */
    }
    w->h.move_flags |= 0x2100;
    w->h.disp_flags |= 1;
    if (w->prm->type == 0x4e || w->prm->type == 0x5e)
        w->h.disp_flags |= 2;
    if (w->prm->type == 0x50)
        w->h.disp_flags &= ~1;
    needle_action_idle(w);
    w->wC = 0x79;
    w->wC -= w->prm->p6;
    return w;
}

/* ========================================================== Goal ring */
static void goal_state_idle(Obj *w);
static void goal_state_spin(Obj *w);

static void goalpart_state_sparkle(Obj *p) {
    if ((p->h.move_flags & 0x80) == 0) {
        p->h.vel_x = (s16)dcp_spd_down(p->h.vel_x, 8);
        p->h.vel_y = (s16)dcp_spd_down(p->h.vel_y, 8);
    }
    if (p->h.disp_flags & 8)
        p->h.obj_flags |= 4;
}
/* GoalRing_CreatePart_ov000_0230a864 (sparkle) */
static void goal_sparkle(Obj *par, s32 dx, s32 dy, s16 vx, s16 vy) {
    Obj *w = effobj_new(OBJ_BAC_GOALRING);
    if (w == NULL)
        return;
    w->kind = OBJK_GOALPART;
    w->spr.order = 7;
    w->h.x = par->h.x;
    w->h.y = par->h.y;
    w->h.vel_x = vx;
    w->h.vel_y = vy;
    if (par->h.disp_flags & 1)
        w->h.disp_flags |= 1;
    if (w->h.disp_flags & 1) {
        w->h.x -= dx << 8;
        w->h.vel_x = (s16)-w->h.vel_x;
    } else {
        w->h.x += dx << 8;
    }
    w->h.y += dy << 8;
    w->h.move_flags |= 0x100;
    w->h.obj_flags |= 2;
    if (vx == 0 && vy == 0) {
        w->h.move_flags |= 0x80;
        w->h.gravity = 4;
        w->h.fall_max = 0x100;
    }
    obj_set_anim(w, 8); /* GoalRingPart_Action_Sparkle */
    w->state = goalpart_state_sparkle;
}
static void goalpart_state_follow(Obj *p) {
    Obj *t0 = p->parent;
    if (t0 == NULL)
        return;
    p->h.x = t0->h.x;
    p->h.y = t0->h.y;
    p->spr.speed = t0->spr.speed;
    if ((t0->f1F4 & 1) && p->w8 == 0 && (t0->h.disp_flags & 8)) {
        if (p->spr.anim == 5)
            obj_set_anim(p, 6);
        else
            obj_set_anim(p, 3);
        p->h.disp_flags |= 4;
        p->w8 = 1;
    }
    if (p->w8 == 0)
        return;
    if (t0->spr.anim == 7)
        p->h.obj_flags |= 4;
}
/* GoalRing_CreatePart_ov000_0230ab48 (spinning ring overlay) */
static void goal_follow(Obj *par) {
    Obj *w = effobj_new(OBJ_BAC_GOALRING);
    if (w == NULL)
        return;
    w->kind = OBJK_GOALPART;
    w->spr.order = 9;
    if (par->h.disp_flags & 1)
        w->h.disp_flags |= 1;
    w->parent = par;
    w->h.x = par->h.x;
    w->h.y = par->h.y;
    w->h.move_flags |= 0x2100;
    w->h.obj_flags |= 2;
    /* GoalRingPart_Action_FollowRing */
    obj_set_anim(w, (u16)(par->spr.anim == 4 ? 5 : 2));
    w->h.disp_flags |= 4;
    w->state = goalpart_state_follow;
}
static void goal_action_idle(Obj *s) {
    obj_set_anim(s, 0);
    s->state = goal_state_idle;
}
static void goal_action_spin(Obj *w, s32 v) {
    s16 a = (s16)((v >> 4) < 0 ? -(v >> 4) : (v >> 4));
    if (a <= 0x10)
        a = 0x10;
    if (a >= 0xc0)
        a = 0xc0;
    obj_set_anim(w, v > 0 ? 1 : 4);
    w->h.disp_flags |= 4;
    w->wC = a;
    w->spr.speed = a;
    w->state = goal_state_spin;
    goal_follow(w);
    w->w8 = 0;
}
static void goal_state_spin(Obj *w) {
    w->w8++;
    if (w->w8 == 0x30)
        w->f1F4 |= 1;
    if (w->f1F4 & 1) {
        w->wC--;
        if (w->wC < 0x10)
            w->wC = 0x10;
        if (w->wC == 0x10 && (w->h.disp_flags & 8)) {
            goal_action_idle(w);
            w->wC = 0x60;
            obj_set_anim(w, 7);
            obj_sfx(0x46);
            goal_sparkle(w, 0, 0, 0x1d0, 0);
            goal_sparkle(w, 0, 0, 0, 0x1d0);
            goal_sparkle(w, 0, 0, -0x1d0, 0);
            goal_sparkle(w, 0, 0, 0, -0x1d0);
            goal_sparkle(w, 0, 0, 0x148, 0x148);
            goal_sparkle(w, 0, 0, 0x148, -0x148);
            goal_sparkle(w, 0, 0, -0x148, -0x148);
            goal_sparkle(w, 0, 0, -0x148, 0x148);
            return;
        }
    }
    w->spr.speed = (s16)w->wC;
    /* NNS_SndPlayerSetTempoRatio: sound module */
    if (!(w->w8 & 0xf))
        goal_sparkle(w, 0xf - (RAND() & 0x1f), 0xf - (RAND() & 0x1f), 0, 0);
}
static void goal_state_idle(Obj *w) {
    w->w8++;
    w->coll[0].zn = -300;
    w->coll[0].zf = 300;
    if (w->spr.anim != 7)
        return;
    if (!(w->w8 & 0xf))
        goal_sparkle(w, 0xf - (RAND() & 0x1f), 0xf - (RAND() & 0x1f), 0, 0);
    if (w->wC == 8 && g_obj_hooks.fade)
        g_obj_hooks.fade(g_obj_hooks.ctx, 4, 0x200);
    if (w->wC == 0) {
        if (g_obj_hooks.stage_exit)
            g_obj_hooks.stage_exit(g_obj_hooks.ctx, W.mode == 1 ? OBJ_EXIT_RESULTS : OBJ_EXIT_CLEAR_DEMO);
        obj_log("stage_exit", W.mode == 1 ? OBJ_EXIT_RESULTS : OBJ_EXIT_CLEAR_DEMO, 0);
        w->wC = -1; /* native: report once (the DS shuts the stage down here) */
    } else if (w->wC > 0) {
        w->wC--;
    }
}

/* the speed bonus shared by the goal and the markers */
static void speed_bonus(const DcPlayer *p) {
    s32 v;
    if (p->pf & 0x80) {
        g_obj_stage.speed_bonus += 10000;
    } else if (p->pf & 0x100) {
        g_obj_stage.speed_bonus += 8000;
    } else {
        v = p->move_x;
        if (v < 0)
            v = -v;
        if (p->run_thr[2] <= v) /* @0x37C */
            g_obj_stage.speed_bonus += 4000;
    }
}
/* StageClear_HandleGoalReached (normal stages) */
static void stage_clear_goal(Obj *a, DcPlayer *p) {
    (void)a;
    speed_bonus(p);
    ++g_obj_stage.bonus_count;
    g_obj_stage.speed_bonus /= g_obj_stage.bonus_count; /* FX_DivS32 */
    g_obj_stage.ring_bonus = p->rings * 10;
    /* gGameState.unk10 == 2: time attack record (not translated) */
    W.flags &= ~1u;
    g_obj_stage.goal_reached = 1;
    if (g_obj_hooks.goal)
        g_obj_hooks.goal(g_obj_hooks.ctx, p); /* Player_Action_ReachGoal */
    g_obj.stats.goals++;
    obj_log("goal", p->x >> 8, p->rings);
}
static void goal_on_hit(ObjBox *a, ObjBox *b) {
    Obj *t0 = (Obj *)a->owner;
    DcPlayer *t3;
    if (t0->h.obj_flags & 2) {
        if (W.mode == 1)
            g_obj_stage.goal_state = 3;
        return;
    }
    t3 = obj_box_player(b);
    if (t3 != NULL) {
        stage_clear_goal(t0, t3);
        goal_action_spin(t0, t3->move_x);
        obj_sfx(0x45);
        t0->coll[0].flags |= 0x1000;
    }
    t0->h.obj_flags |= 2;
}
Obj *goalring_create(ObjParam *p, s32 b, s32 c) {
    Obj *w = mapobj_new(p, b, c, OBJ_BAC_GOALRING);
    if (w == NULL)
        return NULL;
    w->kind = OBJK_GOAL;
    w->spr.order = 0x14;
    if (W.zone == 2 && W.act != 0)
        spr_prio(w, 1);
    rect_set_atkdef(&w->coll[0], 0, (u32)-16);
    w->coll[0].cb = goal_on_hit;
    w->h.move_flags |= 0x2100;
    w->snd = 1;
    goal_action_idle(w);
    return w;
}

/* ============================================================ Marker */
static void marker_state_main(Obj *w) {
    if (w->spr.anim == 0 && (w->prm->flags & 0xff) <= g_obj_stage.check_idx) {
        obj_set_anim(w, 2);
        w->h.disp_flags |= 4;
        w->h.obj_flags |= 2;
    }
    if (w->spr.anim == 1 && (w->h.disp_flags & 8)) {
        obj_set_anim(w, 2);
        w->h.disp_flags |= 4;
    }
}
static void marker_state_clear_fade(Obj *w) {
    if (w->wC == 0x10 && g_obj_hooks.fade)
        g_obj_hooks.fade(g_obj_hooks.ctx, 4, 0x100);
    if (w->wC != 0) {
        w->wC--;
        return;
    }
    if (g_obj_hooks.stage_exit)
        g_obj_hooks.stage_exit(g_obj_hooks.ctx, OBJ_EXIT_CLEAR_DEMO);
    w->wC = -1;
    w->state = NULL;
}
static void marker_action_stage_clear(Obj *w) {
    DcPlayer *p = g_obj.players[0].p;
    w->state = marker_state_clear_fade;
    w->wC = 0x78;
    if (p->chara != 1)
        obj_sfx((RAND() & 1) ? 0x1f : 0x20);
    else
        obj_sfx((RAND() & 1) ? 0x28 : 0x29);
    obj_sfx(0x31);
    stage_clear_goal(w, p);
}
static void marker_state_wait_pass(Obj *w) {
    DcPlayer *p = g_obj.players[0].p;
    if (w->h.disp_flags & 8)
        w->h.disp_flags |= 0x10;
    if (w->h.y + 0x2000 <= p->y)
        return;
    if (w->h.x < p->x && w->h.x >= p->px) {
        w->h.vel_x++;
        if (w->w8 < w->h.vel_x) {
            if (g_obj_hooks.checkpoint_popup)
                g_obj_hooks.checkpoint_popup(g_obj_hooks.ctx, p, W.frame - (u32)w->wC);
            w->wC = (s32)W.frame;
            obj_sfx(0x44);
            w->w8 = w->h.vel_x;
            w->h.disp_flags &= ~0x10;
            if ((u32)w->w8 >= 5) {
                marker_action_stage_clear(w);
                return;
            }
        }
    }
    if (w->h.x > p->x && w->h.x <= p->px)
        w->h.vel_x--;
}
static void marker_on_hit(ObjBox *a, ObjBox *b) {
    Obj *o = (Obj *)a->owner;
    DcPlayer *p = obj_box_player(b);
    s8 h;
    if (p != NULL) {
        h = (s8)(p->box_b + 1);
        obj_set_anim(o, 1);
        g_obj_stage.check_x = o->h.x >> 8;
        g_obj_stage.check_y = (o->h.y >> 8) - h;
        if (o->h.disp_flags & 2)
            g_obj_stage.check_y = h + (o->h.y >> 8);
        g_obj_stage.check_idx = (u8)(o->prm->flags & 0xff);
        /* unk38 = camera bound of the player's camera: camera module */
        g_obj_stage.check_time = W.frame;
        W.check_x = g_obj_stage.check_x;
        W.check_y = g_obj_stage.check_y;
        dcp_add_tension(p, 800);
        speed_bonus(p);
        g_obj_stage.bonus_count++;
        obj_sfx(0x44);
        if (W.mode == 2 && g_obj_hooks.checkpoint_popup)
            g_obj_hooks.checkpoint_popup(g_obj_hooks.ctx, p, W.frame);
        g_obj.stats.checkpoints++;
        obj_log("checkpoint", g_obj_stage.check_x, g_obj_stage.check_y);
    }
    o->h.obj_flags |= 2;
}
Obj *marker_create(ObjParam *a, s32 b, s32 c) {
    Obj *w = mapobj_new(a, b, c, OBJ_BAC_MARKER);
    if (w == NULL)
        return NULL;
    w->kind = OBJK_MARKER;
    w->spr.order = 0x14;
    rect_set_atkdef(&w->coll[0], 0, (u32)-0x10);
    w->coll[0].cb = marker_on_hit;
    w->h.move_flags |= 0x2100;
    if (obj_gravity_flipped(w->h.x, w->h.y))
        w->h.disp_flags |= 2;
    if (W.zone >= 7 && W.zone < 9 && W.act < 2) {
        /* Marker_Action_InitPassCheck */
        obj_set_anim(w, 0);
        w->h.disp_flags |= 4;
        w->h.disp_flags |= 0x10;
        w->h.obj_flags |= 0x10;
        w->h.obj_flags |= 2;
        w->state = marker_state_wait_pass;
        w->w8 = 0;
        w->wC = 0;
        w->h.vel_x = 0;
        /* x < 0x200 px: MarkerTimeList_Create (HUD) */
    } else {
        obj_set_anim(w, 0); /* Marker_Action_Init */
        w->h.disp_flags |= 4;
        w->state = marker_state_main;
    }
    return w;
}

/* ========================================== MapObj40 (invisible triggers) */
/* MapObj40_OnHit: collision-layer switches (0x41/0x42/0x59/0x5a/0x5b), the
 * "launched" flag area (default), score bonus (0x9d), death zone (0x9f). */
static void mapobj40_on_hit(ObjBox *a, ObjBox *b) {
    Obj *w = (Obj *)a->owner;
    DcPlayer *pl = obj_box_player(b);
    s32 v;
    if (w == NULL || pl == NULL)
        return;
    switch (w->prm->type) {
    default:
        if (pl->type != 1)
            return;
        if (!(pl->move_flags & 0x10))
            return;
        pl->pf2 |= 1;
        if (w->prm->flags & 0x10)
            pl->pf2 |= 2;
        return;
    case 0x41:
        pl->obj_flags &= ~1u;
        if (pl->type == 1)
            pl->layer_set = 0;
        return;
    case 0x42:
        pl->obj_flags |= 1;
        if (pl->type == 1)
            pl->layer_set = 0;
        return;
    case 0xc6: /* gSwitchTimers: switch objects not translated (always 0) */
        return;
    case 0x59:
        if (pl->type != 1)
            return;
        if ((pl->layer_ovr & ~0x80) != w->prm->flags + 1 && (pl->anim < 0x1a || pl->anim > 0x20) &&
            !(pl->layer_set & 2)) {
            pl->layer_set = (u8)((pl->obj_flags & 1) | 2);
            pl->layer_ovr = (u8)((w->prm->flags & 0xf) + 1);
            pl->obj_flags &= ~1u;
        }
        pl->pf2 |= 4;
        return;
    case 0x5a:
        if (pl->type != 1 || pl->anim < 0x1a)
            return;
        if (pl->anim <= 0x20)
            pl->obj_flags &= ~1u;
        return;
    case 0x5b:
        if (pl->type != 1 || pl->anim < 0x1a)
            return;
        if (pl->anim <= 0x20)
            pl->obj_flags |= 1;
        return;
    case 0x9d:
        if (pl->type != 1)
            return;
        if (pl->pf & 0x80) {
            g_obj_stage.speed_bonus += 10000;
        } else if (pl->pf & 0x100) {
            g_obj_stage.speed_bonus += 8000;
        } else {
            v = pl->move_x;
            if (v < 0)
                v = -v;
            if (pl->run_thr[2] >= v)
                g_obj_stage.speed_bonus += 4000;
        }
        g_obj_stage.bonus_count++;
        w->h.obj_flags |= 8;
        w->coll[0].flags |= 0x1000;
        w->f1F4 |= 0x10;
        return;
    case 0x9f:
        if (pl->type != 1 || (pl->move_flags & 0x100))
            return;
        dcp_act_die(pl);
        return;
    case 0x7e: /* Player_Action_MapObj40 */
    case 0xa9: /* EffSand_Create */
    case 0xb6: /* Player_Action_GliderSequenceStart */
    case 0xb7: /* Player_Action_GliderSequenceEnd */
    case 0xc3: /* ZoneFx_Zone4_StartBldyRampUp */
    case 0xc4: /* ZoneFx_Zone4_StartBldyRampDown */
    case 0xc5: /* switch timer area */
        obj_log("mapobj40_untranslated", w->prm->type, pl->x >> 8);
        return;
    }
}
Obj *mapobj40_create(ObjParam *p, s32 b, s32 c) {
    Obj *w = obj_alloc(OBJ_PRIO_MAPOBJ, obj_mapobject_dtor);
    if (w == NULL)
        return NULL;
    obj_mapobject_init(w, p, b, c);
    w->kind = OBJK_TRIGGER;
    rect_set_box2d(&w->coll[0], p->p6, p->p7, (s16)(p->p8 + p->p6), (s16)(p->p9 + p->p7));
    rect_set_atkdef(&w->coll[0], 0, (u32)-16);
    w->coll[0].flags |= 0xc0;
    w->coll[0].cb = mapobj40_on_hit;
    w->coll[0].owner = w;
    w->coll[0].owner_kind = OBJ_OWNER_OBJECT;
    w->h.move_flags |= 0x2120;
    switch (w->prm->type) {
    case 0xb7:
        w->h.move_flags |= 0x8000000;
        /* fall through */
    case 0xb6:
        w->h.move_flags &= ~0x2000u;
        rect_set_box2d(&w->coll[0], 0, -0xc8, 0x28, 0xc8);
        break;
    }
    return w;
}
