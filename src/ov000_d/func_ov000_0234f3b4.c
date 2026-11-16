#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_0234f3b4_3E8;

typedef struct {
    u8 pad0[8];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u8 pad18[4];
    s32 unk1C;
    s32 unk20;
    u8 pad24[0x3E8 - 0x24];
    UnkStruct_0234f3b4_3E8 *unk3E8;
    u8 pad3EC[0x3F4 - 0x3EC];
    s32 unk3F4;
    s32 unk3F8;
} UnkStruct_0234f3b4;

s32 func_02068bd8(s32 v, s32 a, s32 b, s32 c, s32 d, s32 e);
void func_ov000_0234f320(UnkStruct_0234f3b4 *w);

void func_ov000_0234f3b4(UnkStruct_0234f3b4 *w) {
    s32 d;

    switch (w->unk8) {
    case 0:
        if (w->unk3E8 != NULL) {
            w->unk1C = w->unk10;
            w->unk20 = w->unk14;
            w->unk10 = func_02068bd8(w->unk10, w->unk3E8->unk10, w->unk3F4, 1, 0x400, 0x100);
            d = w->unk10 - w->unk3E8->unk10;
            if (d < 0) {
                d = -d;
            }
            if (d < 0x800) {
                w->unk14 = func_02068bd8(w->unk14, w->unk3E8->unk14, w->unk3F8, 1, 0x400, 0x100);
            }
            if (w->unk3E8->unk10 == w->unk10 && w->unk3E8->unk14 == w->unk14) {
                w->unk8 = 1;
            }
        } else {
            w->unk8 = 1;
        }
        break;
    case 1:
        w->unkC++;
        if (w->unkC <= 0x10) {
            return;
        }
        func_ov000_0234f320(w);
        break;
    }
}
