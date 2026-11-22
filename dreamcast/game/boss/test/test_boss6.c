/*
 * Host test of the native boss module: Boss6 (Altitude Limit act 3, stage z63)
 * on the real z63 level with the real player module, the real ObjRect code and
 * the converted bs6 models and animations.
 *
 * Usage: test_boss6 <z63/level.bin> <build/dreamcast/player> <models dir> <boss data dir> [-v|-vv]
 *
 * As in test_boss.c, objects_init is not called: the test registers the
 * player's two boxes itself (son.bac boxes of the standing and ball poses).
 * The player's non-looping animations end (DCP_DF_ANIM_DONE, set on the DS by
 * the player model's "ended" bit) after the length of the son.nsbca animation
 * they map to (anim 0xf com_dmg_b: 56 frames, 0x3c com_fw: 60), else 40 frames.
 *
 * Scripted player:
 *   [miss]   900 frames standing still at the start: the boss keeps its health.
 *   [fight]  pick: while the pick is pulled out of the deck (box 0 at "body_core" armed) the
 *            player is put on the floor 30 px beside the core and jumps into it (DamageFlip:
 *            the boss turns round).  hops: after a landing, once the core has sunk (box 1 at
 *            y >= 445), the player is put 40 px beside it, jumps and comes down onto box 1 from
 *            above (box 5, which hurts, lies below it).  Once a stun was seen, the player jumps
 *            away during the pick swing.  tilt: the first tilt is left alone (the player slides
 *            off the low end: edge knockback, falls off the deck, respawns); in the later ones he
 *            mashes A every 4 frames (gPad.trg) and stays on the deck.
 *   [defeat] runs the defeat sequence to the stage-clear request.
 * Env: SEED, LEVEL (1: 8 hit points), BOSSFRAMES, NOMASH / MASHALL, DUMPAT / DUMPFILE (host debug
 * view), TRACEP=<frame> (player trace), PICKDX / LANDDX / LANDY / SWINGJUMP (script tuning).
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
static int g_hurts, g_navi, g_cleared_hook, g_music_stop, g_ring_flags, g_pinch_music;
static int g_help_create, g_help_destroy, g_help_shown = -1, g_help_frames;
static int g_sfx[256];

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
/* animation timing: non-looping player animations end after their 3D animation length */
static int s_anim_start, s_anim_len = 40;
static void hk_anim(void *ctx, DcPlayer *p, s32 anim) {
    (void)ctx;
    (void)p;
    s_anim_start = g_frame;
    s_anim_len = anim == 0xf ? 56 : anim == 0x3c ? 60 : 40;
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
            printf("    f%5d player hurt at x=%d (rings %d)  boss %s phase %#x\n", g_frame, a, b, boss_state_name(0),
                   boss_phase());
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
static void hk_help(void *ctx, s32 kind, s32 a, s32 b) {
    (void)ctx;
    (void)b;
    if (kind == BOSS_HELP_CREATE) {
        g_help_create++;
        g_help_shown = a;
    } else if (kind == BOSS_HELP_DESTROY) {
        g_help_destroy++;
        g_help_shown = -1;
    }
}

extern unsigned char *g_boss_debug_fb;
/* host debug view: projected boss vertices + the player (x, -y, z) as a white cross */
static void dump_view(const char *path) {
    static unsigned char fb[640 * 480 * 3];
    dcm_camera_t cam;
    float v[3], o[4], e[3];
    int i, c, x, y;
    FILE *f;
    memset(fb, 0, sizeof(fb));
    g_boss_debug_fb = fb;
    boss_draw(0);
    g_boss_debug_fb = NULL;
    boss_camera(&cam);
    v[0] = P.x / 256.0f;
    v[1] = -P.y / 256.0f;
    v[2] = P.z / 256.0f;
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
    if (g_frame - s_anim_start >= s_anim_len && !(P.disp_flags & DCP_DF_LOOP))
        P.disp_flags |= DCP_DF_ANIM_DONE;
    dcp_player_frame(&P, held, pressed, released);
    player_collide();
    boss_update(&P);
    if (g_help_shown >= 0)
        g_help_frames++;
    g_frame++;
}

static s32 floor_y(s32 x) {
    s32 yy;
    for (yy = 200; yy < M.bottom; yy++)
        if (rlvl_tile_height(&M, yy, x, 1 | 0x80, NULL, NULL) != 0)
            return yy;
    return M.bottom;
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

static void place_on_floor(s32 x) {
    s16 rings = P.rings;
    u16 mu = P.map_up, md = P.map_down, mr = P.map_right, ml = P.map_left, ma = P.map_a, mb = P.map_b;
    if (x < 16)
        x = 16;
    if (x > 816)
        x = 816;
    memset(&P, 0, sizeof(P));
    dcp_player_create(&P, 0, 0, x, floor_y(x) - 14);
    P.rings = rings;
    P.pf &= ~0x600000u;
    P.map_up = mu;
    P.map_down = md;
    P.map_right = mr;
    P.map_left = ml;
    P.map_a = ma;
    P.map_b = mb;
    s_anim_start = g_frame;
}

int main(int argc, char **argv) {
    long size;
    char path[512];
    s16 *trig;
    u8 *phys;
    const rlvl_object_t *ps;
    s32 sx = 224, sy = 400;
    int r, n, last_phase = -1, last_health = -1, max_frames, start_frame, phases_seen[0x40] = {0};
    int hits_in_miss = 0, hurts_in_miss = 0, cooldown = 0, mode = 0, side = -1, tries_pick = 0, tries_land = 0, dodges = 0;
    int max_bullets = 0, no_mash = getenv("NOMASH") != NULL, mash_all = getenv("MASHALL") != NULL;
    int deaths = 0, deaths_tilt1 = 0, was_dead = 0, knock_tilt1 = 0;
    int pick_dx = getenv("PICKDX") ? atoi(getenv("PICKDX")) : 30, land_dx = getenv("LANDDX") ? atoi(getenv("LANDDX")) : 40;
    int land_y = getenv("LANDY") ? atoi(getenv("LANDY")) : 445;
    int state_age = 0, swing_jump = getenv("SWINGJUMP") ? atoi(getenv("SWINGJUMP")) : 14;
    s32 tx = 0, min_tilt_x = 0x7fffffff, max_tilt_x = -1;
    char last_state[64] = "";
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
    g_dcp_world.zone = 5;
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
    printf("z63: %dx%d px, stage %d, PlayerStart (%d,%d), boss object 0x3a %s, floor y at x=16/%d/416/816: %d/%d/%d/%d\n",
           M.right, M.bottom, ((const rlvl_header_t *)g_level)->stage, sx, sy, find_obj(0x3a) ? "found" : "missing", sx,
           floor_y(16), floor_y(sx), floor_y(416), floor_y(816));
    {
        s32 x, x0 = -1, x1 = -1;
        for (x = 0; x < M.right; x++)
            if (floor_y(x) == 512) {
                if (x0 < 0)
                    x0 = x;
                x1 = x;
            }
        printf("deck floor (y 512) from x=%d to x=%d\n", x0, x1);
    }
    dcp_player_create(&P, 0, 0, sx, sy);
    P.pf &= ~0x600000u;
    g_obj_hooks.log = hk_obj_log;
    g_dcp_hooks.atkdef = hk_atkdef;
    g_dcp_hooks.anim = hk_anim;
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
    g_boss_hooks.help = hk_help;
    boss_set_paths(argv[3], argv[4]);
    boss_set_difficulty(getenv("LEVEL") ? atoi(getenv("LEVEL")) : 0);
    r = boss_init(((const rlvl_header_t *)g_level)->stage, g_level, &P, 0);
    printf("boss_init: %d  health %d/%d phase %#x\n", r, boss_health(), boss_health_max(), boss_phase());
    CHECK(r == 0, "boss_init on z63 (10 bs6 models, bs6.dca, pieces, timing tables, effects)");
    if (r != 0)
        return 1;
    for (n = 0; n < 60; n++)
        frame(0);
    boss_start();
    start_frame = g_frame;
    max_frames = getenv("BOSSFRAMES") ? atoi(getenv("BOSSFRAMES")) : 40000;
    while (g_frame < max_frames && !boss_cleared()) {
        Boss6Info bi;
        int ph = boss_phase();
        u16 keys = 0;
        const char *st = boss_state_name(0);
        int grounded = (P.move_flags & DCP_MF_GROUND) != 0;
        boss6_info(&bi);
        if (g_verbose && (ph != last_phase || boss_health() != last_health || strcmp(last_state, st))) {
            int i;
            printf("  f%5d phase %#x hp %d %-22s boss (%d,%d) core (%d,%d) pelvis (%d,%d) turned %d player (%d,%d) %s armed:",
                   g_frame, ph, boss_health(), st, bi.x >> 8, bi.y >> 8, bi.core_x, bi.core_y, bi.pelvis_x, bi.pelvis_y,
                   bi.turned, P.x >> 8, P.y >> 8, boss_state_name(1));
            for (i = 0; i < 11; i++)
                if (bi.armed[i])
                    printf(" %d", i);
            printf("\n");
        }
        if (ph != last_phase || boss_health() != last_health) {
            if (!g_verbose)
                printf("  f%5d phase %#x hp %d %s\n", g_frame, ph, boss_health(), st);
            if (ph >= 0 && ph < 0x40)
                phases_seen[ph]++;
        }
        last_phase = ph;
        last_health = boss_health();
        if (strcmp(last_state, st))
            state_age = 0;
        else
            state_age++;
        strncpy(last_state, st, sizeof(last_state) - 1);
        if (bi.bullets_alive > max_bullets)
            max_bullets = bi.bullets_alive;
        if ((P.pf & DCP_PF_DEAD) && !was_dead) {
            deaths++;
            if (ph == BOSS6_PHASE_TILT && bi.tilts == 1)
                deaths_tilt1++;
            if (g_verbose)
                printf("    f%5d player died at (%d,%d)\n", g_frame, P.x >> 8, P.y >> 8);
        }
        was_dead = (P.pf & DCP_PF_DEAD) != 0;
        if (bi.tilts == 1)
            knock_tilt1 = (int)bi.edge_knockbacks;
        if (ph == BOSS6_PHASE_TILT && !strcmp(st, "State_Tilting") && (bi.tilts > 1 || mash_all)) {
            if (P.x < min_tilt_x)
                min_tilt_x = P.x;
            if (P.x > max_tilt_x)
                max_tilt_x = P.x;
        }
        P.rings = 50;
        g_dcp_world.lives = 3;
        if (cooldown > 0)
            cooldown--;
        if (g_frame - start_frame < 900) {
            if (g_frame - start_frame == 0)
                place_on_floor(sx);
        } else if (!boss_defeated()) {
            int stunned = P.state == NULL || !strcmp(boss_state_name(1), "State_PlayerStunned");
            /* tilt: mash A (a new press every 4 frames) while the deck is tilted */
            if (ph == BOSS6_PHASE_TILT && (!strcmp(st, "State_Tilting") || !strcmp(st, "State_TiltReturn"))) {
                /* the first tilt is left alone: the player slides off the low end (edge knockback) */
                if (!no_mash && (bi.tilts > 1 || mash_all) && (g_frame & 3) == 0)
                    keys |= DCP_KEY_A;
            } else if (mode == 0 && cooldown == 0 && grounded && !stunned && P.anim != 0xf && P.map_a != 0) {
                /* A: pick pulled out of the deck -> jump into the core (box 0) from beside it */
                if (!strcmp(st, "State_PickPullOut") && bi.armed[0] && state_age >= 6) {
                    side = (P.x >> 8) < bi.core_x ? -1 : 1;
                    place_on_floor(bi.core_x + side * pick_dx);
                    tx = bi.core_x;
                    mode = 1;
                    cooldown = 70;
                    tries_pick++;
                }
                /* B: the boss has just landed from a hop -> jump into its core (box 1) */
                else if (!strcmp(st, "State_LandShake") && bi.armed[1] && bi.box[1].y >= land_y) {
                    const ObjBox *b = &bi.box[1];
                    s32 cx = (b->flags & 1) ? b->x - (b->l + b->r) / 2 : b->x + (b->l + b->r) / 2;
                    side = (P.x >> 8) < cx ? -1 : 1;
                    place_on_floor(cx + side * land_dx);
                    tx = cx;
                    mode = 2;
                    cooldown = 70;
                    tries_land++;
                }
                /* C: once a stun was seen, jump away from the pick before it hits the deck (the swing
                 * animation is 40 frames; the stun hits grounded players only) */
                else if (bi.stuns > 0 && !strcmp(st, "State_PickSwing") && state_age >= swing_jump) {
                    side = (P.x >> 8) < bi.x >> 8 ? -1 : 1;
                    mode = 3;
                    cooldown = 60;
                    dodges++;
                }
            }
            if (mode == 3) {
                if (cooldown >= 35)
                    keys |= DCP_KEY_A;
                keys |= side < 0 ? DCP_KEY_LEFT : DCP_KEY_RIGHT;
                if (cooldown == 0 || (cooldown < 50 && grounded))
                    mode = 0;
            }
            if (mode == 1 || mode == 2) {
                if (g_verbose >= 2) {
                    int b = mode == 1 ? 0 : 1;
                    printf("        f%5d try%d player (%d,%d) anim %x vel (%d,%d) atk %x/%x box %d..%d,%d..%d | box%d (%d,%d) %d..%d,%d..%d fl %x armed %d\n",
                           g_frame, mode, P.x >> 8, P.y >> 8, P.anim, P.vel_x, P.vel_y, s_atk.atk, s_atk.flags,
                           s_atk.x + s_atk.l, s_atk.x + s_atk.r, s_atk.y + s_atk.t, s_atk.y + s_atk.b, b, bi.box[b].x,
                           bi.box[b].y, bi.box[b].l, bi.box[b].r, bi.box[b].t, bi.box[b].b, bi.box[b].flags, bi.armed[b]);
                }
                if (cooldown >= 45 && cooldown <= 67)
                    keys |= DCP_KEY_A; /* full jump (a fresh press two frames after the placement) */
                if (!grounded) {
                    s32 d = tx - (P.x >> 8);
                    if (d > 2)
                        keys |= DCP_KEY_RIGHT;
                    else if (d < -2)
                        keys |= DCP_KEY_LEFT;
                }
                if (cooldown == 0 || (cooldown < 60 && grounded))
                    mode = 0;
            }
        }
        if (getenv("TRACEP") && g_frame >= atoi(getenv("TRACEP")) && g_frame < atoi(getenv("TRACEP")) + 300 && (g_frame % 5) == 0)
            printf("      f%5d player (%d,%d) vel (%d,%d) gnd %d anim %x pf %x obj %x state %s maps %x/%x/%x/%x/%x/%x\n",
                   g_frame, P.x >> 8, P.y >> 8, P.vel_x, P.vel_y, P.gnd_vel, P.anim, P.pf, P.obj_flags,
                   boss_state_name(1), P.map_right, P.map_left, P.map_up, P.map_down, P.map_a, P.map_b);
        frame(keys);
        if (getenv("DUMPAT") && g_frame == atoi(getenv("DUMPAT")))
            dump_view(getenv("DUMPFILE") ? getenv("DUMPFILE") : "/tmp/view6.ppm");
        if (g_frame - start_frame == 900) {
            hits_in_miss = (int)boss_stats()->hits;
            hurts_in_miss = g_hurts;
            printf("  [miss] 900 frames standing still: %d hurts, %d hits, health %d\n", hurts_in_miss, hits_in_miss,
                   boss_health());
        }
    }
    {
        const BossStats *st = boss_stats();
        Boss6Info bi;
        float eye[3], at[3];
        int tris;
        boss6_info(&bi);
        printf("  end f%d: phase %#x health %d cleared %d; state %s\n", g_frame, boss_phase(), boss_health(),
               boss_cleared(), boss_state_name(0));
        printf("  attacks %u (bullets %u, pick %u, tilt %u, edge jumps %u); hits %u (pick %u, landing %u); turns %u\n",
               st->attacks, bi.attacks_d4, bi.attacks_pick, bi.attacks_tilt, bi.attacks_jump, st->hits, bi.hits_pick,
               bi.hits_jump, bi.turns);
        printf("  hops %u, landings %u, bullets %u (max %d alive, %u hit the player), stuns %u, tilts %u (mash presses %u, "
               "edge knockbacks %u, player x %d..%d), pieces plate/wind/shoot/bomb %u/%u/%u/%u\n",
               bi.jumps, bi.landings, bi.bullets, max_bullets, bi.bullet_hits, bi.stuns, bi.tilts, bi.mash_presses,
               bi.edge_knockbacks, min_tilt_x >> 8, max_tilt_x >> 8, bi.pieces[0], bi.pieces[1], bi.pieces[2],
               bi.pieces[3]);
        printf("  player: pick tries %d, landing tries %d, dodges %d, hurts %d, deaths %d (%d in the first tilt); help "
               "create/destroy %d/%d (%d frames)\n",
               tries_pick, tries_land, dodges, g_hurts, deaths, deaths_tilt1, g_help_create, g_help_destroy, g_help_frames);
        printf("  effects %u; navi %d; sfx 0x83 %d 0x86 %d 0x87 %d 0x8d %d; ring_bonus %d; exc_bg %d bright %d\n",
               st->effects_spawned, g_navi, g_sfx[0x83], g_sfx[0x86], g_sfx[0x87], g_sfx[0x8d], g_obj_stage.ring_bonus,
               boss_exc_background(), boss_brightness());
        boss_camera_eye(eye, at);
        printf("  camera eye (%.1f,%.1f,%.1f) at (%.1f,%.1f,%.1f)\n", eye[0], eye[1], eye[2], at[0], at[1], at[2]);
        CHECK(hits_in_miss == 0, "[miss] standing still never damages the boss (%d hurts)", hurts_in_miss);
        CHECK(bi.attacks_d4 > 0 && bi.attacks_pick > 0 && bi.attacks_tilt > 0 && bi.attacks_jump > 0,
              "bullets / pick / tilt / edge jumps all chosen (%u, %u, %u, %u)", bi.attacks_d4, bi.attacks_pick,
              bi.attacks_tilt, bi.attacks_jump);
        CHECK(bi.bullets > 0 && bi.bullet_hits > 0 && bi.pieces[2] > 0 && bi.pieces[3] > 0,
              "Boss6PartD4 bullets fired (%u) and hit the player (%u: eff_shoot / eff_shoot_bomb pieces)", bi.bullets,
              bi.bullet_hits);
        CHECK(bi.stuns > 0, "pick impact stuns a grounded player (%u)", bi.stuns);
        CHECK(bi.tilts > 0 && bi.mash_presses > 0 && bi.pieces[0] > 0 && bi.pieces[1] > 0,
              "deck tilt: A/B mashing read from the pad (%u presses), plates and wind debris", bi.mash_presses);
        if (!mash_all && !no_mash) {
            CHECK(knock_tilt1 >= 1 && deaths_tilt1 >= knock_tilt1,
                  "first tilt (no mashing): the player slides off the low end, edge knockback, falls off the deck "
                  "(%d knockbacks, %d deaths: he respawns on the tilted deck)", knock_tilt1, deaths_tilt1);
            CHECK(bi.tilts < 2 || ((int)bi.edge_knockbacks == knock_tilt1 && min_tilt_x >= 0x800 && max_tilt_x <= 0x33800),
                  "mashing A every 4 frames keeps the player on the deck in the later tilts (x %d..%d)", min_tilt_x >> 8,
                  max_tilt_x >> 8);
        }
        CHECK(g_help_create == (int)bi.tilts && g_help_destroy == (int)bi.tilts && boss_help() < 0,
              "BossHelp hook: shown and removed once per tilt (%d/%d)", g_help_create, g_help_destroy);
        CHECK(bi.hits_pick > 0 && bi.hits_jump > 0 && bi.turns > 0,
              "core hit after a pick (DamageFlip, turns round) and after a landing (Damage)");
        CHECK((int)st->hits == boss_health_max(), "%d hits on the core", boss_health_max());
        CHECK(phases_seen[0x1c] && phases_seen[0x1e] && phases_seen[0x1f] && phases_seen[0x20] && phases_seen[0x21] &&
                  phases_seen[0x22] && phases_seen[0x23],
              "phases move/bullets/pick/tilt/edge jump/hurt/defeat all visited");
        CHECK(g_pinch_music == 1, "pinch music (Sound_PlaySeq 0xf) at health 2");
        CHECK(boss_defeated() && boss_cleared() && g_cleared_hook == 1, "defeat sequence reached Stage_OnBossCleared");
        CHECK(g_music_stop == 1 && (g_ring_flags & 1), "defeat stops the music and sets the ring manager flag");
        CHECK(g_obj_stage.ring_bonus == P.rings * 1000, "ring bonus = rings x 1000 (%d)", g_obj_stage.ring_bonus);
        CHECK(st->effects_spawned > 0 && g_sfx[0x86] > 0 && g_sfx[0x83] > 0, "land / pick / hit effects, impact and shot sounds");
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
