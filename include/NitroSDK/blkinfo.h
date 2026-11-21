#ifndef MAIN_SDK2_BLKINFO_H
#define MAIN_SDK2_BLKINFO_H
#include <nitro/types.h>

typedef struct {
    s32 size;      /* 0x00 */
    s32 count;     /* 0x04 */
    s32 last;      /* 0x08 */
    s32 cursor;    /* 0x0c */
    u32 unk10;     /* 0x10 */
    u32 *bits;     /* 0x14 */
    u8 *base;      /* 0x18 */
} UnkBlockInfo;
#endif
