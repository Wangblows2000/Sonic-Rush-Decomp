#include <nitro/types.h>

typedef struct UnkStruct_022f167c_a {
    u8 pad0[0xe4];
    s32 *unkE4;
} UnkStruct_022f167c_a;

typedef struct UnkStruct_022f167c_q {
    u8 pad0[0x14];
    s16 unk14;
    u8 pad16[2];
    struct UnkStruct_022f167c *unk18;
    u8 pad1C[0x13c - 0x1c];
    s32 unk13C;
    s32 unk140;
    s32 unk144;
} UnkStruct_022f167c_q;

typedef struct UnkStruct_022f167c {
    u8 pad0[0x18];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad24[0x74 - 0x24];
    void (*unk74)(void);
    u8 pad78[0x84 - 0x78];
    UnkStruct_022f167c_a *unk84;
    u8 pad88[0x128 - 0x88];
    u32 unk128;
    u8 pad12C[0x1c8 - 0x12c];
    s32 unk1C8;
    u8 pad1CC[4];
    s32 unk1D0;
    s32 unk1D4;
    s16 unk1D8;
    u8 pad1DA[2];
    UnkStruct_022f167c_q *unk1DC;
} UnkStruct_022f167c;

extern s32 data_ov001_0231f520;
extern s32 data_ov001_0231f524;
extern s16 data_0206e470[];

#define SIN_IDX(i) data_0206e470[(u32)((i) << 1)]
#define COS_IDX(i) data_0206e470[(u32)(((i) << 1) + 1)]

UnkStruct_022f167c *func_ov001_02300c94(void);
u16 FX_Atan2(s32 y, s32 x);
s32 FX_DivS32(s32 n, s32 d);
void func_ov001_022fa94c(s32 p0, s32 p1, s32 p2, s32 p3);
void func_ov001_022f1660(void);
void func_ov001_022f1550(void);
u8 *ExtraBoss_GetWork(void);
void NNS_SndPlayerStopSeq(void *pp, s32 a);
void Sound_PlaySeqArc(u32 *p, u32 a, u32 b);

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void func_ov001_022f167c(UnkStruct_022f167c *p) {
    UnkStruct_022f167c *t = func_ov001_02300c94();
    UnkStruct_022f167c_q *q = p->unk1DC;
    UnkStruct_022f167c *o2 = q->unk18;
    UnkStruct_022f167c_a *a = p->unk84;
    UnkStruct_022f167c_a *b = o2->unk84;
    u16 ang;
    s32 d;
    s32 idx;

    ang = FX_Atan2(t->unk1C - (p->unk1C + 0x19d50), t->unk18 - (p->unk18 - 0x10d60));
    if (ang < 0x4000)
        ang = 0x4000;
    else if (ang > 0xc000)
        ang = 0xc000;
    p->unk1C8--;
    if (p->unk1C8 > 0) {
        ang = FX_DivS32((u16)(ang - 0x4000), 0xb6);
        d = (ang << 13) - *a->unkE4;
        if (d > 0) {
            if (d >= 0x4000)
                d = 0x4000;
        } else if (d < 0) {
            if (d <= -0x4000)
                d = -0x4000;
        }
        *a->unkE4 += d;
        *b->unkE4 += d;
        return;
    }
    func_ov001_022fa94c(0, q->unk13C - data_ov001_0231f524, -(q->unk140 + data_ov001_0231f520), q->unk144);
    p->unk74 = func_ov001_022f1660;
    o2->unk74 = func_ov001_022f1550;
    ang = (*a->unkE4 >> 13) * 0xb6;
    idx = (s32)((u16)(ang + 0x4000) & 0xffff) >> 4;
    o2->unk1D0 = q->unk13C + FX_Mul(COS_IDX(idx), 0x320000);
    o2->unk1D4 = FX_Mul(SIN_IDX(idx), 0x320000) - q->unk140;
    o2->unk1D8 = 0;
    o2->unk18 = q->unk13C;
    o2->unk1C = -q->unk140;
    o2->unk20 = q->unk144;
    q->unk14 = 1;
    o2->unk128 &= ~0x200;
    NNS_SndPlayerStopSeq(ExtraBoss_GetWork() + 0x33c, 0);
    Sound_PlaySeqArc(0, 0, 0xb4);
}
