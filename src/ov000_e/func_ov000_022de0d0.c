#include <nitro/types.h>

typedef struct {
    u8 pad[0x21C];
    u32 unk21C;
    u8 pad220[4];
    s32 unk224;
    u8 pad228[0xBB8 - 0x228];
    s32 unkBB8;
    s32 unkBBC;
    s32 unkBC0;
    s32 unkBC4;
    s32 unkBC8;
    s32 unkBCC;
    u8 padBD0[0xBDC - 0xBD0];
    s32 unkBDC;
} UnkStruct_022de0d0;

void func_ov000_022de304(UnkStruct_022de0d0 *s);
void func_ov000_022dbbe0(s32 a);
s32 func_ov000_022fad3c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i);
BOOL func_ov000_022db9a8(UnkStruct_022de0d0 *s);

void func_ov000_022de0d0(UnkStruct_022de0d0 *s) {
    s32 lim;
    s32 d;

    lim = (s->unk21C & 1) ? 0x2d0 : 0x1e0;
    if (s->unk224 > lim) {
        func_ov000_022de304(s);
        s->unk21C &= ~2;
        func_ov000_022dbbe0(0);
        return;
    }
    if (s->unk224 == 10 && s->unkBDC < 1)
        func_ov000_022fad3c(0xd1, 0, 0, 0, 0, 0, 0, 0, 0);
    s->unkBB8 += s->unkBC0;
    s->unkBBC += s->unkBC4;
    if (s->unk224 & 4) {
        s->unk21C &= ~2;
        func_ov000_022dbbe0(0);
    } else {
        s->unk21C |= 2;
        func_ov000_022dbbe0(1);
    }
    if (!func_ov000_022db9a8(s))
        return;
    d = s->unkBC8 - s->unkBCC;
    if (d > 0x40)
        d = 0x40;
    if (d == 0) {
        s->unkBCC = s->unkBC8;
        s->unkBC4 = 0;
        return;
    }
    s->unkBCC += d;
    s->unkBC4 *= -1;
}
