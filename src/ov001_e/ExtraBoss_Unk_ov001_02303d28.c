#include <nitro/types.h>

typedef struct UnkStruct_02303d28_n {
    s32 x;
    s32 y;
    s32 ax;
    s32 ay;
    s32 vx;
    s32 vy;
    s32 unk18;
    s32 unk1C;
} UnkStruct_02303d28_n;

extern s16 data_0206e470[];

UnkStruct_02303d28_n *ExtraBoss_StackPop(void);
void ExtraBoss_Unk_ov001_0230328c(UnkStruct_02303d28_n *n);
s32 FX_DivS32(s32 n, s32 d);

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void ExtraBoss_Unk_ov001_02303d28(s32 x, s32 y, s32 count) {
    s32 i;
    u16 ang;
    u16 step;
    UnkStruct_02303d28_n *n;

    if (count == 1) {
        ang = 0xbfff;
        step = 0;
    } else {
        ang = 0xaaaa;
        step = FX_DivS32(0x2aab, count - 1);
    }
    for (i = 0; i < count; i++, ang += step) {
        n = ExtraBoss_StackPop();
        n->x = x;
        n->y = y;
        n->ax = 0;
        n->ay = 0x100;
        n->vx = FX_Mul(data_0206e470[((ang & 0xffff) >> 4) * 2 + 1], 0x2000);
        n->vy = FX_Mul(data_0206e470[((ang & 0xffff) >> 4) * 2], 0x2000);
        n->unk18 = 2;
        n->unk1C = 5;
        ExtraBoss_Unk_ov001_0230328c(n);
    }
}
