/*
 * Host test of the native boss module: Boss7 (Dead Line act 3, stage z73) on the
 * real z73 level with the real player module, the real ObjRect code, the
 * converted bs7 / player / eff_* packages and the son.bac hit-box table.
 *
 * Usage: test_boss7 <z73/level.bin> <build/dreamcast/player> <models dir> <boss data dir> [-v|-vv]
 *   SEED=<n> LEVEL=<0|1> STORY=<0|1|2> (2 = both story characters, default)
 *
 * As in test_boss.c, objects_init is not called: the test registers the
 * player's two boxes itself (son.bac boxes of the standing and ball poses).
 *
 * For each story character (0 Sonic: the rival is Blaze, fire pillars; 1 Blaze:
 * the rival is Sonic, tornado + after-image dash) the test runs:
 *   [miss]   900 frames standing still: the rival attacks, the boss keeps its health.
 *   [fight]  when the rival stands or runs on the floor with its body box armed, the
 *            player is put on the floor 36 px beside it and jumps at it (A held,
 *            steering towards it); after 4 failed tries in a row the jumping player
 *            is put just above the rival ("assist", as in the Boss2/Boss4 tests).
 *            The rival is beaten at 1 hit point: defeat scene, talk, clash.
 *   [watch]  at 2 hit points the rival is left alone until it has used its special
 *            (the attack tables pick it only now and then).
 *   [clash]  A is pressed every other frame (30 presses per second).
 *   [defeat] runs to the stage-clear request.
 * and then a second fight where the player does not mash in the clash ([lose]):
 * the rival wins, the player is knocked back and the fight stays stopped.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../boss.h"
#include "../boss7.h"
extern uint32_t g_boss_matanim_draws;   /* boss_model.c */
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
static int g_hurts, g_navi, g_cleared_hook, g_music_stop, g_ring_flags, g_music11, g_talks, g_talk_id;
static int g_help_create, g_help_destroy, g_help_idx = -1;
static int g_sfx[256], g_sfx_handle_play[4], g_sfx_handle_stop[4];
static int g_exc_seen[8], g_bright_max, g_blend_max;

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
            printf("    f%5d player hurt at x=%d (rings %d)  rival %s / %s phase 0x%x\n", g_frame, a, b,
                   boss_state_name(1), boss_state_name(2), boss_phase());
    }
}
static void hk_boss_log(void *ctx, const char *ev, s32 a, s32 b) {
    (void)ctx;
    if ((!strcmp(ev, "effect") || !strcmp(ev, "fx3d")) && g_verbose < 2)
        return;
    if (!g_verbose && strcmp(ev, "attack") && strcmp(ev, "hit") && strcmp(ev, "defeat") && strcmp(ev, "cleared") &&
        strcmp(ev, "start") && strcmp(ev, "special") && strcmp(ev, "clash") && strcmp(ev, "clash_lost") &&
        strcmp(ev, "talk"))
        return;
    printf("    f%5d boss %-14s %6d %6d   player (%d,%d)\n", g_frame, ev, a, b, P.x >> 8, P.y >> 8);
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
static void hk_music(void *ctx, s32 s) { (void)ctx; if (s == 0x11) g_music11++; }
static int g_fade3c, g_vol20;
/* g_boss_hooks.fx3d: Boss7's StageObj_Load3dBAC effects by BAC id */
static int g_fx3d_boost_a, g_fx3d_boost_a2_sb, g_fx3d_boost_a2_bb, g_fx3d_fire, g_fx3d_spin, g_fx3d_other, g_fx3d_far;
static int hk_fx3d(void *ctx, const char *id, s32 anim, s32 x, s32 y, s32 z, u32 flags, s32 scale) {
    (void)ctx, (void)z, (void)scale;
    if (!(flags & 1) || x < -0x10000 || x > (2048 << 8) || y < 0 || y > (2048 << 8))
        g_fx3d_far++;
    if (!strcmp(id, "act__eff_sb_boost_a_3d") && anim == 0) g_fx3d_boost_a++;
    else if (!strcmp(id, "act__eff_sb_boost_a2_3d") && anim == 0) g_fx3d_boost_a2_sb++;
    else if (!strcmp(id, "act__eff_bb_boost_a2_3d") && anim == 0) g_fx3d_boost_a2_bb++;
    else if (!strcmp(id, "act__ac_eff_b7_3d") && anim == 0) g_fx3d_fire++;
    else if (!strcmp(id, "act__ac_eff_kemuri_spin02_3d") && (anim == 0 || anim == 2)) g_fx3d_spin++;
    else g_fx3d_other++;
    return 0;
}
static void hk_music_fade(void *ctx, s32 f) { (void)ctx; if (f == 0x3c) g_fade3c++; }
static void hk_music_volume(void *ctx, s32 v, s32 f) { (void)ctx; if (v == 0x20 && f == 0xb4) g_vol20++; }
static void hk_help(void *ctx, s32 kind, s32 a, s32 b) {
    (void)ctx;
    (void)b;
    if (kind == BOSS_HELP_CREATE) {
        g_help_create++;
        g_help_idx = a;
    } else if (kind == BOSS_HELP_DESTROY)
        g_help_destroy++;
}
static void hk_talk(void *ctx, s32 id) {
    (void)ctx;
    g_talks++;
    g_talk_id = id;
}

static u16 s_prev;
static void frame(u16 held) {
    u16 pressed = held & ~s_prev, released = s_prev & ~held;
    s32 mode, evy;
    s_prev = held;
    rect_check_all();
    dcp_player_frame(&P, held, pressed, released);
    player_collide();
    boss_update(&P);
    g_frame++;
    if ((g_frame & 7) == 0)
        boss_draw(0);   /* the draw step (material animations of the tornado / flame / aura effects) */
    if (boss_exc_background() >= 0 && boss_exc_background() < 8)
        g_exc_seen[boss_exc_background()]++;
    if (boss_brightness() > g_bright_max)
        g_bright_max = boss_brightness();
    boss7_blend(&mode, &evy);
    if (mode == 2 && evy > g_blend_max)
        g_blend_max = evy;
}

static s32 floor_y(s32 x) {
    s32 yy;
    for (yy = 100; yy < M.bottom; yy++)
        if (rlvl_tile_height(&M, yy, x, 1 | 0x80, NULL, NULL) != 0)
            return yy;
    return M.bottom;
}

static void place_on_floor(s32 x, int chara) {
    s16 rings = P.rings;
    if (x < 24)
        x = 24;
    if (x > M.right - 24)
        x = M.right - 24;
    memset(&P, 0, sizeof(P));
    dcp_player_create(&P, (u8)chara, 0, x, floor_y(x) - 14);
    P.rings = rings;
    P.pf &= ~0x600000u;
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

extern unsigned char *g_boss_debug_fb;
static void dump_view(const char *path) {
    static unsigned char fb[640 * 480 * 3];
    FILE *f;
    memset(fb, 0, sizeof(fb));
    g_boss_debug_fb = fb;
    boss_draw(0);
    g_boss_debug_fb = NULL;
    f = fopen(path, "wb");
    if (!f)
        return;
    fprintf(f, "P6 640 480 255\n");
    fwrite(fb, 1, sizeof(fb), f);
    fclose(f);
    printf("  view dumped to %s\n", path);
}

typedef struct {
    int init_ok, hp, waits, hits_in_miss, phases[0x40], defeat_scene, talks, talk_id, clash, clash_frames;
    int won, lost, cleared, cleared_hook, help_create, help_destroy, help_idx, music11, auras_max, pillars_max;
    int assists, tries, hurts, box_geom, vis_hidden, tris, rival_chara, after, pull, exc6, exc7, bright, blend;
    int lost_knockback, lost_state_none, lost_health, mash_p, mash_r, specials, spindashes, air, ring_bonus_ok;
    int frames, player_spd, sfx_hit, bounces, cam_ok, watch, stall, stall_no_special;
    int base_nodes, base_hidden, dark_clash, diffuse_restored, after_posed;
    u32 diffuse_start;
    Boss7Stats st;
    u32 hits;
} RunResult;

static void run_fight(const char *level_path, int chara, int mash, RunResult *R) {
    long size;
    const rlvl_object_t *ps;
    s32 sx = 64, sy = 480;
    int n, last_phase = -1, last_health = -1, cooldown = 0, mode = 0, fails = 0, try_hits = 0, try_frames = 0;
    int max_frames, start_frame, hp_max, assisted = 0, watch_start = 0, stall_frames = 0;
    (void)size;
    memset(R, 0, sizeof(*R));
    memset(&g_dcp_world, 0, sizeof(g_dcp_world));
    g_dcp_world.zone = 6;
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
    g_frame = 0;
    g_hurts = g_navi = g_cleared_hook = g_music_stop = g_ring_flags = g_music11 = g_talks = g_talk_id = 0;
    g_help_create = g_help_destroy = 0;
    g_help_idx = -1;
    memset(g_sfx, 0, sizeof(g_sfx));
    memset(g_exc_seen, 0, sizeof(g_exc_seen));
    g_bright_max = g_blend_max = 0;
    s_prev = 0;
    memset(&P, 0, sizeof(P));
    dcp_player_create(&P, (u8)chara, 0, sx, sy);
    P.pf &= ~0x600000u;
    rect_check_init();
    player_boxes_init();
    g_obj_stage.ring_bonus = 0;
    printf("z73: %dx%d px, stage %d, PlayerStart (%d,%d), boss object 0x39 %s, floor y at x=32/160/288: %d/%d/%d; "
           "story character %s, mash %d\n",
           M.right, M.bottom, ((const rlvl_header_t *)g_level)->stage, sx, sy, find_obj(0x39) ? "found" : "missing",
           floor_y(32), floor_y(160), floor_y(288), chara ? "Blaze" : "Sonic", mash);
    R->init_ok = boss_init(((const rlvl_header_t *)g_level)->stage, g_level, &P, 0) == 0;
    (void)level_path;
    if (!R->init_ok)
        return;
    hp_max = boss_health_max();
    R->hp = boss_health();
    for (n = 0; n < 60; n++)
        frame(0);
    R->waits = !strcmp(boss_state_name(0), "State_WaitForStart");
    boss_start();
    start_frame = g_frame;
    max_frames = getenv("BOSSFRAMES") ? atoi(getenv("BOSSFRAMES")) : 30000;
    while (g_frame < max_frames && !boss_cleared()) {
        Boss7Info bi;
        int ph = boss_phase();
        u16 keys = 0;
        s32 px = P.x >> 8, rx;
        boss7_info(&bi);
        rx = bi.rival_x >> 8;
        /* the DS stall (BOSSES.md): a special chosen while the rival is in the air hands a negative walk
         * counter (unk456) to HoldAirAttackCharge (unk456-- == 0, s16): the AI holds R for ~65530 frames
         * standing idle (it can still be hit; only the defeat replaces the input state) */
        if (!strcmp(bi.rival_input, "InputState_HoldAirAttackCharge") && bi.rival_vy == 0 && boss_health() > 1)
            stall_frames++;
        else
            stall_frames = 0;
        if (stall_frames == 300 && !R->stall) {
            R->stall = 1;
            R->stall_no_special = boss7_stats()->specials == 0;
            printf("  f%5d rival AI stalled in InputState_HoldAirAttackCharge (unk456 < 0), specials so far %u\n",
                   g_frame, boss7_stats()->specials);
        }
        if (ph != last_phase || boss_health() != last_health) {
            if (g_verbose || boss_health() != last_health)
                printf("  f%5d phase 0x%02x health %d  %s / %s / %s  rival (%d,%d) player (%d,%d)\n", g_frame, ph,
                       boss_health(), boss_state_name(0), bi.rival_move, bi.rival_input, rx, bi.rival_y >> 8, px,
                       P.y >> 8);
            last_phase = ph;
            last_health = boss_health();
            if (ph >= 0 && ph < 0x40)
                R->phases[ph]++;
        }
        if (bi.rival_box[0].r > bi.rival_box[0].l && bi.rival_box[0].b > bi.rival_box[0].t)
            R->box_geom = 1;
        if (bi.auras > R->auras_max)
            R->auras_max = bi.auras;
        if (bi.pillars > R->pillars_max)
            R->pillars_max = bi.pillars;
        if (bi.flags20C & 2)
            R->defeat_scene = 1;
        if (bi.player_anim_speed == 0x4000)
            R->player_spd = 1;
        if (g_frame - start_frame == 10) {
            R->base_nodes = bi.base_nodes;
            R->base_hidden = bi.base_hidden;
            R->diffuse_start = bi.base_diffuse;
        }
        if (bi.clash && (bi.base_diffuse & 0xffffff) == 0x212121) /* v = 4: (4 << 3 | 4 >> 2) = 0x21 */
            R->dark_clash = 1;
        if (bi.won && boss_cleared() == 0 && bi.base_diffuse == R->diffuse_start && R->dark_clash)
            R->diffuse_restored = 1;
        if (bi.afterimage)
            R->after_posed = 1;
        P.rings = 50;
        g_dcp_world.lives = 3;
        if (cooldown > 0)
            cooldown--;
        if (bi.clash) {
            R->clash = 1;
            R->clash_frames++;
            if (mash && (g_frame & 1))
                keys |= DCP_KEY_A;
        } else if (g_frame - start_frame < 900) {
            if (g_frame - start_frame == 0)
                place_on_floor(sx, chara);
        } else if (boss_health() > 1 && !bi.won && !bi.lost) {
            /* [fight] */
            int ground_rival = (ph == BOSS7_PHASE_IDLE || ph == BOSS7_PHASE_RUN || ph == BOSS7_PHASE_CROUCH) &&
                               bi.rival_box_armed[0] && bi.hurt_timer == 0;
            /* [watch] at 2 hit points the player leaves the rival alone until it has used its special
             * (chosen from the attack tables only now and then), so every attack kind is covered */
            int hold = boss_health() == 2 && boss7_stats()->specials == 0 && !R->stall;
            if (hold && !R->watch) {
                R->watch = 1;
                watch_start = g_frame;
                printf("  f%5d [watch] no special yet: waiting for one\n", g_frame);
            }
            if (hold && g_frame - watch_start > 8000) {
                hold = 0;
                R->stall = R->stall ? R->stall : 2;
            }
            if (!hold && mode == 0 && cooldown == 0 && ground_rival && (P.move_flags & DCP_MF_GROUND) && P.invuln == 0) {
                int side = px < rx ? -1 : 1;
                if (rx < 60)
                    side = 1;
                if (rx > M.right - 60)
                    side = -1;
                place_on_floor(rx + side * 36, chara);
                P.disp_flags = (u16)((P.disp_flags & ~1) | (side > 0 ? 1 : 0));
                mode = 1;
                cooldown = 60;
                try_hits = (int)boss_stats()->hits;
                try_frames = 0;
                assisted = 0;
                R->tries++;
            }
            if (mode == 1) {
                try_frames++;
                if (g_verbose >= 2)
                    printf("        f%5d try %d: player (%d,%d) anim %x mf %x  rival (%d,%d) box0 %d [%d,%d,%d,%d] fl %x\n",
                           g_frame, try_frames, px, P.y >> 8, P.anim, P.move_flags, rx, bi.rival_y >> 8,
                           bi.rival_box_armed[0], bi.rival_box[0].l, bi.rival_box[0].t, bi.rival_box[0].r,
                           bi.rival_box[0].b, bi.rival_box[0].flags);
                if (try_frames >= 3 && try_frames <= 26) /* after landing from the placement */
                    keys |= DCP_KEY_A;
                if (P.anim == 0x30) {
                    s32 d = (bi.rival_x >> 8) - px;
                    if (abs(d) > 2)
                        keys |= d > 0 ? DCP_KEY_RIGHT : DCP_KEY_LEFT;
                    if (fails >= 4 && try_frames >= 8 && try_frames <= 12 && ground_rival && !assisted) {
                        /* assist: put the jumping player just above the rival */
                        P.x = bi.rival_x;
                        P.y = bi.rival_y - (34 << 8);
                        P.vel_x = 0;
                        P.vel_y = 0x200;
                        P.gnd_vel = 0;
                        R->assists++;
                        assisted = 1;
                        if (g_verbose)
                            printf("      f%5d assist: player put above the rival (%d,%d)\n", g_frame, rx,
                                   (bi.rival_y >> 8) - 34);
                    }
                }
                if ((int)boss_stats()->hits > try_hits) {
                    fails = 0;
                    mode = 0;
                    cooldown = 40;
                } else if (cooldown == 0 && (P.move_flags & DCP_MF_GROUND)) {
                    fails++;
                    mode = 0;
                }
            }
        }
        frame(keys);
        if (getenv("DUMPAT") && g_frame == atoi(getenv("DUMPAT")))
            dump_view(getenv("DUMPFILE") ? getenv("DUMPFILE") : "/tmp/view7.ppm");
        if (g_frame - start_frame == 900)
            R->hits_in_miss = (int)boss_stats()->hits;
        boss7_info(&bi);
        if (bi.lost && !R->lost) {
            R->lost = 1;
            /* let the knockback and the rival's skid play out */
            for (n = 0; n < 240; n++)
                frame(0);
            R->lost_knockback = g_hurts >= 0 && boss7_info(&bi) && bi.lost;
            R->lost_state_none = !strcmp(boss_state_name(0), "State_None");
            R->lost_health = boss_health();
            break;
        }
    }
    {
        Boss7Info bi;
        float eye[3], at[3];
        boss7_info(&bi);
        R->won = bi.won;
        R->cleared = boss_cleared();
        R->cleared_hook = g_cleared_hook;
        R->talks = g_talks;
        R->talk_id = g_talk_id;
        R->help_create = g_help_create;
        R->help_destroy = g_help_destroy;
        R->help_idx = g_help_idx;
        R->music11 = g_music11;
        R->hurts = g_hurts;
        R->rival_chara = bi.rival_chara;
        R->st = *boss7_stats();
        R->hits = boss_stats()->hits;
        R->exc6 = g_exc_seen[6];
        R->exc7 = g_exc_seen[7];
        R->bright = g_bright_max;
        R->blend = g_blend_max;
        R->frames = g_frame - start_frame;
        R->ring_bonus_ok = g_obj_stage.ring_bonus == P.rings * 1000;
        R->sfx_hit = g_sfx[0x7b];
        R->tris = boss_draw(0);
        boss_camera_eye(eye, at);
        R->cam_ok = isfinite(eye[0]) && isfinite(eye[2]) && fabsf(eye[2] - at[2]) > 10.0f;
        printf("  end f%d (%d after the start): phase 0x%x health %d/%d won %d lost %d cleared %d; %s / %s / %s\n",
               g_frame, R->frames, boss_phase(), boss_health(), hp_max, bi.won, bi.lost, boss_cleared(),
               boss_state_name(0), boss_state_name(1), boss_state_name(2));
        printf("  rival %s: decides %u attacks %u (spindash %u, air %u, special %u), jumps %u backhops %u crouches %u "
               "evades %u jump-overs %u repositions %u\n",
               bi.rival_chara ? "Blaze" : "Sonic", R->st.decides, R->st.attacks, R->st.spindashes, R->st.air_attacks,
               R->st.specials, R->st.jumps, R->st.backhops, R->st.crouches, R->st.evades, R->st.jump_overs,
               R->st.repositions);
        printf("  hits %u, rival hurts %u, bounces %u, pushes %u, pillars %u (max %d alive), pull frames %u, "
               "after-image frames %u, auras %u; player tries %d assists %d hurts %d\n",
               R->hits, R->st.hurts, R->st.clash_bounces, R->st.pushes, R->st.pillars, R->pillars_max,
               R->st.pull_frames, R->st.afterimage_frames, R->st.auras, R->tries, R->assists, R->hurts);
        printf("  clash %d frames: presses player %u rival %u, balance %d; talks %d (id 0x%x), help %d/%d (idx %d), "
               "music 0x11 %d; exc_bg 6:%d 7:%d bright %d blend %d; effects %u fx3d %u; camera eye (%.1f,%.1f,%.1f) "
               "at (%.1f,%.1f,%.1f); draw %d\n",
               R->clash_frames, R->st.mash_player, R->st.mash_rival, bi.balance, R->talks, R->talk_id, R->help_create,
               R->help_destroy, R->help_idx, R->music11, R->exc6, R->exc7, R->bright, R->blend,
               boss_stats()->effects_spawned, R->st.fx3d, eye[0], eye[1], eye[2], at[0], at[1], at[2], R->tris);
    }
    boss_shutdown();
}

int main(int argc, char **argv) {
    long size;
    char path[512];
    s16 *trig;
    u8 *phys;
    int story = getenv("STORY") ? atoi(getenv("STORY")) : 2, c;
    int lvl = getenv("LEVEL") ? atoi(getenv("LEVEL")) : 0;
    int hp = lvl == 1 ? 8 : 6;
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
    g_obj_hooks.log = hk_obj_log;
    g_dcp_hooks.atkdef = hk_atkdef;
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
    g_boss_hooks.music_fade = hk_music_fade;
    g_boss_hooks.music_volume = hk_music_volume;
    g_boss_hooks.help = hk_help;
    g_boss_hooks.fx3d = hk_fx3d;
    boss7_set_talk_hook(hk_talk, NULL);
    boss_set_paths(argv[3], argv[4]);
    boss_set_difficulty(lvl);
    for (c = 0; c < 2; c++) {
        RunResult R, L;
        if (story != 2 && story != c)
            continue;
        printf("== story character %s (rival %s), %d hit points, seed %s\n", c ? "Blaze" : "Sonic", c ? "Sonic" : "Blaze",
               hp, getenv("SEED") ? getenv("SEED") : "0x12345678");
        g_fx3d_boost_a = g_fx3d_boost_a2_sb = g_fx3d_boost_a2_bb = g_fx3d_fire = g_fx3d_spin = g_fx3d_other = g_fx3d_far = 0;
        run_fight(argv[1], c, 1, &R);
        CHECK(R.init_ok, "boss_init on z73 (bs7_base/sky, bs7.dca/.dcv, rival models, son.dca/.dcv, D6/D7 packages, "
                         "timing tables, boss7bac.bin, effects)");
        if (!R.init_ok)
            return 1;
        CHECK(R.hp == hp, "Boss7_Create: %d hit points", hp);
        CHECK(R.waits, "the boss waits for the title card");
        CHECK(R.rival_chara == (c == 0 ? 1 : 0), "Boss7_Create / Boss7PartD5_Create character branch: rival %s",
              R.rival_chara ? "Blaze (blz.nsbmd)" : "Sonic (son.nsbmd)");
        CHECK(R.hits_in_miss == 0, "[miss] standing still never damages the rival");
        CHECK(R.st.spindashes > 0 && R.st.air_attacks > 0 && (R.st.specials > 0 || R.stall_no_special) &&
                  R.st.jumps > 0,
              "the rival's AI uses spin dash / air attack / special / jump (%u, %u, %u, %u)%s", R.st.spindashes,
              R.st.air_attacks, R.st.specials, R.st.jumps,
              R.stall_no_special ? " - no special: the DS HoldAirAttackCharge stall came first" : "");
        if (R.stall_no_special)
            CHECK(R.stall && R.st.pillars == 0, "[stall] no special before the HoldAirAttackCharge stall: no Boss7PartD6");
        else if (c == 0)
            CHECK(R.st.pillars > 0 && R.pillars_max >= 1, "Blaze's special: fire pillars (Boss7PartD6, %u, %d at once)",
                  R.st.pillars, R.pillars_max);
        else
            CHECK(R.st.pillars > 0 && R.st.pull_frames > 0 && R.st.afterimage_frames > 0,
                  "Sonic's special: tornado (Boss7PartD6, %u) pulling the player (%u frames), after-image dash (%u)",
                  R.st.pillars, R.st.pull_frames, R.st.afterimage_frames);
        CHECK(R.box_geom, "the rival's hit boxes come from the son.bac command-7 table");
        CHECK(R.base_nodes > 0 && R.base_hidden > 0, "bs7_base visibility (bs7.nsbva anim %d, %s): %d of %d nodes hidden",
              c == 0 ? 4 : 3, c == 0 ? "Nega's pod" : "Eggman's pod", R.base_hidden, R.base_nodes);
        CHECK(R.dark_clash && R.diffuse_restored,
              "clash: the arena takes the global material colour (dark grey), restored at the rival's exit");
        if (c == 1)
            CHECK(R.after_posed, "Sonic's dash after-image is posed with its own light set");
        CHECK((int)R.hits == hp - 1, "%d hits beat the rival (health 1)", hp - 1);
        CHECK(R.phases[0x24] && R.phases[0x25] && R.phases[0x28] && R.phases[0x29] && R.phases[0x2a] &&
                  R.phases[0x2b] && (R.phases[0x2c] || R.stall_no_special) && R.phases[0x2d],
              "phases idle/run/jump/spindash/roll/air attack/%shurt all visited", R.stall_no_special ? "" : "special/");
        CHECK(g_fade3c >= 1 && g_vol20 >= 1, "BGM: ClashFadeOutBGM fades over 0x3c (x%d), defeat scene moves it to 0x20 over 0xb4 (x%d)",
              g_fade3c, g_vol20);
        CHECK(R.defeat_scene && R.talks == 1 && R.talk_id == (c == 0 ? 0xa : 0xb),
              "defeat scene, then TalkDemo 0x%x", c == 0 ? 0xa : 0xb);
        CHECK(R.clash && R.auras_max == 2 && R.help_create == 1 && R.help_destroy == 1 && R.help_idx == 0 &&
                  R.music11 == 1,
              "clash: two auras, BossHelp_Create(0) / Destroy, clash music 0x11");
        CHECK(R.st.mash_player > 0 && R.st.mash_rival > 0, "both sides mash (player %u, rival %u)", R.st.mash_player,
              R.st.mash_rival);
        CHECK(R.player_spd, "the player's clash animation runs at 4x (boss7_info.player_anim_speed)");
        CHECK(g_fx3d_other == 0 && g_fx3d_far == 0 && g_fx3d_spin > 0 && g_fx3d_boost_a + g_fx3d_boost_a2_sb + g_fx3d_boost_a2_bb > 0 &&
                  (c == 0 ? g_fx3d_fire > 0 : g_fx3d_fire == 0),
              "3D BAC effects through g_boss_hooks.fx3d at the rival (world coordinates; fire only from Blaze's roll): boost_a %d, sb/bb boost_a2 %d/%d, "
              "fire_01 %d, kemuri_spin02 %d",
              g_fx3d_boost_a, g_fx3d_boost_a2_sb, g_fx3d_boost_a2_bb, g_fx3d_fire, g_fx3d_spin);
        CHECK(R.won && R.exc6 > 0 && R.exc7 > 0 && R.bright == 16 && R.blend >= 11,
              "clash won: health 0, win cut-ins (exc_bg 6/7), white flashes (bright %d, 3D blend %d)", R.bright, R.blend);
        CHECK(R.cleared && R.cleared_hook == 1 && R.ring_bonus_ok, "the rival jumps away -> Stage_OnBossCleared once, ring bonus");
        CHECK(R.tris > 0 && R.cam_ok, "models pose and project through the boss camera (%d vertices)", R.tris);
        CHECK(g_boss_matanim_draws > 0, "tornado / flame / aura effects drawn with their material and pattern "
              "animations (%u draws)", (unsigned)g_boss_matanim_draws);
        printf("== [lose] story character %s: no mashing in the clash\n", c ? "Blaze" : "Sonic");
        run_fight(argv[1], c, 0, &L);
        CHECK(L.clash && L.lost && L.lost_state_none && L.lost_health == 1 && !L.cleared,
              "[lose] the rival wins the clash: player knocked back, Boss7_State_None, health stays 1, no stage clear");
        CHECK(L.st.mash_player == 0 && L.st.mash_rival > 0, "[lose] only the rival mashed (%u)", L.st.mash_rival);
    }
    printf("%d checks, %d failed\n", g_checks, g_fail);
    return g_fail ? 1 : 0;
}
