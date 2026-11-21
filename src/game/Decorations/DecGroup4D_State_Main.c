#include <nitro/types.h>

typedef struct {
    u8 pad0[0xc];
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u8 pad18[0x1a0 - 0x18];
    u32 unk1A0;
    u8 pad1A4[0x258 - 0x1a4];
    u32 unk258;
} UnkWork_02322fd0;

typedef struct {
    u8 pad0[4];
    s32 x;
    s32 y;
} UnkStruct_0236aad8;

extern UnkStruct_0236aad8 gCamera;

void DecGroup4D_State_Main(UnkWork_02322fd0 *w) {
    s32 a;
    s32 b;

    if (w->unkC == 0) {
        w->unkC = 1;
        return;
    }
    b = gCamera.x >> 8;
    a = w->unk10 >> 8;
    if (b - 0x20 > a || b + 0x120 < a ||
        (b = gCamera.y >> 8, a = w->unk14 >> 8, b - 0x20 > a) || b + 0xe0 < a) {
        w->unk1A0 |= 1;
        w->unk258 |= 8;
    } else {
        w->unk1A0 &= ~1;
        w->unk258 &= ~8;
    }
}
