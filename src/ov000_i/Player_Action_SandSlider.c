#include <nitro/types.h>

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[0x8 - 2];
    s32 unk8;
    s32 unkC;
    u8 pad10[0x28 - 0x10];
    s16 unk28;
    s16 unk2A;
    u8 pad2C[0x2e - 0x2c];
    s16 unk2E;
    u8 pad30[0x34 - 0x30];
    s16 unk34;
    u8 pad36[0x54 - 0x36];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x68 - 0x5e];
    void *unk68;
    u8 pad6C[0x32c - 0x6c];
    u32 unk32C;
    u8 pad330[0x3e8 - 0x330];
    s32 unk3E8;
} UnkStruct_02351f50;

void Player_State_SandSlider(void);
s32 Player_ResetGimmickState();
s32 func_ov000_0234c644();
s32 Player_SetAnimation();

void Player_Action_SandSlider(UnkStruct_02351f50 *w, s32 a) {
    if (w->unk68 == Player_State_SandSlider)
        return;
    Player_ResetGimmickState(w);
    func_ov000_0234c644(w);
    w->unk3E8 = a;
    Player_SetAnimation(w, 0x31);
    w->unk5C |= 4;
    w->unk68 = Player_State_SandSlider;
    w->unk58 |= 0x10;
    w->unk58 |= 0x100;
    w->unk58 |= 0x40000;
    w->unk58 &= ~1;
    w->unk58 &= ~0x200;
    w->unk32C |= 0x100000;
    w->unk32C &= ~1;
    w->unk2E = 0;
    w->unk34 = 0;
    w->unk28 = 0;
    w->unk2A = 0;
    w->unk1 = 0;
    w->unk8 = 0;
    w->unkC = 0;
    w->unk54 |= 1;
    w->unk58 &= ~0x100;
}
