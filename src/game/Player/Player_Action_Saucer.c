#include <nitro/types.h>

typedef struct UnkStruct_0234f6a0 UnkStruct_0234f6a0;
struct UnkStruct_0234f6a0 {
    u8 unk0;
    u8 unk1;
    u8 pad2[8 - 2];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u8 pad18[0x2E - 0x18];
    s16 unk2E;
    s16 unk30;
    u8 pad32[0x34 - 0x32];
    s16 unk34;
    u8 pad36[0x58 - 0x36];
    u32 unk58;
    u16 unk5C;
    u16 unk5E;
    u16 unk60;
    u16 unk62;
    u8 pad64[0x68 - 0x64];
    void (*state)(UnkStruct_0234f6a0 *w);
    u8 pad6C[0x2CC - 0x6C];
    u8 unk2CC[0x324 - 0x2CC];
    u8 unk324;
    u8 pad325[0x32A - 0x325];
    s16 unk32A;
    u32 unk32C;
    u8 pad330[0x384 - 0x330];
    u8 unk384[0x3E8 - 0x384];
    UnkStruct_0234f6a0 *other;
    u8 pad3EC[0x3F0 - 0x3EC];
    s32 unk3F0;
    s32 unk3F4;
    s32 unk3F8;
};

void Player_State_Parachute(UnkStruct_0234f6a0 *w);
void Player_State_ParachuteRelease(UnkStruct_0234f6a0 *w);
void Player_State_Saucer(UnkStruct_0234f6a0 *w);
void Player_InitState(UnkStruct_0234f6a0 *w);
void Player_StopHighSpeed(UnkStruct_0234f6a0 *w);
void Player_StopBoost(UnkStruct_0234f6a0 *w);
void Player_SetAnimation(UnkStruct_0234f6a0 *w, s32 a);
void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);

void Player_Action_Saucer(UnkStruct_0234f6a0 *w, UnkStruct_0234f6a0 *other) {
    if (w->state == Player_State_Parachute)
        return;
    if (w->state == Player_State_ParachuteRelease)
        return;
    if (w->other == other)
        return;
    Player_InitState(w);
    Player_StopHighSpeed(w);
    Player_StopBoost(w);
    w->other = other;
    Player_SetAnimation(w, 0x3C);
    w->unk5C |= 4;
    w->state = Player_State_Saucer;
    w->unk62 = 0x40;
    w->unk32C &= ~0xF;
    w->unk32C |= 0x1000;
    w->unk32C |= 0x100000;
    w->unk5C &= ~1;
    w->unk34 = 0;
    w->unk2E = 0;
    w->unk30 = 0;
    w->unk1 = 0;
    w->unk10 = other->unk10;
    w->unk8 = 0;
    w->unkC = 0;
    w->unk3F0 = 0;
    Sound_PlaySeqArcEx(w->unk384, 0, 0x6D);
}
