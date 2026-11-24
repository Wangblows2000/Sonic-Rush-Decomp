/* Vs CPU race on a real Battle Play stage (host): the real player, objects, cameras and race rules
 * (route/vs_sim.c), player 2 driven by the CPU (vs_cpu.c) along its racing line.
 *
 * usage: test_cpu <stages> <player> <objdata> <sprites> <zNN> <p2 chara> <p2 route> [options]
 *   -p1 <route>   player 1 is a CPU too (the other character's line), else player 1 stands at the start
 *   -seed n -skill k
 *   -upsets n     n upsets at random times (seeded): a Battle Play item from the rival (slowdown,
 *                 confusion, tension drain; warp to the rival when player 1 races) or a knock-back
 *   -frames n     limit (default: the race time limit 0x8c9f + 60)
 * Prints one result line; exit 0 when player 2 reached the goal.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../route/vs_sim.h"
#include "../vs_cpu.h"
#include "../../objects/obj_internal.h"

static void *load(const char *path, long *size) {
    FILE *f = fopen(path, "rb");
    void *b;
    if (!f) return NULL;
    fseek(f, 0, SEEK_END); *size = ftell(f); fseek(f, 0, SEEK_SET);
    b = malloc((size_t)*size);
    if (fread(b, 1, (size_t)*size, f) != (size_t)*size) { free(b); b = NULL; }
    fclose(f);
    return b;
}

int main(int argc, char **argv) {
    long sz2 = 0, sz1 = 0;
    void *r2, *r1 = NULL;
    int chara2, k, upsets = 0, trace = 0, skill = VSC_NORMAL, items = 0, knocks = 0;
    u32 seed = 1, frames = 0x8c9f + 60, i, objs_at = 0xffffffffu, die_at = 0xffffffffu;
    if (argc < 8) { printf("usage: %s <stages> <player> <objdata> <sprites> <zNN> <p2 chara> <p2 route> [-p1 route] [-seed n] [-skill k] [-upsets n] [-frames n]\n", argv[0]); return 2; }
    chara2 = atoi(argv[6]);
    r2 = load(argv[7], &sz2);
    for (k = 8; k < argc; k++) {
        if (!strcmp(argv[k], "-p1") && k + 1 < argc) r1 = load(argv[++k], &sz1);
        else if (!strcmp(argv[k], "-seed") && k + 1 < argc) seed = (u32)atoi(argv[++k]);
        else if (!strcmp(argv[k], "-skill") && k + 1 < argc) skill = atoi(argv[++k]);
        else if (!strcmp(argv[k], "-frames") && k + 1 < argc) frames = (u32)atoi(argv[++k]);
        else if (!strcmp(argv[k], "-upsets") && k + 1 < argc) upsets = atoi(argv[++k]);
        else if (!strcmp(argv[k], "-trace")) trace = 1;
        else if (!strcmp(argv[k], "-die") && k + 1 < argc) die_at = (u32)atoi(argv[++k]);
        else if (!strcmp(argv[k], "-objs") && k + 1 < argc) objs_at = (u32)atoi(argv[++k]);
    }
    if (vs_sim_init(argv[1], argv[2], argv[3], argv[4], argv[5], !chara2, chara2) != 0) return 2;
    if (vs_cpu_begin(1, r2, (u32)sz2, seed, skill) != 0) printf("p2: no racing line\n");
    if (r1 && vs_cpu_begin(0, r1, (u32)sz1, seed * 31 + 7, skill) != 0) printf("p1: no racing line\n");
    {
        static u32 at[64];
        u32 r = seed * 2654435761u + 1;
        int n;
        if (upsets > 64) upsets = 64;
        for (n = 0; n < upsets; n++) { r = r * 1103515245u + 12345u; at[n] = 300 + (r >> 8) % 6000; }
        for (i = 0; i < frames && vs_info()->exit == VS_EXIT_NONE; i++) {
        u16 h1 = 0, h2 = 0, pp, rr;
        DcPlayer *A = &g_sim.p[0], *B = &g_sim.p[1];
        if (r1) vs_cpu_pad(0, A, B, &h1, &pp, &rr);
        vs_cpu_pad(1, B, A, &h2, &pp, &rr);
        for (n = 0; n < upsets; n++)
            if (i == at[n] && !vs_sim_finished(1)) {
                int what = (int)((at[n] / 7 + (u32)n) % (r1 ? 5 : 4));
                if (what == 3) {          /* a boost attack of player 1 (vs_after_players knock-back) */
                    B->vs_timer = 0xc; B->gnd_vel = (s16)(B->gnd_vel >= 0 ? -0x480 : 0x480); B->hitstop = 0x44;
                    knocks++;
                } else {                  /* ItemBox_GiveItem 8..11 collected by player 1: on player 2 */
                    g_obj_hooks.item(g_obj_hooks.ctx, A, what == 4 ? OBJ_ITEM_VS_11 : OBJ_ITEM_VS_SLOW + what);
                    items++;
                }
            }
        if (i == die_at) dcp_act_die(B);   /* a death (respawn into a world that went on) */
        vs_sim_frame(h1, h2);
        if (i == objs_at) {   /* the objects within 300 px of player 2 at this frame */
            Obj *o;
            for (o = g_obj.head; o; o = o->next)
                if (o->used && o->prm && labs((o->h.x >> 8) - (B->x >> 8)) < 300 && labs((o->h.y >> 8) - (B->y >> 8)) < 300)
                    printf("  obj type %3u at %ld,%ld flags %04x p6 %d p7 %d p8 %u p9 %u\n", o->prm->type, (long)(o->h.x >> 8),
                           (long)(o->h.y >> 8), o->prm->flags, o->prm->p6, o->prm->p7, o->prm->p8, o->prm->p9);
        }
        if (trace && (i % 60 == 0 || (trace && (h2 & 0x8000)))) {
            const VsCpuStats *s = vs_cpu_stats(1);
            printf("  f%5u p2 %6ld,%5ld vx %5d g %d gim %d line %d k %5d tgt %ld,%ld on %5d off %5d rec %3d reloc %3d pad %04x%s | p1 %ld,%ld\n", i,
                   (long)(B->x >> 8), (long)(B->y >> 8), B->gnd_vel, (B->move_flags & 1) != 0, B->gimmick != NULL, s->line, s->k,
                   (long)s->tx, (long)s->ty, s->on_line, s->off_line, s->recover, s->relocate, h2, (B->pf & DCP_PF_DEAD) ? " DEAD" : "",
                   (long)(A->x >> 8), (long)(A->y >> 8));
        }
        }
    }
    {
        const VsCpuStats *s = vs_cpu_stats(1);
        printf("%s p2 %s skill %d seed %u%s%s: %s at %u (%u:%02u.%02u), deaths %d, rings %d, on line %d off %d, recover %d, "
               "relocate %d, boosts %d, rival attacks %d, items %d knocks %d",
               argv[5], chara2 ? "Blaze" : "Sonic", skill, seed, r1 ? " vs CPU" : "", upsets ? " upset" : "",
               vs_sim_finished(1) ? "GOAL" : "no goal", vs_info()->time[1], vs_info()->time[1] / 3600,
               vs_info()->time[1] / 60 % 60, vs_info()->time[1] % 60 * 100 / 60, g_sim.deaths[1], g_sim.p[1].rings,
               s->on_line, s->off_line, s->recover, s->relocate, s->boosts, s->rival_attacks, items, knocks);
        if (!vs_sim_finished(1))
            printf(" [stopped at %ld,%ld line point %d/%d]", (long)(g_sim.p[1].x >> 8), (long)(g_sim.p[1].y >> 8), s->idx, s->line_len);
        if (r1) printf(" | p1 %s at %u deaths %d", vs_sim_finished(0) ? "GOAL" : "no goal", vs_info()->time[0], g_sim.deaths[0]);
        printf(" | winner %d\n", vs_info()->winner);
    }
    return vs_sim_finished(1) ? 0 : 1;
}
