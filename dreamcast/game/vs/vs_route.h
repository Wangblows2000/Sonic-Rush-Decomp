/* Vs CPU racing lines (/cd/vs/zNA_C.rte; made at disc build time by vs/route/vs_route from the owner's
 * converted stages): one stage, one character. Line 0 runs from the start to the goal ring; the
 * others are branches the route tool found from places where the CPU, tested with the race's upsets
 * (items, knock-backs, missed jumps, no boost, a rival), lost the lines, and from places the search
 * reached that no line passes; a branch ends where it joins an earlier line in the same state (place,
 * way, speed), or at the goal ring. A point is the position after one frame and the pad held in that
 * frame. Little-endian. */
#ifndef SRDC_VS_ROUTE_H
#define SRDC_VS_ROUTE_H
#include "../player/dcp_types.h"

#define VS_ROUTE_VERSION 4
#define VS_ROUTE_MAX_LINES 160
#define VSR_RIDE 0x8000          /* pad bit 15 (no DS key): carried by a gimmick at this point (ship, cargo,
                                    hand, rail...) */
#define VSR_INCOMPLETE 1         /* header flags: line 0 does not reach the goal (no way was found) */
typedef struct VsRouteHeader {
    char magic[4];       /* "VSRT" */
    u16 version;
    u8 zone, act, chara; /* 0-based zone / act, 0 Sonic 1 Blaze */
    u8 flags;            /* VSR_INCOMPLETE */
    u16 nlines;
    u32 points;          /* all lines */
    u32 key;             /* generation key: the inputs (stage, objects, player data) and the simulation and
                            tool sources it was made from; vs_route -check reuses a file with the same key */
    u32 reserved;
} VsRouteHeader;         /* 24 bytes */
typedef struct VsRouteLine {
    u32 first;           /* index of the line's first point */
    u32 count;           /* points */
    u32 rest;            /* frames to the goal after the last point (0: the last point reaches the goal;
                            a branch ends where it joins an earlier line, rest = that line's time left) */
    u16 sx, sy;          /* the position before the first point */
} VsRouteLine;           /* 16 bytes; nlines of them follow the header */
typedef struct VsRoutePoint {
    u16 x, y;            /* px */
    u16 pad;             /* DS pad bits held this frame (vs_cpu.h VSC_*) | VSR_RIDE */
} VsRoutePoint;          /* 6 bytes */
#endif
