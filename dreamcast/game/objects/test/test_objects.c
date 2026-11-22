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
static s32 g_spring_b, g_spring_n; /* last "spring" event: the Spring_OnHit velocity */
static s32 g_ring_a, g_ring_b;     /* last "dash_ring" event: type, direction */
static int g_rail_panels;          /* "dash_panel" events of type 0x5c */
/* player sound cues (g_dcp_hooks.sfx / sfx_stop), with the frame they were raised in */
static struct { int frame, ch, id; } g_sfx[64];
static int g_nsfx;
static void (*g_prev_sfx)(void *, DcPlayer *, s32, s32);
static void hk_sfx(void *ctx, DcPlayer *p, s32 ch, s32 id) {
    if (g_prev_sfx)
        g_prev_sfx(ctx, p, ch, id);
    if (g_nsfx < 64)
        g_sfx[g_nsfx].frame = g_frame, g_sfx[g_nsfx].ch = ch, g_sfx[g_nsfx].id = id, g_nsfx++;
}
static void (*g_prev_stop)(void *, DcPlayer *, s32);
static void hk_sfx_stop(void *ctx, DcPlayer *p, s32 ch) {
    if (g_prev_stop)
        g_prev_stop(ctx, p, ch);
    if (g_nsfx < 64)
        g_sfx[g_nsfx].frame = g_frame, g_sfx[g_nsfx].ch = ch, g_sfx[g_nsfx].id = -1, g_nsfx++;
}
static void hk_log(void *ctx, const char *ev, s32 a, s32 b) {
    (void)ctx;
    ev_add(ev);
    if (!strcmp(ev, "spring"))
        g_spring_b = b, g_spring_n++;
    if (!strcmp(ev, "dash_ring"))
        g_ring_a = a, g_ring_b = b;
    if (!strcmp(ev, "dash_panel") && a == 0x5c)
        g_rail_panels++;
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
    /* Player_UpdateCamera (camera module, not in the object module): while a gimmick
     * locks the camera (pf2 & 0x10 / 0x20) it follows the gimmick object, offset by
     * @0x3EC + 0x80 / @0x3EE + 0x60 (the special ring keeps it in view this way) */
    if ((P.pf2 & 0x30) && P.gimmick != NULL && g_obj.n_players) {
        const Obj *gm = (const Obj *)P.gimmick;
        if (P.pf2 & 0x10)
            tx = (gm->h.x >> 8) - (g_obj.players[0].g3EC + 0x80);
        if (P.pf2 & 0x20)
            ty = (gm->h.y >> 8) - (g_obj.players[0].g3EE + 0x60);
    }
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

static u8 g_chara; /* CHARA=1: Blaze */
static void place_player(s32 x, s32 y) {
    s16 rings = P.rings;
    memset(&P, 0, sizeof(P));
    dcp_player_create(&P, g_chara, 0, x, y);
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


/* ------------------------------------------------------------ player effects + loop */
static int count_boost_fx(u16 anim) {
    Obj *q;
    int n = 0;
    for (q = g_obj.head; q; q = q->next)
        if (q->used && q->has_spr && !(q->h.obj_flags & 4) && q->spr.res == &g_obj.res[OBJ_BAC_EFF_BOOST] &&
            (anim == 0xffff || q->spr.anim == anim))
            n++;
    return n;
}
static void test_player_fx(void) {
    const rlvl_object_t *ps = find_obj(0x72, 0);
    ObjPlayerCtx *pc;
    int n, aura = 0, burst = 0, hs = 0, dust = 0, def_ok = 0, semi = 1, boost_frames = 0, boost_f0 = 0;
    int trail_max = 0, trail_a[3] = { 0, 0, 0 }, trail_after = -1;
    u16 trail_col = 0;
    Obj *q;
    printf("[player fx] Player_StartBoost / _UpdateBoost / _StartHighSpeed effects, boost hurt-box defence\n");
    if (ps == NULL)
        return;
    place_on_floor(ps->x, ps->y);
    g_dcp_world.flags |= 1; /* stage timer running (an earlier part may have reached the goal) */
    pc = obj_player_ctx(&P);
    for (n = 0; n < 20; n++)
        frame(0);
    P.tension = 0x12c0;
    g_nsfx = 0;
    for (n = 0; n < 50; n++) {
        frame((u16)(DCP_KEY_RIGHT | (n >= 10 ? DCP_KEY_X : 0)));
        if (P.pf & 0x80) {
            ObjTrailQuad tq[24];
            int nq = objects_trail_quads(tq, 24), k;
            if (boost_frames == 0)
                boost_f0 = g_frame - 1; /* (frame() counts up at its end) */
            if (nq > trail_max)
                trail_max = nq;
            if (nq == 10) {
                trail_a[0] = tq[0].alpha, trail_a[2] = tq[9].alpha, trail_col = tq[0].bgr555;
                for (k = 0; k < nq; k++)
                    if (tq[k].alpha > trail_a[1])
                        trail_a[1] = tq[k].alpha;
            }
            boost_frames++;
            aura += count_boost_fx(4) > 0;
            burst += count_boost_fx(5) > 0;
            hs += count_boost_fx(3) > 0;
            if (pc && (pc->body.def & 5) == 5)
                def_ok++;
        }
        for (q = g_obj.head; q; q = q->next)
            if (q->used && q->has_spr && q->spr.res == &g_obj.res[OBJ_BAC_EFF_BOOST] && q->spr.anim >= 3 && q->spr.anim <= 5 &&
                !(q->spr.flags & 0x400))
                semi = 0;
    }
    /* (Player_StartBoost sets pf 0x80 before Player_StartHighSpeed, so no EffHighSpeedStart then) */
    printf("  boost cues: ");
    for (n = 0; n < g_nsfx; n++)
        printf("f%d ch%d %s%x  ", g_sfx[n].frame - boost_f0, g_sfx[n].ch, g_sfx[n].id < 0 ? "stop" : "", g_sfx[n].id < 0 ? 0 : g_sfx[n].id);
    printf("\n");
    {
        /* Player_StartBoost: Sound_PlaySeqArcEx(@384, 0, 0xa), NNS_SndPlayerStopSeq(@38C) in the frame
         * pf 0x80 is set (Blaze: 0x17) */
        int voice = 0, stop = 0;
        for (n = 0; n < g_nsfx; n++) {
            voice |= g_sfx[n].frame == boost_f0 && g_sfx[n].ch == DCP_CH_VOICE && g_sfx[n].id == (P.chara != 1 ? 0xa : 0x17);
            stop |= g_sfx[n].frame == boost_f0 && g_sfx[n].ch == DCP_CH_BOOST && g_sfx[n].id < 0;
        }
        CHECK(voice && stop, "boost start: voice 0x%x and the boost channel stop in the frame the boost starts",
              P.chara != 1 ? 0xa : 0x17);
    }
    printf("  EffTrail: %d segments at most, head alpha %d, peak %d, tail %d, colour %04x\n", trail_max, trail_a[0],
           trail_a[1], trail_a[2], trail_col);
    CHECK(trail_max == 10 && trail_a[0] == 6 && trail_a[1] == 0x12 && trail_a[2] >= 1 && trail_col == 0x7ca5,
          "EffTrail: 10 segments while boosting, alpha 6 -> 0x12 -> 1, colour 0x7ca5");
    printf("  boosting %d frames: aura on %d, burst on %d, high-speed start on %d, hurt box def&5 on %d\n", boost_frames, aura,
           burst, hs, def_ok);
    CHECK(boost_frames > 30 && aura == boost_frames, "EffBoostAura follows the whole boost (%d of %d)", aura, boost_frames);
    CHECK(burst > 0 && burst < boost_frames, "EffBoostBurst plays once at the start and ends with its animation");
    CHECK(def_ok == boost_frames, "boosting makes the hurt box invulnerable (Player_UpdateBoost 2C4 |= 5)");
    CHECK(semi, "aura / burst / high-speed sprites are semi-transparent (AnimatorSprite 0x400)");
    for (n = 0; n < 4; n++)
        frame(DCP_KEY_RIGHT);
    CHECK(!(P.pf & 0x80) && count_boost_fx(4) == 0, "releasing boost ends the aura (EffBoost_State_FollowPlayer)");
    /* EffTrail_State_Follow: one segment less a frame once the boost ends */
    for (n = 0; n < 14 && trail_after != 0; n++) {
        ObjTrailQuad tq[24];
        trail_after = objects_trail_quads(tq, 24);
        frame(DCP_KEY_RIGHT);
    }
    CHECK(trail_after == 0 && n >= 6 && n <= 12, "EffTrail shrinks away after the boost (%d frames)", n);
    CHECK(pc == NULL || pc->body.def == 2, "Player_StopBoost restores the hurt box (def %x)", pc ? pc->body.def : 0);
    /* high-speed mode without boost: dust puffs every 8 frames (EffBoost_Create) */
    for (n = 0; n < 60; n++) {
        frame(DCP_KEY_RIGHT);
        dust += count_boost_fx(0) > 0;
    }
    printf("  high-speed run after the boost: dust on %d of 60 frames (pf 0x100 %d)\n", dust, (P.pf & 0x100) != 0);
    CHECK(!(P.pf & 0x100) || dust > 0, "EffBoost dust puffs in high-speed mode");
}
/* Blaze's flames (ac_eff_boost.bac anim 2): EffBlazeRunFlame (running past run_thr[1], every 4
 * frames), EffBlazeSpinFlame (jump anim 0x30 / rolling, every 8, circling her), EffBlazeBoostFlame
 * (boosting, every 4); no EffTrail for Blaze */
static int count_flames(int *spin) {
    Obj *q;
    int n = 0;
    *spin = 0;
    for (q = g_obj.head; q; q = q->next)
        if (q->used && q->has_spr && !(q->h.obj_flags & 4) && q->spr.res == &g_obj.res[OBJ_BAC_EFF_BOOST] && q->spr.anim == 2) {
            n++;
            *spin += q->ppar == &P;
        }
    return n;
}
static void test_blaze_flames(void) {
    const rlvl_object_t *ps = find_obj(0x72, 0);
    int n, run = 0, jump = 0, spin = 0, sp, boost = 0, trail = 0, made = 0, prev = 0, c;
    u8 ch = g_chara;
    if (ps == NULL)
        return;
    printf("[blaze fx] EffBlazeRunFlame / EffBlazeSpinFlame / EffBlazeBoostFlame\n");
    g_chara = 1;
    place_on_floor(ps->x, ps->y);
    g_dcp_world.flags |= 1;
    for (n = 0; n < 10; n++)
        frame(0);
    for (n = 0; n < 90; n++) { /* run up past run_thr[1] */
        frame(DCP_KEY_RIGHT);
        c = count_flames(&sp);
        if (c > prev && DCP_ABS(P.gnd_vel) > P.run_thr[1] && (P.move_flags & 1))
            run++;
        prev = c;
    }
    for (n = 0; n < 40; n++) { /* jump: anim 0x30 */
        frame((u16)(DCP_KEY_RIGHT | (n < 10 ? DCP_KEY_A : 0)));
        c = count_flames(&sp);
        if (P.anim == 0x30 && sp > 0)
            jump++;
        spin = sp > spin ? sp : spin;
    }
    P.tension = 0x12c0;
    for (n = 0; n < 40; n++) {
        ObjTrailQuad tq[4];
        frame((u16)(DCP_KEY_RIGHT | DCP_KEY_X));
        c = count_flames(&sp);
        if ((P.pf & 0x80) && c > prev)
            boost++;
        prev = c;
        trail += objects_trail_quads(tq, 4);
    }
    made = run + boost;
    printf("  run flames created on %d frames, spin flames following her on %d jump frames (max %d), boost flames on %d "
           "frames, trail quads %d\n", run, jump, spin, boost, trail);
    CHECK(run >= 5 && run <= 25, "EffBlazeRunFlame every 4 frames while running fast (%d)", run);
    CHECK(jump > 0, "EffBlazeSpinFlame circles Blaze while she jumps");
    CHECK(boost >= 5, "EffBlazeBoostFlame while boosting (%d)", boost);
    CHECK(trail == 0, "no EffTrail for Blaze (Player_StartBoost: unk324 == 0 only)");
    (void)made;
    g_chara = ch;
    place_on_floor(ps->x, ps->y);
}
/* Leaf Storm act 1, first loop (x 5380..5700): the MapObj40 0x41 / 0x42 layer switches take the
 * player in on layer A and out on layer B, from a standing start on layer B */
static void test_loop(void) {
    const rlvl_header_t *h = (const rlvl_header_t *)g_level;
    int n, top = 0, maxx = 0;
    if (h->stage != 0)
        return;
    printf("[loop] z11 first loop from x 5100 (layer B; the spring below throws the player in)\n");
    place_on_floor(5100, 2034);
    for (n = 0; n < 3; n++)
        frame(0);
    for (n = 0; n < 420; n++) {
        frame(DCP_KEY_RIGHT);
        if ((P.move_flags & 1) && P.angle >= 0x70 && P.angle <= 0x90)
            top++;
        if ((P.x >> 8) > maxx)
            maxx = P.x >> 8;
    }
    printf("  frames on the loop ceiling %d, max x %d, layer %d\n", top, maxx, (int)(P.obj_flags & 1));
    CHECK(top > 0 && maxx > 5900, "the player runs around the loop and leaves it to the right");
}

/* ------------------------------------------------------------ audit against the original
 * Exact values from src/game/Objects (Spring_OnHit, DashP_OnHit, Needle via Player_OnHurt)
 * and src/game/Player (Player_Action_Launch, _DashPanel, _OnHurt, Player_In). */
static Obj *live_near(u16 type, s32 x, s32 y) {
    Obj *q, *best = NULL;
    s32 bd = 0x7fffffff, d;
    for (q = g_obj.head; q; q = q->next)
        if (q->used && q->prm && !q->is_dec && q->prm->type == type && !(q->h.obj_flags & 4)) {
            d = DCP_ABS((q->h.x >> 8) - x) + DCP_ABS((q->h.y >> 8) - y);
            if (d < bd) {
                bd = d;
                best = q;
            }
        }
    return bd < 64 ? best : NULL;
}
static void test_audit(void) {
    const ObjStats *st = objects_stats();
    const rlvl_header_t *h = (const rlvl_header_t *)g_level;
    u32 i, c;
    int n, nspring = 0, spring_ok = 0, ndash = 0, dash_ok = 0;
    u32 h0;
    printf("[audit] springs / dash panels / needles / item boxes / markers against the original formulas\n");
    for (i = 0; i < h->num_objects; i++) {
        const rlvl_object_t *e = &((const rlvl_object_t *)rlvl_section(g_level, RLVL_SEC_OBJECTS, NULL))[i];
        Obj *o;
        s16 vx, vy;
        u16 id = e->type;
        int ok;
        if (!((id >= 0x43 && id <= 0x4c) || id == 0x5f || id == 0x60 || id == 0x61 || id == 0x62))
            continue;
        if ((id >= 0x43 && id <= 0x4c) ? nspring >= 24 : ndash >= 12)
            continue;
        /* stand the player next to it, let it spawn, then put him on its centre in the air */
        place_player(e->x, e->y - 48);
        P.move_flags |= 0x10;
        for (n = 0; n < 2; n++)
            frame(0);
        o = live_near(id, e->x, e->y);
        if (o == NULL)
            continue;
        if (id >= 0x43 && id <= 0x4c) {
            /* Spring_OnHit: v = -0x780 - p6 * 0x180; up/down 0x43/0x44: vx = 0; side 0x45/0x46: vy = 0;
             * diagonal 0x47..0x4c: both * 0xb5 >> 8; 5C & 2 flips vy, 5C & 1 flips vx */
            vx = (s16)(-0x780 - o->prm->p6 * 0x180);
            vy = vx;
            if (id <= 0x44)
                vx = 0;
            else if (id <= 0x46)
                vy = 0;
            if (id >= 0x47) {
                vx = (s16)((vx * 0xb5) >> 8);
                vy = (s16)((vy * 0xb5) >> 8);
            }
            if (o->h.disp_flags & 2)
                vy = (s16)-vy;
            if (o->h.disp_flags & 1)
                vx = (s16)-vx;
            c = st->springs;
            h0 = st->hurts;
            {
                /* on the centre of its hit box (diagonal springs: the box sits beside the origin; ObjRect
                 * flags 1/2 mirror it, ObjRect_HandleCollider) */
                const ObjBox *bx = &o->coll[0];
                s32 cx = (bx->flags & 1) ? -(bx->l + bx->r) / 2 : (bx->l + bx->r) / 2;
                s32 cy = (bx->flags & 2) ? -(bx->t + bx->b) / 2 : (bx->t + bx->b) / 2;
                place_player((o->h.x >> 8) + cx, (o->h.y >> 8) + cy);
                for (n = 0; n < 30 && st->springs == c; n++)
                    frame(0);
                if (st->springs == c) {
                    /* pushed out of the box by the slope it stands on: drop onto it from above */
                    place_player((o->h.x >> 8) + cx, (o->h.y >> 8) + cy - 24);
                    P.move_flags |= 0x10;
                }
            }
            P.gnd_vel = 0;
            /* the boxes registered this frame are checked at the start of the next (Stage_Main) */
            for (n = 0; n < 30 && st->springs == c; n++)
                frame(0);
            nspring++;
            /* (zone 4: Spring_OnHit calls Player_Action_SpringLaunchAlt = Player_Action_Launch + sound 0x72,
             * the same expectations) */
            /* Player_Action_Launch: vel = (vx, vy) (vx == 0: gnd * cos = 0), anim 0x16 / 0x13,
             * pf |= 3, airborne, dash timer 0x40 for sideways launches, facing from vx */
            {
                s16 gx = P.vel_x, gy = P.vel_y, ga = P.anim;
                u32 gpf = P.pf, gmf = P.move_flags;
                /* the velocity Spring_OnHit passed to Player_Action_SpringLaunch (logged by spring_on_hit) */
                s16 hx = (s16)(g_spring_b & 0xffff), hy = (s16)(g_spring_b >> 16);
                int onhit = hx == vx && hy == vy;
                /* the player task runs after the launch in the same frame: Player_ApplyAirInput with no
                 * direction held clamps vel_x to +-top (side springs), StageObj_Move adds gravity once;
                 * a side spring sets y to its own + 2 px, so the floor collision lands him in that frame and
                 * Player_ApplyGroundInput clamps the same way */
                /* under water (pf 0x4000000) Player_Action_Launch keeps 3/4 of both components */
                if (gpf & 0x4000000) {
                    vx = (s16)((vx >> 1) + (vx >> 2));
                    vy = (s16)((vy >> 1) + (vy >> 2));
                }
                s16 mx = (gpf & 0x80) ? P.boost_top : (gpf & 0x100) ? P.hs_top : P.top;
                s16 wx = vx > mx ? mx : vx < -mx ? (s16)-mx : vx;
                /* landed in the same frame (diagonal-down springs over a floor): Player_Action_LandOnGround
                 * took the velocity, so only the OnHit values and the single launch are compared */
                int landed = ((gmf & 1) && vy > 0) || st->hurts != h0; /* (or hurt by spikes beside it) */
                ok = onhit && st->springs == c + 1 && (landed || gx == wx) && (landed || vy == 0 || gy == (s16)(vy + P.gravity)) &&
                     (landed || ga == (vx != 0 ? 0x16 : gy > 0x40 ? 0x14 : 0x13)) && (landed || (gpf & 3) == 3) &&
                     (landed || (gmf & 0x10)) && (landed || vy == 0 || !(gmf & 1)) && (vx == 0 || P.dash_timer >= 0x3f) &&
                     (vx == 0 || ((P.disp_flags & 1) != 0) == (vx < 0));
                for (n = 0; n < 4; n++)
                    frame(0);
                ok = ok && st->springs == c + 1; /* one launch per contact */
                spring_ok += ok;
                if (!ok)
                    printf("  spring 0x%02x at (%d,%d) p6 %d df %x: vel %d,%d (want %d,%d + gravity) anim %x pf %x mf %x springs +%u\n",
                           id, e->x, e->y, o->prm->p6, o->h.disp_flags, gx, gy, vx, vy, ga, gpf, gmf, st->springs - c);
            }
        } else {
            /* DashP_OnHit: 0x5c..0x60 x = +-top by 5C & 1, 0x61/0x62 y = -+top by 5C & 2; only for a grounded
             * player (0x5c: grinding, anim 0x1a..0x20); Player_Action_DashPanel: dash timer 0x80 */
            const ObjBox *bx = &o->coll[0];
            s32 cx = (o->h.x >> 8) + ((bx->flags & 1) ? -(bx->l + bx->r) / 2 : (bx->l + bx->r) / 2);
            s32 cy = (o->h.y >> 8) + ((bx->flags & 2) ? -(bx->t + bx->b) / 2 : (bx->t + bx->b) / 2);
            u8 ang = id == 0x60 ? 0x80 : id == 0x61 ? 0xc0 : id == 0x62 ? 0x40 : 0;
            s16 want = (id == 0x61 || id == 0x62) ? ((o->h.disp_flags & 2) ? P.top : (s16)-P.top)
                                                   : ((o->h.disp_flags & 1) ? (s16)-P.top : P.top);
            s16 gnd = 0;
            int air_hits;
            /* airborne contact: no boost */
            c = st->dash_panels;
            place_player(cx, cy);
            for (n = 0; n < 4; n++) {
                P.x = cx << 8, P.y = cy << 8, P.vel_x = P.vel_y = P.gnd_vel = 0;
                P.move_flags = (P.move_flags & ~1u) | 0x10;
                frame(0);
            }
            air_hits = (int)(st->dash_panels - c);
            if (ang == 0) {
                /* floor: stand on it */
                place_on_floor(cx, o->h.y >> 8);
                for (n = 0; n < 30 && st->dash_panels == c; n++) {
                    frame(0);
                    gnd = P.gnd_vel;
                }
            }
            /* wall / ceiling: grounded contact on its surface (the flag is read when the boxes are checked,
             * next frame) */
            for (n = 0; n < 8 && st->dash_panels == c; n++) {
                P.x = cx << 8, P.y = cy << 8, P.vel_x = P.vel_y = P.gnd_vel = 0;
                P.move_flags = (P.move_flags | 1) & ~0x10u;
                P.angle = ang;
                frame(0);
                gnd = P.gnd_vel;
            }
            ndash++;
            /* (walls: placed inside the wall, the push-out sets move_flags 4 and Player_Last clears the
             * timer in the same frame; only the activation is checked there) */
            ok = air_hits == 0 && st->dash_panels == c + 1 && (P.dash_timer >= 0x7e || id == 0x61 || id == 0x62);
            /* the dash sets gnd (floor, ceiling) / gnd from y (walls); the ground task then moves with it */
            if (ok && ang == 0)
                ok = gnd == want || P.vel_x == want;
            dash_ok += ok;
            if (!ok)
                printf("  dash panel 0x%02x at (%d,%d) df %x: gnd %d vel %d (want %d) timer %d panels +%u airborne +%d\n", id, e->x,
                       e->y, o->h.disp_flags, gnd, P.vel_x, want, P.dash_timer, st->dash_panels - c, air_hits);
        }
    }
    printf("  springs: %d of %d exact; dash panels: %d of %d\n", spring_ok, nspring, dash_ok, ndash);
    CHECK(nspring == 0 || W.zone == 3 || spring_ok == nspring, "every spring launches with the Spring_OnHit velocity, pose and flags, once");
    CHECK(dash_ok == ndash, "every dash panel sets the dash timer and +-top speed");
    /* Needle: Player_OnHurt once, invulnerability invuln_len frames with the blink, no second hit on the
     * spikes meanwhile, hurt box back to def 2 afterwards (Player_In) */
    {
        const rlvl_object_t *nd = find_obj(0x4d, 0);
        if (nd != NULL) {
            ObjPlayerCtx *pc;
            int hurts0 = (int)st->hurts, blink = 0, inv0;
            place_player(nd->x, nd->y - 40);
            P.rings = 5;
            for (n = 0; n < 2; n++)
                frame(0);
            for (n = 0; n < 40 && (int)st->hurts == hurts0; n++)
                frame(0);
            inv0 = P.invuln;
            pc = obj_player_ctx(&P);
            printf("  needle 0x4d at (%d,%d): hurt after %d frames, vel %d,%d, invuln %d, state hurt %d, rings %d\n", nd->x, nd->y,
                   n, P.vel_x, P.vel_y, inv0, P.state == dcp_state_hurt, P.rings);
            CHECK((int)st->hurts == hurts0 + 1 && P.state == dcp_state_hurt && P.rings == 0 && P.vel_y == -0x300 &&
                      DCP_ABS(P.vel_x) == 0x180,
                  "needle: Player_OnHurt knockback (+-0x180, -0x300), rings scattered");
            CHECK(inv0 >= P.invuln_len - 2 && pc && (pc->body.def & 5) == 5, "needle: invulnerability %d of %d frames", inv0,
                  P.invuln_len);
            for (n = 0; n < inv0 - 1; n++) {
                frame(0);
                blink += (P.disp_flags & 0x20) != 0;
            }
            CHECK((int)st->hurts == hurts0 + 1, "needle: no second hit while invulnerable (%d hits)", (int)st->hurts - hurts0);
            P.x -= 0x4000; /* step off the spikes for the recovery check */
            for (n = 0; n < 4; n++)
                frame(0);
            CHECK(blink > 40 && !(P.disp_flags & 0x20) && pc && (P.invuln != 0 || pc->body.def == 2),
                  "needle: blink during invulnerability (%d frames), hurt box back to def 2 (%x)", blink, pc ? pc->body.def : 0);
        }
    }
    /* item boxes: ItemBox_OnHit on contact (no respawn invulnerability), once, ItemBox_GiveItem by p6;
     * markers: Marker_OnHit once, respawn point (x, y - (box_b + 1)), tension +800 */
    {
        int nb = 0, nb_ok = 0, nm = 0, nm_ok = 0, k, ok;
        for (k = 0; k < 2; k++) {
            for (i = 0; i < h->num_objects && (k ? nm : nb) < 4; i++) {
                const rlvl_object_t *e = &((const rlvl_object_t *)rlvl_section(g_level, RLVL_SEC_OBJECTS, NULL))[i];
                Obj *o;
                const ObjBox *bx;
                s32 cx, cy;
                s16 r0;
                if (k == 0 ? e->type != 0x30 : (e->type != 0x6e || (e->flags & 0xff) <= g_obj_stage.check_idx))
                    continue;
                place_player(e->x, e->y - 64);
                P.move_flags |= 0x10;
                frame(0);
                o = live_near(e->type, e->x, e->y);
                if (o == NULL || (o->h.obj_flags & 2))
                    continue;
                bx = &o->coll[0];
                cx = (o->h.x >> 8) + ((bx->flags & 1) ? -(bx->l + bx->r) / 2 : (bx->l + bx->r) / 2);
                cy = (o->h.y >> 8) + ((bx->flags & 2) ? -(bx->t + bx->b) / 2 : (bx->t + bx->b) / 2);
                c = k ? st->checkpoints : st->item_boxes;
                place_player(cx, cy);
                P.move_flags |= 0x10;
                r0 = P.rings;
                for (n = 0; n < 30 && (k ? st->checkpoints : st->item_boxes) == c; n++) {
                    if (n == 15) {
                        place_player(cx, cy - 24);
                        P.move_flags |= 0x10;
                    }
                    frame(0);
                }
                if (k == 0) {
                    int p6 = (int)(s8)o->prm->p6, dr = P.rings - r0;
                    int give = p6 == 1 ? dr == 5 : 1, t;
                    if (p6 == 0) /* data_ov000_02366b44[Rand() & 7] */
                        for (give = 0, t = 0; t < 8; t++)
                            give |= dr == g_obj.data.itembox_rings[t];
                    for (n = 0; n < 20; n++) {
                        P.x = cx << 8, P.y = cy << 8;
                        frame(0);
                    }
                    nb++;
                    ok = st->item_boxes == c + 1 && give;
                    nb_ok += ok;
                    if (!ok)
                        printf("  item box at (%d,%d) p6 %d: broken %u, rings +%d\n", e->x, e->y, p6, st->item_boxes - c, dr);
                } else {
                    s32 wy = (o->h.y >> 8) - (s8)(P.box_b + 1);
                    if (o->h.disp_flags & 2)
                        wy = (o->h.y >> 8) + (s8)(P.box_b + 1);
                    nm++;
                    ok = st->checkpoints == c + 1 && g_dcp_world.check_x == (o->h.x >> 8) && g_dcp_world.check_y == wy &&
                         g_obj_stage.check_idx == (u8)(e->flags & 0xff);
                    for (n = 0; n < 20; n++) {
                        P.x = cx << 8, P.y = cy << 8;
                        frame(0);
                    }
                    ok = ok && st->checkpoints == c + 1;
                    nm_ok += ok;
                    if (!ok)
                        printf("  marker at (%d,%d): passed %u, respawn %d,%d (want %d,%d)\n", e->x, e->y, st->checkpoints - c,
                               g_dcp_world.check_x, g_dcp_world.check_y, o->h.x >> 8, wy);
                }
            }
        }
        printf("  item boxes: %d of %d; markers: %d of %d\n", nb_ok, nb, nm_ok, nm);
        CHECK(nb_ok == nb, "every item box breaks once on contact and gives its item");
        CHECK(nm_ok == nm, "every marker passes once and sets the respawn point");
    }
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

static int ev_n(const char *name) {
    int i;
    for (i = 0; i < NEV && g_ev[i].name[0]; i++)
        if (!strcmp(g_ev[i].name, name))
            return g_ev[i].n;
    return 0;
}
/* the centre of an object's coll[0] box in world pixels (ObjRect flags 1/2 mirror it) */
static void box_centre(const Obj *o, s32 *cx, s32 *cy) {
    const ObjBox *bx = &o->coll[0];
    *cx = (o->h.x >> 8) + ((bx->flags & 1) ? -(bx->l + bx->r) / 2 : (bx->l + bx->r) / 2);
    *cy = (o->h.y >> 8) + ((bx->flags & 2) ? -(bx->t + bx->b) / 2 : (bx->t + bx->b) / 2);
}
/* spawn the nth object of a type; NULL when absent or not spawned */
static Obj *spawn_nth(u16 type, int nth, const rlvl_object_t **pe) {
    const rlvl_object_t *e = find_obj(type, nth);
    Obj *o;
    if (e == NULL)
        return NULL;
    place_player(e->x, e->y - 64);
    P.move_flags |= 0x10;
    frame(0);
    o = live_near(type, e->x, e->y);
    if (pe)
        *pe = e;
    return o && !(o->h.obj_flags & 2) ? o : NULL;
}
/* hold the player on (cx, cy) for up to n frames with the given ground speed and ground/air flag
 * (the boxes are checked at the start of the next frame, with the flags as they are then); returns
 * the frames until the event counter moved, or -1 */
static int hold_contact(s32 cx, s32 cy, s16 gnd, int grounded, int n, const char *ev) {
    int k, e0 = ev_n(ev);
    for (k = 0; k < n; k++) {
        P.x = cx << 8, P.y = cy << 8;
        P.gnd_vel = gnd, P.vel_x = gnd, P.vel_y = 0;
        if (grounded)
            P.move_flags = (P.move_flags | 1) & ~0x10u;
        else
            P.move_flags = (P.move_flags & ~1u) | 0x10;
        frame(0);
        if (ev_n(ev) != e0)
            return k + 1;
    }
    return -1;
}

/* the rest of item C, class by class: dash rings, trampolines, goal ring, corkscrews, ropes,
 * breakable rocks and walls, enemies */
static void test_audit_classes(void) {
    int i, n, k, tot, good;
    printf("[audit] dash rings / trampolines / goal / corkscrews / ropes / rocks / walls / enemies\n");
    /* DashC_OnHit: v = (cos, sin)(dir << 13) * 0x800 >> 12; Player_Action_DashRing: position = ring,
     * Player_Action_GimmickLaunch (anim 0x18, or 0x13 for vx == 0; Player_State_InAir turns them into
     * 0x19 / 0x14 in the same frame when falling faster than 0x1c0 / 0x40), gravity off (move_flags 0x80),
     * dash timer 0x1e; DashCT (u2A0 & 1): Player_StartLaunchTrick (anim 0x38) */
    {
        static const u16 types[] = { 0x63, 0x64, 0x65, 0x66, 0x6a, 0x6b, 0x6c, 0x6d };
        tot = good = 0;
        for (i = 0; i < 8; i++)
            for (k = 0; k < 3; k++) {
                Obj *o = spawn_nth(types[i], k, NULL);
                s32 cx, cy;
                s16 vx, vy;
                u16 ang;
                int c0, ok;
                if (o == NULL)
                    continue;
                ang = (u16)((u8)o->u2A4 << 13);
                vx = (s16)((dcp_cos(ang) * 0x800) >> 12);
                vy = (s16)((dcp_sin(ang) * 0x800) >> 12);
                box_centre(o, &cx, &cy);
                /* (the first frame can still see the box registered at the spawn position: hold until
                 * this ring fired) */
                for (n = -1, c0 = 0; c0 < 4; c0++) {
                    g_ring_a = -1;
                    n = hold_contact(cx, cy, 0, 0, 20, "dash_ring");
                    if (n > 0 && g_ring_a == types[i] && g_ring_b == o->u2A4)
                        break;
                }
                c0 = (int)objects_stats()->dash_rings - 1;
                if (P.pf & 0x4000000) {
                    vx = (s16)((vx >> 1) + (vx >> 2));
                    vy = (s16)((vy >> 1) + (vy >> 2));
                }
                tot++;
                /* (the player task has run once: gravity stays off, ApplyAirInput does not clamp 0x800) */
                ok = n > 0 && (int)objects_stats()->dash_rings > c0 && P.vel_x == vx && P.vel_y == vy &&
                     !(P.move_flags & 0x80) && P.dash_timer >= 0x1c && P.dash_timer <= 0x1e &&
                     P.anim == ((o->u2A0 & 1) ? 0x38 : vx != 0 ? (vy > 0x1c0 ? 0x19 : 0x18) : (vy > 0x40 ? 0x14 : 0x13));
                good += ok;
                if (!ok)
                    printf("  dash ring 0x%02x dir %d at (%d,%d): n %d vel %d,%d (want %d,%d) anim %x timer %d mf %x\n", types[i],
                           (int)o->u2A4, o->h.x >> 8, o->h.y >> 8, n, P.vel_x, P.vel_y, vx, vy, P.anim, P.dash_timer, P.move_flags);
            }
        printf("  dash rings: %d of %d exact\n", good, tot);
        CHECK(good == tot, "every dash ring launches with the DashC_OnHit velocity and Player_Action_DashRing state");
    }
    /* rail dash panels (0x5c): DashP_OnHit only while grinding (anim 0x1a..0x20), direction by 5C & 1
     * (from flags & 1), Player_Action_DashPanel (airborne branch keeps the larger vel_x) */
    tot = good = 0;
    for (k = 0; k < 4; k++) {
        Obj *o = spawn_nth(0x5c, k, NULL);
        s32 cx, cy;
        int walk, grind, e0, ok, g0;
        s16 want;
        if (o == NULL)
            continue;
        box_centre(o, &cx, &cy);
        want = (o->h.disp_flags & 1) ? (s16)-P.top : P.top;
        /* in the air over it (fall anim): no boost; standing on the rail: Player_CheckStartGrind
         * starts the grind (anim 0x1a) and the panel fires */
        e0 = g_rail_panels;
        place_player(cx, cy);
        P.move_flags |= 0x10;
        frame(0);
        for (n = 0, walk = -1; n < 4 && walk < 0; n++) {
            P.x = cx << 8, P.y = cy << 8, P.gnd_vel = P.vel_x = P.vel_y = 0;
            frame(0);
            if (g_rail_panels != e0)
                walk = n + 1;
        }
        /* (the rail is on layer A or B, or behind a MapObj40 0x59 rail-layer override 1..3) */
        g0 = ev_n("grind");
        for (grind = -1, n = 0; n < 5 && grind < 0; n++) {
            int f;
            place_player(cx, (o->h.y >> 8) - 20);
            P.move_flags |= 0x10;
            if (n == 1)
                P.obj_flags |= 1;
            else if (n >= 2)
                P.layer_ovr = (u8)(n - 1), P.layer_set = 2;
            for (f = 0; f < 30 && grind < 0; f++) {
                frame(0);
                if (g_rail_panels != e0)
                    grind = f + 1;
            }
        }
        if (grind < 0 && ev_n("grind") == g0) {
            printf("  rail dash panel at (%d,%d): no grind rail under it on layers A/B or rail layers 1..3 (skipped)\n",
                   o->h.x >> 8, o->h.y >> 8);
            continue;
        }
        tot++;
        ok = walk < 0 && grind > 0 && P.dash_timer >= 0x7e && (P.gnd_vel == want || P.vel_x == want);
        good += ok;
        if (!ok)
            printf("  rail dash panel at (%d,%d) df %x: walking %d grinding %d gnd %d vel %d (want %d) timer %d; player (%d,%d) anim %x mf %x\n",
                   o->h.x >> 8, o->h.y >> 8, o->h.disp_flags, walk, grind, P.gnd_vel, P.vel_x, want, P.dash_timer, P.x >> 8,
                   P.y >> 8, P.anim, P.move_flags);
    }
    if (tot)
        printf("  rail dash panels: %d of %d\n", good, tot);
    CHECK(good == tot, "rail dash panels boost a grinding player only, +-top by their direction");
    /* Trampoline_OnHit: v = -(my + my/8 + my/16) clamped to -0xc00..-0x780 from the fall (move_y),
     * Player_Action_SpringLaunch(0, v) */
    tot = good = 0;
    for (k = 0; k < 4; k++) {
        Obj *o = spawn_nth(0x55, k, NULL);
        s32 cx, cy;
        s16 my = 0, v;
        int e0, ok;
        if (o == NULL)
            continue;
        box_centre(o, &cx, &cy);
        place_player(cx, cy - 40);
        P.move_flags |= 0x10;
        P.vel_y = (s16)(0x400 + k * 0x200);
        e0 = ev_n("trampoline");
        for (n = 0; n < 30 && ev_n("trampoline") == e0; n++) {
            my = P.move_y;
            frame(0);
        }
        v = (s16)-(my + (my >> 3) + (my >> 4));
        v = v < -0xc00 ? -0xc00 : v > -0x780 ? -0x780 : v;
        tot++;
        ok = ev_n("trampoline") == e0 + 1 && P.vel_y == (s16)(v + P.gravity) && (P.anim == 0x13 || P.anim == 0x14);
        good += ok;
        if (!ok)
            printf("  trampoline at (%d,%d): fired %d, fall %d, vel_y %d (want %d + gravity) anim %x\n", o->h.x >> 8, o->h.y >> 8,
                   ev_n("trampoline") - e0, my, P.vel_y, v, P.anim);
    }
    if (tot)
        printf("  trampolines: %d of %d exact\n", good, tot);
    CHECK(good == tot, "every trampoline launches with the Trampoline_OnHit velocity from the fall speed");
    /* GoalRing_OnHit: once (obj_flags 2 after the first hit) */
    {
        Obj *o = spawn_nth(0x71, 0, NULL);
        if (o != NULL) {
            s32 cx, cy;
            int g0 = g_goal_hook;
            box_centre(o, &cx, &cy);
            n = hold_contact(cx, cy, 0, 0, 20, "goal");
            for (k = 0; k < 20; k++) {
                P.x = cx << 8, P.y = cy << 8;
                frame(0);
            }
            printf("  goal ring at (%d,%d): goal after %d frames, hooks %d\n", o->h.x >> 8, o->h.y >> 8, n, g_goal_hook - g0);
            CHECK(n > 0 && g_goal_hook == g0 + 1 && (o->h.obj_flags & 2), "goal ring: GoalRing_OnHit once");
        }
    }
    /* CorkscrewTrigger_OnHit: gnd_vel >= run_thr[2] in its direction (flags 1: leftwards); flags 0x80:
     * grinding only (not here) */
    tot = good = 0;
    for (i = 0; i < 2; i++)
        for (k = 0; k < 3; k++) {
            const rlvl_object_t *e;
            Obj *o = spawn_nth((u16)(0x68 + i), k, &e);
            s32 cx, cy;
            s16 lim;
            int slow, fast, ok;
            if (o == NULL || (o->prm->flags & 0x80))
                continue;
            box_centre(o, &cx, &cy);
            lim = (s16)((o->prm->flags & 1) ? -P.run_thr[2] : P.run_thr[2]);
            slow = hold_contact(cx, cy, (s16)(lim - ((o->prm->flags & 1) ? -0x40 : 0x40)), 1, 6, "corkscrew");
            place_player(cx, cy);
            fast = hold_contact(cx, cy, lim, 1, 6, "corkscrew");
            tot++;
            ok = slow < 0 && fast > 0;
            good += ok;
            if (!ok)
                printf("  corkscrew 0x%02x at (%d,%d) flags %x: below the run threshold %d, at it %d\n", 0x68 + i, e->x, e->y,
                       o->prm->flags, slow, fast);
        }
    if (tot)
        printf("  corkscrews: %d of %d\n", good, tot);
    CHECK(good == tot, "corkscrews start at run_thr[2] in their direction only");
    /* Rope_OnHit: grounded, |gnd_vel| >= run_thr[2], one rider */
    tot = good = 0;
    for (k = 0; k < 3; k++) {
        Obj *o = spawn_nth(0x58, k, NULL);
        s32 cx, cy;
        int slow, air, fast, ok;
        if (o == NULL)
            continue;
        cx = o->coll[0].x + ((o->coll[0].l + o->coll[0].r) >> 1);
        cy = o->coll[0].y + ((o->coll[0].t + o->coll[0].b) >> 1);
        slow = hold_contact(cx, cy, (s16)(P.run_thr[2] - 0x40), 1, 4, "rope");
        air = hold_contact(cx, cy, P.run_thr[2], 0, 4, "rope");
        place_player(cx, cy);
        fast = hold_contact(cx, cy, P.run_thr[2], 1, 4, "rope");
        tot++;
        ok = slow < 0 && air < 0 && fast > 0 && o->target == &P && P.anim == 0x24;
        good += ok;
        if (!ok)
            printf("  rope at (%d,%d): slow %d air %d fast %d target %d anim %x\n", o->h.x >> 8, o->h.y >> 8, slow, air, fast,
                   o->target == &P, P.anim);
    }
    if (tot)
        printf("  ropes: %d of %d\n", good, tot);
    CHECK(good == tot, "ropes catch a grounded player at run_thr[2] or faster, not slower, not in the air");
    /* RockBreak_OnHit: airborne only, Player_Action_AttackBounce (vel_y -0x300) */
    tot = good = 0;
    for (k = 0; k < 3; k++) {
        Obj *o = spawn_nth(0x6f, k, NULL);
        s32 cx, cy;
        int gnd, air, ok;
        if (o == NULL)
            continue;
        box_centre(o, &cx, &cy);
        /* (the rock is solid; its box reaches 6 px above the solid part, which the son.bac standing box
         * does, but only grounded, and the fall box (-16..4) does not; the jump (ball) box does: stand
         * on it, then jump on the spot) */
        {
            s32 e0 = ev_n("rock_break");
            place_player(cx, (o->h.y >> 8) + o->cmap.oy - 20);
            for (n = 0, gnd = -1; n < 30 && gnd < 0; n++) {
                frame(0);
                if (ev_n("rock_break") != e0)
                    gnd = n + 1;
            }
            for (n = 0, air = -1; n < 90 && air < 0; n++) {
                frame((u16)(n < 4 ? DCP_KEY_A : 0));
                if (ev_n("rock_break") != e0)
                    air = n + 1;
            }
        }
        tot++;
        ok = gnd < 0 && air > 0 && P.vel_y == (s16)(-0x300 + P.gravity) && (o->h.obj_flags & 2);
        good += ok;
        if (!ok)
            printf("  rock at (%d,%d): grounded %d airborne %d vel_y %d anim %x\n", o->h.x >> 8, o->h.y >> 8, gnd, air, P.vel_y, P.anim);
    }
    if (tot)
        printf("  rocks: %d of %d\n", good, tot);
    CHECK(good == tot, "rocks break from the air only, with the AttackBounce (-0x300)");
    /* WallBreak_OnHit: mode (flags & 3) 0 any speed, 1 > run_thr[2], 2 > run_thr[4]; hit-stop 4 */
    tot = good = 0;
    for (k = 0; k < 4; k++) {
        Obj *o = spawn_nth(0x77, k, NULL);
        s32 cx, cy;
        int mode, slow = -1, fast = -1, ok;
        s16 lim;
        if (o == NULL)
            continue;
        mode = o->prm->flags & 3;
        lim = mode == 1 ? P.run_thr[2] : mode == 2 ? P.run_thr[4] : 0;
        box_centre(o, &cx, &cy);
        {
            /* held on its box centre, or run into it on the floor from either side at a held ground speed
             * (the solid ones push a placed player out): every way at the limit first, then above it */
            int pass, way;
            for (pass = 0; pass < 2; pass++)
                for (way = 0; way < 3; way++) {
                    /* (OnHit reads the speed after the previous frame's slope force: a margin of 0x40) */
                    s16 v = pass == 0 ? (s16)(lim - 0x40) : (s16)(lim + 0x40);
                    int e0 = ev_n("wall_break"), r = -1;
                    if ((pass == 0 && mode == 0) || (pass == 1 && fast > 0))
                        continue;
                    if (way == 0) {
                        r = hold_contact(cx, cy, v, 1, 4, "wall_break");
                    } else {
                        if (way == 2)
                            v = (s16)-v;
                        place_on_floor(way == 1 ? cx - 48 : cx + 48, o->h.y >> 8);
                        for (n = 0; n < 40 && r < 0; n++) {
                            P.gnd_vel = v;
                            frame(0);
                            if (ev_n("wall_break") != e0)
                                r = n + 1;
                        }
                    }
                    /* (a dash panel in the run-up gives vel_x = top, which WallBreak_OnHit also takes) */
                    if (pass == 0 && r > 0 && DCP_ABS(P.vel_x) <= lim && DCP_ABS(P.gnd_vel) <= lim)
                        slow = r;
                    else if (pass == 0 && r > 0)
                        fast = r;
                    else if (pass == 1)
                        fast = r;
                }
        }
        tot++;
        ok = slow < 0 && fast > 0 && P.hitstop >= 3;
        good += ok;
        if (!ok)
            printf("  wall at (%d,%d) mode %d: at the limit %d above %d hitstop %d\n", o->h.x >> 8, o->h.y >> 8, mode, slow, fast, P.hitstop);
    }
    if (tot)
        printf("  breakable walls: %d of %d\n", good, tot);
    CHECK(good == tot, "breakable walls break above their WallBreak_OnHit speed only");
    /* enemies: a jump (Player_Action_Jump, attack box) from above destroys the nearest live enemy of a
     * placed pawn / flapper group and Player_Action_AttackBounce throws the player up */
    {
        static const u16 et[] = { 0x00, 0x01, 0x03, 0x07, 0x08, 0x09, 0x0a, 0x0d, 0x0e, 0x0f, 0x14, 0x15,
                                  0x02, 0x04, 0x0b, 0x0c, 0x10, 0x11, 0x12 };
        tot = good = 0;
        for (i = 0; i < (int)(sizeof(et) / sizeof(et[0])) && tot < 6; i++)
            for (k = 0; k < 2 && tot < 6; k++) {
                const rlvl_object_t *e = find_obj(et[i], k);
                const ObjStats *st = objects_stats();
                Obj *q, *best = NULL;
                s32 bd = 0x7fffffff, d, cx, cy;
                u32 d0, h0;
                int ok, bounced = 0, v0, b0;
                if (e == NULL)
                    continue;
                place_player(e->x, e->y - 80);
                P.move_flags |= 0x10;
                P.invuln = 0;
                for (n = 0; n < 3; n++)
                    frame(0);
                /* the enemy itself: the nearest live object with a hurt box (def 0xa) */
                for (q = g_obj.head; q; q = q->next)
                    if (q->used && !q->is_dec && (q->coll[0].def & 0xa) == 0xa && (q->coll[0].flags & 4)) {
                        d = DCP_ABS((q->h.x >> 8) - e->x) + DCP_ABS((q->h.y >> 8) - e->y);
                        if (d < bd)
                            bd = d, best = q;
                    }
                if (best == NULL || bd > 96)
                    continue;
                /* (drop-in pawns, p9 != 0, wait for their enemy-room trigger) */
                if (best->prm && best->prm->p9 != 0 && (et[i] == 0 || et[i] == 0x14 || et[i] == 0x15))
                    continue;
                d0 = st->enemies_destroyed + st->knock_aways;
                h0 = st->hurts;
                v0 = best->vitality;
                b0 = ev_n("pawn_bounce");
                for (n = 0; n < 60 && st->enemies_destroyed + st->knock_aways == d0 && st->hurts == h0; n++) {
                    if (n % 20 == 0) {
                        box_centre(best, &cx, &cy);
                        place_player(cx, cy - 36);
                        dcp_act_jump(&P);
                        P.vel_x = 0, P.vel_y = 0x300;
                    }
                    frame(0);
                    bounced |= P.vel_y < 0;
                }
                (void)bounced;
                tot++;
                /* destroyed (Player_Action_AttackBounce when still airborne at the hit), or a hit taken by
                 * a two-hit enemy (Egg Hammer, vitality 2), or the Rhino's back spring
                 * (EnemyPawnGroup_OnHit_Box2_Bounce) */
                ok = st->hurts == h0 && ((st->enemies_destroyed + st->knock_aways > d0) || ev_n("pawn_bounce") != b0 ||
                                         (best->used && best->vitality < v0));
                good += ok;
                if (!ok)
                    printf("  enemy 0x%02x at (%d,%d) (obj kind %d at %d,%d): destroyed %u hurt %u vel_y %d\n", et[i], e->x, e->y,
                           best->kind, best->h.x >> 8, best->h.y >> 8, st->enemies_destroyed + st->knock_aways - d0, st->hurts - h0,
                           P.vel_y);
            }
        if (tot)
            printf("  enemies: %d of %d hit from a jump\n", good, tot);
        CHECK(good == tot, "a jump onto an enemy hits it (destroyed, one of two hits, or the Rhino spring)");
    }
}

/* ------------------------------------------------------- core gameplay (tricks, homing, rings,
 * lives, death): each against the formula of the decompiled function named in the check */
static struct { s32 a[64]; int n; } g_score;
static void (*g_prev_score)(void *, DcPlayer *, s32);
static void hk_score(void *ctx, DcPlayer *p, s32 v) {
    if (g_prev_score)
        g_prev_score(ctx, p, v);
    if (g_score.n < 64)
        g_score.a[g_score.n++] = v;
}
static struct { s16 vx[80], vy[80]; u16 f[80]; int n; } g_rspawn;
static void (*g_prev_spawn)(void *, DcPlayer *, s32, s32, s16, s16, u16);
static void hk_spawn(void *ctx, DcPlayer *p, s32 x, s32 y, s16 vx, s16 vy, u16 f) {
    if (g_prev_spawn)
        g_prev_spawn(ctx, p, x, y, vx, vy, f);
    if (g_rspawn.n < 80)
        g_rspawn.vx[g_rspawn.n] = vx, g_rspawn.vy[g_rspawn.n] = vy, g_rspawn.f[g_rspawn.n] = f, g_rspawn.n++;
}
static int g_lives_added;
static void (*g_prev_life)(void *, DcPlayer *, s32);
static void hk_life(void *ctx, DcPlayer *p, s32 n) {
    if (g_prev_life)
        g_prev_life(ctx, p, n);
    g_lives_added += n;
}
static void gameplay_hooks(void) {
    if (g_dcp_hooks.score != hk_score)
        g_prev_score = g_dcp_hooks.score, g_dcp_hooks.score = hk_score;
    if (g_dcp_hooks.spawn_ring != hk_spawn)
        g_prev_spawn = g_dcp_hooks.spawn_ring, g_dcp_hooks.spawn_ring = hk_spawn;
    if (g_dcp_hooks.add_life != hk_life)
        g_prev_life = g_dcp_hooks.add_life, g_dcp_hooks.add_life = hk_life;
}
/* a spring launch to trick from (the first vertical spring of the act) */
static int spring_launch(void) {
    const ObjStats *st = objects_stats();
    Obj *o;
    s32 cx, cy;
    u32 c;
    int n, k;
    for (k = 0; k < 6; k++) {
        o = spawn_nth(0x43, k, NULL);
        if (o == NULL)
            continue;
        box_centre(o, &cx, &cy);
        c = st->springs;
        o->prm->pA = 0; /* the trick counter of the source object (Player_SetSourceMapObject) */
        place_player(cx, cy);
        for (n = 0; n < 30 && st->springs == c; n++)
            frame(0);
        if (st->springs != c && (P.pf & 2))
            return 1;
    }
    return 0;
}
static void test_tricks(void) {
    int ch, n;
    u8 save = g_chara;
    printf("[tricks] Player_Action_TrickForward / TrickUp / TrickCombo, Player_AddTrickScore, tension\n");
    gameplay_hooks();
    for (ch = 0; ch < 2; ch++) {
        s16 vx0, vx, vy, t0;
        int ok;
        g_chara = (u8)ch;
        if (!spring_launch()) {
            printf("  no spring launch in this act\n");
            break;
        }
        /* R in the air after a launch (pf & 2): forward trick */
        g_score.n = 0;
        vx0 = P.vel_x;
        frame(DCP_KEY_R);
        vx = P.vel_x, vy = P.vel_y;
        if (ch == 0) {
            /* Sonic: vel_x += (2 * jump * sin(+-0x4000) << 4) >> 16 = +-2 * jump, vel_y 0, dash timer 0xc */
            s16 t = (s16)(((P.disp_flags & 1) ? -1 : 1) * 2 * P.jump);
            s16 want = (s16)(((t > 0 && vx0 < 0) || (t < 0 && vx0 > 0) ? 0 : vx0) + t);
            ok = P.anim == 0x37 && vx == want && vy == P.gravity && (P.pf & 8);
            printf("  Sonic forward trick: anim %x vel %d,%d (want %d,%d) score %d,%d\n", P.anim, vx, vy, want, P.gravity,
                   g_score.n > 0 ? g_score.a[0] : -1, g_score.n > 1 ? g_score.a[1] : -1);
        } else {
            /* Blaze: vel_x = +-2 * boost_top, frozen in the pose for 0xc frames (air_lock), then vel_x
             * = the saved speed (or 1/8 of the dash below 0x300) */
            s16 want = (s16)(((P.disp_flags & 1) ? -1 : 1) * 2 * P.boost_top);
            ok = P.anim == 0x37 && vx == want && P.air_lock >= 0xa && (P.pf & 8);
            printf("  Blaze forward trick: anim %x vel %d (want %d) air lock %d score %d,%d\n", P.anim, vx, want, P.air_lock,
                   g_score.n > 0 ? g_score.a[0] : -1, g_score.n > 1 ? g_score.a[1] : -1);
        }
        CHECK(ok && g_score.n == 2 && g_score.a[0] == 0 && g_score.a[1] == (200 >> P.trick_lvl),
              "%s forward trick: pose 0x37, velocity, pf 8, score 0 + 200 (first trick)", ch ? "Blaze" : "Sonic");
        /* combo: A after more than 0x18 frames in the pose -> 0x39 (+400 tension), A -> 0x3a (+400),
         * B (pf 0x4000, input 1) -> 0x3b (+0x280); each trick scores (100 / 200 / 300) >> lvl + 200 >> lvl */
        {
            /* (trick_input: B = 2 -> 0x39 / 0x3a, A = 1 with pf 0x4000 -> 0x3b) */
            static const u16 keys[3] = { DCP_KEY_B, DCP_KEY_B, DCP_KEY_A };
            static const u8 anims[3] = { 0x39, 0x3a, 0x3b };
            static const s16 tens[3] = { 400, 400, 0x280 };
            int step, lvl = P.trick_lvl, got = 0, sp0 = g_spring_n;
            for (step = 0; step < 3; step++) {
                s16 before = P.tension;
                u8 a0 = P.anim;
                int early = 0;
                /* press every other frame until the pose changes: not before 0x18 frames in a pose */
                g_score.n = 0;
                for (n = 0; n < 0x40 && P.anim == a0 && !(P.move_flags & 1); n++) {
                    P.invuln = 0x40; /* (no enemy hit in the middle of the combo) */
                    frame((n & 1) ? keys[step] : 0);
                    if (P.anim != a0 && n + 1 <= 0x18 && a0 >= 0x37)
                        early = 1;
                }
                if ((P.move_flags & 1) || g_spring_n != sp0)
                    break; /* (landed, or carried onto another spring: a fresh launch) */
                lvl = P.trick_lvl;
                got -= early;
                t0 = (s16)(P.tension - before);
                printf("  combo %d: anim %x after %d frames%s, tension +%d (want %d) score", step, P.anim, n, early ? " (early)" : "", t0,
                       tens[step] >> lvl);
                for (n = 0; n < g_score.n; n++)
                    printf(" %d", g_score.a[n]);
                printf("\n");
                got += P.anim == anims[step] && (t0 == (tens[step] >> lvl) || P.tension == 0x12c0) && g_score.n >= 2;
            }
            if (step < 2)
                printf("  (%s: no room for a combo after this spring: %d steps)\n", ch ? "Blaze" : "Sonic", step);
            CHECK(got == step,
                  "%s trick combo 0x39 -> 0x3a (-> 0x3b) with the TrickCombo tension, timing and scores (%d of %d before landing)",
                  ch ? "Blaze" : "Sonic", got, step);
        }
        /* Up + R: up trick, vel_y = -jump (Blaze: -2 * jump), pose 0x35 */
        if (spring_launch()) {
            frame((u16)(DCP_KEY_UP | DCP_KEY_R));
            vy = P.vel_y;
            /* (Blaze: hit-stop 0x44 freezes the move in that frame, so no gravity yet) */
            ok = P.anim == 0x35 && (vy == (s16)((ch ? -2 * P.jump : -P.jump) + P.gravity) || (ch && vy == -2 * P.jump));
            printf("  %s up trick: anim %x vel_y %d (want %d + gravity)\n", ch ? "Blaze" : "Sonic", P.anim, vy,
                   ch ? -2 * P.jump : -P.jump);
            CHECK(ok, "%s up trick: pose 0x35, Player_Action_TrickUp velocity", ch ? "Blaze" : "Sonic");
        }
        /* no trick without a launch: R in a plain jump is the air action (air dash / hover) */
        place_on_floor(find_obj(0x72, 0) ? find_obj(0x72, 0)->x : 200, find_obj(0x72, 0) ? find_obj(0x72, 0)->y : 800);
        for (n = 0; n < 4; n++)
            frame(0);
        frame(DCP_KEY_A);
        for (n = 0; n < 6; n++)
            frame(DCP_KEY_A);
        frame(DCP_KEY_R);
        CHECK(P.anim < 0x35 || P.anim > 0x3b, "%s: R in a jump is not a trick (anim %x)", ch ? "Blaze" : "Sonic", P.anim);
    }
    g_chara = save;
}

/* Player_FindHomingTarget (nearest type-2 box, no range), Player_Action_HomingAttack (d^2 < 0x8000,
 * target not above), Player_State_HomingAttack (vel = (cos, sin) >> 1), AttackBounce, and
 * MapObject_OnHit_Default: Player_AddTensionCombo(400): +400, +600, +800 within 0x60 frames */
static void test_homing(void) {
    int i, k, n, kills = 0;
    s16 t[4];
    printf("[homing] Player_FindHomingTarget / Action_HomingAttack / AddTensionCombo chain\n");
    if (g_chara == 1) {
        printf("  (Blaze has no homing attack: Player_Action_Hover)\n");
        return;
    }
    static const u16 et[] = { 0x00, 0x01, 0x03, 0x07, 0x09, 0x0d, 0x0f, 0x14, 0x02, 0x04, 0x0b, 0x0c, 0x10, 0x11, 0x12 };
    for (k = 0; k < (int)(sizeof(et) / sizeof(et[0])) * 4 && kills < 3; k++) {
        const rlvl_object_t *e = find_obj(et[k / 4], k % 4);
        Obj *q, *best = NULL;
        s32 bd = 0x7fffffff, d;
        s16 tb;
        u32 e0;
        if (e == NULL)
            continue;
        {
            u16 cb = P.combo, ct = P.combo_timer;
            place_player(e->x, e->y - 80);
            P.combo = cb, P.combo_timer = ct;
        }
        P.move_flags |= 0x10;
        if (kills == 0)
            P.tension = 0, P.combo = 0, P.combo_timer = 0;
        else
            P.tension = 0;
        for (n = 0; n < 2; n++)
            frame(0);
        for (q = g_obj.head; q; q = q->next)
            if (q->used && !q->is_dec && q->h.type == 2 && !(q->h.obj_flags & 4)) {
                d = DCP_ABS((q->h.x >> 8) - e->x) + DCP_ABS((q->h.y >> 8) - e->y);
                if (d < bd)
                    bd = d, best = q;
            }
        if (best == NULL || bd > 64)
            continue;
        /* 60 px above it, in a jump: the search picks it, the homing attack heads at it */
        {
            /* (the chain: Player_AddTensionCombo's count and timer carry over) */
            u16 cb = P.combo, ct = P.combo_timer;
            place_player(best->h.x >> 8, (best->h.y >> 8) - 60);
            P.combo = cb, P.combo_timer = ct;
        }
        dcp_act_jump(&P);
        P.vel_y = 0x100;
        frame(0);
        tb = P.tension;
        e0 = objects_stats()->enemies_destroyed;
        frame(DCP_KEY_R);
        frame(0);
        if (P.state != dcp_state_homing && objects_stats()->enemies_destroyed == e0) {
            printf("  homing not started at (%d,%d): homing %p\n", e->x, e->y, (void *)P.homing);
            continue;
        }
        {
            u16 a = (u16)dcp_atan2((best->h.y - P.y) >> 8, (best->h.x - P.x) >> 8);
            printf("  homing at (%d,%d): vel %d,%d (cos/sin >> 1: %d,%d) anim %x\n", best->h.x >> 8, best->h.y >> 8, P.vel_x,
                   P.vel_y, dcp_cos(a) >> 1, dcp_sin(a) >> 1, P.anim);
        }
        for (n = 0; n < 0x20 && objects_stats()->enemies_destroyed == e0; n++)
            frame(0);
        if (objects_stats()->enemies_destroyed == e0)
            continue;
        t[kills] = (s16)(P.tension - tb);
        kills++;
    }
    printf("  tension per kill:");
    for (i = 0; i < kills; i++)
        printf(" +%d", t[i]);
    printf("\n");
    if (kills < 2) {
        printf("  (fewer than two homing kills set up in this act: chain not checked)\n");
        return;
    }
    CHECK(t[0] == 400 && t[1] == 600 && (kills < 3 || t[2] == 800),
          "Player_AddTensionCombo chain: +400, +600 (, +800) within 0x60 frames");
}

/* Player_OnHurt -> RingManager_ScatterRings: min(rings, 64) rings, the arc of the original loop,
 * level +1 per scatter (max 4), back to 0 once every scattered ring is gone; RingManager_SpawnRing
 * lifetime 0x100 + (Rand & 0x1f), no pickup while the timer is >= 0xd8, blinking (timer & 2) in the
 * last 0x20 frames; Player_AddRings: a life at every 100 */
static void scatter_expect(int nrings, u8 level, s16 *evx, s16 *evy, int *cnt) {
    s32 ang = 0x488 + (level << 8), sh;
    s16 vx = 0, vy = 0;
    int i, n = nrings > 0x40 ? 0x40 : nrings, c = 0;
    for (i = 0; i < 0x40; i++) {
        if (ang >= 0) {
            sh = ang >> 8;
            if (sh >= 6)
                sh = 9 - sh;
            vx = (s16)(dcp_sin((u16)((ang & 0xff) << 8)) >> sh);
            vy = (s16)(dcp_cos((u16)((ang & 0xff) << 8)) >> sh);
            vx -= vx / 4;
            vy -= vy / 4;
            ang = (ang + 0x10) | 0x80;
        }
        evx[c] = vx, evy[c] = vy;
        ang = -ang;
        vx = (s16)-vx;
        c++;
        if (c >= n)
            break;
    }
    *cnt = c;
}
static void test_rings_lives(void) {
    const rlvl_object_t *ps = find_obj(0x72, 0);
    s16 evx[64], evy[64];
    int n, cnt, ok, k, drawn_late = 0, blink = 0, picked_early = 0, max_alive = 0, win = 0;
    u8 lvl0, lvl1;
    printf("[rings] Player_OnHurt ring scatter, RingManager_SpawnRing lifetime, Player_AddRings lives\n");
    gameplay_hooks();
    if (ps == NULL)
        return;
    for (k = 0; k < 2; k++) {
        int want_n = k == 0 ? 30 : 100;
        place_on_floor(ps->x, ps->y);
        for (n = 0; n < 4; n++)
            frame(0);
        P.rings = (s16)want_n;
        P.invuln = 0;
        lvl0 = *objects_ring_scatter_level();
        g_rspawn.n = 0;
        dcp_on_hurt(&P, 0);
        lvl1 = *objects_ring_scatter_level();
        scatter_expect(want_n, lvl0, evx, evy, &cnt);
        ok = g_rspawn.n == cnt && P.rings == 0;
        for (n = 0; n < cnt && n < g_rspawn.n; n++)
            ok = ok && g_rspawn.vx[n] == evx[n] && g_rspawn.vy[n] == evy[n] && (g_rspawn.f[n] & 0x1000);
        printf("  %d rings, level %d: %d spawned (want %d), first (%d,%d) want (%d,%d), level after %d\n", want_n, lvl0,
               g_rspawn.n, cnt, g_rspawn.n ? g_rspawn.vx[0] : 0, g_rspawn.n ? g_rspawn.vy[0] : 0, evx[0], evy[0], lvl1);
        CHECK(ok && lvl1 == (lvl0 < 4 ? lvl0 + 1 : 4), "scatter of %d rings: count %d, the RingManager_ScatterRings arc, level +1",
              want_n, cnt);
        if (k == 0) {
            /* lifetime / blink / pickup delay, with the player held where the rings spawned */
            s32 hx = P.x, hy = P.y;
            for (n = 0; n < 0x140; n++) {
                int i, alive = 0, dr = 0;
                s32 x, y;
                int vf;
                if (n < 0x26) {
                    P.x = hx, P.y = hy, P.invuln = 0x40;
                }
                frame(0);
                for (i = 0; i < ring_draw_count(); i++)
                    if (ring_draw_get(i, &x, &y, &vf))
                        dr++;
                alive = objects_ring_count();
                if (alive > max_alive)
                    max_alive = alive;
                if (n < 0x26 && P.rings > 0)
                    picked_early = 1;
                if (n >= 0x100 - 0x20 && n < 0x11f && dr < alive)
                    blink++;
                if (n >= 0x100 - 0x20 && n < 0x11f && alive > 0)
                    win++;
                if (n >= 0x121)
                    drawn_late += alive;
            }
            printf("  scattered rings: no pickup in the first 0x26 frames %d, blink frames near the end %d, alive after 0x121 "
                   "frames %d, scatter level now %d\n",
                   !picked_early, blink, drawn_late, *objects_ring_scatter_level());
            /* (rings that leave the view end early: RingManager_Main view-out) */
            CHECK(!picked_early && (blink > 0 || win == 0) && drawn_late == 0,
                  "scattered rings: pickup delay, blink, lifetime 0x100 + 0..0x1f");
            /* (RingManager_Main: picking one scattered ring back up clears flag 2, and then the level stays) */
            CHECK(P.rings > 0 ? *objects_ring_scatter_level() == lvl1 : *objects_ring_scatter_level() == 0,
                  "scatter level back to 0 once the scattered rings expire uncollected (kept after a pickup: %d rings)",
                  P.rings);
        }
    }
    /* Player_AddRings: one life when crossing each 100 (story mode), 999 cap */
    g_lives_added = 0;
    P.rings = 98;
    dcp_add_rings(&P, 1);
    dcp_add_rings(&P, 1);
    dcp_add_rings(&P, 150);
    n = g_lives_added;
    P.rings = 990;
    dcp_add_rings(&P, 20);
    printf("  rings 98 -> 100 -> 250: lives +%d; 990 + 20 -> %d\n", n, P.rings);
    CHECK(n == 2 && P.rings == 999, "Player_AddRings: a life at 100 and at 200, cap 999");
    /* item box p6 2: Player_AddLives(1) */
    {
        Obj *o = NULL;
        int k2;
        for (k2 = 0; k2 < 30 && o == NULL; k2++) {
            const rlvl_object_t *e = find_obj(0x30, k2);
            Obj *q;
            if (e == NULL)
                break;
            q = spawn_nth(0x30, k2, NULL);
            if (q && (s8)q->prm->p6 == 2)
                o = q;
        }
        if (o != NULL) {
            s32 cx, cy;
            g_lives_added = 0;
            box_centre(o, &cx, &cy);
            hold_contact(cx, cy, 0, 0, 20, "item_box");
            printf("  1-up item box at (%d,%d): lives +%d\n", o->h.x >> 8, o->h.y >> 8, g_lives_added);
            CHECK(g_lives_added == 1, "ItemBox_GiveItem kind 2: Player_AddLives(1)");
        } else {
            printf("  (no 1-up item box in this act)\n");
        }
    }
}

/* Player_In pit line (map height - 0x1a), Player_Action_Die, Player_State_Die (0x3c frames + fade),
 * Player_Respawn at the marker, lives - 1, game over (pf 0x800000) at 0 lives; Player_CheckCrushed */
static void test_death(void) {
    const rlvl_object_t *mk = find_obj(0x6e, 0);
    int n, died_at = -1, back_at = -1;
    u8 lives0;
    printf("[death] pit, Player_State_Die, Player_Respawn, lives, crush\n");
    if (mk == NULL)
        return;
    /* pass the marker for the respawn point */
    {
        Obj *o = spawn_nth(0x6e, 0, NULL);
        s32 cx, cy;
        if (o != NULL) {
            box_centre(o, &cx, &cy);
            hold_contact(cx, cy, 0, 0, 20, "checkpoint");
        }
    }
    lives0 = g_dcp_world.lives = 3;
    g_dcp_world.mode = 0;
    if (g_dcp_world.zone == 6 && g_dcp_world.act == 0) {
        /* Player_In: no pit line in zone 6 act 0 */
        place_player(mk->x, M.bottom - 0x19);
        P.move_flags |= 0x10;
        frame(0);
        printf("  zone 6 act 0: below the line, state die %d (Player_In skips the pit line here)\n", P.state == dcp_state_die);
        CHECK(P.state != dcp_state_die, "zone 6 act 0 has no pit line (Player_In)");
        return;
    }
    /* drop below the map: the pit line is the map height - 0x1a (death_y from dcp_bind_map) */
    place_player(mk->x, M.bottom - 0x19); /* (Player_In checks the line before any collision) */
    P.move_flags |= 0x10;
    for (n = 0; n < 0x100 && back_at < 0; n++) {
        frame(0);
        if (died_at < 0 && P.state == dcp_state_die)
            died_at = n;
        if (died_at >= 0 && P.state != dcp_state_die)
            back_at = n;
    }
    if (died_at < 0)
        printf("  (not dead: mf %x pf %x zone %d act %d state %p)\n", P.move_flags, P.pf, g_dcp_world.zone, g_dcp_world.act,
               (void *)P.state);
    printf("  pit: dead at frame %d (y %d, line %d), back after %d frames at (%d,%d), respawn (%d,%d), lives %d -> %d\n",
           died_at, P.y >> 8, g_dcp_world.death_y - 0x1a, back_at - died_at, P.x >> 8, P.y >> 8, g_dcp_world.check_x,
           g_dcp_world.check_y, lives0, g_dcp_world.lives);
    CHECK(g_dcp_world.death_y == M.bottom, "dcp_bind_map: pit line from the map height (%d)", g_dcp_world.death_y);
    CHECK(died_at >= 0 && back_at - died_at >= 0x3b && g_dcp_world.lives == lives0 - 1,
          "Player_State_Die: 0x3c frames, then Player_Respawn, lives - 1");
    CHECK(DCP_ABS((P.x >> 8) - g_dcp_world.check_x) <= 2 && P.rings == 0, "respawn at the marker with no rings");
    /* game over: no lives left -> pf 0x800000, no respawn */
    g_dcp_world.lives = 0;
    place_player(mk->x, M.bottom - 0x19);
    P.move_flags |= 0x10;
    for (n = 0; n < 0x100 && !(P.pf & 0x800000); n++)
        frame(0);
    CHECK((P.pf & 0x800000) != 0, "Player_Respawn at 0 lives: game over flag pf 0x800000 for the UI");
    /* crush: Player_CheckCrushed with pf 0x800 (riding), no touch object, walls on both sides */
    g_dcp_world.lives = 3;
    place_on_floor(mk->x, mk->y);
    frame(0);
    P.pf |= 0x800;
    P.move_flags |= 4 | 8;
    dcp_check_crushed(&P);
    CHECK(P.state == dcp_state_die, "Player_CheckCrushed: pf 0x800 with both side walls kills");
    g_dcp_world.lives = 3;
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
    g_chara = getenv("CHARA") ? (u8)atoi(getenv("CHARA")) : 0;
    dcp_player_create(&P, g_chara, 0, sx, sy);
    g_prev_sfx = g_dcp_hooks.sfx, g_dcp_hooks.sfx = hk_sfx;
    g_prev_stop = g_dcp_hooks.sfx_stop, g_dcp_hooks.sfx_stop = hk_sfx_stop;
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
    if (getenv("AUDIT_ONLY")) {
        /* make audit: the per-class audit on any stage, either character */
        test_audit();
        test_audit_classes();
        test_tricks();
        test_homing();
        test_rings_lives();
        test_death();
        printf("%d checks, %d failed\n", g_checks, g_fail);
        objects_shutdown();
        return g_fail ? 1 : 0;
    }
    test_unit();
    test_run(getenv("RUNFRAMES") ? atoi(getenv("RUNFRAMES")) : 1500);
    test_targets();
    test_v2();
    test_sweep();
    test_respawn();
    test_player_fx();
    test_blaze_flames();
    test_audit();
    test_audit_classes();
    test_tricks();
    test_homing();
    test_rings_lives();
    test_death();
    test_loop();
    printf("%d checks, %d failed\n", g_checks, g_fail);
    objects_shutdown();
    return g_fail ? 1 : 0;
}
