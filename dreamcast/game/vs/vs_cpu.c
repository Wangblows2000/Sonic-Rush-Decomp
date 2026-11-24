/*
 * Battle Play "Vs CPU" (native): the computer's pad. See vs_cpu.h and vs_route.h.
 *
 * Every frame: where am I on my line (the frame to play next, found around the expected one, so
 * standing still or a loop keeps the place)? On the line: play its pad. Off it: the nearest point of
 * any line with the least time left to the goal (a grid of the points) and play from there; nothing
 * near: steer back to a line point ahead with a player's moves. No progress for 3 s: recovery moves.
 */
#include <stdlib.h>
#include <string.h>
#include "vs_cpu.h"
#include "vs_route.h"
#include "../player/dcp_collision.h"

#define VSC_BUTTONS (VSC_A | VSC_B | VSC_X | VSC_Y | VSC_R | VSC_L)
#define NEAR_PX 40          /* on the line within this distance */
#define JOIN_PX 64          /* another line's point this near can be taken */
#define WIN_BACK 90         /* local search window (frames) around the expected one */
#define WIN_AHEAD 240
#define GCELL 128           /* point grid cell (px) */
#define NFAIL 32            /* places where a line failed, remembered */
#define FAIL_PX 320         /* a failed line is not taken within this distance of the place */
#define FAIL_T 1800         /* for 30 s (a gimmick comes back, a platform cycles; then it is tried again) */

typedef struct Cpu {
    const VsRouteLine *ln;
    const VsRoutePoint *pt;
    int nl, n;               /* lines, points */
    u32 *gidx; u32 *gstart;  /* points by grid cell */
    int gw, gh;
    u32 seed, rng;
    int skill;
    u32 t;
    int cl, k;               /* current line, its frame to play next */
    int best_rem;            /* least frames to the goal reached so far */
    u32 best_t;              /* frame best_rem last fell */
    u16 prev;                /* last held pad */
    int jump_hold, jump_cool;
    int rec, rec_t, rec_n;   /* stuck recovery: step, frames left, count */
    int wait;                /* frames waited at a gimmick's boarding place */
    int ban, ban_until;      /* a line that failed here: not taken for a while (-1) */
    int tl, tk;              /* off the line: the point steered to (line, frame; tl -1: none) */
    u32 attack_t;            /* last boost attack on the rival */
    int incomplete;          /* the stage's line 0 does not reach the goal (partial) */
    int ru_l, ru_k, ru_n, ru_at;  /* run-up: line, point to go back to, tries at this place, the place (point) */
    int delay;               /* reaction at GO: frames without keys (variation; the line waits, time shifts) */
    VsRoutePoint *rv;        /* the rival's path this race (position after each frame, its pad) */
    int rvn, rvcap;
    s32 rvx, rvy;            /* the rival's position before its first recorded frame */
    int ridx;                /* the rival's place on line 0 (-1) */
    int rw_l, rw_b, rw_t;    /* a ride missed (the line rides from point rw_b of line rw_l, we are not on it): wait
                                at its boarding place for it to come back (rw_l -1: none) */
    struct { s16 l; s16 x, y; u32 until; } fail[NFAIL];   /* lines that failed at a place (stuck there), for a while */
    int nfail;
    int stuck_n;             /* stuck detections since the last progress */
    int deaths;              /* this race */
    s32 death_x, death_y;    /* where it last died */
    int was_dead;
    u32 lost_until;          /* following the rival's path until then (the lines fail here) */
    VsCpuStats st;
} Cpu;
static Cpu C[2];

static u32 hash(u32 a) { a ^= a >> 16; a *= 0x7feb352du; a ^= a >> 15; a *= 0x846ca68bu; a ^= a >> 16; return a; }
static u32 rnd(Cpu *c) { c->rng = c->rng * 1103515245u + 12345u; return (c->rng >> 16) & 0x7fff; }

static void free_grid(Cpu *c) { free(c->gidx); free(c->gstart); free(c->rv); c->gidx = NULL; c->gstart = NULL; c->rv = NULL; }
static int build_grid(Cpu *c) {
    int i, mx = 0, my = 0, cells;
    u32 *cnt;
    for (i = 0; i < c->n; i++) { if (c->pt[i].x > mx) mx = c->pt[i].x; if (c->pt[i].y > my) my = c->pt[i].y; }
    c->gw = mx / GCELL + 1; c->gh = my / GCELL + 1;
    cells = c->gw * c->gh;
    c->gstart = (u32 *)calloc((size_t)cells + 1, sizeof(u32));
    c->gidx = (u32 *)malloc(sizeof(u32) * (size_t)c->n);
    cnt = (u32 *)calloc((size_t)cells, sizeof(u32));
    if (!c->gstart || !c->gidx || !cnt) { free(cnt); free_grid(c); return -1; }
    for (i = 0; i < c->n; i++) c->gstart[(c->pt[i].y / GCELL) * c->gw + c->pt[i].x / GCELL + 1]++;
    for (i = 0; i < cells; i++) c->gstart[i + 1] += c->gstart[i];
    for (i = 0; i < c->n; i++) {
        int g = (c->pt[i].y / GCELL) * c->gw + c->pt[i].x / GCELL;
        c->gidx[c->gstart[g] + cnt[g]++] = (u32)i;
    }
    free(cnt);
    return 0;
}

int vs_cpu_begin(int who, const void *route, u32 route_size, u32 seed, int skill) {
    Cpu *c = &C[who & 1];
    const VsRouteHeader *h = (const VsRouteHeader *)route;
    free_grid(c);
    memset(c, 0, sizeof *c);
    c->seed = seed;
    c->rng = seed ? seed : 1;
    c->skill = skill;
    c->ridx = -1;
    c->ban = -1;
    c->rw_l = -1;
    c->delay = 0;   /* (a reaction delay at GO would shift the objects' timing against the line: none) */
    c->tl = -1;
    c->best_rem = 0x7fffffff;
    if (h != NULL && route_size >= sizeof *h && !memcmp(h->magic, "VSRT", 4) && h->version == VS_ROUTE_VERSION &&
        h->nlines > 0 && h->nlines <= VS_ROUTE_MAX_LINES && h->points > 1 && h->points <= 0x400000 &&
        route_size >= sizeof *h + h->nlines * sizeof(VsRouteLine) + h->points * sizeof(VsRoutePoint)) {
        int i, ok = 1;
        c->ln = (const VsRouteLine *)(h + 1);
        c->pt = (const VsRoutePoint *)(c->ln + h->nlines);
        c->nl = h->nlines; c->n = (int)h->points;
        for (i = 0; i < c->nl; i++)
            if (c->ln[i].count == 0 || c->ln[i].first + c->ln[i].count > h->points) ok = 0;
        if (!ok || build_grid(c) != 0) { c->ln = NULL; c->pt = NULL; c->nl = c->n = 0; }
    }
    c->incomplete = c->nl && (h->flags & VSR_INCOMPLETE);
    c->rvcap = 0x8ca0;   /* the race time limit */
    c->rv = (VsRoutePoint *)malloc(sizeof(VsRoutePoint) * (size_t)c->rvcap);
    if (!c->rv) c->rvcap = 0;
    c->st.line_len = c->nl ? (int)c->ln[0].count : 0;
    return c->nl ? 0 : -1;
}
void vs_cpu_end(int who) { free_grid(&C[who & 1]); }
const VsCpuStats *vs_cpu_stats(int who) { return &C[who & 1].st; }

/* ------------------------------------------------------------------ senses */
static u32 layer(const DcPlayer *p) { return (p->obj_flags & 1) | RLVL_Q_SKIP_TOP; }
static s32 wall_dist(const DcPlayer *p, s32 x, s32 y, int dir) {
    return rlvl_probe_distance_long(g_dcp_col.map, x, y, layer(p), dir > 0 ? RLVL_DIR_RIGHT : RLVL_DIR_LEFT, NULL, NULL);
}
static const VsRoutePoint *P(const Cpu *c, int l, int i) { return &c->pt[c->ln[l].first + (u32)i]; }
/* frames to the goal from frame k of line l */
static s32 left(const Cpu *c, int l, int k) { return (s32)c->ln[l].count - k + (s32)c->ln[l].rest; }
/* the position before frame k of line l (k = 0: the line's start) */
static void before(const Cpu *c, int l, int k, s32 *x, s32 *y) {
    if (k <= 0) { *x = c->ln[l].sx; *y = c->ln[l].sy; }
    else { *x = P(c, l, k - 1)->x; *y = P(c, l, k - 1)->y; }
}
static s32 sq(s32 dx, s32 dy) {
    if (dx < -4000 || dx > 4000 || dy < -4000 || dy > 4000) return 0x7fffffff;
    return dx * dx + dy * dy;
}
/* on line l: the frame to play next from (x, y), around the expected one (time goes on: a cost per
 * frame away from it, so standing still or a loop does not lose the place) */
static int track(const Cpu *c, int l, int expect, s32 x, s32 y, s32 *dist2) {
    int i, lo = expect - WIN_BACK, hi = expect + WIN_AHEAD, best = -1, cnt = (int)c->ln[l].count;
    s32 bc = 0x7fffffff, bd = 0x7fffffff;
    if (lo < 0) lo = 0;
    if (hi > cnt - 1) hi = cnt - 1;
    for (i = lo; i <= hi; i++) {
        s32 px, py, d, cost;
        before(c, l, i, &px, &py);
        d = sq(px - x, py - y);
        if (d == 0x7fffffff) continue;
        cost = d + 8 * (i > expect ? i - expect : expect - i);
        if (cost < bc) { bc = cost; bd = d; best = i; }
    }
    *dist2 = bd;
    return best;
}
/* line l failed near (x, y) (the CPU got stuck on it there): not taken here for a while */
static int failed_here(const Cpu *c, int l, s32 x, s32 y) {
    int i;
    for (i = 0; i < c->nfail; i++)
        if (c->fail[i].l == l && c->t < c->fail[i].until && DCP_ABS(c->fail[i].x - x) < FAIL_PX && DCP_ABS(c->fail[i].y - y) < FAIL_PX)
            return 1;
    return 0;
}
static void add_fail(Cpu *c, int l, s32 x, s32 y) {
    int i, o = 0;
    for (i = 0; i < c->nfail; i++) if (c->fail[i].until < c->fail[o].until) o = i;   /* the oldest goes */
    i = c->nfail < NFAIL ? c->nfail++ : o;
    c->fail[i].l = (s16)l; c->fail[i].x = (s16)x; c->fail[i].y = (s16)y; c->fail[i].until = c->t + FAIL_T;
}
/* integer square root (small values) */
static s32 isqrt(s32 v) { s32 r = 0; while ((r + 8) * (r + 8) <= v) r += 8; while ((r + 1) * (r + 1) <= v) r++; return r; }
/* any line: the point within r px (by the grid) with the least time left to the goal (distance counted
 * as time, about 4 px a frame); *line / *k: the frame to play next */
static int join(const Cpu *c, s32 x, s32 y, s32 r, int not_line, int *line, int *k, s32 *dist2) {
    int gx0 = (x - r) / GCELL, gx1 = (x + r) / GCELL, gy0 = (y - r) / GCELL, gy1 = (y + r) / GCELL, gx, gy, l;
    s32 best = 0x7fffffff, now = (s32)g_dcp_world.frame, dt;
    u32 j;
    if (gx0 < 0) gx0 = 0;
    if (gy0 < 0) gy0 = 0;
    if (gx1 >= c->gw) gx1 = c->gw - 1;
    if (gy1 >= c->gh) gy1 = c->gh - 1;
    *line = -1;
    for (gy = gy0; gy <= gy1; gy++)
        for (gx = gx0; gx <= gx1; gx++) {
            int g = gy * c->gw + gx;
            for (j = c->gstart[g]; j < c->gstart[g + 1]; j++) {
                int a = c->gidx[j], i;
                s32 d = sq(c->pt[a].x - x, c->pt[a].y - y), cost;
                if (d > r * r) continue;
                for (l = 0; l < c->nl - 1; l++)
                    if ((u32)a < c->ln[l].first + c->ln[l].count) break;
                if (l == not_line || failed_here(c, l, x, y)) continue;
                i = a - (int)c->ln[l].first + 1;                       /* the point is the position before frame i */
                if (i >= (int)c->ln[l].count) i = (int)c->ln[l].count - 1;
                /* + the time apart: the line's pads were found in the world as it was at its own time (objects
                 * move on the race clock), a line made at about this time fits the world as it is now */
                dt = now - (s32)(c->ln[l].t0 + (u32)i);
                if (dt < 0) dt = -dt;
                cost = left(c, l, i) + isqrt(d) / 4 + (dt > 1200 ? 1200 : dt) / 4;
                if (cost < best) { best = cost; *line = l; *k = i; *dist2 = d; }
            }
        }
    return *line >= 0;
}
/* the rival's path: the position before its frame k */
static void rv_before(const Cpu *c, int k, s32 *x, s32 *y) {
    if (k <= 0) { *x = c->rvx; *y = c->rvy; }
    else { *x = c->rv[k - 1].x; *y = c->rv[k - 1].y; }
}
/* on the rival's path: the frame to play next around the expected one (as track) */
static int rv_track(const Cpu *c, int expect, s32 x, s32 y, s32 *dist2) {
    int i, lo = expect - WIN_BACK, hi = expect + WIN_AHEAD, best = -1;
    s32 bc = 0x7fffffff, bd = 0x7fffffff;
    if (lo < 0) lo = 0;
    if (hi > c->rvn - 1) hi = c->rvn - 1;
    for (i = lo; i <= hi; i++) {
        s32 px, py, d, cost;
        rv_before(c, i, &px, &py);
        d = sq(px - x, py - y);
        if (d == 0x7fffffff) continue;
        cost = d + 8 * (i > expect ? i - expect : expect - i);
        if (cost < bc) { bc = cost; bd = d; best = i; }
    }
    *dist2 = bd;
    return best;
}
/* the rival's path: the furthest point it has been to within r px (the most progress), at least 2 s
 * behind the rival (so there is a way on to follow) */
static int rv_join(const Cpu *c, s32 x, s32 y, s32 r, s32 *dist2) {
    int i;
    for (i = c->rvn - 120; i >= 0; i -= 2) {
        s32 d = sq(c->rv[i].x - x, c->rv[i].y - y);
        if (d <= r * r) { *dist2 = d; return i + 1; }
    }
    return -1;
}

/* drop the line's boost on this stretch? (skill; never just before one of the line's jumps) */
static int skip_boost(Cpu *c, int k, int behind) {
    int s = k, i, cnt = (int)c->ln[c->cl].count;
    u32 pct;
    if (c->skill >= VSC_HARD || behind) return 0;
    while (s > 0 && (P(c, c->cl, s - 1)->pad & VSC_X)) s--;
    for (i = k; i < k + 60 && i < cnt; i++)
        if ((P(c, c->cl, i)->pad & VSC_A) && !(P(c, c->cl, i > 0 ? i - 1 : 0)->pad & VSC_A)) return 0;
    pct = hash(((u32)s + ((u32)c->cl << 16)) * 2654435761u ^ c->seed) % 100;
    return c->skill == VSC_EASY && (int)pct < 50;
}

/* ------------------------------------------------------------------ steering back to a line */
static u16 steer(Cpu *c, const DcPlayer *p, s32 tx, s32 ty) {
    s32 x = p->x >> 8, y = p->y >> 8;
    int dir = tx >= x ? 1 : -1, grounded = (p->move_flags & DCP_MF_GROUND) != 0;
    u16 h = dir > 0 ? VSC_RIGHT : VSC_LEFT;
    if (c->jump_cool > 0) c->jump_cool--;
    if (c->jump_hold > 0) { c->jump_hold--; return h | VSC_A; }
    if (grounded && c->jump_cool == 0) {
        int jump = ty < y - 40;                                                     /* the line is above */
        if (wall_dist(p, x + dir * 8, y - 4, dir) < 14 + DCP_ABS(p->gnd_vel) / 64) jump = 1;   /* a wall */
        if (jump) { c->jump_hold = 20; c->jump_cool = 30; return h | VSC_A; }
    }
    if (!grounded && p->vel_y > 0 && DCP_ABS(tx - x) > 48 && ty < y + 16 && !(c->prev & VSC_R) && !(rnd(c) & 7))
        h |= VSC_R;                                                                 /* the air action */
    return h;
}

/* the line crouches near frame k (a spin dash or a roll: a boost there would spoil it) */
static int crouch_near(const Cpu *c, int k) {
    int i;
    if (c->cl < 0 || k < 0) return 0;
    for (i = k - 12; i <= k + 12; i++)
        if (i >= 0 && i < (int)c->ln[c->cl].count && (P(c, c->cl, i)->pad & VSC_DOWN)) return 1;
    return 0;
}

/* confusion (the rival's item turns the D-pad): like a player, after a moment, press the key that
 * now gives the wanted direction */
static u16 unconfuse(const DcPlayer *p, u16 h) {
    static const u16 keys[4] = { VSC_RIGHT, VSC_LEFT, VSC_UP, VSC_DOWN };
    u16 out = h & (u16)~0xf0, want = h & 0xf0;
    int i;
    if (p->confuse == 0 || 127 - (p->confuse & 0x7f) < 40)
        return h;
    for (i = 0; i < 4; i++) {
        u16 m = keys[i] == VSC_RIGHT ? p->map_right : keys[i] == VSC_LEFT ? p->map_left : keys[i] == VSC_UP ? p->map_up : p->map_down;
        if (want & m) out |= keys[i];
    }
    return out;
}

/* ------------------------------------------------------------------ the pad */
void vs_cpu_pad(int who, const DcPlayer *self, const DcPlayer *rival, u16 *held, u16 *pressed, u16 *released) {
    Cpu *c = &C[who & 1];
    const DcPlayer *p = self;
    s32 x = p->x >> 8, y = p->y >> 8, dist2 = 0x7fffffff;
    u16 h = 0;
    int on = 0, behind = 0, k = -1, riding = p->gimmick != NULL;
    c->t++;
    if (c->st.best_x < x) c->st.best_x = x;
    if (p->pf & DCP_PF_DEAD) {
        /* dead: after the respawn the progress counts anew (the start or a checkpoint is behind the best
         * place reached; without this the CPU takes itself for stuck 3 s after every respawn) */
        c->best_rem = 0x7fffffff; c->best_t = c->t;
        c->rec = 0; c->ru_n = 0; c->rw_l = -1; c->jump_hold = 0; c->tl = -1;
        c->stuck_n = 0;
        if (!c->was_dead) {
            /* died again near where it died before: the lines lead into it in this race (the world moved on);
             * after the respawn the rival's way is followed for 30 s. Else the lines first */
            int again = c->deaths > 0 && sq(x - c->death_x, y - c->death_y) < 640 * 640;
            c->deaths++; c->death_x = x; c->death_y = y;
            c->lost_until = again && c->rvn > 240 ? c->t + 1800 : 0;
            if (again && c->rvn > 240) c->st.follow++;
        }
        c->was_dead = 1;
        goto out;
    }
    c->was_dead = 0;
    if (c->delay > 0) { c->delay--; c->best_t = c->t; goto out; }   /* reacting to GO */
    /* the rival's path (what a player sees of it on the split screen) */
    if (rival != NULL && c->rvcap > 0 && !(rival->pf & DCP_PF_DEAD)) {
        if (c->rvn == 0) { c->rvx = rival->x >> 8; c->rvy = rival->y >> 8; }
        if (c->rvn < c->rvcap) {
            c->rv[c->rvn].x = (u16)(rival->x >> 8); c->rv[c->rvn].y = (u16)(rival->y >> 8);
            c->rv[c->rvn].pad = rival->held & (u16)~VSR_RIDE;
            c->rvn++;
        }
    }
    if (c->nl > 0 || c->rvn > 0) {
        int ban = (int)c->t < c->ban_until ? c->ban : -1;
        /* lost: the lines failed here twice running (an item, a knock-back, a death, an enemy, the world
         * not as the line knew it). The rival's path is a way on that works in this race: it is followed
         * (its pads replayed from the nearest place it passed, with the player's own moves) until the CPU
         * is back on a line past the trouble */
        int lost = c->t < c->lost_until && c->rvn > 0;
        if (c->nl == 0 && c->cl >= 0) c->cl = -1;   /* no lines: only the rival's path */
        if (c->cl == -2 && !lost && c->nl > 0 && (c->t & 7) == 0) {
            /* on the rival's path, not lost any more: a line here (on it, not one that failed here) is taken */
            int l2, k2;
            s32 d2;
            if (join(c, x, y, NEAR_PX, ban, &l2, &k2, &d2) && !failed_here(c, l2, x, y)) {
                c->cl = l2; c->k = k2; c->best_rem = 0x7fffffff; c->best_t = c->t;
            }
        }
        if (c->cl == -2) k = rv_track(c, c->k, x, y, &dist2);
        else if (c->nl > 0 && c->cl != ban && !failed_here(c, c->cl, x, y)) k = track(c, c->cl, c->k, x, y, &dist2);
        /* the end of a branch (it joins an earlier line there), off the line, or a banned line: another line */
        if (k < 0 || dist2 > NEAR_PX * NEAR_PX ||
            (c->cl >= 0 && k >= (int)c->ln[c->cl].count - 1 && (c->ln[c->cl].rest > 0))) {
            int l2 = -1, k2 = 0, at_end = c->cl >= 0 && k >= 0 && k >= (int)c->ln[c->cl].count - 1;
            s32 d2 = 0x7fffffff, dr = 0x7fffffff;
            int excl = at_end ? c->cl : ban, kr = -1;
            int got = c->nl > 0 && join(c, x, y, JOIN_PX, excl, &l2, &k2, &d2);
            /* partial lines (no way to the goal was found): the rival's path when it is near */
            if (c->rvn > 0 && (c->nl == 0 || c->incomplete || lost)) kr = rv_join(c, x, y, JOIN_PX, &dr);
            if (kr >= 0 && (!got || (c->incomplete && l2 == 0) || lost)) {
                if (c->cl != -2) { c->st.relocate++; c->best_rem = 0x7fffffff; c->best_t = c->t; }
                c->cl = -2; k = kr; dist2 = dr;
            } else if (got && (k < 0 || d2 < dist2 || excl == c->cl)) {
                if (l2 != c->cl || k2 < k - 120 || k2 > k + 240) c->st.relocate++;
                c->cl = l2; k = k2; dist2 = d2;
            }
        }
        if (c->rec == 4) {
            on = 0;   /* (going back for a run-up: the line is not played) */
        } else if (k >= 0 && c->cl >= 0) {
            int rem = (int)left(c, c->cl, k);
            c->k = k + 1 < (int)c->ln[c->cl].count ? k + 1 : k;
            if (dist2 <= NEAR_PX * NEAR_PX && rem < c->best_rem - 4) { c->best_rem = rem; c->best_t = c->t; c->stuck_n = 0; }
            if (c->cl == 0) c->st.idx = k;
        } else if (k >= 0 && c->cl == -2) {
            c->k = k + 1 < c->rvn ? k + 1 : k;
            /* progress on the rival's path: further along it */
            if (dist2 <= NEAR_PX * NEAR_PX && -k < c->best_rem - 4) { c->best_rem = -k; c->best_t = c->t; }
        }
        on = c->rec != 4 && k >= 0 && dist2 <= NEAR_PX * NEAR_PX;
        if (rival != NULL && c->nl > 0 && (c->t & 7) == 0) {   /* the rival's place on line 0 (lines only) */
            s32 rd;
            int r = track(c, 0, c->ridx < 0 ? 0 : c->ridx, rival->x >> 8, rival->y >> 8, &rd);
            if (r >= 0 && rd < 96 * 96) c->ridx = r;
        }
        behind = c->cl == 0 && c->ridx > k + 120;   /* it is a race: no easing off behind */
    }
    /* stuck: no progress for 3 s (not while carried or waiting for a ride) -> recovery moves */
    if (c->rec == 0 && c->t - c->best_t > (riding ? 600u : 180u) && c->wait == 0) {   /* (a ride that goes nowhere: 10 s) */
        if (++c->stuck_n >= 2 && c->rvn > 240 && c->cl != -2) { c->lost_until = c->t + 600; c->st.follow++; }
        if (on && c->cl >= 0) {
            /* stuck on the line: too slow for what comes (a slope, a jump): back to an earlier point of the
             * line for a run-up, further back each try; after three tries here the line is left for a while */
            int l3 = -1, k3 = 0;
            s32 d3 = 0;
            if (c->ru_n > 0 && (k - c->ru_at > 60 || c->ru_at - k > 300)) c->ru_n = 0;
            if (c->ru_n == 0 && join(c, x, y, JOIN_PX, c->cl, &l3, &k3, &d3) && l3 != c->cl) {
                /* another line from here (a branch the route tool made for this world: the gimmick gone, the
                 * platform elsewhere): that one first, this one is not taken here for a while */
                add_fail(c, c->cl, x, y);
            } else if (c->ru_n < 2) {
                c->ru_at = c->ru_n == 0 ? k : c->ru_at;
                c->ru_l = c->cl;
                c->ru_k = c->ru_at - 90 * (c->ru_n + 1);
                if (c->ru_k < 0) c->ru_k = 0;
                c->ru_n++;
                c->rec = 4; c->rec_t = 300;
            } else {
                add_fail(c, c->cl, x, y); c->ru_n = 0;   /* the line fails here (the world is not as it was) */
            }
        } else if (c->tl >= 0) { add_fail(c, c->tl, x, y); c->tl = -1; }   /* out of reach from here */
        if (c->rec == 0) {
            c->rec = 1 + (c->rec_n % 4);
            if (c->rec == 4) c->rec = 5;   /* (4: the run-up) */
            c->rec_n++;
            c->rec_t = c->rec == 2 ? 30 + (int)(rnd(c) % 30) : c->rec == 5 ? 120 + (int)(rnd(c) % 180) : 36;
        }
        c->best_t = c->t;
        c->st.recover++;
    }
    if (on && c->rec == 0 && c->cl >= 0 && !riding && (P(c, c->cl, k)->pad & VSR_RIDE) && c->rw_l < 0) {
        /* the line rides here (ship, crane, rope, rocket...) and we are not on board: the ride went without
         * us (we came late or early). Further on the line runs into the air: wait at the boarding place */
        int b = k;
        while (b > 0 && (P(c, c->cl, b - 1)->pad & VSR_RIDE)) b--;
        if (k - b >= 6 && b > 0) { c->rw_l = c->cl; c->rw_b = b; c->rw_t = 0; }
    }
    if (c->rw_l >= 0) {
        if (riding || c->rw_t > 600 || (p->pf & DCP_PF_DEAD)) {
            if (!riding && c->rw_t > 600) add_fail(c, c->rw_l, x, y);   /* it does not come back (10 s): another way */
            if (riding) { c->cl = c->rw_l; c->k = c->rw_b; }
            c->rw_l = -1;
        } else {
            s32 bx, by;
            before(c, c->rw_l, c->rw_b, &bx, &by);
            c->rw_t++;
            c->best_t = c->t;
            c->st.off_line++;
            if (sq(bx - x, by - y) > 20 * 20 && c->rw_t < 300) h = steer(c, p, bx, by);
            else h = (c->rw_t % 180) < 4 ? VSC_A : 0;   /* at the boarding place: wait, a hop now and then */
            goto rival_check;
        }
    }
    if (on && c->rec == 0 && c->cl == -2 && rival != NULL && rival->chara != p->chara) {
        /* on the rival's path, the rival the other character (its moves are not ours: Blaze's hover,
         * Sonic's air dash): after it with our own moves, to its place 0.5 s further on */
        s32 tx, ty;
        int a = k + 30 < c->rvn ? k + 30 : c->rvn - 1;
        rv_before(c, a, &tx, &ty);
        c->st.on_line++;
        c->wait = 0; c->tl = -1;
        h = steer(c, p, tx, ty);
    } else if (on && c->rec == 0) {
        /* on the line: the pad the line used from here */
        const VsRoutePoint *q = c->cl == -2 ? &c->rv[k] : P(c, c->cl, k);
        const VsRoutePoint *q0 = c->cl == -2 ? &c->rv[k > 0 ? k - 1 : 0] : P(c, c->cl, k > 0 ? k - 1 : 0);
        u16 want = q->pad & (u16)~VSR_RIDE, edge = (u16)(want & ~q0->pad);
        c->st.on_line++;
        c->wait = 0; c->jump_hold = 0; c->tl = -1;
        if (k == 0) edge = want;
        /* a press we already hold: let go for a frame (buttons only: a direction let go every other frame
         * while the line waits for a press that does not take, e.g. a boost without tension, stalls the run) */
        if (edge & c->prev & VSC_BUTTONS) want &= (u16)~(edge & c->prev & VSC_BUTTONS);
        /* the line holds A from before here and we do not (we joined it, or let go): pressing it now would
         * jump where the line did not (its press was long ago, the jump done or not taken). Not pressed
         * until the line lets go and presses again; a press of the last 3 frames is still made (late) */
        if (c->cl >= 0 && k > 0 && (want & q0->pad & VSC_A) && !(c->prev & VSC_A)) {
            int j = k - 1;
            while (j > 0 && k - j <= 4 && (P(c, c->cl, j - 1)->pad & VSC_A)) j--;
            if (k - j > 3) want &= (u16)~VSC_A;
        }
        if (c->cl >= 0 && (want & VSC_X) && skip_boost(c, k, behind)) want &= (u16)~VSC_X;
        h = want;
    } else if (c->rec) {
        u16 fw = VSC_RIGHT, bk = VSC_LEFT;
        if (k >= 0 && c->cl >= 0) {
            int a = k + 30 < (int)c->ln[c->cl].count ? k + 30 : (int)c->ln[c->cl].count - 1;
            if (P(c, c->cl, a)->x < x) { fw = VSC_LEFT; bk = VSC_RIGHT; }
        } else if (k >= 0 && c->cl == -2) {
            int a = k + 30 < c->rvn ? k + 30 : c->rvn - 1;
            if (c->rv[a].x < x) { fw = VSC_LEFT; bk = VSC_RIGHT; }
        }
        switch (c->rec) {
        case 4: {                                                                /* run-up: back to an earlier point */
            s32 tx, ty;
            before(c, c->ru_l, c->ru_k, &tx, &ty);
            if (sq(tx - x, ty - y) <= NEAR_PX * NEAR_PX) {
                c->cl = c->ru_l; c->k = c->ru_k; c->rec_t = 1; c->best_t = c->t; c->best_rem = 0x7fffffff;
                h = 0;
            } else {
                h = steer(c, p, tx, ty);
            }
            break;
        }
        case 1: h = fw | (c->rec_t > 8 ? VSC_A : 0); break;                      /* big jump on */
        case 5: h = 0; c->best_t = c->t; break;                                  /* wait (a ride, a platform, a door
                                                                                    comes back) */
        case 2: h = c->rec_t > 10 ? bk : (u16)(fw | VSC_A); break;               /* back off, run and jump */
        default:                                                                 /* spin dash */
            h = VSC_DOWN | ((c->rec_t > 10 && (c->rec_t % 3) == 0) ? VSC_A : 0);
            if (c->rec_t <= 10) h = fw;
            break;
        }
        if (--c->rec_t <= 0) c->rec = 0;
    } else if (c->nl > 0 && k >= 0 && c->cl >= 0 && !(c->t < c->lost_until && c->rvn > 0)) {
        /* off the line (lost: to the rival's path, below). A ride ahead (ship, cargo...) that left without us: wait at its boarding place */
        const VsRoutePoint *q = P(c, c->cl, k);
        int b = k;
        c->st.off_line++;
        while (b > 0 && (P(c, c->cl, b - 1)->pad & VSR_RIDE)) b--;
        if (!riding && (q->pad & VSR_RIDE) && b > 0 && c->wait < 3600) {
            s32 bx, by;
            before(c, c->cl, b, &bx, &by);
            if (sq(bx - x, by - y) < 96 * 96) {
                c->wait++;
                c->best_t = c->t;
                h = (c->wait % 240) < 4 ? VSC_A : 0;   /* a hop now and then */
                goto rival_check;
            }
        }
        c->wait = 0;
        /* back to the line point near with the least time left (every 8 frames, 320 px, then 1024) */
        if ((c->t & 7) == 0 || c->tl < 0) {
            int l2, k2;
            s32 d2;
            int ban = (int)c->t < c->ban_until ? c->ban : -1;
            if (join(c, x, y, 320, ban, &l2, &k2, &d2) || ((c->t & 31) == 0 && join(c, x, y, 1024, ban, &l2, &k2, &d2))) {
                int rem = (int)left(c, l2, k2) + isqrt(d2) / 4;
                c->tl = l2; c->tk = k2;
                if (rem < c->best_rem - 8) { c->best_rem = rem; c->best_t = c->t; }
            }
        }
        if (c->tl >= 0) {
            s32 tx, ty;
            before(c, c->tl, c->tk, &tx, &ty);
            h = steer(c, p, tx, ty);
        } else {
            int a = k + 40 < (int)c->ln[c->cl].count ? k + 40 : (int)c->ln[c->cl].count - 1;
            h = steer(c, p, P(c, c->cl, a)->x, P(c, c->cl, a)->y);
        }
    } else {
        /* no line near: the furthest place of the rival's path within reach, else straight on */
        s32 dr;
        int kr = c->rvn > 0 && (c->nl == 0 || c->incomplete || c->t < c->lost_until) ? rv_join(c, x, y, 1024, &dr) : -1;
        c->st.off_line++;
        if (kr >= 0) {
            s32 tx, ty;
            int a = kr + 20 < c->rvn ? kr + 20 : c->rvn - 1;
            rv_before(c, a, &tx, &ty);
            h = steer(c, p, tx, ty);
            if (-kr < c->best_rem - 8) { c->best_rem = -kr; c->best_t = c->t; }
        } else {
            h = steer(c, p, x + 64, y);
        }
    }
rival_check:
    /* the rival right in front at the same height, both on the ground: boost into it (knock-back) */
    if (rival != NULL && on && !(h & VSC_A) && (p->move_flags & DCP_MF_GROUND) && (rival->move_flags & DCP_MF_GROUND) &&
        p->tension > 0x600 && (c->attack_t == 0 || c->t - c->attack_t > 240) && DCP_ABS((rival->y >> 8) - y) < 24 &&
        DCP_ABS(p->gnd_vel) >= 0x600 && !crouch_near(c, k)) {   /* (running: not from a stand, a spin dash or a roll) */
        s32 dx = (rival->x >> 8) - x;
        if (((h & VSC_RIGHT) && dx > 8 && dx < 56) || ((h & VSC_LEFT) && dx < -8 && dx > -56)) {
            if (!(h & VSC_X)) { c->st.rival_attacks++; c->attack_t = c->t; }
            h |= VSC_X;
        }
    }
    h = unconfuse(p, h);
    c->st.line = c->cl; c->st.k = k;
    c->st.rem = (on && c->cl >= 0 && c->cl < c->nl) ? (int)left(c, c->cl, k) : -1;
    if (!on && c->tl >= 0) before(c, c->tl, c->tk, &c->st.tx, &c->st.ty); else { c->st.tx = c->st.ty = -1; }
out:
    if ((h & VSC_X) && !(c->prev & VSC_X)) c->st.boosts++;
    if ((h & VSC_A) && !(c->prev & VSC_A)) c->st.jumps++;
    *held = h;
    *pressed = h & ~c->prev;
    *released = c->prev & ~h;
    c->prev = h;
}
