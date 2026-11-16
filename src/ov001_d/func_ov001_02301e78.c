#include <nitro/types.h>

typedef struct UnkStruct_02301e78_d {
    u16 unk0;
    u8 pad2[0x14 - 2];
    s32 unk14;
} UnkStruct_02301e78_d;

typedef struct UnkStruct_02301e78_s {
    u8 pad0[8];
    UnkStruct_02301e78_d *unk8;
} UnkStruct_02301e78_s;

typedef struct UnkStruct_02301e78 {
    u8 pad0[0x14];
    u32 unk14;
    u8 pad18[0x3c - 0x18];
    u16 unk3C;
    u8 pad3E[0x50 - 0x3e];
    s32 unk50;
    s32 unk54;
    u8 pad58[0x9c - 0x58];
    void *unk9C;
    void *unkA0;
    u8 padA4[0xb0 - 0xa4];
    u8 unkB0[0x1c8 - 0xb0];
    u16 unk1C8;
    u8 pad1CA[2];
    s32 unk1CC;
    u8 pad1D0[0x1dc - 0x1d0];
    UnkStruct_02301e78_s *unk1DC;
} UnkStruct_02301e78;

typedef struct UnkStruct_02301e78_g {
    u8 pad0[0x1c];
    u16 unk1C;
    u8 pad1E[0x3c - 0x1e];
    s32 unk3C;
} UnkStruct_02301e78_g;

void func_ov001_02302398(void);
UnkStruct_02301e78_g *ExtraBoss_GetWork(void);
BOOL func_ov001_02302408(UnkStruct_02301e78 *p);
void func_ov001_023024d0(UnkStruct_02301e78 *p, s32 n);
s32 FX_Sqrt(s32 a);
s32 func_0200225c(s32 n, s32 d);
void func_ov001_023017c8(UnkStruct_02301e78 *p);

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void func_ov001_02301e78(UnkStruct_02301e78 *p) {
    UnkStruct_02301e78_s *s = p->unk1DC;
    s32 half;
    s32 d;
    s32 base;

    func_ov001_02302398();
    if (ExtraBoss_GetWork()->unk3C == 0) {
        p->unk1CC--;
        if (p->unk1CC <= 0)
            p->unk9C = NULL;
    }
    if (p->unk50 == 0 && p->unk54 == 0) {
        if (!func_ov001_02302408(p)) {
            func_ov001_023024d0(p, 1);
            p->unk14 &= ~0x700;
        }
        p->unkA0 = NULL;
        p->unk9C = p->unkB0;
        return;
    }
    half = s->unk8->unk14 >> 1;
    d = FX_Sqrt(FX_Mul(p->unk50, p->unk50) + FX_Mul(p->unk54, p->unk54));
    if (half > d) {
        base = p->unk1C8 > 0x8000 ? 0x10000 : 0;
        p->unk3C = (s32)(p->unk1C8 * d) / half + base * (half - d) / half;
    }
    if (func_0200225c(d, s->unk8->unk14) >= 0x800)
        return;
    if (s->unk8->unk0 & ExtraBoss_GetWork()->unk1C)
        func_ov001_023017c8(p);
}
