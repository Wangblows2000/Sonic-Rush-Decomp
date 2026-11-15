#ifndef MAIN_SDK2_MBCTX_H
#define MAIN_SDK2_MBCTX_H
#include <nitro/types.h>

typedef struct {
    u8 state;
    u8 type;
    u8 unk2;
    u8 unk3;
    u8 pad4[0x68 - 4];
} UnkMbEntry;

typedef struct {
    u32 unk0;        /* 0x000 */
    u32 unk4;        /* 0x004 */
    u16 unk8;        /* 0x008 */
    u16 unkA;        /* 0x00a */
    u8 padC[4];
    u32 unk10;       /* 0x010 */
    u8 pad14[0x138 - 0x14];
    u32 unk138;      /* 0x138 */
    u8 pad13c[4];
    u16 unk140;      /* 0x140 */
    u16 unk142;      /* 0x142 */
    u8 pad144[4];
    u32 unk148;      /* 0x148 */
    u8 pad14c[0x1d4 - 0x14c];
    UnkMbEntry entries[16]; /* 0x1d4 */
} UnkMbCtx;

typedef struct {
    u32 type;
    u8 pad[0x98];
} UnkMbParam;
#endif
