/* Vs CPU racing line finder (host tool, native).
 *
 * Finds a way through one Battle Play stage for one character with the real game simulation
 * (vs_sim.c: player, objects, cameras, race rules) and writes the racing line the CPU follows
 * (vs_cpu.c): position and pad of every frame from the start to the goal ring.
 *
 * Search (exploration with an archive, "Go-Explore"): the stage is cut into 64 px cells; for each
 * cell reached the archive keeps a snapshot of the whole simulation (snap.c) and the fastest known
 * way there. Each round picks a promising cell (close to the goal by a distance field through the
 * stage's free space, rarely picked so far), restores it and plays a few hundred frames of pad
 * macros (run, jump, boost, spin dash, air action, back off), biased along the distance field.
 * New cells and faster ways in are kept. After the first goal the search goes on for a while for a
 * faster line. The result is replayed from the start snapshot and must reach the goal; -replay
 * checks a written line from a fresh process.
 *
 * usage: vs_route <stages> <player> <objdata> <sprites> <zNN> <chara 0/1> <out.rte> [-v] [-seed n] [-budget s]
 *        vs_route <stages> <player> <objdata> <sprites> <zNN> <chara 0/1> <in.rte> -replay
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <dirent.h>
#include "vs_sim.h"
#include "snap.h"
#include "../vs_cpu.h"
#include "../vs_route.h"
#include "../../objects/obj_internal.h"

#define CELL 16               /* distance field cell */
#define ACELL 64              /* archive cell */
#define MAXF 0x8c9f           /* the race time limit */
#define INF 0x3fffffff

static int GW, GH, AW, AH;
static int verbose, show_map;
static int s_win[4], s_win_on;   /* -win x0 y0 x1 y1: a 32 px map of that box after each search */
static u32 rng = 1;
static u32 rnd(void) { rng = rng * 1103515245u + 12345u; return (rng >> 8) & 0xffffff; }
static double rndf(void) { return rnd() / (double)0x1000000; }

/* ------------------------------------------------------------ generation key */
#ifndef VSR_SRC
#define VSR_SRC "unknown"        /* the Makefile passes a hash of the simulation and tool sources */
#endif
static u32 crc_tab[256];
static u32 crc32_add(u32 c, const u8 *b, size_t n) {
    size_t i;
    if (!crc_tab[1]) {
        u32 k, v;
        int j;
        for (k = 0; k < 256; k++) { v = k; for (j = 0; j < 8; j++) v = (v & 1) ? 0xedb88320u ^ (v >> 1) : v >> 1; crc_tab[k] = v; }
    }
    c = ~c;
    for (i = 0; i < n; i++) c = crc_tab[(c ^ b[i]) & 0xff] ^ (c >> 8);
    return ~c;
}
static u32 crc_file(u32 c, const char *path) {
    FILE *f = fopen(path, "rb");
    u8 buf[8192];
    size_t n;
    if (!f) return crc32_add(c, (const u8 *)"-", 1);
    while ((n = fread(buf, 1, sizeof buf, f)) > 0) c = crc32_add(c, buf, n);
    fclose(f);
    return c;
}
static int cmp_str(const void *a, const void *b) { return strcmp(*(const char *const *)a, *(const char *const *)b); }
/* the inputs a line depends on: the stage, the object data and hit boxes, the player tables, the version
 * of the format and the sources of the simulation and this tool */
static u32 gen_key(const char *stages, const char *player, const char *objdata, const char *stage, int chara) {
    char path[512];
    u32 c = 0;
    DIR *d;
    struct dirent *e;
    char *names[4096];
    int n = 0, i;
    c = crc32_add(c, (const u8 *)VSR_SRC, strlen(VSR_SRC));
    snprintf(path, sizeof path, "v%d %s %d", VS_ROUTE_VERSION, stage, chara); c = crc32_add(c, (const u8 *)path, strlen(path));
    snprintf(path, sizeof path, "%s/%s/level.bin", stages, stage); c = crc_file(c, path);
    snprintf(path, sizeof path, "%s/objdata.bin", objdata); c = crc_file(c, path);
    snprintf(path, sizeof path, "%s/physics.bin", player); c = crc_file(c, path);
    snprintf(path, sizeof path, "%s/trig.bin", player); c = crc_file(c, path);
    snprintf(path, sizeof path, "%s/hbx", objdata);
    if ((d = opendir(path)) != NULL) {
        while ((e = readdir(d)) != NULL && n < 4096)
            if (e->d_name[0] != '.') { names[n] = (char *)malloc(strlen(e->d_name) + 1); strcpy(names[n++], e->d_name); }
        closedir(d);
        qsort(names, (size_t)n, sizeof(char *), cmp_str);
        for (i = 0; i < n; i++) {
            snprintf(path, sizeof path, "%s/hbx/%s", objdata, names[i]);
            c = crc32_add(c, (const u8 *)names[i], strlen(names[i]));
            c = crc_file(c, path);
            free(names[i]);
        }
    }
    return c ? c : 1;
}

/* object events seen while exploring (outside the snapshots) */
static struct { const char *name; long n; } s_ev[32];
static void hk_log(void *ctx, const char *event, s32 a, s32 b) {
    int i;
    (void)ctx; (void)a; (void)b;
    for (i = 0; i < 32 && s_ev[i].name; i++) if (!strcmp(s_ev[i].name, event)) { s_ev[i].n++; return; }
    if (i < 32) { s_ev[i].name = event; s_ev[i].n = 1; }
}

/* ------------------------------------------------------------ distance field */
static int solid_pt(s32 x, s32 y, int l) {
    return rlvl_probe_distance_long(&g_sim.map, x, y, (u32)l | RLVL_Q_SKIP_TOP, RLVL_DIR_DOWN, NULL, NULL) <= 0;
}
/* a 16 px cell the player can pass: in one of the two collision layers, no solid point on the
 * cell's vertical and horizontal centre lines (8 px floors and walls block) */
static int free_cell(s32 x0, s32 y0) {
    int l, k;
    if (x0 < 0 || y0 < 0 || x0 >= g_sim.map.right || y0 >= g_sim.map.bottom) return 0;
    if (y0 + CELL > g_dcp_world.death_y - 48) return 0;   /* the bottom band: falling out, not a way */
    for (l = 0; l < 2; l++) {
        int ok = 1;
        for (k = 2; k < CELL && ok; k += 4)
            if (solid_pt(x0 + CELL / 2, y0 + k, l) || solid_pt(x0 + k, y0 + CELL / 2, l)) ok = 0;
        if (ok) return 1;
    }
    return 0;
}
/* The guide: a chain of targets ending at the goal ring (plain: the goal alone; when that search
 * fails: the checkpoint markers in their order, bit 0x1, 0x2, 0x4... of the entry flags, then the goal).
 * Field s = distance through free space to target s; potential(x, y, s) = field s + the chain after it. */
#define MAXW 12
static s32 *fields[MAXW];
static s32 wx[MAXW], wy[MAXW], wrest[MAXW];
static int NW;
static u8 *s_free;
/* cells with something to stand on within 160 px below (any floor of either layer, jump-through ones
 * too): the way the guide prefers; free space far above any floor costs 3x (a way through the air needs
 * a spring, a ride or a launcher) */
static u8 *s_near;
static void near_floor(void) {
    u8 *fl;
    int i, j, gx, gy;
    fl = (u8 *)calloc((size_t)GW * GH, 1);
    s_near = (u8 *)calloc((size_t)GW * GH, 1);
    for (i = 0; i < GW * GH; i++) {
        s32 x = (i % GW) * CELL + CELL / 2, y0 = (i / GW) * CELL, k;
        for (k = 0; k < CELL && !fl[i]; k += 4)
            if (rlvl_probe_distance_long(&g_sim.map, x, y0 + k, 0, RLVL_DIR_DOWN, NULL, NULL) <= 0 ||
                rlvl_probe_distance_long(&g_sim.map, x, y0 + k, 1, RLVL_DIR_DOWN, NULL, NULL) <= 0) fl[i] = 1;
    }
    for (gx = 0; gx < GW; gx++) {
        int below = 1000;   /* cells down to the next floor */
        for (gy = GH - 1; gy >= 0; gy--) {
            j = gy * GW + gx;
            if (fl[j]) below = 0; else below++;
            s_near[j] = below <= 10 && (g_sim.map.bottom <= 0 || gy * CELL < g_dcp_world.death_y - 48);
        }
    }
    free(fl);
}
static void field_to(int s) {
    int *hp, hn = 0, i, gx, gy;
    s32 *dist;
    if (!s_free) {
        GW = g_sim.map.right / CELL + 1; GH = g_sim.map.bottom / CELL + 1;
        s_free = (u8 *)malloc((size_t)GW * GH);
        for (i = 0; i < GW * GH; i++) s_free[i] = (u8)free_cell((i % GW) * CELL, (i / GW) * CELL);
        near_floor();
    }
    if (!fields[s]) fields[s] = (s32 *)malloc(sizeof(s32) * GW * GH);
    dist = fields[s];
    hp = (int *)malloc(sizeof(int) * (size_t)GW * GH * 4);
    for (i = 0; i < GW * GH; i++) dist[i] = INF;
    gx = wx[s] / CELL; gy = wy[s] / CELL;
    if (gx >= GW) gx = GW - 1;
    if (gy >= GH) gy = GH - 1;
    dist[gy * GW + gx] = 0;
    hp[hn++] = gy * GW + gx;
    /* Dijkstra (binary heap of cells, stale entries skipped by their distance) */
#define HLESS(a, b) (dist[hp[a]] < dist[hp[b]])
    while (hn > 0) {
        int c = hp[0], cx, cy, dx, dy, q = 0;
        s32 dc;
        hp[0] = hp[--hn];
        for (;;) {   /* sift down */
            int l = 2 * q + 1, r = l + 1, m = q, t;
            if (l < hn && HLESS(l, m)) m = l;
            if (r < hn && HLESS(r, m)) m = r;
            if (m == q) break;
            t = hp[m]; hp[m] = hp[q]; hp[q] = t; q = m;
        }
        dc = dist[c]; cx = c % GW; cy = c / GW;
        for (dy = -1; dy <= 1; dy++)
            for (dx = -1; dx <= 1; dx++) {
                int nx = cx + dx, ny = cy + dy, n, k;
                s32 nd;
                if ((!dx && !dy) || nx < 0 || ny < 0 || nx >= GW || ny >= GH) continue;
                n = ny * GW + nx;
                if (!s_free[n]) continue;
                nd = dc + (s_near[c] ? CELL : 3 * CELL);   /* (the field runs from the goal: c is the step's far end) */
                if (nd >= dist[n] || hn >= GW * GH * 4) continue;
                dist[n] = nd;
                k = hn++;
                hp[k] = n;
                while (k > 0 && HLESS(k, (k - 1) / 2)) { int t = hp[k]; hp[k] = hp[(k - 1) / 2]; hp[(k - 1) / 2] = t; k = (k - 1) / 2; }
            }
    }
#undef HLESS
    free(hp);
}
static s32 field_at(int s, s32 x, s32 y) {
    int cx = x / CELL, cy = y / CELL, r, dx, dy;
    s32 best = INF, *dist = fields[s];
    for (r = 0; r <= 3 && best == INF; r++)
        for (dy = -r; dy <= r; dy++)
            for (dx = -r; dx <= r; dx++) {
                int nx = cx + dx, ny = cy + dy;
                if (nx < 0 || ny < 0 || nx >= GW || ny >= GH) continue;
                if (dist[ny * GW + nx] < INF && dist[ny * GW + nx] + r * CELL < best) best = dist[ny * GW + nx] + r * CELL;
            }
    return best == INF ? 1000000 : best;
}
static s32 potential(s32 x, s32 y, int s) { return field_at(s, x, y) + wrest[s]; }
/* chain = the goal (markers 0) or the markers in order then the goal (markers 1) */
static int set_chain(int markers) {
    const rlvl_header_t *h = g_sim.hdr;
    const rlvl_object_t *o = (const rlvl_object_t *)rlvl_section(h, RLVL_SEC_OBJECTS, NULL);
    int s, bit;
    u32 i;
    NW = 0;
    if (markers)
        for (bit = 0; bit < 16 && NW < MAXW - 1; bit++)
            for (i = 0; i < h->num_objects; i++)
                if (o[i].type == 0x6e && (o[i].flags & 0x3fff) == (1u << bit)) { wx[NW] = o[i].x; wy[NW] = o[i].y - 16; NW++; }
    wx[NW] = g_sim.goal_x; wy[NW] = g_sim.goal_y; NW++;
    for (s = 0; s < NW; s++) field_to(s);
    wrest[NW - 1] = 0;
    for (s = NW - 2; s >= 0; s--) wrest[s] = field_at(s + 1, wx[s], wy[s]) + wrest[s + 1];
    return NW;
}
/* the stage of the chain for a position nobody tracked: the best one (skipping allowed) */
static int stage_for(s32 x, s32 y) {
    int s, bs = NW - 1;
    s32 b = INF;
    for (s = 0; s < NW; s++) { s32 v = potential(x, y, s); if (v < b) { b = v; bs = s; } }
    return bs;
}

/* ------------------------------------------------------------ macros */
/* a pad pattern relative to the direction fwd (VSC_RIGHT / VSC_LEFT), f = frame in the macro */
#define NMACRO 21
static u16 macro_pad(int m, int f, u16 fwd) {
    u16 back = fwd == VSC_RIGHT ? VSC_LEFT : VSC_RIGHT;
    switch (m) {
    case 0: return fwd;                                           /* run */
    case 1: return fwd | VSC_X;                                   /* boost */
    case 2: return fwd | VSC_A;                                   /* full jump */
    case 3: return fwd | (f < 4 ? VSC_A : 0);                     /* short hop */
    case 4: return back;                                          /* back off */
    case 5: return back | VSC_A;                                  /* jump back */
    case 6: return 0;                                             /* let go (brake, wait) */
    case 7: return f < 9 ? (u16)(VSC_DOWN | ((f % 3) == 1 ? VSC_A : 0)) : fwd; /* spin dash */
    case 8: return fwd | (f < 2 ? VSC_R : 0);                     /* air action / trick */
    case 9: return VSC_A;                                         /* jump in place */
    case 10: return fwd | VSC_A | VSC_X;                          /* boost jump */
    case 11: return fwd | (f < 6 ? VSC_A : 0) | (f >= 8 && f < 10 ? VSC_R : 0); /* jump + air action */
    case 12: return VSC_UP;                                       /* look up (gimmicks) */
    case 13: return VSC_DOWN | fwd;                               /* roll */
    case 14: return VSC_DOWN;                                     /* crouch (shooters charge, let go to fire) */
    case 15: return fwd | VSC_UP;                                 /* air steering up (gliders, swings) */
    case 16: return (u16)((f < 2 ? VSC_R | VSC_UP : 0) | (f >= 6 && f < 8 ? VSC_A : 0) | (f >= 12 && f < 14 ? VSC_B : 0)); /* trick up + combo */
    case 17: return (u16)(fwd | (f < 2 ? VSC_R : 0) | (f >= 8 && f < 10 ? VSC_A : 0));   /* trick forward + combo */
    case 19: return (u16)(fwd | (f < 8 ? VSC_A : 0) | (f >= 10 ? VSC_X : 0)); /* jump, then boost in the air (tension) */
    case 20: return (u16)(fwd | VSC_X);                           /* boost in the air (falling, off a ledge) */
    case 18: return 0;                                            /* a long wait (timed gimmicks: platforms that come
                                                                     every minute, doors, catapults coming back) */
    }
    return 0;
}
static const int W_MACRO[NMACRO] = { 12, 13, 15, 8, 6, 4, 9, 3, 5, 4, 7, 5, 1, 2, 2, 0, 0, 0, 1, 3, 0 };   /* sum 100 (15..17, 20: below) */

/* ------------------------------------------------------------ archive search */
typedef struct Ent {
    Snap snap;
    int parent;              /* entry the segment starts from (-1: the search's start) */
    u16 *seg; int nseg;      /* pads from the parent */
    u32 frame;               /* frames from the search's start */
    s32 pot, x, y;
    int chosen, alive, fails, gains;
    int stage;               /* targets of the chain passed */
    int grounded;
} Ent;
static Ent *E;
static int NE, CAPE;
static int *cellv;            /* archive cell -> entry (-1) */
/* an archive cell's state besides the place: in the air, the collision layer (paths that cross), a
 * switch timer running (the gimmicks it opens: neon routes, layer zones) */
#define NSUB 8
static int sub_state(const DcPlayer *p) {
    const s16 *sw = obj_switch_timers();
    int i, on = 0;
    for (i = 0; i < 32 && !on; i++) on = sw[i] != 0;
    return (int)((p->move_flags & DCP_MF_GROUND) == 0) | (int)(p->obj_flags & 1) << 1 | on << 2;
}
static s32 best_pot;
static int s_partial;          /* search(): keep the way to the furthest grounded place when no goal */
static u16 *s_partial_pads;
static u32 s_partial_len;
static s32 s_partial_pot;
static int best_ent, stagnant;

/* the archive's snapshots are kept under 1.2 GB (two tools at once on an 8 GB machine): past that the
 * search goes on with the cells it has */
#define ARCHIVE_BYTES (1200u << 20)
static size_t s_arch_bytes;
static void archive_reset(void) {
    int i;
    for (i = 0; i < NE; i++) { snap_free(&E[i].snap); free(E[i].seg); }
    s_arch_bytes = 0;
    NE = 0;
    for (i = 0; i < AW * AH * MAXW * NSUB; i++) cellv[i] = -1;
    best_pot = INF; best_ent = 0; stagnant = 0;
}
static int add_entry(int parent, const u16 *seg, int nseg, u32 frame, s32 pot, s32 x, s32 y, int stage) {
    Ent *e;
    if (NE == CAPE) { CAPE = CAPE ? CAPE * 2 : 1024; E = (Ent *)realloc(E, sizeof(Ent) * (size_t)CAPE); }
    e = &E[NE];
    memset(e, 0, sizeof *e);
    snap_take(&e->snap);
    s_arch_bytes += sizeof(unsigned) * 2 * e->snap.n + sizeof(u16) * (size_t)nseg + sizeof(Ent);
    e->parent = parent;
    e->nseg = nseg;
    e->seg = (u16 *)malloc(sizeof(u16) * (size_t)(nseg ? nseg : 1));
    if (nseg) memcpy(e->seg, seg, sizeof(u16) * (size_t)nseg);
    e->frame = frame; e->pot = pot; e->x = x; e->y = y; e->alive = 1; e->stage = stage;
    e->grounded = (g_sim.p[1].move_flags & DCP_MF_GROUND) != 0;
    if (pot < best_pot) { best_pot = pot; best_ent = NE; }
    return NE++;
}
/* the pads from the search's start to entry i, then seg */
static u16 *path_to(int i, const u16 *seg, int nseg, u32 *len) {
    u32 n = (u32)nseg, k;
    int j;
    u16 *out;
    for (j = i; j >= 0; j = E[j].parent) n += (u32)E[j].nseg;
    out = (u16 *)malloc(sizeof(u16) * (n ? n : 1));
    k = n - (u32)nseg;
    if (nseg) memcpy(out + k, seg, sizeof(u16) * (size_t)nseg);
    for (j = i; j >= 0; j = E[j].parent) {
        k -= (u32)E[j].nseg;
        if (E[j].nseg) memcpy(out + k, E[j].seg, sizeof(u16) * (size_t)E[j].nseg);
    }
    *len = n;
    return out;
}
static int pick(void) {
    double tot = 0, r, T = 700.0 * (1.0 + stagnant / 1000.0);
    int i;
    static double *w;
    static int wcap;
    if (wcap < NE) { wcap = NE * 2; w = (double *)realloc(w, sizeof(double) * (size_t)wcap); }
    for (i = 0; i < NE; i++) {
        Ent *e = &E[i];
        w[i] = e->alive ? exp(-(double)(e->pot - best_pot) / T) / sqrt(1.0 + e->chosen) / (1.0 + 0.5 * e->fails) *
                          (1.0 + 0.2 * (e->gains > 5 ? 5 : e->gains)) : 0;
        tot += w[i];
    }
    r = rndf() * tot;
    for (i = 0; i < NE; i++) { r -= w[i]; if (r <= 0) return i; }
    return NE - 1;
}

/* merge mode (branches): reaching an earlier line going the same way ends the search like the goal */
static int s_merge;
static int merge_hit(s32 x, s32 y, s32 dx, s32 dy);
static s32 s_px[4096], s_py[4096];   /* positions in the round (merge direction) */

/* Search from the simulation's current state (its frame counter: frame0) to the goal ring for at most
 * max_rounds rounds (budget seconds at most); after the first goal it goes on for a faster way (6000 rounds).
 * Returns the pads (malloc) and their count, NULL when no goal was found. */
static u16 *search(u32 frame0, int max_rounds, double budget, u32 *out_len, const char *what) {
    int it, goal_it = -1, k;
    u32 best_goal = INF, goal_len = 0;
    u16 *goal_pads = NULL, *seg = (u16 *)malloc(sizeof(u16) * 4096);
    clock_t t0 = clock();
    archive_reset();
    {
        DcPlayer *p = &g_sim.p[1];
        s32 x = p->x >> 8, y = p->y >> 8;
        int c = (y / ACELL) * AW + x / ACELL, st = stage_for(x, y);
        if (c < 0 || c >= AW * AH) c = 0;
        cellv[(c * MAXW + st) * NSUB + sub_state(p)] = add_entry(-1, seg, 0, frame0, potential(x, y, st), x, y, st);
    }
    for (it = 0;; it++) {
        int ei = pick(), n = 0, m = 0, mlen = 0, last_ent = ei, seg_start = 0, mf = 0, ne = NE, died = 0, st;
        s32 bp = best_pot;
        u32 frame;
        double el = (double)(clock() - t0) / CLOCKS_PER_SEC;
        /* limits in rounds (the same line on every machine); the time is only a safety cap */
        if (it >= max_rounds || el > budget) break;
        if (goal_it >= 0 && it - goal_it > (s_merge ? 1500 : 6000)) break;
        E[ei].chosen++;
        snap_restore(&E[ei].snap);
        frame = E[ei].frame;
        st = E[ei].stage;
        while ((n < 400 || (m == 18 && mlen > 0)) && n < 4000 && frame < MAXF) {   /* a long wait goes on past 400 */
            DcPlayer *p = &g_sim.p[1];
            s32 x, y;
            int c;
            u16 fwd;
            if (mlen == 0) {
                int r = (int)(rnd() % 100), s = 0;
                if ((p->pf & DCP_PF_TRICKABLE) && !(p->move_flags & DCP_MF_GROUND) && r < 45) {
                    /* launched (spring, ramp, dash ring): tricks (tension) or steering in the air */
                    m = r < 18 ? 16 : r < 34 ? 17 : 15;
                    mlen = 16 + (int)(rnd() % 16);
                } else if (!(p->move_flags & DCP_MF_GROUND) && r < 12) {
                    m = r < 6 ? 15 : 20; mlen = 8 + (int)(rnd() % 30);    /* steering / boost in the air */
                } else if (p->gimmick != NULL) {
                    /* carried by a gimmick (ship, rail, cargo, hand...): mostly stay on board, hop over hazards */
                    m = r < 55 ? 6 : r < 75 ? 9 : r < 85 ? 3 : r < 92 ? 0 : 4;
                    mlen = m == 6 ? 10 + (int)(rnd() % 60) : 4 + (int)(rnd() % 12);
                } else {
                    for (m = 0; m < NMACRO; m++) { s += W_MACRO[m]; if (r < s) break; }
                    if (m >= NMACRO) m = 0;
                    mlen = m == 6 ? 20 + (int)(rnd() % 100) : m == 14 ? 12 + (int)(rnd() % 60) : m == 18 ? 120 + (int)(rnd() % 3300) :
                           6 + (int)(rnd() % 30);   /* waiting: rides, platforms */
                }
                mf = 0;
            }
            x = p->x >> 8; y = p->y >> 8;
            fwd = potential(x + 48, y, st) <= potential(x - 48, y, st) ? VSC_RIGHT : VSC_LEFT;
            seg[n] = macro_pad(m, mf++, fwd);
            vs_sim_frame(0, seg[n]);
            s_px[n] = p->x >> 8; s_py[n] = p->y >> 8;
            n++; frame++; mlen--;
            if (vs_sim_finished(1) || (s_merge && n > 4 && frame - frame0 >= 20 &&
                                       merge_hit(p->x >> 8, p->y >> 8, s_px[n - 1] - s_px[n - 5], s_py[n - 1] - s_py[n - 5]))) {
                if (frame < best_goal) {
                    free(goal_pads);
                    goal_pads = path_to(last_ent, seg + seg_start, n - seg_start, &goal_len);
                    best_goal = frame;
                    if (goal_it < 0) goal_it = it;
                    if (verbose) printf("  %s: goal at frame %u (round %d, %.1f s, archive %d)\n", what, frame, it, el, NE);
                }
                break;
            }
            if (p->pf & DCP_PF_DEAD) { died = 1; break; }
            x = p->x >> 8; y = p->y >> 8;
            if (x < 0 || y < 0 || x >= g_sim.map.right || y >= g_sim.map.bottom) break;
            if (p->state == dcp_state_hurt) continue;
            if (y > g_dcp_world.death_y - 96 && p->vel_y > 0 && !(p->move_flags & DCP_MF_GROUND)) continue;   /* falling out */
            if (st < NW - 1 && (x - wx[st]) * (x - wx[st]) + (y - wy[st]) * (y - wy[st]) < 96 * 96) st++;   /* target passed */
            c = (((y / ACELL) * AW + x / ACELL) * MAXW + st) * NSUB + sub_state(p);   /* + air, layer, switches */
            if ((cellv[c] < 0 || E[cellv[c]].frame > frame + 4) && s_arch_bytes < ARCHIVE_BYTES) {
                if (cellv[c] >= 0) E[cellv[c]].alive = 0;
                cellv[c] = add_entry(last_ent, seg + seg_start, n - seg_start, frame, potential(x, y, st), x, y, st);
                last_ent = cellv[c];
                seg_start = n;
            }
        }
        if (died) E[ei].fails++;
        if (NE > ne) E[ei].gains++;
        if (best_pot < bp) stagnant = 0; else stagnant++;
        if (verbose && it % 5000 == 0)
            printf("  %s: round %6d %.1f s: archive %d, best potential %ld at %ld,%ld (frame %u), goal %u\n", what, it, el, NE,
                   (long)best_pot, (long)E[best_ent].x, (long)E[best_ent].y, E[best_ent].frame, best_goal);
    }
    (void)k;
    if (show_map) {   /* where the search has been: 256 px cells (# reached, G goal, P start, * best) */
        int gw = g_sim.map.right / 256 + 1, gh = g_sim.map.bottom / 256 + 1, i, gx, gy;
        char *m = (char *)malloc((size_t)gw * gh);
        memset(m, '.', (size_t)gw * gh);
        for (i = 0; i < NE; i++) m[(E[i].y / 256) * gw + E[i].x / 256] = '#';
        m[(g_sim.goal_y / 256) * gw + g_sim.goal_x / 256] = 'G';
        m[(g_sim.start_y / 256) * gw + g_sim.start_x / 256] = 'P';
        m[(E[best_ent].y / 256) * gw + E[best_ent].x / 256] = '*';
        for (gy = 0; gy < gh; gy++) { for (gx = 0; gx < gw; gx++) putchar(m[gy * gw + gx]); putchar('\n'); }
        free(m);
    }
    if (s_win_on) {   /* 32 px cells: g reached on the ground, a in the air only, o an object, G goal */
        int gw = (s_win[2] - s_win[0]) / 32 + 1, gh = (s_win[3] - s_win[1]) / 32 + 1, i, gx, gy;
        const rlvl_object_t *ob = (const rlvl_object_t *)rlvl_section(g_sim.hdr, RLVL_SEC_OBJECTS, NULL);
        char *m = (char *)malloc((size_t)gw * gh);
        memset(m, '.', (size_t)gw * gh);
        for (i = 0; i < (int)(gw * gh); i++) {
            s32 cx = s_win[0] + (i % gw) * 32, cy = s_win[1] + (i / gw) * 32, q;
            if (!free_cell(cx + 8, cy + 8)) { m[i] = ' '; continue; }
            for (q = 0; q < 32; q += 4)   /* a top-only (jump-through) floor in the cell */
                if (rlvl_probe_distance_long(&g_sim.map, cx + 16, cy + q, 0, RLVL_DIR_DOWN, NULL, NULL) <= 0 ||
                    rlvl_probe_distance_long(&g_sim.map, cx + 16, cy + q, 1, RLVL_DIR_DOWN, NULL, NULL) <= 0) { m[i] = '='; break; }
        }
        for (i = 0; i < (int)g_sim.hdr->num_objects; i++) {
            gx = (ob[i].x - s_win[0]) / 32; gy = (ob[i].y - s_win[1]) / 32;
            if (ob[i].x >= s_win[0] && gx < gw && ob[i].y >= s_win[1] && gy < gh) m[gy * gw + gx] = 'o';
        }
        for (i = 0; i < NE; i++) {
            gx = (E[i].x - s_win[0]) / 32; gy = (E[i].y - s_win[1]) / 32;
            if (E[i].x < s_win[0] || E[i].y < s_win[1] || gx >= gw || gy >= gh) continue;
            if (E[i].grounded) m[gy * gw + gx] = 'g'; else if (m[gy * gw + gx] != 'g') m[gy * gw + gx] = 'a';
        }
        for (gy = 0; gy < gh; gy++) { printf("%6d ", s_win[1] + gy * 32); for (gx = 0; gx < gw; gx++) putchar(m[gy * gw + gx]); putchar('\n'); }
        free(m);
    }
    free(seg);
    if (!goal_pads && s_partial) {
        /* no goal: the way to the furthest place reached on the ground (a partial line) */
        int i, bi = -1;
        for (i = 0; i < NE; i++)
            if (E[i].grounded && E[i].y < g_dcp_world.death_y - 96 && (bi < 0 || E[i].pot < E[bi].pot)) bi = i;
        if (bi >= 0) {
            u32 len;
            u16 *pp = path_to(bi, NULL, 0, &len);
            free(s_partial_pads);
            s_partial_pads = pp; s_partial_len = len; s_partial_pot = E[bi].pot;
        }
    }
    *out_len = goal_pads ? goal_len : 0;
    if (verbose) printf("  %s: %s, %u frames, %d rounds, archive %d, %.1f s\n", what, goal_pads ? "goal" : "no goal",
                        goal_pads ? goal_len : 0, it, NE, (double)(clock() - t0) / CLOCKS_PER_SEC);
    return goal_pads;
}

/* ------------------------------------------------------------ the lines */
typedef struct Line { VsRoutePoint *pt; u32 n, rest, t0; u16 sx, sy; } Line;
static Line L[VS_ROUTE_MAX_LINES];
static int NL;

/* play pads from the current state; the points (position after each frame, ride flag); returns the
 * points up to the goal, 0 when the goal is not reached */
static int s_record_partial;   /* record(): the pads need not reach the goal */
#define MAXNL 140               /* lines a file may get (VS_ROUTE_MAX_LINES 160 on the reader side) */
#define MAXPTS 400000u          /* points (2.4 MB) */
static u32 s_key;
static u32 record(const u16 *pads, u32 n, VsRoutePoint *out) {
    u32 i;
    for (i = 0; i < n; i++) {
        vs_sim_frame(0, pads[i]);
        out[i].x = (u16)(g_sim.p[1].x >> 8); out[i].y = (u16)(g_sim.p[1].y >> 8);
        out[i].pad = (u16)(pads[i] | (g_sim.p[1].gimmick != NULL ? VSR_RIDE : 0));
        if (vs_sim_finished(1)) return i + 1;
    }
    return s_record_partial ? n : 0;
}
/* a branch ends where it meets an earlier line going the same way: the first of its points (after 20
 * frames) within 12 px of a point of another line whose next 4 frames move the same way */
/* the earlier lines' points by 16 px cell (merge tests) */
static int *mg_head, *mg_next, *mg_l, mg_gw, mg_gh;
static u32 *mg_i;
static void merge_build(int upto) {
    int l;
    u32 tot = 0, i, a;
    free(mg_head); free(mg_next); free(mg_l); free(mg_i);
    mg_gw = g_sim.map.right / 16 + 1; mg_gh = g_sim.map.bottom / 16 + 1;
    for (l = 0; l < upto; l++) tot += L[l].n;
    mg_head = (int *)malloc(sizeof(int) * (size_t)mg_gw * mg_gh);
    mg_next = (int *)malloc(sizeof(int) * (tot ? tot : 1));
    mg_l = (int *)malloc(sizeof(int) * (tot ? tot : 1));
    mg_i = (u32 *)malloc(sizeof(u32) * (tot ? tot : 1));
    for (i = 0; i < (u32)(mg_gw * mg_gh); i++) mg_head[i] = -1;
    for (l = 0, a = 0; l < upto; l++)
        for (i = 0; i < L[l].n; i++, a++) {
            int c = (L[l].pt[i].y / 16) * mg_gw + L[l].pt[i].x / 16;
            mg_l[a] = l; mg_i[a] = i; mg_next[a] = mg_head[c]; mg_head[c] = (int)a;
        }
}
/* a point of an earlier line within 12 px of (x, y), moving the same way (its previous 4 frames against
 * dx, dy) at about the same speed */
static u32 s_merge_rem;   /* branches: join only points with fewer frames to the goal (0: any) */
static int merge_find(s32 x, s32 y, s32 dx, s32 dy, int *jl, u32 *jj) {
    int gx, gy;
    for (gy = y / 16 - 1; gy <= y / 16 + 1; gy++)
        for (gx = x / 16 - 1; gx <= x / 16 + 1; gx++) {
            int e;
            if (gx < 0 || gy < 0 || gx >= mg_gw || gy >= mg_gh) continue;
            for (e = mg_head[gy * mg_gw + gx]; e >= 0; e = mg_next[e]) {
                const Line *o = &L[mg_l[e]];
                u32 j = mg_i[e];
                s32 ex, ey, dox, doy, lb, lo;
                if (j < 4 || j + 1 >= o->n) continue;
                if (s_merge_rem && (u32)(o->n - j - 1) + o->rest >= s_merge_rem) continue;   /* not past the trouble */
                ex = o->pt[j].x - x; ey = o->pt[j].y - y;
                if (ex * ex + ey * ey > 10 * 10) continue;
                dox = o->pt[j].x - o->pt[j - 4].x; doy = o->pt[j].y - o->pt[j - 4].y;
                lb = dx * dx + dy * dy; lo = dox * dox + doy * doy;
                if (dx * dox + dy * doy <= 0 && (lb > 16 || lo > 16)) continue;   /* not the same way */
                if ((lb > 64 || lo > 64) && (lb * 4 > lo * 9 || lo * 4 > lb * 9)) continue; /* not the same speed (1.5x) */
                *jl = mg_l[e]; *jj = j;
                return 1;
            }
        }
    return 0;
}
static int merge_hit(s32 x, s32 y, s32 dx, s32 dy) { int l; u32 j; return merge_find(x, y, dx, dy, &l, &j); }
/* a branch ends where it meets an earlier line going the same way: its first such point after 20 frames */
static int join_point(const Line *b, u32 *at, int *jl, u32 *jj) {
    u32 i;
    merge_build(NL);
    for (i = 16; i < b->n; i++)
        if (merge_find(b->pt[i].x, b->pt[i].y, b->pt[i].x - b->pt[i - 4].x, b->pt[i].y - b->pt[i - 4].y, jl, jj)) {
            *at = i;
            return 1;
        }
    return 0;
}
static int add_line(const u16 *pads, u32 n) {
    Line *l;
    u32 at, jj;
    int jl;
    if (NL >= VS_ROUTE_MAX_LINES) return -1;
    l = &L[NL];
    l->sx = (u16)(g_sim.p[1].x >> 8); l->sy = (u16)(g_sim.p[1].y >> 8);
    l->t0 = g_dcp_world.frame;
    l->pt = (VsRoutePoint *)malloc(sizeof(VsRoutePoint) * (n ? n : 1));
    l->n = record(pads, n, l->pt);
    l->rest = 0;
    if (l->n == 0) { free(l->pt); return -1; }
    if (NL > 0 && join_point(l, &at, &jl, &jj)) {
        l->n = at + 1;
        l->rest = (L[jl].n - (jj + 1)) + L[jl].rest;
        if (verbose) printf("  branch %d: joins line %d at point %u after %u frames\n", NL, jl, jj, at + 1);
    } else if (NL > 0 && !vs_sim_finished(1)) {
        free(l->pt);   /* a merge branch that does not join (or reach the goal): dropped */
        return -1;
    }
    return NL++;
}
static u32 total_points(void) { u32 t = 0; int i; for (i = 0; i < NL; i++) t += L[i].n; return t; }
/* the route file in memory */
static u8 *build_file(const char *stage, int chara, u32 *size) {
    VsRouteHeader h;
    u32 tp = total_points(), first = 0, sz;
    u8 *b, *w;
    int i;
    sz = (u32)(sizeof h + NL * sizeof(VsRouteLine) + tp * sizeof(VsRoutePoint));
    b = w = (u8 *)malloc(sz);
    memset(&h, 0, sizeof h);
    memcpy(h.magic, "VSRT", 4);
    h.version = VS_ROUTE_VERSION;
    h.zone = (u8)(stage[1] - '1'); h.act = (u8)(stage[2] - '1'); h.chara = (u8)chara;
    h.nlines = (u16)NL; h.points = tp;
    h.flags = (u8)(L[0].rest ? VSR_INCOMPLETE : 0);
    h.key = s_key;
    memcpy(w, &h, sizeof h); w += sizeof h;
    for (i = 0; i < NL; i++) {
        VsRouteLine ln;
        ln.first = first; ln.count = L[i].n; ln.rest = L[i].rest; ln.sx = L[i].sx; ln.sy = L[i].sy; ln.t0 = L[i].t0;
        memcpy(w, &ln, sizeof ln); w += sizeof ln;
        first += L[i].n;
    }
    for (i = 0; i < NL; i++) { memcpy(w, L[i].pt, sizeof(VsRoutePoint) * L[i].n); w += sizeof(VsRoutePoint) * L[i].n; }
    *size = sz;
    return b;
}

/* ------------------------------------------------------------ shortcuts */
/* line 0 made faster: from a random point of it, a search for a way that meets line 0 again further on
 * (same place, way and speed) sooner than line 0 gets there; kept when the whole new line replays from
 * the start to the goal in fewer frames */
static u16 *shortcuts(const Snap *start, u16 *pads, u32 *n, int tries, int rounds) {
    int t, kept = 0;
    u32 n_in = *n;
    for (t = 0; t < tries && *n > 600; t++) {
        u32 k = 60 + rnd() % (*n - 400), i, nb = 0, j = 0, tot;
        u16 *p2, *cand;
        int jl;
        Snap at;
        char what[48];
        /* line 0 as it is now, for the merge tests */
        snap_restore(start);
        L[0].pt = (VsRoutePoint *)malloc(sizeof(VsRoutePoint) * *n);
        s_record_partial = 1;
        L[0].n = record(pads, *n, L[0].pt);
        s_record_partial = 0;
        L[0].rest = 0; NL = 1;
        merge_build(1);
        snap_restore(start);
        for (i = 0; i < k; i++) vs_sim_frame(0, pads[i]);
        memset(&at, 0, sizeof at);
        snap_take(&at);
        s_merge = 1; s_merge_rem = *n - k > 90 ? *n - k - 90 : 1;
        snprintf(what, sizeof what, "shortcut %d", t);
        p2 = search(k, rounds, 60, &nb, what);
        s_merge = 0;
        cand = NULL;
        if (p2) {
            /* where it met line 0 */
            s32 x4 = 0, y4 = 0;
            snap_restore(&at);
            for (i = 0; i < nb; i++) {
                if (i + 4 == nb) { x4 = g_sim.p[1].x >> 8; y4 = g_sim.p[1].y >> 8; }
                vs_sim_frame(0, p2[i]);
            }
            if (nb >= 5 && merge_find(g_sim.p[1].x >> 8, g_sim.p[1].y >> 8, (g_sim.p[1].x >> 8) - x4, (g_sim.p[1].y >> 8) - y4, &jl, &j) && jl == 0) {
                tot = k + nb + (*n - j - 1);
                if (tot + 30 < *n) {
                    cand = (u16 *)malloc(sizeof(u16) * (tot + 1));
                    memcpy(cand, pads, sizeof(u16) * k);
                    memcpy(cand + k, p2, sizeof(u16) * nb);
                    memcpy(cand + k + nb, pads + j + 1, sizeof(u16) * (*n - j - 1));
                    snap_restore(start);
                    for (i = 0; i < tot && !vs_sim_finished(1); i++) vs_sim_frame(0, cand[i]);
                    if (vs_sim_finished(1) && i + 30 < *n) {
                        if (verbose) printf("  shortcut %d at frame %u: %u -> %u frames\n", t, k, *n, i);
                        free(pads); pads = cand; *n = i; kept++;
                        cand = NULL;
                    }
                }
            }
            free(p2);
        }
        free(cand);
        s_merge_rem = 0;
        snap_free(&at);
        free(L[0].pt); L[0].pt = NULL; NL = 0;
    }
    if (verbose) printf("  shortcuts: %d kept, line 0 %u -> %u frames\n", kept, n_in, *n);
    return pads;
}

/* ------------------------------------------------------------ robustness trials */
/* the race's upsets for the CPU (player 2): from the rival's item boxes (ItemBox_GiveItem 8..10 on the
 * collector's rival), its boost attack (knock-back), the CPU's own slips (a missed jump, no boost for
 * a while, the easy skill's boost choices), a death (respawn into a world that went on: gimmicks used,
 * platforms in other phases) and two of them in one race */
enum { UP_SLOW, UP_CONFUSE, UP_DRAIN, UP_KNOCK, UP_NOJUMP, UP_NOBOOST, UP_EASY, UP_RIVAL, UP_RIVAL2, UP_DEATH, UP_TWO, UP_COUNT };
static const char *const UP_NAME[UP_COUNT] = { "slowdown", "confusion", "tension drain", "knock-back", "missed jump", "no boost",
                                                "easy skill", "rival", "rival + item", "death", "two upsets" };
#define TRIAL_SNAPS 96
static void upset(int u, u32 seed, DcPlayer *A, DcPlayer *B) {
    switch (u) {
    case UP_SLOW: g_obj_hooks.item(g_obj_hooks.ctx, A, OBJ_ITEM_VS_SLOW); break;
    case UP_CONFUSE: g_obj_hooks.item(g_obj_hooks.ctx, A, OBJ_ITEM_VS_CONFUSE); break;
    case UP_DRAIN: g_obj_hooks.item(g_obj_hooks.ctx, A, OBJ_ITEM_VS_DRAIN); break;
    case UP_RIVAL2: g_obj_hooks.item(g_obj_hooks.ctx, A, OBJ_ITEM_VS_SLOW + (int)(seed % 3)); break;
    case UP_KNOCK:
        if (B->move_flags & DCP_MF_GROUND) { B->vs_timer = 0xc; B->hitstop = 0x44; B->gnd_vel = (s16)(B->gnd_vel >= 0 ? -0x480 : 0x480); }
        break;
    case UP_DEATH: if (!(B->pf & DCP_PF_DEAD) && B->state != dcp_state_die) dcp_act_die(B); break;
    default: break;
    }
}
/* one trial: the CPU on the current lines from the start, upset u at frame fu. 0 = goal; else the
 * CPU stopped getting closer to the goal (its line's frames to the goal) for 7 s, or died three
 * times: the snapshot just before its last progress (after its last respawn) in *at, and the frames to
 * the goal it had there in *at_rem (a branch from there must join the lines past that) */
static int trial(const Snap *start, int u, u32 fu, u32 seed, const u8 *file, u32 fsize, Snap *at, u32 *at_frame, u32 *at_rem) {
    static Snap ring[TRIAL_SNAPS], stuck;
    static u32 ring_f[TRIAL_SNAPS], stuck_f, stuck_for;
    int nr = 0, i, dead = 0, deaths = 0, u2 = u, rival = u == UP_RIVAL || u == UP_RIVAL2;
    u32 f, limit = L[0].n * 2 + 2400, best_t = 0, life_t = 0, fu2 = 0xffffffffu;
    int best_rem = 0x7fffffff;
    stuck_for = 0xffffffffu;
    if (u == UP_TWO) {   /* two upsets of the plain kinds, 5..20 s apart */
        u = (int)(seed % 4); u2 = (int)((seed / 4) % 6);
        if (u2 == UP_NOJUMP || u2 == UP_NOBOOST) u2 = UP_DEATH;
        fu2 = fu + 300 + (seed / 24) % 900;
    }
    snap_restore(start);
    vs_cpu_begin(1, file, fsize, seed, u == UP_EASY ? VSC_EASY : VSC_HARD);
    /* the rival: player 1 (the other character) drives the same lines, starting up to 1.5 s apart: pushes,
     * boost attacks, the objects it takes first */
    if (rival) vs_cpu_begin(0, file, fsize, seed ^ 0x9e3779b9u, VSC_NORMAL);
    for (f = 0; f < limit && !vs_sim_finished(1); f++) {
        DcPlayer *A = &g_sim.p[0], *B = &g_sim.p[1];
        u16 h, pp, rr, h1 = 0;
        const VsCpuStats *st;
        if (f % 30 == 0) {
            Snap *s = &ring[nr % TRIAL_SNAPS];
            snap_free(s);
            snap_take(s);
            ring_f[nr % TRIAL_SNAPS] = f;
            nr++;
        }
        vs_cpu_pad(1, B, A, &h, &pp, &rr);
        if (rival && f >= (fu % 90)) vs_cpu_pad(0, A, B, &h1, &pp, &rr);
        if (f == fu) upset(u, seed, A, B);
        if (f == fu2) upset(u2, seed, A, B);
        if (u == UP_NOJUMP && f >= fu && f < fu + 20) h &= (u16)~VSC_A;
        if (u == UP_NOBOOST && f >= fu && f < fu + 300) h &= (u16)~VSC_X;
        vs_sim_frame(h1, h);
        st = vs_cpu_stats(1);
        if ((B->pf & DCP_PF_DEAD) && !dead) deaths++;
        if (!(B->pf & DCP_PF_DEAD) && dead) { best_rem = 0x7fffffff; best_t = life_t = f; }   /* respawned: progress anew */
        dead = (B->pf & DCP_PF_DEAD) != 0;
        if (!dead && st->rem >= 0 && st->rem < best_rem - 4) { best_rem = st->rem; best_t = f; }
        if (f == best_t + 180 && !dead) {   /* the frame the CPU finds itself stuck (vs_cpu.c, 3 s): the branch's start */
            snap_free(&stuck);
            snap_take(&stuck);
            stuck_f = f + 1; stuck_for = best_t;
        }
        if (vs_sim_finished(0)) break;   /* the rival won: player 2 stops (VS rules) */
        if ((!dead && f - best_t > 420) || deaths >= 3) break;
    }
    if (vs_sim_finished(1)) return 0;
    if (vs_sim_finished(0) && f - best_t < 300) return 0;   /* still getting on when the rival won */
    /* the snapshot 2 s after the last progress (where the CPU is when it finds itself stuck, so the branch
     * starts where it looks for another line), not before the last respawn */
    if (stuck_for == best_t && stuck.n && best_t >= life_t) {
        snap_free(at);
        at->n = stuck.n;
        at->w = (unsigned *)malloc(sizeof(unsigned) * 2 * (at->n ? at->n : 1));
        memcpy(at->w, stuck.w, sizeof(unsigned) * 2 * at->n);
        *at_frame = stuck_f;
        *at_rem = best_rem == 0x7fffffff ? 0 : (u32)best_rem;
        return 1;
    }
    {
        int bi = -1;
        u32 target = best_t + 120;
        if (target >= f) target = f > 30 ? f - 30 : 0;
        if (target < life_t) target = life_t;
        for (i = 0; i < nr && i < TRIAL_SNAPS; i++) {
            int j = (nr - 1 - i) % TRIAL_SNAPS;
            if (ring_f[j] <= target && (ring_f[j] >= life_t || i + 1 == nr || i + 1 == TRIAL_SNAPS)) { bi = j; break; }
        }
        if (bi < 0) bi = (nr - 1) % TRIAL_SNAPS;
        snap_free(at);
        at->n = ring[bi].n;
        at->w = (unsigned *)malloc(sizeof(unsigned) * 2 * (at->n ? at->n : 1));
        memcpy(at->w, ring[bi].w, sizeof(unsigned) * 2 * at->n);
        *at_frame = ring_f[bi];
        *at_rem = best_rem == 0x7fffffff ? 0 : (u32)best_rem;
    }
    return 1;
}

/* ------------------------------------------------------------ check a written file */
static int replay(const char *path) {
    FILE *f = fopen(path, "rb");
    long size;
    u8 *b;
    const VsRouteHeader *h;
    const VsRouteLine *ln;
    const VsRoutePoint *pt;
    u32 i, off = 0;
    if (!f) { printf("no route %s\n", path); return 2; }
    fseek(f, 0, SEEK_END); size = ftell(f); fseek(f, 0, SEEK_SET);
    b = (u8 *)malloc((size_t)size);
    if (fread(b, 1, (size_t)size, f) != (size_t)size) { fclose(f); return 2; }
    fclose(f);
    h = (const VsRouteHeader *)b;
    if (memcmp(h->magic, "VSRT", 4) || h->version != VS_ROUTE_VERSION) { printf("bad route %s\n", path); return 2; }
    ln = (const VsRouteLine *)(h + 1);
    pt = (const VsRoutePoint *)(ln + h->nlines);
    for (i = 0; i < ln[0].count; i++) {
        vs_sim_frame(0, pt[i].pad & (u16)~VSR_RIDE);
        if ((g_sim.p[1].x >> 8) != pt[i].x || (g_sim.p[1].y >> 8) != pt[i].y) off++;
        if (vs_sim_finished(1)) { i++; break; }
    }
    printf("replay %s: %u lines, %u points; line 0 %s at frame %u of %u, %u frames off the line\n", path, h->nlines, h->points,
           vs_sim_finished(1) ? "GOAL" : "no goal", i, ln[0].count, off);
    return vs_sim_finished(1) && off == 0 ? 0 : 1;
}

/* the file goes to <path>.tmp and is renamed when complete, so a run that is stopped leaves the last
 * complete file (or none) and the next run (with -check) carries on with the pairs not yet made; the
 * pair's report goes to <path>.txt */
static int write_route(const char *path, const u8 *file, u32 size, const char *report) {
    char tmp[600];
    FILE *fo;
    snprintf(tmp, sizeof tmp, "%s.tmp", path);
    fo = fopen(tmp, "wb");
    if (!fo || fwrite(file, 1, size, fo) != size) { if (fo) fclose(fo); printf("cannot write %s\n", tmp); return -1; }
    fclose(fo);
    remove(path);
    if (rename(tmp, path) != 0) { printf("cannot rename %s\n", tmp); return -1; }
    snprintf(tmp, sizeof tmp, "%s.txt", path);
    if ((fo = fopen(tmp, "w")) != NULL) { fputs(report, fo); fclose(fo); }
    return 0;
}
static char s_report[4096];
static void report_lines(void) {
    size_t o = strlen(s_report);
    int i;
    o += (size_t)snprintf(s_report + o, sizeof s_report - o, "key %08x, version %d, source %s\n", s_key, VS_ROUTE_VERSION, VSR_SRC);
    for (i = 0; i < NL && o < sizeof s_report - 80; i++)
        o += (size_t)snprintf(s_report + o, sizeof s_report - o, "line %d: %u frames from %u,%u to %u,%u, %u to the goal after\n", i, L[i].n,
                              L[i].sx, L[i].sy, L[i].pt[L[i].n - 1].x, L[i].pt[L[i].n - 1].y, L[i].rest);
}

int main(int argc, char **argv) {
    const char *from = NULL;
    int chara, k, dorep = 0, dorekey = 0, docheck = 0, trials = 90, pass_needed = 15, passes = 0, t;
    double budget = 600, branch_budget = 120;
    int rounds = 60000;
    u32 n0, fsize, seed = 1;
    u16 *pads;
    u8 *file;
    Snap start;
    clock_t t0;
    if (argc < 8) {
        printf("usage: %s <stages> <player> <objdata> <sprites> <zNN> <chara> <route> [-v] [-seed n] [-budget s] [-trials n] [-replay]\n", argv[0]);
        return 2;
    }
    setvbuf(stdout, NULL, _IOLBF, 0);   /* progress shows in a log while it runs */
    chara = atoi(argv[6]);
    for (k = 8; k < argc; k++) {
        if (!strcmp(argv[k], "-v")) verbose = 1;
        else if (!strcmp(argv[k], "-replay")) dorep = 1;
        else if (!strcmp(argv[k], "-check")) docheck = 1;
        else if (!strcmp(argv[k], "-rekey")) dorekey = 1;
        else if (!strcmp(argv[k], "-from") && k + 1 < argc) from = argv[++k];
        else if (!strcmp(argv[k], "-map")) show_map = 1;
        else if (!strcmp(argv[k], "-win") && k + 4 < argc) { int q; for (q = 0; q < 4; q++) s_win[q] = atoi(argv[++k]); s_win_on = 1; }
        else if (!strcmp(argv[k], "-seed") && k + 1 < argc) { seed = (u32)atoi(argv[++k]); rng = seed; }
        else if (!strcmp(argv[k], "-budget") && k + 1 < argc) budget = atof(argv[++k]);
        else if (!strcmp(argv[k], "-rounds") && k + 1 < argc) rounds = atoi(argv[++k]);
        else if (!strcmp(argv[k], "-trials") && k + 1 < argc) trials = atoi(argv[++k]);
    }
    s_key = gen_key(argv[1], argv[2], argv[3], argv[5], chara);
    if (docheck) {
        /* reuse a file made from the same inputs, simulation and tool (the key); anything else is made again */
        FILE *fi = fopen(argv[7], "rb");
        VsRouteHeader h;
        if (fi && fread(&h, 1, sizeof h, fi) == sizeof h && !memcmp(h.magic, "VSRT", 4) && h.version == VS_ROUTE_VERSION && h.key == s_key) {
            fclose(fi);
            printf("%s chara %d: up to date (key %08x, %u lines, %u points%s)\n", argv[5], chara, s_key, h.nlines, h.points,
                   (h.flags & VSR_INCOMPLETE) ? ", PARTIAL" : "");
            return (h.flags & VSR_INCOMPLETE) ? 3 : 0;
        }
        /* another build made it: its line 0 is the start (a complete one is kept when it still replays to the
         * goal: the line 0 search is the long part; a partial one is searched on from its end); the
         * coverage, the trials and the branches are made again */
        if (fi && h.version >= 4 && h.version <= VS_ROUTE_VERSION && !memcmp(h.magic, "VSRT", 4)) from = argv[7];
        if (fi) fclose(fi);
    }
    if (vs_sim_init(argv[1], argv[2], argv[3], argv[4], argv[5], !chara, chara) != 0) return 2;
    if (dorep) return replay(argv[7]);
    if (dorekey) {
        /* a file made by an earlier build of the tool whose changes do not reach the lines (checked: its
         * line 0 replays to the goal frame for frame with this build): give it this build's key */
        FILE *f;
        static u8 buf[1 << 23];
        size_t sz;
        int r = replay(argv[7]);
        VsRouteHeader *h = (VsRouteHeader *)buf;
        if (r != 0) { printf("%s: does not replay, key kept\n", argv[7]); return 1; }
        f = fopen(argv[7], "rb"); sz = f ? fread(buf, 1, sizeof buf, f) : 0; if (f) fclose(f);
        if (sz < sizeof *h) return 1;
        h->key = gen_key(argv[1], argv[2], argv[3], argv[5], chara);
        {   /* (this build: the goal ring is the winner's gimmick from the goal frame, so a line's last point at
             * the goal carries the ride flag) */
            const VsRouteLine *ln = (const VsRouteLine *)(buf + sizeof *h);
            VsRoutePoint *pt = (VsRoutePoint *)(buf + sizeof *h + sizeof(VsRouteLine) * h->nlines);
            u32 l;
            for (l = 0; l < h->nlines; l++)
                if (ln[l].rest == 0 && ln[l].count > 0 && !(h->flags & VSR_INCOMPLETE && l == 0)) pt[ln[l].first + ln[l].count - 1].pad |= VSR_RIDE;
        }
        f = fopen(argv[7], "wb");
        if (!f || fwrite(buf, 1, sz, f) != sz) { printf("cannot write %s\n", argv[7]); return 2; }
        fclose(f);
        printf("%s: key %08x\n", argv[7], h->key);
        return 0;
    }
    for (k = 8; k + 4 < argc; k++)
        if (!strcmp(argv[k], "-objs")) {   /* diagnosis: the stage's objects in a box */
            const rlvl_object_t *o = (const rlvl_object_t *)rlvl_section(g_sim.hdr, RLVL_SEC_OBJECTS, NULL);
            int x0 = atoi(argv[k + 1]), y0 = atoi(argv[k + 2]), x1 = atoi(argv[k + 3]), y1 = atoi(argv[k + 4]);
            u32 i;
            for (i = 0; i < g_sim.hdr->num_objects; i++)
                if (o[i].x >= x0 && o[i].x <= x1 && o[i].y >= y0 && o[i].y <= y1)
                    printf("obj %3u (0x%02x) at %u,%u flags %04x p %d %d %u %u\n", o[i].type, o[i].type, o[i].x, o[i].y, o[i].flags,
                           o[i].p0, o[i].p1, o[i].p2, o[i].p3);
            printf("goal %ld,%ld start %ld,%ld death_y %ld map %ld x %ld\n", (long)g_sim.goal_x, (long)g_sim.goal_y, (long)g_sim.start_x,
                   (long)g_sim.start_y, (long)g_dcp_world.death_y, (long)g_sim.map.right, (long)g_sim.map.bottom);
            return 0;
        }
    t0 = clock();
    set_chain(0);
    if (verbose) g_obj_hooks.log = hk_log;
    if (snap_init() != 0) { printf("snapshot regions not found\n"); return 2; }
    AW = g_sim.map.right / ACELL + 1; AH = g_sim.map.bottom / ACELL + 1;
    cellv = (int *)malloc(sizeof(int) * (size_t)(AW * AH * MAXW * NSUB));
    memset(&start, 0, sizeof start);
    snap_take(&start);
    /* line 0: start to goal */
    s_partial = 1;
    if (from) {
        /* -from <route>: carry on from the end of that file's line 0 (a partial line made before) */
        FILE *fi = fopen(from, "rb");
        static u8 buf[1 << 22];
        size_t sz = fi ? fread(buf, 1, sizeof buf, fi) : 0;
        const VsRouteHeader *h = (const VsRouteHeader *)buf;
        if (fi) fclose(fi);
        if (sz < sizeof *h || memcmp(h->magic, "VSRT", 4) || h->version < 4 || h->version > VS_ROUTE_VERSION) { printf("-from %s: not a v4/v%d route file\n", from, VS_ROUTE_VERSION); return 2; }
        {
            u32 lsz = h->version == 4 ? 16 : sizeof(VsRouteLine);   /* (v4 lines had no t0) */
            const VsRouteLine *ln = (const VsRouteLine *)(buf + sizeof *h);
            const VsRoutePoint *pt = (const VsRoutePoint *)(buf + sizeof *h + lsz * h->nlines) + ln[0].first;
            u32 i;
            s_partial_len = ln[0].count;
            s_partial_pads = (u16 *)malloc(sizeof(u16) * s_partial_len);
            for (i = 0; i < s_partial_len; i++) s_partial_pads[i] = pt[i].pad & (u16)~VSR_RIDE;
            set_chain(1);
            for (i = 0; i < s_partial_len && !vs_sim_finished(1); i++) vs_sim_frame(0, s_partial_pads[i]);
            if (!(h->flags & VSR_INCOMPLETE) && ln[0].rest == 0 && vs_sim_finished(1) && i == s_partial_len) {
                /* a complete line 0 that replays to the goal frame for frame: kept */
                pads = s_partial_pads; n0 = s_partial_len;
                s_partial_pads = NULL; s_partial_len = 0;
                snap_restore(&start);
                if (verbose) printf("  line 0: kept from %s, %u frames\n", from, n0);
                goto have_line0;
            }
            s_partial_pot = potential(g_sim.p[1].x >> 8, g_sim.p[1].y >> 8, stage_for(g_sim.p[1].x >> 8, g_sim.p[1].y >> 8));
            snap_restore(&start);
            if (verbose) printf("  line 0: from %s, %u frames\n", from, s_partial_len);
        }
        pads = NULL;
    } else
    pads = search(0, rounds, budget, &n0, "line 0");
    if (0) {
    have_line0:
        from = NULL;
    }
    if (!pads && !from && set_chain(1) > 1) {   /* the free-space guide leads into a dead end: the checkpoints in order */
        snap_restore(&start);
        if (verbose) printf("  line 0: again along the %d checkpoints\n", NW - 1);
        pads = search(0, rounds * 2, budget, &n0, "line 0 (checkpoints)");
    }
    if (!pads && s_partial_pads && s_partial_len >= 60) {
        /* stages: the search goes on from the furthest place it reached (a fresh archive there explores the
         * gimmicks that need time: waits, rides, switches), up to 8 times while it gets closer; a stage that
         * does not starts 5 s further back */
        u16 *pre = s_partial_pads;
        u32 npre = s_partial_len, back = 0;
        s32 pot = s_partial_pot;
        int stg;
        s_partial_pads = NULL;
        for (stg = 0; stg < 8 && !pads; stg++) {
            u32 use = npre > back ? npre - back : 0, i, n2 = 0;
            u16 *p2;
            char what[64];
            snap_restore(&start);
            for (i = 0; i < use; i++) vs_sim_frame(0, pre[i]);
            if (g_sim.p[1].pf & DCP_PF_DEAD) break;
            snprintf(what, sizeof what, "line 0 stage %d", stg + 1);
            if (verbose) printf("  %s: from %ld,%ld (frame %u, potential %ld)\n", what, (long)(g_sim.p[1].x >> 8), (long)(g_sim.p[1].y >> 8),
                                use, (long)pot);
            if (getenv("VSR_PROBE")) {   /* diagnosis: idle here and list the moving objects near */
                int pf, nprobe = atoi(getenv("VSR_PROBE"));
                Snap tmp;
                memset(&tmp, 0, sizeof tmp);
                snap_take(&tmp);
                for (pf = 0; pf < nprobe; pf++) {
                    Obj *o;
                    vs_sim_frame(0, 0);
                    if (pf % 60) continue;
                    printf("  probe +%d: p2 %ld,%ld g %d |", pf, (long)(g_sim.p[1].x >> 8), (long)(g_sim.p[1].y >> 8),
                           (g_sim.p[1].move_flags & DCP_MF_GROUND) != 0);
                    for (o = g_obj.head; o; o = o->next)
                        if (o->used && o->prm && o->prm->type != 0xaf && labs((o->h.x >> 8) - (g_sim.p[1].x >> 8)) < 700 &&
                            labs((o->h.y >> 8) - (g_sim.p[1].y >> 8)) < 500)
                            printf(" %x@%ld,%ld", o->prm->type, (long)(o->h.x >> 8), (long)(o->h.y >> 8));
                    printf("\n");
                }
                snap_restore(&tmp);
                snap_free(&tmp);
            }
            p2 = search(use, rounds, budget, &n2, what);
            if (p2) {
                pads = (u16 *)malloc(sizeof(u16) * (use + n2));
                memcpy(pads, pre, sizeof(u16) * use);
                memcpy(pads + use, p2, sizeof(u16) * n2);
                n0 = use + n2;
                free(p2);
                break;
            }
            if (s_partial_pads && s_partial_pot < pot - 64) {
                u16 *cat = (u16 *)malloc(sizeof(u16) * (use + s_partial_len));
                memcpy(cat, pre, sizeof(u16) * use);
                memcpy(cat + use, s_partial_pads, sizeof(u16) * s_partial_len);
                free(pre);
                pre = cat; npre = use + s_partial_len; pot = s_partial_pot; back = 0;
            } else {
                back += 300;
            }
            free(s_partial_pads); s_partial_pads = NULL;
        }
        if (pads) free(pre);
        else { s_partial_pads = pre; s_partial_len = npre; }
    }
    if (!pads) {
        /* no way to the goal found: a partial line as far as the search got (the CPU follows the rival from
         * its end, vs_cpu.c) */
        if (!s_partial_pads || s_partial_len < 60) { printf("%s chara %d: no goal (%.1f s)\n", argv[5], chara, (double)(clock() - t0) / CLOCKS_PER_SEC); return 1; }
        snap_restore(&start);
        s_record_partial = 1;
        if (add_line(s_partial_pads, s_partial_len) < 0) return 1;
        s_record_partial = 0;
        L[0].rest = 0xffffff;
        file = build_file(argv[5], chara, &fsize);
        snprintf(s_report, sizeof s_report, "%s chara %d: PARTIAL, line 0 %u frames to %u,%u (no goal), %.1f s\n", argv[5], chara, L[0].n,
                 L[0].pt[L[0].n - 1].x, L[0].pt[L[0].n - 1].y, (double)(clock() - t0) / CLOCKS_PER_SEC);
        fputs(s_report, stdout);
        report_lines();
        if (write_route(argv[7], file, fsize, s_report) != 0) return 2;
        if (verbose) { int i; printf("  events:"); for (i = 0; i < 32 && s_ev[i].name; i++) if (strcmp(s_ev[i].name, "ring")) printf(" %s %ld", s_ev[i].name, s_ev[i].n); printf("\n"); }
        return 3;
    }
    s_partial = 0;
    snap_restore(&start);
    pads = shortcuts(&start, pads, &n0, 40, 6000);
    snap_restore(&start);
    if (add_line(pads, n0) < 0) { printf("%s chara %d: line 0 does not replay to the goal\n", argv[5], chara); return 1; }
    free(pads);
    /* coverage: places the search reached early that no line passes near (where an upset CPU may land):
     * a branch from each to an earlier line */
    {
        enum { NCOV = 24 };
        static Snap cov[NCOV];
        static u32 cov_f[NCOV];
        static s32 cov_x[NCOV], cov_y[NCOV];
        int ncov = 0, i, j, *ord = (int *)malloc(sizeof(int) * (size_t)(NE ? NE : 1)), no = 0;
        merge_build(NL);
        for (i = 0; i < NE; i++)
            if (E[i].grounded && E[i].frame < L[0].n) ord[no++] = i;
        /* earliest first */
        for (i = 1; i < no; i++) { int v = ord[i]; j = i - 1; while (j >= 0 && E[ord[j]].frame > E[v].frame) { ord[j + 1] = ord[j]; j--; } ord[j + 1] = v; }
        for (i = 0; i < no && ncov < NCOV; i++) {
            Ent *e = &E[ord[i]];
            int far = 1, k2;
            u32 q;
            for (q = 0; q < L[0].n && far; q += 2) {
                s32 dx = L[0].pt[q].x - e->x, dy = L[0].pt[q].y - e->y;
                if (dx * dx + dy * dy < 192 * 192) far = 0;
            }
            for (k2 = 0; k2 < ncov && far; k2++)
                if ((cov_x[k2] - e->x) * (cov_x[k2] - e->x) + (cov_y[k2] - e->y) * (cov_y[k2] - e->y) < 384 * 384) far = 0;
            if (!far) continue;
            cov[ncov].n = e->snap.n;
            cov[ncov].w = (unsigned *)malloc(sizeof(unsigned) * 2 * (e->snap.n ? e->snap.n : 1));
            memcpy(cov[ncov].w, e->snap.w, sizeof(unsigned) * 2 * e->snap.n);
            cov_f[ncov] = e->frame; cov_x[ncov] = e->x; cov_y[ncov] = e->y;
            ncov++;
        }
        free(ord);
        for (i = 0; i < ncov && NL < MAXNL; i++) {
            u32 nb;
            char what[64];
            snprintf(what, sizeof what, "cover %d", i);
            snap_restore(&cov[i]);
            merge_build(NL);
            s_merge = 1;
            pads = search(cov_f[i], 8000, branch_budget, &nb, what);
            s_merge = 0;
            if (pads) {
                snap_restore(&cov[i]);
                s_record_partial = 1;
                if (add_line(pads, nb) >= 0 && verbose) printf("  cover %d from %ld,%ld: line %d\n", i, (long)cov_x[i], (long)cov_y[i], NL - 1);
                s_record_partial = 0;
                free(pads);
            }
            snap_free(&cov[i]);
        }
    }
    /* branches: where the CPU, upset, stops getting closer to the goal. A branch must join the lines past
     * the place it got stuck (fewer frames to the goal), so it goes round the trouble in the world as the
     * upset left it (a gimmick gone, a platform elsewhere); the same trial runs again with the branch
     * (up to 4 repairs) until it reaches the goal */
    file = build_file(argv[5], chara, &fsize);
    for (t = 0; t < trials && passes < pass_needed && NL < MAXNL && total_points() < MAXPTS; t++) {
        int u = (int)(rnd() % (UP_COUNT + 3)), rep;
        if (u >= UP_COUNT) u = u == UP_COUNT ? UP_DEATH : u == UP_COUNT + 1 ? UP_TWO : UP_RIVAL2;   /* (the hard ones more often) */
        u32 fu = 120 + rnd() % (L[0].n > 400 ? L[0].n - 300 : 100), at_f = 0, at_rem = 0, nb, tseed = seed * 1000 + (u32)t;
        Snap at;
        memset(&at, 0, sizeof at);
        u32 last_f = 0xffffffffu;
        for (rep = 0; rep < 5; rep++) {
            int added = 0;
            if (!trial(&start, u, fu, tseed, file, fsize, &at, &at_f, &at_rem)) {
                if (verbose) printf("  trial %2d (%s at %u): goal%s\n", t, UP_NAME[u], fu, rep ? " (repaired)" : "");
                break;
            }
            if (rep == 4 || NL >= MAXNL || total_points() >= MAXPTS || at_f == last_f) break;   /* (the same place again: the branch did not help) */
            last_f = at_f;
            snap_restore(&at);
            {
                s32 x = g_sim.p[1].x >> 8, y = g_sim.p[1].y >> 8;
                char what[64];
                snprintf(what, sizeof what, "branch %d", NL);
                if (verbose) printf("  trial %2d (%s at %u): stuck after frame %u (%ld,%ld, %u to the goal): branch search\n", t, UP_NAME[u], fu, at_f,
                                    (long)x, (long)y, at_rem);
                merge_build(NL);
                s_merge = 1;
                s_merge_rem = at_rem > 30 ? at_rem - 30 : 0;
                pads = search(at_f, 12000, branch_budget, &nb, what);
                s_merge = 0;
            }
            if (pads) {
                snap_restore(&at);
                s_record_partial = 1;
                if (add_line(pads, nb) >= 0) { free(file); file = build_file(argv[5], chara, &fsize); added = 1; }
                s_record_partial = 0;
                free(pads);
            }
            s_merge_rem = 0;
            if (!added) break;
        }
        if (rep == 0) passes++; else passes = 0;
        snap_free(&at);
    }
    snprintf(s_report, sizeof s_report, "%s chara %d: GOAL, line 0 %u frames (%u:%02u.%02u), %d lines, %u points, %d trials (last %d passed), %.1f s\n",
             argv[5], chara, L[0].n, L[0].n / 3600, L[0].n / 60 % 60, L[0].n % 60 * 100 / 60, NL, total_points(), t, passes,
             (double)(clock() - t0) / CLOCKS_PER_SEC);
    fputs(s_report, stdout);
    report_lines();
    if (write_route(argv[7], file, fsize, s_report) != 0) return 2;
    if (verbose) { int i; printf("  events:"); for (i = 0; i < 32 && s_ev[i].name; i++) if (strcmp(s_ev[i].name, "ring")) printf(" %s %ld", s_ev[i].name, s_ev[i].n); printf("\n"); }
    return 0;
}
