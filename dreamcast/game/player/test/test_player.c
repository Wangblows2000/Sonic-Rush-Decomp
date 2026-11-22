/*
 * Host-side unit test of the native player physics.
 *
 * Worlds are built as real rlvl_map_t (tools/dreamcast/stages/level_format.h)
 * from a heightfield, so the whole path player -> StageObj_* translations ->
 * rlvl_* map queries is exercised. Expected values are derived from the
 * original constants (physics.bin from the owner's ROM) with the closed-form
 * rules of the decompiled code, independently of the module's state machine.
 *
 * Usage: test_player <build/dreamcast/player> [<build/dreamcast/stages/z11/level.bin>]
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../dcp_player.h"

static int g_fail, g_checks;
#define CHECK(c, ...)                                                   \
    do {                                                                \
        g_checks++;                                                     \
        if (!(c)) {                                                     \
            g_fail++;                                                   \
            printf("  FAIL %s:%d: ", __FILE__, __LINE__);               \
            printf(__VA_ARGS__);                                        \
            printf("\n");                                               \
        }                                                               \
    } while (0)

static void *load(const char *path, long *size) {
    FILE *f = fopen(path, "rb");
    void *b;
    if (!f) {
        printf("cannot open %s\n", path);
        exit(2);
    }
    fseek(f, 0, SEEK_END);
    *size = ftell(f);
    fseek(f, 0, SEEK_SET);
    b = malloc(*size);
    if (fread(b, 1, *size, f) != (size_t)*size)
        exit(2);
    fclose(f);
    return b;
}

/* ------------------------------------------------------------ test hooks */
static int g_rings_spawned;
static void hk_ring(void *c, DcPlayer *p, s32 x, s32 y, s16 vx, s16 vy, u16 fl) {
    (void)c; (void)p; (void)x; (void)y; (void)vx; (void)vy; (void)fl;
    g_rings_spawned++;
}

/* ------------------------------------------------- synthetic rlvl world */
#define WB 128 /* width in blocks (8192 px) */
#define HB 16 /* height in blocks (1024 px) */
typedef s32 (*floor_fn)(s32 x); /* first solid row of column x */

static rlvl_header_t s_hdr;
static u16 s_blocks[WB * HB * 64];
static u16 s_layer[WB * HB];
static s8 s_height[1024 * 8], s_width[1024 * 8];
static u8 s_angle[1024], s_attr[1024];
static rlvl_map_t s_map;

static void build_world(floor_fn F, u8 angle) {
    int nchars = 1, bx, by, cx, cy, i;
    memset(s_height, 0, sizeof s_height);
    memset(s_width, 0, sizeof s_width);
    memset(s_angle, 0, sizeof s_angle);
    memset(s_attr, 0, sizeof s_attr);
    s_hdr.magic = RLVL_MAGIC;
    s_hdr.version = RLVL_VERSION;
    s_hdr.width_blocks = WB;
    s_hdr.height_blocks = HB;
    for (by = 0; by < HB; by++)
        for (bx = 0; bx < WB; bx++) {
            int b = by * WB + bx;
            s_layer[b] = (u16)b;
            for (cy = 0; cy < 8; cy++)
                for (cx = 0; cx < 8; cx++) {
                    s32 tx = bx * 8 + cx, ty = by * 8 + cy;
                    s8 h[8], w[8];
                    int k, id = -1, empty = 1;
                    for (k = 0; k < 8; k++) {
                        s32 c = F(tx * 8 + k) - ty * 8;
                        h[k] = (s8)(c <= 0 ? 8 : (c >= 8 ? 0 : c));
                        if (h[k])
                            empty = 0;
                    }
                    for (k = 0; k < 8; k++) {
                        s32 y = ty * 8 + k, j, first = -1, last = -1, n = 0;
                        for (j = 0; j < 8; j++)
                            if (y >= F(tx * 8 + j)) {
                                if (first < 0)
                                    first = j;
                                last = j;
                                n++;
                            }
                        if (n == 0)
                            w[k] = 0;
                        else if (n == 8)
                            w[k] = 8;
                        else if (last == 7 && n == 8 - first)
                            w[k] = (s8)first;
                        else if (first == 0 && n == last + 1)
                            w[k] = (s8)-n;
                        else
                            w[k] = 8;
                    }
                    if (empty) {
                        id = 0;
                    } else {
                        for (i = 1; i < nchars; i++)
                            if (!memcmp(&s_height[i * 8], h, 8) && !memcmp(&s_width[i * 8], w, 8)) {
                                id = i;
                                break;
                            }
                        if (id < 0) {
                            if (nchars >= 1024) {
                                printf("too many chars\n");
                                exit(2);
                            }
                            id = nchars++;
                            memcpy(&s_height[id * 8], h, 8);
                            memcpy(&s_width[id * 8], w, 8);
                            s_angle[id] = angle;
                        }
                    }
                    s_blocks[b * 64 + cy * 8 + cx] = (u16)id;
                }
        }
    s_map.hdr = &s_hdr;
    s_map.blocks = s_blocks;
    s_map.layer[0] = s_layer;
    s_map.layer[1] = s_layer;
    s_map.height = s_height;
    s_map.width = s_width;
    s_map.angle = s_angle;
    s_map.attr = s_attr;
    s_map.left = 0;
    s_map.top = 0;
    s_map.right = WB * 64;
    s_map.bottom = HB * 64;
    dcp_bind_map(&s_map);
}

#define GROUND 400
#define SLOPE_X0 0
static s32 f_flat(s32 x) { (void)x; return GROUND; }
static s32 f_down(s32 x) { return GROUND + ((x - SLOPE_X0) >> 2); } /* 1:4 descending to the right */
static s32 f_up(s32 x) { return GROUND + 400 - ((x - SLOPE_X0) >> 2); }

/* --------------------------------------------------------------- helpers */
static DcPlayer P;

static void reset_world(void) {
    g_dcp_world.zone = 0;
    g_dcp_world.act = 0;
    g_dcp_world.mode = 0;
    g_dcp_world.flags = 1;
    g_dcp_world.frame = 0;
    g_dcp_world.objmgr = 0x18;
    g_dcp_world.map_left = 0;
    g_dcp_world.map_right = WB * 64;
    g_dcp_world.death_y = 0x7fff;
    g_dcp_world.timescale = 0x100;
    g_dcp_world.wind_x = g_dcp_world.wind_y = 0;
    g_dcp_world.rand_seed = 0;
}

static void frame(u16 held, u16 pressed) {
    static u16 prev;
    dcp_player_frame(&P, held, pressed, (u16)(prev & ~held));
    prev = held;
}

/* drop the player so that its feet (y + box_b) are on row floor-1, settle */
static int spawn_settle(s32 x_px, s32 floor_row) {
    int i;
    dcp_player_create(&P, 0, 0, x_px, floor_row - 1 - 13);
    for (i = 0; i < 10; i++) {
        frame(0, 0);
        if (P.state == dcp_state_ground_idle && (P.move_flags & DCP_MF_GROUND))
            return i + 1;
    }
    return -1;
}

/* reference: one frame of Player_ApplyGroundInput acceleration (no slope bonus
 * unless given), returns new speed; mirrors the formula of the decompiled code */
static s32 ref_accel(s32 v, s32 acc, s32 max, s32 thr) {
    s32 k = 0, a;
    if (abs(v) > thr) {
        k = (s16)(((abs(v) - thr) << 8) / (max - thr));
        if (k > 0x100)
            k = 0x100;
        k = (k * 0xf8 << 8) >> 16;
    }
    a = (s16)(acc - ((acc * k) >> 8));
    v += a;
    if (v > max)
        v = max;
    return v;
}

/* ------------------------------------------------------------------ tests */
static void test_flat_run(void) {
    const DcpPhysics *t = dcp_physics(0);
    s32 v = 0, x0, y0, n, hs = 0, max = t->top, thr = (s16)(t->top * 0.3), mismatch = 0;
    s32 xexp;
    int s;
    printf("[flat] hold right from rest (Sonic)\n");
    reset_world();
    build_world(f_flat, 0);
    s = spawn_settle(1000, GROUND);
    CHECK(s > 0, "settled on the floor (frames %d)", s);
    CHECK(P.y == (GROUND - 14) << 8, "rest y %d.%02x expected %d", P.y >> 8, P.y & 0xff, GROUND - 14);
    x0 = P.x;
    y0 = P.y;
    xexp = x0;
    for (n = 1; n <= 520; n++) {
        frame(DCP_KEY_RIGHT, 0);
        /* reference: Player_UpdateHighSpeedTimer counts frames held at >= top;
         * at hs_time it starts high-speed mode, whose top speed (hs_top) is
         * used from the next ApplyGroundInput call on (max is read before) */
        {
            int start = 0;
            if (hs < t->hs_time && abs(v) >= t->top) {
                hs++;
                if (hs >= t->hs_time)
                    start = 1;
            }
            v = ref_accel(v, t->acc, max, thr);
            if (start)
                max = t->hs_top;
        }
        xexp += v;
        if (n <= 29)
            CHECK(P.gnd_vel == 24 * n, "frame %d gnd %d expected %d", n, P.gnd_vel, 24 * n);
        if (P.gnd_vel != v || P.x != xexp || P.y != y0) {
            if (mismatch < 5)
                printf("  frame %d: gnd %d (exp %d) x %d (exp %d) y %d (exp %d)\n", n, P.gnd_vel, v, P.x, xexp, P.y,
                       y0);
            mismatch++;
        }
        if (n == 1 || n == 29 || n == 60 || n == 120 || n == 200 || n == 400 || n == 520)
            printf("  f%3d gnd=%5d x=%7d.%02x y=%4d ang=%02x anim=%02x pf=%08x\n", n, P.gnd_vel, P.x >> 8, P.x & 0xff,
                   P.y >> 8, P.angle, P.anim, P.pf);
    }
    CHECK(mismatch == 0, "%d frames differ from the reference", mismatch);
    CHECK(P.gnd_vel == t->hs_top, "final speed %d expected hs_top %d", P.gnd_vel, t->hs_top);
    CHECK(P.pf & DCP_PF_HIGHSPEED, "high-speed mode entered");
    CHECK(P.state == dcp_state_ground_move, "still in ground move");

    /* release: friction dec per frame down to 0, then idle */
    {
        s32 f = 0;
        v = P.gnd_vel;
        while (v > 0 && f < 200) {
            frame(0, 0);
            f++;
            v -= t->dec;
            if (v < 0)
                v = 0;
            if (P.gnd_vel != v) {
                CHECK(0, "brake frame %d gnd %d expected %d", f, P.gnd_vel, v);
                break;
            }
        }
        frame(0, 0);
        printf("  released: stopped after %d frames, state idle=%d\n", f, P.state == dcp_state_ground_idle);
        CHECK(P.gnd_vel == 0 && P.state == dcp_state_ground_idle, "stops and returns to idle");
    }
}

static void test_jump(void) {
    const DcpPhysics *t = dcp_physics(0);
    s32 y0, vy, yexp, n, apex = 0, landed = -1;
    printf("[flat] full jump (A held)\n");
    reset_world();
    build_world(f_flat, 0);
    spawn_settle(1000, GROUND);
    y0 = P.y;
    vy = -t->jump; /* Player_Action_Jump on flat: -jump */
    yexp = y0;
    for (n = 1; n <= 200; n++) {
        frame(DCP_KEY_A, n == 1 ? DCP_KEY_A : 0);
        if (landed < 0) {
            /* StageObj_Move adds gravity only when not grounded: in the jump
             * frame the GROUND bit of the last collision pass is still set */
            if (n > 1)
                vy += t->gravity;
            if (vy > t->fall_max)
                vy = t->fall_max;
            yexp += vy;
            if (yexp >= y0) {
                landed = n;
                yexp = y0;
            }
            if (P.y != yexp) {
                CHECK(0, "jump frame %d y %d expected %d (vy %d exp %d)", n, P.y, yexp, P.vel_y, vy);
                break;
            }
            if (P.y < y0 - apex)
                apex = y0 - P.y;
        } else if (n == landed + 1) {
            CHECK(P.state == dcp_state_ground_idle || P.state == dcp_state_ground_move, "landed state");
            break;
        }
    }
    printf("  apex %d.%02x px, airtime %d frames\n", apex >> 8, apex & 0xff, landed);
    {
        /* closed form: rise while vy < 0 */
        s32 v = -t->jump, h = 0;
        h = t->jump; /* first frame without gravity */
        while (1) {
            v += t->gravity;
            if (v >= 0)
                break;
            h -= v;
        }
        CHECK(apex == h, "apex %d expected %d", apex, h);
    }
    CHECK(landed > 0, "landed");
    CHECK(P.y == y0, "back on the floor");

    printf("[flat] short jump (A released after 4 frames)\n");
    spawn_settle(1000, GROUND);
    y0 = P.y;
    vy = -t->jump;
    yexp = y0;
    for (n = 1; n <= 60; n++) {
        u16 held = n <= 4 ? DCP_KEY_A : 0;
        frame(held, n == 1 ? DCP_KEY_A : 0);
        /* reference: InAir cuts the rise to -0x40 when A/B are not held */
        if (n > 1 && !held && vy < -0x40)
            vy = -0x40;
        if (n > 1)
            vy += t->gravity;
        yexp += vy;
        if (yexp >= y0)
            break;
        if (P.y != yexp) {
            CHECK(0, "short jump frame %d y %d expected %d", n, P.y, yexp);
            break;
        }
    }
    printf("  short jump lands after %d frames\n", n);
}

static void test_slope(floor_fn F, u8 ang, const char *name, int foot_dx) {
    const DcpPhysics *t = dcp_physics(0);
    s32 n, v = 0, xexp, x;
    s32 cs = dcp_cos((u16)(ang << 8)), sn = dcp_sin((u16)(ang << 8));
    s32 bonus = (s32)((u32)(t->slope_bonus * sn) << 4) >> 16;
    s32 max = t->top + (bonus > 0 ? bonus : 0);
    s32 thr = (s16)(t->top * 0.3), bad = 0;
    printf("[%s] hold right on a 1:4 slope (angle 0x%02x, cos %d sin %d, top %d)\n", name, ang, cs, sn, max);
    reset_world();
    build_world(F, ang);
    x = 1000;
    dcp_player_create(&P, 0, 0, x, F(x + foot_dx) - 14);
    for (n = 0; n < 10 && !(P.state == dcp_state_ground_idle && (P.move_flags & 1)); n++)
        frame(0, 0);
    CHECK(P.angle == ang, "angle 0x%02x expected 0x%02x", P.angle, ang);
    xexp = P.x;
    for (n = 1; n <= 120; n++) {
        frame(DCP_KEY_RIGHT, 0);
        v = ref_accel(v, t->acc, max, thr);
        xexp += (s16)((v * cs) >> 12);
        if (n <= 29)
            CHECK(P.gnd_vel == 24 * n, "slope frame %d gnd %d expected %d", n, P.gnd_vel, 24 * n);
        {
            s32 want_y = F((P.x >> 8) + foot_dx) - 14;
            if (P.gnd_vel != v || P.x != xexp || (P.y >> 8) != want_y || P.angle != ang || !(P.move_flags & 1)) {
                if (bad < 5)
                    printf("  frame %d: gnd %d/%d x %d/%d y %d/%d ang %02x grounded %d\n", n, P.gnd_vel, v, P.x, xexp,
                           P.y >> 8, want_y, P.angle, P.move_flags & 1);
                bad++;
            }
        }
        if (n == 1 || n == 30 || n == 60 || n == 120)
            printf("  f%3d gnd=%5d x=%5d.%02x y=%4d.%02x ang=%02x\n", n, P.gnd_vel, P.x >> 8, P.x & 0xff, P.y >> 8,
                   P.y & 0xff, P.angle);
    }
    CHECK(bad == 0, "%d slope frames differ (speed, x, surface contact, angle)", bad);
}

static void test_spindash(void) {
    const DcpPhysics *t = dcp_physics(0);
    s32 c, n, want;
    printf("[flat] spin dash: crouch, 1 press, hold 10 frames, release\n");
    reset_world();
    build_world(f_flat, 0);
    spawn_settle(1000, GROUND);
    frame(DCP_KEY_DOWN, 0);
    CHECK(P.state == dcp_state_crouch, "crouching");
    frame(DCP_KEY_DOWN, DCP_KEY_A);
    CHECK(P.state == dcp_state_spindash && P.charge == t->sd_base, "spin dash charge %d", P.charge);
    c = t->sd_base;
    for (n = 0; n < 10; n++) {
        frame(DCP_KEY_DOWN, 0);
        c -= c >> 5; /* ObjSpd_DownSet(charge, charge >> 5) */
    }
    CHECK(P.charge == c, "charge after decay %d expected %d", P.charge, c);
    frame(0, 0);
    want = ((c >> 8) << 7) + 0x800;
    printf("  release speed %d (expected %d), state roll=%d\n", P.gnd_vel, want, P.state == dcp_state_roll);
    CHECK(P.state == dcp_state_roll, "rolling");
    /* the release frame already moved: the roll state starts next frame */
    CHECK(P.gnd_vel == want, "release speed");
    for (n = 0; n < 600 && P.state == dcp_state_roll; n++)
        frame(0, 0);
    printf("  roll lasted %d frames\n", n);
    CHECK(P.state == dcp_state_ground_idle, "roll ends in idle");
}

static void test_hurt_rings(void) {
    printf("[flat] hit with 10 rings, then hit with 0 rings\n");
    reset_world();
    build_world(f_flat, 0);
    spawn_settle(1000, GROUND);
    P.rings = 10;
    g_rings_spawned = 0;
    dcp_on_hurt(&P, 0);
    CHECK(P.rings == 0 && g_rings_spawned == 10, "rings %d spawned %d", P.rings, g_rings_spawned);
    CHECK(P.vel_x == -0x180 && P.vel_y == -0x300, "knockback %d,%d", P.vel_x, P.vel_y);
    CHECK(P.state == dcp_state_hurt && P.invuln == P.invuln_len, "hurt state, invuln %d", P.invuln);
    CHECK(P.tension == 0x640 - 0x4b0, "tension %d", P.tension);
    {
        int n;
        for (n = 0; n < 120 && P.state == dcp_state_hurt; n++)
            frame(0, 0);
        printf("  knockback landed after %d frames\n", n);
        /* LandOnGround keeps the knockback x speed as gnd_vel (|gnd| < |vel_x|),
         * so the idle action hands over to ground move, which applies friction
         * in the same frame */
        CHECK(P.state == dcp_state_ground_move && P.gnd_vel == -0x180 + dcp_physics(0)->dec, "landed into ground move (one friction step), gnd %d", P.gnd_vel);
    }
    P.invuln = 0;
    dcp_on_hurt(&P, 0);
    CHECK(P.state == dcp_state_die && (P.pf & DCP_PF_DEAD), "no rings: death");
    CHECK(P.vel_y == -P.jump && (P.move_flags & DCP_MF_NOCOLLIDE), "death hop");
    {
        int n;
        s32 lives = g_dcp_world.lives;
        for (n = 0; n < 200 && P.state == dcp_state_die; n++)
            frame(0, 0);
        printf("  respawned after %d frames at (%d,%d), lives %d -> %d\n", n, P.x >> 8, P.y >> 8, lives,
               g_dcp_world.lives);
        CHECK(n == 0x3c && g_dcp_world.lives == lives - 1, "respawn after 60 frames, one life used");
    }
}

static void test_boost_spring_trick(void) {
    const DcpPhysics *t = dcp_physics(0);
    s32 tens;
    int n;
    printf("[flat] boost, spring, trick\n");
    reset_world();
    build_world(f_flat, 0);
    spawn_settle(1000, GROUND);
    for (n = 0; n < 10; n++)
        frame(DCP_KEY_RIGHT, 0);
    tens = P.tension;
    frame(DCP_KEY_RIGHT | DCP_KEY_Y, DCP_KEY_Y);
    /* Player_In runs UpdateBoost (start: speed := boost_top, -0x70 tension) and
     * DrainBoostTension (-8) before the state applies boost acceleration */
    CHECK(P.pf & DCP_PF_BOOST, "boosting");
    CHECK(P.tension == tens - 0x70 - 8, "tension %d expected %d", P.tension, tens - 0x70 - 8);
    CHECK(P.gnd_vel == t->boost_top, "boost speed %d expected %d", P.gnd_vel, t->boost_top);
    for (n = 0; n < 20; n++)
        frame(DCP_KEY_RIGHT | DCP_KEY_Y, 0);
    CHECK(P.tension == tens - 0x70 - 8 * 21, "tension drain %d", P.tension);
    frame(DCP_KEY_RIGHT, 0);
    CHECK(!(P.pf & DCP_PF_BOOST), "boost stops on release");

    dcp_act_spring(&P, 0, -0x1000);
    CHECK(P.state == dcp_state_in_air && P.vel_y == -0x1000 && (P.pf & DCP_PF_TRICKABLE), "spring launch");
    frame(0, 0);
    {
        /* trick frame: ApplyAirInput (no direction: clamp to +-max, minus dec)
         * then Player_Action_TrickForward adds (2*jump*sin(0x4000)<<4)>>16 */
        s32 vx0 = P.vel_x, max = (P.pf & DCP_PF_HIGHSPEED) ? t->hs_top : t->top, want;
        if (vx0 > max)
            vx0 = max;
        want = vx0 - t->dec + ((s32)((u32)((t->jump << 1) * 4096) << 4) >> 16);
        frame(DCP_KEY_R, DCP_KEY_R);
        printf("  trick forward: vel_x=%d (expected %d) vel_y=%d anim=%02x\n", P.vel_x, want, P.vel_y, P.anim);
        CHECK(P.anim == 0x37 && P.vel_x == want && P.vel_y == t->gravity, "trick forward impulse");
    }
}

static void test_z11(const char *path) {
    long size;
    void *file = load(path, &size);
    rlvl_map_t m;
    const rlvl_object_t *obj;
    u32 i, nobj;
    s32 sx = -1, sy = -1, n, grounded = 0, maxx = 0;
    if (rlvl_bind(&m, file) != 0) {
        CHECK(0, "rlvl_bind %s", path);
        return;
    }
    obj = (const rlvl_object_t *)rlvl_section(file, RLVL_SEC_OBJECTS, NULL);
    nobj = ((const rlvl_header_t *)file)->num_objects;
    for (i = 0; i < nobj; i++)
        if (obj[i].type == 114) {
            sx = obj[i].x;
            sy = obj[i].y;
        }
    printf("[z11] Leaf Storm act 1: PlayerStart (%d,%d), map %dx%d px, hold right 900 frames\n", sx, sy, m.right,
           m.bottom);
    CHECK(sx >= 0, "PlayerStart object found");
    if (sx < 0)
        return;
    reset_world();
    g_dcp_world.map_right = m.right;
    g_dcp_world.death_y = m.bottom;
    g_dcp_world.check_x = sx;
    g_dcp_world.check_y = sy;
    dcp_bind_map(&m);
    dcp_player_create(&P, 0, 0, sx, sy);
    for (n = 1; n <= 900; n++) {
        frame(n > 30 ? DCP_KEY_RIGHT : 0, 0);
        if (P.move_flags & 1)
            grounded++;
        if (P.x > maxx)
            maxx = P.x;
        if (n % 30 == 0 || n <= 3)
            printf("  f%3d x=%5d.%02x y=%4d.%02x gnd=%5d vx=%5d vy=%5d ang=%02x gr=%d anim=%02x st=%s\n", n, P.x >> 8,
                   P.x & 0xff, P.y >> 8, P.y & 0xff, P.gnd_vel, P.vel_x, P.vel_y, P.angle, P.move_flags & 1, P.anim,
                   P.state == dcp_state_ground_move   ? "move"
                   : P.state == dcp_state_ground_idle ? "idle"
                   : P.state == dcp_state_in_air      ? "air"
                   : P.state == dcp_state_die         ? "die"
                                                      : "other");
    }
    printf("  grounded %d/900 frames, max x %d px, final (%d,%d)\n", grounded, maxx >> 8, P.x >> 8, P.y >> 8);
    CHECK(grounded > 600, "mostly grounded on the real level");
    CHECK((maxx >> 8) > sx + 2000, "travelled right more than 2000 px");
    CHECK(!(P.pf & DCP_PF_DEAD), "did not die");
}

int main(int argc, char **argv) {
    char path[512];
    long size;
    s16 *trig;
    u8 *phys;
    if (argc < 2) {
        printf("usage: %s <build/dreamcast/player> [level.bin]\n", argv[0]);
        return 2;
    }
    snprintf(path, sizeof path, "%s/trig.bin", argv[1]);
    trig = (s16 *)load(path, &size);
    if (size != (8192 + 129) * 2)
        return 2;
    snprintf(path, sizeof path, "%s/physics.bin", argv[1]);
    phys = (u8 *)load(path, &size);
    dcp_set_trig(trig, trig + 8192);
    if (dcp_set_physics(phys, (u32)size) != 2)
        return 2;
    g_dcp_hooks.spawn_ring = hk_ring;

    {
        /* dcp_mul_double vs the host's real double arithmetic, all s16 inputs */
        static const double c[5] = { 0.15, 0.3, 0.5, 0.7, 0.9 };
        const DcpDouble d[5] = { DCP_D_0_15, DCP_D_0_3, DCP_D_0_5, DCP_D_0_7, DCP_D_0_9 };
        int i, bad = 0;
        s32 v;
        for (i = 0; i < 5; i++)
            for (v = -32768; v <= 32767; v++)
                if (dcp_mul_double(v, d[i]) != (s32)(v * c[i]))
                    bad++;
        printf("[math] dcp_mul_double vs host double: %d mismatches over 5 x 65536 inputs\n", bad);
        CHECK(bad == 0, "double emulation");
    }
    test_flat_run();
    test_jump();
    test_slope(f_down, 0x0a, "slope down", -9);
    test_slope(f_up, 0xf6, "slope up", 9);
    test_spindash();
    test_hurt_rings();
    test_boost_spring_trick();
    if (argc > 2)
        test_z11(argv[2]);
    printf("%d checks, %d failed\n", g_checks, g_fail);
    return g_fail ? 1 : 0;
}
