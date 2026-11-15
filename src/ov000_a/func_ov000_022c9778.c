#include <nitro/types.h>

typedef struct UnkStruct_022c9778_3 {
    u8 pad[0x216];
    u16 unk216;
    u8 pad0[0x242 - 0x218];
    u16 unk242;
    u8 pad1[0x29a - 0x244];
    u16 unk29A;
    u8 pad2[0x304 - 0x29c];
    s32 unk304;
    u8 pad3[4];
    u8 unk30C[4];
} UnkStruct_022c9778_3;

typedef struct UnkStruct_022c9778_2 {
    u8 pad[0x214];
    UnkStruct_022c9778_3 *unk214;
} UnkStruct_022c9778_2;

typedef struct UnkStruct_022c9778_4 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_022c9778_4;

typedef struct UnkStruct_022c9778_5 {
    u8 pad[8];
    u16 unk8;
} UnkStruct_022c9778_5;

typedef struct UnkStruct_022c9778 UnkStruct_022c9778;
struct UnkStruct_022c9778 {
    u8 pad[0x68];
    void (*func)(UnkStruct_022c9778 *s);
    u8 pad1[0x20c - 0x6c];
    u16 unk20C;
    u8 pad1b[0x218 - 0x20e];
    u16 unk218;
    u8 pad2[0x220 - 0x21a];
    s32 unk220;
    u8 pad3[4];
    u8 unk228[4];
};

extern UnkStruct_022c9778_2 *data_ov000_02369acc;
extern UnkStruct_022c9778_4 data_022c4560;
extern UnkStruct_022c9778_5 data_ov000_02369ac0;
extern s16 data_ov000_02362d04[][8];
extern s16 data_ov000_02362ce4[][8];

BOOL func_ov000_022cc6d4(void);
void func_ov000_022cc00c(void *p, s32 a, s32 b, s32 c, s32 d);
void func_ov000_022c96b8(UnkStruct_022c9778 *s);

void func_ov000_022c9778(UnkStruct_022c9778 *s) {
    UnkStruct_022c9778_3 *p = data_ov000_02369acc->unk214;
    s32 v;
    BOOL b;
    s32 i;
    if (func_ov000_022cc6d4()) {
        b = TRUE;
        if (data_022c4560.unk10 != 2 && data_022c4560.unk10 != 3)
            b = FALSE;
        if (b)
            i = 1;
        else
            i = data_022c4560.unk14;
        v = data_ov000_02362d04[i][data_ov000_02369ac0.unk8 - 1];
    } else {
        b = TRUE;
        if (data_022c4560.unk10 != 2 && data_022c4560.unk10 != 3)
            b = FALSE;
        if (b)
            i = 1;
        else
            i = data_022c4560.unk14;
        v = data_ov000_02362ce4[i][data_ov000_02369ac0.unk8 - 1];
    }
    func_ov000_022cc00c(s->unk228, s->unk220, 0x11, v, 0);
    func_ov000_022cc00c(p->unk30C, p->unk304, 4, v, 0);
    p->unk216 |= 4;
    p->unk29A |= 4;
    p->unk242 &= ~4;
    s->unk20C = 0x40;
    s->unk218 = 0;
    s->func = func_ov000_022c96b8;
}
