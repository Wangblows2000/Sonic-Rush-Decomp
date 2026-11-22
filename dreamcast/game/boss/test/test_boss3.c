/*
 * Host test of the native boss module: Boss3 (Mirage Road act 3, stage z33) on
 * the real z33 level with the real player module, the real ObjRect code and
 * the converted bs3 models / animations.
 *
 * Usage: test_boss3 <z33/level.bin> <build/dreamcast/player> <models dir> <boss data dir> [-v|-vv]
 *
 * As in test_boss.c, objects_init is not called: the test registers the
 * player's two boxes itself (son.bac boxes of the standing and ball poses).
 *
 * The arena is a circle: the player's x is the arc position (0x40..0x63e px,
 * wrapped by Boss3_Draw_PlayerOnArena).  The boss only takes damage from its
 * own ball: during a kick (phase 0x10) the rolling ball alternates harmful /
 * hittable; a jumping player that touches it while hittable reflects it
 * (Boss3PartCD_OnHit), and the reflected ball hits the body (Boss3PartCC_OnHit).
 *
 * Scripted player:
 *   [miss]   900 frames standing still at the start: the boss keeps its health.
 *   [fight]  kick: when the rolling ball is hittable and stays hittable for at
 *            least 20 more frames, the player is put on the floor in front of it
 *            (logged as "intercept") and jumps; after a reflection the boss
 *            drives the player (input lock, Boss3_Draw_PlayerOnArena).
 *            push: the player jumps on the ball when it is hittable (stagger) and
 *            runs ahead of it while it is harmful.  Throw attack: keeps running.
 *   [defeat] runs the defeat sequence to the stage-clear request.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../boss.h"
#include "../../objects/objects.h"

static int g_fail, g_checks, g_verbose;
#define CHECK(c, ...)                                                   \
    do {                                                                \
        g_checks++;                                                     \
        if (!(c)) {                                                     \
            g_fail++;                                                   \
            printf("  FAIL %s:%d: ", __FILE__, __LINE__);               \
            printf(__VA_ARGS__);                                        \
            printf("\n");                                               \
        } else {                                                        \
            printf("  ok   ");                                          \
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
static int g_frame;
static int g_hurts, g_navi, g_cleared_hook, g_music_stop, g_ring_flags, g_pinch_music, g_ball_white_seen;
static int g_sfx[256], g_sfx_handle_play[4], g_sfx_handle_stop[4];

void rect_check_all(void);
void rect_check_init(void);
void rect_set_box3d(ObjBox *b, s16 l, s16 t, s16 r, s16 bb, s16 zn, s16 zf);
void rect_set_atkdef(ObjBox *b, u32 atk, u32 def);
void obj_handle_collider(DcPlayer *h, void *owner, int kind, ObjBox *b, int has_sprite);
void obj_player_on_hurt(ObjBox *body, ObjBox *attacker);
int boss3_ball_white(void);
static ObjBox s_body, s_atk;
static s16 s_prev_invuln;

static void hk_atkdef(void *ctx, DcPlayer *p, s32 box, s32 atk, s32 def) {
    (void)ctx;
    (void)p;
    rect_set_atkdef(box == 0 ? &s_body : &s_atk, (u32)atk, (u32)def);
}
static void player_boxes_init(void) {
    memset(&s_body, 0, sizeof(s_body));
    memset(&s_atk, 0, sizeof(s_atk));
    s_body.cb = obj_player_on_hurt;
    s_body.mask = 0x21;
    s_body.flags |= 0x20;
    rect_set_atkdef(&s_body, 0xa, 2);
    s_atk.mask = 0x21;
    s_atk.flags |= 0x20;
    rect_set_atkdef(&s_atk, 0, 0);
    rect_set_box3d(&s_body, -7, -12, 7, 13, -4, 4);
    rect_set_box3d(&s_atk, -12, -12, 12, 12, -4, 4);
    s_body.owner = s_atk.owner = &P;
    s_body.owner_kind = s_atk.owner_kind = OBJ_OWNER_PLAYER;
}
static void player_collide(void) {
    if (P.invuln != 0)
        s_body.def |= 5;
    else if (s_prev_invuln != 0)
        s_body.def = 2;
    s_prev_invuln = P.invuln;
    if (P.move_flags & DCP_MF_AIR) {
        rect_set_box3d(&s_body, -8, -8, 8, 8, -4, 4);
        rect_set_box3d(&s_atk, -8, -9, 8, 9, -4, 4);
    } else {
        rect_set_box3d(&s_body, -6, -16, 6, 16, -4, 4);
        rect_set_box3d(&s_atk, -6, -16, 6, 16, -4, 4);
    }
    obj_handle_collider(&P, &P, OBJ_OWNER_PLAYER, &s_body, 1);
    obj_handle_collider(&P, &P, OBJ_OWNER_PLAYER, &s_atk, 1);
}

/* ------------------------------------------------------------ hooks */
static void hk_obj_log(void *ctx, const char *ev, s32 a, s32 b) {
    (void)ctx;
    if (!strcmp(ev, "hurt")) {
        g_hurts++;
        if (g_verbose)
            printf("    f%5d player hurt at x=%d (rings %d)  boss %s / %s phase %d\n", g_frame, a, b,
                   boss_state_name(1), boss_state_name(2), boss_phase());
    }
}
static void hk_boss_log(void *ctx, const char *ev, s32 a, s32 b) {
    (void)ctx;
    if (!strcmp(ev, "effect") && g_verbose < 2)
        return;
    if (!g_verbose && strncmp(ev, "attack", 6) && strcmp(ev, "hit") && strcmp(ev, "defeat") && strcmp(ev, "cleared") &&
        strcmp(ev, "start") && strcmp(ev, "reflect") && strcmp(ev, "summon"))
        return;
    printf("    f%5d boss %-16s %6d %6d   player (%d,%d)  body arc %d\n", g_frame, ev, a, b, P.x >> 8, P.y >> 8,
           boss3_arc_x() >> 8);
}
static void hk_sfx(void *ctx, s32 id) {
    (void)ctx;
    if (id >= 0 && id < 256)
        g_sfx[id]++;
}
static void hk_sfx_handle(void *ctx, s32 h, s32 id) {
    hk_sfx(ctx, id);
    if (h >= 0 && h < 4)
        g_sfx_handle_play[h]++;
}
static void hk_sfx_stop(void *ctx, s32 h) {
    (void)ctx;
    if (h >= 0 && h < 4)
        g_sfx_handle_stop[h]++;
}
static void hk_navi(void *ctx, s32 face, s32 msg, s32 t) { (void)ctx; (void)face; (void)msg; (void)t; g_navi++; }
static void hk_cleared(void *ctx) { (void)ctx; g_cleared_hook++; }
static void hk_music_stop(void *ctx) { (void)ctx; g_music_stop++; }
static void hk_ring_flags(void *ctx, u32 f) { (void)ctx; g_ring_flags |= (int)f; }
static void hk_music(void *ctx, s32 s) { (void)ctx; if (s == 0xf) g_pinch_music++; }

extern unsigned char *g_boss_debug_fb;
/* host debug view: projected boss vertices + the player (arena transform) as a white cross */
static void dump_view(const char *path) {
    static unsigned char fb[640 * 480 * 3];
    dcm_camera_t cam;
    float v[3], o[4], e[3];
    s32 tr[3], rot[9];
    int i, c, x, y;
    FILE *f;
    memset(fb, 0, sizeof(fb));
    g_boss_debug_fb = fb;
    boss_draw(0);
    g_boss_debug_fb = NULL;
    boss_camera(&cam);
    if (boss_player_model(tr, rot)) {
        v[0] = tr[0] / 4096.0f; v[1] = tr[1] / 4096.0f; v[2] = tr[2] / 4096.0f;
    } else {
        v[0] = P.x / 256.0f; v[1] = -P.y / 256.0f; v[2] = P.z / 256.0f;
    }
    for (c = 0; c < 3; c++)
        e[c] = v[0] * cam.view.m[0][c] + v[1] * cam.view.m[1][c] + v[2] * cam.view.m[2][c] + cam.view.m[3][c];
    for (c = 0; c < 4; c++)
        o[c] = e[0] * cam.proj[0][c] + e[1] * cam.proj[1][c] + e[2] * cam.proj[2][c] + cam.proj[3][c];
    x = (int)((o[0] / o[3] * 0.5f + 0.5f) * 640);
    y = (int)((0.5f - o[1] / o[3] * 0.5f) * 480);
    for (i = -6; i <= 6; i++) {
        if (x + i >= 0 && x + i < 640 && y >= 0 && y < 480) memset(fb + 3 * (y * 640 + x + i), 255, 3);
        if (y + i >= 0 && y + i < 480 && x >= 0 && x < 640) memset(fb + 3 * ((y + i) * 640 + x), 255, 3);
    }
    f = fopen(path, "wb");
    if (!f) return;
    fprintf(f, "P6 640 480 255\n");
    fwrite(fb, 1, sizeof(fb), f);
    fclose(f);
    printf("  view dumped to %s (player at screen %d,%d)\n", path, x, y);
}

/* ZoneFx_Zone3Boss_Update stand-in (the game links zonefx.c): the same counter, two known
 * colour sets, to check the boss module applies them to its lights */
static u16 s_z3b;
static int g_light_calls, g_light_changes, g_zs3_seen, g_light_seen;
int zonefx_boss3_lights(int boss_active, uint16_t out[3]) {
    static const uint16_t a[3] = { 0x73ff, 0x00c9, 0x1e19 }, b[3] = { 0x73ff, 0x00c9, 0x0d79 };
    const uint16_t *p;
    g_light_calls++;
    if (!boss_active)
        return 0;
    s_z3b++;
    if ((s_z3b & ~0x8000) <= 6)
        return 0;
    p = (s_z3b & 0x8000) ? a : b;
    out[0] = p[0];
    out[1] = p[1];
    out[2] = p[2];
    s_z3b &= 0x8000;
    s_z3b ^= 0x8000;
    g_light_changes++;
    return 1;
}
int boss3_zone_script_state(void);
int boss3_light_state(uint16_t out[3]);

static u16 s_prev;
static void frame(u16 held) {
    u16 pressed = held & ~s_prev, released = s_prev & ~held;
    s_prev = held;
    rect_check_all();
    dcp_player_frame(&P, held, pressed, released);
    player_collide();
    boss_update(&P);
    {
        uint16_t c[3];
        if (boss3_zone_script_state() > 0x1e)
            g_zs3_seen++;
        if (boss3_light_state(c) && (c[2] == 0x1e19 || c[2] == 0x0d79))
            g_light_seen++;
    }
    g_frame++;
}

static s32 floor_y(s32 x) {
    s32 yy;
    for (yy = 200; yy < M.bottom; yy++)
        if (rlvl_tile_height(&M, yy, x, 1 | 0x80, NULL, NULL) != 0)
            return yy;
    return M.bottom;
}

#define CIRC 0x5fe /* arena circumference (px) */
static s32 wrap_arc(s32 x) {
    while (x < 0x40)
        x += CIRC;
    while (x > 0x63e)
        x -= CIRC;
    return x;
}

static void place_on_floor(s32 x) {
    s16 rings = P.rings;
    u16 mu = P.map_up, md = P.map_down, mr = P.map_right, ml = P.map_left;
    u16 disp = P.disp_flags & 0x2040;
    x = wrap_arc(x);
    memset(&P, 0, sizeof(P));
    dcp_player_create(&P, 0, 0, x, floor_y(x) - 14);
    P.rings = rings;
    P.pf &= ~0x600000u;
    P.map_up = mu;
    P.map_down = md;
    P.map_right = mr;
    P.map_left = ml;
    P.disp_flags |= disp;
}

static const rlvl_object_t *find_obj(u16 type) {
    const rlvl_header_t *h = (const rlvl_header_t *)g_level;
    const rlvl_object_t *o = (const rlvl_object_t *)rlvl_section(g_level, RLVL_SEC_OBJECTS, NULL);
    u32 i;
    for (i = 0; i < h->num_objects; i++)
        if (o[i].type == type)
            return &o[i];
    return NULL;
}

/* signed distance on the circle from a to b, -CIRC/2..CIRC/2 */
static s32 arc_delta(s32 a, s32 b) {
    s32 d = (b - a) % CIRC;
    if (d > CIRC / 2)
        d -= CIRC;
    if (d < -CIRC / 2)
        d += CIRC;
    return d;
}

int main(int argc, char **argv) {
    long size;
    char path[512];
    s16 *trig;
    u8 *phys;
    const rlvl_object_t *ps;
    s32 sx = 224, sy = 400;
    int r, n, last_phase = -1, last_health = -1, phases_seen[0x20] = {0};
    char last_state[64] = "", last_cstate[64] = "";
    int hits_in_miss = 0, hurts_in_miss = 0, cooldown = 0, intercepts = 0, max_balls = 0;
    int max_frames, start_frame, tris;
    float ball_r_min = 1e9f, ball_r_max = 0;
    if (argc < 5) {
        printf("usage: %s <level.bin> <player dir> <models dir> <boss dir> [-v|-vv]\n", argv[0]);
        return 2;
    }
    if (argc > 5)
        g_verbose = strcmp(argv[5], "-vv") ? 1 : 2;
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
    dcp_bind_map(&M);
    memset(&g_dcp_world, 0, sizeof(g_dcp_world));
    g_dcp_world.zone = 2;
    g_dcp_world.act = 2;
    g_dcp_world.flags = 1;
    g_dcp_world.objmgr = 0x18;
    g_dcp_world.map_right = M.right;
    g_dcp_world.death_y = M.bottom;
    g_dcp_world.timescale = 0x100;
    g_dcp_world.lives = 3;
    g_dcp_world.rand_seed = getenv("SEED") ? (u32)strtoul(getenv("SEED"), NULL, 0) : 0x12345678;
    ps = find_obj(0x72);
    if (ps) {
        sx = ps->x;
        sy = ps->y;
    }
    g_dcp_world.check_x = sx;
    g_dcp_world.check_y = sy;
    printf("z33: %dx%d px, stage %d, PlayerStart (%d,%d), boss object 0x3d %s, floor y at x=64/%d/1590: %d/%d/%d\n",
           M.right, M.bottom, ((const rlvl_header_t *)g_level)->stage, sx, sy, find_obj(0x3d) ? "found" : "missing", sx,
           floor_y(64), floor_y(sx), floor_y(1590));
    dcp_player_create(&P, 0, 0, sx, sy);
    P.pf &= ~0x600000u;
    g_obj_hooks.log = hk_obj_log;
    g_dcp_hooks.atkdef = hk_atkdef;
    rect_check_init();
    player_boxes_init();
    g_boss_hooks.log = hk_boss_log;
    g_boss_hooks.sfx = hk_sfx;
    g_boss_hooks.voice = hk_sfx;
    g_boss_hooks.sfx_handle = hk_sfx_handle;
    g_boss_hooks.sfx_stop = hk_sfx_stop;
    g_boss_hooks.navi = hk_navi;
    g_boss_hooks.cleared = hk_cleared;
    g_boss_hooks.music_stop = hk_music_stop;
    g_boss_hooks.ring_flags = hk_ring_flags;
    g_boss_hooks.music = hk_music;
    boss_set_paths(argv[3], argv[4]);
    boss_set_difficulty(getenv("LEVEL") ? atoi(getenv("LEVEL")) : 0);
    r = boss_init(((const rlvl_header_t *)g_level)->stage, g_level, &P, 0);
    printf("boss_init: %d  health %d/%d phase %d\n", r, boss_health(), boss_health_max(), boss_phase());
    CHECK(r == 0, "boss_init on z33 (5 bs3 models, bs3.dca, bs3.nsbta timing, effects, bossdata.bin)");
    if (r != 0)
        return 1;
    {
        int hp = getenv("LEVEL") && atoi(getenv("LEVEL")) == 1 ? 8 : 6;
        CHECK(boss_health() == hp && boss_health_max() == hp, "Boss3_Create: %d hit points", hp);
    }
    for (n = 0; n < 60; n++)
        frame(0);
    CHECK(!strcmp(boss_state_name(0), "State_WaitForStart") && !strcmp(boss_state_name(1), "State_Idle"),
          "boss waits for the title card (%s / %s)", boss_state_name(0), boss_state_name(1));
    boss_start();
    start_frame = g_frame;
    max_frames = getenv("BOSSFRAMES") ? atoi(getenv("BOSSFRAMES")) : 40000;
    while (g_frame < max_frames && !boss_cleared()) {
        u16 keys = 0;
        int ph = boss_phase();
        Boss3BallInfo b;
        int have = boss3_ball(&b);
        s32 px = P.x >> 8;
        if (ph != last_phase || boss_health() != last_health) {
            s32 bx, by, bz;
            boss_hammer_pos(&bx, &by, &bz);
            printf("  f%5d phase %2d health %d  %s / %s / ball %s  body arc %d (%d,%d,%d)  player x %d\n", g_frame, ph,
                   boss_health(), boss_state_name(0), boss_state_name(1), boss_state_name(2), boss3_arc_x() >> 8,
                   bx >> 8, by >> 8, bz >> 8, px);
            last_phase = ph;
            last_health = boss_health();
            if (ph >= 0 && ph < 0x20)
                phases_seen[ph]++;
        }
        if (g_verbose >= 2 && (strcmp(last_state, boss_state_name(1)) || strcmp(last_cstate, boss_state_name(2)))) {
            printf("      f%5d -> %s / ball %s  ball x %d v %d flags %x  player x %d\n", g_frame, boss_state_name(1),
                   boss_state_name(2), b.x >> 8, b.vel_x, b.flags, px);
            snprintf(last_state, sizeof(last_state), "%s", boss_state_name(1));
            snprintf(last_cstate, sizeof(last_cstate), "%s", boss_state_name(2));
        }
        if (boss3_balls_alive() > max_balls)
            max_balls = boss3_balls_alive();
        if (boss3_ball_white())
            g_ball_white_seen = 1;
        P.rings = 50;
        g_dcp_world.lives = 3;
        if (cooldown > 0)
            cooldown--;
        if (g_frame - start_frame < 900) {
            if (g_frame - start_frame == 0)
                place_on_floor(sx);
        } else if (!boss_defeated() && !(P.pf & 0x200000)) {
            if (ph == BOSS3_PHASE_KICK && have && (b.flags & 1) && !(b.flags & 0x42) && b.vel_x != 0) {
                s32 dir = b.vel_x > 0 ? 1 : -1, spd = abs(b.vel_x) >> 8;
                s32 ahead = arc_delta(b.x >> 8, px) * dir; /* > 0: the ball rolls towards the player */
                int left = b.cycle_period - b.cycle_count;
                if (cooldown == 0 && b.hittable && left >= 24 && (P.move_flags & DCP_MF_GROUND)) {
                    /* intercept: stand in front of the ball and jump so that the jump meets it */
                    place_on_floor((b.x >> 8) + dir * (spd * 13 + 44));
                    cooldown = 60;
                    intercepts++;
                    if (g_verbose)
                        printf("      f%5d intercept: ball x %d v %d (hittable %d more frames), player at %d\n",
                               g_frame, b.x >> 8, b.vel_x, left, P.x >> 8);
                } else if (cooldown == 0 && b.harmful && ahead > 0 && ahead < 140) {
                    keys |= dir > 0 ? DCP_KEY_RIGHT : DCP_KEY_LEFT; /* run ahead of the harmful ball */
                    if (ahead < 70 && (P.move_flags & DCP_MF_GROUND))
                        keys |= DCP_KEY_A; /* or jump over it */
                }
                if (cooldown >= 44 && cooldown <= 56)
                    keys |= DCP_KEY_A; /* jump 4 frames after the placement (the player is on the floor) */
            } else if (ph == BOSS3_PHASE_PUSH && have) {
                s32 d = arc_delta(px, b.x >> 8);
                if (b.hittable && abs(d) < 90 && cooldown == 0 && (P.move_flags & DCP_MF_GROUND)) {
                    cooldown = 40; /* jump onto the hittable ball: the body staggers */
                    intercepts++;
                }
                if (cooldown > 20) {
                    keys |= DCP_KEY_A;
                    keys |= d > 0 ? DCP_KEY_RIGHT : DCP_KEY_LEFT;
                } else if (b.harmful && abs(d) < 160)
                    keys |= d > 0 ? DCP_KEY_LEFT : DCP_KEY_RIGHT; /* keep away from the harmful ball */
            }
        }
        frame(keys);
        {
            s32 bx, by, bz;
            boss_hammer_pos(&bx, &by, &bz);
            if (ph == BOSS3_PHASE_PUSH || ph == BOSS3_PHASE_KICK) {
                float rr = sqrtf((float)bx * bx + (float)bz * bz) / 256.0f;
                if (rr < ball_r_min) ball_r_min = rr;
                if (rr > ball_r_max) ball_r_max = rr;
            }
        }
        if (getenv("DUMPAT") && g_frame == atoi(getenv("DUMPAT")))
            dump_view(getenv("DUMPFILE") ? getenv("DUMPFILE") : "/tmp/boss3/view.ppm");
        if (g_frame - start_frame == 900) {
            hits_in_miss = (int)boss_stats()->hits;
            hurts_in_miss = g_hurts;
            printf("  [miss] 900 frames standing still: %d hurts, %d hits, health %d\n", hurts_in_miss, hits_in_miss,
                   boss_health());
        }
    }
    {
        const BossStats *st = boss_stats();
        float eye[3], at[3];
        s32 tr[3], rot[9];
        printf("  end f%d: phase %d health %d cleared %d; state %s / %s\n", g_frame, boss_phase(), boss_health(),
               boss_cleared(), boss_state_name(0), boss_state_name(1));
        printf("  stats: attacks %u (push %u, kick %u, throw %u), summons %u, hits %u, reflects %u, staggers %u, "
               "catches %u, jumps over the ball %u\n",
               st->attacks, st->pushes, st->kicks, st->throw_attacks, st->summons, st->hits, st->reflects, st->staggers,
               st->catches, st->jumps);
        printf("  balls: spawned %u thrown %u exploded %u (max alive %d); intercepts %d; player hurts %d\n",
               st->balls_spawned, st->balls_thrown, st->ball_explosions, max_balls, intercepts, g_hurts);
        printf("  effects %u; navi %d; sfx 0x93 %d 0x8e %d 0x97 %d 0xcb %d 0xe6 %d; handle 1 play/stop %d/%d; "
               "ring_bonus %d\n",
               st->effects_spawned, g_navi, g_sfx[0x93], g_sfx[0x8e], g_sfx[0x97], g_sfx[0xcb], g_sfx[0xe6],
               g_sfx_handle_play[1], g_sfx_handle_stop[1], g_obj_stage.ring_bonus);
        printf("  body radius while pushing / kicking %.1f..%.1f px\n", ball_r_min, ball_r_max);
        boss_camera_eye(eye, at);
        printf("  camera eye (%.1f,%.1f,%.1f) at (%.1f,%.1f,%.1f)\n", eye[0], eye[1], eye[2], at[0], at[1], at[2]);
        CHECK(hits_in_miss == 0, "[miss] standing still never damages the boss (%d hurts)", hurts_in_miss);
        CHECK(st->pushes > 0 && st->kicks > 0 && st->throw_attacks > 0 && st->summons > 0,
              "push / kick / throw attack / summon all chosen (%u, %u, %u, %u)", st->pushes, st->kicks,
              st->throw_attacks, st->summons);
        CHECK(st->balls_thrown > 0 && st->ball_explosions > 0 && max_balls >= 2,
              "thrown balls fly and explode at the arena edge (%u thrown, %u exploded, %d alive at once)",
              st->balls_thrown, st->ball_explosions, max_balls);
        CHECK(st->reflects >= st->hits && (int)st->hits == boss_health_max(),
              "%d hits, each from a ball the player reflected (%u reflects)", boss_health_max(), st->reflects);
        CHECK(phases_seen[0xd] && phases_seen[0xe] && phases_seen[0xf] && phases_seen[0x10] && phases_seen[0x11] &&
                  phases_seen[0x12] && phases_seen[0x13],
              "phases idle/push/summon/kick/throw/hurt/defeat all visited");
        CHECK(g_pinch_music == 1, "pinch music (Sound_PlaySeq 0xf) at health 2");
        CHECK(boss_defeated() && boss_cleared() && g_cleared_hook == 1, "defeat sequence reached Stage_OnBossCleared");
        CHECK(g_music_stop == 1 && (g_ring_flags & 1), "defeat stops the music and sets the ring manager flag");
        CHECK(g_obj_stage.ring_bonus == P.rings * 1000, "ring bonus = rings x 1000 (%d)", g_obj_stage.ring_bonus);
        CHECK(st->effects_spawned > 0 && g_sfx[0xcb] > 0, "hit / clash / pop / land effects spawned, clash sound");
        CHECK(g_ball_white_seen && g_sfx_handle_play[1] > 0 && g_sfx_handle_stop[1] > 0,
              "hittable ball blinks (material flag), body sound handle played and stopped");
        CHECK(fabsf(ball_r_min - 244.2f) < 1.0f && fabsf(ball_r_max - 244.2f) < 1.0f,
              "body on the arena circle while pushing / kicking (radius %.1f..%.1f px)", ball_r_min, ball_r_max);
        CHECK(boss_player_model(tr, rot) && fabsf(sqrtf((float)tr[0] * tr[0] + (float)tr[2] * tr[2]) / 4096.0f - 244.2f) < 1.0f,
              "player model on the arena circle (radius %.1f px)", sqrtf((float)tr[0] * tr[0] + (float)tr[2] * tr[2]) / 4096.0f);
        CHECK(isfinite(eye[0]) && isfinite(eye[2]) && fabsf(eye[0] - at[0]) + fabsf(eye[2] - at[2]) > 10.0f,
              "boss camera produces a finite eye/at");
        tris = boss_draw(0);
        printf("  boss_draw (host: vertices transformed) %d\n", tris);
        CHECK(tris > 0, "boss models pose and project through the boss camera");
        printf("  StageZoneScript_Zone3Boss: ball camera set in %d frames; ZoneFx_Zone3Boss lights: %d calls, "
               "%d colour changes, set in %d frames\n", g_zs3_seen, g_light_calls, g_light_changes, g_light_seen);
        CHECK(g_zs3_seen > 0, "StageZoneScript_Zone3Boss: the camera follows the ball 30 frames after Boss3 unk210 = 1");
        CHECK(g_light_changes > 10 && g_light_seen > 0, "ZoneFx_Zone3Boss_Update: the light colours alternate in the fight");
    }
    printf("%d checks, %d failed\n", g_checks, g_fail);
    boss_shutdown();
    return g_fail ? 1 : 0;
}
