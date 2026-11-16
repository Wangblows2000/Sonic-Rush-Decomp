#include <nitro/types.h>

typedef struct UnkStruct_022f18e4_o {
    u8 pad0[0xe4];
    s32 *unkE4;
    u8 padE8[0x10c - 0xe8];
    u8 unk10C;
    u8 pad10D[3];
    u8 unk110;
    u8 pad111[3];
    s32 unk114;
} UnkStruct_022f18e4_o;

typedef struct UnkStruct_022f18e4_q {
    u8 pad0[0x84];
    UnkStruct_022f18e4_o *unk84;
} UnkStruct_022f18e4_q;

typedef struct UnkStruct_022f18e4_s {
    u8 pad0[0x18];
    UnkStruct_022f18e4_q *unk18;
} UnkStruct_022f18e4_s;

typedef struct UnkStruct_022f18e4 {
    u8 pad0[0x74];
    void (*unk74)(void);
    u8 pad78[0x84 - 0x78];
    UnkStruct_022f18e4_o *unk84;
    u8 pad88[0x1c8 - 0x88];
    s32 unk1C8;
    u8 pad1CC[0x1dc - 0x1cc];
    UnkStruct_022f18e4_s *unk1DC;
} UnkStruct_022f18e4;

typedef struct UnkStruct_022f18e4_t {
    u8 pad0[0x68];
    u16 unk68;
    u16 unk6A;
    u16 unk6C;
    u16 unk6E;
} UnkStruct_022f18e4_t;

extern void *data_ov001_0231f068;
extern void *data_ov001_0231f074;
extern UnkStruct_022f18e4_t data_ov001_02316840;
extern u32 gRandSeed;

void ExtraBossObj_State_Unk_ov001_022f167c(void);
void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);
u8 *ExtraBoss_GetWork(void);
void Sound_PlaySeqArc(void *p, u32 a, u32 b);
void Sound_PlayVoice(void *p, u32 a, u32 b);

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return gRandSeed >> 16;
}

void ExtraBossObj_State_Unk_ov001_022f18e4(UnkStruct_022f18e4 *p) {
    UnkStruct_022f18e4_o *a = p->unk84;
    UnkStruct_022f18e4_o *b;
    s32 r;

    if (!(a->unk10C & 2))
        return;
    b = p->unk1DC->unk18->unk84;
    p->unk74 = ExtraBossObj_State_Unk_ov001_022f167c;
    Model_SetAnimation(a, 0, data_ov001_0231f068, data_ov001_02316840.unk68, 0);
    Model_SetAnimation(a, 4, data_ov001_0231f074, data_ov001_02316840.unk6A, 0);
    a->unk10C &= ~4;
    a->unk110 |= 4;
    a->unk114 = 0;
    *a->unkE4 = 0xb4000;
    Model_SetAnimation(b, 0, data_ov001_0231f068, data_ov001_02316840.unk6C, 0);
    Model_SetAnimation(b, 4, data_ov001_0231f074, data_ov001_02316840.unk6E, 0);
    b->unk10C &= ~4;
    b->unk110 |= 4;
    b->unk114 = 0;
    *b->unkE4 = 0xb4000;
    r = Rand() & 0xff;
    if (r < 0x4d) {
        p->unk1C8 = 0x1e;
    } else if (r < 0xcd) {
        p->unk1C8 = 0x78;
    } else {
        p->unk1C8 = 0xd2;
    }
    Sound_PlaySeqArc(ExtraBoss_GetWork() + 0x33c, 0, 0xb3);
    Sound_PlayVoice(0, 0, 0xd1);
}
