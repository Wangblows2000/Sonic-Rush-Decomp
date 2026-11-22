/*
 * Host test of the native boss module: BossF (the final zone boss, stage zf3 = 23) on the real zf3 level
 * with the real player module, the real ObjRect and ObjCollisionMap code and the converted bsf / eff models
 * and animations.
 *
 * Usage: test_bossf <zf3/level.bin> <build/dreamcast/player> <models dir> <boss data dir> [-v|-vv]
 *   SEED=n LEVEL=0|1 STORY=0|1 (1: Blaze) BOSSFRAMES=n EXPLORE=1|2
 *   NOCROUCH=1: the player stays standing on the arm when the hand swings and must be knocked off
 *
 * As in test_boss.c, objects_init is not called: the test registers the player's two boxes itself and
 * installs the ObjCollisionMap probe (colmap_probe_for_obj) and the per-frame commit that
 * objects_frame_begin does in the game.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../boss.h"
extern uint32_t g_boss_matanim_draws;   /* boss_model.c */
#include "../boss_final.h"
#include "../../objects/objects.h"
#include "../../player/dcp_collision.h"

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
static int g_hurts, g_navi, g_cleared_hook, g_music_stop, g_ring_flags, g_talks, g_talk_id = -1;
static int g_help_create, g_help_destroy, g_help_last = -1, g_help_shown, g_enemy_room;
static int g_music[32];
static int g_hand_vel_seen, g_hand_vel; /* P.vel_x right after a fist hit during the throw (phase 0x32) */
static int g_sfx[256];

void rect_check_all(void);
void rect_check_init(void);
void rect_set_box3d(ObjBox *b, s16 l, s16 t, s16 r, s16 bb, s16 zn, s16 zf);
void rect_set_atkdef(ObjBox *b, u32 atk, u32 def);
void obj_handle_collider(DcPlayer *h, void *owner, int kind, ObjBox *b, int has_sprite);
void obj_player_on_hurt(ObjBox *body, ObjBox *attacker);
void colmap_reset(void);
void colmap_commit(void);
s32 colmap_probe_for_obj(DcPlayer *a, s32 x, s32 y, u32 flags, u32 dir, u8 *angle, u8 *attr);

static ObjBox s_body, s_atk;
static s16 s_prev_invuln;

static void hk_atkdef(void *ctx, DcPlayer *p, s32 box, s32 atk, s32 def) {
    (void)ctx;
    (void)p;
    rect_set_atkdef(box == 0 ? &s_body : &s_atk, (u32)atk, (u32)def);
}
static s32 hk_obj_probe(void *ctx, DcPlayer *p, s32 x, s32 y, u32 flags, u32 dir, u8 *angle, u8 *attr) {
    (void)ctx;
    return colmap_probe_for_obj(p, x, y, flags, dir, angle, attr);
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
            printf("    f%5d player hurt at x=%d (rings %d)  boss %s phase %#x\n", g_frame, a, b, boss_state_name(1),
                   boss_phase());
    }
}
static void hk_boss_log(void *ctx, const char *ev, s32 a, s32 b) {
    (void)ctx;
    if (!strcmp(ev, "hand_hit") && a == BOSSF_PHASE_THROW) {
        g_hand_vel_seen++;
        g_hand_vel = P.vel_x;
    }
    if (!strcmp(ev, "effect") && g_verbose < 2)
        return;
    if (!g_verbose && strncmp(ev, "attack", 6) && strcmp(ev, "hit") && strcmp(ev, "hand_hit") && strcmp(ev, "defeat") &&
        strcmp(ev, "cleared") && strcmp(ev, "start") && strcmp(ev, "talk") && strcmp(ev, "base_destroyed"))
        return;
    printf("    f%5d boss %-16s %6d %6d   player (%d,%d) anim %#x\n", g_frame, ev, a, b, P.x >> 8, P.y >> 8, P.anim);
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
static void hk_music(void *ctx, s32 s) { (void)ctx; if (s >= 0 && s < 32) g_music[s]++; }
static int g_fade3c;
static void hk_music_fade(void *ctx, s32 f) { (void)ctx; if (f == 0x3c) g_fade3c++; }
static void hk_talk(void *ctx, s32 id) { (void)ctx; g_talks++; g_talk_id = id; }
static void hk_help(void *ctx, s32 kind, s32 a, s32 b) {
    (void)ctx;
    (void)b;
    if (kind == BOSS_HELP_CREATE) {
        g_help_create++;
        g_help_last = a;
        g_help_shown = 1;
    } else if (kind == BOSS_HELP_DESTROY) {
        g_help_destroy++;
        g_help_shown = 0;
    } else if (kind == BOSS_HELP_ENEMY_ROOM_FIX)
        g_enemy_room++;
}

static u16 s_prev;
static void frame(u16 held) {
    u16 pressed = held & ~s_prev, released = s_prev & ~held;
    s_prev = held;
    rect_check_all();        /* objects_frame_begin: ObjRect_CheckAllGroup */
    colmap_commit();         /*                      ObjCollisionMap_CommitFrame */
    boss_update_pre(&P);     /* BossFPartD8 (task 0xf0) */
    dcp_player_frame(&P, held, pressed, released);
    player_collide();
    boss_update(&P);
    if (g_dcp_world.flags & 1)
        g_dcp_world.frame++; /* the stage timer (gGameState.unk20) */
    g_frame++;
}

static s32 floor_y(s32 x) {
    s32 yy;
    for (yy = 200; yy < M.bottom; yy++)
        if (rlvl_tile_height(&M, yy, x, 1 | 0x80, NULL, NULL) != 0)
            return yy;
    return M.bottom;
}

static void place(s32 x, s32 y) {
    s16 rings = P.rings;
    u16 mu = P.map_up, md = P.map_down, mr = P.map_right, ml = P.map_left;
    u8 ch = P.chara;
    memset(&P, 0, sizeof(P));
    dcp_player_create(&P, ch, 0, x, y);
    P.rings = rings;
    P.pf &= ~0x600000u;
    P.map_up = mu;
    P.map_down = md;
    P.map_right = mr;
    P.map_left = ml;
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

static void box_px(const ObjBox *b, s32 *l, s32 *t, s32 *r, s32 *bb) {
    if (b->flags & 1) { /* mirrored (ObjRect flag 1: StageObj_HandleCollider sets it when the owner is not flipped) */
        *l = b->x - b->r;
        *r = b->x - b->l;
    } else {
        *l = b->x + b->l;
        *r = b->x + b->r;
    }
    *t = b->y + b->t;
    *bb = b->y + b->b;
}

int main(int argc, char **argv) {
    long size;
    char path[512];
    s16 *trig;
    u8 *phys;
    const rlvl_object_t *ps;
    s32 sx = 384, sy = 752;
    int r, n, last_phase = -1, last_health = -1, phases_seen[0x40] = {0};
    int max_frames, start_frame, story, explore, boost, hop, nocrouch, knock_frame = -1;
    int cooldown = 0, mode = 0, tries = 0, stuck = 0, jump_hold = 0, core_tries = 0, sky_tries = 0;
    s32 last_px = 0;
    s32 tgt_x = 0;
    int miss_ok = -1, fight, timer_stop_sky = 0, timer_restart_sky = 0, blend_max = 0, blend_mode2 = 0;
    int riding_seen = 0, ride_draw_seen = 0, ride_model_ok = 0, sky_b_seen = 0, health_max;
    int max_balls = 0, max_shocks = 0, max_waves = 0, max_lasers = 0, max_flashes = 0, max_beams = 0;
    float eye[3], at[3];
    if (argc < 5) {
        printf("usage: %s <level.bin> <player dir> <models dir> <boss dir> [-v|-vv]\n", argv[0]);
        return 2;
    }
    if (argc > 5)
        g_verbose = strcmp(argv[5], "-vv") ? 1 : 2;
    story = getenv("STORY") ? atoi(getenv("STORY")) : 0;
    explore = getenv("EXPLORE") ? atoi(getenv("EXPLORE")) : 0;
    boost = getenv("BOOST") ? atoi(getenv("BOOST")) : 0;
    hop = getenv("HOP") ? atoi(getenv("HOP")) : 0;
    nocrouch = getenv("NOCROUCH") ? atoi(getenv("NOCROUCH")) : 0;
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
    g_dcp_col.obj_probe = hk_obj_probe;
    colmap_reset();
    memset(&g_dcp_world, 0, sizeof(g_dcp_world));
    g_dcp_world.zone = 7;
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
    printf("zf3: %dx%d px, stage %d, PlayerStart (%d,%d), boss object 0x38 %s, floor y at x=64/%d/800/1200: %d/%d/%d/%d\n",
           M.right, M.bottom, ((const rlvl_header_t *)g_level)->stage, sx, sy, find_obj(0x38) ? "found" : "missing", sx,
           floor_y(64), floor_y(sx), floor_y(800), floor_y(1200));
    dcp_player_create(&P, (u8)story, 0, sx, sy);
    P.pf &= ~0x600000u;
    g_obj_hooks.log = hk_obj_log;
    g_dcp_hooks.atkdef = hk_atkdef;
    rect_check_init();
    player_boxes_init();
    g_boss_hooks.log = hk_boss_log;
    g_boss_hooks.sfx = hk_sfx;
    g_boss_hooks.voice = hk_sfx;
    g_boss_hooks.navi = hk_navi;
    g_boss_hooks.cleared = hk_cleared;
    g_boss_hooks.music_stop = hk_music_stop;
    g_boss_hooks.ring_flags = hk_ring_flags;
    g_boss_hooks.music = hk_music;
    g_boss_hooks.music_fade = hk_music_fade;
    g_boss_hooks.talk = hk_talk;
    g_boss_hooks.help = hk_help;
    boss_set_paths(argv[3], argv[4]);
    boss_set_difficulty(getenv("LEVEL") ? atoi(getenv("LEVEL")) : 0);
    r = boss_init(((const rlvl_header_t *)g_level)->stage, g_level, &P, 0);
    printf("boss_init: %d  health %d/%d phase %#x\n", r, boss_health(), boss_health_max(), boss_phase());
    CHECK(r == 0, "boss_init on zf3 (bsf models, bsf.dca/.dcv, passage, sky, effects, bsf_st_r collision map)");
    if (r != 0)
        return 1;
    for (n = 0; n < 60; n++)
        frame(0);
    CHECK(!strcmp(boss_state_name(0), "State_WaitForStart") && !strcmp(boss_state_name(1), "State_Idle"),
          "boss waits for the title card (%s / %s)", boss_state_name(0), boss_state_name(1));
    boss_start();
    start_frame = g_frame;
    max_frames = getenv("BOSSFRAMES") ? atoi(getenv("BOSSFRAMES")) : 60000;
    while (g_frame < max_frames && !boss_cleared()) {
        BossFInfo bi;
        int ph = boss_phase();
        u16 keys = 0;
        bossf_info(&bi);
        fight = g_frame - start_frame >= 900;
        if (g_frame - start_frame == 900) {
            miss_ok = boss_health() == boss_health_max();
            printf("  [miss] 900 frames standing still: health %d/%d, player hurts %d\n", boss_health(),
                   boss_health_max(), g_hurts);
        }
        {
            s32 bm, be;
            bossf_blend(&bm, &be);
            if (bm == 2) {
                blend_mode2 = 1;
                if (be > blend_max)
                    blend_max = be;
            }
        }
        if (ph == BOSSF_PHASE_SKY && !(g_dcp_world.flags & 1))
            timer_stop_sky = 1;
        if (timer_stop_sky && ph == BOSSF_PHASE_SKY && (g_dcp_world.flags & 1))
            timer_restart_sky = 1;
        if (bi.riding)
            riding_seen++;
        if (bi.ride_draw) {
            s32 tr[3], rot[9];
            ride_draw_seen++;
            if (boss_player_model(tr, rot) && abs(rot[0]) <= 4096 && abs(rot[4]) <= 4096 && abs(rot[8]) <= 4096 &&
                (rot[0] | rot[1] | rot[2]) != 0)
                ride_model_ok++;
        }
        if (bi.sky_model == 1)
            sky_b_seen = 1;
        if (bi.balls > max_balls) max_balls = bi.balls;
        if (bi.shocks > max_shocks) max_shocks = bi.shocks;
        if (bi.waves > max_waves) max_waves = bi.waves;
        if (bi.lasers > max_lasers) max_lasers = bi.lasers;
        if (bi.flashes > max_flashes) max_flashes = bi.flashes;
        if (bi.beams > max_beams) max_beams = bi.beams;
        if (ph != last_phase || boss_health() != last_health) {
            printf("  f%5d phase %#x health %d  %s  body (%d,%d,%d) core (%d,%d) fistL (%d,%d) fistR (%d,%d) rot %#x player (%d,%d)\n",
                   g_frame, ph, boss_health(), boss_state_name(1), bi.x >> 8, bi.y >> 8, bi.z >> 8, bi.core[0] >> 12,
                   -bi.core[1] >> 12, bi.fist_l[0] >> 12, -bi.fist_l[1] >> 12, bi.fist_r[0] >> 12, -bi.fist_r[1] >> 12,
                   bi.rot, P.x >> 8, P.y >> 8);
            last_phase = ph;
            last_health = boss_health();
            if (ph >= 0 && ph < 0x40)
                phases_seen[ph]++;
        }
        if (explore && (g_frame % 30) == 0) {
            int i;
            printf("    f%5d %-34s armed %02x cmap %d ride %d mapR %d pl (%d,%d) anim %#x  ", g_frame, boss_state_name(1),
                   bi.boxes_armed, bi.cmap_on, bi.riding, bi.map_right, P.x >> 8, P.y >> 8, P.anim);
            for (i = 0; i < 7; i++)
                if (bi.boxes_armed & (1 << i)) {
                    s32 l, t, rr, b;
                    box_px(&bi.box[i], &l, &t, &rr, &b);
                    printf("[%d: %d..%d x %d..%d] ", i, l, rr, t, b);
                }
            printf("\n");
        }
        P.rings = 50;
        if (boost)
            P.tension = 0x12c0;
        g_dcp_world.lives = 3;
        if (cooldown > 0)
            cooldown--;
        /* [fight] a fist lies on the floor with its damage box armed: jump on it */
        if (fight && mode == 0 && cooldown == 0 && (P.move_flags & DCP_MF_GROUND) &&
            (ph == BOSSF_PHASE_HANDSLAM || ph == BOSSF_PHASE_PRESS || ph == BOSSF_PHASE_THROW) &&
            (bi.boxes_armed & 0x48)) {
            s32 l, t, rr, b, cx, side;
            box_px(&bi.box[(bi.boxes_armed & 0x08) ? 3 : 6], &l, &t, &rr, &b);
            cx = (l + rr) / 2;
            side = (P.x >> 8) < cx ? -1 : 1;
            if (cx + side * 56 < 16 || cx + side * 56 > bi.map_right - 16)
                side = -side;
            place(cx + side * 56, floor_y(cx + side * 56) - 14);
            tgt_x = cx;
            mode = 1;
            cooldown = 70;
            tries++;
        }
        if (mode == 1) {
            if (cooldown >= 50 && cooldown <= 68)
                keys |= DCP_KEY_A;
            if (abs((P.x >> 8) - tgt_x) > 6)
                keys |= (P.x >> 8) < tgt_x ? DCP_KEY_RIGHT : DCP_KEY_LEFT;
            if (cooldown == 0)
                mode = 0;
        }
        /* [ride] the arm lies on the floor and the body is open: climb the arm, jump at the core; crouch
         * while the warning is shown and the hand swings */
        if (fight && mode == 0 && (ph == BOSSF_PHASE_RIDE || ph == BOSSF_PHASE_RIDE_MOVE)) {
            s32 l, t, rr, b, px = P.x >> 8;
            box_px(&bi.box[0], &l, &t, &rr, &b);
            int warn_end = g_help_shown && (!strcmp(boss_state_name(1), "State_HandRideWarn") ? bi.timer <= 40 : 1);
            if (nocrouch && (warn_end || ph == BOSSF_PHASE_RIDE_MOVE)) {
                jump_hold = 0; /* stand still on the arm */
            } else if (warn_end || ph == BOSSF_PHASE_RIDE_MOVE) {
                /* stop, then crouch (holding down while moving rolls) */
                if ((P.move_flags & DCP_MF_GROUND) && P.gnd_vel > 0x60)
                    keys |= DCP_KEY_LEFT;
                else if ((P.move_flags & DCP_MF_GROUND) && P.gnd_vel < -0x60)
                    keys |= DCP_KEY_RIGHT;
                else if (P.move_flags & DCP_MF_GROUND)
                    keys |= DCP_KEY_DOWN;
                jump_hold = 0;
            } else if (px < l - 40) {
                keys |= DCP_KEY_RIGHT;
                if (boost && px > 790)
                    keys |= DCP_KEY_X;
                if ((P.move_flags & DCP_MF_GROUND) && px <= last_px)
                    stuck++;
                else
                    stuck = 0;
                if ((stuck > 3 || (hop && px >= 760)) && jump_hold == 0 && (P.move_flags & DCP_MF_GROUND))
                    jump_hold = 24;
            } else {
                if (px < (l + rr) / 2)
                    keys |= DCP_KEY_RIGHT;
                if (!nocrouch && (P.move_flags & DCP_MF_GROUND) && jump_hold == 0 && cooldown == 0) {
                    jump_hold = 14;
                    cooldown = 40;
                    core_tries++;
                }
            }
            if (jump_hold > 0) {
                keys |= DCP_KEY_A;
                jump_hold--;
            }
            last_px = px;
        }
        /* [beam] a beam ball waits (BossFPartDE_State_Aim) until the player stands between its ends, which never
         * reach a player pressed into an arena corner: walk back to the middle as a player would */
        if (fight && mode == 0 && ph == BOSSF_PHASE_BEAM && ((P.x >> 8) < 160 || (P.x >> 8) > bi.map_right - 160))
            mode = 2;
        if (mode == 2) {
            s32 px = P.x >> 8, mid = bi.map_right / 2;
            if (abs(px - mid) > 40 && ph == BOSSF_PHASE_BEAM)
                keys |= px < mid ? DCP_KEY_RIGHT : DCP_KEY_LEFT;
            else
                mode = 0;
        }
        /* [sky] the last hit point: the body charges three times, then hangs low in front of the base
         * with its core open: jump at it */
        if (fight && mode == 0 && ph == BOSSF_PHASE_SKY && (bi.boxes_armed & 1)) {
            s32 l, t, rr, b, px = P.x >> 8, cx;
            box_px(&bi.box[0], &l, &t, &rr, &b);
            cx = (l + rr) / 2;
            if (abs(px - cx) > 6)
                keys |= px < cx ? DCP_KEY_RIGHT : DCP_KEY_LEFT;
            else if ((P.move_flags & DCP_MF_GROUND) && jump_hold == 0 && cooldown == 0) {
                jump_hold = 24;
                cooldown = 50;
                sky_tries++;
            }
            if (jump_hold > 0) {
                keys |= DCP_KEY_A;
                jump_hold--;
            }
        }
        if (explore == 2 && (ph == BOSSF_PHASE_RIDE || ph == BOSSF_PHASE_RIDE_MOVE) && g_frame < 4000)
            printf("      f%5d %-24s t %3d pl (%4d,%4d) v (%5d,%5d) g %5d gr %d ride %d anim %#4x keys %#x\n", g_frame,
                   boss_state_name(1), bi.timer, P.x >> 8, P.y >> 8, P.vel_x, P.vel_y, P.gnd_vel,
                   (P.move_flags & DCP_MF_GROUND) != 0, bi.riding, P.anim, keys);
        frame(keys);
        if (nocrouch && knock_frame < 0 && bossf_stats()->knock_offs > 0)
            knock_frame = g_frame;
        if (nocrouch && knock_frame >= 0 && (boss_phase() == BOSSF_PHASE_IDLE || g_frame - knock_frame > 6000))
            break;
    }
    printf("  end f%d: phase %#x health %d cleared %d; state %s\n", g_frame, boss_phase(), boss_health(), boss_cleared(),
           boss_state_name(1));
    printf("  tries %d core tries %d sky tries %d hurts %d\n", tries, core_tries, sky_tries, g_hurts);
    {
        const BossFStats *st = bossf_stats();
        int level = getenv("LEVEL") ? atoi(getenv("LEVEL")) : 0, tris;
        health_max = boss_health_max();
        printf("  stats: slams %u presses %u throws %u rides %u ride_moves %u lasers %u beams %u sky %u; hand hits %u body "
               "hits %u sky hits %u knock-offs %u; spawned D9 %u DA %u DB %u DC %u DD %u DE %u; talks %u helps %u/%u "
               "white-outs %u\n",
               st->hand_slams, st->presses, st->throws, st->rides, st->ride_moves, st->lasers, st->beams, st->sky_charges,
               st->hand_hits, st->body_hits, st->sky_hits, st->knock_offs, st->balls, st->shocks, st->waves,
               st->laser_objs, st->flashes, st->beam_objs, st->talks, st->helps, st->help_destroys, st->white_outs);
        printf("  max alive D9 %d DA %d DB %d DC %d DD %d DE %d; blend max evy %d; riding %d frames, drawn on the hand "
               "%d (%d valid); hand-hit vel_x %#x (%d); ring_bonus %d rings %d; talk id %#x; help create %d (idx %d) "
               "destroy %d enemy-room %d; music 0x13 %d; music_stop %d ring_flags %d\n",
               max_balls, max_shocks, max_waves, max_lasers, max_flashes, max_beams, blend_max, riding_seen,
               ride_draw_seen, ride_model_ok, g_hand_vel, g_hand_vel_seen, g_obj_stage.ring_bonus, P.rings, g_talk_id,
               g_help_create, g_help_last, g_help_destroy, g_enemy_room, g_music[0x13], g_music_stop, g_ring_flags);
        if (nocrouch) {
            BossFInfo bi;
            bossf_info(&bi);
            CHECK(knock_frame >= 0 && st->knock_offs >= 1 && st->body_hits == 0,
                  "[knock-off] standing on the arm while the hand swings knocks the player off (f%d, %u knock-offs)",
                  knock_frame, st->knock_offs);
            CHECK(phases_seen[BOSSF_PHASE_RIDE_END] >= 1 && !bi.riding && !bi.ride_draw && boss_health() == health_max,
                  "the ride ends (phase 0x35 x%d) without damage to the boss, the player is off the hand, back to idle "
                  "by f%d", phases_seen[BOSSF_PHASE_RIDE_END], g_frame);
            CHECK(boss_phase() == BOSSF_PHASE_IDLE, "the boss returns to its attack cycle (phase %#x)", boss_phase());
            printf("%d checks, %d failed\n", g_checks, g_fail);
            boss_shutdown();
            return g_fail ? 1 : 0;
        }
        CHECK(miss_ok == 1, "[miss] no damage to the boss while the player stands still for 900 frames");
        CHECK(boss_cleared() && g_cleared_hook == 1 && boss_defeated() && boss_phase() == BOSSF_PHASE_DEFEAT,
              "[fight] the whole fight runs to the stage clear (defeat phase 0x3b, cleared hook once) by f%d", g_frame);
        CHECK(boss_health() == 0 && st->body_hits + st->sky_hits == (u32)health_max && st->sky_hits == 1,
              "every hit point is taken: %u body hits (hand ride) + %u sky hit = max health %d", st->body_hits,
              st->sky_hits, health_max);
        CHECK(phases_seen[BOSSF_PHASE_IDLE] && phases_seen[BOSSF_PHASE_RIDE] && phases_seen[BOSSF_PHASE_RIDE_MOVE] &&
                  phases_seen[BOSSF_PHASE_SKY] && phases_seen[BOSSF_PHASE_HURT] && phases_seen[BOSSF_PHASE_HAND_HURT] &&
                  phases_seen[BOSSF_PHASE_DEFEAT] && phases_seen[BOSSF_PHASE_LASER] + phases_seen[BOSSF_PHASE_BEAM] > 0 &&
                  phases_seen[BOSSF_PHASE_HANDSLAM] + phases_seen[BOSSF_PHASE_PRESS] + phases_seen[BOSSF_PHASE_THROW] > 0,
              "phases: idle %d slam %d press %d throw %d ride %d ride-move %d laser %d beam %d sky %d hand-hurt %d hurt %d "
              "defeat %d",
              phases_seen[0x2f], phases_seen[0x30], phases_seen[0x31], phases_seen[0x32], phases_seen[0x33],
              phases_seen[0x34], phases_seen[0x36], phases_seen[0x37], phases_seen[0x38], phases_seen[0x39],
              phases_seen[0x3a], phases_seen[0x3b]);
        CHECK(g_help_create >= 1 && g_help_last == 1 && g_help_destroy >= 1 && g_help_shown == 0 &&
                  st->helps == (u32)g_help_create,
              "the hand-ride warning goes through the help hook: BossHelp_Create(1) x%d, destroyed x%d", g_help_create,
              g_help_destroy);
        CHECK(g_enemy_room >= 1, "EffEnemyRoomFix_Create reaches the help hook as kind %d (x%d)",
              BOSS_HELP_ENEMY_ROOM_FIX, g_enemy_room);
        CHECK(g_talks == 1 && g_talk_id == (story ? 0x10 : 0xf), "the sky-phase talk demo: TalkDemo_Create(%#x) once",
              g_talk_id);
        CHECK(g_fade3c >= 1, "SkyChargeStartFadeOut fades the BGM over 0x3c (x%d)", g_fade3c);
        CHECK(g_music[0x13] >= 1 && g_music_stop >= 1 && (g_ring_flags & 1),
              "music 0x13 started, the music is stopped at the defeat, ring manager flag 1 set");
        CHECK(g_obj_stage.ring_bonus == P.rings * 1000, "ring bonus = rings x 1000 (%d)", g_obj_stage.ring_bonus);
        CHECK(sky_b_seen && st->white_outs >= 1 && blend_mode2 && blend_max == 16,
              "white-out: BG0 brightened to evy %d (mode 2), bsf_sky_b shown", blend_max);
        CHECK(timer_stop_sky && timer_restart_sky,
              "the stage timer stops for the sky charges and runs again after SkyChargeImpact3");
        CHECK(riding_seen > 0 && ride_draw_seen > 0 && ride_model_ok == ride_draw_seen,
              "the player rides the arm (%d frames) and is drawn on the hand with a valid transform (%d frames)",
              riding_seen, ride_draw_seen);
        CHECK(g_hand_vel_seen == 0 || g_hand_vel == (story ? -0xb00 : -0x800),
              "OnHitHand during the throw bounces %s back by %#x (%d throw hits)", story ? "Blaze" : "Sonic",
              -g_hand_vel, g_hand_vel_seen);
        CHECK(st->shocks + st->waves > 0 && st->laser_objs > 0 && st->flashes > 0 && st->beam_objs > 0 &&
                  (level == 0 || st->balls > 0),
              "parts spawned: D9 %u DA %u DB %u DC %u DD %u DE %u", st->balls, st->shocks, st->waves, st->laser_objs,
              st->flashes, st->beam_objs);
        boss_camera_eye(eye, at);
        CHECK(isfinite(eye[0]) && isfinite(eye[1]) && isfinite(eye[2]) && isfinite(at[0]) &&
                  fabsf(eye[0] - at[0]) + fabsf(eye[1] - at[1]) + fabsf(eye[2] - at[2]) > 10.0f,
              "camera eye (%.0f,%.0f,%.0f) at (%.0f,%.0f,%.0f)", eye[0], eye[1], eye[2], at[0], at[1], at[2]);
        tris = boss_draw(0);
        CHECK(tris > 0, "boss_draw transforms %d vertices", tris);
        CHECK(g_boss_matanim_draws > 0, "models drawn with their material / SRT animations (bsf sky): %u",
              (unsigned)g_boss_matanim_draws);
    }
    printf("%d checks, %d failed\n", g_checks, g_fail);
    boss_shutdown();
    return g_fail ? 1 : 0;
}
