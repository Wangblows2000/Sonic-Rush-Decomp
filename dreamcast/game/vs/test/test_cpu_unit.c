/* Vs CPU decisions (vs_cpu.c) on synthetic racing lines, without a stage: the place on a line kept
 * while the line stands still, presses of held buttons, confusion, the hand-over at a branch's end,
 * the rival boost attack's limits, an invalid file. (The races themselves: test_cpu / make cpu-test.) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../vs_cpu.h"
#include "../vs_route.h"

static int g_fail, g_checks;
#define CHECK(c, ...) do { g_checks++; if (!(c)) { g_fail++; printf("FAIL: "); printf(__VA_ARGS__); printf("\n"); } } while (0)

typedef struct { VsRouteHeader h; VsRouteLine l[2]; VsRoutePoint p[200]; } Route;
static Route R;

static void line_file(int nl) {
    memset(&R.h, 0, sizeof R.h);
    memcpy(R.h.magic, "VSRT", 4);
    R.h.version = VS_ROUTE_VERSION;
    R.h.nlines = (u16)nl;
}
/* the file as vs_route writes it: header, lines, points (packed) */
static u8 *pack(u32 *size) {
    u32 hs = sizeof(VsRouteHeader) + R.h.nlines * sizeof(VsRouteLine), ps = R.h.points * sizeof(VsRoutePoint);
    u8 *b = (u8 *)malloc(hs + ps);
    memcpy(b, &R.h, sizeof R.h);
    memcpy(b + sizeof R.h, R.l, R.h.nlines * sizeof(VsRouteLine));
    memcpy(b + hs, R.p, ps);
    *size = hs + ps;
    return b;
}
static DcPlayer P, Q;
static void at(DcPlayer *p, s32 x, s32 y) { p->x = x << 8; p->y = y << 8; p->move_flags = DCP_MF_GROUND; }

int main(void) {
    u16 h, pr, rl;
    u32 size;
    u8 *f;
    int i;
    /* line 0: 10 frames standing at (100, 500) with no keys, then running right with RIGHT, A pressed at
     * frame 40 (held to 50), again at 51 (a new press right after a release) */
    line_file(1);
    R.l[0].first = 0; R.l[0].count = 100; R.l[0].rest = 0; R.l[0].sx = 100; R.l[0].sy = 500;
    for (i = 0; i < 100; i++) {
        R.p[i].x = (u16)(i < 10 ? 100 : 100 + (i - 9) * 4);
        R.p[i].y = 500;
        R.p[i].pad = (u16)(i < 10 ? 0 : VSC_RIGHT | ((i >= 40 && i < 50) || (i >= 51 && i < 60) ? VSC_A : 0));
    }
    R.h.points = 100;
    f = pack(&size);
    CHECK(vs_cpu_begin(1, f, size, 7, VSC_HARD) == 0, "the line is taken");
    memset(&P, 0, sizeof P); memset(&Q, 0, sizeof Q);
    at(&Q, 50, 500);
    /* standing still on the line: the place goes on with the time, the run starts on time */
    for (i = 0; i < 12; i++) {
        at(&P, i < 10 ? 100 : R.p[i - 1].x, 500);
        vs_cpu_pad(1, &P, &Q, &h, &pr, &rl);
        if (i < 10) CHECK(h == 0, "frame %d (standing): pad %04x", i, h);
    }
    CHECK((h & VSC_RIGHT) && vs_cpu_stats(1)->k >= 10, "after the stand: running (pad %04x, point %d)", h, vs_cpu_stats(1)->k);
    /* the A of frame 40 is pressed, held; the new press at 51 comes as a press */
    for (i = 12; i < 60; i++) {
        at(&P, R.p[i - 1].x, 500);
        vs_cpu_pad(1, &P, &Q, &h, &pr, &rl);
        if (i == 40) CHECK((pr & VSC_A) != 0, "A pressed at the line's frame 40 (%04x)", pr);
        if (i == 45) CHECK((h & VSC_A) && !(pr & VSC_A), "A held at 45");
        if (i == 50) CHECK(!(h & VSC_A), "A let go at 50");
        if (i == 51) CHECK((pr & VSC_A) != 0, "A pressed again at 51");
    }
    CHECK(vs_cpu_stats(1)->on_line >= 59 && vs_cpu_stats(1)->off_line == 0, "on the line all along (%d on, %d off)",
          vs_cpu_stats(1)->on_line, vs_cpu_stats(1)->off_line);
    /* confusion: the D-pad turned (right gives left); not right away, after a moment the CPU presses
     * the key that now gives right */
    P.confuse = 0x1ff;               /* just turned: (confuse & 0x7f) == 0x7f */
    P.map_right = 0x20; P.map_left = 0x10; P.map_up = 0x40; P.map_down = 0x80;
    at(&P, R.p[59].x, 500);
    vs_cpu_pad(1, &P, &Q, &h, &pr, &rl);
    CHECK((h & VSC_RIGHT) && !(h & VSC_LEFT), "confused, no reaction yet: RIGHT (%04x)", h);
    P.confuse = 0x180 + 0x7f - 50;     /* 50 frames after the turn */
    at(&P, R.p[60].x, 500);
    vs_cpu_pad(1, &P, &Q, &h, &pr, &rl);
    CHECK((h & VSC_LEFT) && !(h & VSC_RIGHT), "confused, after a moment: LEFT for right (%04x)", h);
    P.confuse = 0;
    vs_cpu_end(1);
    free(f);

    /* a branch (line 1) that joins line 0: at its end the CPU goes on on line 0 */
    line_file(2);
    R.l[0].first = 0; R.l[0].count = 100; R.l[0].rest = 0; R.l[0].sx = 100; R.l[0].sy = 500;
    for (i = 0; i < 100; i++) { R.p[i].x = (u16)(104 + i * 4); R.p[i].y = 500; R.p[i].pad = VSC_RIGHT; }
    R.l[1].first = 100; R.l[1].count = 30; R.l[1].rest = 50; R.l[1].sx = 200; R.l[1].sy = 400;
    for (i = 0; i < 30; i++) {   /* from (200, 400) down to line 0's point 49 (300, 500) */
        R.p[100 + i].x = (u16)(203 + i * 3 + (i == 29 ? 7 : 0)); R.p[100 + i].y = (u16)(400 + (i + 1) * 100 / 30);
        R.p[100 + i].pad = VSC_RIGHT | VSC_X;
    }
    R.h.points = 130;
    f = pack(&size);
    CHECK(vs_cpu_begin(1, f, size, 9, VSC_HARD) == 0, "two lines taken");
    at(&P, 200, 400);
    vs_cpu_pad(1, &P, NULL, &h, &pr, &rl);
    CHECK(vs_cpu_stats(1)->line == 1 && (h & VSC_X), "on the branch (line %d, pad %04x)", vs_cpu_stats(1)->line, h);
    for (i = 0; i < 30; i++) { at(&P, R.p[100 + i].x, R.p[100 + i].y); vs_cpu_pad(1, &P, NULL, &h, &pr, &rl); }
    at(&P, 304, 500);
    vs_cpu_pad(1, &P, NULL, &h, &pr, &rl);
    CHECK(vs_cpu_stats(1)->line == 0 && vs_cpu_stats(1)->k >= 48 && vs_cpu_stats(1)->k <= 52 && h == VSC_RIGHT,
          "at the branch's end: line 0 at its point 49 (line %d point %d pad %04x)", vs_cpu_stats(1)->line, vs_cpu_stats(1)->k, h);
    /* the rival right ahead on the ground, tension to spare: one boost attack, then not again for 4 s */
    {
        int attacks0 = vs_cpu_stats(1)->rival_attacks, n = 0;
        P.tension = 0x1000;
        for (i = 50; i < 99; i++) {
            at(&P, R.p[i - 1].x, 500);
            at(&Q, R.p[i - 1].x + 30, 500);
            vs_cpu_pad(1, &P, &Q, &h, &pr, &rl);
            if (h & VSC_X) n++;
        }
        CHECK(vs_cpu_stats(1)->rival_attacks == attacks0 + 1 && n >= 1, "one boost attack in 49 frames (%d)", vs_cpu_stats(1)->rival_attacks - attacks0);
    }
    vs_cpu_end(1);
    free(f);
    /* a file that is not a line set: no line (the CPU then follows the rival) */
    CHECK(vs_cpu_begin(1, "not a route", 11, 1, VSC_NORMAL) != 0, "an invalid file is refused");
    vs_cpu_end(1);
    printf("vs cpu decisions: %d checks, %d failed\n", g_checks, g_fail);
    return g_fail != 0;
}
