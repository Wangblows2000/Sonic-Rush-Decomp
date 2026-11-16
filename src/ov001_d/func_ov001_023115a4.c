#include <nitro/types.h>

typedef struct UnkStruct_023115a4 {
    u8 pad0[0x12];
    u16 unk12;
    u8 pad14[0x128 - 0x14];
    s32 unk128;
    u8 pad12C[0x14c - 0x12c];
    s16 unk14C;
    u16 unk14E;
    s16 unk150;
    s16 unk152;
    s32 unk154;
    s32 unk158;
} UnkStruct_023115a4;

extern s32 data_ov001_023178cc[];
extern u32 gRandSeed;

s16 func_ov001_023146b4(s16 a, u16 b, s16 c);
void func_ov001_0231599c(s32 v);
void func_ov001_023122d8(UnkStruct_023115a4 *p, s32 n);
void Sound_PlaySeqArc(void *p, u32 a, u32 b);
void func_ov001_0230d6f4(s32 v);
void func_ov001_023098cc(s32 v);
s32 func_ov001_02314614(s16 a, u16 b);
s16 func_ov001_02314694(s16 a);

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return gRandSeed >> 16;
}

void func_ov001_023115a4(UnkStruct_023115a4 *p) {
    s32 n;

    p->unk14C = func_ov001_023146b4(p->unk150, p->unk14E, p->unk14C);
    p->unk14E++;
    p->unk12 = 0;
    if (p->unk14C == -1) {
        func_ov001_0231599c(0);
        func_ov001_023122d8(p, 0xe);
        p->unk128 = 0;
        Sound_PlaySeqArc(0, 0x10, 0xb);
        Sound_PlaySeqArc(0, 0x10, 0x10);
        func_ov001_0230d6f4(0x800);
        func_ov001_023098cc(3);
        return;
    }
    if (p->unk14C == 0x1234) {
        n = func_ov001_02314614(p->unk150, p->unk14E);
        p->unk158 = n;
        p->unk154 = data_ov001_023178cc[n];
        p->unk152 = func_ov001_02314694(p->unk150);
        func_ov001_023098cc(n);
        func_ov001_023122d8(p, Rand() % 3 + 0xb);
    }
}
