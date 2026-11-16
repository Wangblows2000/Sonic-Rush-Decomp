#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    s32 unk8;
    s32 unkC;
    u8 pad10[0x30 - 0x10];
    s16 unk30;
    u8 pad32[0x58 - 0x32];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x68 - 0x5E];
    void (*unk68)(void);
    u8 pad6C[0x32C - 0x6C];
    u32 unk32C;
    u8 pad330[0x384 - 0x330];
    u8 unk384[0x3F0 - 0x384];
    s32 unk3F0;
    s32 unk3F4;
} UnkStruct_0234f320;

void Player_State_ParachuteRelease(void);
void Player_SetAnimation(UnkStruct_0234f320 *w, s32 a);
void func_ov000_02311eac(UnkStruct_0234f320 *w);
void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);

void Player_Action_ParachuteRelease(UnkStruct_0234f320 *w) {
    Player_SetAnimation(w, 0x13);
    w->unk5C |= 4;
    w->unk68 = Player_State_ParachuteRelease;
    w->unk58 |= 0x80;
    w->unk30 = -0x780;
    w->unk30 += w->unk3F0 * -0x180;
    w->unk3F0 = 0;
    w->unk3F4 = 0;
    w->unk32C &= ~0xF;
    w->unk8 = 0;
    w->unkC = 0;
    Sound_PlaySeqArcEx(w->unk384, 0, 0x42);
    func_ov000_02311eac(w);
}
