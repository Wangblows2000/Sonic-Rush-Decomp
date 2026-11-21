#ifndef MAIN_SDK2_MBSEG_H
#define MAIN_SDK2_MBSEG_H
#include <nitro/types.h>

typedef struct {
    u32 src;
    u32 addr;
    u32 size;
    u32 flag : 1;
    u32 unkC : 31;
} UnkMbSeg;

typedef struct {
    u8 pad0[0xc];
    UnkMbSeg seg[3];
} UnkMbHeader;

typedef struct {
    u32 offset[3]; /* 0x00 */
    u16 blk[3];    /* 0x0c */
    u16 total;     /* 0x12 */
} UnkMbSegInfo;

typedef struct {
    u32 src;
    u32 size;
    u32 offset;
    u8 seg;
} UnkMbSegPos;
#endif
