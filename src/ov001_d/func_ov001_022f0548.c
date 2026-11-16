#include <nitro/types.h>

typedef struct UnkStruct_022f0548_o {
    u8 pad0[0xe4];
    s32 *unkE4;
    u8 padE8[0x10c - 0xe8];
    u8 unk10C;
    u8 pad10D[3];
    u8 unk110;
    u8 pad111[3];
    s32 unk114;
} UnkStruct_022f0548_o;

typedef struct UnkStruct_022f0548_q {
    u8 pad0[0x84];
    UnkStruct_022f0548_o *unk84;
} UnkStruct_022f0548_q;

typedef struct UnkStruct_022f0548_s {
    u8 pad0[0x18];
    UnkStruct_022f0548_q *unk18;
} UnkStruct_022f0548_s;

typedef struct UnkStruct_022f0548 {
    u8 pad0[0x74];
    void (*unk74)(void);
    u8 pad78[0x84 - 0x78];
    UnkStruct_022f0548_o *unk84;
    u8 pad88[0x1c8 - 0x88];
    s32 unk1C8;
    u8 pad1CC[0x1dc - 0x1cc];
    UnkStruct_022f0548_s *unk1DC;
} UnkStruct_022f0548;

typedef struct UnkStruct_022f0548_t {
    u8 pad0[0xa0];
    u16 unkA0;
    u16 unkA2;
    u16 unkA4;
    u16 unkA6;
} UnkStruct_022f0548_t;

extern void *data_ov001_0231f068;
extern void *data_ov001_0231f074;
extern UnkStruct_022f0548_t data_ov001_02316840;
extern u32 gRandSeed;

void func_ov001_022f034c(void);
void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);
u8 *ExtraBoss_GetWork(void);
void Sound_PlaySeqArc(void *p, u32 a, u32 b);
void func_020366f4(void *p, u32 a, u32 b);

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return gRandSeed >> 16;
}

void func_ov001_022f0548(UnkStruct_022f0548 *p) {
    UnkStruct_022f0548_o *a = p->unk84;
    UnkStruct_022f0548_o *b;
    s32 r;

    if (!(a->unk10C & 2))
        return;
    b = p->unk1DC->unk18->unk84;
    p->unk74 = func_ov001_022f034c;
    Model_SetAnimation(a, 0, data_ov001_0231f068, data_ov001_02316840.unkA0, 0);
    Model_SetAnimation(a, 4, data_ov001_0231f074, data_ov001_02316840.unkA2, 0);
    a->unk10C |= 4;
    a->unk110 |= 4;
    a->unk114 = 0x1000;
    Model_SetAnimation(b, 0, data_ov001_0231f068, data_ov001_02316840.unkA4, 0);
    Model_SetAnimation(b, 4, data_ov001_0231f074, data_ov001_02316840.unkA6, 0);
    b->unk10C |= 4;
    b->unk110 |= 4;
    b->unk114 = 0x1000;
    r = Rand() & 0xff;
    if (r < 0x4d) {
        p->unk1C8 = 0x3c;
    } else if (r < 0xcd) {
        p->unk1C8 = 0x78;
    } else {
        p->unk1C8 = 0xd2;
    }
}
