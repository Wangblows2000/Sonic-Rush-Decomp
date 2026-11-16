#include <ov000_c/stageobj.h>

typedef struct {
    u16 b0 : 5;
    u16 b5 : 11;
} UnkStruct_Bits5;

typedef struct {
    u8 pad0[0x24];
    UnkStruct_Bits5 unk24;
} UnkStruct_0208922c;

typedef struct {
    UnkStruct_StageObj base;
    u32 unk20C;
    s32 unk210;
    s16 unk214;
    s16 unk216;
    u8 pad218[0x21C - 0x218];
    s16 unk21C;
    u8 pad21E[2];
    s32 unk220;
    u8 pad224[0x22C - 0x224];
    s32 unk22C;
    u8 pad230[0x254 - 0x230];
    s32 unk254;
} UnkStruct_022f75d8;

extern volatile UnkStruct_0208922c data_0208922c;
extern UnkStruct_Bits5 data_02089250;

void func_ov000_022f7804(UnkStruct_022f75d8 *w);
s32 func_ov000_022fad3c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i);

#define FX_MUL(a, b) ((s32)(((s64)(a) * (b) + 0x800) >> 12))

void func_ov000_022f75d8(UnkStruct_022f75d8 *w) {
    s32 x;
    u32 f;
    s32 v;

    if (!(w->unk20C & 0x10) && data_0208922c.unk24.b0 != 0 && !(w->unk210 & 3)) {
        data_02089250.b0 -= 1;
        if (data_0208922c.unk24.b0 == 0)
            w->unk20C |= 0x10;
    }
    func_ov000_022f7804(w);
    w->unk220 += w->base.unk2E;
    x = w->unk220;
    if (x < 0x1800)
        x = 0x1800;
    else if (x > 0x32800)
        x = 0x32800;
    w->unk220 = x;
    f = w->unk20C;
    if (!(f & 1)) {
        if (w->base.unk2E > 0 && w->unk220 >= w->unk22C)
            w->unk20C = f | 1;
        else if (w->base.unk2E < 0 && w->unk220 <= w->unk22C)
            w->unk20C |= 1;
        v = w->base.unk2E;
        if (v < 0)
            v = -v;
        w->unk21C += v;
        if (w->unk21C >= 0x2000) {
            w->unk21C -= 0x2000;
            func_ov000_022fad3c(0xdd, w->unk220, 0x30000, 0xf, 0, 0, 0, 0, 0);
        }
    } else {
        w->unk20C = f | 8;
        w->unk216 += 0x88;
        if (w->unk216 >= 0x1000) {
            w->unk216 = 0x1000;
            w->base.unk54 |= 4;
        }
        w->base.unk2E = w->unk214 + FX_MUL(-w->unk214, w->unk216);
        w->unk254 = FX_MUL(w->unk216, -0x3333) + 0x34cc;
    }
    w->unk210++;
}
