#include <nitro/types.h>

typedef struct UnkStruct_02302418 {
    u8 pad[0x30];
    s16 unk30;
    u8 pad1[0x44 - 0x32];
    u16 unk44;
    u16 unk46;
    u8 pad2[0x54 - 0x48];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad3[0x68 - 0x5e];
    void (*func)(void);
} UnkStruct_02302418;

extern u32 gRandSeed;

void StageObj_SetAnimation(UnkStruct_02302418 *s, s32 a);
void Bubble_State_Grow(void);

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

void Bubble_Action_Spawn(UnkStruct_02302418 *s) {
    StageObj_SetAnimation(s, 0);
    s->unk5C |= 4;
    s->unk58 |= 0x10;
    s->unk44 = 0;
    s->unk46 = 0;
    s->unk30 = (-0xc1 - (Rand() & 0x1e)) >> 1;
    s->unk54 |= 2;
    s->func = Bubble_State_Grow;
}
