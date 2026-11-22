/*
 * Host test of the native object module on the real Leaf Storm act 1 level.
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
 *  [respawn] collected rings stay collected, despawned objects come back.
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

static void hk_log(void *ctx, const char *ev, s32 a, s32 b) {
    (void)ctx;
    if (!g_verbose)
        return;
    if (!strcmp(ev, "ring") && g_verbose < 2)
        return;
    printf("    f%5d %-16s %6d %6d   player (%d,%d) rings %d\n", g_frame, ev, a, b, P.x >> 8, P.y >> 8, P.rings);
}
static int g_goal_hook, g_exit_hook;
static void hk_goal(void *ctx, DcPlayer *p) { (void)ctx; (void)p; g_goal_hook++; }
static void hk_exit(void *ctx, s32 k) { (void)ctx; (void)k; g_exit_hook++; }

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
    /* Egg Pawn 0x00: jump onto it -> destroyed, player bounces */
    o = find_obj(0x00, 1);
    CHECK(o != NULL, "egg pawn in z11");
    if (o) {
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
            if (getenv("OBJDBG")) {
                ObjPlayerCtx *pc = &g_obj.players[0];
                Obj *q;
                printf("    k%3d P(%d,%d) anim %d spr %d f%d atk[%x/%x fl %x box %d,%d,%d,%d] body[%x/%x fl %x %d,%d,%d,%d]\n", k,
                       P.x >> 8, P.y >> 8, P.anim, pc->spr.anim, pc->spr.frame_no, pc->atk.atk, pc->atk.def, pc->atk.flags,
                       pc->atk.l, pc->atk.t, pc->atk.r, pc->atk.b, pc->body.atk, pc->body.def, pc->body.flags, pc->body.l,
                       pc->body.t, pc->body.r, pc->body.b);
                for (q = g_obj.head; q; q = q->next)
                    if (q->kind == OBJK_PAWN)
                        printf("        pawn (%d,%d) anim %d c0[fl %x %d,%d,%d,%d] c1[fl %x %d,%d,%d,%d]\n", q->h.x >> 8, q->h.y >> 8,
                               q->spr.anim, q->coll[0].flags, q->coll[0].l, q->coll[0].t, q->coll[0].r, q->coll[0].b,
                               q->coll[1].flags, q->coll[1].l, q->coll[1].t, q->coll[1].r, q->coll[1].b);
            }
        }
        printf("  egg pawn at (%d,%d): after %d frames destroyed %u hurt %u, player vel %d,%d\n", o->x, o->y, k,
               st->enemies_destroyed - e0, st->hurts - h0, P.vel_x, P.vel_y);
        CHECK(st->enemies_destroyed > e0 && st->hurts == h0, "jumping player destroys the enemy");
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
    g_dcp_world.zone = 0;
    g_dcp_world.act = 0;
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
        int s, nres = 0;
        printf("objects_init: %d; stage %d; sprites:", r, ((const rlvl_header_t *)g_level)->stage);
        for (s = 0; s < OBJ_BAC_COUNT; s++) {
            printf(" %s%s", g_obj.data.ids[s], g_obj.res[s].loaded ? "" : "(MISSING)");
            nres += g_obj.res[s].loaded;
        }
        printf("\n");
        CHECK(r == 0, "objects_init");
        CHECK(nres == OBJ_BAC_COUNT, "all %d sprite resources loaded", OBJ_BAC_COUNT);
        if (r != 0)
            return 1;
    }
    camera_follow(1);
    test_unit();
    test_run(getenv("RUNFRAMES") ? atoi(getenv("RUNFRAMES")) : 1500);
    test_targets();
    test_respawn();
    printf("%d checks, %d failed\n", g_checks, g_fail);
    objects_shutdown();
    return g_fail ? 1 : 0;
}
