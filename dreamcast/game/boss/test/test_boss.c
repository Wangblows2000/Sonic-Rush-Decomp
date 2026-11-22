/*
 * Host test of the native boss module: Egg Hammer (Boss1) on the real z13
 * level with the real player and object modules and the converted models.
 * The same test runs Boss5 (the zone-5 variant on z53: battery turrets and
 * Boss5PartD3 missiles) when given the z53 level (the zone comes from the
 * level header).
 *
 * Usage: test_boss <z13|z53/level.bin> <build/dreamcast/player> <unused> <unused>
 *                  <models dir> <boss data dir> [-v|-vv]
 *
 * Object system: the real ObjRect code (obj_rect.c: ObjRect_CheckAllGroup,
 * StageObj_HandleCollider, Player_OnHurt, Player_Action_AttackBounce) is
 * linked, but objects_init is not called (the object module's data format is
 * being changed by its owner): the test registers the player's two boxes
 * itself, like Player_Collide, with the son.bac boxes of the two poses the
 * script uses (frames.json of act__ac_sonic): standing anim 0 body
 * (-6,-16,6,16); jump ball anim 7 body (-8,-8,8,8) and attack (-8,-9,8,9).
 * Attack/defence flags come from the player module's atkdef hook.
 *
 * Scripted player:
 *   [miss]  the player stands still under the boss for the first attacks:
 *           the slams hit him (hurts logged), the boss keeps its health.
 *   [fight] whenever the hammer's damage box (box 0) is armed after a slam,
 *           the player is put on the floor beside the hammer and jumps onto
 *           it (real jump: son.bac attack box vs ObjRect box 0). Rolling
 *           hammers are dodged by jumping when they come close.
 *   [defeat] runs the defeat sequence to the stage-clear request.
 * Logs phase / health / state changes and checks the outcome.
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
static s32 g_cam_x, g_cam_y;
static int g_hurts, g_navi, g_cleared_hook, g_music_stop, g_ring_flags, g_pinch_music;
static int g_sfx[256];

/* object-system entry points (obj_rect.c / obj_player.c) */
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
/* Player_InitState collision-box part (objects_add_player) */
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
/* Player_In hurt-box flags + Player_Collide */
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

static void camera_follow(int snap) {
    s32 tx = (P.x >> 8) - 128, ty = (P.y >> 8) - 105;
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
}

/* ------------------------------------------------------------ hooks */
static void hk_obj_log(void *ctx, const char *ev, s32 a, s32 b) {
    (void)ctx;
    if (!strcmp(ev, "hurt")) {
        g_hurts++;
        printf("    f%5d player hurt at x=%d (rings %d)  boss %s / %s / %s\n", g_frame, a, b, boss_state_name(0),
               boss_state_name(1), boss_state_name(2));
    }
}
static void hk_boss_log(void *ctx, const char *ev, s32 a, s32 b) {
    (void)ctx;
    if (!strcmp(ev, "effect") && g_verbose < 2)
        return;
    if (!g_verbose && strncmp(ev, "attack", 6) && strcmp(ev, "hit") && strcmp(ev, "defeat") && strcmp(ev, "cleared") &&
        strcmp(ev, "start"))
        return;
    printf("    f%5d boss %-16s %6d %6d   player (%d,%d)\n", g_frame, ev, a, b, P.x >> 8, P.y >> 8);
}
static void hk_sfx(void *ctx, s32 id) {
    (void)ctx;
    if (id >= 0 && id < 256)
        g_sfx[id]++;
}
static void hk_navi(void *ctx, s32 face, s32 msg, s32 t) { (void)ctx; (void)face; (void)msg; (void)t; g_navi++; }
static void hk_cleared(void *ctx) { (void)ctx; g_cleared_hook++; }
static void hk_music_stop(void *ctx) { (void)ctx; g_music_stop++; }
static void hk_ring_flags(void *ctx, u32 f) { (void)ctx; g_ring_flags |= (int)f; }
static void hk_music(void *ctx, s32 s) { (void)ctx; if (s == 0xf) g_pinch_music++; }

/* ------------------------------------------------------------ frame */
extern unsigned char *g_boss_debug_fb;
/* host debug view: projected boss vertices + the player as a white cross */
static void dump_view(const char *path) {
    static unsigned char fb[640 * 480 * 3];
    dcm_camera_t cam;
    float v[4], o[4];
    int i, x, y;
    FILE *f;
    memset(fb, 0, sizeof(fb));
    g_boss_debug_fb = fb;
    boss_draw(0);
    g_boss_debug_fb = NULL;
    boss_camera(&cam);
    v[0] = P.x / 256.0f; v[1] = -P.y / 256.0f; v[2] = P.z / 256.0f; v[3] = 1;
    {
        float e[3];
        int c;
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
    rect_check_all();                 /* Stage_Main: ObjRect_CheckAllGroup */
    dcp_player_frame(&P, held, pressed, released);
    player_collide();                 /* Player_Collide */
    boss_update(&P);
    camera_follow(0);
    g_frame++;
}

static s32 floor_y(s32 x) {
    s32 yy;
    for (yy = 300; yy < M.bottom; yy++)
        if (rlvl_tile_height(&M, yy, x, 1 | 0x80, NULL, NULL) != 0)
            return yy;
    return M.bottom;
}

static void place_on_floor(s32 x) {
    s16 rings = P.rings;
    u16 mu = P.map_up, md = P.map_down;
    memset(&P, 0, sizeof(P));
    dcp_player_create(&P, 0, 0, x, floor_y(x) - 14);
    P.rings = rings;
    P.pf &= ~0x600000u; /* StageTitleCard_State_StartStage: input enabled */
    P.map_up = mu;
    P.map_down = md;
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

int main(int argc, char **argv) {
    long size;
    char path[512];
    s16 *trig;
    u8 *phys;
    const rlvl_object_t *ps;
    s32 sx = 224, sy = 400;
    int r, n, last_phase = -1, last_health = -1, phases_seen[0x20] = {0};
    char last_states[3][64] = {"", "", ""};
    int hits_in_miss = 0, hurts_in_miss = 0, miss_frames = 0, attack_cooldown = 0;
    int max_frames, attempts = 0;
    float hy_min = 1e9f, hy_max = -1e9f;
    int tris = 0, start_frame;
    if (argc < 7) {
        printf("usage: %s <level.bin> <player dir> <objdata dir> <sprites dir> <models dir> <boss dir> [-v|-vv]\n",
               argv[0]);
        return 2;
    }
    if (argc > 7)
        g_verbose = strcmp(argv[7], "-vv") ? 1 : 2;
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
    g_dcp_world.zone = ((const rlvl_header_t *)g_level)->zone; /* 0 = z13 Boss1, 4 = z53 Boss5 */
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
    printf("z%d3: %dx%d px, stage %d, PlayerStart (%d,%d), floor y at x=%d: %d\n", g_dcp_world.zone + 1, M.right, M.bottom,
           ((const rlvl_header_t *)g_level)->stage, sx, sy, sx, floor_y(sx));
    dcp_player_create(&P, 0, 0, sx, sy);
    P.pf &= ~0x600000u;
    g_obj_hooks.log = hk_obj_log;
    g_dcp_hooks.atkdef = hk_atkdef;
    rect_check_init();
    player_boxes_init();
    camera_follow(1);
    g_boss_hooks.log = hk_boss_log;
    g_boss_hooks.sfx = hk_sfx;
    g_boss_hooks.voice = hk_sfx;
    g_boss_hooks.navi = hk_navi;
    g_boss_hooks.cleared = hk_cleared;
    g_boss_hooks.music_stop = hk_music_stop;
    g_boss_hooks.ring_flags = hk_ring_flags;
    g_boss_hooks.music = hk_music;
    boss_set_paths(argv[5], argv[6]);
    boss_set_difficulty(getenv("LEVEL") ? atoi(getenv("LEVEL")) : 0);
    r = boss_init(((const rlvl_header_t *)g_level)->stage, g_level, &P, 0);
    printf("boss_init: %d  health %d/%d phase %d\n", r, boss_health(), boss_health_max(), boss_phase());
    CHECK(r == 0, "boss_init on z%d3 (models, animations, bossdata.bin)", g_dcp_world.zone + 1);
    if (r != 0)
        return 1;
    {
        int hp = getenv("LEVEL") && atoi(getenv("LEVEL")) == 1 ? 8 : 6;
        CHECK(boss_health() == hp && boss_health_max() == hp, "Boss_GetStartHealth: %d hit points (gGameState.unk14 = %s)", hp,
              getenv("LEVEL") ? getenv("LEVEL") : "0");
    }

    if (getenv("JUMPTEST")) {
        int k;
        place_on_floor(atoi(getenv("JUMPTEST")));
        for (k = 0; k < 30; k++) {
            frame(k >= 5 && k < 20 ? DCP_KEY_A : 0);
            printf("  jt %d y %d vy %d mf %x\n", k, P.y >> 8, P.vel_y, P.move_flags);
        }
        return 0;
    }
    /* title card: 60 frames before StageTitleCard_State_StartStage sets gBossStatus.unk0 */
    for (n = 0; n < 60; n++)
        frame(0);
    CHECK(!strcmp(boss_state_name(0), "State_WaitForStart"), "boss waits for the title card (%s)", boss_state_name(0));
    boss_start();
    start_frame = g_frame;

    max_frames = getenv("BOSSFRAMES") ? atoi(getenv("BOSSFRAMES")) : 30000;
    while (g_frame < max_frames && !boss_cleared()) {
        u16 keys = 0;
        s32 hx, hy, hz;
        int ph = boss_phase(), i;
        boss_hammer_pos(&hx, &hy, &hz);
        /* log changes */
        if (ph != last_phase || boss_health() != last_health) {
            printf("  f%5d phase %2d health %d  states %s / %s / %s  hammer (%d,%d,%d)\n", g_frame, ph, boss_health(),
                   boss_state_name(0), boss_state_name(1), boss_state_name(2), hx >> 8, hy >> 8, hz >> 8);
            last_phase = ph;
            last_health = boss_health();
            if (ph >= 0 && ph < 0x20)
                phases_seen[ph]++;
        }
        if (g_verbose >= 2)
            for (i = 0; i < 3; i++)
                if (strcmp(last_states[i], boss_state_name(i))) {
                    printf("      f%5d part %d -> %s\n", g_frame, i, boss_state_name(i));
                    snprintf(last_states[i], sizeof(last_states[i]), "%s", boss_state_name(i));
                }
        if (boss_hammer_box_armed(0)) {
            if (hy >> 8 < hy_min) hy_min = (float)(hy >> 8);
            if (hy >> 8 > hy_max) hy_max = (float)(hy >> 8);
        }
        P.rings = 50; /* keep the player alive: hurts scatter rings instead of killing */
        g_dcp_world.lives = 3;
        if (g_frame - start_frame < 900) {
            /* [miss] stand under the boss's centre, never attack */
            miss_frames++;
            if (g_frame - start_frame == 0)
                place_on_floor(0xe1);
        } else if (!boss_defeated()) {
            /* [fight] */
            if (attack_cooldown > 0)
                attack_cooldown--;
            if (boss_hammer_box_armed(0) && attack_cooldown == 0 && (P.move_flags & DCP_MF_GROUND)) {
                s32 side = (P.x > hx) ? 1 : -1;
                place_on_floor((hx >> 8) + side * 40);
                attack_cooldown = 70;
                attempts++;
            }
            if (attack_cooldown > 30 && attack_cooldown <= 66)
                keys |= DCP_KEY_A; /* full-height jump */
            if (attack_cooldown > 0 && !(P.move_flags & DCP_MF_GROUND) && (P.y >> 8) < 446 && abs((P.x >> 8) - (hx >> 8)) > 2)
                keys |= P.x > hx ? DCP_KEY_LEFT : DCP_KEY_RIGHT; /* over the push box, then onto the head */
            if (attack_cooldown == 0 && boss_phase() == BOSS_PHASE_ROLL && abs((P.x >> 8) - (hx >> 8)) < 70 &&
                (P.move_flags & DCP_MF_GROUND))
                keys = DCP_KEY_A; /* hop over the rolling hammer */
        }
        frame(keys);
        if (getenv("DUMPAT") && g_frame == atoi(getenv("DUMPAT")))
            dump_view(getenv("DUMPFILE") ? getenv("DUMPFILE") : "/tmp/boss/view.ppm");
        if (getenv("DBGHIT") && boss_hammer_box_armed(0) && g_frame - start_frame >= 900 && g_frame - start_frame < 1400)
            printf("      dbg f%d hammer (%d,%d) player (%d,%d) atk %x flags %x ground %d keys %x pf %x held %x state %p mf %x vy %d jump %d push %d hs %x\n", g_frame, hx >> 8,
                   hy >> 8, P.x >> 8, P.y >> 8, s_atk.atk, s_atk.flags, (P.move_flags & 1) != 0, keys, P.pf, P.held, (void *)P.state, P.move_flags, P.vel_y, P.jump, P.push_x, P.hitstop);
        if (g_frame - start_frame == 900) {
            hits_in_miss = (int)boss_stats()->hits;
            hurts_in_miss = g_hurts;
            printf("  [miss] %d frames standing under the boss: %d hurts, %d hits, health %d\n", miss_frames,
                   hurts_in_miss, hits_in_miss, boss_health());
        }
    }
    {
        const BossStats *st = boss_stats();
        dcm_camera_t cam;
        float eye[3], at[3];
        printf("  end f%d: phase %d health %d cleared %d; states %s / %s / %s\n", g_frame, boss_phase(), boss_health(),
               boss_cleared(), boss_state_name(0), boss_state_name(1), boss_state_name(2));
        printf("  stats: attacks %u (slam %u triple %u big %u roll %u missile %u), hits %u, attack attempts %d, player hurts %d\n",
               st->attacks, st->slams, st->triple_slams, st->big_slams, st->rolls, st->missiles, st->hits, attempts,
               g_hurts);
        printf("  effects %u, pebbles %u; navi messages %d; sfx 0x7f %d 0x80 %d 0x82 %d; ring_bonus %d\n",
               st->effects_spawned, st->pebbles_spawned, g_navi, g_sfx[0x7f], g_sfx[0x80], g_sfx[0x82],
               g_obj_stage.ring_bonus);
        printf("  hammer y while its damage box was armed: %.0f..%.0f px\n", hy_min, hy_max);
        boss_camera(&cam);
        boss_camera_eye(eye, at);
        printf("  camera eye (%.1f,%.1f,%.1f) at (%.1f,%.1f,%.1f)\n", eye[0], eye[1], eye[2], at[0], at[1], at[2]);
        CHECK(hurts_in_miss > 0 && hits_in_miss == 0, "[miss] standing under the slams hurts the player (%d) and never damages the boss",
              hurts_in_miss);
        CHECK(st->slams > 0 && st->big_slams > 0, "slam and big slam attacks chosen (%u, %u)", st->slams, st->big_slams);
        if (g_dcp_world.zone == 4) {
            printf("  missiles fired %u, exploded %u\n", st->missiles_fired, st->missile_explosions);
            CHECK(st->missiles > 0 && st->missiles_fired > 0 && st->missile_explosions > 0 && phases_seen[0x1b],
                  "zone 5: missile attacks (%u) fire Boss5PartD3 missiles (%u fired, %u exploded)", st->missiles,
                  st->missiles_fired, st->missile_explosions);
        } else
            CHECK(st->rolls > 0, "hammer roll chosen at health 3..5 / 1..2 (%u)", st->rolls);
        CHECK((int)st->hits == boss_health_max(), "%d hits on the hammer's damage box (%u)", boss_health_max(), st->hits);
        CHECK(phases_seen[1] && phases_seen[2] && phases_seen[3] && (phases_seen[4] || g_dcp_world.zone == 4) &&
                  phases_seen[5] && phases_seen[6],
              "phases idle/slam/bigslam/%s/hurt/defeat all visited", g_dcp_world.zone == 4 ? "missile" : "roll");
        CHECK(g_pinch_music == 1, "pinch music (Sound_PlaySeq 0xf) at health 2");
        CHECK(boss_defeated() && boss_cleared() && g_cleared_hook == 1, "defeat sequence reached Stage_OnBossCleared");
        CHECK(g_music_stop == 1 && (g_ring_flags & 1), "defeat stops the music and sets the ring manager flag");
        CHECK(g_obj_stage.ring_bonus == P.rings * 1000, "ring bonus = rings x 1000 (%d)", g_obj_stage.ring_bonus);
        CHECK(st->effects_spawned > 0 && (st->pebbles_spawned > 0 || g_dcp_world.zone == 4),
              "impact/hit effects and roll pebbles (no roll in zone 5) spawned");
        CHECK(hy_max > 470 && hy_max < 500 && hy_min > 300,
              "hammer centre from the neck_ham / hammer node matrices lands at the floor (max y %.0f) when the damage box arms", hy_max);
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
