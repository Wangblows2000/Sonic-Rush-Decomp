/*
 * Host test of the objdata v3 kinds on the real Water Palace (z21, z22) and
 * Mirage Road (z31, z32) levels.
 *
 * Usage: test_zones <level.bin> <build/dreamcast/player> <objdata dir> <sprites bac dir> [-v]
 *
 *  [init]    objects_init, every preloaded sprite resource loads.
 *  [run]     scripted run from PlayerStart (hold right, jump every 50 frames).
 *  [zone 2]  per-gimmick checks: water entry / exit, drowning countdown and
 *            drowning, air bubble breathing, bubble spawner, water run,
 *            water current (MapObj7D), water slide (MapObj80), launch zone,
 *            Wheel2D, mine (grab, countdown, explosion, release), Kuragen
 *            (bomb drop and explosion, pulse), water jet, tilting raft
 *            (LandUnrest), Egg Lancer charge, sand/water slope (MapObj40
 *            0x7e), decorations (fish schools, shared waterfall animation).
 *  [zone 3]  rail, cargo, hand, sand ship (GmkTable) with drop-in Egg Pawns,
 *            enemy room (start, gates, entrance spawns, clear), sand
 *            sliders 0x90 / 0x91 and the sand slide, falling rock, Egg
 *            Bishop throw, Egg Hammer stomp, MapObj40 0xa9 sand, decorations.
 *  [sweep]   runs from every 25th placed ring over the whole act.
 * Each check reports whether the interaction was reached by play from a
 * nearby spot or the player was put into the trigger box directly.
 */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../objects.h"
#include "../obj_internal.h"

static int g_fail, g_checks, g_ok, g_n;
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
static const rlvl_header_t *g_hdr;
static int g_verbose = 1, g_frame;
static s32 g_cam_x, g_cam_y;

#define NEV 96
static struct { char name[24]; int n; } g_ev[NEV];
static int ev_count(const char *ev) {
    int i;
    for (i = 0; i < NEV; i++)
        if (!strcmp(g_ev[i].name, ev))
            return g_ev[i].n;
    return 0;
}
static int g_quiet;
static s32 g_ring_xy[64][2]; /* collected ring positions (event state test) */
static int g_n_ring_xy, g_rec_rings;
static void hk_log(void *ctx, const char *ev, s32 a, s32 b) {
    int i;
    (void)ctx;
    if (g_rec_rings && !strcmp(ev, "ring") && g_n_ring_xy < 64) {
        g_ring_xy[g_n_ring_xy][0] = a;
        g_ring_xy[g_n_ring_xy][1] = b;
        g_n_ring_xy++;
    }
    for (i = 0; i < NEV; i++) {
        if (!g_ev[i].name[0])
            strncpy(g_ev[i].name, ev, sizeof(g_ev[i].name) - 1);
        if (!strcmp(g_ev[i].name, ev)) {
            g_ev[i].n++;
            break;
        }
    }
    if (g_verbose >= 2 && !g_quiet && strcmp(ev, "ring") && strcmp(ev, "camera_bounds"))
        printf("    f%5d %-20s %6d %6d   player (%d,%d)\n", g_frame, ev, a, b, P.x >> 8, P.y >> 8);
}
static int g_fx[32];
static void hk_effect(void *ctx, DcPlayer *p, s32 kind, s32 a, s32 b) {
    (void)ctx;
    (void)p;
    (void)a;
    (void)b;
    if (kind >= 0 && kind < 32)
        g_fx[kind]++;
}

static void camera_follow(int snap) {
    s32 tx = (P.x >> 8) - 128, ty = (P.y >> 8) - 105;
    /* Player_UpdateCamera (camera module, not in the object module): while a gimmick
     * locks the camera (pf2 & 0x10 / 0x20) it follows the gimmick object, offset by
     * @0x3EC + 0x80 / @0x3EE + 0x60 */
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
    /* the DS spawns for both screens: the second view stands for the top screen,
     * directly above the bottom one (an approximation of the dual-screen camera) */
    objects_set_view(1, g_cam_x << 8, (g_cam_y - 192) << 8);
}
static u16 s_prev;
static u32 g_peak; /* pool peak (live objects + decorations) */
static u32 g_pf2_pre, g_of_pre; /* player flags between the box checks and the player task */
static void frame(u16 held) {
    u16 pressed = held & ~s_prev, released = s_prev & ~held;
    s_prev = held;
    objects_frame_begin();
    g_pf2_pre = P.pf2;
    g_of_pre = P.obj_flags;
    dcp_player_frame(&P, held, pressed, released);
    objects_update(&P);
    camera_follow(0);
    if (objects_stats()->live > g_peak)
        g_peak = objects_stats()->live;
    g_frame++;
}
static void frames(int n, u16 held) {
    while (n-- > 0)
        frame(held);
}
static void place_player(s32 x, s32 y) {
    s16 rings = P.rings;
    memset(&P, 0, sizeof(P));
    dcp_player_create(&P, 0, 0, x, y);
    objects_player_init_state(&P); /* (Player_InitState box part: the player-module hook, OBJECTS.md 2.2) */
    P.rings = rings;
    P.invuln = 0x7fff; /* the checks are about gimmicks: enemies must not interrupt them */
    camera_follow(1);
    objects_respawn_view();
}
static void teleport(s32 x, s32 y) { /* (px/py too: StageObj_Collision sweeps from the previous position) */
    P.x = P.px = x << 8;
    P.y = P.py = y << 8;
}
static int floor_y(s32 x, s32 y) { /* first floor (layer B) at or below y-64; -1 = none */
    s32 yy;
    for (yy = y - 64; yy < y + 96; yy++)
        if (rlvl_tile_height(&M, yy, x, 1 | 0x80, NULL, NULL) != 0)
            return yy;
    return -1;
}
static int place_on_floor(s32 x, s32 y) {
    s32 yy = floor_y(x, y);
    place_player(x, (yy < 0 ? y : yy) - 14);
    return yy >= 0;
}
static const rlvl_object_t *find_obj(u16 type, int nth) {
    const rlvl_object_t *o = (const rlvl_object_t *)rlvl_section(g_level, RLVL_SEC_OBJECTS, NULL);
    u32 i;
    for (i = 0; i < g_hdr->num_objects; i++)
        if (o[i].type == type && nth-- == 0)
            return &o[i];
    return NULL;
}
static int n_obj(u16 type) {
    int n = 0;
    while (find_obj(type, n))
        n++;
    return n;
}
static Obj *obj_at(u16 type, s32 x, s32 y) { /* the live object of a placed entry */
    Obj *o;
    for (o = g_obj.head; o; o = o->next)
        if (o->used && o->prm && o->prm->type == type && o->home_x == x && o->home_y == y && !(o->h.obj_flags & 4))
            return o;
    return NULL;
}
static Obj *obj_type_near(u16 type, s32 x, s32 y, s32 r) {
    Obj *o;
    for (o = g_obj.head; o; o = o->next)
        if (o->used && o->prm && o->prm->type == type && !(o->h.obj_flags & 4) && DCP_ABS((o->h.x >> 8) - x) < r &&
            DCP_ABS((o->h.y >> 8) - y) < r)
            return o;
    return NULL;
}
static int count_res_anim(int bac, int lo, int hi) {
    Obj *o;
    int n = 0;
    for (o = g_obj.head; o; o = o->next)
        if (o->used && o->has_spr && o->spr.res == &g_obj.res[bac] && o->spr.anim >= lo && o->spr.anim <= hi &&
            !(o->h.obj_flags & 4))
            n++;
    return n;
}
static void result(const char *what, int ok, int direct, const char *fmt, ...) {
    va_list ap;
    g_n++;
    g_ok += ok != 0;
    printf("  %-24s %-4s %-7s ", what, ok ? "ok" : "MISS", direct ? "direct" : "play");
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    printf("\n");
    CHECK(ok, "%s", what);
}
#define FIND_PRESENT(type) (n_obj(type) > 0)

/* ============================================================ zone 2 */
static u16 water(void) { return g_obj_camera[0].water; }
static int underwater_spot(s32 *ox, s32 *oy) { /* a floor 24+ px below the water surface */
    const rlvl_ring_t *rg = (const rlvl_ring_t *)rlvl_section(g_level, RLVL_SEC_RINGS, NULL);
    u32 i;
    s32 fy;
    for (i = 0; i < g_hdr->num_rings; i++) {
        if (rg[i].y < water() + 40 || rg[i].y > water() + 200)
            continue;
        fy = floor_y(rg[i].x, rg[i].y);
        if (fy < 0 || fy - 14 < water() + 24)
            continue;
        if (obj_type_near(0x84, rg[i].x, rg[i].y, 0x180) != NULL)
            continue;
        *ox = rg[i].x;
        *oy = fy;
        return 1;
    }
    return 0;
}
static void test_water(void) {
    const ObjStats *st = objects_stats();
    s32 x, y;
    int k, seen[10] = {0}, i, ok;
    u32 e0, s0, d0;
    u16 lv;
    frames(5, 0);
    lv = water();
    printf("  water level %d (camera flags %#x)\n", lv, g_obj_camera[0].flags & 0x3000000);
    CHECK(lv != 0 && lv != 0xffff, "WaterLevelTrigger set the water level");
    if (!underwater_spot(&x, &y)) {
        result("underwater", 0, 0, "no floor below the water found");
        return;
    }
    /* enter the water from above */
    e0 = st->water_entries;
    s0 = st->water_splashes;
    place_player(x, lv - 40);
    for (k = 0; k < 120 && !(P.pf & DCP_PF_UNDERWATER); k++)
        frame(0);
    frames(2, 0);
    result("water entry", (P.pf & DCP_PF_UNDERWATER) && st->water_entries > e0 && st->water_splashes > s0, 0,
           "(%d,%d) under after %d frames, splash %u, air bubbles alive %d, bgm layer calls %d", x, lv, k,
           st->water_splashes - s0, count_res_anim(OBJ_BAC_BUBBLE, 2, 3), g_fx[OBJ_FX_BGM_LAYER]);
    /* the drowning countdown: digits 5..0 (ac_eff_water anims 9..4), then Player_Action_Die */
    d0 = st->drownings;
    P.unk3CA = 0;
    for (k = 0; k < P.p374 + 120 && st->drownings == d0; k++) {
        frame(0);
        for (i = 4; i <= 9; i++)
            if (count_res_anim(OBJ_BAC_EFF_WATER, i, i))
                seen[i] = 1;
    }
    ok = st->drownings > d0 && seen[4] && seen[5] && seen[6] && seen[7] && seen[8] && seen[9];
    result("drowning countdown", ok, 0, "air time %d frames: digits seen %d%d%d%d%d%d, drowned after %d frames, jingle %d",
           P.p374, seen[9], seen[8], seen[7], seen[6], seen[5], seen[4], k, g_fx[OBJ_FX_JINGLE]);
    /* air bubble: a breathable bubble (0xe8) rising into the player resets the counter */
    place_on_floor(x, y);
    frames(10, 0);
    {
        u32 b0 = st->air_bubbles;
        int got = -1;
        Obj *b = obj_mapobject_spawn(0xe8, P.x, P.y + 0x2800, 0, 0, 0, 0, 0);
        P.unk3CA = 300;
        for (k = 0; k < 300 && st->air_bubbles == b0; k++)
            frame(0);
        if (st->air_bubbles > b0)
            got = P.state == Player_State_BreatheAirBubble && P.unk3CA <= 1;
        for (k = 0; k < 60 && P.state == Player_State_BreatheAirBubble; k++) {
            if (k == 20)
                P.disp_flags |= DCP_DF_ANIM_DONE; /* the 3D model's breathing animation ends (renderer side) */
            frame(0);
            if (getenv("OBJDBG") && k % 10 == 0)
                printf("    breathe k%d anim %#x disp %#x timer %d mf %#x spr anim %d frame %d/%d flags %x\n", k, P.anim, P.disp_flags,
                       P.timer, P.move_flags, g_obj.players[0].spr.anim, g_obj.players[0].spr.frame_no, g_obj.players[0].spr.frame_cnt,
                       g_obj.players[0].spr.flags);
        }
        result("air bubble", b != NULL && got == 1 && P.state != Player_State_BreatheAirBubble, 1,
               "bubble spawned under the player: breathed (state breathe %d, count reset); left the state %d frames after "
               "the model animation end was signalled", got, k - 20);
    }
    /* leave the water: splash + gravity restored */
    {
        u32 x0 = st->water_exits;
        place_player(x, lv + 30);
        frames(3, 0);
        P.vel_y = -0x900;
        P.move_flags &= ~1u;
        for (k = 0; k < 60 && st->water_exits == x0; k++)
            frame(0);
        result("water exit", st->water_exits > x0 && !(P.pf & DCP_PF_UNDERWATER), 0, "after %d frames, gravity %d",
               k, P.gravity);
    }
}
static void test_bubble_spawner(void) {
    const ObjStats *st = objects_stats();
    const rlvl_object_t *o;
    int i, k = 0;
    u32 b0 = st->bubbles_spawned;
    for (i = 0; st->bubbles_spawned == b0 && (o = find_obj(0x84, i)) != NULL; i++) {
        place_player(o->x, o->y - 40);
        for (k = 0; k < 400 && st->bubbles_spawned == b0; k++)
            frame(0);
    }
    result("bubble spawner 0x84", st->bubbles_spawned > b0, 0, "%d spawners, breathable bubble after %d frames near #%d",
           n_obj(0x84), k, i - 1);
}
static void test_water_run(void) {
    const rlvl_object_t *o;
    int i, k = 0, run = 0, splash = 0, n = 0, play = 0;
    s32 ymin = 0x7fffffff, ymax = -1, x0 = 0;
    /* by play: the runs out of the water slides (MapObj80) end on a 0x7b trigger */
    for (i = 0; !run && (o = find_obj(0x80, i)) != NULL; i++) {
        if (o->flags & 3)
            continue;
        if (!place_on_floor(o->x - 24, o->y + 40))
            continue;
        frames(5, 0);
        for (k = 0; k < 400 && P.state != Player_State_WaterRun; k++)
            frame(DCP_KEY_RIGHT);
        run = play = P.state == Player_State_WaterRun;
    }
    /* direct: at the trigger, at the surface, running right fast */
    for (i = 0; !run && (o = find_obj(0x7b, i)) != NULL; i++) {
        place_player(o->x + 8, o->y - 4);
        P.disp_flags &= ~1;
        P.gnd_vel = (s16)P.run_thr[3];
        for (k = 0; k < 5 && P.state != Player_State_WaterRun; k++) {
            teleport(o->x + 8, o->y - 4);
            P.gnd_vel = (s16)P.run_thr[3];
            frame(DCP_KEY_RIGHT);
        }
        run = P.state == Player_State_WaterRun;
    }
    if (run) {
        x0 = P.x;
        for (n = 0; n < 40 && P.state == Player_State_WaterRun; n++) {
            frame(DCP_KEY_RIGHT);
            if ((P.y >> 8) < ymin) ymin = P.y >> 8;
            if ((P.y >> 8) > ymax) ymax = P.y >> 8;
            splash += count_res_anim(OBJ_BAC_EFF_WATER, 2, 2) > 0;
        }
    }
    result("water run 0x7b", run && splash > 0 && ymin == water() - 15 && P.x != x0, !play,
           "running on the water after %d frames, y %d..%d (surface %d - 15), moved %d px in %d frames, splash frames %d",
           k, ymin, ymax, water(), (P.x - x0) >> 8, n, splash);
}
static void test_current(void) {
    const rlvl_object_t *o = find_obj(0x7d, 0);
    if (!o)
        return;
    {
        s16 v0, d = 0;
        int k;
        place_player(o->x + o->p0 + o->p2 / 2, o->y + o->p1 + o->p3 / 2);
        frames(2, 0);
        for (k = 0; k < 4; k++) {
            teleport(o->x + o->p0 + o->p2 / 2, o->y + o->p1 + o->p3 / 2);
            P.vel_y = 0;
            v0 = P.vel_y;
            frame(0);
            d = (s16)(P.vel_y - v0);
        }
        result("water current 0x7d", d == 0x54 || d == 0xa8, 1,
               "gravity applied inside the current %#x per frame (normal %#x; Player_Last restores it)", d, P.gravity);
    }
}
static void test_slide80(void) {
    const rlvl_object_t *o;
    int i, k = 0, ok = 0;
    for (i = 0; !ok && (o = find_obj(0x80, i)) != NULL; i++) {
        if (o->flags & 3)
            continue;
        if (!place_on_floor(o->x - 24, o->y + 40))
            continue;
        frames(5, 0);
        for (k = 0; k < 120 && P.state != Player_State_MapObj80; k++)
            frame(DCP_KEY_RIGHT);
        ok = P.state == Player_State_MapObj80;
    }
    if (ok) {
        s32 x0 = P.x;
        frames(30, 0);
        result("water slide 0x80", 1, 0, "#%d: slide state after %d frames, then moved %d px in 30 frames (state %s)", i - 1,
               k, (P.x - x0) >> 8, P.state == Player_State_MapObj80 ? "slide" : "left");
    } else {
        result("water slide 0x80", 0, 0, "never entered");
    }
}
static void test_launch_zone(void) {
    const rlvl_object_t *o = find_obj(0x83, 0);
    int k;
    if (!o)
        return;
    s16 mx = 0, my = 0, vx = 0, vy = 0, ex, ey;
    u16 fl = o->flags;
    place_player(o->x + o->p0 + o->p2 / 2, o->y + o->p1 + o->p3 / 2);
    for (k = 0; k < 10 && P.state != Player_State_ZoneLaunch; k++) {
        mx = P.move_x, my = P.move_y;
        frame(0);
    }
    /* LaunchZone_OnHit: (+-0xa00 unless flags 8, -0xa00 / +0xa00 by flags 2 / 4) * 0xb5 >> 8 unless
     * flags 8; Player_Action_ZoneLaunch: vel = (move + v) >> 1; Player_State_ZoneLaunch in the same
     * frame: ObjSpd_DownSet(vel, 0x60), gravity back on (move flag 0x80) before the move */
    if (!(fl & 8))
        vx = (fl & 1) ? -0xa00 : 0xa00;
    if (fl & 2)
        vy = -0xa00;
    if (fl & 4)
        vy = 0xa00;
    if (!(fl & 8)) {
        vx = (s16)((vx * 0xb5) >> 8);
        vy = (s16)((vy * 0xb5) >> 8);
    }
    ex = (s16)dcp_spd_down((s16)((mx + vx) >> 1), 0x60);
    ey = (s16)dcp_spd_down((s16)((my + vy) >> 1), 0x60); /* + one frame of gravity (water: varies) */
    result("launch zone 0x83", P.state == Player_State_ZoneLaunch && P.vel_x == ex && P.vel_y >= ey && P.vel_y <= ey + 0x2a, 1,
           "flags %#x: launched after %d frames, vel %d,%d (LaunchZone_OnHit formula %d,%d + gravity)", o->flags, k, P.vel_x, P.vel_y, ex, ey);
    frames(90, 0);
}
static void test_wheel(void) {
    const rlvl_object_t *o = find_obj(0x82, 0);
    Obj *w;
    int k, mv;
    s32 w8;
    u8 r0;
    ObjModelView v;
    if (!o)
        return;
    place_player(o->x - 0x6e, o->y - 0x10);
    frames(2, 0);
    w = obj_at(0x82, o->x, o->y);
    for (k = 0; k < 30 && P.state != Player_State_Wheel2D; k++)
        frame(0);
    r0 = P.rot3;
    w8 = w ? w->w8 : 0;
    frames(60, 0);
    mv = objects_model_count() > 0 && objects_model_get(0, &v);
    result("wheel 0x82", w && P.gimmick == w && P.rot3 != r0 && w->w8 != w8, 1,
           "caught after %d frames; player angle %#x -> %#x, wheel angle %#x -> %#x, model view %s", k, r0, P.rot3, w8,
           w ? w->w8 : 0, mv ? v.model : "-");
    for (k = 0; k < 600 && P.state == Player_State_Wheel2D; k++)
        frame(0);
    printf("    released after %d more frames (state wheel %d)\n", k, P.state == Player_State_Wheel2D);
}
static void test_mine(void) {
    const ObjStats *st = objects_stats();
    const rlvl_object_t *o;
    Obj *m = NULL;
    int i, k = 0, grabbed = 0, rel;
    u32 x0 = st->mines_exploded;
    for (i = 0; !grabbed && (o = find_obj(0x7f, i)) != NULL; i++) {
        place_player(o->x, o->y - 40);
        frames(2, 0);
        m = obj_at(0x7f, o->x, o->y);
        teleport(o->x, o->y + 64); /* into the handle box (BAC: -7..7, 60..68 below the mine) */
        for (k = 0; k < 20 && P.state != Player_State_AttachedToObject; k++)
            frame(0);
        grabbed = m != NULL && P.state == Player_State_AttachedToObject && P.gimmick == m;
    }
    if (!grabbed) {
        result("mine 0x7f", 0, 1, "not grabbed");
        return;
    }
    for (k = 0; k < 700 && st->mines_exploded == x0; k++)
        frame(0);
    frames(2, 0);
    rel = P.state != Player_State_AttachedToObject && P.gimmick == NULL;
    result("mine 0x7f", st->mines_exploded > x0 && rel, 1, "#%d (timer %d): exploded after %d frames, player released %d", i - 1,
           o ? o->p0 : -1, k, rel);
}
static void test_kuragen(void) {
    const ObjStats *st = objects_stats();
    const rlvl_object_t *o;
    int i, k = 0;
    u32 d0 = st->kuragen_drops;
    int b0 = ev_count("kuragen_bomb");
    const rlvl_object_t *kd = NULL;
    for (i = 0; st->kuragen_drops == d0 && (o = find_obj(5, i)) != NULL; i++) {
        kd = o;
        place_player(o->x, o->y + 0x90);
        P.move_flags |= 0x2000; /* hover under it (inside the detect box), like a swimming player */
        for (k = 0; k < 200 && st->kuragen_drops == d0; k++) {
            teleport(o->x, o->y + 0x90);
            frame(0);
        }
    }
    for (k = 0; k < 900 && ev_count("kuragen_bomb") == b0; k++) {
        if (kd != NULL)
            teleport(kd->x, kd->y + 0x90);
        frame(0);
    }
    P.move_flags &= ~0x2000u;
    result("kuragen bomb 0x05", st->kuragen_drops > d0 && ev_count("kuragen_bomb") > b0, 1,
           "#%d detected the player, bomb exploded %d frames later", i - 1, k);
    o = find_obj(6, 0);
    if (o) {
        Obj *w;
        int on = 0, off = 0;
        place_on_floor(o->x + 0x60, o->y + 0x40);
        frames(2, 0);
        w = obj_at(6, o->x, o->y);
        for (k = 0; k < 400 && w && w->used; k++) {
            frame(0);
            if (w->coll[0].cb)
                on++;
            else
                off++;
        }
        result("kuragen pulse 0x06", w && on > 0 && off > 0, 1, "hit box on %d / off %d frames of 400", on, off);
    }
}
static void test_water_jet(void) {
    const rlvl_object_t *o;
    int i, k = 0, ok = 0;
    for (i = 0; !ok && (o = find_obj(0x85, i)) != NULL; i++) {
        place_player(o->x + 0x18, o->y - 0x20);
        P.move_flags |= 0x2000; /* hold the player inside the box until the jet spouts */
        for (k = 0; k < 700 && P.vel_y > -0x700; k++) {
            teleport(o->x + 0x18, o->y - 0x20);
            frame(0);
            if (getenv("OBJDBG") && k % 25 == 0) {
                Obj *j = obj_at(o->type, o->x, o->y);
                printf("    jet k%d %p f20C %x wC %d c1 flags %x owner %p body flags %x atk %x pos %d,%d box %d %d\n", k, (void *)j,
                       j ? j->k.bub.f20C : 0, j ? j->wC : 0, j ? j->coll[1].flags : 0, j ? j->coll[1].owner : 0,
                       g_obj.players[0].body.flags, g_obj.players[0].body.atk, P.x >> 8, P.y >> 8, j ? j->coll[1].x : 0, j ? j->coll[1].y : 0);
            }
        }
        P.move_flags &= ~0x2000u;
        {
            /* WaterJet_OnHit: v = -0x780 - p6 * 0x180 (0x85 up), Player_Action_SpringLaunch, 3/4 under
             * water; the player held (move flag 0x2000) so no gravity in that frame */
            Obj *j = obj_at(o->type, o->x, o->y);
            s16 v = (s16)(-0x780 - (j ? j->prm->p6 : 0) * 0x180);
            if (P.pf & 0x4000000)
                v = (s16)((v >> 1) + (v >> 2));
            ok = P.vel_y == v || P.vel_y == (s16)(v + P.gravity);
            if (!ok)
                printf("    water jet: vel_y %d, WaterJet_OnHit formula %d\n", P.vel_y, v);
        }
    }
    result("water jet 0x85", ok, 1, "#%d (period %d): launched after %d frames, vel_y %d", i - 1, o ? o->p2 * 100 + o->p3 : 0,
           k, P.vel_y);
}
static void test_raft(void) {
    const rlvl_object_t *o = find_obj(0x81, 0);
    Obj *w;
    int k, maxang = 0;
    if (!o)
        return;
    place_player(o->x + 20, o->y - 40);
    frames(3, 0);
    w = obj_at(0x81, o->x, o->y);
    for (k = 0; k < 90 && w; k++) {
        frame(0);
        if (w->h.angle && (w->h.angle < 0x80 ? w->h.angle : 0x100 - w->h.angle) > maxang)
            maxang = w->h.angle < 0x80 ? w->h.angle : 0x100 - w->h.angle;
    }
    result("tilting raft 0x81", w && maxang > 0, 0, "player stood off-centre: max tilt %#x, riding %d", maxang,
           w && P.ride == &w->ref);
    if (w) {
        /* LandUnrest_State_Tilt, ridden: rot3 = (rider x - x) >> 8, angle = ObjAngle_MoveToward8(angle, rot3, 2),
         * w8 = angle > 0x80 ? (0x100 - angle) >> 1 : 0x100 - (angle >> 1) (0 outside 4..0xfc); off it the
         * angle goes back 2 per frame toward w8 / 0 */
        int bad = 0, rode = 0, steps = 0;
        place_player((w->h.x >> 8) + 20, (w->h.y >> 8) - 40);
        frames(3, 0);
        for (k = 0; k < 40; k++) {
            u8 a0 = w->h.angle, a;
            s32 w8;
            frame(0);
            if (P.ride != &w->ref)
                continue;
            rode++;
            a = w->h.angle;
            w8 = a == 0 ? 0 : a > 0x80 ? (0x100 - a) >> 1 : 0x100 - (a >> 1);
            if (w8 < 4 || w8 > 0xfc)
                w8 = 0;
            /* (the ride carry and the slope slide move the player after the tilt: rot3 is from its position
             * before that, within a few px) */
            if ((u8)(w->h.rot3 - (u8)((P.x - w->h.x) >> 8) + 4) > 8 || a != dcp_angle_toward8(a0, w->h.rot3, 2) || w->w8 != w8) {
                bad++;
                if (getenv("OBJDBG") || bad < 4)
                    printf("    raft k%d: angle %#x -> %#x rot3 %#x (P-x %#x) w8 %#x (want %#x)\n", k, a0, a, w->h.rot3,
                           (u8)((P.x - w->h.x) >> 8), (unsigned)w->w8, (unsigned)w8);
            }
            steps += a != a0;
        }
        CHECK(rode > 10 && bad == 0 && steps > 0, "raft 0x81 exact: rot3 / 2-per-frame tilt / w8 over %d ridden frames (%d off, %d steps)",
              rode, bad, steps);
    }
}
static void test_lancer(void) {
    const rlvl_object_t *o;
    int i, k = 0, ok = 0;
    Obj *w = NULL;
    for (i = 0; !ok && (o = find_obj(3, i)) != NULL; i++) {
        place_on_floor(o->x - 0x50, o->y);
        frames(2, 0);
        w = obj_at(3, o->x, o->y);
        for (k = 0; k < 400 && w && w->used && !ok; k++) {
            frame(0);
            ok = w->spr.anim == 3 && DCP_ABS(w->h.gnd_vel) > 0x100;
        }
    }
    result("egg lancer 0x03", ok, 0, "#%d noticed the player and charged after %d frames (speed %d)", i - 1, k,
           w ? w->h.gnd_vel : 0);
}
static void test_slope7e(void) {
    const rlvl_object_t *o;
    int i, k = 0, ok = 0;
    for (i = 0; !ok && (o = find_obj(0x7e, i)) != NULL; i++) {
        s32 cx = o->x + o->p0 + o->p2 / 2;
        if (!place_on_floor(cx, o->y + o->p1 + o->p3 / 2))
            continue;
        for (k = 0; k < 20 && P.state != Player_State_MapObj40; k++)
            frame(0);
        ok = P.state == Player_State_MapObj40;
    }
    result("water slope 0x7e", ok, 1, "#%d: slide state after %d frames", i - 1, k);
    frames(60, 0);
}
static void test_decor2(void) {
    const ObjStats *st = objects_stats();
    Obj *o;
    int fish = 0, parts = 0, shared = 0, k, i;
    u32 f0 = st->fish_flee;
    const rlvl_decor_t *d = g_obj.lv_dec;
    for (i = 0; d && i < (int)g_hdr->num_decor && st->fish_flee == f0; i++) {
        if (d[i].type != 0xa6 && d[i].type != 0xa9 && d[i].type != 0xae)
            continue;
        place_player(d[i].x - 60, d[i].y);
        frames(3, 0);
        P.vel_x = 0x800;
        P.move_flags |= 0x10;
        for (k = 0; k < 30 && st->fish_flee == f0; k++)
            frame(DCP_KEY_RIGHT);
    }
    for (o = g_obj.head; o; o = o->next) {
        if (!o->used)
            continue;
        if (o->has_spr && o->spr.res == &g_obj.res[OBJ_BAC_DEC_FISH])
            o->parent ? parts++ : fish++;
        if (o->spr.vram != NULL)
            shared++;
    }
    result("fish / decorations", st->dec_spawned > 0 && (st->fish_flee > f0 || !d), 0,
           "decorations spawned %u; fish fled %u; live fish %d, school fish %d, shared-frame decorations %d",
           st->dec_spawned, st->fish_flee - f0, fish, parts, shared);
}

/* ============================================================ zone 3 */
static void test_rail(void) {
    const ObjStats *st = objects_stats();
    const rlvl_object_t *o;
    int i, k = 0, ok = 0;
    s32 x0 = 0;
    Obj *r = NULL;
    u32 r0 = st->rails;
    for (i = 0; !ok && (o = find_obj(0x88, i)) != NULL; i++) {
        place_player(o->x, o->y + 36); /* the handle box (BAC slot 0: -6..5, 32..40 below the rail) */
        frames(1, 0);
        teleport(o->x, o->y + 36);
        r = obj_at(0x88, o->x, o->y);
        P.move_x = (s16)(P.run_thr[0] + 0x100);
        for (k = 0; k < 10 && st->rails == r0; k++) {
            P.move_x = (s16)(P.run_thr[0] + 0x100);
            teleport(o->x, o->y + 36);
            frame(0);
            if (getenv("OBJDBG"))
                printf("    railtry #%d k%d r %p c0 flags %x owner %p box %d,%d P(%d,%d) body %x,%d,%d\n", i, k, (void *)r,
                       r ? r->coll[0].flags : 0, r ? r->coll[0].owner : 0, r ? r->coll[0].x : 0, r ? r->coll[0].y : 0,
                       P.x >> 8, P.y >> 8, g_obj.players[0].body.flags, g_obj.players[0].body.x, g_obj.players[0].body.y);
        }
        ok = st->rails > r0 && P.gimmick == r;
    }
    if (ok) {
        x0 = r->h.x;
        for (k = 0; k < 40; k++) {
            frame(0);
            if (getenv("OBJDBG") && k % 8 == 0)
                printf("    rail k%d (%d,%d) gnd %d vel %d,%d w8 %d wC %x f1F4 %x of %x mf %x | P (%d,%d) state att %d gim %d\n", k,
                       r->h.x >> 8, r->h.y >> 8, r->h.gnd_vel, r->h.vel_x, r->h.vel_y, r->w8 >> 8, r->wC, r->f1F4,
                       r->h.obj_flags, r->h.move_flags, P.x >> 8, P.y >> 8, P.state == Player_State_AttachedToObject,
                       P.gimmick == r);
        }
    }
    result("rail 0x88", ok && r->h.x != x0, 1, "#%d: attached %d, rail moved %d px in 40 frames, player follows %d", i - 1, ok,
           ok ? (r->h.x - x0) >> 8 : 0, ok && DCP_ABS((P.x - r->h.x) >> 8) < 4);
    frames(200, 0);
}
static void test_cargo(void) {
    const ObjStats *st = objects_stats();
    const rlvl_object_t *o;
    int i, k = 0, ok = 0;
    Obj *c = NULL;
    u32 c0 = st->cargo_rides;
    for (i = 0; !ok && (o = find_obj(0x89, i)) != NULL; i++) {
        s32 cx = (o->flags & 2) ? o->x + 28 : o->x - 28; /* BAC slot 0: 16..40, -11..9 (mirrored unless flags & 2) */
        place_player(cx, o->y);
        frames(1, 0);
        c = obj_at(0x89, o->x, o->y);
        for (k = 0; k < 10 && st->cargo_rides == c0; k++) {
            P.move_x = (o->flags & 2) ? (s16)-(P.run_thr[1] + 0x100) : (s16)(P.run_thr[1] + 0x100);
            teleport(cx, o->y);
            frame(0);
        }
        ok = st->cargo_rides > c0 && P.state == Player_State_Cargo && P.gimmick == c;
    }
    if (ok) {
        for (k = 0; k < 400 && P.state == Player_State_Cargo; k++) {
            frame(0);
            if (getenv("OBJDBG") && k % 2 == 0)
                printf("    cargo k%d (%d,%d) gnd %d wC %d f1F4 %x mf %x state_move %d | P (%d,%d)\n", k, c->h.x >> 8, c->h.y >> 8,
                       c->h.gnd_vel, c->wC, c->f1F4, c->h.move_flags, c->h.move_flags & 0x2000, P.x >> 8, P.y >> 8);
        }
    }
    result("cargo 0x89", ok && P.state != Player_State_Cargo && k > 10, 1,
           "#%d: riding %d, cargo swung and released the player after %d frames (gnd %d)", i - 1, ok, k, P.gnd_vel);
}
static void test_hand(void) {
    const rlvl_object_t *o = find_obj(0x8a, 0);
    Obj *h;
    int k;
    u8 r0;
    if (!o)
        return;
    place_player(o->x, o->y - 30);
    frames(2, 0);
    h = obj_at(0x8a, o->x, o->y);
    for (k = 0; k < 60 && P.state != Player_State_Hand2D; k++)
        frame(0);
    r0 = P.rot3;
    frames(20, 0);
    result("hand 0x8a", h && P.gimmick == h && P.state == Player_State_Hand2D && P.rot3 != r0, 0,
           "stood on the hand: carried after %d frames, player angle %#x -> %#x", k, r0, P.rot3);
    {
        /* Player_State_Hand2D: g3F4 -= (hand z >> 4) + 0x470, rot3 = (g3F4 & 0xffff) >> 8; let go:
         * ground speed = the g3F0 saved on the grab */
        ObjPlayerCtx *c = obj_player_ctx(&P);
        int bad = 0, n = 0;
        s32 g0 = c->g3F0;
        for (k = 0; k < 400 && P.state == Player_State_Hand2D; k++) {
            s32 a0 = c->g3F4, z0 = h ? h->h.z : 0, z1;
            frame(0);
            z1 = h ? h->h.z : 0;
            if (P.state == Player_State_Hand2D && P.gimmick == h) {
                n++;
                if ((c->g3F4 != a0 - ((z0 >> 4) + 0x470) && c->g3F4 != a0 - ((z1 >> 4) + 0x470)) ||
                    P.rot3 != (u8)((c->g3F4 & 0xffff) >> 8))
                    bad++;
            }
        }
        printf("    released after %d more frames\n", k);
        CHECK(n > 0 && bad == 0 && (P.state == Player_State_Hand2D || P.gnd_vel == (s16)g0 || !(P.move_flags & 1)),
              "hand 0x8a exact: turn -((z >> 4) + 0x470) per frame over %d frames (%d off), released at gnd %d (saved %d)", n, bad,
              P.gnd_vel, (int)g0);
    }
}
static void test_table(void) {
    const ObjStats *st = objects_stats();
    const rlvl_object_t *o = find_obj(0x8b, 0);
    Obj *t, *q;
    int k, landed = 0, pawns = 0, boarded = 0, bob_n = 0, bob_bad = 0, rel0 = ev_count("table_release"), sail_n = 0,
        sail_bad = 0;
    s32 x0, xmax = 0;
    u32 r0 = st->rocks_fallen, h0 = st->hurts;
    if (!o)
        return;
    place_player(o->x, o->y - 60);
    frames(3, 0);
    t = obj_at(0x8b, o->x, o->y);
    for (k = 0; k < 120 && !(t && P.gimmick == t); k++)
        frame(0);
    boarded = t && P.gimmick == t;
    x0 = t ? t->h.x : 0;
    P.invuln = 0;
    for (k = 0; k < 5000 && t && t->used && !(t->h.obj_flags & 4); k++) {
        /* GmkTable_UpdateBob (released ship): v42A += (u428 > 0 ? -1 : 1) (max 0xc0), u428 += v42A;
         * u424 += (w8 > 0x1a00 ? -0x100 : 0x100) (max 0x30), w8 += u424 */
        int rel = ev_count("table_release") > rel0;
        s16 ba = t->k.tbl.u428, bv = t->k.tbl.u42A, bb = t->k.tbl.u424;
        s32 bw = t->w8;
        bv = (s16)dcp_spd_up(bv, ba > 0 ? -1 : 1, 0xc0);
        ba = (s16)(ba + bv);
        bb = (s16)dcp_spd_up(bb, (u32)bw > 0x1a00u ? -256 : 0x100, 0x30);
        bw += bb;
        u16 w41C = t->k.tbl.u41C;
        s32 w420 = t->k.tbl.u420;
        s16 sg = t->h.gnd_vel < 0x100 ? 0x100 : t->h.gnd_vel; /* (clamped at the top of the state, changed after the step) */
        frame(k % 90 < 6 ? DCP_KEY_A : 0); /* hop over the falling rocks now and then */
        /* GmkTable_State_Main: ground speed >= 0x100, wheel u41C -= speed, distance u420 += speed */
        if (!rel && t->used && !(t->h.obj_flags & 4) && ev_count("table_release") == rel0 && !(t->h.move_flags & 0x2000)) {
            sail_n++;
            if (t->k.tbl.u41C != (u16)(w41C - sg) || t->k.tbl.u420 != w420 + sg) {
                if (sail_bad < 4)
                    printf("    ship k%d: gnd %d wheel %#x -> %#x (d %d) dist d %d\n", k, t->h.gnd_vel, w41C, t->k.tbl.u41C,
                           (s16)(w41C - t->k.tbl.u41C), (int)(t->k.tbl.u420 - w420));
                sail_bad++;
            }
        }
        if (rel && t->used && !(t->h.obj_flags & 4)) {
            bob_n++;
            if (t->k.tbl.u428 != ba || t->k.tbl.u42A != bv || t->k.tbl.u424 != bb || t->w8 != bw)
                bob_bad++;
        }
        if (P.invuln == 0)
            P.invuln = 0x7fff; /* (stay on board: the check is about the ship sequence) */
        xmax = t->h.x;
        if (getenv("OBJDBG") && k % 200 == 0) {
            u32 e;
            printf("    ship k%d (%d,%d) P(%d,%d) cam (%d,%d) pf2 %x gim %d:", k, t->h.x >> 8, t->h.y >> 8, P.x >> 8, P.y >> 8, g_cam_x, g_cam_y, P.pf2, P.gimmick == t);
            for (e = 0; e < g_hdr->num_objects; e++)
                if ((g_obj.prm[e].type == 0x14 || g_obj.prm[e].type == 0x15) && g_obj.prm[e].p9)
                    printf(" %x@%d,%d:%s", g_obj.prm[e].type, g_obj.prm[e].ax, g_obj.prm[e].ay, g_obj.prm[e].x0 == 0xff ? "S" : "-");
            printf("\n");
        }
        for (q = g_obj.head; q; q = q->next)
            if (q->used && q->kind == OBJK_PAWN && q->prm && q->prm->p9 != 0) {
                pawns |= 1;
                if (q->u23A != 0)
                    landed = 1;
            }
    }
    CHECK(sail_n > 100 && sail_bad == 0, "sand ship 0x8b exact: wheel / distance step by max(speed, 0x100) over %d frames (%d off)", sail_n,
          sail_bad);
    if (bob_n)
        CHECK(bob_bad == 0, "sand ship 0x8b exact: released bob (GmkTable_UpdateBob) over %d frames (%d off)", bob_n, bob_bad);
    else
        printf("    (sand ship not released in the run: bob not checked)\n");
    result("sand ship 0x8b", boarded && xmax > x0 + 0x10000, 0,
           "boarded %d, sailed %d px in %d frames; drop-in pawns seen %d, landed on the ship %d; rocks fallen on it %u; "
           "released %d, hurts %u", boarded, (xmax - x0) >> 8, k, pawns, landed, st->rocks_fallen - r0,
           ev_count("table_release"), st->hurts - h0);
    if (pawns)
        CHECK(landed, "a drop-in Egg Pawn lands on the sand ship");
    else
        printf("    (no drop-in pawn came into the spawn views: they spawn only when a view is within 60 px of them, "
               "which depends on the camera; see OBJECTS.md)\n");
    CHECK(st->rocks_fallen > r0, "a falling rock lands on the sand ship");
}
static void test_room(void) {
    const ObjStats *st = objects_stats();
    const rlvl_object_t *o = find_obj(0x8c, 0), *b, *b0 = NULL, *b1 = NULL;
    Obj *r, *q;
    int k, i, killed = 0, walls_closed = 0, spawned = 0, room;
    u32 c0 = st->enemy_rooms;
    if (!o)
        return;
    room = (o->flags & 0xe0) >> 5;
    /* the room bounds: the MapObj8F pair of the same room index (flags & 1 clear: room bound,
     * flags & 2 clear: top-left, set: bottom-right) */
    for (i = 0; (b = find_obj(0x8f, i)) != NULL; i++) {
        if (((b->flags & 0xe0) >> 5) != room || (b->flags & 1))
            continue;
        if (b->flags & 2)
            b1 = b;
        else
            b0 = b;
    }
    if (b0 && b1)
        place_on_floor((b0->x + b1->x) / 2, (b0->y + b1->y) / 2);
    else
        place_on_floor(o->x, o->y);
    frames(10, 0);
    r = obj_type_near(0x8c, o->x, o->y, 0x400);
    printf("    room %d controller %s: bounds %d,%d..%d,%d\n", room, r ? "live" : "-", r ? r->k.room.b216 : -1,
           r ? r->k.room.b218 : -1, r ? r->k.room.b21A : -1, r ? r->k.room.b21C : -1);
    for (k = 0; k < 900 && st->enemy_rooms == c0; k++) {
        frame(0);
        for (q = g_obj.head; q; q = q->next) {
            if (!q->used)
                continue;
            if (q->prm && q->prm->type == 0x8e && q->h.y != q->w8)
                walls_closed = 1;
            /* the enemies an enemy room spawns are marked 0x10 (no view-out): defeat them */
            if (q->kind == OBJK_PAWN && (q->h.obj_flags & 0x10) && !(q->h.obj_flags & 4) && !obj_pawn_in_entrance(q)) {
                q->h.obj_flags |= 4;
                killed++;
            }
            if (q->kind == OBJK_PAWN && (q->h.obj_flags & 0x10))
                spawned = 1;
        }
    }
    result("enemy room 0x8c..0x8f", st->enemy_rooms > c0 && spawned && walls_closed, 0,
           "room %d: started %d, gates moved %d, enemies spawned with the entrance effect %d (defeated by the test %d), "
           "cleared after %d frames, hud calls %d", room, ev_count("enemy_room_start"), walls_closed, spawned, killed, k,
           g_fx[OBJ_FX_ENEMY_ROOM]);
}
static void test_sand_slider(void) {
    const rlvl_object_t *o = find_obj(0x90, 0);
    int k, s90 = 0, slide = 0;
    if (o) {
        place_player(o->x, o->y - 24);
        for (k = 0; k < 30 && P.state != Player_State_SandSlider90; k++)
            frame(0);
        s90 = P.state == Player_State_SandSlider90;
        for (k = 0; k < 400 && P.state != Player_State_SandSlide; k++)
            frame(0);
        slide = P.state == Player_State_SandSlide;
        result("sand slider 0x90", s90, 1, "sinking state %d; sand slide %d after %d frames (z %d)", s90, slide, k, P.z >> 8);
        frames(120, DCP_KEY_RIGHT);
    }
    o = find_obj(0x91, 0);
    if (o) {
        place_player(o->x, o->y - 20);
        P.z = 0x4000; /* 0x91 stands in the background (@0x18 = 0x4000): the player arrives there from a 0x90 */
        for (k = 0; k < 30 && P.state != Player_State_SandSlider; k++) {
            teleport(o->x, o->y - 20);
            frame(0);
        }
        s90 = P.state == Player_State_SandSlider;
        for (k = 0; k < 300 && P.state == Player_State_SandSlider; k++)
            frame(0);
        result("sand slider 0x91", s90, 1, "rising state %d, landed after %d frames (z %d)", s90, k, P.z >> 8);
    }
}
static void test_bishop_hammer(void) {
    const ObjStats *st = objects_stats();
    const rlvl_object_t *o;
    int i, k = 0, ok = 0;
    u32 b0 = st->bishops_spawned;
    for (i = 0; st->bishops_spawned == b0 && (o = find_obj(7, i)) != NULL; i++) {
        if (o->p3 != 0)
            continue;
        place_on_floor(o->x - 0x50, o->y);
        for (k = 0; k < 400 && st->bishops_spawned == b0; k++)
            frame(0);
    }
    result("egg bishop 0x07", st->bishops_spawned > b0, 0, "#%d threw an Egg Bishop (0xe4) after %d frames", i - 1, k);
    for (i = 0; !ok && (o = find_obj(8, i)) != NULL; i++) {
        Obj *w;
        place_on_floor(o->x - 0x50, o->y);
        frames(2, 0);
        w = obj_at(8, o->x, o->y);
        for (k = 0; k < 400 && w && w->used && !ok; k++) {
            frame(0);
            ok = w->spr.anim == 3 && (w->f1F4 & 4);
        }
    }
    result("egg hammer 0x08", ok, 0, "#%d stomped after %d frames (screen shake via hook)", i - 1, k);
}
static void test_sand_a9(void) {
    const rlvl_object_t *o;
    int i, k = 0, n = 0;
    for (i = 0; n == 0 && (o = find_obj(0xa9, i)) != NULL; i++) {
        if (!place_on_floor(o->x + o->p0 + o->p2 / 2, o->y + o->p1 + o->p3 / 2))
            continue;
        for (k = 0; k < 20 && n == 0; k++) {
            frame(0);
            n = count_res_anim(OBJ_BAC_EFF_SAND, 0, 0);
        }
    }
    result("sand area 0xa9", n > 0, 1, "#%d: EffSand after %d frames", i - 1, k);
}

/* ============================================================ zones 4..9 (objdata v4) */
/* trigger search: put the player at offsets around a point (nearest first) until
 * the gimmick's event fires or the predicate holds.  Reported as "direct". */
#define M_PUSH 1  /* hold the direction toward the point */
#define M_DOWN 2  /* hold down */
#define M_FREE 4  /* teleport on the first frame only */
#define M_RUN 8   /* on the floor, ground speed toward the point, no teleport */
#define M_HURT 16 /* not invulnerable */
#define M_JUMP 32 /* jumping (ball) */
typedef struct {
    int i, dx, dy, k;
    Obj *o;
    const rlvl_object_t *e;
} Hit;
typedef int (*HitPred)(const Hit *h);
static const rlvl_object_t *s_pe; /* entry under test (predicates) */
static s16 s_move_x;              /* move_x given to the player while searching (0: none) */
static int trig_at(s32 cx, s32 cy, const char *ev, HitPred pred, int r, int step, int hold, int mode, Hit *h) {
    int d, dx, dy, k, e0;
    for (d = 0; d <= r; d += step)
        for (dy = -d; dy <= d; dy += step)
            for (dx = -d; dx <= d; dx += step) {
                u16 keys = 0;
                if (DCP_ABS(dx) != d && DCP_ABS(dy) != d)
                    continue;
                e0 = ev ? ev_count(ev) : 0;
                if (mode & M_RUN) {
                    if (!place_on_floor(cx + dx, cy + dy))
                        continue;
                } else
                    place_player(cx + dx, cy + dy);
                if (mode & M_HURT) /* (1, not 0: counting down to 0 lets Player_In reset the body defence) */
                    P.invuln = 1;
                if (mode & M_PUSH)
                    keys |= dx <= 0 ? DCP_KEY_RIGHT : DCP_KEY_LEFT;
                if (mode & M_DOWN)
                    keys |= DCP_KEY_DOWN;
                if (mode & M_JUMP)
                    dcp_act_jump(&P);
                for (k = 0; k < hold; k++) {
                    if (!(mode & (M_FREE | M_RUN)) || k == 0)
                        teleport(cx + dx, (mode & M_RUN) ? P.y >> 8 : cy + dy);
                    if (mode & M_RUN)
                        P.gnd_vel = (s16)(dx <= 0 ? 0x400 : -0x400);
                    if (s_move_x)
                        P.move_x = s_move_x;
                    frame(keys);
                    h->dx = dx;
                    h->dy = dy;
                    h->k = k;
                    if ((ev && ev_count(ev) > e0) || (pred && pred(h)))
                        return 1;
                }
            }
    return 0;
}
static int trig_search(u16 type, const char *ev, HitPred pred, int maxi, int r, int step, int hold, int mode, Hit *h) {
    const rlvl_object_t *o;
    int i;
    memset(h, 0, sizeof(*h));
    for (i = 0; i < maxi && (o = find_obj(type, i)) != NULL; i++) {
        s_pe = h->e = o;
        h->i = i;
        if (trig_at(o->x, o->y, ev, pred, r, step, hold, mode, h)) {
            h->o = obj_at(type, o->x, o->y);
            return 1;
        }
    }
    return 0;
}
static Obj *live_entry(void) { return s_pe ? obj_at(s_pe->type, s_pe->x, s_pe->y) : NULL; }
/* world centre of the first callback box of a live object (coll[0..2], detect) */
static int box_center(const Obj *w, s32 *x, s32 *y) {
    const ObjBox *b[4];
    int i;
    b[0] = &w->coll[0];
    b[1] = &w->coll[1];
    b[2] = &w->coll[2];
    b[3] = &w->detect;
    for (i = 0; i < 4; i++)
        if (b[i]->cb != NULL && b[i]->r > b[i]->l) {
            /* (box flags 1 / 2: mirrored, as obj_handle_collider sets them) */
            *x = (b[i]->flags & 1) ? b[i]->x - (b[i]->l + b[i]->r) / 2 : b[i]->x + (b[i]->l + b[i]->r) / 2;
            *y = (b[i]->flags & 2) ? b[i]->y - (b[i]->t + b[i]->b) / 2 : b[i]->y + (b[i]->t + b[i]->b) / 2;
            return 1;
        }
    return 0;
}
/* the same search centred on the live object's callback box (boxes far from
 * the object origin, or moving with it) */
static int trig_box(u16 type, const char *ev, int maxi, int r, int step, int hold, int mode, Hit *h) {
    const rlvl_object_t *o;
    int i;
    s32 bx, by;
    memset(h, 0, sizeof(*h));
    for (i = 0; i < maxi && (o = find_obj(type, i)) != NULL; i++) {
        Obj *w;
        s_pe = h->e = o;
        h->i = i;
        place_player(o->x, o->y - 160);
        frames(2, 0);
        w = obj_at(type, o->x, o->y);
        if (!w || !box_center(w, &bx, &by))
            continue;
        if (trig_at(bx, by, ev, NULL, r, step, hold, mode, h)) {
            h->o = obj_at(type, o->x, o->y);
            return 1;
        }
    }
    return 0;
}
/* drop the player onto a solid object from above until it stands on it */
static int drop_onto(u16 type, int maxi, Hit *h) {
    static const s8 dxs[] = {0, 8, -8, 16, -16, 24, -24};
    static const s8 dys[] = {-96, -48}; /* (the second from below a ceiling) */
    const rlvl_object_t *o;
    int i, a, b, k;
    memset(h, 0, sizeof(*h));
    for (i = 0; i < maxi && (o = find_obj(type, i)) != NULL; i++) {
        s_pe = h->e = o;
        h->i = i;
        for (b = 0; b < 2; b++)
            for (a = 0; a < 7; a++) {
                Obj *w;
                place_player(o->x + dxs[a], o->y + dys[b]);
                frames(1, 0);
                w = live_entry();
                if (!w)
                    break;
                for (k = 0; k < 90; k++) {
                    frame(0);
                    if ((w->cmap.rider == &P || w->cmap2.rider == &P) && (P.move_flags & 1)) {
                        h->dx = dxs[a];
                        h->dy = dys[b];
                        h->k = k;
                        h->o = w;
                        return 1;
                    }
                }
            }
    }
    return 0;
}
static int count_type(u16 type) {
    Obj *o;
    int n = 0;
    for (o = g_obj.head; o; o = o->next)
        if (o->used && o->prm && o->prm->type == type && !(o->h.obj_flags & 4))
            n++;
    return n;
}
static int follow_state(void (*st)(DcPlayer *), int max) { /* frames while the player stays in st */
    int k;
    for (k = 0; k < max && P.state == st; k++)
        frame(0);
    return k;
}
static int rect_center(const rlvl_object_t *o, s32 *x, s32 *y) { /* MapObj40-style area (p0..p3) */
    *x = o->x + o->p0 + o->p2 / 2;
    *y = o->y + o->p1 + o->p3 / 2;
    return 1;
}

/* ---- enemies (all zones 4..7): detection by play, destroyed by a jump */
static void test_enemy(u16 type, const char *name) {
    static const char *evs[] = {"pawn_detect", "pawn_jump_attack", "pawn_push", "pawn_bounce", "flapper_detect", "flapper_shot"};
    const ObjStats *st = objects_stats();
    const rlvl_object_t *o;
    int i, k = 0, j, act = 0, shots0;
    u32 e0 = st->enemies_destroyed;
    int ev0[6];
    char what[32];
    Obj *w = NULL;
    Hit h;
    const char *ev = strstr(name, "flapper") ? "flapper_detect" : "pawn_detect";
    if (!FIND_PRESENT(type))
        return;
    snprintf(what, sizeof(what), "%s 0x%02x", name, type);
    for (j = 0; j < 6; j++)
        ev0[j] = ev_count(evs[j]);
    shots0 = ev_count("flapper_shot");
    memset(&h, 0, sizeof(h));
    /* by play: run at it from 0x50 px on either side */
    for (i = 0; !act && (o = find_obj(type, i)) != NULL && i < 4; i++) {
        int s;
        for (s = -1; s <= 1 && !act; s += 2) {
            if (!place_on_floor(o->x + s * 0x50, o->y))
                continue;
            for (k = 0; k < 120 && !act; k++) {
                frame(s < 0 ? DCP_KEY_RIGHT : DCP_KEY_LEFT);
                act = ev_count(ev) > ev0[j = (ev[0] == 'f' ? 4 : 0)];
            }
        }
    }
    h.i = i - 1;
    if (act)
        result(what, 1, 0, "#%d noticed the player running at it after %d frames", h.i, k);
    else {
        /* direct: hold the player at offsets around it until its detect box fires */
        act = trig_search(type, ev, NULL, 2, 0x90, 0x18, 12, 0, &h);
        result(what, act, 1, "#%d noticed the player held at (%+d,%+d)", h.i, h.dx, h.dy);
    }
    {
        static const u16 shots[] = {0xe6, 0xe7, 0xeb, 0xec, 0xf0};
        int mx[5] = {0}, s;
        for (k = 0; act && k < 240; k++) {
            frame(0);
            for (s = 0; s < 5; s++)
                if (count_type(shots[s]) > mx[s])
                    mx[s] = count_type(shots[s]);
        }
        for (s = 0; s < 5; s++)
            if (mx[s])
                printf("    spawned attack objects 0x%02x: up to %d live\n", shots[s], mx[s]);
    }
    printf("    %s events:", what);
    for (j = 0; j < 6; j++)
        if (ev_count(evs[j]) > ev0[j])
            printf(" %s=%d", evs[j], ev_count(evs[j]) - ev0[j]);
    printf(" (shots fired %d)\n", ev_count("flapper_shot") - shots0);
    for (i = 0; st->enemies_destroyed == e0 && (o = find_obj(type, i)) != NULL && i < 6; i++) {
        place_player(o->x - 0x60, o->y);
        frames(2, 0);
        w = obj_type_near(type, o->x, o->y, 0x100);
        if (!w)
            continue;
        for (k = 0; k < 160 && st->enemies_destroyed == e0 && w->used; k++) {
            if (k % 16 == 0) { /* the enemy moves: drop onto it again (Rhino 0xe: vitality 2) */
                place_player(w->h.x >> 8, (w->h.y >> 8) - 36);
                dcp_act_jump(&P);
                P.vel_y = 0x200;
            }
            frame(DCP_KEY_A);
        }
    }
    result("  destroyed by a jump", st->enemies_destroyed > e0, 1, "#%d: %u destroyed", i - 1, st->enemies_destroyed - e0);
}

/* ---- zone 4: Night Carnival (z41, z42; z91) */
static void test_crane_slide(void) {
    Hit h;
    int ok, k = 0;
    s32 x0 = 0;
    if (!FIND_PRESENT(0xae))
        return;
    s_move_x = (s16)(find_obj(0xae, 0)->p0 >= 0 ? 0x600 : -0x600); /* jumping at it with speed */
    ok = trig_box(0xae, "crane_slide", 4, 32, 4, 4, M_FREE, &h);
    s_move_x = 0;
    if (ok && h.o) {
        int bad = 0, n = 0;
        x0 = h.o->h.x;
        for (k = 0; k < 300 && P.gimmick == h.o; k++) {
            /* CraneSlide_State: carrying, not yet stopped: speed = ObjSpd_DownSet(speed, 10), move = clamp(+-0x800) */
            s16 g0 = h.o->h.gnd_vel;
            u32 f0 = h.o->f1F4;
            frame(0);
            if (P.gimmick == h.o && !(f0 & 3) && !(h.o->f1F4 & 1)) {
                s16 g = (s16)dcp_spd_down(g0, 10), mv = g < -0x800 ? -0x800 : g > 0x800 ? 0x800 : g;
                n++;
                /* (the start-of-rail clamp zeroes both) */
                if (!(h.o->h.gnd_vel == g && h.o->h.vel_x == mv) && !(h.o->h.gnd_vel == 0 && h.o->h.vel_x == 0))
                    bad++;
            }
        }
        printf("    crane slide exact: %d carried frames checked, %d off\n", n, bad);
        CHECK(n > 0 && bad == 0, "crane slide 0xae exact: speed -10 per frame, move clamped to +-0x800 (%d of %d off)", bad, n);
    }
    result("crane slide 0xae", ok, 1, "#%d grabbed at (%+d,%+d); slid %d px over %d frames holding on", h.i, h.dx, h.dy,
           ok && h.o ? (h.o->h.x - x0) >> 8 : 0, k);
}
static void test_shooter(void) {
    Hit h;
    int ok, k, launched = 0, cam = 0, e0 = ev_count("shooter");
    if (FIND_PRESENT(0xbb)) {
        ok = drop_onto(0xbb, 5, &h);
        s32 w8 = 0;
        if (ok) {
            for (k = 0; k < 40; k++) {
                frame(DCP_KEY_DOWN);
                w8 = h.o ? h.o->w8 : 0;
                cam |= (g_obj_camera[2].flags & 0x40000) != 0;
                if (getenv("OBJDBG"))
                    printf("    shooter k%d w8 %d wC %d vel_y %d rider %d toucher %d P(%d,%d) vy %d mf %x\n", k, h.o ? h.o->w8 : -1,
                           h.o ? h.o->wC : 0, h.o ? h.o->h.vel_y : 0, h.o && h.o->cmap.rider == &P,
                           h.o && h.o->cmap.toucher == &P, P.x >> 8, P.y >> 8, P.vel_y, P.move_flags);
            }
            for (k = 0; k < 10 && ev_count("shooter") == e0; k++) {
                if (h.o && h.o->w8)
                    w8 = h.o->w8; /* (the shooter runs before the player: it still saw Down held once) */
                frame(0);
            }
            launched = ev_count("shooter") > e0;
            if (launched) {
                /* Shooter_State_Main (0xbb): charge w8 + 1 per frame held (max 0x40), released above 8:
                 * Player_Action_SpringLaunch(p, 0, -(w8 << 6)) (3/4 under water) */
                s16 v = (s16)-((w8 > 0x40 ? 0x40 : w8) << 6);
                if (P.pf & 0x4000000)
                    v = (s16)((v >> 1) + (v >> 2));
                v = (s16)(v + P.gravity); /* the player moves after the shooter in the same frame */
                CHECK(w8 > 8 && P.vel_y == v, "shooter 0xbb exact: charge %d -> vel_y %d (got %d; pf %x mf %x st in_air %d grav %d)", (int)w8, v,
                      P.vel_y, P.pf, P.move_flags, P.state == dcp_state_in_air, P.gravity);
            }
        }
        result("shooter 0xbb", launched && P.vel_y < 0, 1, "#%d stood on (%+d,%+d), charged 40 frames: launched %d vel_y %d, camera flag 0x40000 %d",
               h.i, h.dx, h.dy, launched, P.vel_y, cam);
        frames(60, 0);
    }
    if (FIND_PRESENT(0xbc)) {
        e0 = ev_count("shooter");
        ok = 0;
        {
            /* charge by pushing, then let go */
            const rlvl_object_t *o = find_obj(0xbc, 0);
            int s;
            for (s = -1; s <= 1 && !ok; s += 2) {
                place_on_floor(o->x + s * 40, o->y);
                Obj *so = obj_at(0xbc, o->x, o->y);
                s32 w8 = 0;
                for (k = 0; k < 60; k++) {
                    /* the push animation 0xb -> 0xc waits for the animation end flag (disp 8),
                     * which the player sprite animator sets: no animator in this harness */
                    if (P.anim == 0xb)
                        P.disp_flags |= 8;
                    frame(s < 0 ? DCP_KEY_RIGHT : DCP_KEY_LEFT);
                    w8 = so ? so->w8 : 0;
                }
                for (k = 0; k < 10 && ev_count("shooter") == e0; k++) {
                    if (so && so->w8)
                        w8 = so->w8;
                    frame(0);
                }
                ok = ev_count("shooter") > e0;
                if (ok && so) {
                    /* Shooter_State_Main (0xbc): s = w8 << 6, negated unless the shooter faces left;
                     * Player_Action_SpringLaunch(p, s, 0) */
                    s16 v = (s16)((w8 > 0x40 ? 0x40 : w8) << 6);
                    if (!(so->h.disp_flags & 1))
                        v = (s16)-v;
                    if (P.pf & 0x4000000)
                        v = (s16)((v >> 1) + (v >> 2));
                    CHECK(w8 > 8 && (P.vel_x == v || P.gnd_vel == v), "shooter 0xbc exact: charge %d -> vel_x %d (got %d / gnd %d)", (int)w8,
                          v, P.vel_x, P.gnd_vel);
                }
            }
        }
        result("shooter 0xbc", ok, 0, "pushed into the side shooter for 60 frames and let go (push anim end flag set by the test): launched %d, gnd_vel %d", ok, P.gnd_vel);
    }
}
static void test_swing_sky(void) {
    Hit h;
    int ok, k = 0;
    if (!FIND_PRESENT(0xbd))
        return;
    ok = trig_box(0xbd, "swing_sky", 4, 32, 4, 4, M_FREE, &h);
    if (ok) {
        s32 x0 = h.o ? h.o->wC : 0, amp = 0;
        int bad = 0, n = 0;
        for (k = 0; k < 150 && P.state == Player_State_SwingSky; k++) {
            /* Player_State_SwingSky: the player is the swing's hang point (@1C/@20), angle = swing - 0x40,
             * ground speed / vel_y = the swing's displacement (@28/@2A) */
            s32 bx = h.o ? h.o->h.px : 0, by = h.o ? h.o->h.py : 0;
            u8 ba = h.o ? h.o->h.angle : 0;
            s16 bmx = h.o ? h.o->h.move_x : 0, bmy = h.o ? h.o->h.move_y : 0;
            frame(k < 60 ? ((k / 20) & 1 ? DCP_KEY_LEFT : DCP_KEY_RIGHT) : 0); /* pump */
            if (h.o && DCP_ABS(h.o->wC - x0) > amp)
                amp = DCP_ABS(h.o->wC - x0);
            if (h.o && P.state == Player_State_SwingSky) {
                int before = P.x == bx && P.y == by && P.angle == (u8)(ba - 0x40) && P.gnd_vel == bmx && P.vel_y == bmy;
                int after = P.x == h.o->h.px && P.y == h.o->h.py && P.angle == (u8)(h.o->h.angle - 0x40) && P.gnd_vel == h.o->h.move_x &&
                            P.vel_y == h.o->h.move_y;
                n++;
                bad += !before && !after;
            }
        }
        CHECK(n > 0 && bad == 0, "swing sky 0xbd exact: player at the hang point, angle - 0x40, speed = swing displacement (%d of %d off)",
              bad, n);
        frame(DCP_KEY_A);
        frames(2, 0);
        printf("    swing amplitude %#x, released with A: %d (vel %d,%d)\n", amp, P.state != Player_State_SwingSky, P.vel_x, P.vel_y);
        ok = ok && P.state != Player_State_SwingSky;
    }
    result("swing sky 0xbd", ok && k > 0, 1, "#%d caught at (%+d,%+d); swung %d frames then jumped off", h.i, h.dx, h.dy, k);
}
static void test_switch_neon(void) {
    Hit h;
    u16 t;
    int ok, k, moved = 0, n = 0;
    s16 *tm = obj_switch_timers();
    for (t = 0xbe; t <= 0xbf; t++) {
        int tl0 = ev_count("switch_timer");
        if (!FIND_PRESENT(t))
            continue;
        ok = trig_search(t, "switch", NULL, 6, 48, 4, 30, M_PUSH, &h);
        if (ok && h.o) {
            /* Switch_State_*: gSwitchTimers[p6] = (p8 + p9) * 2 while pressed; SwitchTimer_Main then -1 per frame */
            s16 v = (s16)((h.o->prm->p8 + h.o->prm->p9) * 2), got = tm[h.o->prm->p6 & 0x1f], g2;
            frame(0);
            g2 = tm[h.o->prm->p6 & 0x1f];
            CHECK((got == v || got == v - 1) && (g2 == v || g2 == got - 1 || g2 == v - 1),
                  "switch %#x exact: timer (p8 %d + p9 %d) * 2 = %d (got %d, then %d)", t, h.o->prm->p8, h.o->prm->p9, v, got, g2);
        }
        result(t == 0xbe ? "switch 0xbe" : "switch 0xbf", ok && tm[h.e->p0 & 0x1f] != 0, 1,
               "#%d pressed at (%+d,%+d): timer[%d] = %d, SwitchTimer %d", h.i, h.dx, h.dy, ok ? h.e->p0 & 0x1f : -1,
               ok ? tm[h.e->p0 & 0x1f] : 0, ev_count("switch_timer") > tl0);
    }
    for (t = 0xc0; t <= 0xc1; t++) {
        const rlvl_object_t *o = find_obj(t, 0);
        Obj *w;
        s32 x0, y0, px0, pw0 = 0, py0 = 0, ppy0 = 0;
        int carried = 0;
        if (!o)
            continue;
        place_player(o->x, o->y - 60);
        frames(2, 0);
        w = obj_at(t, o->x, o->y);
        if (!w)
            continue;
        tm[o->p0 & 0x1f] = 600; /* direct: the switch timer (SwitchTimer counts it down) */
        x0 = w->h.x;
        y0 = w->h.y;
        frames(2, 0);
        ok = 0;
        px0 = 0;
        place_player((w->h.x >> 8) + 8, (w->h.y >> 8) - 64);
        for (k = 0; k < 90 && w->used; k++) {
            frame(0);
            if (!ok && (w->cmap.rider == &P || w->cmap2.rider == &P) && (P.move_flags & 1)) {
                ok = 1;
                px0 = P.x;
                pw0 = w->h.x;
                py0 = w->h.y;
                ppy0 = P.y;
            }
            n = w->spr.n_multi;
        }
        if (ok) /* carried: the player moved with the route */
            carried = DCP_ABS((P.x - px0) - (w->h.x - pw0)) < 0x400 && DCP_ABS((P.y - ppy0) - (w->h.y - py0)) < 0x400 &&
                      (w->h.x != pw0 || w->h.y != py0);
        {
            /* NeonRoute_State_Move: w8 = 12-px segments passed (one per frame), back to home after p8 * 12 */
            int bad = 0, wraps = 0, n;
            ObjParam *pp = w->prm;
            for (n = 0; n < 240 && w->used && w->state != NULL; n++) {
                s32 pos, home, d, pos0 = pp->type == 0xc0 ? w->h.x : w->h.y, mv;
                u32 seg0 = w->w8;
                tm[pp->p6 & 0x1f] = 600;
                frame(0);
                pos = pp->type == 0xc0 ? w->h.x : w->h.y;
                home = pp->type == 0xc0 ? w->home_x : w->home_y;
                d = pp->p7 > 0 ? pos - (home << 8) : (home << 8) - pos;
                /* (the segment test runs before this frame's move) */
                mv = DCP_ABS(pos - pos0);
                if (mv > 0x1000)
                    mv = 0; /* (the wrap) */
                d -= mv;
                if (w->w8 == 0 && seg0 != 0)
                    wraps++;
                else if ((u32)w->w8 != seg0 && (u32)w->w8 != seg0 + 1)
                    bad++;
                if (d < 0 || d > (s32)(pp->p8 * 12) << 8 || (u32)d > (w->w8 + 1) * 12u << 8) {
                    if (bad < 3)
                        printf("    neon n%d: d %d.%02x px, seg %u -> %u, p8 %d p7 %d\n", n, d >> 8, d & 0xff, (unsigned)seg0,
                               (unsigned)w->w8, pp->p8, pp->p7);
                    bad++;
                }
            }
            CHECK(bad == 0, "neon route %#x exact: 12-px segment count and the p8 * 12 wrap over 240 frames (%d off, %d wraps)", t, bad,
                  wraps);
        }
        moved = (w->h.x != x0 || w->h.y != y0);
        result(t == 0xc0 ? "neon route 0xc0" : "neon route 0xc1", moved, 1,
               "timer[%d] set: route moved (%d,%d) px, solid %d, stood on %d, carried with it %d, %d segments drawn",
               o->p0 & 0x1f, (w->h.x - x0) >> 8, (w->h.y - y0) >> 8, w->has_cmap2, ok, carried, n);
    }
}
static void test_dark_area(void) {
    const rlvl_object_t *o;
    u16 t;
    s32 x, y;
    int f0 = g_fx[OBJ_FX_ZONE4_BLDY], in = 0, out = 0;
    if ((o = find_obj(0xc3, 0)) != NULL) {
        rect_center(o, &x, &y);
        place_player(x, y);
        frames(3, 0);
        in = (P.pf2 & 0x8000) != 0;
        result("dark area in 0xc3", in && g_fx[OBJ_FX_ZONE4_BLDY] > f0, 1, "pf2 0x8000 %d, ZoneFx bldy hook %d", in,
               g_fx[OBJ_FX_ZONE4_BLDY] - f0);
    }
    if ((o = find_obj(0xc4, 0)) != NULL && in) {
        f0 = g_fx[OBJ_FX_ZONE4_BLDY];
        rect_center(o, &x, &y);
        teleport(x, y); /* (not place_player: it would clear pf2) */
        camera_follow(1);
        objects_respawn_view();
        frames(3, 0);
        out = !(P.pf2 & 0x8000);
        result("dark area out 0xc4", out && g_fx[OBJ_FX_ZONE4_BLDY] > f0, 1, "pf2 0x8000 cleared %d, ramp-down hook %d", out,
               g_fx[OBJ_FX_ZONE4_BLDY] - f0);
    }
    for (t = 0xc5; t <= 0xc6; t++) {
        /* layer areas active while gSwitchTimers[0] runs: pf2 0x2000 / 0x10000 is a
         * one-frame flag set by the box check and consumed by the player task */
        u32 bit = t == 0xc5 ? 0x2000 : 0x10000;
        int ok, k, seen;
        if ((o = find_obj(t, 0)) == NULL)
            continue;
        rect_center(o, &x, &y);
        obj_switch_timers()[0] = 0;
        place_player(x, y);
        frames(3, 0);
        ok = !(g_pf2_pre & bit) && (g_of_pre & 1);
        obj_switch_timers()[0] = 100;
        place_player(x, y);
        for (k = 0, seen = 0; k < 6; k++) {
            frame(0);
            seen |= (g_pf2_pre & bit) && !(g_of_pre & 1);
        }
        result(t == 0xc5 ? "switch area 0xc5" : "switch layer 0xc6", ok && seen, 1,
               "inactive while timer[0] = 0 %d; timer set: pf2 %#x + layer bit cleared %d, layer override %d", ok, bit, seen,
               t == 0xc6 ? P.layer_ovr : 0);
        obj_switch_timers()[0] = 0;
    }
}

/* ---- zone 5: Huge Crisis (z51, z52; z92) */
static void test_crane(void) {
    const rlvl_object_t *o;
    Hit h;
    int i, ok = 0, k;
    Obj *ed = NULL;
    s32 y0 = 0;
    if (!FIND_PRESENT(0xa8))
        return;
    memset(&h, 0, sizeof(h));
    for (i = 0; !ok && (o = find_obj(0xa8, i)) != NULL && i < 4; i++) {
        place_player(o->x, o->y + 40);
        frames(2, 0);
        s32 bx, by;
        ed = obj_type_near(0xed, o->x, o->y, 0x200);
        if (!ed || !box_center(ed, &bx, &by))
            continue;
        s_pe = NULL;
        ok = trig_at(bx, by, "crane", NULL, 24, 4, 3, M_FREE, &h);
        h.i = i;
    }
    if (ok) {
        y0 = ed->h.y;
        for (k = 0; k < 60; k++)
            frame(0);
        printf("    holding the hook: %d (player gimmick), crane flag f1F4 %#x\n", P.gimmick == ed, ed->parent ? ed->parent->f1F4 : 0);
    }
    result("crane 0xa8 / 0xed", ok, 1, "#%d hook (0xed) touched at (%+d,%+d), hook moved %d px in 60 frames; 0xee parts live %d",
           h.i, h.dx, h.dy, ok ? (ed->h.y - y0) >> 8 : 0, count_type(0xee));
}
static void test_pipe(void) {
    Hit h;
    u16 t, ts[4] = {0xa6, 0xab, 0xac, 0xad};
    int ok = 0, k = 0, j;
    s32 x0 = 0, y0 = 0;
    memset(&h, 0, sizeof(h));
    if (!FIND_PRESENT(0xa6) && !FIND_PRESENT(0xab) && !FIND_PRESENT(0xac) && !FIND_PRESENT(0xad))
        return;
    for (j = 0; j < 4 && !ok; j++) {
        t = ts[j];
        if (!FIND_PRESENT(t))
            continue;
        ok = trig_search(t, "pipe", NULL, 4, 32, 4, 3, M_FREE, &h);
    }
    if (ok) {
        x0 = P.x;
        y0 = P.y;
        k = follow_state(Player_State_Pipe, 1200);
    }
    result("pipe 0xa6/0xab..0xad", ok && k > 0, 1, "0x%02x #%d entered at (%+d,%+d); travelled (%d,%d) px in %d frames, exited %d",
           ok ? h.e->type : 0, h.i, h.dx, h.dy, (P.x - x0) >> 8, (P.y - y0) >> 8, k, P.state != Player_State_Pipe);
}
static void test_jet_catapult(void) {
    Hit h;
    int ok, k = 0, l0 = ev_count("jet_catapult_launch");
    if (!FIND_PRESENT(0xa7))
        return;
    ok = trig_search(0xa7, "jet_catapult", NULL, 4, 48, 4, 3, 0, &h);
    if (ok)
        for (k = 0; k < 900 && ev_count("jet_catapult_launch") == l0; k++)
            frame(0);
    result("jet catapult 0xa7", ok && ev_count("jet_catapult_launch") > l0, 1,
           "#%d boarded at (%+d,%+d); catapult stopped and launched the player after %d frames (vel %d,%d)", h.i, h.dx, h.dy, k,
           P.vel_x, P.vel_y);
}
static void test_piston(void) {
    Hit h;
    u16 t;
    for (t = 0xa0; t <= 0xa1; t++) {
        int ok, k, ymin = 1 << 30, ymax = -(1 << 30);
        if (!FIND_PRESENT(t))
            continue;
        ok = drop_onto(t, 4, &h);
        for (k = 0; ok && k < 120 && h.o->cmap.rider == &P; k++) {
            frame(0);
            if (P.y < ymin) ymin = P.y;
            if (P.y > ymax) ymax = P.y;
        }
        result(t == 0xa0 ? "piston 0xa0" : "piston 0xa1", ok, 1, "#%d ridden from (%+d,%+d); player carried over %d px for %d frames",
               h.i, h.dx, h.dy, ok ? (ymax - ymin) >> 8 : 0, k);
        if (ok && h.o) {
            /* Piston_UpdateOscillation: t = (frame * spd + phase) & 0x3ff (spd = flags & 7; 0: ((frame +
             * (phase >> 2)) & 0xff) << 2), y = (centre + ((p9 / 2) * sin(t << 6 (+0x8000 if flags 8))) >> 12) << 8 */
            /* (the piston runs before the player task, i.e. before this frame's counter step: frame - 1) */
            Obj *o = h.o;
            int bad = 0, n;
            s32 y0 = o->h.y;
            for (n = 0; n < 64; n++) {
                u16 f = o->prm->flags, tt = (u16)o->wC;
                u8 spd = f & 7;
                s32 amp = (o->prm->p9 << 15) >> 16, y;
                u32 fr;
                frame(0);
                if (!o->used || o->prm == NULL || o->prm->type != t || o->state == NULL)
                    break; /* (went out of view: released) */
                fr = W.frame - (o->prio < OBJ_PRIO_PLAYER ? 1 : 0);
                tt = spd ? (u16)((fr * spd + tt) & 0x3ff) : (u16)(((fr + (tt >> 2)) & 0xff) << 2);
                y = (o->h.py + (((s16)amp * dcp_sin((u16)((tt << 6) + ((f & 8) ? 0x8000 : 0)))) >> 12)) << 8;
                if (o->h.y != y || o->h.move_y != (s16)(y - y0)) {
                    if (bad < 3)
                        printf("    piston n%d: y %#x want %#x (py %d t %#x flags %#x p9 %d) move %d\n", n, o->h.y, y, o->h.py, tt, f,
                               o->prm->p9, o->h.move_y);
                    bad++;
                }
                y0 = o->h.y;
            }
            CHECK(bad == 0 && n > 20, "piston %#x exact: oscillation y over %d frames (%d off)", t, n, bad);
        }
    }
}
static void test_valve(void) {
    const rlvl_object_t *o;
    int i, k = 0, ok = 0, steam = 0, e0 = ev_count("valve");
    if (!FIND_PRESENT(0xa2) && !FIND_PRESENT(0xa3) && !FIND_PRESENT(0xa4) && !FIND_PRESENT(0xa5))
        return;
    for (i = 0; !ok && i < 64; i++) {
        u16 t = (u16)(0xa2 + (i & 3));
        if ((o = find_obj(t, i >> 2)) == NULL)
            continue;
        place_player(o->x - 64, o->y - 32);
        for (k = 0; k < 700 && !ok; k++) {
            teleport(o->x - 64, o->y - 32);
            frame(0);
            ok = ev_count("valve") > e0;
        }
        for (; ok && k < 760; k++) {
            frame(0);
            if (count_res_anim(OBJ_BAC_EFF_STEAM, 0, 99) > steam)
                steam = count_res_anim(OBJ_BAC_EFF_STEAM, 0, 99);
        }
    }
    result("valve 0xa2..0xa5 / steam", ok && steam > 0, 0, "valve opened after its timer (%d frames); steam puffs (0xf1) live %d", k,
           steam);
}

/* ---- zone 6: Altitude Limit (z61, z62) */
/* teleport keeping the player state, spawn around the new spot, return the live
 * entry's callback-box centre */
static int goto_box(const rlvl_object_t *o, s32 *bx, s32 *by) {
    Obj *w;
    teleport(o->x, o->y);
    camera_follow(1);
    objects_respawn_view();
    frame(0);
    w = obj_at(o->type, o->x, o->y);
    return w && box_center(w, bx, by);
}
static void test_glider(void) {
    const rlvl_object_t *b6 = find_obj(0xb6, 0), *b7 = find_obj(0xb7, 0);
    Hit h;
    int ok, k = 0, e0 = ev_count("glider_sequence"), e7 = ev_count("glider_sequence_end"), play = 0, intro = 0, seq = 0, maxr = 0, r0;
    s32 x0 = 0, bx, by;
    if (!FIND_PRESENT(0xb5))
        return;
    ok = trig_search(0xb5, "glider", NULL, 2, 48, 4, 3, M_FREE, &h);
    if (ok) {
        int bad = 0, n = 0;
        x0 = P.x;
        for (k = 0; k < 900 && P.state == Player_State_Glider && ev_count("glider_sequence") == e0; k++) {
            /* Player_State_Glider: timer + 1, vel_y = ObjSpd_UpSet(vel_y, timer & 0x10 ? -0x10 : 0x10, 0x80),
             * let go after 0x168 frames (or on the ground after 0xc) */
            s32 t0 = P.timer + 1;
            s16 vy = (s16)dcp_spd_up(P.vel_y, (t0 & 0x10) ? -0x10 : 0x10, 0x80);
            frame(0);
            if (P.state == Player_State_Glider) {
                n++;
                if (P.timer != t0 || P.vel_y != vy)
                    bad++;
            } else if (t0 <= 0xc || (t0 <= 0x168 && !(P.move_flags & 1) && ev_count("glider_sequence") == e0))
                bad++;
        }
        play = ev_count("glider_sequence") > e0;
        CHECK(bad == 0 && n > 0, "glider 0xb5 exact: timer, vel_y bob +-0x10 / max 0x80, release after 0x168 frames (%d of %d off, %d)", bad,
              n, (int)P.timer);
    }
    result("glider 0xb5", ok && P.x != x0, 1, "#%d grabbed at (%+d,%+d); glided %d px in %d frames%s", h.i, h.dx, h.dy,
           ok ? (P.x - x0) >> 8 : 0, k, play ? ", into the glider sequence area by play" : "");
    if (!b6)
        return;
    if (!play) { /* direct: gliding, put into the 0xb6 area */
        if (P.state != Player_State_Glider)
            trig_search(0xb5, "glider", NULL, 2, 48, 4, 3, M_FREE, &h);
        if (P.state == Player_State_Glider && goto_box(b6, &bx, &by)) {
            teleport(bx, by);
            frames(2, 0);
        }
    }
    ok = ev_count("glider_sequence") > e0;
    x0 = P.x;
    r0 = ev_count("glider_ring");
    for (k = 0; ok && k < 3000 && (P.state == Player_State_GliderSequenceIntro || P.state == Player_State_GliderSequence); k++) {
        frame((k / 40) & 1 ? DCP_KEY_UP : DCP_KEY_DOWN);
        intro |= P.state == Player_State_GliderSequenceIntro;
        seq |= P.state == Player_State_GliderSequence;
        if (count_type(0xb8) > maxr)
            maxr = count_type(0xb8);
    }
    result("glider sequence 0xb6", ok && intro && seq, !play,
           "started%s: intro %d, sequence %d for %d frames, wind %#x, rings 0xb8 live up to %d, collected %d", play ? " by play" : "",
           intro, seq, k, W.wind_x, maxr, ev_count("glider_ring") - r0);
    result("glider ring 0xb8", ev_count("glider_ring") > r0, 0,
           "%d collected flying the sequence (MapObj13 exists only while the sequence wind blows)", ev_count("glider_ring") - r0);
    if (b7) {
        /* flown into by play: the sequence ends at the 0xb7 area (the loop above stops there) */
        int ex = P.state == Player_State_GliderSequenceExit, endp = ev_count("glider_sequence_end") > e7;
        for (k = 0; k < 300 && P.state == Player_State_GliderSequenceExit; k++)
            frame(0);
        result("glider sequence end 0xb7", endp && ex, 0, "reached the end area flying: exit state %d, left it after %d frames, wind %#x",
               ex, k, W.wind_x);
    }
}
static void test_parachute(void) {
    Hit h;
    int ok, k = 0, rel = 0;
    if (!FIND_PRESENT(0xb4))
        return;
    ok = trig_search(0xb4, "parachute", NULL, 3, 48, 4, 3, M_FREE, &h);
    if (ok) {
        for (k = 0; k < 600 && (P.state == Player_State_Parachute || P.state == Player_State_ParachuteRelease); k++) {
            frame(k > 120 ? DCP_KEY_A : 0);
            rel |= P.state == Player_State_ParachuteRelease;
        }
    }
    result("parachute 0xb4", ok, 1, "#%d grabbed at (%+d,%+d): floated %d frames, release state %d", h.i, h.dx, h.dy, k, rel);
}
static void test_saucer(void) {
    Hit h;
    int ok, k = 0, x0 = ev_count("saucer_explode");
    if (!FIND_PRESENT(0xb3))
        return;
    ok = trig_search(0xb3, "saucer", NULL, 3, 48, 4, 3, M_FREE, &h);
    if (ok)
        for (k = 0; k < 900 && ev_count("saucer_explode") == x0; k++)
            frame(0);
    result("saucer 0xb3", ok, 1, "#%d boarded at (%+d,%+d); exploded %d after %d frames", h.i, h.dx, h.dy,
           ev_count("saucer_explode") > x0, k);
}
static void test_needle_g(void) {
    const ObjStats *st = objects_stats();
    Hit h;
    u16 t;
    for (t = 0xb1; t <= 0xb2; t++) {
        u32 h0 = st->hurts;
        int ok;
        if (!FIND_PRESENT(t))
            continue;
        ok = trig_search(t, "hurt", NULL, 3, 24, 4, 4, M_HURT, &h) || st->hurts > h0;
        result(t == 0xb1 ? "needle 0xb1" : "needle 0xb2", ok, 1, "#%d hurt the player at (%+d,%+d)", h.i, h.dx, h.dy);
    }
}
static void test_clouds(void) {
    int n13 = count_type(0x13), n9 = count_type(0xb9), na = count_type(0xba);
    if (!FIND_PRESENT(0xc7))
        return;
    result("cloud spawner 0xc7", ev_count("cloud_spawner") > 0, 0, "spawner created %d; MapObj13 live 0x13 %d, 0xb9 %d, 0xba %d",
           ev_count("cloud_spawner"), n13, n9, na);
}

/* ---- zone 7: Dead Line (z71, z72) */
static void test_rocket(void) {
    Hit h;
    int ok, k = 0, x0 = ev_count("rocket_explode");
    if (!FIND_PRESENT(0x93))
        return;
    ok = trig_search(0x93, "rocket", NULL, 4, 48, 4, 3, M_FREE, &h);
    if (ok)
        for (k = 0; k < 900 && ev_count("rocket_explode") == x0; k++)
            frame(0);
    result("rocket 0x93", ok, 1, "#%d grabbed at (%+d,%+d); exploded %d after %d frames", h.i, h.dx, h.dy,
           ev_count("rocket_explode") > x0, k);
}
static void test_air(void) {
    Hit h;
    int ok, k = 0;
    if (!FIND_PRESENT(0x94))
        return;
    ok = trig_search(0x94, "air", NULL, 4, 48, 4, 3, 0, &h);
    if (ok)
        k = follow_state(Player_State_Air, 300);
    result("air 0x94", ok, 1, "#%d blown at (%+d,%+d); Air state %d frames, vel_y %d", h.i, h.dx, h.dy, k, P.vel_y);
}
static void test_cannon(void) {
    Hit h;
    int ok, k = 0;
    if (!FIND_PRESENT(0x95))
        return;
    ok = trig_search(0x95, "cannon", NULL, 4, 48, 4, 3, M_FREE, &h);
    if (ok) {
        for (k = 0; k < 300 && P.state == Player_State_Cannon; k++)
            frame(k == 60 ? DCP_KEY_A : 0);
    }
    result("cannon 0x95", ok && k > 0, 1, "#%d entered at (%+d,%+d); fired after %d frames, vel %d,%d", h.i, h.dx, h.dy, k, P.vel_x,
           P.vel_y);
}
static void test_swing(void) {
    Hit h;
    int ok, k = 0;
    if (!FIND_PRESENT(0x96))
        return;
    ok = trig_search(0x96, "swing", NULL, 6, 64, 8, 20, M_RUN, &h);
    if (ok) {
        k = follow_state(Player_State_Swing, 600);
        if (k > 0) {
            /* jump off mid-swing: Player_State_Swing clears the angle and the ground speed, then act_jump:
             * vel (0, -jump) (+ one gravity step) */
            int k2;
            s16 vx, vy;
            ok = trig_search(0x96, "swing", NULL, 6, 64, 8, 20, M_RUN, &h);
            for (k2 = 0; k2 < 6 && P.state == Player_State_Swing; k2++)
                frame(0);
            printf("    swing jump-off: %s\n", P.state == Player_State_Swing ? "pressed A mid-swing" : "not caught again");
            if (P.state == Player_State_Swing) {
                frame(DCP_KEY_A);
                vx = P.vel_x, vy = P.vel_y;
                CHECK(vx == 0 && (vy == -P.jump || vy == (s16)(-P.jump + P.gravity)) && P.angle == 0,
                      "swing 0x96 exact: A mid-swing -> vel (0, -jump %d) got (%d, %d), angle %#x", P.jump, vx, vy, P.angle);
            }
        }
    }
    result("swing 0x96", ok && k > 0, 0, "#%d caught running from (%+d,%+d); swung %d frames, released vel %d,%d", h.i, h.dx, h.dy,
           k, P.vel_x, P.vel_y);
}
static void test_duct(void) {
    const rlvl_object_t *o;
    int i, k = 0, ok = 0, e0 = ev_count("duct");
    if (!FIND_PRESENT(0x97))
        return;
    for (i = 0; !ok && (o = find_obj(0x97, i)) != NULL && i < 6; i++) {
        place_player(o->x, o->y);
        frames(2, 0);
        P.rings = 20;
        dcp_scatter_rings(&P, objects_ring_scatter_level());
        for (k = 0; k < 120 && !ok; k++) {
            frame(0);
            ok = ev_count("duct") > e0;
        }
    }
    result("duct 0x97", ok, 1, "#%d: scattered rings beside it, sucked in after %d frames", i - 1, k);
    if (ok) {
        /* RingManager_SuckScatteredRings: the rings caught spiral in (speed + 0x10 per frame) and vanish within
         * 4 px; Duct_State_CheckPlayer re-arms its sound (disp 0x10) once none is left */
        Obj *d = obj_at(0x97, o->x, o->y);
        int n;
        for (n = 0; n < 120 && d && !(d->h.disp_flags & 0x10); n++)
            frame(0);
        CHECK(d && (d->h.disp_flags & 0x10) && n < 120, "duct 0x97 exact: sucked rings gone and the sound re-armed after %d frames", n);
    }
}
static void test_9c(void) {
    const rlvl_object_t *o;
    int i, k, ok = 0, z = 0, cnt = 0;
    s32 bx, by;
    if (!FIND_PRESENT(0x9c))
        return;
    for (i = 0; !ok && (o = find_obj(0x9c, i)) != NULL && i < 8; i++) {
        Obj *w;
        place_player(o->x, o->y);
        frames(2, 0);
        w = obj_at(0x9c, o->x, o->y);
        if (!w || !box_center(w, &bx, &by))
            continue;
        place_player(bx, by);
        for (k = 0; k < 30; k++) {
            frame(0);
            /* pf2 0x1000 (set by the box, turned into 0x80000 by Player_Last) */
            if (g_pf2_pre & 0x1000)
                cnt++;
        }
        ok = cnt > 0;
        z = P.z >> 8;
        if (ok)
            printf("    lane #%d depth %d: player depth %d, on the lane floor %d\n", i, w->h.z >> 8, z, (P.pf2 & 0x100000) != 0);
    }
    result("depth lane 0x9c", ok, 1, "inside the lane %d of 30 frames", cnt);
}
static void test_v4_decor(const char *zone) {
    const ObjStats *st = objects_stats();
    char what[32];
    snprintf(what, sizeof(what), "decorations (%s)", zone);
    result(what, st->dec_spawned > 0, 0, "spawned %u, despawned %u; palette-anim hooks %d", st->dec_spawned, st->dec_despawned,
           g_fx[OBJ_FX_PALETTE_ANIM]);
    if (g_fx[OBJ_FX_PALETTE_ANIM] > 0) {
        /* PaletteAnim (obj_palanim.c): the row of a started decoration steps through
         * the animations of its sequence table (a palette-bank choice when drawn) */
        Obj *o;
        int slot = 0, k, steps = 0, last, first = -2;
        for (o = g_obj.head; o; o = o->next)
            if (o->used && o->spr.palanim) {
                slot = o->spr.palanim;
                break;
            }
        last = obj_palanim_anim(slot);
        for (k = 0; k < 120 && slot; k++) {
            frame(0);
            if (obj_palanim_anim(slot) != last) {
                last = obj_palanim_anim(slot);
                if (first == -2)
                    first = k;
                steps++;
            }
        }
        if (!slot) {
            /* no decoration with one is alive now: a stand-in DecArrow (table
             * 0x0236457c: anims 3..9, 6 frames each; the first step after 7 calls) */
            static Obj fake;
            static const s16 want[8] = { 4, 5, 6, 7, 8, 9, 3, 4 };
            int bad = 0;
            memset(&fake, 0, sizeof(fake));
            obj_palanim_start(&fake, 0x0236457c, 0x88);
            slot = fake.spr.palanim;
            last = obj_palanim_anim(slot);
            for (k = 0; k < 120 && slot; k++) {
                obj_palanim_main();
                if (obj_palanim_anim(slot) != last) {
                    last = obj_palanim_anim(slot);
                    if (first == -2)
                        first = k;
                    if (steps < 8 && last != want[steps])
                        bad++;
                    steps++;
                }
            }
            obj_palanim_release(&fake);
            obj_palanim_main();
            if (bad || first != 6 || obj_palanim_anim(slot) != -1)
                steps = 0;
        }
        snprintf(what, sizeof(what), "palette animation (%s)", zone);
        result(what, slot != 0 && steps >= 2, 1, "slot %d: %d palette steps in 120 frames (first after %d), anim %d",
               slot, steps, first, last);
    }
}

/* ============================================================ common */
static void test_run(int nf) {
    const ObjStats *st = objects_stats();
    int n, maxx = 0, i;
    printf("[run] PlayerStart, hold right, jump every 50 frames, %d frames\n", nf);
    for (n = 0; n < nf; n++) {
        u16 k = DCP_KEY_RIGHT;
        if (n % 50 >= 40 && n % 50 < 48)
            k |= DCP_KEY_A;
        frame(k);
        if ((P.x >> 8) > maxx)
            maxx = P.x >> 8;
    }
    printf("  max x %d, rings %d, live objects %u, spawned %u, destroyed %u, gimmicks %u, decorations %u\n  events:", maxx,
           P.rings, st->live, st->spawned, st->destroyed, st->gimmicks, st->dec_spawned);
    for (i = 0; i < NEV && g_ev[i].name[0]; i++)
        printf(" %s=%d", g_ev[i].name, g_ev[i].n);
    printf("\n");
    if (st->spawned == 0 && maxx - g_dcp_world.check_x < 200) /* z52: the route from PlayerStart is not "hold right" */
        printf("  note: the scripted run is blocked %d px from PlayerStart; the targets and the sweep cover the act\n",
               maxx - g_dcp_world.check_x);
    else
        CHECK(st->spawned > 0 && (st->rings_collected > 0 || g_hdr->num_rings == 0),
              "objects spawned and rings collected along the run");
}
static void test_sweep(void) {
    const rlvl_ring_t *rg = (const rlvl_ring_t *)rlvl_section(g_level, RLVL_SEC_RINGS, NULL);
    const ObjStats *st = objects_stats();
    ObjStats s0 = *st;
    u32 i;
    int n, segs = 0, i_ev, ev0[NEV];
    for (i_ev = 0; i_ev < NEV; i_ev++)
        ev0[i_ev] = g_ev[i_ev].n;
    if (g_hdr->num_rings == 0) {
        printf("[sweep] no placed rings in this act: skipped\n");
        return;
    }
    g_quiet = 1;
    for (i = 0; i < g_hdr->num_rings; i += 25) {
        place_player(rg[i].x, rg[i].y - 8);
        P.invuln = 0;
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
    g_quiet = 0;
    printf("[sweep] %d segments x 240 frames: rings %u, springs %u, dash panels %u, enemies %u, hurts %u, gimmicks %u, "
           "water entries %u, water runs %u, zone launches %u, air bubbles %u, mines %u, sand slides %u, cargo %u, rails %u, "
           "rocks %u, rooms %u, decorations %u\n  events:",
           segs, st->rings_collected - s0.rings_collected, st->springs - s0.springs, st->dash_panels - s0.dash_panels,
           st->enemies_destroyed - s0.enemies_destroyed, st->hurts - s0.hurts, st->gimmicks - s0.gimmicks,
           st->water_entries - s0.water_entries, st->water_runs - s0.water_runs, st->zone_launches - s0.zone_launches,
           st->air_bubbles - s0.air_bubbles, st->mines_exploded - s0.mines_exploded, st->sand_slides - s0.sand_slides,
           st->cargo_rides - s0.cargo_rides, st->rails - s0.rails, st->rocks_fallen - s0.rocks_fallen,
           st->enemy_rooms - s0.enemy_rooms, st->dec_spawned - s0.dec_spawned);
    for (i_ev = 0; i_ev < NEV && g_ev[i_ev].name[0]; i_ev++)
        if (g_ev[i_ev].n != ev0[i_ev])
            printf(" %s=%d", g_ev[i_ev].name, g_ev[i_ev].n - ev0[i_ev]);
    printf("\n");
    CHECK(st->rings_collected > s0.rings_collected && st->dec_spawned > s0.dec_spawned, "sweep collects rings, spawns decorations");
    CHECK(st->gimmicks > s0.gimmicks, "the sweep reaches gimmicks by play");
}

/* ============================================================ special stage round trip */
static int placed_ring_index(s32 x, s32 y) {
    u32 i;
    for (i = 0; i < g_hdr->num_rings; i++)
        if (g_obj.lv_ring[i].x == x && g_obj.lv_ring[i].y == y)
            return (int)i;
    return -1;
}
static int is_enemy(u16 t) { return t <= 0x12 && t != 0x04 && t != 0x05; }
/* rings collected standing still at a spot for n frames */
static int rings_at(s32 x, s32 y, int n) {
    s16 r0;
    int k;
    place_player(x, y);
    r0 = P.rings;
    for (k = 0; k < n; k++) {
        teleport(x, y);
        frame(0);
    }
    return P.rings - r0;
}
static void test_event_state(int restore_after_init) {
    const ObjStats *st = objects_stats();
    const rlvl_object_t *o = NULL;
    int ri[64], nr = 0, i, k, gone_idx = -1, live_idx = -1, ok_rings, ok_enemy, ok_ctl, ok_vars, ok_cam, ctl_ring = -1, again;
    u32 e0 = st->enemies_destroyed;
    s32 cx, cy;
    const char *how = restore_after_init ? "restore after objects_init" : "restore before objects_init";
    printf("[special stage round trip: %s]\n", how);
    /* collect rings by play */
    g_n_ring_xy = 0;
    g_rec_rings = 1;
    for (i = 0; nr < 4 && i < (int)g_hdr->num_rings; i += 7) {
        const rlvl_ring_t *rg = &g_obj.lv_ring[i];
        place_player(rg->x - 40, rg->y);
        for (k = 0; k < 40; k++)
            frame(DCP_KEY_RIGHT);
        for (; nr < g_n_ring_xy && nr < 64; nr++)
            ri[nr] = placed_ring_index(g_ring_xy[nr][0], g_ring_xy[nr][1]);
    }
    g_rec_rings = 0;
    /* destroy one enemy (dropped on), remember an untouched one of the same type */
    for (i = 0; gone_idx < 0 && i < (int)g_hdr->num_objects; i++) {
        Obj *w;
        o = &g_obj.lv_obj[i];
        if (!is_enemy(o->type))
            continue;
        place_player(o->x - 0x60, o->y);
        frames(2, 0);
        w = obj_at(o->type, o->x, o->y);
        for (k = 0; w && k < 160 && st->enemies_destroyed == e0 && w->used; k++) {
            if (k % 16 == 0) {
                place_player(w->h.x >> 8, (w->h.y >> 8) - 36);
                dcp_act_jump(&P);
                P.vel_y = 0x200;
            }
            frame(DCP_KEY_A);
        }
        if (st->enemies_destroyed > e0)
            gone_idx = i;
    }
    for (i = 0; gone_idx >= 0 && i < (int)g_hdr->num_objects; i++) {
        const rlvl_object_t *c = &g_obj.lv_obj[i];
        if (i != gone_idx && c->type == g_obj.lv_obj[gone_idx].type &&
            (g_obj.prm[i].x0 != 0xff || obj_at(c->type, c->x, c->y) != NULL) && /* (not destroyed earlier) */
            (DCP_ABS(c->x - g_obj.lv_obj[gone_idx].x) > 600 || DCP_ABS(c->y - g_obj.lv_obj[gone_idx].y) > 400)) {
            live_idx = i;
            break;
        }
    }
    /* a ring never collected, away from the collected ones */
    for (i = (int)g_hdr->num_rings - 1; i >= 0 && ctl_ring < 0; i -= 3) {
        int j, near = 0;
        if (g_obj.ring_state[i] == 0xff)
            continue;
        for (j = 0; j < nr; j++)
            if (ri[j] >= 0 && DCP_ABS(g_obj.lv_ring[ri[j]].x - g_obj.lv_ring[i].x) < 600)
                near = 1;
        if (!near)
            ctl_ring = i;
    }
    g_obj_stage.check_idx = 3; /* data_022c4590 fields the return keeps */
    g_obj_stage.check_x = 1234;
    cx = g_obj_camera[0].x;
    cy = g_obj_camera[0].y;
    for (i = 0, k = 0; i < nr; i++)
        k += ri[i] >= 0;
    printf("  before: %d rings collected (%d of them placed rings), enemy #%d (type 0x%02x) destroyed, live objects %u\n", nr, k,
           gone_idx, gone_idx >= 0 ? g_obj.lv_obj[gone_idx].type : 0, st->live);
    /* main.c: stage_exit(OBJ_EXIT_SPECIAL_STAGE) -> save, shutdown; special stage; reload */
    objects_save_event_state();
    CHECK(st->live == 0, "save destroys the object tasks (Stage_DestroyTasks)");
    objects_shutdown();
    g_obj_stage.check_idx = 0;
    if (!restore_after_init)
        objects_restore_event_state();
    if (objects_init(&M, g_level) != 0) {
        CHECK(0, "objects_init after the round trip");
        return;
    }
    if (restore_after_init)
        objects_restore_event_state();
    ok_vars = g_obj_stage.check_idx == 3 && g_obj_stage.check_x == 1234;
    ok_cam = objects_restart_camera(NULL, NULL, NULL) && g_obj_camera[0].x == (cx & ~0xff) && g_obj_camera[0].y == (cy & ~0xff);
    /* collected rings stay gone, the control ring is still there */
    ok_rings = 1;
    for (i = 0; i < nr; i++)
        if (ri[i] >= 0 && g_obj.ring_state[ri[i]] != 0xff)
            ok_rings = 0;
    again = 0;
    for (i = 0; i < nr && i < 6; i++)
        if (ri[i] >= 0)
            again += rings_at(g_obj.lv_ring[ri[i]].x, g_obj.lv_ring[ri[i]].y, 6);
    ok_ctl = ctl_ring < 0 || rings_at(g_obj.lv_ring[ctl_ring].x, g_obj.lv_ring[ctl_ring].y, 6) > 0;
    if (g_hdr->num_rings == 0)
        printf("  (no placed rings in this act)\n");
    else
    result("rings stay collected", nr > 0 && ok_rings && again == 0 && ok_ctl, 1,
           "%d collected before; list entries still collected %d; picked up again at their spots %d; untouched ring #%d still there %d", nr,
           ok_rings, again, ctl_ring, ok_ctl);
    ok_enemy = 0;
    if (gone_idx >= 0) {
        const rlvl_object_t *g = &g_obj.lv_obj[gone_idx];
        place_player(g->x - 0x60, g->y - 16);
        frames(3, 0);
        ok_enemy = g_obj.prm[gone_idx].x0 == 0xff && obj_at(g->type, g->x, g->y) == NULL;
        if (live_idx >= 0) {
            const rlvl_object_t *c = &g_obj.lv_obj[live_idx];
            place_player(c->x - 0x60, c->y - 16);
            frames(3, 0);
            ok_ctl = obj_at(c->type, c->x, c->y) != NULL;
        } else
            ok_ctl = 1;
    }
    for (i = 0, k = 0; i < (int)g_hdr->num_objects; i++)
        k |= is_enemy(g_obj.lv_obj[i].type);
    if (!k)
        printf("  (no enemies placed in this act)\n");
    else
    result("enemy stays destroyed", gone_idx >= 0 && ok_enemy && ok_ctl, 1,
           "entry #%d not respawned %d; another of its type (#%d) respawns %d", gone_idx, ok_enemy, live_idx, ok_ctl);
    result("restart camera / stage vars", ok_cam && ok_vars, 1, "camera (%d,%d) restored %d; checkpoint index / position kept %d",
           g_obj_camera[0].x >> 8, g_obj_camera[0].y >> 8, ok_cam, ok_vars);
    g_obj_stage.check_idx = 0;
    g_obj_stage.check_x = 0;
}

int main(int argc, char **argv) {
    long size;
    char path[512];
    s16 *trig;
    u8 *phys;
    s32 sx = 128, sy = 768;
    const rlvl_object_t *ps;
    int r, s, nres = 0, nfail = 0;
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
    if (dcp_set_physics(phys, (u32)size) != 2)
        return 2;
    g_level = (u8 *)load(argv[1], &size);
    if (rlvl_bind(&M, g_level) != 0)
        return 2;
    g_hdr = (const rlvl_header_t *)g_level;
    dcp_bind_map(&M);
    memset(&g_dcp_world, 0, sizeof(g_dcp_world));
    g_dcp_world.zone = g_hdr->zone;
    g_dcp_world.act = g_hdr->act;
    g_dcp_world.flags = 1;
    g_dcp_world.objmgr = 0x18;
    g_dcp_world.map_right = M.right;
    g_dcp_world.death_y = M.bottom;
    g_dcp_world.timescale = 0x100;
    g_dcp_world.lives = 99;
    ps = find_obj(0x72, 0);
    if (ps) {
        sx = ps->x;
        sy = ps->y;
    }
    g_dcp_world.check_x = sx;
    g_dcp_world.check_y = sy;
    dcp_player_create(&P, 0, 0, sx, sy);
    g_obj_hooks.log = hk_log;
    g_obj_hooks.effect = hk_effect;
    objects_set_paths(argv[3], argv[4]);
    objects_add_player(&P);
    objects_set_view_count(2);
    camera_follow(1);
    r = objects_init(&M, g_level);
    printf("objects_init: %d; stage %d (zone %d act %d); preloaded sprites:", r, g_hdr->stage, g_hdr->zone, g_hdr->act);
    for (s = 0; s < OBJ_BAC_COUNT; s++) {
        if (g_obj.res[s].loaded)
            printf(" %s", g_obj.data.ids[s]);
        nres += g_obj.res[s].loaded;
        nfail += g_obj.res[s].id[0] == 1;
    }
    printf("\n  %d of %d slots preloaded, %d failed\n", nres, OBJ_BAC_COUNT, nfail);
    CHECK(r == 0, "objects_init");
    CHECK(nfail == 0 && nres > 10, "every preloaded sprite resource loads");
    if (r != 0)
        return 1;
    camera_follow(1);
    test_run(getenv("RUNFRAMES") ? atoi(getenv("RUNFRAMES")) : 1500);
    if (g_hdr->zone == 1) {
        printf("[zone 2 targets]\n");
        test_water();
        test_bubble_spawner();
        test_water_run();
        test_current();
        test_slide80();
        test_launch_zone();
        test_wheel();
        test_mine();
        test_kuragen();
        test_water_jet();
        test_raft();
        test_lancer();
        test_slope7e();
        test_decor2();
    } else if (g_hdr->zone == 2) {
        printf("[zone 3 targets]\n");
        test_rail();
        test_cargo();
        test_hand();
        test_sand_slider();
        test_bishop_hammer();
        test_sand_a9();
        test_room();
        test_table();
        {
            const ObjStats *st = objects_stats();
            result("decorations", st->dec_spawned > 0, 0, "spawned %u, despawned %u", st->dec_spawned, st->dec_despawned);
        }
    } else if (g_hdr->zone >= 3) {
        printf("[zone %d targets]\n", g_hdr->zone + 1);
        /* zone 4 (and z91) */
        test_crane_slide();
        test_shooter();
        test_swing_sky();
        test_switch_neon();
        test_dark_area();
        /* zone 5 (and z92) */
        test_crane();
        test_pipe();
        test_jet_catapult();
        test_piston();
        test_valve();
        /* zone 6 */
        test_glider();
        test_parachute();
        test_saucer();
        test_needle_g();
        test_clouds();
        /* zone 7 */
        test_rocket();
        test_air();
        test_cannon();
        test_swing();
        test_duct();
        test_9c();
        /* enemies */
        test_enemy(0x09, "egg pawn");
        test_enemy(0x0a, "egg pawn");
        test_enemy(0x0b, "flapper");
        test_enemy(0x0c, "flapper");
        test_enemy(0x0d, "egg pawn");
        test_enemy(0x0e, "egg pawn");
        test_enemy(0x0f, "egg pawn");
        test_enemy(0x10, "flapper");
        test_enemy(0x11, "flapper");
        test_enemy(0x12, "flapper");
        test_v4_decor(g_hdr->zone == 3 ? "zone 4" : g_hdr->zone == 4 ? "zone 5" : g_hdr->zone == 5 ? "zone 6" : g_hdr->zone == 6 ? "zone 7" : "zone 9");
    }
    printf("  targets: %d of %d interactions seen; pool live %u of %d; late sprite loads %d\n", g_ok, g_n,
           objects_stats()->live, OBJ_MAX, g_obj.lazy_loads);
    test_sweep();
    test_event_state(0);
    test_event_state(1);
    printf("pool peak %u of %d objects\n", g_peak, OBJ_MAX);
    printf("%d checks, %d failed\n", g_checks, g_fail);
    objects_shutdown();
    return g_fail ? 1 : 0;
}
