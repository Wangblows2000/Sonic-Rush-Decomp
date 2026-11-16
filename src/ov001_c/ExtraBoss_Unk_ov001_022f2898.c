#include <nitro/types.h>

typedef struct UnkStruct_022f2898_o {
    u8 pad0[0x10c];
    u8 unk10C;
    u8 pad10D[3];
    u8 unk110;
    u8 pad111[3];
    s32 unk114;
    u8 pad118[0x12c - 0x118];
    s16 unk12C;
} UnkStruct_022f2898_o;

typedef struct UnkStruct_022f2898_l {
    u8 pad0[0x18];
    struct UnkStruct_022f2898 *unk18;
} UnkStruct_022f2898_l;

typedef struct UnkStruct_022f2898 {
    u8 pad0[0x74];
    void (*unk74)(void);
    u8 pad78[0x84 - 0x78];
    UnkStruct_022f2898_o *unk84;
    u8 pad88[0xb8 - 0x88];
    u32 unkB8;
    u8 padBC[0xf0 - 0xbc];
    u32 unkF0;
    u8 padF4[0x128 - 0xf4];
    u32 unk128;
    u8 pad12C[0x160 - 0x12c];
    u32 unk160;
    u8 pad164[0x1c8 - 0x164];
    s32 unk1C8;
    u8 pad1CC[0x1dc - 0x1cc];
    UnkStruct_022f2898_l *unk1DC;
} UnkStruct_022f2898;

extern u16 data_ov001_02316840[];
extern void *data_ov001_0231f068;
extern void *data_ov001_0231f074;
extern s32 data_ov001_02316810[];
extern u32 gRandSeed;

void ExtraBossObj_State_Unk_ov001_022f2858(void);
void ExtraBossObj_State_Unk_ov001_022f2ca4(void);
void Model_SetAnimation(UnkStruct_022f2898_o *o, u32 slot, void *res, u32 idx, void *tex);
void ExtraBoss_Unk_ov001_022eecdc(UnkStruct_022f2898 *p);

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

void ExtraBoss_Unk_ov001_022f2898(UnkStruct_022f2898 *p) {
    UnkStruct_022f2898 *q = p->unk1DC->unk18;
    UnkStruct_022f2898_o *a = p->unk84;
    UnkStruct_022f2898_o *b = q->unk84;

    p->unk74 = ExtraBossObj_State_Unk_ov001_022f2858;
    q->unk74 = ExtraBossObj_State_Unk_ov001_022f2ca4;
    p->unkB8 &= ~0x200;
    p->unkF0 &= ~0x200;
    p->unk128 |= 0x200;
    p->unk160 |= 0x200;
    q->unkF0 |= 0x200;
    q->unk128 |= 0x200;
    a->unk10C |= 0x40;
    Model_SetAnimation(a, 0, data_ov001_0231f068, data_ov001_02316840[0], 0);
    Model_SetAnimation(a, 4, data_ov001_0231f074, data_ov001_02316840[1], 0);
    a->unk12C = 0x400;
    a->unk10C |= 4;
    a->unk110 |= 4;
    a->unk114 = 0x1000;
    b->unk10C |= 0x40;
    Model_SetAnimation(b, 0, data_ov001_0231f068, data_ov001_02316840[2], 0);
    Model_SetAnimation(b, 4, data_ov001_0231f074, data_ov001_02316840[3], 0);
    b->unk12C = 0x400;
    b->unk10C |= 4;
    b->unk110 |= 4;
    b->unk114 = 0x1000;
    p->unk1C8 = (Rand() & 0x3f) + data_ov001_02316810[Rand() & 3];
    ExtraBoss_Unk_ov001_022eecdc(p);
}
