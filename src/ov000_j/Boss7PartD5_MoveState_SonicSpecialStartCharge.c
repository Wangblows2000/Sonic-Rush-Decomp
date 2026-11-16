#include <nitro/types.h>

typedef struct {
    u8 pad0[0x114];
    s32 unk114;
} UnkStruct_022e7f00_124;

typedef struct {
    u8 pad0[0x5c];
    u16 unk5C;
    u8 pad5E[0xbe - 0x5e];
    s16 unkBE;
    u8 padC0[0x106 - 0xc0];
    s16 unk106;
    u8 pad108[0x124 - 0x108];
    UnkStruct_022e7f00_124 *unk124;
    u8 pad128[0x440 - 0x128];
    void *unk440;
} UnkStruct_022e7f00;

typedef struct {
    u8 pad0[0x22c];
    s16 unk22C;
} UnkStruct_022e7f00_g;

extern UnkStruct_022e7f00_g *gBoss7;

void Boss7_SetRivalAnimation(UnkStruct_022e7f00 *p, s32 a);
void Boss7PartD5_MoveState_SonicSpecialCharge(UnkStruct_022e7f00 *p);

void Boss7PartD5_MoveState_SonicSpecialStartCharge(UnkStruct_022e7f00 *p) {
    gBoss7->unk22C = -8;
    Boss7_SetRivalAnimation(p, 0x32);
    p->unk5C |= 4;
    p->unkBE = 0;
    p->unk106 = 0;
    p->unk124->unk114 = 0;
    p->unk440 = Boss7PartD5_MoveState_SonicSpecialCharge;
}
