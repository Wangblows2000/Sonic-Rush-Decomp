#include <nitro/types.h>

typedef struct UnkStruct_022c7be4 UnkStruct_022c7be4;
struct UnkStruct_022c7be4 {
    u8 pad[0x68];
    void (*func)(UnkStruct_022c7be4 *s);
    u8 pad1[0x20c - 0x6c];
    u16 unk20C;
    s16 unk20E;
    u16 unk210;
    u8 pad2[2];
    s32 unk214;
};

extern u32 gRandSeed;

s32 func_ov000_022cc4cc(s32 x, s32 y, s32 z, s32 a);
void Boss1Neck_State_Unk_ov000_022c7b20(UnkStruct_022c7be4 *s);

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

void Boss1Neck_State_Unk_ov000_022c7be4(UnkStruct_022c7be4 *s) {
    s->unk214 = (Rand() & 0xff) * 0x1c2;
    s->unk20E = func_ov000_022cc4cc(s->unk214, 0x20000, 0, s->unk20C & 1);
    s->unk210 = 0;
    s->func = Boss1Neck_State_Unk_ov000_022c7b20;
}
