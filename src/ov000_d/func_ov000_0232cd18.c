#include <nitro/types.h>

typedef struct {
    u8 pad0[7];
    u8 unk7;
    u8 pad8[0x46 - 8];
    s16 unk46;
    u8 pad48[0x54 - 0x48];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x258 - 0x5E];
    s16 unk258;
    s16 unk25A;
    u8 pad25C[4];
    s16 unk260;
    s16 unk262;
    u8 pad264[0x276 - 0x264];
    s16 unk276;
} UnkStruct_0232cd18;

s16 func_02068bd8(s16 v, s32 a, s32 b, s32 c, s32 d, s32 e);
void func_ov000_0232cc2c(UnkStruct_0232cd18 *w);

void func_ov000_0232cd18(UnkStruct_0232cd18 *w) {
    w->unk46 = func_02068bd8(w->unk46, 0, 0x100, 0, 0x10, 0);
    w->unk7 = ((0x100 - w->unk46) * 3) >> 8;
    w->unk258 = func_02068bd8(w->unk258, 0, 0x100, 0, 0x10, 0);
    w->unk260 = (s8)(((0x100 - w->unk258) * 4) >> 8);
    w->unk25A = func_02068bd8(w->unk25A, 0, 0x100, 0, 0x10, 0);
    w->unk262 = (s8)(((0x100 - w->unk25A) * 4) >> 8);
    if (w->unk46 != 0) {
        return;
    }
    w->unk5C |= 0x20;
    w->unk54 &= ~0x3000;
    w->unk276++;
    if (w->unk276 > 0x20) {
        func_ov000_0232cc2c(w);
    }
}
