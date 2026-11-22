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
static void hk_log(void *ctx, const char *ev, s32 a, s32 b) {
    int i;
    (void)ctx;
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
static void frame(u16 held) {
    u16 pressed = held & ~s_prev, released = s_prev & ~held;
    s_prev = held;
    objects_frame_begin();
    dcp_player_frame(&P, held, pressed, released);
    objects_update(&P);
    camera_follow(0);
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
    place_player(o->x + o->p0 + o->p2 / 2, o->y + o->p1 + o->p3 / 2);
    for (k = 0; k < 10 && P.state != Player_State_ZoneLaunch; k++)
        frame(0);
    result("launch zone 0x83", P.state == Player_State_ZoneLaunch, 1, "flags %#x: launched after %d frames, vel %d,%d",
           o->flags, k, P.vel_x, P.vel_y);
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
        ok = P.vel_y <= -0x780;
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
    for (k = 0; k < 400 && P.state == Player_State_Hand2D; k++)
        frame(0);
    printf("    released after %d more frames\n", k);
}
static void test_table(void) {
    const ObjStats *st = objects_stats();
    const rlvl_object_t *o = find_obj(0x8b, 0);
    Obj *t, *q;
    int k, landed = 0, pawns = 0, boarded = 0;
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
        frame(k % 90 < 6 ? DCP_KEY_A : 0); /* hop over the falling rocks now and then */
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
    CHECK(st->spawned > 0 && st->rings_collected > 0, "objects spawned and rings collected along the run");
}
static void test_sweep(void) {
    const rlvl_ring_t *rg = (const rlvl_ring_t *)rlvl_section(g_level, RLVL_SEC_RINGS, NULL);
    const ObjStats *st = objects_stats();
    ObjStats s0 = *st;
    u32 i;
    int n, segs = 0, i_ev, ev0[NEV];
    for (i_ev = 0; i_ev < NEV; i_ev++)
        ev0[i_ev] = g_ev[i_ev].n;
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
    }
    printf("  targets: %d of %d interactions seen; pool live %u of %d; late sprite loads %d\n", g_ok, g_n,
           objects_stats()->live, OBJ_MAX, g_obj.lazy_loads);
    test_sweep();
    printf("%d checks, %d failed\n", g_checks, g_fail);
    objects_shutdown();
    return g_fail ? 1 : 0;
}
