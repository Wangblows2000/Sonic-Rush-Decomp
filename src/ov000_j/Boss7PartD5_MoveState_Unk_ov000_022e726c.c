#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2e];
    s16 unk2E;
    s16 unk30;
    u8 pad32[0x58 - 0x32];
    u32 unk58;
    u8 pad5C[0x440 - 0x5c];
    void *unk440;
} UnkStruct_022e726c;

typedef struct {
    u8 pad0[4];
    s32 unk4;
} UnkStruct_02369ac0;

extern UnkStruct_02369ac0 gBossStatus;

void Boss7_Unk_ov000_022ee198(UnkStruct_022e726c *p, s32 a);
void Boss7PartD5_MoveState_Unk_ov000_022e7210(UnkStruct_022e726c *p);

void Boss7PartD5_MoveState_Unk_ov000_022e726c(UnkStruct_022e726c *p) {
    gBossStatus.unk4 = 0x2d;
    Boss7_Unk_ov000_022ee198(p, 0xf);
    p->unk58 |= 0x190;
    p->unk58 &= ~1;
    p->unk2E = 0x300;
    p->unk30 = -0x600;
    p->unk440 = Boss7PartD5_MoveState_Unk_ov000_022e7210;
}
