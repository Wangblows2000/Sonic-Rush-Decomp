#include <nitro/types.h>

typedef struct UnkStruct_02350968 {
    u8 unk0;
    u8 unk1;
    u8 pad2[3];
    u8 unk5;
    u8 pad6[2];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u8 pad18[0x2E - 0x18];
    s16 unk2E;
    s16 unk30;
    u8 pad32[2];
    s16 unk34;
    u8 pad36[0x54 - 0x36];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x68 - 0x5E];
    void (*unk68)(void);
    u8 pad6C[0x32C - 0x6C];
    u32 unk32C;
    u32 unk330;
    u8 pad334[0x384 - 0x334];
    u8 unk384[0x3E8 - 0x384];
    struct UnkStruct_02350968 *unk3E8;
    u8 pad3EC[2];
    s16 unk3EE;
    s32 unk3F0;
    s32 unk3F4;
    s32 unk3F8;
} UnkStruct_02350968;

void Player_State_GmkSpecial(void);
void Player_ResetGimmickState(UnkStruct_02350968 *w);
void func_ov000_0234c644(UnkStruct_02350968 *w);
void Player_SetAnimation(UnkStruct_02350968 *w, s32 a);
u16 FX_Atan2(s32 y, s32 x);
void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);

void Player_Action_GmkSpecial(UnkStruct_02350968 *w, UnkStruct_02350968 *o) {
    Player_ResetGimmickState(w);
    func_ov000_0234c644(w);
    w->unk3E8 = o;
    Player_SetAnimation(w, 0x25);
    w->unk5C |= 4;
    w->unk68 = Player_State_GmkSpecial;
    w->unk5C &= ~1;
    w->unk58 &= ~0x10;
    w->unk58 |= 0x8000;
    w->unk58 |= 0x2000;
    w->unk58 |= 0x100;
    w->unk58 &= ~0x80;
    w->unk58 &= ~0x40;
    w->unk54 |= 2;
    w->unk32C |= 0x2000;
    w->unk330 |= 0x30;
    w->unk330 |= 8;
    w->unk34 = w->unk2E + w->unk30;
    w->unk2E = 0;
    w->unk30 = 0;
    w->unk3F8 = FX_Atan2(o->unk14 - w->unk14, o->unk10 - w->unk10);
    w->unk3F8 += w->unk5 << 8;
    if (w->unk3F8 < 0x4000) {
        w->unk3F8 += 0x10000;
    }
    w->unk1 = w->unk3F8 >> 8;
    w->unk8 = 0;
    w->unkC = 0;
    w->unk3F0 = -0x2000;
    w->unk3F4 = 0;
    w->unk3EE = 0;
    Sound_PlaySeqArcEx(w->unk384, 0, 0x6d);
}
