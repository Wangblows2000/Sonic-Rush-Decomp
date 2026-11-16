#include <nitro/types.h>

typedef struct UnkStruct_022c8428_6 {
    u8 pad[4];
    u16 unk4;
} UnkStruct_022c8428_6;

typedef struct UnkStruct_022c8428_5 {
    s32 unk0;
    u8 pad[4];
    UnkStruct_022c8428_6 *unk8;
} UnkStruct_022c8428_5;

typedef struct UnkStruct_022c8428_3 {
    u8 pad[0x10];
    s32 unk10;
    u8 pad0[0x304 - 0x14];
    s32 unk304;
    u8 pad1[4];
    u8 unk30C[0x418 - 0x30c];
    u8 unk418;
    u8 pad2[0x420 - 0x419];
    s32 unk420;
} UnkStruct_022c8428_3;

typedef struct UnkStruct_022c8428_2 {
    u8 pad[0x214];
    UnkStruct_022c8428_3 *unk214;
} UnkStruct_022c8428_2;

typedef struct UnkStruct_022c8428 UnkStruct_022c8428;
struct UnkStruct_022c8428 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad0[0x2e - 0x1c];
    u16 unk2E;
    u8 pad0b[0x68 - 0x30];
    void (*func)(UnkStruct_022c8428 *s);
    u8 pad1[0x20c - 0x6c];
    u16 unk20C;
    u8 pad2[0x210 - 0x20e];
    u16 unk210;
    u8 pad2b[0x220 - 0x212];
    s32 unk220;
    s32 unk224;
    u8 unk228[0x30c - 0x228];
    UnkStruct_022c8428_5 *unk30C;
    u8 pad4[0x334 - 0x310];
    u8 unk334;
    u8 pad5[0x33c - 0x335];
    s32 unk33C;
};

extern UnkStruct_022c8428_2 *data_ov000_02369acc;

void func_ov000_022cc00c(void *p, s32 a, s32 b, s32 c, s32 d);
void func_ov000_022cc06c(void *p, s32 a, s32 b);

typedef struct UnkStruct_022c8428_4 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_022c8428_4;

typedef struct UnkStruct_022c8428_7 {
    u8 pad[8];
    u16 unk8;
} UnkStruct_022c8428_7;

extern UnkStruct_022c8428_4 data_022c4560;
extern UnkStruct_022c8428_7 data_ov000_02369ac0;
extern u16 data_ov000_02362d44[][8];
extern u16 data_ov000_02362d24[][8];

BOOL func_ov000_022cc6d4(void);
void func_ov000_022ca00c(UnkStruct_022c8428 *s);
BOOL func_ov000_022c7274(u32 mask);
void ScreenShake_Start(s32 a);
u8 *BossEffect_CreateImpactSmall(s32 a, s32 b, s32 c, s32 d);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void func_ov000_022c83d4(UnkStruct_022c8428 *s);
void func_ov000_022c8104(UnkStruct_022c8428 *s);

void func_ov000_022c8428(UnkStruct_022c8428 *s) {
    UnkStruct_022c8428_5 *q = s->unk30C;
    u32 v;
    BOOL b;
    s32 i;
    u8 *obj;
    func_ov000_022cc6d4();
    if (func_ov000_022cc6d4()) {
        b = TRUE;
        if (data_022c4560.unk10 != 2 && data_022c4560.unk10 != 3)
            b = FALSE;
        if (b)
            i = 1;
        else
            i = data_022c4560.unk14;
        v = data_ov000_02362d44[i][data_ov000_02369ac0.unk8 - 1];
    } else {
        b = TRUE;
        if (data_022c4560.unk10 != 2 && data_022c4560.unk10 != 3)
            b = FALSE;
        if (b)
            i = 1;
        else
            i = data_022c4560.unk14;
        v = data_ov000_02362d24[i][data_ov000_02369ac0.unk8 - 1];
    }
    if (v >= ((q->unk8->unk4 << 12) - q->unk0) >> 12)
        s->unk2E = 0;
    else
        func_ov000_022ca00c(s);
    if (s->unk20C == 0) {
        if (func_ov000_022c7274(2))
            s->unk20C = 1;
    }
    if (!(s->unk334 & 2))
        return;
    ScreenShake_Start(0xb);
    obj = BossEffect_CreateImpactSmall(1, data_ov000_02369acc->unk214->unk10, 0x20000, 0);
    *(s32 *)(obj + 0x248) = 0x1000;
    *(s32 *)(obj + 0x14c) = 0x34cc;
    *(s32 *)(obj + 0x150) = 0x34cc;
    *(s32 *)(obj + 0x154) = 0x34cc;
    Sound_PlaySeqArc(0, 0, 0x7f);
    s->unk210++;
    if (s->unk210 < 3)
        s->func = func_ov000_022c83d4;
    else
        s->func = func_ov000_022c8104;
}
