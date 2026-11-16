#pragma opt_strength_reduction off
#include <nitro/types.h>

typedef struct UnkOam_022df490 {
    u32 attr01;
    u16 attr2;
    u16 attr3;
} UnkOam_022df490;

typedef struct UnkStruct_022df490 {
    u8 pad0[0x54];
    UnkOam_022df490 oam[0x80];
} UnkStruct_022df490;

void TransformDemo_InitCaptureOam(UnkStruct_022df490 *w) {
    s32 i;
    s32 x;
    s32 y;
    s32 n;
    UnkOam_022df490 *oam;
    s32 row;
    s32 col;

    n = 0;
    oam = w->oam;
    for (i = 0; i < 0x80; i++) {
        *(u32 *)&oam[i] = 0;
        *((u32 *)&oam[i] + 1) = 0;
    }
    for (y = 0, row = 0; y < 0xc0; y += 0x40, row += 8) {
        for (x = 0, col = 0; x < 0x100; x += 0x40, col += 8, n++) {
            oam[n].attr01 = (y & 0xff) | 0xc0000c00 | ((x & 0x1ff) << 16);
            oam[n].attr2 = col + (row << 5) | 0xf000;
        }
    }
}
