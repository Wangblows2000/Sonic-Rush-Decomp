#include <nitro/types.h>

typedef struct UnkStruct_022dbd04 {
    u8 pad[0x4dc];
    s32 unk4DC;
    u8 pad1[0x4f0 - 0x4e0];
    void (*unk4F0)(void);
    s32 unk4F4;
} UnkStruct_022dbd04;

extern u32 gRandSeed;

void func_ov000_022dbcb8(void);

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

void func_ov000_022dbd04(UnkStruct_022dbd04 *s) {
    s->unk4F0 = func_ov000_022dbcb8;
    s->unk4F4 = 0;
    s->unk4DC = (Rand() & 1) ? 0x1600 : 0xa00;
}
