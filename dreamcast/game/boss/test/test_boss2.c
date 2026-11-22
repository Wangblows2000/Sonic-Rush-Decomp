/*
 * Host test of the native boss module: Egg Turtle (Boss2) on the real z23
 * level (Water Palace act 3) with the real player module, the real ObjRect
 * code and the converted bs2 models / animations.
 *
 * Usage: test_boss2 <z23/level.bin> <build/dreamcast/player> <models dir> <boss data dir> [-v|-vv]
 *
 * As in test_boss.c, objects_init is not called: the test registers the
 * player's two boxes itself (son.bac boxes of the standing and ball poses).
 *
 * Scripted player (the arena is a circle: the player's x is the arc position,
 * 0x40..0x70d px, wrapped by Boss2_Draw_PlayerOnArena):
 *   [miss]   900 frames: the player stands still at the start position and
 *            never attacks.  Dashes / strikes may hurt him; the boss keeps
 *            its health.
 *   [fight]  whenever a damage box is armed (box 4: head after the attack-2
 *            strike, box 5: head after the attack-3 catch), the player is put
 *            on the floor 40 px beside the head (arc) and does a full jump,
 *            steering onto the head (real jump: son.bac attack box vs ObjRect).
 *            If the head is out of jump reach, the script puts the jumping
 *            player above the box (logged as "assist") and lets him fall.
 *            Dashes are jumped over when the boss comes close.
 *   [defeat] runs the defeat sequence to the stage-clear request.
 * Logs phase / health / state changes, water level and attacks, and checks
 * the outcome.
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
static int g_hurts, g_navi, g_cleared_hook, g_music_stop, g_ring_flags, g_pinch_music, g_water_fx;
static int g_sfx[256], g_sfx_handle_play[4], g_sfx_handle_stop[4];

void rect_check_all(void);
void rect_check_init(void);
void rect_set_box3d(ObjBox *b, s16 l, s16 t, s16 r, s16 bb, s16 zn, s16 zf);
void rect_set_atkdef(ObjBox *b, u32 atk, u32 def);
void obj_handle_collider(DcPlayer *h, void *owner, int kind, ObjBox *b, int has_sprite);
void obj_player_on_hurt(ObjBox *body, ObjBox *attacker);
s32 boss2_palette(int model);
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
        printf("    f%5d player hurt at x=%d (rings %d)  boss %s phase %d\n", g_frame, a, b, boss_state_name(0),
               boss_phase());
    }
}
static void hk_boss_log(void *ctx, const char *ev, s32 a, s32 b) {
    (void)ctx;
    if ((!strcmp(ev, "effect") || !strcmp(ev, "water_fx")) && g_verbose < 2)
        return;
    if (!g_verbose && strncmp(ev, "attack", 6) && strcmp(ev, "hit") && strcmp(ev, "defeat") && strcmp(ev, "cleared") &&
        strcmp(ev, "start") && strncmp(ev, "water_", 6) && strcmp(ev, "jump"))
        return;
    printf("    f%5d boss %-16s %6d %6d   player (%d,%d) water %d\n", g_frame, ev, a, b, P.x >> 8, P.y >> 8,
           boss_water_level());
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
static void hk_water_fx(void *ctx, s32 x, s32 y, s32 z) { (void)ctx; (void)x; (void)y; (void)z; g_water_fx++; }

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

static void place_on_floor(s32 x) {
    s16 rings = P.rings;
    u16 mu = P.map_up, md = P.map_down, mr = P.map_right, ml = P.map_left;
    u16 disp = P.disp_flags & 0x2040;
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

static s32 arc_dist(s32 a, s32 b) { /* px on the 0x6cd circle */
    s32 d = abs(a - b) % 0x6cd;
    return d > 0x6cd / 2 ? 0x6cd - d : d;
}

int main(int argc, char **argv) {
    long size;
    char path[512];
    s16 *trig;
    u8 *phys;
    const rlvl_object_t *ps;
    s32 sx = 224, sy = 400;
    int r, n, last_phase = -1, last_health = -1, phases_seen[0x20] = {0}, last_water = -1;
    char last_state[64] = "", last_wstate[64] = "";
    int hits_in_miss = 0, hurts_in_miss = 0, attack_cooldown = 0, attempts = 0, assists = 0;
    int max_frames, start_frame, tris, water_min = 9999, water_max = -1, lowered_seen = 0;
    s32 prev_boss_x = 0, target_box = -1, box_x = 0, box_y = 0, box_z = 0;
    s16 rect[4];
    int head_y_min[2] = {9999, 9999}, head_y_max[2] = {-9999, -9999};
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
    g_dcp_world.zone = 1;
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
    printf("z23: %dx%d px, stage %d, PlayerStart (%d,%d), boss object 0x3e %s, floor y at x=64/%d/1800: %d/%d/%d\n",
           M.right, M.bottom, ((const rlvl_header_t *)g_level)->stage, sx, sy, find_obj(0x3e) ? "found" : "missing", sx,
           floor_y(64), floor_y(sx), floor_y(1800));
    {
        const rlvl_header_t *h = (const rlvl_header_t *)g_level;
        const rlvl_object_t *o = (const rlvl_object_t *)rlvl_section(g_level, RLVL_SEC_OBJECTS, NULL);
        u32 i;
        printf("z23 objects:");
        for (i = 0; i < h->num_objects; i++)
            printf(" 0x%x@(%d,%d)", o[i].type, o[i].x, o[i].y);
        printf("\n");
    }
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
    g_boss_hooks.water_fx = hk_water_fx;
    boss_set_paths(argv[3], argv[4]);
    boss_set_difficulty(getenv("LEVEL") ? atoi(getenv("LEVEL")) : 0);
    r = boss_init(((const rlvl_header_t *)g_level)->stage, g_level, &P, 0);
    printf("boss_init: %d  health %d/%d phase %d\n", r, boss_health(), boss_health_max(), boss_phase());
    CHECK(r == 0, "boss_init on z23 (11 bs2 models, bs2.dca, bs2.nsbta timing, effects, bossdata.bin)");
    if (r != 0)
        return 1;
    {
        int hp = getenv("LEVEL") && atoi(getenv("LEVEL")) == 1 ? 8 : 6;
        CHECK(boss_health() == hp && boss_health_max() == hp && boss_phase() == BOSS2_PHASE_IDLE,
              "Boss2_Create: %d hit points, phase 7", hp);
    }
    for (n = 0; n < 60; n++)
        frame(0);
    CHECK(!strcmp(boss_state_name(0), "State_WaitForStart"), "boss waits for the title card (%s)", boss_state_name(0));
    boss_start();
    start_frame = g_frame;
    max_frames = getenv("BOSSFRAMES") ? atoi(getenv("BOSSFRAMES")) : 40000;
    while (g_frame < max_frames && !boss_cleared()) {
        u16 keys = 0;
        int ph = boss_phase(), w = boss_water_level();
        s32 bx, by, bz;
        if (ph != last_phase || boss_health() != last_health) {
            boss_hammer_pos(&bx, &by, &bz);
            printf("  f%5d phase %2d health %d  state %s  water %d (%s)  boss arc x %d body (%d,%d,%d)  player x %d\n",
                   g_frame, ph, boss_health(), boss_state_name(0), w, boss_state_name(1), boss2_arc_x() >> 8, bx >> 8,
                   by >> 8, bz >> 8, P.x >> 8);
            last_phase = ph;
            last_health = boss_health();
            if (ph >= 0 && ph < 0x20)
                phases_seen[ph]++;
        }
        if (g_verbose >= 2 && (strcmp(last_state, boss_state_name(0)) || strcmp(last_wstate, boss_state_name(1)))) {
            printf("      f%5d -> %s / water %s (%d)\n", g_frame, boss_state_name(0), boss_state_name(1), w);
            snprintf(last_state, sizeof(last_state), "%s", boss_state_name(0));
            snprintf(last_wstate, sizeof(last_wstate), "%s", boss_state_name(1));
        }
        if (w != last_water && g_verbose >= 2 && (w % 32 == 0 || w == 410 || w == 614))
            printf("      f%5d water level %d\n", g_frame, w);
        last_water = w;
        if (g_frame > start_frame + 2) {
            if (w < water_min) water_min = w;
            if (w > water_max) water_max = w;
            if (w >= 600)
                lowered_seen = 1;
        }
        {
            int k;
            for (k = 4; k <= 5; k++)
                if (boss2_box(k, &box_x, &box_y, &box_z, rect)) {
                    if (box_y < head_y_min[k - 4]) head_y_min[k - 4] = box_y;
                    if (box_y > head_y_max[k - 4]) head_y_max[k - 4] = box_y;
                }
        }
        P.rings = 50;
        g_dcp_world.lives = 3;
        if (g_frame - start_frame < 900) {
            if (g_frame - start_frame == 0)
                place_on_floor(sx);
        } else if (!boss_defeated()) {
            int k, armed = -1;
            if (attack_cooldown > 0)
                attack_cooldown--;
            for (k = 4; k <= 5; k++)
                if (boss_hammer_box_armed(k))
                    armed = k;
            if (armed >= 0 && attack_cooldown == 0 && (P.move_flags & DCP_MF_GROUND)) {
                boss2_box(armed, &box_x, &box_y, &box_z, rect);
                target_box = armed;
                place_on_floor(box_x - 40);
                attack_cooldown = 80;
                attempts++;
                if (g_verbose)
                    printf("      f%5d attack box %d at (%d,%d) rect (%d,%d,%d,%d)\n", g_frame, armed, box_x, box_y,
                           rect[0], rect[1], rect[2], rect[3]);
            }
            if (attack_cooldown > 40 && attack_cooldown <= 76)
                keys |= DCP_KEY_A;
            if (target_box >= 0 && attack_cooldown > 0 && !(P.move_flags & DCP_MF_GROUND)) {
                if (boss2_box(target_box, &box_x, &box_y, &box_z, rect)) {
                    s32 top = box_y + rect[1];
                    /* steer over the head */
                    if ((P.y >> 8) < top && abs((P.x >> 8) - box_x) > 2)
                        keys |= (P.x >> 8) > box_x ? DCP_KEY_LEFT : DCP_KEY_RIGHT;
                    /* out of jump reach: put the jumping player above the head */
                    if (P.vel_y > 0 && (P.y >> 8) > top - 4 && attack_cooldown < 60 && attack_cooldown > 30 &&
                        arc_dist(P.x >> 8, box_x) < 60) {
                        P.x = box_x << 8;
                        P.y = (top - 24) << 8;
                        P.vel_x = 0;
                        P.vel_y = 0x100;
                        assists++;
                        if (g_verbose)
                            printf("      f%5d assist: player put above box %d (%d,%d)\n", g_frame, target_box, box_x,
                                   top - 24);
                        attack_cooldown = 29;
                    }
                }
            }
            if (attack_cooldown == 0 && boss_phase() == BOSS2_PHASE_DASH) {
                s32 bxn = boss2_arc_x() >> 8, dx = bxn - prev_boss_x;
                if (dx > 0x300) dx -= 0x6cd;
                if (dx < -0x300) dx += 0x6cd;
                if (!(boss_stats()->dashes & 1)) {
                    /* even dashes: run away in the dash direction (Boss2_State_Dash then jumps at the player) */
                    if (dx != 0 && arc_dist(P.x >> 8, bxn) < 260)
                        keys |= dx > 0 ? DCP_KEY_RIGHT : DCP_KEY_LEFT;
                } else if ((P.move_flags & DCP_MF_GROUND) && arc_dist(P.x >> 8, bxn) < 90)
                    keys = DCP_KEY_A; /* odd dashes: hop over the dashing turtle */
            }
        }
        prev_boss_x = boss2_arc_x() >> 8;
        frame(keys);
        if (getenv("DUMPAT") && g_frame == atoi(getenv("DUMPAT")))
            dump_view(getenv("DUMPFILE") ? getenv("DUMPFILE") : "/tmp/boss2/view.ppm");
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
        dcm_lights_t L;
        s32 tr[3], rot[9];
        printf("  end f%d: phase %d health %d cleared %d; state %s\n", g_frame, boss_phase(), boss_health(), boss_cleared(),
               boss_state_name(0));
        printf("  stats: attacks %u (dash %u, jumps %u, strike %u, catch %u), hits %u, attempts %d (assists %d), player hurts %d\n",
               st->attacks, st->dashes, st->jumps, st->strikes, st->catches, st->hits, attempts, assists, g_hurts);
        printf("  water: lowers %u raises %u, level %d..%d; ripples %u splashes %u water_fx %d\n", st->water_lowers,
               st->water_raises, water_min, water_max, st->ripples, st->splashes, g_water_fx);
        printf("  effects %u; navi %d; sfx 0x7b %d 0xa0 %d 0xa3 %d 0x9f %d; handle play/stop %d/%d %d/%d %d/%d; ring_bonus %d\n",
               st->effects_spawned, g_navi, g_sfx[0x7b], g_sfx[0xa0], g_sfx[0xa3], g_sfx[0x9f], g_sfx_handle_play[1],
               g_sfx_handle_stop[1], g_sfx_handle_play[2], g_sfx_handle_stop[2], g_sfx_handle_play[3], g_sfx_handle_stop[3],
               g_obj_stage.ring_bonus);
        printf("  head y while damage box armed: box4 %d..%d, box5 %d..%d; palette sel body %d head %d\n", head_y_min[0],
               head_y_max[0], head_y_min[1], head_y_max[1], boss2_palette(7), boss2_palette(9));
        boss_camera_eye(eye, at);
        printf("  camera eye (%.1f,%.1f,%.1f) at (%.1f,%.1f,%.1f)\n", eye[0], eye[1], eye[2], at[0], at[1], at[2]);
        CHECK(hits_in_miss == 0, "[miss] standing still never damages the boss (%d hurts)", hurts_in_miss);
        CHECK(st->dashes > 0 && st->strikes > 0 && st->catches > 0, "dash / attack-2 strike / attack-3 catch all chosen (%u, %u, %u)",
              st->dashes, st->strikes, st->catches);
        CHECK(st->jumps > 0, "dash turns into a jump at a fleeing player (%u)", st->jumps);
        CHECK(st->water_lowers > 0 && st->water_raises > 0 && lowered_seen && water_min == 410,
              "water drained for attack 3 and raised again (level %d..%d)", water_min, water_max);
        CHECK((int)st->hits == boss_health_max(), "%d hits on the head damage boxes (%u)", boss_health_max(), st->hits);
        CHECK(phases_seen[7] && phases_seen[8] && phases_seen[9] && phases_seen[10] && phases_seen[11] && phases_seen[12],
              "phases idle/dash/attack2/attack3/hurt/defeat all visited");
        CHECK(g_pinch_music == 1, "pinch music (Sound_PlaySeq 0xf) at health 2");
        CHECK(boss_defeated() && boss_cleared() && g_cleared_hook == 1, "defeat sequence reached Stage_OnBossCleared");
        CHECK(g_music_stop == 1 && (g_ring_flags & 1), "defeat stops the music and sets the ring manager flag");
        CHECK(g_obj_stage.ring_bonus == P.rings * 1000, "ring bonus = rings x 1000 (%d)", g_obj_stage.ring_bonus);
        CHECK(st->effects_spawned > 0 && st->ripples > 0 && st->splashes > 0, "hit / water ripple / splash effects spawned");
        CHECK(boss_player_model(tr, rot) && fabsf(sqrtf((float)tr[0] * tr[0] + (float)tr[2] * tr[2]) / 4096.0f - 277.3f) < 1.0f,
              "player model on the arena circle (radius %.1f px)", sqrtf((float)tr[0] * tr[0] + (float)tr[2] * tr[2]) / 4096.0f);
        CHECK(boss_lights(&L) && L.count == 3, "boss lights available for the player / models");
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
