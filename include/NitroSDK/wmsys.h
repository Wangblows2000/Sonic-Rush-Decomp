#ifndef MAIN_SDK2_WMSYS_H
#define MAIN_SDK2_WMSYS_H
#include <nitro/types.h>

typedef struct {
    u32 unk0;
    u16 *state;    /* 0x4 */
    u8 pad8[4];
    u32 *cmdBuf;   /* 0xc */
    u8 pad10[4];
    u16 dma;       /* 0x14 */
} UnkWmSys;

extern UnkWmSys *data_0207faec;
#endif
