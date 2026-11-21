#include <game/Stage/stageobj_alt.h>

typedef struct {
    u8 pad0[8];
    s32 unk8;
    u32 unkC;
    u8 pad10[0x28 - 0x10];
    s16 unk28;
    s16 unk2A;
    s16 unk2C;
    u8 pad2E[2];
    s16 unk30;
    u8 pad32[0x54 - 0x32];
    u32 unk54;
    u32 unk58;
    u8 pad5C[0x68 - 0x5C];
    void *unk68;
} UnkStruct_0232727c;

typedef struct {
    u8 pad0[0x20];
    u32 unk20;
} UnkStruct_0232727c_Sys;

extern UnkStruct_0232727c_Sys gGameState;

UnkStruct_0232727c *EffFire01_3D_Create(void *p, s16 a, s16 b, s16 c);
void EffFire01_3D_State_FollowOffset(void);

void EffFire01_3D_CreateFollowing(void *p, s16 a, s16 b, s16 c) {
    UnkStruct_0232727c *e;

    a += 3 - (Rand() & 6);
    b += 3 - (Rand() & 6);
    c += 3 - (Rand() & 6);
    e = EffFire01_3D_Create(p, a, b, c);
    e->unk30 = -0x80;
    e->unkC = (gGameState.unk20 & 0xf) << 12;
    e->unk28 = a;
    e->unk2A = b;
    e->unk2C = c;
    e->unk8 = 0x200;
    e->unk58 |= 0x2100;
    e->unk54 |= 2;
    e->unk68 = EffFire01_3D_State_FollowOffset;
}
