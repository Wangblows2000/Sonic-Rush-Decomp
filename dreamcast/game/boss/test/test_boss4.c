/*
 * Host test of the native boss module: Boss4 (Night Carnival act 3, stage z43)
 * on the real z43 level with the real player module, the real ObjRect code and
 * the converted bs4 / eff_ele_* models and animations.
 *
 * Usage: test_boss4 <z43/level.bin> <build/dreamcast/player> <models dir> <boss data dir> [-v|-vv]
 *
 * As in test_boss.c, objects_init is not called: the test registers the
 * player's two boxes itself (son.bac boxes of the standing and ball poses).
 *
 * The arena is a circle (the player's x is the arc position, 0x40..0x789 px,
 * wrapped by Boss4_Draw_PlayerOnArena).  The boss is a spinning seesaw: the
 * body (damage box) rides high, the arm low.  A jumping player that hits the
 * arm while it is not electrified makes it clash (Boss4_OnHit_Damage), and
 * the body drops an enemy (Boss4PartD1); bouncing on the enemy
 * (Player_Action_AttackBounceVel -0x900) takes the player up to the body.
 *
 * Scripted player:
 *   [miss]   900 frames standing still at the start: the boss keeps its health.
 *   [fight]  arm: while the arm is not electrified, the player is put on the
 *            floor in front of it and jumps (clash).  enemy: when the body is
 *            30..60 frames away from the enemy, the player jumps and is put
 *            just above the enemy (logged as "assist"), bounces, and steers
 *            under the body box.
 *   [watch]  at 5 hit points or less the body is left alone until the boss
 *            has slammed once (slams are only chosen at 5 hit points or less).
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
static int g_hurts, g_navi, g_cleared_hook, g_music_stop, g_ring_flags, g_pinch_music, g_box_flags_calls;
static int g_sfx[256], g_sfx_handle_play[4], g_sfx_handle_stop[4];

void rect_check_all(void);
void rect_check_init(void);
void rect_set_box3d(ObjBox *b, s16 l, s16 t, s16 r, s16 bb, s16 zn, s16 zf);
void rect_set_atkdef(ObjBox *b, u32 atk, u32 def);
void obj_handle_collider(DcPlayer *h, void *owner, int kind, ObjBox *b, int has_sprite);
void obj_player_on_hurt(ObjBox *body, ObjBox *attacker);


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
    printf("    f%5d boss %-16s %6d %6d   player (%d,%d)\n", g_frame, ev, a, b, P.x >> 8, P.y >> 8);
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
static void hk_box_flags(void *ctx, DcPlayer *p, s32 box, u16 clear, u16 set) {
    ObjBox *b = box == 0 ? &s_body : &s_atk;
    (void)ctx;
    (void)p;
    b->flags = (u16)((b->flags & ~clear) | set);
    g_box_flags_calls++;
}

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

static u16 s_prev;
static void frame(u16 held) {
    u16 pressed = held & ~s_prev, released = s_prev & ~held;
    s_prev = held;
    rect_check_all();
    dcp_player_frame(&P, held, pressed, released);
    player_collide();
    boss_update(&P);
    g_frame++;
}

static s32 floor_y(s32 x) {
    s32 yy;
    for (yy = 200; yy < M.bottom; yy++)
        if (rlvl_tile_height(&M, yy, x, 1 | 0x80, NULL, NULL) != 0)
            return yy;
    return M.bottom;
}

#define CIRC 0x749 /* arena circumference (px) */
static s32 wrap_arc(s32 x) {
    while (x < 0x40)
        x += CIRC;
    while (x > 0x789)
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
    int hits_in_miss = 0, hurts_in_miss = 0, cooldown = 0, mode = 0, bounced = 0, bounces_seen = 0, assists = 0;
    int arm_tries = 0, body_tries = 0, max_shots = 0, max_waves = 0, body_low = 0;
    s32 prev_body = 0, prev_arm = 0;
    int max_frames, start_frame, tris;
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
    g_dcp_world.zone = 3;
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
    printf("z43: %dx%d px, stage %d, PlayerStart (%d,%d), boss object 0x3c %s, floor y at x=64/%d/1850: %d/%d/%d\n",
           M.right, M.bottom, ((const rlvl_header_t *)g_level)->stage, sx, sy, find_obj(0x3c) ? "found" : "missing", sx,
           floor_y(64), floor_y(sx), floor_y(1850));
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
    g_boss_hooks.player_box_flags = hk_box_flags;
    boss_set_paths(argv[3], argv[4]);
    boss_set_difficulty(getenv("LEVEL") ? atoi(getenv("LEVEL")) : 0);
    r = boss_init(((const rlvl_header_t *)g_level)->stage, g_level, &P, 0);
    printf("boss_init: %d  health %d/%d phase %d\n", r, boss_health(), boss_health_max(), boss_phase());
    CHECK(r == 0, "boss_init on z43 (7 bs4 models, bs4.dca, eff_ele_* packages, bs4/eff nsbma+nsbtp timing, effects)");
    if (r != 0)
        return 1;
    {
        int hp = getenv("LEVEL") && atoi(getenv("LEVEL")) == 1 ? 8 : 6;
        CHECK(boss_health() == hp && boss_health_max() == hp, "Boss4_Create: %d hit points", hp);
    }
    for (n = 0; n < 60; n++)
        frame(0);
    CHECK(!strcmp(boss_state_name(0), "State_WaitForStart") && !strcmp(boss_state_name(1), "-"),
          "boss waits for the title card (%s / %s)", boss_state_name(0), boss_state_name(1));
    boss_start();
    start_frame = g_frame;
    max_frames = getenv("BOSSFRAMES") ? atoi(getenv("BOSSFRAMES")) : 40000;
    while (g_frame < max_frames && !boss_cleared()) {
        Boss4Info bi;
        int ph = boss_phase();
        u16 keys = 0;
        s32 px = P.x >> 8, bx, ax, vb, va;
        boss4_info(&bi);
        bx = bi.body_x >> 8;
        ax = bi.arm_x >> 8;
        vb = arc_delta(prev_body, bx);
        va = arc_delta(prev_arm, ax);
        prev_body = bx;
        prev_arm = ax;
        if (ph != last_phase || boss_health() != last_health) {
            printf("  f%5d phase %2d health %d  %s / %s  body (%d,%d) arm (%d,%d) tilt %d elec %d  player x %d\n", g_frame, ph,
                   boss_health(), boss_state_name(0), boss_state_name(1), bx, bi.body_y >> 8, ax, bi.arm_y >> 8, bi.tilt,
                   bi.electrified, px);
            last_phase = ph;
            last_health = boss_health();
            if (ph >= 0 && ph < 0x20)
                phases_seen[ph]++;
        }
        if (bi.shots > max_shots) max_shots = bi.shots;
        if (bi.waves > max_waves) max_waves = bi.waves;
        if (bi.body_box_valid && (bi.body_box.y + bi.body_box.b) > body_low) body_low = bi.body_box.y + bi.body_box.b;
        P.rings = 50;
        g_dcp_world.lives = 3;
        if (cooldown > 0)
            cooldown--;
        if (g_frame - start_frame < 900) {
            if (g_frame - start_frame == 0)
                place_on_floor(sx);
        } else if (!boss_defeated()) {
            int busy = 0;
            /* [watch] at 5 hit points or less the player leaves the body alone until a slam was seen
             * (slams are only chosen at 5 hit points or less), so every attack kind is covered */
            int hold = boss_health() <= 5 && boss_stats()->slams == 0 && g_frame - start_frame < 20000;
            /* C: the body hangs low (clash / hurt tilt): jump at it like at the arm */
            if (!hold && mode == 0 && cooldown == 0 && bi.body_box_valid && bi.body_box.y + bi.body_box.b >= 432 &&
                (P.move_flags & DCP_MF_GROUND)) {
                place_on_floor(bx + 20 + vb * 21);
                cooldown = 70;
                body_tries++;
                mode = 4;
            }
            /* B: an enemy is on the floor: bounce on it while the body comes round */
            if (!hold && bi.enemy && (bi.enemy_y >> 8) >= 512 && !bounced && mode == 0 && cooldown == 0 && vb != 0) {
                s32 d = arc_delta(bx, bi.enemy_x >> 8) * (vb > 0 ? 1 : -1); /* > 0: body comes towards the enemy */
                int go = abs(vb) >= 3 ? (d > 30 * abs(vb) && d < 60 * abs(vb)) : abs(arc_delta(bx + 20, bi.enemy_x >> 8)) < 40;
                if (go && (P.move_flags & DCP_MF_GROUND)) {
                    place_on_floor((bi.enemy_x >> 8) + 300);
                    mode = 1;
                    cooldown = 120;
                    if (g_verbose)
                        printf("      f%5d enemy at %d, body %d (v %d): jump\n", g_frame, bi.enemy_x >> 8, bx, vb);
                }
            }
            if (mode == 1) {
                busy = 1;
                if (cooldown >= 100 && cooldown <= 118)
                    keys |= DCP_KEY_A;
                if (P.anim == 0x30 && !(P.move_flags & DCP_MF_GROUND) && bi.enemy) {
                    /* assist: put the jumping player just above the enemy */
                    P.x = bi.enemy_x;
                    P.y = bi.enemy_y - (40 << 8);
                    P.vel_x = 0;
                    P.vel_y = 0x100;
                    P.gnd_vel = 0;
                    assists++;
                    mode = 2;
                    if (g_verbose)
                        printf("      f%5d assist: player put above the enemy (%d,%d)\n", g_frame, bi.enemy_x >> 8,
                               (bi.enemy_y >> 8) - 40);
                } else if (!bi.enemy || cooldown == 0)
                    mode = 0;
            } else if (mode == 2) {
                busy = 1;
                if (boss_stats()->enemy_bounces > (u32)bounces_seen) {
                    bounces_seen = (int)boss_stats()->enemy_bounces;
                    mode = 3;
                }
                if (P.move_flags & DCP_MF_GROUND)
                    mode = 0;
            } else if (mode == 3) {
                /* after the bounce: steer under the body box */
                s32 cx = bx + 20, d = arc_delta(px, cx);
                busy = 1;
                if (abs(d) > 4)
                    keys |= d > 0 ? DCP_KEY_RIGHT : DCP_KEY_LEFT;
                if (P.move_flags & DCP_MF_GROUND) {
                    mode = 0;
                    cooldown = 30;
                }
            }
            /* A: hit the arm (not electrified): clash, an enemy drops */
            if (mode == 4)
                busy = 1;
            if (!busy && !bi.electrified && !bi.enemy && cooldown == 0 && (P.move_flags & DCP_MF_GROUND) &&
                (ph == BOSS4_PHASE_SPIN || ph == BOSS4_PHASE_TILTSPIN) && va != 0) {
                place_on_floor(ax + va * 21);
                cooldown = 70;
                arm_tries++;
                mode = 4;
            }
            if (mode == 4) {
                if (g_verbose >= 2)
                    printf("        f%5d try: player (%d,%d) anim %x atk %x  arm (%d,%d) v %d box y %d..%d\n", g_frame, px,
                           P.y >> 8, P.anim, s_atk.atk, ax, bi.arm_y >> 8, va, (bi.arm_y >> 8) + 0x64 - 0x23,
                           (bi.arm_y >> 8) + 0x64 + 0x23);
                if (cooldown >= 30 && cooldown <= 68)
                    keys |= DCP_KEY_A; /* full jump */
                if (cooldown == 0)
                    mode = 0;
            }
        }
        frame(keys);
        if (getenv("DUMPAT") && g_frame == atoi(getenv("DUMPAT")))
            dump_view(getenv("DUMPFILE") ? getenv("DUMPFILE") : "/tmp/boss3/view4.ppm");
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
        printf("  stats: attacks %u (shot bursts %u, slams %u, tilt spins %u), shots %u, waves %u, enemies %u, "
               "enemy bounces %u, clashes %u, hits %u, electrified %u\n",
               st->attacks, st->shot_bursts, st->slams, st->tilt_spins, st->shots, st->waves, st->enemies,
               st->enemy_bounces, st->clashes, st->hits, st->electrified);
        printf("  body jumps %d, arm tries %d, assists %d, player hurts %d; max shots %d waves %d alive; body box lowest bottom y %d\n",
               body_tries, arm_tries, assists, g_hurts, max_shots, max_waves, body_low);
        printf("  effects %u; navi %d; sfx 0x89 %d 0x8a %d 0x8c %d 0x8d %d 0x33 %d; ring_bonus %d\n", st->effects_spawned,
               g_navi, g_sfx[0x89], g_sfx[0x8a], g_sfx[0x8c], g_sfx[0x8d], g_sfx[0x33], g_obj_stage.ring_bonus);
        boss_camera_eye(eye, at);
        printf("  camera eye (%.1f,%.1f,%.1f) at (%.1f,%.1f,%.1f)\n", eye[0], eye[1], eye[2], at[0], at[1], at[2]);
        CHECK(hits_in_miss == 0, "[miss] standing still never damages the boss (%d hurts)", hurts_in_miss);
        CHECK(st->shot_bursts > 0 && st->slams > 0 && st->tilt_spins > 0,
              "electric shots / slam / tilt spin all chosen (%u, %u, %u)", st->shot_bursts, st->slams, st->tilt_spins);
        CHECK(st->shots > 0 && st->waves > 0 && max_shots >= 2, "shots fired from the arm gun (%u, %d at once), shock waves (%u)",
              st->shots, max_shots, st->waves);
        CHECK(st->clashes > 0 && st->enemies > 0 && st->enemy_bounces > 0,
              "arm hit -> clash -> enemy dropped -> bounce on it (%u, %u, %u)", st->clashes, st->enemies, st->enemy_bounces);
        CHECK((int)st->hits == boss_health_max(), "%d hits on the body", boss_health_max());
        CHECK(phases_seen[0x14] && phases_seen[0x15] && phases_seen[0x16] && phases_seen[0x18] && phases_seen[0x19] &&
                  phases_seen[0x1a],
              "phases spin/clash/hurt/slam/tilt spin/defeat all visited");
        CHECK(g_pinch_music == 1, "pinch music (Sound_PlaySeq 0xf) at health 2");
        CHECK(boss_defeated() && boss_cleared() && g_cleared_hook == 1, "defeat sequence reached Stage_OnBossCleared");
        CHECK(g_music_stop == 1 && (g_ring_flags & 1) && g_box_flags_calls == 2,
              "defeat stops the music, sets the ring manager flag, disarms the player's boxes");
        CHECK(g_obj_stage.ring_bonus == P.rings * 1000, "ring bonus = rings x 1000 (%d)", g_obj_stage.ring_bonus);
        CHECK(st->effects_spawned > 0 && g_sfx[0x8d] > 0 && g_sfx[0x89] > 0, "hit / clash / impact effects, clash and shot sounds");
        CHECK(boss_player_model(tr, rot) && fabsf(sqrtf((float)tr[0] * tr[0] + (float)tr[2] * tr[2]) / 4096.0f - 297.0f) < 1.0f,
              "player model on the arena circle (radius %.1f px)", sqrtf((float)tr[0] * tr[0] + (float)tr[2] * tr[2]) / 4096.0f);
        CHECK(isfinite(eye[0]) && isfinite(eye[2]) && fabsf(eye[0] - at[0]) + fabsf(eye[2] - at[2]) > 10.0f,
              "boss camera produces a finite eye/at");
        tris = boss_draw(0);
        printf("  boss_draw (host: vertices transformed) %d\n", tris);
        CHECK(tris > 0, "boss models pose and project through the boss camera");
    }
    printf("%d checks, %d failed\n", g_checks, g_fail);
    boss_shutdown();
    return g_fail ? 1 : 0;
}
