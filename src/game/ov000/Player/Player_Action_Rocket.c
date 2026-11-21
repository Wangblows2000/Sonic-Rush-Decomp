#include <nitro/types.h>

typedef struct UnkStruct_02351d28 {
    u8 unk0;
    u8 unk1;
    u8 pad2[3];
    u8 unk5;
    u8 pad6[2];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u8 pad18[4];
    s32 unk1C;
    s32 unk20;
    u8 pad24[0x2E - 0x24];
    s16 unk2E;
    s16 unk30;
    u8 pad32[2];
    s16 unk34;
    u8 pad36[0x58 - 0x36];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x68 - 0x5E];
    void (*unk68)(void);
    u8 pad6C[0x32C - 0x6C];
    u32 unk32C;
    u8 pad330[0x3D1 - 0x330];
    u8 unk3D1;
    u8 pad3D2[0x3E8 - 0x3D2];
    struct UnkStruct_02351d28 *unk3E8;
    u8 pad3EC[4];
    s32 unk3F0;
    s32 unk3F4;
    s32 unk3F8;
    s32 unk3FC;
} UnkStruct_02351d28;

void Player_State_Rocket(void);
void Player_ResetGimmickState(UnkStruct_02351d28 *w);
void Player_SetAnimation(UnkStruct_02351d28 *w, s32 a);

void Player_Action_Rocket(UnkStruct_02351d28 *w, UnkStruct_02351d28 *o, s32 x, s32 y) {
    if (w->unk68 == Player_State_Rocket) {
        return;
    }
    Player_ResetGimmickState(w);
    w->unk3E8 = o;
    Player_SetAnimation(w, 0x25);
    w->unk5C |= 4;
    w->unk68 = Player_State_Rocket;
    w->unk58 |= 0x10;
    w->unk58 |= 0x8000;
    w->unk58 |= 0x2000;
    w->unk58 &= ~0x80;
    w->unk5C &= ~1;
    w->unk5C |= o->unk5C & 1;
    w->unk5C ^= 1;
    w->unk32C |= 0x100000;
    w->unk2E = 0;
    w->unk30 = 0;
    w->unk34 = 0;
    w->unk1 = 0;
    w->unk8 = 0;
    w->unkC = 0;
    w->unk3F0 = x << 8;
    w->unk3F4 = y << 8;
    w->unk3F8 = 0;
    w->unk3FC = 0;
    if (w->unk5 != 0) {
        w->unk3F0 = -w->unk3F0;
    }
    w->unk1C = w->unk10;
    w->unk20 = w->unk14;
    w->unk10 = w->unk3E8->unk10 + w->unk3F0;
    w->unk14 = w->unk3E8->unk14 + w->unk3F4;
    w->unk3D1 = 8;
}
