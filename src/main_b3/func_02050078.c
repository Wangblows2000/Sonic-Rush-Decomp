#include <nitro/types.h>

typedef struct {
    u32 unk0;
    s32 unk4;
    s32 unk8;
    u8 padC[0x8];
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 pad54[0x300];
    s32 unk354;
    s32 unk358;
    s32 unk35C;
    u8 pad360[0x1a8];
    s32 unk508[5];
    s32 unk51C[8];
    s32 unk53C;
    s32 unk540;
    s32 unk544;
    s32 unk548;
    s32 unk54C;
    u8 pad550[0x34];
    s32 unk584;
} UnkWork_02050078;

typedef struct { u8 unk0[0x10]; UnkWork_02050078 *work; } UnkTask_02050078;

extern UnkTask_02050078 *data_022c45fc;
extern s32 data_02074ca8[];
extern s32 data_02074c3c[];
extern s32 data_02074bf0[];
extern s32 data_02074bdc[];

void func_0205002c(void);
UnkTask_02050078 *func_020372d0(void (*f)(void), void (*g)(void), s32 b, s32 c, s32 d, s32 e);
void MIi_CpuClear32(u32 v, void *dst, u32 size);
void func_0204fe40(void);
void func_0204fe60(void);
s32 func_0204f17c(s32 a, s32 b);
s32 func_0204f0c8(s32 a, s32 b);
void func_0204f2fc(UnkWork_02050078 *w, s32 a);
BOOL func_020521a0(void);
void func_020522f4(void);

#pragma thumb on
void func_02050078(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7) {
    UnkWork_02050078 *w;
    s32 i;
    vu32 zero;

    func_0205002c();
    if (a0 >= 0x1b)
        a0 = 0x1c;
    if (a1 >= 2)
        a1 = 3;
    if (a2 >= 4)
        a2 = 5;
    data_022c45fc = func_020372d0(func_0204fe40, func_0204fe60, 0, 0x1000, 0, sizeof(UnkWork_02050078));
    w = data_022c45fc->work;
    zero = 0;
    MIi_CpuClear32(zero, w, sizeof(UnkWork_02050078));
    w->unk0 = 0;
    w->unk1C = a0;
    w->unk20 = a1;
    w->unk24 = a2;
    w->unk28 = a3;
    w->unk2C = a4;
    w->unk34 = a5;
    w->unk38 = a7;
    w->unk48 = 0;
    w->unk4C = 0;
    w->unk18 = 1;
    switch (w->unk2C) {
    case 0:
        w->unk3C = 1;
        break;
    case 1:
        w->unk3C = 2;
        break;
    case 2:
        w->unk3C = 4;
        break;
    case 3:
        w->unk3C = 8;
        break;
    }
    w->unk40 = a6;
    w->unk50 = 1;
    for (i = 0; i < 5; i++)
        w->unk508[i] = -1;
    switch (w->unk24) {
    case 0:
        w->unk4 = 0;
        w->unk8 = 1;
        break;
    case 1:
        w->unk4 = 1;
        w->unk8 = 2;
        break;
    case 2:
        w->unk4 = 0;
        w->unk8 = 1;
        break;
    case 3:
    default:
        w->unk4 = 0;
        w->unk8 = 0;
        break;
    }
    if (w->unk24 == 1)
        w->unk51C[0] = 1;
    else
        w->unk51C[0] = 0;
    w->unk51C[1] = 1;
    w->unk51C[2] = 0;
    w->unk51C[4] = 0;
    w->unk51C[3] = 0;
    w->unk51C[5] = 0;
    w->unk51C[6] = 0;
    w->unk51C[7] = 0;
    if (w->unk1C != 0x1c)
        w->unk51C[data_02074ca8[w->unk1C]] = 1;
    w->unk53C = 0;
    w->unk540 = 0;
    w->unk544 = 0;
    w->unk548 = 0;
    w->unk54C = 0;
    if (w->unk1C != 0x1c)
        w->unk44 = data_02074c3c[w->unk1C];
    else
        w->unk44 = 0;
    switch (w->unk44) {
    case 1:
        w->unk540 = 1;
        break;
    case 0:
        w->unk53C = 1;
        break;
    case 2:
        w->unk544 = 1;
        break;
    case 3:
        w->unk548 = 1;
        break;
    case 4:
        w->unk54C = 1;
        break;
    default:
        w->unk53C = 1;
        break;
    }
    w->unk584 = 0;
    w->unk14 = 0;
    w->unk354 = func_0204f17c(w->unk28, w->unk2C);
    w->unk30 = func_0204f0c8(w->unk28, w->unk2C);
    w->unk358 = data_02074bf0[w->unk44];
    w->unk35C = data_02074bdc[w->unk44];
    func_0204f2fc(w, 0);
    func_0204fe40();
    if (!func_020521a0()) {
        func_020522f4();
        w->unk4C = 1;
    }
}
#pragma thumb reset
