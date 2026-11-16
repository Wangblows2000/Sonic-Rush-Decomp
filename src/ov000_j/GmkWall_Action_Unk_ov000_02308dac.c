#include <nitro/types.h>

typedef struct {
    u8 pad0[0xc];
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u8 pad18[0x68 - 0x18];
    void *unk68;
    u8 pad6C[0x1d6 - 0x6c];
    s16 unk1D6;
} UnkStruct_02308dac;

BOOL Stage_IsGravityFlipped(s32 x, s32 y);
void GmkWall_State_Unk_ov000_02308c68(UnkStruct_02308dac *p);

void GmkWall_Action_Unk_ov000_02308dac(UnkStruct_02308dac *p) {
    p->unkC = 0x28;
    if (Stage_IsGravityFlipped(p->unk10, p->unk14) == 0)
        p->unk1D6 = 0x20;
    else
        p->unk1D6 = -0x60;
    p->unk68 = GmkWall_State_Unk_ov000_02308c68;
}
