/*
 * Battle Play "Vs CPU": the computer-controlled player 2 (native; the DS game had no CPU rival).
 *
 * The CPU only produces a DS pad (held / pressed / released, the same bits main.c makes from a
 * controller); the player then runs the original player, object and race code like a human's.
 * It drives along the stage's racing line (vs_route.h: found at disc build time by vs/route with the
 * real game simulation, from the owner's converted stages): every frame it finds where it is on the
 * line and plays the pad the line used there. Off the line (knocked back, hurt, warped, items) it
 * steers back to a point ahead with the same moves a player has (run, jump, back off, spin dash,
 * air action). It senses only what a player sees: its own state, the map around it, the rival.
 * Skill: how much of the line's boosting it uses, decided per boost stretch from the race seed.
 */
#ifndef SRDC_VS_CPU_H
#define SRDC_VS_CPU_H
#include "../player/dcp_player.h"

/* the DS pad bits the player code reads (main.c ds_pad) */
enum {
    VSC_A = 0x001, VSC_B = 0x002, VSC_RIGHT = 0x010, VSC_LEFT = 0x020, VSC_UP = 0x040, VSC_DOWN = 0x080,
    VSC_R = 0x100, VSC_L = 0x200, VSC_X = 0x400, VSC_Y = 0x800
};
enum { VSC_EASY = 0, VSC_NORMAL = 1, VSC_HARD = 2 };

/* start of a race (and of each rematch). route: a vs_route.h file in memory (kept by the caller for
 * the race; NULL or invalid = no line, the CPU drives by its senses alone); seed: reproducible
 * variation; skill VSC_*. Returns 0 when the line was taken. */
int vs_cpu_begin(int who, const void *route, u32 route_size, u32 seed, int skill);
/* end of the race: frees the CPU's line index */
void vs_cpu_end(int who);
/* once per frame before the player's frame: the pad of player `who` */
void vs_cpu_pad(int who, const DcPlayer *self, const DcPlayer *rival, u16 *held, u16 *pressed, u16 *released);

typedef struct VsCpuStats {
    int on_line;           /* frames driven on the line */
    int off_line;          /* frames steering back */
    int recover;           /* stuck recoveries started */
    int relocate;          /* global searches of the line (respawn, warp) */
    int jumps, boosts, rival_attacks;
    int idx;               /* where on line 0 (point index) */
    int line, k;           /* the line followed (-2: the rival's path, -1: none), its frame */
    s32 tx, ty;            /* off the line: the point steered to */
    int line_len;
    int rem;               /* on a line: its frames to the goal from here (-1 off the lines) */
    s32 best_x;
} VsCpuStats;
const VsCpuStats *vs_cpu_stats(int who);

#endif
