#include <nitro/types.h>

typedef struct UnkStruct_023523d0 UnkStruct_023523d0;
struct UnkStruct_023523d0 {
    u8 unk0;
    u8 unk1;
    u8 pad2[8 - 2];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u8 pad18[0x28 - 0x18];
    s16 unk28;
    s16 unk2A;
    u8 pad2C[0x2E - 0x2C];
    s16 unk2E;
    s16 unk30;
    u8 pad32[0x34 - 0x32];
    s16 unk34;
    u8 pad36[0x54 - 0x36];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u16 unk5E;
    u16 unk60;
    u8 pad62[0x68 - 0x62];
    void (*state)(UnkStruct_023523d0 *w);
    u8 pad6C[0x2CC - 0x6C];
    u8 unk2CC[0x324 - 0x2CC];
    u8 unk324;
    u8 pad325[0x32A - 0x325];
    s16 unk32A;
    u32 unk32C;
    u8 pad330[0x384 - 0x330];
    u8 unk384[0x3E8 - 0x384];
    UnkStruct_023523d0 *other;
    u8 pad3EC[0x3F0 - 0x3EC];
    s32 unk3F0;
    s32 unk3F4;
    s32 unk3F8;
};

void Player_State_SandSlider90(UnkStruct_023523d0 *w);
void Player_ResetGimmickState(UnkStruct_023523d0 *w);
void Player_SetAnimation(UnkStruct_023523d0 *w, s32 a);
void func_ov000_02346d10(UnkStruct_023523d0 *w);
void func_ov000_02346aa8(UnkStruct_023523d0 *w);
void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);

void Player_Action_SandSlider90(UnkStruct_023523d0 *w, UnkStruct_023523d0 *other) {
    if (w->state == Player_State_SandSlider90)
        return;
    Player_ResetGimmickState(w);
    w->other = other;
    Player_SetAnimation(w, 0x31);
    w->unk5C |= 4;
    w->state = Player_State_SandSlider90;
    w->unk58 |= 0x10;
    w->unk58 |= 0x100;
    w->unk58 &= ~1;
    w->unk54 &= ~1;
    func_ov000_02346d10(w);
    func_ov000_02346aa8(w);
    w->unk32C |= 0x100000;
    w->unk32C &= ~1;
    w->unk2E = 0;
    w->unk34 = 0;
    w->unk28 = 0;
    w->unk2A = 0;
    w->unk1 = 0;
    w->unk8 = 0;
    w->unkC = 0;
    if (w->unk324 != 1) {
        Sound_PlaySeqArcEx(w->unk384, 0, 0);
    } else {
        Sound_PlaySeqArcEx(w->unk384, 0, 0x11);
    }
}
