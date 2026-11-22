/*
 * Object movement and map collision core used by the player.
 * Faithful translations of (src/game/Stage/ObjectSystem/):
 *   StageObj_Move, StageObj_Collision, StageObj_CollideMapImpl,
 *   StageObj_CollideMapHorizontal, StageObj_CollideMapVertical (hand
 *   translated from the ARM listing in src/asm_fallback/), StageObj_CheckCeiling,
 *   StageObj_ProbeCollision, StageObj_GetMapProbeFlags,
 *   StageObj_ApplyCollisionAttr, StageObj_HandleRide.
 */
#include "dcp_player.h"

#define TS (g_dcp_world.timescale)

/* ------------------------------------------------------- StageObj_Move */
void dcp_obj_move(DcPlayer *o) {
    s32 dy = 0;
    s32 dx = 0;
    s16 vx = o->push_x;
    s16 vy = o->push_y;

    o->px = o->x;
    o->py = o->y;
    o->pz = o->z;
    if (o->move_flags & 0x10000000) {
        o->push_x = 0;
        o->push_y = 0;
        o->push_z = 0;
    }
    if ((vx != 0 || vy != 0) && o->grav_rot != 0)
        dcp_rotate_vec(&vx, &vy, o->grav_rot);
    if (o->hitstop & 0xf) {
        o->move_x = (s16)((vx * TS) >> 8);
        o->move_y = (s16)((vy * TS) >> 8);
        o->move_z = (s16)((o->push_z * TS) >> 8);
    } else {
        if ((o->move_flags & 1) == 0) {
            if ((o->move_flags & 0x80) && (o->move_flags & 1) == 0)
                o->vel_y += (s16)((o->gravity * TS) >> 8);
            if ((o->move_flags & 0x80) && o->vel_y > o->fall_max)
                o->vel_y = o->fall_max;
        }
        if (o->move_flags & 0x40) {
            if ((o->move_flags & 0x20000) && (o->gnd_vel != 0 || (o->move_flags & 0x40000) == 0) &&
                (u8)(o->angle + o->slope_lim) >= o->slope_lim * 2) {
                if (o->angle != 0)
                    o->gnd_vel = (s16)dcp_spd_up(o->gnd_vel, (o->slope_acc * dcp_sin((u16)(o->angle << 8))) >> 12, o->gnd_max);
                else
                    o->gnd_vel = (s16)dcp_spd_up(o->gnd_vel, (-o->slope_acc * dcp_sin((u16)(o->angle << 8))) >> 12, o->gnd_max);
            }
            if ((o->move_flags & 0x8000) == 0) {
                /* original names: dy = spd*COS (x part), dx = spd*SIN (y part) */
                dy = (s16)((o->gnd_vel * dcp_cos((u16)(o->angle << 8))) >> 12);
                dx = (s16)((o->gnd_vel * dcp_sin((u16)(o->angle << 8))) >> 12);
            }
        }
        if (o->move_flags & 0x8000000) {
            o->move_x = (s16)(((o->vel_x + dy + vx) * TS) >> 8);
            o->move_y = (s16)(((o->vel_y + dx + vy) * TS) >> 8);
        } else {
            o->move_x = (s16)(((o->vel_x + dy + vx + g_dcp_world.wind_x) * TS) >> 8);
            o->move_y = (s16)(((o->vel_y + dx + vy + g_dcp_world.wind_y) * TS) >> 8);
        }
        o->move_z = (s16)(((o->push_z + o->vel_z) * TS) >> 8);
        dcp_rotate_vec(&o->move_x, &o->move_y, o->grav_rot);
    }
    o->push_x = 0;
    o->push_y = 0;
    o->push_z = 0;
    o->x += o->move_x;
    o->y += o->move_y;
    o->z += o->move_z;
}

/* -------------------------------------------------- StageObj_Collision */
void dcp_obj_collision(DcPlayer *o) {
    s32 savePx = o->px;
    s32 savePy = o->py;
    s32 tx = o->x;
    s32 ty = o->y;
    s32 oldx, oldy;
    u16 step = 0x800;
    u16 hit = 0;
    u32 fl = 0;

    o->prev_hit = o->hit;
    o->hit = 0;
    if (o->move_flags & 0x100)
        goto end;
    if (o->move_flags & 0x200000) {
        if (!(o->move_flags & 1))
            step >>= 1;
        if (DCP_ABS(o->x - o->px) > step || DCP_ABS(o->y - o->py) > step) {
            o->x = o->px;
            o->y = o->py;
            for (;;) {
                if (DCP_ABS(o->x - tx) > step) {
                    o->px = o->x;
                    if (tx > o->px)
                        o->x = o->px + step;
                    else
                        o->x = o->px - step;
                } else {
                    o->x = tx;
                }
                if (DCP_ABS(o->y - ty) > step) {
                    o->py = o->y;
                    if (ty > o->py)
                        o->y = o->py + step;
                    else
                        o->y = o->py - step;
                } else {
                    o->y = ty;
                }
                oldx = o->x;
                if (oldx == tx && o->y == ty)
                    break;
                oldy = o->y;
                dcp_collide_map(o);
                if (oldx != o->x)
                    tx = o->x;
                hit |= o->hit;
                fl |= o->move_flags & 0xf;
                if (oldy != o->y)
                    ty = o->y;
            }
        }
    }
    dcp_collide_map(o);
    o->hit = hit | o->hit;
    o->move_flags |= fl;
end:
    o->px = savePx;
    o->py = savePy;
}

/* ---------------------------------------------- StageObj_CollideMapImpl */
void dcp_collide_map(DcPlayer *p) {
    s32 spd;
    p->move_flags &= ~0x400000;
    if (p->move_flags & 1)
        p->move_flags |= 0x400000;
    p->move_flags &= ~0xf;
    p->hit = 0;
    spd = p->move_x;
    if ((p->move_flags & 0x400) == 0)
        dcp_collide_horizontal(p, DCP_FALSE, spd);
    if ((p->move_flags & 0x800) == 0)
        dcp_collide_vertical(p);
    if (p->move_flags & 1)
        p->y &= ~0xff;
    if (p->move_flags & 0x400)
        return;
    dcp_collide_horizontal(p, DCP_TRUE, spd);
}

/* ---------------------------------------------- probes / flags / attrs */
u8 dcp_probe_flags(DcPlayer *p) {
    u8 layer = 0;
    if (!(p->move_flags & 0x100000))
        layer |= 0x80;
    if (p->obj_flags & 1)
        layer |= 1;
    if (p->move_flags & 0x80000)
        layer |= 0x40;
    return layer;
}

s32 dcp_probe_obj(DcPlayer *p, s32 x, s32 y, u32 flags, u32 dir, u8 *angle, u8 *attr) {
    if (g_dcp_col.obj_probe == NULL)
        return 0x18;
    return g_dcp_col.obj_probe(g_dcp_col.obj_ctx, p, x, y, flags, dir, angle, attr);
}

s32 dcp_probe(DcPlayer *p, s32 x, s32 y, u32 flags, u32 dir, u8 *angle, u8 *attr) {
    s32 d = 0x20;
    s32 d2;
    if (!(p->move_flags & 0x1000))
        d = dcp_probe_long(x, y, flags & 0xff, dir & 0xff, angle, attr);
    if (!(p->move_flags & 0x200)) {
        d2 = dcp_probe_obj(p, x, y, flags & 0xff, dir & 0xff, angle, attr);
        if (d > d2)
            d = d2;
    }
    return d;
}

void dcp_apply_attr(DcPlayer *p, u8 attr) {
    u8 a = attr >> 1;
    if (a == 1)
        p->hit |= 1;
    if (a == 3)
        p->hit |= 0x10;
    if (a == 2 || a == 3)
        p->hit |= 8;
}

/* ------------------------------------------------ StageObj_CheckCeiling */
dcp_bool dcp_check_ceiling(DcPlayer *p) {
    s32 d, d2, dx1, dx2, dy1, dy2, x, y;
    u8 dir, q, layer;

    layer = dcp_probe_flags(p);
    x = p->x >> 8;
    y = p->y >> 8;
    if (!(p->move_flags & 0x10))
        q = ((p->angle + 0x20) & 0xc0) >> 6;
    else
        q = 0;
    if (p->grav_rot != 0)
        q = (u8)(q + (u8)(((p->grav_rot + 0x20) & 0xc0) >> 6)) & 3;
    switch (q) {
    case 0:
    default:
        dy1 = p->box_t;
        dx1 = (s8)(p->box_r - 2);
        dir = 3;
        dx2 = (s8)(p->box_l + 2);
        dy2 = dy1;
        break;
    case 1:
        dx1 = p->box_r;
        dir = 0;
        dy1 = (s8)(p->box_l + 2);
        dy2 = (s8)(dx1 - 2);
        dx2 = dx1;
        break;
    case 2:
        dy1 = p->box_b;
        dx1 = (s8)(p->box_r - 2);
        dir = 2;
        dx2 = (s8)(p->box_l + 2);
        dy2 = dy1;
        break;
    case 3:
        dx1 = p->box_l;
        dir = 1;
        dy1 = (s8)(dx1 + 2);
        dx2 = dx1;
        dy2 = (s8)(p->box_r - 2);
        break;
    }
    d = (s8)dcp_probe(p, x + dx1, y + dy1, layer, dir, NULL, NULL);
    d2 = (s8)dcp_probe(p, x + dx2, y + dy2, layer, dir, NULL, NULL);
    if (d >= d2)
        d = d2;
    return d <= 0;
}

/* ---------------------------------------- StageObj_CollideMapHorizontal */
static void set_wall_probe(DcPlayer *p, u8 sel, s32 flip, s32 extra, int second,
                           s8 *dx1, s8 *dy1, s8 *dx2, s8 *dy2, u8 *dir) {
    /* the two switch statements of the original differ only in the sign of
     * the dy values in cases 1 and 3 */
    switch (sel) {
    case 0:
        *dy1 = p->box_r + 2;
        *dx1 = p->box_b - 4;
        *dx2 = p->box_t + 4;
        *dy2 = *dy1;
        *dir = 2;
        break;
    case 1:
        *dx1 = p->box_l - 2;
        if (!second) {
            *dy1 = p->box_b - 4;
            *dy2 = p->box_t + extra;
        } else {
            *dy1 = -(p->box_b - 4);
            *dy2 = -(p->box_t + extra);
        }
        *dx2 = *dx1;
        if (flip) {
            *dy1 = -*dy1;
            *dy2 = -*dy2;
        }
        *dir = 1;
        break;
    case 2:
        *dy1 = p->box_l - 2;
        *dx1 = p->box_b - 4;
        *dir = 3;
        *dy2 = *dy1;
        *dx2 = p->box_t + 4;
        break;
    case 3:
        *dx1 = p->box_r + 2;
        if (!second) {
            *dy1 = -(p->box_b - 4);
            *dy2 = -(p->box_t + extra);
        } else {
            *dy1 = p->box_b - 4;
            *dy2 = p->box_t + extra;
        }
        *dx2 = *dx1;
        if (flip) {
            *dy1 = -*dy1;
            *dy2 = -*dy2;
        }
        *dir = 0;
        break;
    }
}

void dcp_collide_horizontal(DcPlayer *p, dcp_bool noMove, s32 spd) {
    u8 ang, a1, layer, dir = 0, flip;
    s8 dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0;
    s32 d, d2, e, e2, q, extra, x, y;

    layer = dcp_probe_flags(p);
    flip = 0;
    x = p->x >> 8;
    y = p->y >> 8;
    extra = 0;
    a1 = p->angle;
    q = ((a1 + 0x20) & 0xc0) >> 6;
    ang = 0x40;
    if (q == 2)
        ang += 0x80;
    if (spd > 0) {
        ang = -ang;
        flip = 1;
    } else if (!(p->disp_flags & 1)) {
        ang = -ang;
        flip = 1;
    }
    if (p->grav_rot != 0) {
        ang += p->grav_rot;
        if (spd > 0) {
            ang = -ang;
            flip = 0;
        }
    }
    if (q == 2)
        flip ^= 1;
    if (!(p->move_flags & 0x10))
        ang += a1;
    if (dcp_check_ceiling(p))
        extra = 6;

    set_wall_probe(p, ((ang + 0x20) & 0xc0) >> 6, flip, extra, 0, &dx1, &dy1, &dx2, &dy2, &dir);
    d = (s8)dcp_probe(p, x + dx1, y + dy1, layer | 0x80, dir, NULL, NULL);
    d2 = (s8)dcp_probe(p, x + dx2, y + dy2, layer | 0x80, dir, NULL, NULL);
    if (d >= d2)
        d = d2;
    if (d <= 0) {
        if (!noMove) {
            switch (dir) {
            default:
            case 2: y += d; break;
            case 1: x -= d; break;
            case 3: y -= d; break;
            case 0: x += d; break;
            }
        } else {
            p->move_flags |= 4;
            if (!(p->move_flags & 0x4000)) {
                if (dir == 1 && p->move_x < 0) {
                    p->gnd_vel = 0;
                    p->vel_x = 0;
                }
                if (dir == 0 && p->move_x > 0) {
                    p->gnd_vel = 0;
                    p->vel_x = 0;
                }
            }
            if ((dir == 2 || dir == 3) && !(p->move_flags & 0x4000)) {
                p->vel_y = 0;
                p->gnd_vel = 0;
            }
        }
    }

    set_wall_probe(p, ((((u8)(ang + 0x80)) + 0x20) & 0xc0) >> 6, flip, extra, 1, &dx1, &dy1, &dx2, &dy2, &dir);
    e = (s8)dcp_probe(p, x + dx1, y + dy1, (u32)layer | 0x80, dir, NULL, NULL);
    e2 = (s8)dcp_probe(p, x + dx2, y + dy2, (u32)layer | 0x80, dir, NULL, NULL);
    if (e >= e2)
        e = e2;
    if (e <= 0) {
        if (!noMove) {
            switch (dir) {
            default:
            case 2: y += e; break;
            case 1: x -= e; break;
            case 3: y -= e; break;
            case 0: x += e; break;
            }
        } else {
            if (e < 0)
                p->move_flags |= 8;
            if (dir == 1 && spd < 0)
                p->gnd_vel = 0;
            if (dir == 0 && spd > 0)
                p->gnd_vel = 0;
            if (!(p->move_flags & 0x4000)) {
                if (dir == 1 && p->vel_x < 0)
                    p->vel_x = 0;
                if (dir == 0 && p->vel_x > 0)
                    p->vel_x = 0;
            }
        }
    }
    if (!noMove) {
        p->x -= ((p->x >> 8) - x) << 8;
        p->y -= ((p->y >> 8) - y) << 8;
    }
}

/* move (x, y) by d along dir: the recurring jump table of the vertical pass */
static void step_dir(u32 dir, s32 d, s32 *x, s32 *y) {
    switch (dir) {
    case 0: *x += d; break;
    case 1: *x -= d; break;
    case 2: *y += d; break;
    case 3: *y -= d; break;
    }
}

/* ------------------------------------------ StageObj_CollideMapVertical
 * Hand translation of the 0xd60-byte ARM function (src/asm_fallback/
 * StageObj_CollideMapVertical.c). Register / stack slot names are kept in
 * comments so the translation can be checked line by line. */
void dcp_collide_vertical(DcPlayer *p) {
    u8 attr = 0;            /* sp+0x67 */
    s32 saved80;            /* sp+0x18: p->ride saved */
    DcpObjRef *saved_ride;
    u8 layer;               /* r10 */
    s32 x, y;               /* r7, r6 (pixels) */
    u8 ang1, ang2, angt = 0;/* sp+0x64, sp+0x65, sp+0x66 */
    u32 quad;               /* sp+0x14 */
    s32 dx1, dy1, dy2, dx2; /* r11, sp+0x20, sp+0x24, sp+0x1c */
    s32 ofx, ofy;           /* r8, r4 (pre-probe offsets) */
    s32 along, into;        /* sp+0x0c, sp+0x10 */
    u32 dir;                /* r5 */
    s32 d1, d2, d, t;       /* r8, sp+0x58, r4 */
    u32 i, n;

    (void)saved80;
    saved_ride = p->ride;
    layer = dcp_probe_flags(p);
    x = p->x >> 8;
    y = p->y >> 8;
    ang2 = p->angle;
    ang1 = ang2;
    if (p->move_flags & 0x10)
        quad = 0;
    else
        quad = (((p->angle + 0x20) & 0xc0) >> 6) & 0xff;
    if (p->grav_rot != 0)
        quad = ((quad + ((((p->grav_rot + 0x20) & 0xc0) >> 6) & 0xff)) & 0xff) & 3;

    switch (quad) {
    default:
    case 0: /* floor below */
        dx1 = (s8)(p->box_r + 2);
        dy1 = p->box_b;
        dy2 = p->box_b;
        dx2 = (s8)(p->box_l - 2);
        along = p->move_x;
        into = p->move_y;
        ofx = 0;
        ofy = -5;
        dir = 2;
        break;
    case 1: /* floor on the left */
        dx1 = (s8)(-p->box_b);
        dy1 = (s8)(p->box_l - 2);
        dy2 = (s8)(p->box_r + 2);
        ofx = (s8)(p->box_b - DCP_ABS(p->box_r));
        into = (s16)-p->move_x;
        along = (s16)-p->move_y;
        dx2 = dx1;
        ofy = 0;
        dir = 1;
        break;
    case 2: /* floor above (ceiling running) */
        dy1 = (s8)(-p->box_b);
        dy2 = dy1;
        dx1 = (s8)(p->box_r + 2);
        dx2 = (s8)(p->box_l - 2);
        along = (s16)-p->move_x;
        into = (s16)-p->move_y;
        ofx = 0;
        ofy = 5;
        dir = 3;
        break;
    case 3: /* floor on the right */
        dy1 = (s8)(p->box_l - 2);
        dy2 = (s8)(p->box_r + 2);
        ofy = 0;
        ofx = (s8)(-(p->box_b - DCP_ABS(p->box_l)));
        dx1 = p->box_b;
        dx2 = dx1;
        into = p->move_x;
        along = p->move_y;
        dir = 0;
        break;
    }

    /* _270: decide whether jump-through tiles are solid */
    if (!(p->move_flags & 0x400000)) {
        u32 lf = layer & ~0x80u;
        d = (s8)dcp_probe(p, x + dx1 + ofx, y + dy1 + ofy, lf & 0xff, dir, NULL, NULL);
        t = (s8)dcp_probe(p, x + dx2 + ofx, y + dy2 + ofy, lf & 0xff, dir, NULL, NULL);
        if (d >= t)
            d = t;
        if (d >= 0)
            p->move_flags |= 0x100000;
        else
            p->move_flags &= ~0x100000;
    } else {
        if (dir == 3 || dir == 2 || (p->prev_hit & 8))
            p->move_flags |= 0x100000;
        else
            p->move_flags &= ~0x100000;
    }
    /* _35c */
    if (!(p->move_flags & 0x100000))
        layer = (u8)(layer | 0x80);
    p->ride = saved_ride;
    d1 = (s8)dcp_probe(p, x + dx1, y + dy1, layer, dir, &ang1, &attr);
    dcp_apply_attr(p, attr);
    d2 = (s8)dcp_probe(p, x + dx2, y + dy2, layer, dir, &ang2, &attr);
    dcp_apply_attr(p, attr);

    if ((p->move_flags & 0x200000) && !(p->move_flags & 0x200)) {
        /* scan the solid objects between the two feet */
        if (dx1 > dx2) {
            n = (u32)(dx1 - dx2 - 1);
            if ((s32)n > 1) {
                for (i = 1; i < n; i = (i + 1) & 0xff) {
                    t = (s8)dcp_probe_obj(p, x + dx2 + (s32)i, y + dy1, layer, dir, &angt, NULL);
                    if (t < d1) {
                        d1 = t;
                        ang1 = angt;
                    }
                }
            }
        } else {
            n = (u32)(dx2 - dx1 - 1);
            if ((s32)n > 1) {
                for (i = 1; i < n; i = (i + 1) & 0xff) {
                    t = (s8)dcp_probe_obj(p, x + dx1 + (s32)i, y + dy1, layer, dir, &angt, NULL);
                    if (t < d1)
                        d1 = t;
                    ang1 = angt; /* unconditional in the original */
                }
            }
        }
    }
    /* _554: centre probe, only its attribute is used */
    if (dx1 < dx2)
        (void)dcp_probe(p, x + dx1 + ((DCP_ABS(dx2) + DCP_ABS(dx1)) >> 1), y + dy2, layer, dir, NULL, &attr);
    else
        (void)dcp_probe(p, x + dx2 + ((DCP_ABS(dx2) + DCP_ABS(dx1)) >> 1), y + dy2, layer, dir, NULL, &attr);
    dcp_apply_attr(p, attr);

    if ((p->hit & 8) && (p->move_flags & 0x10) && into > 0 && !(p->hit & 1)) {
        u8 a = (u8)(ang1 + p->grav_rot);
        u8 b = (u8)(ang2 + p->grav_rot);
        if ((a >= 0x40 && a <= 0xc0) || (b >= 0x40 && b <= 0xc0)) {
            d1 = 0x18;
            d2 = 0x18;
        }
    }
    /* _668 */
    d = d1 < d2 ? d1 : d2;
    if (d == 0) {
        /* _9e4 */
        if (!(p->move_flags & 0x10) || p->vel_y >= 0)
            p->move_flags |= 1;
    } else if (d < 0) {
        if (!(p->move_flags & 0x10) || into >= 0)
            p->move_flags |= 1;
        /* _6ac */
        if (d >= -14 && (p->move_flags & 1)) {
            if (d == -1 && (p->move_flags & 0x10000)) {
                s32 r0 = dx1, r1 = dy1, s28 = dx2, s2c = dy2;
                if (d1 >= d2)
                    d1 = d2;
                switch (dir) {
                case 2: r1 = (s8)(r1 + d1); s2c = (s8)(s2c + d1); break;
                case 1: r0 = (s8)(r0 - d1); s28 = (s8)(s28 - d1); break;
                case 3: r1 = (s8)(r1 - d1); s2c = (s8)(s2c - d1); break;
                case 0: r0 = (s8)(r0 + d1); s28 = (s8)(s28 + d1); break;
                }
                d1 = (s8)dcp_probe_tile(x + r0, y + r1, layer, dir, NULL, NULL);
                t = (s8)dcp_probe_tile(x + s28, y + s2c, layer, dir, NULL, NULL);
                if (d1 >= t)
                    d1 = t;
                if (d1 != 1) {
                    step_dir(dir, d, &x, &y);
                    p->move_flags &= ~0x10000;
                }
            } else {
                step_dir(dir, d, &x, &y);
                p->move_flags &= ~0x10000;
            }
        }
    } else {
        /* _89c: floor below us at distance d > 0 */
        s32 lim;
        if (d == 1)
            p->move_flags |= 0x10000;
        if (!(p->move_flags & 0x10)) {
            if (quad & 1)
                lim = (s8)((DCP_ABS(into) >> 8) + 3);
            else
                lim = (s8)((DCP_ABS(along) >> 8) + 3);
            if (lim > 11)
                lim = 11;
            if (d > lim) {
                p->move_flags &= ~1;
            } else {
                p->move_flags |= 1;
                step_dir(dir, d, &x, &y);
                if (!(p->move_flags & 0x200) && !(p->move_flags & 0x40) && p->touch == NULL) {
                    (void)dcp_probe_obj(p, x + dx1, y + dy1, layer, dir, NULL, NULL);
                    (void)dcp_probe_obj(p, x + dx2, y + dy2, layer, dir, NULL, NULL);
                }
            }
        }
    }
    /* _a08: grounded -> take the floor angle */
    if (p->move_flags & 1) {
        if (!(p->hit & 1) && (p->move_flags & 0x40)) {
            if (d1 < d2) {
                /* keep ang1 */
            } else if (d1 > d2) {
                ang1 = ang2;
            } else {
                s32 cur = (u8)(p->angle + p->grav_rot);
                s32 e2 = cur - ang2;
                s32 e1 = cur - ang1;
                if (e2 < 0)
                    e2 = -e2;
                if (e1 < 0)
                    e1 = -e1;
                if (e1 > e2)
                    ang1 = ang2;
            }
            if (p->grav_rot != 0)
                ang1 = (u8)(ang1 + p->grav_rot);
            if (!(p->move_flags & 0x800000))
                p->angle = ang1;
            else
                p->angle = dcp_angle_toward8(p->angle, ang1, 1);
        }
        /* _ab0: remove velocity into the surface */
        if (!(p->move_flags & 0x4000)) {
            u32 q2 = (p->move_flags & 0x10) ? 0 : (((p->angle + 0x20) & 0xc0) >> 6) & 0xff;
            switch (q2) {
            case 0:
                if (p->vel_y > 0) {
                    if (p->move_flags & 0x20000)
                        p->vel_x += (s16)((p->vel_y * dcp_sin((u16)(p->angle << 8))) >> 12);
                    p->vel_y = 0;
                }
                break;
            case 1:
                if (p->vel_x < 0)
                    p->vel_x = 0;
                break;
            case 2:
                if (p->vel_y < 0)
                    p->vel_y = 0;
                break;
            case 3:
                if (p->vel_x > 0)
                    p->vel_x = 0;
                break;
            }
        }
    } else {
        p->ride = saved_ride;
    }
    /* _b8c: head probe */
    if (into < 0x100) {
        u32 hdir = 0;
        switch (dir) {
        case 2: hdir = 3; dy1 = dy2 = (s8)(p->box_t + 2); break;
        case 1: hdir = 0; dx1 = dx2 = (s8)(p->box_r - 2); break;
        case 3: hdir = 2; dy1 = dy2 = (s8)(p->box_b - 2); break;
        case 0: hdir = 1; dx1 = dx2 = (s8)(p->box_l + 2); break;
        }
        d = (s8)dcp_probe(p, x + dx1, y + dy1, (layer | 0x80) & 0xff, hdir, NULL, NULL);
        t = (s8)dcp_probe(p, x + dx2, y + dy2, (layer | 0x80) & 0xff, hdir, NULL, NULL);
        if (d >= t)
            d = t;
        if (d <= 0) {
            p->move_flags |= 2;
            if (d >= -14) {
                step_dir(hdir, d, &x, &y);
                if (!(p->move_flags & 0x4000) && into < 0) {
                    if ((u8)(hdir - 2) <= 1)
                        p->vel_y = 0;
                    else
                        p->vel_x = 0;
                }
            }
        }
    }
    /* _d24 */
    p->x -= ((p->x >> 8) - x) << 8;
    p->y -= ((p->y >> 8) - y) << 8;
}

/* ------------------------------------------------- StageObj_HandleRide
 * (only the ride/touch part; the 0x130 link part is a gimmick feature) */
void dcp_handle_ride(DcPlayer *o) {
    s32 v, lo;
    if (o->touch != NULL) {
        if ((o->touch->move_flags & 0x4000000) && (o->move_flags & 0x2000000)) {
            o->push_x += (s16)(o->touch->x - o->touch->px);
            v = o->touch->vx;
            lo = v & 0xff;
            if (lo != 0) {
                if (v > 0)
                    o->push_x += 0x100 - lo;
                else
                    o->push_x -= lo + 0x100;
            }
        }
        if (o->touch->obj_flags & 4)
            o->touch = NULL;
    }
    if (o->ride == NULL)
        return;
    if (o->ride->obj_flags & 4) {
        o->ride = NULL;
        return;
    }
    if (o->ride->move_flags & 0x100) {
        o->push_x += o->ride->vx;
        o->push_y += o->ride->vy;
        o->push_z += o->ride->vz;
    } else {
        o->push_x += (s16)(o->ride->x - o->ride->px);
        o->push_y += (s16)(o->ride->y - o->ride->py);
        o->push_z += (s16)(o->ride->z - o->ride->pz);
    }
    lo = o->ride->vy & 0xff;
    if (lo != 0)
        o->push_y += 0x100 - lo;
}
