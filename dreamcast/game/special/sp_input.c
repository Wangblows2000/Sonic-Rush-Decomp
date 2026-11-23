/*
 * sp_input.c - the Dreamcast controller scheme of the special stages
 * (docs/dreamcast/translation/SPECIAL.md, section 4).
 *
 * The DS special stage is played on the touch screen.  The port does not emulate a stylus:
 * the original mechanics take physical inputs directly, through the few places where the
 * original read gTouchState:
 *
 *  - half-pipe steering (SpecialStagePlayer_UpdateTouchSteer): the DS turns the touch point
 *    into a target pipe position r = 0x1e000 * dx / |d| (the horizontal component of the
 *    direction from the pipe centre, SpecialStage_TouchToPipeAngle).  Here the analogue
 *    stick's horizontal axis is that component, from the edge of the special-stage dead zone
 *    (SP_STICK_DZ, at least g_input_deadzone) through an ease-in curve (fine control near the
 *    bottom of the pipe, fast toward the walls) to SP_STICK_EDGE just below the wall; only
 *    full tilt (|x| >= SP_STICK_FULL) is the DS wall target (edge jump when held,
 *    SpecialStagePlayer_State_Run).  The stick target is low-pass filtered (1/4 per frame) so
 *    stick noise and flicks do not throw Sonic across the pipe.  The stick in its centre
 *    targets the bottom of the pipe (every frame, so also after a jump).  D-pad LEFT / RIGHT move the target along the pipe at
 *    SP_DPAD_RATE per frame from Sonic's position (held), D-pad DOWN brings it back to the
 *    bottom; released, the target stays (as when the stylus is lifted).  Sonic's own
 *    movement (turn rate, momentum, collision) is the original.
 *  - homing attack (SpecialStagePlayer_CheckTouchTarget): A attacks the nearest attackable
 *    gimmick (the ones SpecialStageGimmick_Unk_ov001_0230f0f4 marks with the target cursor:
 *    in reach, on screen, not destroyed); nothing attackable = A does nothing.
 *  - Trick Spring targets (SpecialStageTrick_CheckTouch): target n is input n of
 *    1 A, 2 B, 3 X, 4 Y, 5 L, 6 R, 7 Up, 8 Right, 9 Down, 10 Left (the targets show these).
 *    Press events only (a held input does not repeat; the triggers have hysteresis +
 *    release debounce).  The rule is the DS rule for touches: the current target's input
 *    advances; the input of a later target is a wrong touch (MISS); inputs of done targets
 *    or of numbers the sequence does not have touch nothing.  More than one target input in
 *    the same frame is one touch on a wrong target unless it is only the current one.
 *  - intro "PRESS START TO BEGIN" (the DS waits for any touch): START or A.
 *  - START: pause (gPad[2] & 8, the DS START button); pause menu START / A continue,
 *    B goes back (sp_core.c).
 * Context: the Trick Spring inputs exist only while a sequence is open (player state 0xa with
 * the trick flag); then steering and attack are off.  Outside it A attacks and the D-pad
 * steers.  START is never a trick input.
 */
#include <stdlib.h>
#include <string.h>
#include "sp_internal.h"

SpPad g_sp_pad;
u16 gPad[3];

const char *const sp_qte_names[SP_QTE_COUNT] = {"A", "B", "X", "Y", "L", "R", "UP", "RIGHT", "DOWN", "LEFT"};

enum {
    SP_STICK_DZ = 40,      /* special-stage dead zone (the engine's is smaller) */
    SP_STICK_FULL = 124,   /* |x| from here = full tilt (the wall) */
    SP_STICK_EDGE = 0x3a00, /* largest stick target below full tilt (the wall is 0x4000) */
    SP_DPAD_RATE = 0x600   /* pipe angle per frame of a held D-pad direction */
};

static struct {
    int stick;             /* the stick is the steering input (until the D-pad steers) */
    int dpad_active;       /* a D-pad steering target is held */
    s32 dpad_angle;
    s32 stick_angle;       /* filtered stick target */
    u32 prev_dpad;
    SpTrig lt, rt;
} S;

static int s_legend = 1;   /* show the legend at the next intro prompt (first special stage of the session) */

void sp_input_reset(void) {
    memset(&g_sp_pad, 0, sizeof(g_sp_pad));
    memset(gPad, 0, sizeof(gPad));
    memset(&S, 0, sizeof(S));
    S.lt.down = S.rt.down = 1;   /* a trigger held at entry counts after a release */
}

int sp_legend_pending(void) { return s_legend; }
void special_show_legend(int on) { s_legend = on; }

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
    if (b & IN_R)
        r |= 0x100;
    if (b & IN_L)
        r |= 0x200;
    if (b & IN_BOOST)
        r |= 0x400;
    if (b & IN_ALT)
        r |= 0x800;
    return r;
}

int sp_trigger_step(SpTrig *t, int v) {
    if (!t->down) {
        if (v >= SP_TRIG_ON) {
            t->down = 1;
            t->low = 0;
            return 1;
        }
        return 0;
    }
    if (v <= SP_TRIG_OFF) {
        if (++t->low >= 2) {
            t->down = 0;
            t->low = 0;
        }
    } else
        t->low = 0;
    return 0;
}

s32 sp_stick_to_angle(int ax) {
    int m = ax < 0 ? -ax : ax, dz = g_input_deadzone > SP_STICK_DZ ? g_input_deadzone : SP_STICK_DZ;
    s32 t, c, a;
    if (m <= dz)
        return 0;
    if (dz >= SP_STICK_FULL)
        dz = 0;
    if (m >= SP_STICK_FULL) {
        a = 0x4000;   /* the DS wall target */
    } else {
        t = (s32)(m - dz) * 4096 / (SP_STICK_FULL - dz);       /* 0..4096 */
        c = (t * 1536 + (t * t >> 12) * 2560) >> 12;          /* 0.375 t + 0.625 t^2 */
        a = c * SP_STICK_EDGE >> 12;
        if (a < 1)
            a = 1;
    }
    return ax < 0 ? -a : a;
}

/* low-pass filter of the stick target; the wall target and small steps are taken at once */
static s32 sp_stick_filter(s32 target) {
    s32 d = target - S.stick_angle;
    if (target == 0x4000 || target == -0x4000 || (d > -0x100 && d < 0x100))
        S.stick_angle = target;
    else
        S.stick_angle += d / 4;
    return S.stick_angle;
}

s32 sp_qte_step(s32 cur, s32 count, u32 ev) {
    u32 all, later;
    if (count > SP_QTE_COUNT)
        count = SP_QTE_COUNT;
    if (cur < 0 || cur >= count || !ev)
        return cur;
    all = count >= 32 ? 0xffffffffu : (1u << count) - 1;
    later = ev & all & ~((1u << (cur + 1)) - 1);
    if (later)
        return -1;                                   /* a later target first: MISS */
    if (ev & (1u << cur))
        return cur < count - 1 ? cur + 1 : 0x1234;   /* next target / sequence done */
    return cur;
}

/* raw values: the engine patch dreamcast/game/special/patches/engine_input_raw.patch adds the
 * analogue triggers and the D-pad without the stick; without it the triggers are the engine's
 * digital IN_L / IN_R (threshold 128) and the stick also gives D-pad directions */
static void raw_inputs(const InputState *in, int *lt, int *rt, u32 *dpad) {
#ifdef IN_HAS_RAW
    *lt = in->lt;
    *rt = in->rt;
    *dpad = in->dpad & (IN_LEFT | IN_RIGHT | IN_UP | IN_DOWN);
#else
    *lt = (in->held & IN_L) ? 255 : 0;
    *rt = (in->held & IN_R) ? 255 : 0;
    *dpad = in->held & (IN_LEFT | IN_RIGHT | IN_UP | IN_DOWN);
#endif
}

void sp_input_update(const InputState *in) {
    SpWork *g = gSpecialStageWork;
    SpObj *p = g ? g->unk18 : NULL;
    int lt, rt, lpress, rpress;
    u32 dpad, dpress;
    raw_inputs(in, &lt, &rt, &dpad);
    lpress = sp_trigger_step(&S.lt, lt);
    rpress = sp_trigger_step(&S.rt, rt);
    dpress = dpad & ~S.prev_dpad;
    S.prev_dpad = dpad;
    gPad[0] = ds_bits(in->held);
    gPad[2] = ds_bits(in->pressed);
    memset(&g_sp_pad, 0, sizeof(g_sp_pad));
    if (!g)
        return;
    if (g->taskC == 0) {
        /* intro: "PRESS START TO BEGIN" (the original accepts any touch) */
        if (in->pressed & (IN_START | IN_JUMP)) {
            g_sp_pad.begin = 1;
            if (g->timer == 0x5c)
                s_legend = 0;   /* the legend was on screen with the prompt */
        }
        return;
    }
    if (!p)
        return;
    if (p->unk228 == 0xa && p->unk128) {
        /* Trick Spring sequence open: the numbered inputs only */
        u32 e = 0;
        if (in->pressed & IN_JUMP)
            e |= 1u << 0;
        if (in->pressed & IN_TRICK)
            e |= 1u << 1;
        if (in->pressed & IN_BOOST)
            e |= 1u << 2;
        if (in->pressed & IN_ALT)
            e |= 1u << 3;
        if (lpress)
            e |= 1u << 4;
        if (rpress)
            e |= 1u << 5;
        if (dpress & IN_UP)
            e |= 1u << 6;
        if (dpress & IN_RIGHT)
            e |= 1u << 7;
        if (dpress & IN_DOWN)
            e |= 1u << 8;
        if (dpress & IN_LEFT)
            e |= 1u << 9;
        g_sp_pad.qte = e;
        S.dpad_active = 0;
        return;
    }
    /* homing attack */
    if (in->pressed & IN_JUMP)
        g_sp_pad.attack = 1;
    /* steering */
    if (in->ax != 0) {
        g_sp_pad.steer = 1;
        if (!S.stick)
            S.stick_angle = p ? (s16)p->unk16 : 0;   /* start from Sonic's position */
        g_sp_pad.steer_angle = sp_stick_filter(sp_stick_to_angle(in->ax));
        S.stick = 1;
        S.dpad_active = 0;
    } else if (dpad & (IN_LEFT | IN_RIGHT | IN_DOWN)) {
        if (!S.dpad_active) {
            S.dpad_active = 1;
            S.dpad_angle = (s16)p->unk16;
        }
        if ((dpad & IN_LEFT) && !(dpad & IN_RIGHT))
            S.dpad_angle -= SP_DPAD_RATE;
        else if ((dpad & IN_RIGHT) && !(dpad & IN_LEFT))
            S.dpad_angle += SP_DPAD_RATE;
        else if (dpad & IN_DOWN) {
            if (S.dpad_angle > SP_DPAD_RATE)
                S.dpad_angle -= SP_DPAD_RATE;
            else if (S.dpad_angle < -SP_DPAD_RATE)
                S.dpad_angle += SP_DPAD_RATE;
            else
                S.dpad_angle = 0;
        }
        if (S.dpad_angle > 0x4000)
            S.dpad_angle = 0x4000;
        if (S.dpad_angle < -0x4000)
            S.dpad_angle = -0x4000;
        g_sp_pad.steer = 1;
        g_sp_pad.steer_angle = S.dpad_angle;
        S.stick = 0;
    } else {
        S.dpad_active = 0;
        if (S.stick) {   /* the stick in its centre: the bottom of the pipe (a spring-centred stylus) */
            g_sp_pad.steer = 1;
            g_sp_pad.steer_angle = sp_stick_filter(0);
        }
    }
}
