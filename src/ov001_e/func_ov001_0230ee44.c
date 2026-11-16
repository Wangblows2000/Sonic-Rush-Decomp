#include <nitro/types.h>

typedef struct UnkStruct_0230ee44_r {
    u8 pad0[0x48];
    s32 unk48;
    u8 pad4C[0x60 - 0x4c];
    u16 unk60;
    u8 pad62[2];
    u16 unk64;
} UnkStruct_0230ee44_r;

typedef struct UnkStruct_0230ee44 {
    u8 pad0[0x16];
    u16 rot;
    u8 pad18[4];
    s32 unk1C;
    u8 pad20[0x78 - 0x20];
    s32 unk78;
} UnkStruct_0230ee44;

extern u32 gRandSeed;

float func_0202f1bc(u32 v);
UnkStruct_0230ee44_r *func_ov001_02307148(s32 a, s32 b, u16 ang, s32 c, s32 speed);

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return gRandSeed >> 16;
}

void func_ov001_0230ee44(UnkStruct_0230ee44 *p) {
    UnkStruct_0230ee44_r *r;
    u16 v;
    float f;

    v = Rand();
    Rand();
    f = p->unk1C;
    r = func_ov001_02307148(p->unk78, 2, p->rot + (u16)(gRandSeed >> 16) % 0x2000 - 0x1000, 0,
                            f + 143360.0f * (func_0202f1bc(v) / 65535.0f) - 69632.0f);
    if (r == NULL)
        return;
    r->unk48 = 0x800;
    r->unk64 &= ~0x1000;
    r->unk64 &= ~0x2000;
    r->unk60 &= ~0x20;
}
