#include <nitro/types.h>

typedef struct UnkStruct_0234ee38 UnkStruct_0234ee38;
struct UnkStruct_0234ee38 {
    u8 unk0;
    u8 unk1;
    u8 pad2[0xc - 2];
    s32 unkC;
    u8 pad10[0x30 - 0x10];
    s16 unk30;
    u8 pad32[0x58 - 0x32];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x32c - 0x5e];
    u32 unk32C;
    u8 pad330[0x334 - 0x330];
    void (*unk334)(UnkStruct_0234ee38 *w);
    u8 pad338[0x340 - 0x338];
    s32 unk340;
    u8 pad344[0x3e8 - 0x344];
    s32 unk3E8;
};

s16 ObjSpd_UpSet(s32 v, s32 a, s32 b);
void Player_Action_LandOnGround(UnkStruct_0234ee38 *w, s32 a);

void Player_State_Glider(UnkStruct_0234ee38 *w) {
    w->unkC++;
    if (w->unkC & 0x10)
        w->unk30 = ObjSpd_UpSet(w->unk30, -0x10, 0x80);
    else
        w->unk30 = ObjSpd_UpSet(w->unk30, 0x10, 0x80);
    if ((!(w->unk58 & 1) || w->unkC <= 0xc) && w->unkC <= 0x168)
        return;
    if (w->unk340 == 0)
        return;
    if (w->unk334 == NULL)
        return;
    w->unk3E8 = 0;
    w->unk5C |= 0x200;
    w->unk58 |= 0xc0;
    w->unk58 &= ~0x2000;
    w->unk58 &= ~0x4000;
    w->unk32C &= ~0x100000;
    w->unk1 = 0;
    Player_Action_LandOnGround(w, 0);
    w->unk334(w);
}
