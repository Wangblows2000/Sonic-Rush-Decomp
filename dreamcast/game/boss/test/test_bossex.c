/*
 * Host test of the Extra Zone boss (boss_extra*.c): the whole five-round fight on the converted data
 * (bossex.bin, zex.bin, sprites, bsex / e_son / e_blz models and animations) through the public API,
 * driven by a scripted pad (InputState) only - no object is moved by the test.
 *
 * Usage: test_bossex <build/dreamcast/player> <models dir> <boss/extra dir> [-v]
 *
 * Scripted pad (reads the object pool to decide; everything goes through boss_extra_run_frame):
 *   - dodge: every harmful hit box (group 0 attackers: beams, missiles, claws, catch hand, sword, bombs,
 *     meteors, the bosses' bodies) is predicted 10 frames ahead with its velocity; each frame the pad
 *     direction with the best "goal distance + danger" score is held.
 *   - Super Sonic (rounds 0, 2, 4): beam units orbit the player and homing beams come down at him; when
 *     one of them lies in a dash direction that points at the boss's hit box (8-way, +-25 degrees),
 *     the bot dashes (A) into it, so it is deflected into the boss.  Meteors in the round 0 field are
 *     dodged (or dashed through when they block).
 *   - Burning Blaze (rounds 1, 3): lines up with the hand boss's hit box to its left and fires (A: the
 *     special shot flies right).
 *   - rings: when nothing is to be done, the nearest loose / placed ring is the goal.
 * Runs:
 *   [clear]  default seed and difficulty: the fight to BOSS_EXTRA_CLEARED, all five rounds, both bosses,
 *            the character switch four times, the result (time / rings / lives) and gBossStatus.
 *   [seeds]  three more seeds and the hard difficulty: cleared.
 *   [fail]   1 life each and an idle pad: rings run out, BOSS_EXTRA_FAILED (game over), no leak of the
 *            object pool after shutdown, re-init works.
 *   [pause]  START sets boss_extra_paused(), the fight does not advance until boss_extra_resume().
 *   [render] host frames of the single 640x480 view (title, round 0 field, weapon boss, hand boss,
 *            character switch, VS cut-in) into $DUMPDIR (default /tmp/bossex) as PPM.
 * Env: SEED, LEVEL, FRAMES (frame budget per run), DUMPDIR, NORENDER, ONLY=clear|seeds|fail|pause,
 *      DUMPAT=<frame> (one more frame), TRACEHURT (what hit the player), MARGIN / BLZDX / SONDX / RINGLOW (bot).
 * The character-switch frame is also rendered with the DS cameras (slot 1 = top screen, slot 2 = bottom
 * screen: *_switch_ds_slot1/2.ppm) next to the composed single view (*_switch.ppm).
 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../boss.h"
#include "../boss_extra_int.h"

void boss_extra_host_target(u8 *rgb);
extern int g_ex_host_switch_slot;
int boss_extra_host_items(void);

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

/* ------------------------------------------------------------------ hooks */
static int g_sfx_count, g_voice_count, g_music[4], g_music_n, g_pause_hook;
static void h_sfx(void *c, s32 id) { (void)c; (void)id; g_sfx_count++; }
static void h_voice(void *c, s32 id) { (void)c; (void)id; g_voice_count++; }
static void h_music(void *c, s32 seq) {
    (void)c;
    if (g_music_n < 4)
        g_music[g_music_n] = seq;
    g_music_n++;
}
static void h_pause(void *c) { (void)c; g_pause_hook++; }

/* ---------------------------------------------------------------- render */
static u8 g_rgb[640 * 480 * 3];
static const char *g_dumpdir = "/tmp/bossex";
static int g_render = 1;
static void dump(const char *name) {
    char path[512];
    FILE *f;
    if (!g_render)
        return;
    boss_extra_draw(0);
    snprintf(path, sizeof(path), "%s/%s.ppm", g_dumpdir, name);
    f = fopen(path, "wb");
    if (!f)
        return;
    fprintf(f, "P6\n640 480\n255\n");
    fwrite(g_rgb, 1, sizeof(g_rgb), f);
    fclose(f);
    printf("  [render] %s (%d models)\n", path, boss_extra_host_items());
}
static int frame_nonblack(void) {
    int i, n = 0;
    for (i = 0; i < 640 * 480 * 3; i += 3)
        n += (g_rgb[i] | g_rgb[i + 1] | g_rgb[i + 2]) != 0;
    return n;
}

/* ---------------------------------------------------------------- the bot */
typedef struct { float x, y, hx, hy, vx, vy, r; int rot; const ExObj *o; } Box;
static Box g_danger[160];
static int g_ndanger;
static float g_margin = 20, g_blz_dx = 130, g_son_dx = 110, g_ring_low = 35;

static float fxf(s32 v) { return v / 4096.0f; }
static int coll_active(const ExCollider *c) {
    return c != NULL && c->owner != NULL && !(c->unk8 & 0x200) && !(c->owner->unk14 & (8 | 0x1000));
}
static Box coll_box(const ExCollider *c) {
    Box b;
    b.x = fxf(c->unk28[0] + c->unkC[0]);
    b.y = fxf(c->unk28[1] + c->unkC[1]);
    b.hx = fxf(c->unk18[0]);
    b.hy = fxf(c->unk18[1]);
    b.vx = fxf(c->owner->unk50);
    b.vy = fxf(c->owner->unk54);
    b.rot = (u16)(c->unk24 + c->unk34) != 0;
    b.r = sqrtf(b.hx * b.hx + b.hy * b.hy);
    b.o = c->owner;
    return b;
}
static void gather_danger(void) {
    int i, k;
    g_ndanger = 0;
    for (i = 0; i < 0x2e; i++) {
        ExObj *o = gExtraBossPlayerManager->unkBC[i];
        if (o == NULL || o->unk4 == 0)
            continue;
        for (k = 0; k < 5; k++) {
            const ExCollider *c = o->unk9C[k];
            if (!coll_active(c) || (c->unk8 & 0x101) != 0x101 || g_ndanger >= 160)
                continue;
            g_danger[g_ndanger++] = coll_box(c);
        }
    }
}
/* danger of the player at (x, y) at frame t (half size ph) */
static float danger_at(float x, float y, int t, float ph) {
    float d = 0;
    int i;
    for (i = 0; i < g_ndanger; i++) {
        const Box *b = &g_danger[i];
        float bx = b->x + b->vx * t, by = b->y + b->vy * t, ex, ey;
        if (b->rot) {
            float dist = sqrtf((x - bx) * (x - bx) + (y - by) * (y - by)) - b->r - ph;
            ex = dist;
            ey = dist;
        } else {
            ex = fabsf(x - bx) - b->hx - ph;
            ey = fabsf(y - by) - b->hy - ph;
        }
        {
            float m = ex > ey ? ex : ey;
            if (m < 0)
                d += 4000 + -m * 50;
            else if (m < g_margin)
                d += (g_margin - m) * (g_margin - m) * 6;
        }
    }
    return d;
}

static const int DIRX[9] = {0, 1, 1, 0, -1, -1, -1, 0, 1}, DIRY[9] = {0, 0, 1, 1, 1, 0, -1, -1, -1};
static u32 dir_keys(int d) {
    u32 k = 0;
    if (DIRX[d] > 0)
        k |= IN_RIGHT;
    if (DIRX[d] < 0)
        k |= IN_LEFT;
    if (DIRY[d] > 0)
        k |= IN_DOWN;
    if (DIRY[d] < 0)
        k |= IN_UP;
    return k;
}
typedef struct {
    int dashes, shots, frames_threat;
    u32 prev;
    int cool;
} Bot;

static int boss_target(float *tx, float *ty) {
    int i, k, n = 0;
    float sx = 0, sy = 0;
    for (i = 0; i < 5; i++) {
        ExObj *o = gExtraBossPlayerManager->unk8[i];
        if (o == NULL)
            continue;
        for (k = 0; k < 5; k++) {
            const ExCollider *c = o->unk9C[k];
            if (!coll_active(c) || !(c->unk8 & 4) || (c->unk8 & 0x100))
                continue;
            {
                Box b = coll_box(c);
                sx += b.x;
                sy += b.y;
                n++;
            }
            break;
        }
        if (n)
            break;
    }
    if (n == 0) {
        ExObj *o = gExtraBossPlayerManager->unk8[0];
        if (o == NULL)
            return 0;
        *tx = fxf(o->unk18);
        *ty = fxf(o->unk1C);
        return 1;
    }
    *tx = sx / n;
    *ty = sy / n;
    return 2;
}

static u32 bot_step(Bot *bt) {
    ExWork *G = gExtraBossWork;
    ExObj *pl;
    ExPlayerWork *pw;
    float px, py, gx, gy, bx = 0, by = 0, fl, ft, fr, fb, best = 1e30f;
    int i, k, bestd = 0, have_boss, want_fire = 0, want_dash = -1, st;
    u32 keys;
    if (G == NULL || G->unk8->fn != ExtraBoss_State_Battle || (G->unk0 & 0x12))
        return 0;
    pl = ExtraBoss_GetPlayer();
    if (pl == NULL)
        return 0;
    pw = (ExPlayerWork *)pl->unk1DC;
    st = pw->unk4;
    px = fxf(pl->unk18);
    py = fxf(pl->unk1C);
    fl = fxf(G->unk54) + 16;
    fr = fxf(G->unk58) - 16;
    ft = fxf(G->unk50) + 16;
    fb = fxf(G->unk5C) - 16;
    gather_danger();
    have_boss = boss_target(&bx, &by);
    gx = (fl + fr) / 2;
    gy = (ft + fb) / 2;
    if (G->unk3C == 0) {
        /* Super Sonic: deflect beam units / homing beams into the boss */
        float bestd2 = 1e30f;
        if (have_boss) {
            gx = bx - g_son_dx;
            gy = by;
        } else {   /* round 0 field: stay at the left, in the middle */
            gx = fl + 60;
            gy = (ft + fb) / 2;
        }
        for (i = 0; i < 0x2e && have_boss == 2; i++) {
            ExObj *o = gExtraBossPlayerManager->unkBC[i];
            if (o == NULL || o->unk4 != 1)
                continue;
            for (k = 0; k < 5; k++) {
                const ExCollider *c = o->unk9C[k];
                Box b;
                float ox, oy, dist, aim, ang, diff;
                int d;
                if (!coll_active(c) || (c->unk8 & 0x102) != 2)
                    continue;
                b = coll_box(c);
                ox = b.x - px;
                oy = b.y - py;
                dist = sqrtf(ox * ox + oy * oy);
                if (dist > 120 || dist < 4)
                    continue;
                /* the dash direction (8-way) that reaches the target */
                ang = atan2f(oy, ox);
                d = ((int)lrintf(ang / 0.785398f) + 8) % 8;
                aim = atan2f(by - b.y, bx - b.x);
                diff = fabsf(remainderf(aim - d * 0.785398f, 6.2831853f));
                if (fabsf(remainderf(ang - d * 0.785398f, 6.2831853f)) < 0.42f && diff < 0.45f && dist < bestd2) {
                    bestd2 = dist;
                    want_dash = d;
                }
                if (dist < bestd2 && want_dash < 0) {   /* move to the far side of it from the boss */
                    float a2 = atan2f(b.y - by, b.x - bx);
                    gx = b.x + cosf(a2) * 45;
                    gy = b.y + sinf(a2) * 45;
                }
            }
        }
    } else if (have_boss) {
        /* Burning Blaze: line up to the left of the hand boss's hit box and fire */
        gx = bx - g_blz_dx;
        if (gx < fl + 10)
            gx = fl + 10;
        gy = by;
        if (fabsf(py - by) < 10 && px < bx - 30)
            want_fire = 1;
    }
    /* rings when nothing urgent */
    if (want_dash < 0 && gExtraBossRingWork != NULL && (G->unk4C < g_ring_low || !have_boss)) {
        const ExRing *r;
        float rd = 160 * 160;
        for (k = 0; k < 2; k++)
            for (r = k ? gExtraBossRingWork->unkA8 : gExtraBossRingWork->unk98; r != NULL; r = r->next) {
                float dx = fxf(r->x) - px, dy = fxf(r->y) - py;
                if (dx * dx + dy * dy < rd && fxf(r->x) > fl && fxf(r->x) < fr && fxf(r->y) > ft && fxf(r->y) < fb) {
                    rd = dx * dx + dy * dy;
                    gx = fxf(r->x);
                    gy = fxf(r->y);
                }
            }
    }
    if (gx < fl) gx = fl;
    if (gx > fr) gx = fr;
    if (gy < ft) gy = ft;
    if (gy > fb) gy = fb;
    /* movement: best of the 9 pad directions over the next frames */
    {
        float sp = fxf(pw->unk8->unkC);
        for (i = 0; i < 9; i++) {
            float s = 0, x = px, y = py, n = DIRX[i] && DIRY[i] ? 0.7071f : 1.0f;
            int t;
            for (t = 1; t <= 10; t++) {
                x += DIRX[i] * sp * n;
                y += DIRY[i] * sp * n;
                if (x < fl - 8 || x > fr + 8 || y < ft - 8 || y > fb + 8)
                    s += 300;
                s += danger_at(x, y, t, 10) / (t < 4 ? 1 : t / 3.0f);
            }
            s += sqrtf((x - gx) * (x - gx) + (y - gy) * (y - gy)) * 3;
            if (i == 0)
                s -= 1;   /* prefer standing still on ties */
            if (s < best) {
                best = s;
                bestd = i;
            }
        }
    }
    keys = (sqrtf((px - gx) * (px - gx) + (py - gy) * (py - gy)) < 3 && danger_at(px, py, 2, 10) == 0) ? 0 : dir_keys(bestd);
    if (danger_at(px, py, 3, 10) > 0)
        bt->frames_threat++;
    if (bt->cool > 0)
        bt->cool--;
    if (want_dash >= 0 && bt->cool == 0 && (st == 1 || st == 2)) {
        static const int map8[8] = {1, 2, 3, 4, 5, 6, 7, 8};   /* angle step -> pad direction */
        keys = dir_keys(map8[want_dash]) | IN_JUMP;
        bt->dashes++;
        bt->cool = 20;
    } else if (want_fire && bt->cool == 0 && (st == 1 || st == 2)) {
        keys |= IN_JUMP;
        bt->shots++;
        bt->cool = 12;
    }
    return keys;
}

/* ------------------------------------------------------------------ runs */
typedef struct {
    int result, frames, rounds_seen, chars_seen[2], switches, max_round, title_frames;
    int phases[8];
    int boss_hp_max[5], hp_seen_decrease;
    BossExtraResult res;
    BossExStats stats;
    Bot bot;
} Run;

static const char *g_player_dir, *g_models, *g_data;

static int run_fight(Run *R, u32 seed, int level, int lives, int idle, int max_frames, const char *tag) {
    InputState in;
    int f, last_round = -1, last_phase = -1, last_hp = -1, dumped_mask = 0, title_since = -1, sw_frames = 0;
    char name[96];
    memset(R, 0, sizeof(*R));
    memset(&in, 0, sizeof(in));
    g_dcp_world.rand_seed = seed;
    boss_extra_set_paths(g_models, g_data);
    boss_extra_set_lives(lives, lives);
    boss_extra_set_difficulty(level);
    if (boss_extra_init(1) != 0) {
        printf("  boss_extra_init failed\n");
        return -1;
    }
    for (f = 0; f < max_frames; f++) {
        int r, ph, rd;
        in.held = idle ? 0 : bot_step(&R->bot);
        in.pressed = in.held & ~R->bot.prev;
        R->bot.prev = in.held;
        {
            u32 hurts0 = g_bossex_stats.player_hurts;
            float hx = 0, hy = 0;
            if (gExtraBossWork && ExtraBoss_GetPlayer()) {
                hx = fxf(ExtraBoss_GetPlayer()->unk18);
                hy = fxf(ExtraBoss_GetPlayer()->unk1C);
            }
            r = boss_extra_run_frame(&in);
            if (getenv("TRACEHURT") && g_bossex_stats.player_hurts != hurts0) {
                int i, bi = -1;
                float bd = 1e9f;
                for (i = 0; i < g_ndanger; i++) {
                    float d = fabsf(g_danger[i].x - hx) - g_danger[i].hx;
                    float e = fabsf(g_danger[i].y - hy) - g_danger[i].hy;
                    if ((d > e ? d : e) < bd) {
                        bd = d > e ? d : e;
                        bi = i;
                    }
                }
                printf("    hurt f%d round %d at (%.0f,%.0f) keys %x: ", f, boss_extra_round(), hx, hy, (unsigned)in.held);
                if (bi >= 0)
                    printf("kind %d fx %d %s box (%.0f,%.0f) half (%.0f,%.0f) v (%.1f,%.1f) gap %.0f\n",
                           g_danger[bi].o->unk4, g_danger[bi].o->unk1C8, ex_state_name(g_danger[bi].o->unk74),
                           g_danger[bi].x, g_danger[bi].y, g_danger[bi].hx, g_danger[bi].hy, g_danger[bi].vx,
                           g_danger[bi].vy, bd);
                else
                    printf("no known danger\n");
            }
        }
        if (r != BOSS_EXTRA_RUNNING) {
            R->result = r;
            break;
        }
        ph = boss_phase();
        rd = boss_extra_round();
        if (ph >= BOSSEX_PHASE_TITLE && ph <= BOSSEX_PHASE_CLEAR)
            R->phases[ph - BOSSEX_PHASE_TITLE]++;
        if (ph == BOSSEX_PHASE_SWITCH)
            sw_frames++;
        if (ph == BOSSEX_PHASE_TITLE && title_since < 0)
            title_since = f;
        if (ph != BOSSEX_PHASE_TITLE && title_since >= 0) {
            R->title_frames = f - title_since;
            title_since = -1;
        }
        R->chars_seen[boss_extra_character() & 1] = 1;
        if (rd > R->max_round)
            R->max_round = rd;
        if (rd < 5 && boss_health() > R->boss_hp_max[rd])
            R->boss_hp_max[rd] = boss_health();
        if (last_hp > 0 && boss_health() == last_hp - 1)
            R->hp_seen_decrease++;
        if (rd != last_round || ph != last_phase) {
            if (g_verbose)
                printf("    f%6d round %d char %d phase %02x hp %d/%d rings %d time %d lives %d/%d\n", f, rd,
                       boss_extra_character(), ph, boss_health(), gBossStatus.unkA, boss_extra_rings(),
                       boss_extra_time(), gExtraBossWork->unk60[0], gExtraBossWork->unk60[1]);
            last_round = rd;
            last_phase = ph;
        }
        last_hp = boss_health();
        /* frames of the composition */
        if (tag != NULL && g_render) {
            int key = -1;
            if (f == 40)
                key = 0, snprintf(name, sizeof(name), "%s_title", tag);
            else if (ph == BOSSEX_PHASE_SCROLL && R->phases[1] == 400)
                key = 1, snprintf(name, sizeof(name), "%s_r0_field", tag);
            else if (ph == BOSSEX_PHASE_SCROLL && R->phases[1] == 100)
                key = 2, snprintf(name, sizeof(name), "%s_r0_vs_cutin", tag);
            else if (ph == BOSSEX_PHASE_WEAPON && R->phases[2] == 300)
                key = 3, snprintf(name, sizeof(name), "%s_weapon", tag);
            else if (ph == BOSSEX_PHASE_HAND && R->phases[3] == 300)
                key = 4, snprintf(name, sizeof(name), "%s_hand", tag);
            else if (ph == BOSSEX_PHASE_SWITCH && sw_frames == 40)
                key = 5, snprintf(name, sizeof(name), "%s_switch", tag);
            else if (ph == BOSSEX_PHASE_HAND && R->phases[3] == 900)
                key = 6, snprintf(name, sizeof(name), "%s_hand2", tag);
            else if (ph == BOSSEX_PHASE_WEAPON && R->phases[2] == 1500)
                key = 7, snprintf(name, sizeof(name), "%s_weapon2", tag);
            else if (ph == BOSSEX_PHASE_DEFEAT && R->phases[4] == 30)
                key = 8, snprintf(name, sizeof(name), "%s_defeat", tag);
            else if (ph == BOSSEX_PHASE_SWITCH && sw_frames == 10)
                key = 9, snprintf(name, sizeof(name), "%s_switch_early", tag);
            else if (ph == BOSSEX_PHASE_SCROLL && R->phases[1] == 40)
                key = 10, snprintf(name, sizeof(name), "%s_r0_vs_open", tag);
            else if (ph == BOSSEX_PHASE_SCROLL && R->phases[1] == 145)
                key = 11, snprintf(name, sizeof(name), "%s_r0_vs_fade", tag);
            if (key >= 0 && !(dumped_mask & (1 << key))) {
                dumped_mask |= 1 << key;
                dump(name);
                if (key == 5) {   /* what the two DS screens show (slots 1 / 2), for comparison */
                    char n2[112];
                    int sl;
                    for (sl = 1; sl <= 2; sl++) {
                        g_ex_host_switch_slot = sl;
                        snprintf(n2, sizeof(n2), "%s_ds_slot%d", name, sl);
                        dump(n2);
                    }
                    g_ex_host_switch_slot = 3;
                }
                if (key == 3)
                    CHECK(frame_nonblack() > 640 * 480 / 2, "%s weapon-boss frame drawn (%d lit pixels)", tag,
                          frame_nonblack());
            }
        }
        if (getenv("DUMPAT") && f == atoi(getenv("DUMPAT"))) {
            snprintf(name, sizeof(name), "at_%d", f);
            dump(name);
        }
    }
    R->frames = f;
    R->switches = (int)g_bossex_stats.switches;
    R->stats = g_bossex_stats;
    boss_extra_result(&R->res);
    boss_extra_shutdown();
    return 0;
}

static void report(const Run *R) {
    const BossExStats *s = &R->stats;
    printf("    %d frames, result %d, round %d; boss hits %u, player hurts %u, downs %u; dashes %d (deflects %u), "
           "shots %d (%u); rings +%u -%u\n",
           R->frames, R->result, R->max_round, s->boss_hits, s->player_hurts, s->player_downs, R->bot.dashes,
           s->deflects, R->bot.shots, s->shots, s->rings_collected, s->rings_lost);
    printf("    attacks: meteors %u (broken %u), big beams %u, beam units %u, homing beams %u, missiles %u, swords %u, "
           "claws %u, catches %u (grabs %u), suctions %u; switches %u; bosses %u weapon / %u hand\n",
           s->meteors, s->meteors_broken, s->big_beams, s->beam_units, s->homing_beams, s->missiles, s->swords,
           s->claws, s->catches, s->grabs, s->suctions, s->switches, s->boss_spawned[0], s->boss_spawned[1]);
    printf("    result: time %d rings %d lives %d/%d current %d\n", R->res.time, R->res.rings, R->res.lives_sonic,
           R->res.lives_blaze, R->res.lives_current);
}

int main(int argc, char **argv) {
    long size;
    char path[512];
    s16 *trig;
    Run R;
    const char *only = getenv("ONLY");
    u32 seed = getenv("SEED") ? (u32)strtoul(getenv("SEED"), NULL, 0) : 0x1234u;
    int level = getenv("LEVEL") ? atoi(getenv("LEVEL")) : 0;
    int budget = getenv("FRAMES") ? atoi(getenv("FRAMES")) : 60 * 60 * 25;
    if (argc < 4) {
        printf("usage: %s <player dir> <models dir> <boss/extra dir> [-v]\n", argv[0]);
        return 2;
    }
    g_verbose = argc > 4;
    g_player_dir = argv[1];
    g_models = argv[2];
    g_data = argv[3];
    if (getenv("DUMPDIR"))
        g_dumpdir = getenv("DUMPDIR");
    g_render = getenv("NORENDER") == NULL;
    if (getenv("MARGIN"))
        g_margin = (float)atof(getenv("MARGIN"));
    if (getenv("BLZDX"))
        g_blz_dx = (float)atof(getenv("BLZDX"));
    if (getenv("SONDX"))
        g_son_dx = (float)atof(getenv("SONDX"));
    if (getenv("RINGLOW"))
        g_ring_low = (float)atof(getenv("RINGLOW"));
    snprintf(path, sizeof(path), "%s/trig.bin", g_player_dir);
    trig = (s16 *)load(path, &size);
    dcp_set_trig(trig, trig + 8192);
    memset(&g_boss_hooks, 0, sizeof(g_boss_hooks));
    g_boss_hooks.sfx = h_sfx;
    g_boss_hooks.voice = h_voice;
    g_boss_hooks.music = h_music;
    g_boss_extra_hooks.pause = h_pause;
    if (g_render) {
        snprintf(path, sizeof(path), "mkdir -p %s", g_dumpdir);
        if (system(path) != 0)
            g_render = 0;
        boss_extra_host_target(g_rgb);
    }

    if (!only || !strcmp(only, "clear")) {
        printf("[clear] seed %#x level %d\n", seed, level);
        g_music_n = 0;
        run_fight(&R, seed, level, 3, 0, budget, "ex");
        report(&R);
        CHECK(R.result == BOSS_EXTRA_CLEARED, "fight cleared (result %d after %d frames)", R.result, R.frames);
        CHECK(R.res.cleared == 1, "result.cleared");
        CHECK(R.max_round == 4, "all five rounds played (last round %d)", R.max_round);
        CHECK(R.chars_seen[0] && R.chars_seen[1], "Super Sonic and Burning Blaze both played");
        CHECK(R.switches >= 4, "character switch between the rounds (%d)", R.switches);
        CHECK(R.stats.boss_spawned[0] >= 3 && R.stats.boss_spawned[1] >= 2, "weapon boss x%u, hand boss x%u",
              R.stats.boss_spawned[0], R.stats.boss_spawned[1]);
        CHECK(R.boss_hp_max[0] == 2 && R.boss_hp_max[1] == 2 && R.boss_hp_max[4] == 3,
              "boss hit points per round (normal) %d %d %d %d %d", R.boss_hp_max[0], R.boss_hp_max[1],
              R.boss_hp_max[2], R.boss_hp_max[3], R.boss_hp_max[4]);
        CHECK(R.stats.boss_hits >= 11, "boss hits %u (2+2+2+2+3)", R.stats.boss_hits);
        CHECK(R.hp_seen_decrease >= 6, "boss_health() goes down hit by hit (%d)", R.hp_seen_decrease);
        CHECK(R.stats.deflects > 0 && R.stats.shots > 0, "Sonic's deflects (%u) and Blaze's shots (%u)",
              R.stats.deflects, R.stats.shots);
        CHECK(R.phases[1] > 0 && R.phases[2] > 0 && R.phases[3] > 0 && R.phases[4] > 0 && R.phases[5] > 0,
              "phases seen: title %d scroll %d weapon %d hand %d defeat %d switch %d", R.phases[0], R.phases[1],
              R.phases[2], R.phases[3], R.phases[4], R.phases[5]);
        CHECK(R.stats.meteors > 0, "round 0 meteor field (%u meteors)", R.stats.meteors);
        CHECK(R.res.time > 0 && R.res.time <= 0x8c9f && R.res.rings >= 0 && R.res.rings <= 999,
              "result time %d rings %d in range", R.res.time, R.res.rings);
        CHECK(g_music_n >= 1 && g_music[0] == 0x14, "round music 0x14 first (%d calls)", g_music_n);
        CHECK(R.title_frames >= BOSS_EXTRA_TITLE_FRAMES - 2 && R.title_frames <= BOSS_EXTRA_TITLE_FRAMES + 2,
              "title-card stand-in %d frames", R.title_frames);
        CHECK(g_sfx_count > 100 && g_voice_count > 0, "sound hooks (sfx %d, voices %d)", g_sfx_count, g_voice_count);
        CHECK(boss_phase() == BOSSEX_PHASE_CLEAR || boss_phase() == 0, "boss_phase after the fight %#x", boss_phase());
    }
    if (!only || !strcmp(only, "seeds")) {
        static const u32 seeds[3] = {1, 0xdeadbeefu, 77777};
        int i;
        for (i = 0; i < 4; i++) {
            u32 s = i < 3 ? seeds[i] : seed;
            int lv = i < 3 ? 0 : 1;
            printf("[seeds] seed %#x level %d\n", s, lv);
            run_fight(&R, s, lv, 3, 0, budget, i == 3 ? "hard" : NULL);
            report(&R);
            CHECK(R.result == BOSS_EXTRA_CLEARED, "seed %#x level %d cleared (%d frames, round %d)", s, lv, R.frames,
                  R.max_round);
            if (lv == 1)
                CHECK(R.boss_hp_max[0] == 3 && R.boss_hp_max[4] == 4, "hard hit points %d .. %d", R.boss_hp_max[0],
                      R.boss_hp_max[4]);
        }
    }
    if (!only || !strcmp(only, "fail")) {
        printf("[fail] idle pad, 0 lives in reserve\n");
        run_fight(&R, seed, 0, 0, 1, budget, NULL);
        report(&R);
        CHECK(R.result == BOSS_EXTRA_FAILED, "idle player: game over (result %d after %d frames)", R.result, R.frames);
        CHECK(R.res.cleared == 0 && R.stats.player_downs >= 1, "not cleared, %u player downs", R.stats.player_downs);
        CHECK(R.frames > 51 * 60 + BOSS_EXTRA_TITLE_FRAMES, "rings ran out (50 rings, one a second, plus the "
              "rings the idle player touched: %d frames)", R.frames);
        CHECK(gExtraBossWork == NULL && gExtraBossPlayerManager == NULL && gExtraBossRingWork == NULL &&
                  data_ov001_0231f54c == NULL && data_ov001_0231f5a8 == NULL && data_ov001_0231f094 == NULL &&
                  data_ov001_0231f080 == NULL,
              "work, object manager, rings, BG, sub screen, flags and camera slots freed after shutdown");
        printf("[fail] 1 life each: the pair restarts at round 0\n");
        run_fight(&R, seed, 0, 1, 1, budget, NULL);
        CHECK(R.result == BOSS_EXTRA_FAILED && R.stats.player_downs == 2 && R.stats.rounds_started >= 2,
              "two downs (%u), round restarted (%u starts)", R.stats.player_downs, R.stats.rounds_started);
    }
    if (!only || !strcmp(only, "pause")) {
        InputState in;
        int f, t0;
        printf("[pause]\n");
        memset(&in, 0, sizeof(in));
        boss_extra_set_paths(g_models, g_data);
        boss_extra_set_lives(3, 3);
        CHECK(boss_extra_init(0) == 0, "init");
        for (f = 0; f < 400; f++)
            boss_extra_run_frame(&in);
        t0 = boss_extra_time();
        g_pause_hook = 0;
        in.held = IN_START;
        boss_extra_run_frame(&in);
        in.held = 0;
        for (f = 0; f < 100; f++)
            boss_extra_run_frame(&in);
        CHECK(boss_extra_paused() && g_pause_hook == 1, "START pauses (hook %d)", g_pause_hook);
        CHECK(boss_extra_time() == t0, "time stands while paused (%d -> %d)", t0, boss_extra_time());
        boss_extra_resume();
        for (f = 0; f < 60; f++)
            boss_extra_run_frame(&in);
        CHECK(!boss_extra_paused() && boss_extra_time() > t0, "resumed (%d)", boss_extra_time());
        boss_extra_shutdown();
        boss_extra_run_frame(&in);
        boss_extra_draw(0);
        CHECK(gExtraBossWork == NULL && boss_extra_round() == 0, "run_frame / draw after shutdown do nothing");
        boss_extra_set_paths(g_models, "/nonexistent");
        CHECK(boss_extra_init(0) < 0 && gExtraBossWork == NULL, "missing data: init fails cleanly");
        boss_extra_shutdown();
        boss_extra_set_paths(g_models, g_data);
    }
    printf("%d checks, %d failed\n", g_checks, g_fail);
    return g_fail ? 1 : 0;
}
