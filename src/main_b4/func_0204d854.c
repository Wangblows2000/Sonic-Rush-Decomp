#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    s32 state;
    s32 unkc;
    s32 unk10;
    s32 unk14;
    u8 pad18[4];
    s32 unk1c;
    s32 unk20;
    u8 pad24[0xa];
    s16 unk2e;
    s16 unk30;
    u8 pad32[0x30];
    s16 unk62;
} UnkWork_0204d854;

typedef struct {
    u8 pad0[0x10];
    u32 unk10;
} UnkStruct_022b65b4;

extern UnkStruct_022b65b4 gTouchState;

s32 ObjShift_Set(s32 v, s32 a, s32 b, s32 c, s32 d);
s32 Math_LerpFx32(s32 a, s32 b, u16 t);

void func_0204d854(UnkWork_0204d854 *w) {
    if (((u16)(((*(vu16 *)0x4000130 | *(vu16 *)0x27fffa8) ^ 0x2fff) & 0x2fff) & 0xc0b) || (gTouchState.unk10 & 0x40))
        w->state = 3;
    switch (w->state) {
    case 0:
        w->unk10 = ObjShift_Set(w->unk10, w->unk20 << 8, 2, 0xf00, 0x400);
        if (w->unk10 == w->unk20 << 8) {
            w->state = 1;
            w->unk30 = -0x200;
            w->unk62 = 0x88;
            w->unkc = w->unk2e * 2 + 4;
        }
        break;
    case 1:
        if (w->unkc != 0)
            w->unkc--;
        if (w->unkc == 0)
            w->state = 2;
        break;
    case 2:
        w->unk30 += 0x2a;
        w->unk14 += w->unk30;
        if (w->unk14 > 0x6800) {
            w->unk30 = 0;
            w->unk14 = 0x6800;
        }
        w->unkc = ObjShift_Set(w->unkc, 0x1000, 1, 0x180, 0x180);
        w->unk10 = Math_LerpFx32(w->unk1c, w->unk10, w->unkc);
        if (w->unk10 != w->unk1c)
            break;
        if (w->unk14 == 0x6800) {
            w->state = 3;
            w->unk62 = 0x40;
        }
        break;
    case 3:
        w->unk10 = w->unk1c;
        w->unk14 = 0x6800;
        break;
    }
}
