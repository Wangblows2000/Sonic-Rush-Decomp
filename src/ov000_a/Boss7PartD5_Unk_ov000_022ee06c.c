#include <nitro/types.h>

typedef struct UnkStruct_022ee06c {
    s32 m[9];
} UnkStruct_022ee06c;

extern u32 gRandSeed;
extern s16 data_0206e470[];

u8 *BossEffect_CreateHit(s32 a, s32 b, s32 c, s32 d);
void MTX_RotX33_(void *m, s32 s, s32 c);
void MTX_RotY33_(void *m, s32 s, s32 c);
void MTX_Concat33(void *a, void *b, void *c);

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

void Boss7PartD5_Unk_ov000_022ee06c(s32 a, s32 b, s32 c) {
    UnkStruct_022ee06c mtx;
    u8 *obj = BossEffect_CreateHit(1, a, b, c);
    u16 ang = (Rand() & 0xfff) + 0xaaa;
    MTX_RotX33_(obj + 0x158, data_0206e470[((ang & 0xffff) >> 4) * 2], data_0206e470[((ang & 0xffff) >> 4) * 2 + 1]);
    ang = Rand();
    MTX_RotY33_(&mtx, data_0206e470[((ang & 0xffff) >> 4) * 2], data_0206e470[((ang & 0xffff) >> 4) * 2 + 1]);
    MTX_Concat33(obj + 0x158, &mtx, obj + 0x158);
}
