#include <ov000_b/stageobj.h>

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[0x2e - 2];
    s16 unk2E;
    u8 pad30[4];
    s16 unk34;
    u8 pad36[0x58 - 0x36];
    u32 unk58;
} UnkWork_02312c20;

s16 func_02068db4(s32 v, s32 d);

void func_ov000_02312c20(UnkWork_02312c20 *w) {
    w->unk34 = func_02068db4(w->unk34, 0x80);
    w->unk2E = func_02068db4(w->unk2E, 0x80);
    if (w->unk1 != 0)
        w->unk58 &= ~0x4000000;
    else
        w->unk58 |= 0x4000000;
}
