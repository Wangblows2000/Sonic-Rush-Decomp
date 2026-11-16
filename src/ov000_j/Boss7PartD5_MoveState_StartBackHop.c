#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2e];
    s16 unk2E;
    s16 unk30;
    u8 pad32[0x58 - 0x32];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x440 - 0x5e];
    void *unk440;
} UnkStruct_022e9488;

typedef struct {
    u8 pad0[4];
    s32 unk4;
} UnkStruct_02369ac0;

extern UnkStruct_02369ac0 gBossStatus;

void Boss7_SetRivalAnimation(UnkStruct_022e9488 *p, s32 a);
void Boss7_SetCollidersEnabled(UnkStruct_022e9488 *p, s32 a, s32 b);
void Boss7PartD5_MoveState_BackHop(UnkStruct_022e9488 *p);

void Boss7PartD5_MoveState_StartBackHop(UnkStruct_022e9488 *p) {
    Boss7_SetRivalAnimation(p, 0x15);
    Boss7_SetCollidersEnabled(p, 0, 0);
    if (p->unk5C & 1)
        p->unk2E = 0xa00;
    else
        p->unk2E = -0xa00;
    p->unk30 = -0x140;
    p->unk58 |= 0x10;
    gBossStatus.unk4 = 0x26;
    p->unk440 = Boss7PartD5_MoveState_BackHop;
}
