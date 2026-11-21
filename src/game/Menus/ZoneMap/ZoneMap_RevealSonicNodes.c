/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 ZoneMap_SetPathDotsType();

void ZoneMap_RevealSonicNodes(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x4);
    s32 t1 = (*(u32 *)((u8 *)p0 + 0xa4) + 0x674);
    if (t0 >= 0) {
        *(u32 *)((u8 *)t1 + 0x1c) = 0;
    }
    if (t0 >= 5) {
        *(u32 *)((u8 *)t1 + 0x2c) = 0;
        ZoneMap_SetPathDotsType(p0, 0, 0);
    }
    if (t0 >= 0xb) {
        *(u32 *)(t1 + 0xfc) = 0;
        *(u32 *)((u8 *)t1 + 0x3c) = 0;
        ZoneMap_SetPathDotsType(p0, 9, 0);
        ZoneMap_SetPathDotsType(p0, 1, 0);
    }
    if (t0 >= 0x11) {
        *(u32 *)((u8 *)t1 + 0x10c) = 0;
        *(u32 *)((u8 *)t1 + 0x6c) = 0;
        ZoneMap_SetPathDotsType(p0, 0xa, 0);
        ZoneMap_SetPathDotsType(p0, 2, 0);
    }
    if (t0 >= 0x13) {
        *(u32 *)((u8 *)t1 + 0x7c) = 0;
        ZoneMap_SetPathDotsType(p0, 3, 0);
    }
    if (t0 >= 0x14) {
        *(u32 *)((u8 *)t1 + 0x7c) = 2;
    }
    if (t0 >= 0x16) {
        *(u32 *)(t1 + 0x8c) = 0;
        ZoneMap_SetPathDotsType(p0, 4, 0);
    }
    if (t0 >= 0x18) {
        *(u32 *)(t1 + 0x9c) = 0;
        ZoneMap_SetPathDotsType(p0, 5, 0);
    }
    if (t0 >= 0x19) {
        *(u32 *)(t1 + 0x9c) = 2;
    }
    if (t0 >= 0x1b) {
        *(u32 *)(t1 + 0xcc) = 0;
        ZoneMap_SetPathDotsType(p0, 6, 0);
    }
    if (t0 >= 0x1e) {
        *(u32 *)(t1 + 0xdc) = 0;
        ZoneMap_SetPathDotsType(p0, 7, 0);
    }
    if (t0 >= 0x21) {
        *(u32 *)(t1 + 0xec) = 0;
        ZoneMap_SetPathDotsType(p0, 8, 0);
    }
}

#pragma thumb reset
