/*
 * sp_input.c - the special stage is played on the DS touch screen; this file
 * turns the Dreamcast controller into the same gTouchState the original reads
 * (Touch_UpdateSingle semantics: 0x10 held, 0x40 pressed this frame, 0x80
 * released, x/y current, startX/startY = press point) and builds gPad[2].
 * The original code is unchanged: it still converts "touch points" to pipe
 * angles, finds the touched gimmick and checks the trick circles.
 *
 * Mapping (docs/dreamcast/translation/SPECIAL.md, section 4):
 *  - steering: the touch point's direction from the pipe centre (128, 45) is
 *    where Sonic goes on the half-pipe (SpecialStage_TouchToPipeAngle).
 *    Analogue stick: the stick direction is that direction (stick down =
 *    bottom of the pipe, left/right = the walls, diagonals in between), a
 *    virtual stylus held 100 px from the centre while the stick is pushed.
 *    D-pad LEFT/RIGHT: the virtual stylus slides along the pipe at 0x500 per
 *    frame (90 degrees in 13 frames) from Sonic's position, held while pressed;
 *    D-pad DOWN: back to the bottom.
 *  - tap on an enemy / bomb (homing attack, SpecialStagePlayer_CheckTouchTarget):
 *    A or B taps the nearest targetable gimmick in reach (the one the cursor
 *    ring of SpecialStageGimmick_Unk_ov001_0230f0f4 marks); nothing targetable =
 *    no tap.
 *  - trick circles (SpecialStageTrick_CheckTouch): flick the stick / press the
 *    D-pad toward the next circle, measured from the previous circle (the
 *    first one from the screen centre): within 45 degrees of the next circle
 *    taps it; within 22.5 degrees of another remaining circle taps that one
 *    (a miss, as a wrong tap on the DS); anything else taps nothing.  Return
 *    to neutral between flicks (a direction counts after 2 stable frames, so
 *    D-pad diagonals register).  A/B do nothing during the trick input.
 *  - "touch the touch screen to continue" (intro): A, B, X, Y or START.
 *  - START: pause (gPad[2] & 8, as the DS START button).
 */
#include <stdlib.h>
#include <string.h>
#include "sp_internal.h"

SpTouch gTouchState;
u16 gPad[3];

static struct {
    int dpad_active;      /* D-pad virtual stylus held */
    s32 dpad_angle;       /* pipe angle -0x4000..0x4000 */
    int flick_armed;
    int flick_dir;        /* 8-way direction index, -1 none */
    int flick_frames;
    int last_dir;
} S;

/* last virtual stylus position (debug / test) */
int g_sp_vtouch_x = -1, g_sp_vtouch_y = -1;

void sp_input_reset(void) {
    memset(&gTouchState, 0, sizeof(gTouchState));
    gTouchState.x = gTouchState.y = 0xffff;
    memset(gPad, 0, sizeof(gPad));
    memset(&S, 0, sizeof(S));
    S.flick_armed = 1;
    S.flick_dir = -1;
    S.last_dir = -1;
}

static u16 ds_bits(u32 b) {
    u16 r = 0;
    if (b & IN_JUMP)
        r |= 1;
    if (b & IN_TRICK)
        r |= 2;
    if (b & IN_START)
        r |= 8;
    if (b & IN_RIGHT)
        r |= 0x10;
    if (b & IN_LEFT)
        r |= 0x20;
    if (b & IN_UP)
        r |= 0x40;
    if (b & IN_DOWN)
        r |= 0x80;
    if (b & IN_BOOST)
        r |= 0x400;
    if (b & IN_ALT)
        r |= 0x800;
    return r;
}

/* stylus point at pipe angle a (0 = bottom, +-0x4000 = walls), 100 px from (128, 45) */
static void pipe_point(s32 a, s32 *x, s32 *y) {
    *x = 128 + (100 * SIN((u32)a & 0xffff)) / 4096;
    *y = 45 + (100 * COS((u32)a & 0xffff)) / 4096;
}

/* 8-way direction of the pad / stick: 0 = right, then counter-clockwise (screen y up); -1 none */
static int read_dir(const InputState *in, float *ang) {
    int dx = 0, dy = 0;
    if (in->ax * in->ax + in->ay * in->ay >= g_input_dir_threshold * g_input_dir_threshold) {
        dx = in->ax;
        dy = -in->ay;
    } else {
        if (in->held & IN_RIGHT)
            dx += 64;
        if (in->held & IN_LEFT)
            dx -= 64;
        if (in->held & IN_UP)
            dy += 64;
        if (in->held & IN_DOWN)
            dy -= 64;
    }
    if (dx == 0 && dy == 0)
        return -1;
    *ang = (float)FX_Atan2(dy * 64, dx * 64) * (360.0f / 65536.0f);
    return ((int)((*ang + 22.5f) / 45.0f)) & 7;
}

static float angdiff(float a, float b) {
    float d = a - b;
    while (d > 180.0f)
        d -= 360.0f;
    while (d < -180.0f)
        d += 360.0f;
    return d < 0 ? -d : d;
}

static float point_angle(s32 fx, s32 fy, s32 tx, s32 ty) {
    s32 dx = tx - fx, dy = fy - ty;   /* screen y down -> up */
    if (dx == 0 && dy == 0)
        return -1000.0f;
    return (float)FX_Atan2(dy << 12, dx << 12) * (360.0f / 65536.0f);
}

static int trick_tap(const InputState *in, s32 *tx, s32 *ty) {
    SpObj *p = SpecialStage_GetPlayer();
    SpTrickSeq *e;
    float ang = 0;
    int dir = read_dir(in, &ang), cur, j;
    s32 px, py;
    if (dir < 0) {
        S.flick_armed = 1;
        S.flick_dir = -1;
        S.flick_frames = 0;
        return 0;
    }
    if (!S.flick_armed)
        return 0;
    if (dir != S.flick_dir) {
        S.flick_dir = dir;
        S.flick_frames = 1;
        return 0;
    }
    if (++S.flick_frames < 2)
        return 0;
    S.flick_armed = 0;
    e = SpecialStageTrick_GetSequence(p->unk150);
    cur = p->unk14C;
    if (cur < 0 || cur >= e->count)
        return 0;
    if (cur > 0) {
        px = e->pts[cur - 1].x;
        py = e->pts[cur - 1].y;
    } else {
        px = 128;
        py = 96;
    }
    if (angdiff(ang, point_angle(px, py, e->pts[cur].x, e->pts[cur].y)) <= 45.0f + 5.0f) {
        *tx = e->pts[cur].x;
        *ty = e->pts[cur].y;
        return 1;
    }
    for (j = cur + 1; j < e->count; j++)
        if (angdiff(ang, point_angle(px, py, e->pts[j].x, e->pts[j].y)) <= 22.5f) {
            *tx = e->pts[j].x;
            *ty = e->pts[j].y;
            return 1;
        }
    return 0;
}

static int attack_tap(s32 *tx, s32 *ty) {
    SpObjList *l;
    SpObj *o, *best = NULL;
    SpObj *pl = SpecialStage_GetPlayer();
    s32 sx, sy;
    if (!gSpecialStageWork->unk1C || !pl)
        return 0;
    l = *gSpecialStageWork->unk1C;
    for (o = l->head.next; o != &l->tail; o = o->next)
        if ((o->unk60 & 9) == 0 && o->unkC4 != NULL && (o->unk60 & 0x100) == 0 && SpecialStageObj_IsInReach(pl, o))
            if (!best || o->pos.z > best->pos.z)
                best = o;
    if (!best)
        return 0;
    NNS_G3dWorldPosToScrPos(&best->pos, &sx, &sy);
    *tx = sx + ((const s32 *)best->unkC4)[0] / 4096;
    *ty = sy + ((const s32 *)best->unkC4)[1] / 4096;
    return 1;
}

void sp_input_update(const InputState *in) {
    SpWork *g = gSpecialStageWork;
    SpObj *p = g ? g->unk18 : NULL;
    s32 x = 0, y = 0;
    int held = 0, tap = 0;
    gPad[0] = ds_bits(in->held);
    gPad[2] = ds_bits(in->pressed);
    if (!g)
        return;
    if (g->taskC == 0) {
        /* intro: "touch the touch screen to continue" */
        if (in->pressed & (IN_JUMP | IN_TRICK | IN_BOOST | IN_ALT | IN_START)) {
            tap = 1;
            x = 128;
            y = 96;
        }
    } else if (p && p->unk228 == 0xa && p->unk128) {
        tap = trick_tap(in, &x, &y);
        S.dpad_active = 0;
    } else if (p) {
        /* homing attack tap */
        if ((in->pressed & (IN_JUMP | IN_TRICK)) && attack_tap(&x, &y))
            tap = 1;
        /* steering */
        if (!tap) {
            if (in->ax * in->ax + in->ay * in->ay >= g_input_dir_threshold * g_input_dir_threshold) {
                {
                    VecFx32 v;
                    s32 m;
                    v.x = in->ax << 12;
                    v.y = in->ay << 12;
                    v.z = 0;
                    m = VEC_Mag(&v) >> 12;
                    if (m < 1)
                        m = 1;
                    x = 128 + (100 * in->ax) / m;
                    y = 45 + (100 * in->ay) / m;
                }
                held = 1;
                S.dpad_active = 0;
            } else if (in->held & (IN_LEFT | IN_RIGHT | IN_DOWN)) {
                if (!S.dpad_active) {
                    S.dpad_active = 1;
                    S.dpad_angle = (s16)p->unk16;
                }
                if (in->held & IN_LEFT)
                    S.dpad_angle -= 0x500;
                if (in->held & IN_RIGHT)
                    S.dpad_angle += 0x500;
                if ((in->held & IN_DOWN) && !(in->held & (IN_LEFT | IN_RIGHT))) {
                    if (S.dpad_angle > 0x500)
                        S.dpad_angle -= 0x500;
                    else if (S.dpad_angle < -0x500)
                        S.dpad_angle += 0x500;
                    else
                        S.dpad_angle = 0;
                }
                if (S.dpad_angle > 0x4000)
                    S.dpad_angle = 0x4000;
                if (S.dpad_angle < -0x4000)
                    S.dpad_angle = -0x4000;
                pipe_point(S.dpad_angle, &x, &y);
                held = 1;
            } else
                S.dpad_active = 0;
        }
    }
    /* Touch_UpdateSingle */
    gTouchState.lastX = gTouchState.x;
    gTouchState.lastY = gTouchState.y;
    gTouchState.flags = (gTouchState.flags & ~0x20u) | (((gTouchState.flags & 0x10) >> 4) << 5);
    if (tap) {
        gTouchState.flags &= ~0x80u;
        gTouchState.flags |= 0x10 | 0x40;
        gTouchState.startX = gTouchState.x = (u16)x;
        gTouchState.startY = gTouchState.y = (u16)y;
        gTouchState.endX = gTouchState.endY = 0xffff;
    } else if (held) {
        if (gTouchState.flags & 0x10) {
            gTouchState.flags &= ~0x40u;
            gTouchState.x = (u16)x;
            gTouchState.y = (u16)y;
        } else {
            gTouchState.flags &= ~0x80u;
            gTouchState.flags |= 0x10 | 0x40;
            gTouchState.startX = gTouchState.x = (u16)x;
            gTouchState.startY = gTouchState.y = (u16)y;
            gTouchState.endX = gTouchState.endY = 0xffff;
        }
    } else if (gTouchState.flags & 0x10) {
        gTouchState.flags &= ~0x50u;
        gTouchState.flags |= 0x80;
        gTouchState.endX = gTouchState.x;
        gTouchState.endY = gTouchState.y;
        gTouchState.x = gTouchState.y = 0xffff;
    } else {
        gTouchState.flags &= ~0x80u;
        gTouchState.x = gTouchState.y = 0xffff;
    }
    g_sp_vtouch_x = (gTouchState.flags & 0x10) ? gTouchState.x : -1;
    g_sp_vtouch_y = (gTouchState.flags & 0x10) ? gTouchState.y : -1;
}
