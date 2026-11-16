#include <nitro/types.h>

typedef struct {
    u8 pad0[7];
    s8 unk7;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u8 pad18[0x46 - 0x18];
    s16 unk46;
    u8 pad48[0x54 - 0x48];
    u32 unk54;
    u8 pad58[0x62 - 0x58];
    s16 unk62;
    u8 pad64[0x254 - 0x64];
    s16 unk254;
    s16 unk256;
    s16 unk258;
    s16 unk25A;
    u8 pad25C[0x264 - 0x25C];
    s16 unk264;
    s16 unk266;
    u8 pad268[0x276 - 0x268];
    s16 unk276;
} UnkStruct_0232d084;

s32 func_02068bd8(s32 v, s32 a, s32 b, s32 c, s32 d, s32 e);
s32 func_02068bb8(s32 a, s32 b, u16 t);
void func_0204b7ec(void *p, s32 a, s32 b);
void func_ov000_0232d30c(UnkStruct_0232d084 *w, s32 a);
void func_ov000_0232d05c(UnkStruct_0232d084 *w);

void func_ov000_0232d084(UnkStruct_0232d084 *w) {
    switch (w->unk8) {
    case 0:
        w->unk46 = func_02068bd8(w->unk46, 0x120, 0, 0, 0x10, 0);
        w->unk7 = ((0x100 - w->unk46) * -5) >> 8;
        if (w->unk46 == 0x120) {
            w->unkC = 0x20;
            w->unk8 = 1;
        }
        break;
    case 1:
        w->unk46 = 0x100;
        w->unkC--;
        if (w->unkC == 0) {
            w->unk8 = 2;
            w->unk54 |= 0x1000;
            w->unk254 = w->unk258 = 0x180;
            w->unk264 = 4;
            w->unk256 = w->unk25A = 0x180;
            w->unk266 = 4;
            w->unk276 = 0;
            func_0204b7ec(NULL, 0, 0xe7);
        }
        break;
    case 2:
        func_ov000_0232d30c(w, 0);
        if (w->unk254 <= 0x100) {
            if (!(w->unk54 & 0x2000))
                func_0204b7ec(NULL, 0, 0xe7);
            w->unk54 |= 0x2000;
            func_ov000_0232d30c(w, 1);
        }
        if (w->unk264 != 0)
            return;
        if (w->unk266 != 0)
            return;
        w->unk276++;
        if (w->unk276 > 0x30) {
            w->unk8 = 3;
            w->unkC = 0;
        }
        break;
    case 3:
        w->unkC = func_02068bd8(w->unkC, 0x1000, 0, 1, 0, 0);
        w->unk10 = func_02068bb8(0xd400, 0x8000, w->unkC);
        w->unk14 = func_02068bb8(0xc00, 0x2800, w->unkC);
        if (w->unk10 != 0xd400)
            return;
        if (w->unk14 != 0xc00)
            return;
        w->unk62 = 0x40;
        func_ov000_0232d05c(w);
        break;
    }
}
