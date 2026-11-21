#include <nitro/types.h>

typedef struct UnkStruct_02311700 {
    u8 pad0[0x12];
    u16 rot;
    u8 pad14[0x20 - 0x14];
    s32 x;
    s32 y;
    s32 z;
    u8 pad2C[0x48 - 0x2c];
    s32 unk48;
    u8 pad4C[0x6c - 0x4c];
    u32 unk6C;
    u8 pad70[0x150 - 0x70];
    s16 unk150;
    u8 pad152[0x15c - 0x152];
    s32 unk15C;
} UnkStruct_02311700;

extern u32 gRandSeed;

void SpecialStagePlayer_Action_StartTrick(UnkStruct_02311700 *p, s32 v);
void SpecialStageActObj_SpawnAt(s32 type, s32 x, s32 y, s32 z);

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return gRandSeed >> 16;
}

void SpecialStagePlayer_State_TrickJump(UnkStruct_02311700 *p) {
    if (p->unk15C < 3) {
        u16 old = p->rot;
        p->rot += 0xb00;
        if (old > 0x8000 && p->rot <= 0x8000) {
            p->unk15C++;
            if (p->unk15C >= 3)
                p->rot = 0;
        }
    }
    if (p->unk48 < 0) {
        SpecialStagePlayer_Action_StartTrick(p, p->unk150);
        return;
    }
    if (p->unk48 <= 0x800)
        return;
    if (!(p->unk6C & 1))
        return;
    SpecialStageActObj_SpawnAt(2, p->x + Rand() % 0x6000 - 0x3000, p->y + Rand() % 0x6000 - 0x3000, p->z);
}
