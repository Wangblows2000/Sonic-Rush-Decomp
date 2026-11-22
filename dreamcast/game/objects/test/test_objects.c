/*
 * Host test of the native object module on the real Leaf Storm levels (z11, z12).
 *
 * Usage: test_objects <z11/level.bin> <build/dreamcast/player> <objdata dir> <sprites bac dir>
 *
 *  [unit]    ObjRect_FlagCheck attack/defence table of the player protocol,
 *            AnimatorSprite timing on converted frames.bin (durations, loop,
 *            non-loop end), hit box events.
 *  [run]     scripted run from PlayerStart (hold right, jump every 50 frames)
 *            with the real object lists: ring pickups, springs, ... logged.
 *  [targets] the player is put next to the first object of each kind and
 *            the expected interaction is checked (spring launch velocity,
 *            dash panel speed, item box, spikes hurt + scattered rings and
 *            their collection, checkpoint, enemy destroy / hurt, dash ring,
 *            goal).
 *  [v2 targets] objdata v2 kinds: Leaf Storm gimmicks, shared gimmicks,
 *            flapper, Egg Pawn M child, boost knock-away, homing attack,
 *            magnet, decorations (see OBJECTS.md section 7).
 *  [sweep]   runs from spots spread over the whole act; pickups, kills and
 *            gimmick triggers are reported.
 *  [respawn] collected rings stay collected, despawned objects come back.
 * The test runs on z11 and z12 (zone/act from the level header).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../objects.h"
#include "../obj_internal.h"

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

static DcPlayer P;
static rlvl_map_t M;
static u8 *g_level;
static int g_verbose = 1, g_frame;
static s32 g_cam_x, g_cam_y;

#define NEV 64
static struct { char name[24]; int n; } g_ev[NEV];
static int ev_count(const char *ev) {
    int i;
    for (i = 0; i < NEV; i++)
        if (!strcmp(g_ev[i].name, ev))
            return g_ev[i].n;
    return 0;
}
static void ev_add(const char *ev) {
    int i;
    for (i = 0; i < NEV; i++) {
        if (!g_ev[i].name[0])
            strncpy(g_ev[i].name, ev, sizeof(g_ev[i].name) - 1);
        if (!strcmp(g_ev[i].name, ev)) {
            g_ev[i].n++;
            return;
        }
    }
}
static int g_quiet_ev; /* do not print these (camera bounds, decorations) unless -v */
static void hk_log(void *ctx, const char *ev, s32 a, s32 b) {
    (void)ctx;
    ev_add(ev);
    if (g_verbose < 2 && (!strcmp(ev, "camera_bounds") || !strcmp(ev, "bird_fly") || !strcmp(ev, "leaf_burst") ||
                          !strcmp(ev, "freed_animal") || !strcmp(ev, "eggpawn_m_child") || g_quiet_ev))
        return;
    if (!g_verbose)
        return;
    if (!strcmp(ev, "ring") && g_verbose < 2)
        return;
    printf("    f%5d %-16s %6d %6d   player (%d,%d) rings %d\n", g_frame, ev, a, b, P.x >> 8, P.y >> 8, P.rings);
}
static int g_goal_hook, g_exit_hook;
static void hk_goal(void *ctx, DcPlayer *p) { (void)ctx; (void)p; g_goal_hook++; }
static int g_exit_kind = -1;
static void hk_exit(void *ctx, s32 k) { (void)ctx; g_exit_kind = k; g_exit_hook++; }

static void camera_follow(int snap) {
    s32 px = P.x >> 8, py = P.y >> 8;
    s32 tx = px - 128, ty = py - 105;
    if (snap) {
        g_cam_x = tx;
        g_cam_y = ty;
    } else {
        g_cam_x += (tx - g_cam_x) / 4;
        g_cam_y += (ty - g_cam_y) / 5;
    }
    if (g_cam_x > M.right - 256) g_cam_x = M.right - 256;
    if (g_cam_y > M.bottom - 192) g_cam_y = M.bottom - 192;
    if (g_cam_x < 0) g_cam_x = 0;
    if (g_cam_y < 0) g_cam_y = 0;
    objects_set_view(0, g_cam_x << 8, g_cam_y << 8);
}

static u16 s_prev;
static void frame(u16 held) {
    u16 pressed = held & ~s_prev, released = s_prev & ~held;
    s_prev = held;
    objects_frame_begin();
    dcp_player_frame(&P, held, pressed, released);
    objects_update(&P);
    camera_follow(0);
    g_frame++;
}

static void place_player(s32 x, s32 y) {
    s16 rings = P.rings;
    memset(&P, 0, sizeof(P));
    dcp_player_create(&P, 0, 0, x, y);
    P.rings = rings;
    camera_follow(1);
    objects_respawn_view();
}

/* stand the player on the first floor (layer B) at or below y-64 */
static void place_on_floor(s32 x, s32 y) {
    s32 yy;
    for (yy = y - 64; yy < y + 64; yy++)
        if (rlvl_tile_height(&M, yy, x, 1 | 0x80, NULL, NULL) != 0)
            break;
    place_player(x, yy - 14);
}

/* ------------------------------------------------------------------ unit */
static void test_unit(void) {
    ObjSprite s;
    int n, ended = -1, frames_seen[16] = {0}, i;
    printf("[unit] ObjRect_FlagCheck (attacker atk, defender def)\n");
    CHECK(!rect_flag_check(0xa, 0xa), "player body (atk 0xa) does not hit enemy body (def 0xa)");
    CHECK(rect_flag_check(0x13, 0xa), "player attack box (0x13) hits enemy body (0xa)");
    CHECK(rect_flag_check(0x13, 2), "enemy attack box (0x13) hurts player body (def 2)");
    CHECK(!rect_flag_check(0x13, 7), "invulnerable player body (def 2|5) is not hurt");
    CHECK(!rect_flag_check(9, 2), "enemy body (atk 9) does not hurt the player body");
    CHECK(rect_flag_check(0xa, (u32)-0x10), "player body touches gimmicks (def -0x10)");
    CHECK(!rect_flag_check(0x13, (u32)-0x10), "player attack box does not trigger gimmicks");
    CHECK(rect_flag_check(0x53, 2), "spikes (0x53) hurt the player body");
    CHECK(rect_flag_check(0x1f, 0xa), "invincible body (0xa|0x15) destroys enemies");

    printf("[unit] AnimatorSprite on converted frames.bin\n");
    /* spring anim 1: durations 2,4,2,4,2, non-looping */
    spr_init(&s, &g_obj.res[OBJ_BAC_SPRING], 0, 0);
    spr_set_anim(&s, 1);
    for (n = 1; n <= 30; n++) {
        spr_process(&s, NULL, NULL);
        if (s.frame_no < 16)
            frames_seen[s.frame_no]++;
        if ((s.flags & SPR_ENDED) && ended < 0)
            ended = n;
    }
    printf("  spring anim 1: frame calls 1..5 = %d %d %d %d %d (last held), end flag at call %d\n", frames_seen[1],
           frames_seen[2], frames_seen[3], frames_seen[4], frames_seen[5], ended);
    CHECK(frames_seen[1] == 2 && frames_seen[2] == 4 && frames_seen[3] == 2 && frames_seen[4] == 4, "durations 2,4,2,4");
    CHECK(ended == 15 && (s.flags & SPR_STOPPED) && s.frame_no == 5, "non-looping: ends after 14 frames, stops on frame 5");
    /* ring anim 0: 8 x 4 frames, looping */
    spr_init(&s, &g_obj.res[OBJ_BAC_RING], 0, SPR_LOOP);
    for (n = 1, ended = -1, i = 0; n <= 70; n++) {
        spr_process(&s, NULL, NULL);
        if (s.flags & SPR_ENDED) {
            if (ended < 0) ended = n;
            i++;
        }
    }
    printf("  ring anim 0 (loop): first wrap at call %d, %d wraps in 70 calls, frame %d\n", ended, i, s.frame_no);
    CHECK(ended == 33 && i == 2 && !(s.flags & SPR_STOPPED), "loop every 32 frames");
    /* hit boxes: eggpawn anim 0 frame 1 has boxes in slot 0 and 1 */
    {
        const ObjRes *r = &g_obj.res[OBJ_BAC_EGGPAWN];
        CHECK(r->hframes != NULL && r->hframes[0].count == 2 && r->hevents[0].slot == 0 && r->hevents[0].l == -16 &&
                  r->hevents[0].t == -36 && r->hevents[1].slot == 1,
              "eggpawn hit boxes loaded from .hbx");
        r = &g_obj.res[OBJ_BAC_PLAYER];
        CHECK(r->hframes != NULL && r->n_anims == 37, "son.bac timing + boxes (37 anims)");
    }
}

/* -------------------------------------------------------------- the run */
static void test_run(int frames) {
    const ObjStats *st = objects_stats();
    int n, maxx = 0;
    printf("[run] PlayerStart, hold right, jump every 50 frames, %d frames\n", frames);
    for (n = 0; n < frames; n++) {
        u16 k = DCP_KEY_RIGHT;
        if (n % 50 >= 40 && n % 50 < 48)
            k |= DCP_KEY_A;
        frame(k);
        if ((P.x >> 8) > maxx)
            maxx = P.x >> 8;
    }
    printf("  max x %d, rings %d, live objects %u, spawned %u, despawned %u, destroyed %u\n", maxx, P.rings, st->live,
           st->spawned, st->despawned, st->destroyed);
    printf("  rings placed %u collected %u scattered %u (re-collected %u); springs %u dash panels %u dash rings %u item boxes %u hurts %u enemies %u checkpoints %u goals %u\n",
           st->rings_placed, st->rings_collected, st->rings_scattered, st->rings_scatter_collected, st->springs,
           st->dash_panels, st->dash_rings, st->item_boxes, st->hurts, st->enemies_destroyed, st->checkpoints,
           st->goals);
    printf("  gimmick triggers %u (grinds %u, walls %u, rocks %u), decorations spawned %u; events:", st->gimmicks,
           st->grinds, st->walls_broken, st->rocks_broken, st->dec_spawned);
    {
        int i;
        for (i = 0; i < NEV && g_ev[i].name[0]; i++)
            printf(" %s=%d", g_ev[i].name, g_ev[i].n);
        printf("\n");
    }
    CHECK(st->spawned > 0 && st->rings_placed > 0, "objects and rings spawned along the run");
    CHECK(st->rings_collected > 0, "rings collected along the run");
}

static const rlvl_object_t *find_obj(u16 type, int nth) {
    const rlvl_header_t *h = (const rlvl_header_t *)g_level;
    const rlvl_object_t *o = (const rlvl_object_t *)rlvl_section(g_level, RLVL_SEC_OBJECTS, NULL);
    u32 i;
    for (i = 0; i < h->num_objects; i++)
        if (o[i].type == type && nth-- == 0)
            return &o[i];
    return NULL;
}

static int run_until(int max, u16 keys, const u32 *counter, u32 start) {
    int n;
    for (n = 0; n < max; n++) {
        frame(keys);
        if (*counter != start)
            return n + 1;
    }
    return -1;
}

static int try_pawn_jump(const rlvl_object_t *o) {
    const ObjStats *st = objects_stats();
    int k;
    u32 e0 = st->enemies_destroyed, h0 = st->hurts;
    place_on_floor(o->x - 90, o->y);
    for (k = 0; k < 20; k++)
        frame(0);
    for (k = 0; k < 160 && st->enemies_destroyed == e0 && st->hurts == h0; k++) {
        Obj *q;
        s32 dx = 9999;
        for (q = g_obj.head; q; q = q->next)
            if (q->kind == OBJK_PAWN && DCP_ABS((q->h.x >> 8) - (P.x >> 8)) < DCP_ABS(dx))
                dx = (q->h.x >> 8) - (P.x >> 8);
        /* run right, jump when the pawn is 48 px ahead */
        frame((dx < 48 && dx > 0 && (P.move_flags & 1)) || !(P.move_flags & 1) ? (DCP_KEY_RIGHT | DCP_KEY_A) : DCP_KEY_RIGHT);
    }
    printf("  egg pawn at (%d,%d): after %d frames destroyed %u hurt %u, player vel %d,%d\n", o->x, o->y, k,
           st->enemies_destroyed - e0, st->hurts - h0, P.vel_x, P.vel_y);
    return st->enemies_destroyed > e0 && st->hurts == h0;
}

static void test_targets(void) {
    const ObjStats *st = objects_stats();
    const rlvl_object_t *o;
    int n;
    u32 c;
    s16 r0;
    printf("[targets]\n");

    /* spring 0x43 (up, p6 = 0): drop onto it -> Player_Action_SpringLaunch(0, -0x780) */
    o = find_obj(0x43, 0);
    CHECK(o != NULL, "spring 0x43 in z11");
    if (o) {
        place_player(o->x, o->y - 60);
        c = st->springs;
        n = run_until(120, 0, &st->springs, c);
        printf("  spring 0x43 at (%d,%d): triggered after %d frames, vel %d,%d, anim %d\n", o->x, o->y, n, P.vel_x,
               P.vel_y, P.anim);
        CHECK(n > 0, "spring triggered");
        CHECK(P.vel_x == 0 && P.vel_y < -0x600, "spring launch velocity up (vy %d)", P.vel_y);
    }
    /* sideways spring 0x45/0x46 */
    o = find_obj(0x46, 0);
    if (o) {
        place_on_floor(o->x + 40, o->y);
        for (n = 0; n < 20; n++) frame(0);
        c = st->springs;
        n = run_until(200, DCP_KEY_LEFT, &st->springs, c);
        printf("  spring 0x46 at (%d,%d): triggered after %d frames, vel %d,%d\n", o->x, o->y, n, P.vel_x, P.vel_y);
        CHECK(n > 0 && P.vel_x > 0 && P.vel_y == 0, "side spring triggered, launches right");
    }
    /* dash panel 0x5f */
    o = find_obj(0x5f, 0);
    CHECK(o != NULL, "dash panel 0x5f in z11");
    if (o) {
        place_on_floor(o->x - 2, o->y);
        c = st->dash_panels;
        n = run_until(200, 0, &st->dash_panels, c);
        printf("  dash panel 0x5f at (%d,%d): after %d frames gnd %d (top %d) dash timer %d\n", o->x, o->y, n,
               P.gnd_vel, P.top, P.dash_timer);
        CHECK(n > 0, "dash panel triggered");
        CHECK(n > 0 && P.dash_timer > 0 && DCP_ABS(P.gnd_vel) >= P.top, "dash panel speed");
    }
    /* dash ring 0x6b */
    o = find_obj(0x6b, 0);
    if (o) {
        place_player(o->x, o->y - 40);
        c = st->dash_rings;
        n = run_until(120, 0, &st->dash_rings, c);
        printf("  dash ring 0x6b at (%d,%d): after %d frames vel %d,%d\n", o->x, o->y, n, P.vel_x, P.vel_y);
        CHECK(n > 0, "dash ring triggered");
    }
    /* item box 0x30 (p6 = 1: 5 rings) */
    o = find_obj(0x30, 0);
    CHECK(o != NULL, "item box 0x30 in z11");
    if (o) {
        place_on_floor(o->x - 50, o->y);
        for (n = 0; n < 30; n++) frame(0);
        r0 = P.rings;
        c = st->item_boxes;
        n = run_until(200, DCP_KEY_RIGHT, &st->item_boxes, c);
        printf("  item box 0x30 (item %d) at (%d,%d): broken after %d frames, rings %d -> %d\n", o->p0, o->x, o->y, n,
               r0, P.rings);
        CHECK(n > 0, "item box broken");
        if (o->p0 == 1)
            CHECK(P.rings >= r0 + 5, "item 1 gives 5 rings");
    }
    /* spikes 0x4d: land on them with rings -> hurt, rings scattered, collect some back */
    o = find_obj(0x4d, 0);
    CHECK(o != NULL, "needle 0x4d in z11");
    if (o) {
        u32 sc0 = st->rings_scattered, rc0 = st->rings_scatter_collected;
        P.rings = 10;
        place_player(o->x, o->y - 60);
        P.rings = 10;
        c = st->hurts;
        n = run_until(120, 0, &st->hurts, c);
        printf("  needle 0x4d at (%d,%d): hurt after %d frames, rings now %d, scattered %u, state hurt %d, ride %s\n",
               o->x, o->y, n, P.rings, st->rings_scattered - sc0, P.state == dcp_state_hurt,
               P.ride ? "needle" : "-");
        CHECK(n > 0, "spikes hurt the player");
        CHECK(st->rings_scattered - sc0 == 10 && P.rings == 0, "10 rings scattered");
        for (n = 0; n < 300; n++)
            frame(n < 150 ? 0 : DCP_KEY_LEFT);
        printf("  scattered rings re-collected: %u, ring parts alive %d, scatter level %d\n",
               st->rings_scatter_collected - rc0, objects_ring_count(), *objects_ring_scatter_level());
        CHECK(objects_ring_count() >= 0, "ring manager alive");
    }
    /* checkpoint 0x6e */
    o = find_obj(0x6e, 0);
    CHECK(o != NULL, "marker 0x6e in z11");
    if (o) {
        place_on_floor(o->x - 50, o->y);
        for (n = 0; n < 20; n++) frame(0);
        c = st->checkpoints;
        n = run_until(200, DCP_KEY_RIGHT, &st->checkpoints, c);
        printf("  marker 0x6e (index %d) at (%d,%d): passed after %d frames, respawn point (%d,%d)\n", o->flags & 0xff,
               o->x, o->y, n, g_dcp_world.check_x, g_dcp_world.check_y);
        CHECK(n > 0 && g_dcp_world.check_x == o->x, "checkpoint stored");
    }
    /* Egg Pawn 0x00: jump onto it -> destroyed, player bounces (first pawn
     * that can be reached by running at it from 90 px) */
    {
        int tries;
        u32 e00 = st->enemies_destroyed;
        for (tries = 1; tries < 8; tries++) {
            o = find_obj(0x00, tries);
            if (o == NULL)
                break;
            if (try_pawn_jump(o))
                break;
        }
        CHECK(o != NULL && st->enemies_destroyed > e00, "a jumping player destroys an egg pawn");
        CHECK(P.vel_y < 0, "player bounces off");
    }
    /* Egg Pawn walking into the player (no attack) -> hurt */
    o = find_obj(0x00, 3);
    if (o) {
        int k;
        u32 e0 = st->enemies_destroyed, h0 = st->hurts;
        P.rings = 3;
        place_on_floor(o->x - 40, o->y);
        P.rings = 3;
        P.invuln = 0;
        for (k = 0; k < 400 && st->enemies_destroyed == e0 && st->hurts == h0; k++)
            frame(0);
        printf("  egg pawn at (%d,%d), player standing: after %d frames destroyed %u hurt %u\n", o->x, o->y, k,
               st->enemies_destroyed - e0, st->hurts - h0);
        CHECK(st->hurts > h0 && st->enemies_destroyed == e0, "standing player is hurt by the enemy");
    }
    /* goal ring 0x71 */
    o = find_obj(0x71, 0);
    CHECK(o != NULL, "goal ring in z11");
    if (o) {
        place_on_floor(o->x - 60, o->y);
        for (n = 0; n < 20; n++) frame(0);
        c = st->goals;
        n = run_until(300, DCP_KEY_RIGHT, &st->goals, c);
        printf("  goal 0x71 at (%d,%d): reached after %d frames, ring bonus %d, speed bonus %d\n", o->x, o->y, n,
               g_obj_stage.ring_bonus, g_obj_stage.speed_bonus);
        CHECK(n > 0 && g_goal_hook == 1, "goal reached once");
        for (n = 0; n < 600 && !g_exit_hook; n++)
            frame(0);
        printf("  stage exit requested after %d more frames\n", n);
        CHECK(g_exit_hook == 1, "goal ring spin -> stage exit");
    }
}


/* ------------------------------------------------------ objdata v2 kinds */
static Obj *find_live(u16 type, s32 near_x) {
    Obj *q, *best = NULL;
    s32 bd = 0x7fffffff;
    for (q = g_obj.head; q; q = q->next)
        if (q->used && q->prm && !q->is_dec && q->prm->type == type && DCP_ABS((q->h.x >> 8) - near_x) < bd) {
            bd = DCP_ABS((q->h.x >> 8) - near_x);
            best = q;
        }
    return best;
}
static int count_kind_type(int dec, u16 lo, u16 hi) {
    Obj *q;
    int n = 0;
    for (q = g_obj.head; q; q = q->next)
        if (q->used && q->prm && q->is_dec == dec && q->prm->type >= lo && q->prm->type <= hi)
            n++;
    return n;
}
/* run with keys until the event count grows; returns frames or -1 */
static int run_ev(int max, u16 keys, const char *ev) {
    int n, c = ev_count(ev);
    for (n = 0; n < max; n++) {
        frame(keys);
        if (ev_count(ev) != c)
            return n + 1;
    }
    return -1;
}
/* approach object o on the floor from dist px (sign = side), holding dir (+ extra keys) */
static int approach(const rlvl_object_t *o, s32 dist, u16 extra, int max, const char *ev) {
    int n;
    u16 dir = dist > 0 ? DCP_KEY_LEFT : DCP_KEY_RIGHT;
    place_on_floor(o->x + dist, o->y);
    for (n = 0; n < 10; n++)
        frame(0);
    return run_ev(max, dir | extra, ev);
}
static int g_v2_ok, g_v2_n;
static void v2_result(const char *what, int ok, const char *fmt, ...);
#include <stdarg.h>
static void v2_result(const char *what, int ok, const char *fmt, ...) {
    va_list ap;
    printf("  %-22s %s  ", what, ok ? "ok  " : "MISS");
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    printf("\n");
    g_v2_n++;
    g_v2_ok += ok;
}

static void test_v2(void) {
    const rlvl_header_t *h = (const rlvl_header_t *)g_level;
    const ObjStats *st = objects_stats();
    const rlvl_object_t *o;
    int n, i, k;
    u32 c;
    printf("[v2 targets] stage %d (zone %d act %d)\n", h->stage, h->zone, h->act);

    /* Trampoline 0x55: drop on it -> SpringLaunch(0, v), -0xc00 <= v <= -0x780 */
    o = find_obj(0x55, 0);
    if (o) {
        place_player(o->x, o->y - 70);
        n = run_ev(120, 0, "trampoline");
        /* (one frame of gravity, +0x2a, is applied after the launch in the same frame) */
        v2_result("trampoline 0x55", n > 0 && P.vel_y <= -0x780 + 0x2a && P.vel_y >= -0xc00,
                  "(%d,%d) after %d frames vel_y %d (+0x2a gravity)", o->x, o->y, n, P.vel_y);
        CHECK(n > 0 && P.vel_y <= -0x780 + 0x2a && P.vel_y >= -0xc00, "trampoline bounce velocity");
    }
    /* Bend stick 0x56: stand on it -> Player_Action_BendStick, launched after t frames */
    for (i = 0, n = -1; n < 0 && (o = find_obj(0x56, i)) != NULL && i < 4; i++) {
        place_player(o->x + 8, o->y - 40);
        n = run_ev(120, 0, "bend_stick");
    }
    if (o) {
        s16 a0 = P.anim;
        for (k = 0; k < 40 && P.anim != 0x30; k++)
            frame(0);
        v2_result("bend stick 0x56", n > 0 && a0 == 0x31 && P.anim == 0x30 && P.vel_y < 0,
                  "(%d,%d) after %d frames anim %#x, launch after %d more: anim %#x vel %d,%d", o->x, o->y, n, a0, k, P.anim,
                  P.vel_x, P.vel_y);
        CHECK(n > 0 && P.vel_y < 0, "bend stick launch");
    }
    /* Rope 0x58: the ropes hang at the end of wall runs; the player is put in
     * the grab box with ground contact and speed (Rope_OnHit conditions) */
    o = find_obj(0x58, 0);
    if (o) {
        int c0 = ev_count("rope");
        s16 a0 = -1;
        place_player(o->x + o->p0 + o->p2 / 2, o->y + o->p1 + o->p3 / 2);
        for (n = 0; n < 10 && ev_count("rope") == c0; n++) {
            P.x = (o->x + o->p0 + o->p2 / 2) << 8;
            P.y = (o->y + o->p1 + o->p3 / 2) << 8;
            P.move_flags &= ~0x10u;
            P.gnd_vel = 0x700;
            P.angle = 0;
            frame(0);
        }
        a0 = P.anim;
        for (k = 0; k < 120 && P.gimmick != NULL; k++)
            frame(DCP_KEY_RIGHT);
        v2_result("rope 0x58", ev_count("rope") > c0 && a0 == 0x24,
                  "(%d,%d) caught after %d frames (anim %#x), released after %d: vel %d,%d", o->x, o->y, n, a0, k, P.vel_x,
                  P.vel_y);
        CHECK(ev_count("rope") > c0, "rope caught the player");
    }
    /* Jump pedestal 0x67: run over it -> Player_Action_PedestalLaunch */
    for (i = 0, n = -1; n < 0 && (o = find_obj(0x67, i)) != NULL && i < 6; i++) {
        n = approach(o, (o->flags & 4) ? 300 : -300, 0, 240, "pedestal");
        if (n < 0)
            n = approach(o, (o->flags & 4) ? -300 : 300, 0, 240, "pedestal");
    }
    if (o) {
        v2_result("jump pedestal 0x67", n > 0 && P.vel_y < 0, "(%d,%d) flags %#x after %d frames vel %d,%d", o->x, o->y,
                  o->flags, n, P.vel_x, P.vel_y);
        CHECK(n > 0, "jump pedestal launch");
    }
    /* Corkscrew 0x68/0x69: run through at speed -> Player_Action_CorkscrewPath */
    for (i = 0, n = -1; n < 0 && i < 20; i++) {
        o = find_obj(i & 1 ? 0x69 : 0x68, i >> 1);
        if (!o)
            continue;
        P.tension = 0x12c0;
        n = approach(o, (o->flags & 1) ? 400 : -400, DCP_KEY_X, 260, "corkscrew");
    }
    if (o) {
        int moved = P.x;
        for (k = 0; k < 60; k++)
            frame((o->flags & 1) ? DCP_KEY_LEFT : DCP_KEY_RIGHT);
        v2_result("corkscrew 0x68/0x69", n > 0, "(%d,%d) flags %#x after %d frames, then 60 frames: moved %d px", o->x,
                  o->y, o->flags, n, (P.x - moved) >> 8);
        CHECK(n > 0, "corkscrew path entered");
    }
    /* Rock break 0x6f: fall onto it -> broken, player bounces */
    o = find_obj(0x6f, 0);
    if (o) {
        c = st->rocks_broken;
        place_player(o->x, o->y - 70);
        dcp_act_jump(&P); /* jump state: attack box on */
        P.vel_y = 0x100;
        n = run_until(120, 0, &st->rocks_broken, c);
        v2_result("rock break 0x6f", n > 0, "(%d,%d) after %d frames, player vel_y %d", o->x, o->y, n, P.vel_y);
        CHECK(n > 0, "rock broken");
    }
    /* Wall break 0x77: run into it (fast enough for its mode) */
    for (i = 0, n = -1; n < 0 && (o = find_obj(0x77, i)) != NULL && i < 8; i++) {
        P.tension = 0x12c0;
        n = approach(o, -360, DCP_KEY_X, 200, "wall_break");
        if (n < 0) {
            P.tension = 0x12c0;
            n = approach(o, 360, DCP_KEY_X, 200, "wall_break");
        }
    }
    if (o) {
        Obj *wb = find_live(0x77, o->x);
        for (k = 0; k < 20; k++)
            frame(0);
        v2_result("wall break 0x77", n > 0, "(%d,%d) mode %d after %d frames; 20 frames later piece 0 at %+d,%+d px",
                  o->x, o->y, o->flags & 3, n, wb ? wb->k.brk.pos[0][0] >> 8 : 0, wb ? wb->k.brk.pos[0][1] >> 8 : 0);
        CHECK(n > 0, "wall broken");
    }
    /* RopeC 0x7a: touch its grab box -> Player_State_RopeC swing, jump to release */
    o = find_obj(0x7a, 0);
    if (o) {
        s32 bx = (o->flags & 1) ? 0x88 : -0x84, by = 0x44;
        place_player(o->x + bx, o->y + by);
        n = run_ev(30, 0, "rope_c");
        for (k = 0; k < 40; k++)
            frame(0);
        {
            s16 a0 = P.anim;
            s32 len = P.timer;
            frame(DCP_KEY_A);
            v2_result("rope C 0x7a", n > 0 && a0 == 0x25, "(%d,%d) after %d frames anim %#x, rope length %#x after 40 "
                      "frames, released by A: gimmick %s anim %#x", o->x, o->y, n, a0, len, P.gimmick ? "set" : "NULL",
                      P.anim);
        }
        CHECK(n > 0, "rope C grabbed");
        {
            ObjModelView mv;
            int nm = objects_model_count();
            int ok = nm > 0 && objects_model_get(0, &mv);
            printf("    model views %d: %s scale %#x %#x %#x visible %d\n", nm, ok ? mv.model : "-", ok ? mv.scale[0] : 0,
                   ok ? mv.scale[1] : 0, ok ? mv.scale[2] : 0, ok ? mv.visible : 0);
        }
    }
    /* Special ring 0x9e: boost into it -> spin, boosted 0xc0 frames -> enter (special stage exit) */
    for (i = 0, n = -1; n < 0 && (o = find_obj(0x9e, i)) != NULL && i < 3; i++) {
        /* the rings float in the air: put the player in the ring, boosting */
        int c0 = ev_count("special_ring");
        place_player(o->x - 40, o->y);
        P.tension = 0x12c0;
        dcp_start_boost(&P);
        for (n = 0; n < 20 && ev_count("special_ring") == c0; n++) {
            if (P.gimmick == NULL) { /* fly level through the ring */
                P.y = o->y << 8;
                P.vel_y = 0;
                P.vel_x = 0x400;
            }
            frame(DCP_KEY_X | DCP_KEY_RIGHT);
        }
        if (ev_count("special_ring") == c0)
            n = -1;
        if (getenv("OBJDBG")) {
            Obj *q = find_live(0x9e, o->x);
            printf("    special %d: obj %p (%d,%d) c0 fl %x box %d,%d,%d,%d atk %x def %x; P (%d,%d) state %p\n", i, (void *)q,
                   q ? q->h.x >> 8 : 0, q ? q->h.y >> 8 : 0, q ? q->coll[0].flags : 0, q ? q->coll[0].l : 0,
                   q ? q->coll[0].t : 0, q ? q->coll[0].r : 0, q ? q->coll[0].b : 0, q ? q->coll[0].atk : 0,
                   q ? q->coll[0].def : 0, P.x >> 8, P.y >> 8, (void *)P.state);
        }
    }
    if (o) {
        int ex0 = g_exit_hook;
        for (k = 0; k < 400 && g_exit_hook == ex0; k++)
            frame(DCP_KEY_X);
        v2_result("special ring 0x9e", n > 0 && g_exit_kind == OBJ_EXIT_SPECIAL_STAGE,
                  "(%d,%d) entered after %d frames, boost %d, exit kind %d after %d more frames", o->x, o->y, n,
                  (P.pf & 0x80) != 0, g_exit_kind, k);
        CHECK(n > 0, "special ring caught the player");
        P.pf &= ~0x40200000u;
        P.disp_flags &= ~0x20;
    }
    /* Platform 0x51: stand on it, ride along */
    o = find_obj(0x51, 0);
    if (o) {
        Obj *lp;
        s32 x0, y0;
        place_player(o->x, o->y - 40);
        frame(0);
        lp = find_live(0x51, o->x);
        for (k = 0; k < 40; k++) {
            if (lp && !P.ride && k < 20) { /* keep the player over the moving platform until it lands */
                P.x = lp->h.x;
                if (P.y > lp->h.y - 0x1000)
                    P.y = lp->h.y - 0x2000;
            }
            frame(0);
        }
        x0 = P.x;
        y0 = P.y;
        for (k = 0; k < 60; k++)
            frame(0);
        v2_result("platform 0x51", lp && P.ride == &lp->ref, "(%d,%d) p8 %d p9 %d: riding %d, player moved %+d,%+d px in 60 "
                  "frames", o->x, o->y, o->p2, o->p3, lp && P.ride == &lp->ref, (P.x - x0) >> 8, (P.y - y0) >> 8);
        CHECK(lp && P.ride == &lp->ref, "player rides the platform");
    }
    /* Crumbling floor 0x76 */
    o = find_obj(0x76, 0);
    if (o) {
        place_player(o->x, o->y - 40);
        n = run_ev(200, 0, "land_crumble");
        for (k = 0; k < 80; k++)
            frame(0);
        v2_result("crumbling floor 0x76", n > 0, "(%d,%d) crumbles %d frames after landing; 80 frames later player y %d "
                  "(floor %d)", o->x, o->y, n, P.y >> 8, o->y);
        CHECK(n > 0, "crumbling floor");
    }
    /* Iron ball 0x78 (z12): touch its start box -> rolls */
    o = find_obj(0x78, 0);
    if (o) {
        Obj *ib;
        s32 x0, x1;
        place_player(o->x + o->p0 + o->p2 / 2, o->y + o->p1 + o->p3 / 2);
        n = run_ev(60, 0, "iron_ball_roll");
        ib = find_live(0x78, o->x);
        x0 = x1 = ib ? ib->h.x : 0;
        for (k = 0; k < 120 && ib && ib->used && ib->prm && ib->prm->type == 0x78; k++) {
            x1 = ib->h.x;
            if (getenv("OBJDBG") && ib && k % 10 == 0)
                printf("    ball k%d (%d,%d) used %d type %d gnd %d mf %x\n", k, ib->h.x >> 8, ib->h.y >> 8, ib->used,
                       ib->prm ? ib->prm->type : -1, ib->h.gnd_vel, ib->h.move_flags);
            frame(0);
        }
        v2_result("iron ball 0x78", n > 0 && x1 != x0, "(%d,%d) rolls after %d frames, moved %+d px in %d frames", o->x,
                  o->y, n, (x1 - x0) >> 8, k);
        CHECK(n > 0, "iron ball rolls");
    }
    /* Flapper 0x02: jump into it from below -> destroyed */
    for (i = 0, n = -1; n < 0 && (o = find_obj(0x02, i)) != NULL && i < 7; i++) {
        Obj *fl;
        c = st->enemies_destroyed;
        place_player(o->x, o->y + 40);
        frame(0);
        fl = find_live(0x02, o->x);
        if (fl) {
            P.x = fl->h.x;
            P.y = fl->h.y + 0x2800;
        }
        dcp_act_jump(&P);
        n = run_until(40, DCP_KEY_A, &st->enemies_destroyed, c);
    }
    if (o)
        v2_result("flapper 0x02", n > 0, "(%d,%d) destroyed after %d frames, animals now %d", o->x, o->y, n,
                  count_kind_type(1, 0, 0xe));
    CHECK(o == NULL || n > 0, "flapper destroyed");
    /* Egg Pawn M spawner 0x01: stand in front of it -> it throws Egg Pawn M (type 0xdf) */
    for (i = 0, n = -1; n < 0 && (o = find_obj(0x01, i)) != NULL && i < 7; i++)
        n = approach(o, -70, 0, 240, "eggpawn_m_child");
    if (o)
        v2_result("egg pawn M 0x01", n > 0, "(%d,%d) child spawned after %d frames", o->x, o->y, n);
    CHECK(o == NULL || n > 0, "egg pawn M child");
    /* Boost (AttackHitbox): boost into an egg pawn -> knocked away */
    {
        u32 ka = st->knock_aways;
        for (i = 0; i < 8 && st->knock_aways == ka; i++) {
            o = find_obj(0x00, i);
            if (!o)
                break;
            place_on_floor(o->x - 160, o->y);
            for (k = 0; k < 10; k++)
                frame(0);
            P.tension = 0x12c0;
            for (k = 0; k < 120 && st->knock_aways == ka; k++)
                frame(DCP_KEY_RIGHT | DCP_KEY_X);
        }
        v2_result("boost knock-away", st->knock_aways > ka, "%u enemies knocked away by the boost hit box",
                  st->knock_aways - ka);
        CHECK(st->knock_aways > ka, "boost hit box knocks enemies away");
    }
    /* Homing target search: Sonic in the air near an enemy -> p->homing; R = homing attack */
    {
        int found = 0, hit = 0;
        u32 e0;
        for (i = 0; i < 18 && !hit; i++) {
            Obj *q;
            o = find_obj(0x00, i);
            if (!o)
                break;
            place_on_floor(o->x - 60, o->y);
            frame(0);
            q = find_live(0x00, o->x);
            if (!q)
                continue;
            /* above and behind the enemy: Player_Action_HomingAttack needs the target below */
            P.x = q->h.x - 0x3000;
            P.y = q->h.y - 0x4000;
            dcp_act_jump(&P);
            frame(DCP_KEY_A);
            if (P.homing != &q->ref || P.state != dcp_state_in_air || g_obj.players[0].atk.atk != 0x13)
                continue; /* another enemy is nearer, or the jump hit the level */
            found = 1;
            e0 = st->enemies_destroyed;
            frame(DCP_KEY_R);
            for (k = 0; k < 40 && st->enemies_destroyed == e0; k++) {
                if (getenv("OBJDBG"))
                    printf("    homing k%d P(%d,%d) homing %d state_homing %d atk %x/%x enemy (%d,%d)\n", k, P.x >> 8, P.y >> 8,
                           P.homing != NULL, P.state == dcp_state_homing, g_obj.players[0].atk.atk, g_obj.players[0].atk.flags,
                           q->h.x >> 8, q->h.y >> 8);
                frame(0);
            }
            hit = st->enemies_destroyed > e0;
        }
        v2_result("homing attack", found && hit, "target found %d, enemy destroyed by the homing attack %d", found, hit);
        CHECK(found, "homing target search sets p->homing");
    }
    /* Magnet barrier: rings within 0x48 px fly to the player */
    {
        u32 a0 = st->rings_attracted;
        const rlvl_ring_t *rg = (const rlvl_ring_t *)rlvl_section(g_level, RLVL_SEC_RINGS, NULL);
        for (i = 0; i < (int)h->num_rings && st->rings_attracted == a0; i += 7) {
            if (g_obj.ring_state[i] == 0xff)
                continue;
            place_player(rg[i].x - 40, rg[i].y);
            Player_GiveMagnet(&P);
            for (k = 0; k < 30; k++)
                frame(0);
        }
        v2_result("magnet barrier", st->rings_attracted > a0, "%u rings attracted (pf %#x)", st->rings_attracted - a0,
                  P.pf & 0x30000000);
        CHECK(st->rings_attracted > a0, "magnet attracts rings");
        P.pf &= ~0x30000000u;
    }
    /* decorations */
    printf("  decorations: spawned %u despawned %u, live: flowers/vines %d, vines(hang) %d, leaves %d, birds %d, "
           "animals %d; bird fly-aways %d, leaf bursts %d, freed animals %d\n",
           st->dec_spawned, st->dec_despawned, count_kind_type(1, 0x0f, 0x30) + count_kind_type(1, 0x3a, 0x40),
           count_kind_type(1, 0x31, 0x34), count_kind_type(1, 0x35, 0x36), count_kind_type(1, 0x37, 0x39),
           count_kind_type(1, 0, 0xe), ev_count("bird_fly"), ev_count("leaf_burst"), ev_count("freed_animal"));
    CHECK(st->dec_spawned > 0, "decorations spawned");
    printf("  v2 targets: %d of %d interactions seen; pool live %u of %d; late sprite loads %d\n", g_v2_ok, g_v2_n,
           st->live, OBJ_MAX, g_obj.lazy_loads);
}

/* sweep: from every 25th placed ring (reachable spots spread over the act),
 * run right for 240 frames with periodic jumps and boosts */
static void test_sweep(void) {
    const rlvl_header_t *h = (const rlvl_header_t *)g_level;
    const rlvl_ring_t *rg = (const rlvl_ring_t *)rlvl_section(g_level, RLVL_SEC_RINGS, NULL);
    const ObjStats *st = objects_stats();
    ObjStats s0 = *st;
    u32 i;
    int n, segs = 0, i_ev;
    int ev0[NEV];
    for (i_ev = 0; i_ev < NEV; i_ev++)
        ev0[i_ev] = g_ev[i_ev].n;
    g_quiet_ev = 1;
    for (i = 0; i < h->num_rings; i += 25) {
        place_player(rg[i].x, rg[i].y - 8);
        P.tension = 0x12c0;
        for (n = 0; n < 240; n++) {
            u16 k = DCP_KEY_RIGHT;
            if (n % 60 >= 45 && n % 60 < 52)
                k |= DCP_KEY_A;
            if (n % 120 < 30)
                k |= DCP_KEY_X;
            frame(k);
        }
        segs++;
    }
    g_quiet_ev = 0;
    printf("[sweep] %d segments x 240 frames: rings %u, item boxes %u, springs %u, dash panels %u, dash rings %u, "
           "enemies %u (knocked away %u), hurts %u, checkpoints %u, gimmicks %u, grinds %u, walls %u, rocks %u, "
           "decorations %u, homing targets %u\n",
           segs, st->rings_collected - s0.rings_collected, st->item_boxes - s0.item_boxes, st->springs - s0.springs,
           st->dash_panels - s0.dash_panels, st->dash_rings - s0.dash_rings,
           st->enemies_destroyed - s0.enemies_destroyed, st->knock_aways - s0.knock_aways, st->hurts - s0.hurts,
           st->checkpoints - s0.checkpoints, st->gimmicks - s0.gimmicks, st->grinds - s0.grinds,
           st->walls_broken - s0.walls_broken, st->rocks_broken - s0.rocks_broken, st->dec_spawned - s0.dec_spawned,
           st->homing_targets - s0.homing_targets);
    printf("  events:");
    for (i_ev = 0; i_ev < NEV && g_ev[i_ev].name[0]; i_ev++)
        if (g_ev[i_ev].n != ev0[i_ev])
            printf(" %s=%d", g_ev[i_ev].name, g_ev[i_ev].n - ev0[i_ev]);
    printf("\n");
    CHECK(st->rings_collected > s0.rings_collected && st->dec_spawned > s0.dec_spawned, "sweep collects rings, spawns decorations");
}

static void test_respawn(void) {
    const rlvl_header_t *h = (const rlvl_header_t *)g_level;
    u32 i, collected = 0, placed = 0, n;
    const ObjStats *st = objects_stats();
    u32 sp0;
    printf("[respawn]\n");
    for (i = 0; i < h->num_rings; i++)
        if (g_obj.ring_state[i] == 0xff)
            collected++;
    /* go back to the start: everything in view gets (re)spawned */
    sp0 = st->spawned;
    {
        const rlvl_object_t *o = find_obj(0x43, 0);
        place_player(o->x - 100, o->y - 40);
    }
    for (n = 0; n < 5; n++)
        frame(0);
    for (i = 0; i < h->num_rings; i++)
        if (g_obj.ring_state[i] == 0xff)
            placed++;
    printf("  ring entries marked (placed or collected): %u of %u; objects spawned on return %u; live %u\n", placed,
           h->num_rings, st->spawned - sp0, st->live);
    CHECK(st->spawned > sp0, "objects respawn when the view comes back");
    CHECK(placed <= collected + OBJ_RING_MAX, "despawned rings were released");
}

int main(int argc, char **argv) {
    long size;
    char path[512];
    s16 *trig;
    u8 *phys;
    s32 sx = 128, sy = 768;
    const rlvl_object_t *ps;
    if (argc < 5) {
        printf("usage: %s <level.bin> <player tables dir> <objdata dir> <sprites bac dir> [-v]\n", argv[0]);
        return 2;
    }
    if (argc > 5)
        g_verbose = 2;
    snprintf(path, sizeof(path), "%s/trig.bin", argv[2]);
    trig = (s16 *)load(path, &size);
    snprintf(path, sizeof(path), "%s/physics.bin", argv[2]);
    phys = (u8 *)load(path, &size);
    dcp_set_trig(trig, trig + 8192);
    if (dcp_set_physics(phys, (u32)size) != 2) {
        printf("bad physics.bin\n");
        return 2;
    }
    g_level = (u8 *)load(argv[1], &size);
    if (rlvl_bind(&M, g_level) != 0) {
        printf("bad level\n");
        return 2;
    }
    dcp_bind_map(&M);
    memset(&g_dcp_world, 0, sizeof(g_dcp_world));
    g_dcp_world.zone = ((const rlvl_header_t *)g_level)->zone;
    g_dcp_world.act = ((const rlvl_header_t *)g_level)->act;
    g_dcp_world.flags = 1;
    g_dcp_world.objmgr = 0x18;
    g_dcp_world.map_right = M.right;
    g_dcp_world.death_y = M.bottom;
    g_dcp_world.timescale = 0x100;
    g_dcp_world.lives = 3;
    ps = find_obj(0x72, 0);
    if (ps) {
        sx = ps->x;
        sy = ps->y;
    }
    g_dcp_world.check_x = sx;
    g_dcp_world.check_y = sy;
    dcp_player_create(&P, 0, 0, sx, sy);
    g_obj_hooks.log = hk_log;
    g_obj_hooks.goal = hk_goal;
    g_obj_hooks.stage_exit = hk_exit;
    objects_set_paths(argv[3], argv[4]);
    objects_add_player(&P);
    camera_follow(1);
    {
        int r = objects_init(&M, g_level);
        int s, nres = 0, nfail = 0;
        printf("objects_init: %d; stage %d; preloaded sprites:", r, ((const rlvl_header_t *)g_level)->stage);
        for (s = 0; s < OBJ_BAC_COUNT; s++) {
            if (g_obj.res[s].loaded)
                printf(" %s", g_obj.data.ids[s]);
            nres += g_obj.res[s].loaded;
            nfail += g_obj.res[s].id[0] == 1;
        }
        printf("\n  %d of %d slots preloaded, %d failed, PlayerStart respawn point (%d,%d)\n", nres, OBJ_BAC_COUNT, nfail,
               g_dcp_world.check_x, g_dcp_world.check_y);
        CHECK(r == 0, "objects_init");
        CHECK(nfail == 0 && nres > 10, "every preloaded sprite resource loads");
        if (r != 0)
            return 1;
    }
    camera_follow(1);
    test_unit();
    test_run(getenv("RUNFRAMES") ? atoi(getenv("RUNFRAMES")) : 1500);
    test_targets();
    test_v2();
    test_sweep();
    test_respawn();
    printf("%d checks, %d failed\n", g_checks, g_fail);
    objects_shutdown();
    return g_fail ? 1 : 0;
}
