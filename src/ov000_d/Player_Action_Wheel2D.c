#include <nitro/types.h>

typedef struct UnkStruct_02353408 {
    u8 unk0;
    u8 unk1;
    u8 pad2[6];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u8 pad18[0x28 - 0x18];
    s16 unk28;
    u8 pad2A[0x2E - 0x2A];
    s16 unk2E;
    s16 unk30;
    s16 unk32;
    s16 unk34;
    u8 pad36[0x58 - 0x36];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x68 - 0x5E];
    void (*unk68)(void);
    u8 pad6C[0x32C - 0x6C];
    u32 unk32C;
    u32 unk330;
    u8 pad334[0x37C - 0x334];
    s16 unk37C;
    u8 pad37E[0x384 - 0x37E];
    u8 unk384[0x3E8 - 0x384];
    struct UnkStruct_02353408 *unk3E8;
    u8 pad3EC[0x3F4 - 0x3EC];
    s32 unk3F4;
    s32 unk3F8;
    s32 unk3FC;
} UnkStruct_02353408;

void Player_State_Wheel2D(void);
void func_ov000_0234cb70(UnkStruct_02353408 *w);
void Player_SetAnimation(UnkStruct_02353408 *w, s32 a);
void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);

void Player_Action_Wheel2D(UnkStruct_02353408 *w, UnkStruct_02353408 *o) {
    if (w->unk68 == Player_State_Wheel2D) {
        return;
    }
    func_ov000_0234cb70(w);
    w->unk3E8 = o;
    Player_SetAnimation(w, 0x28);
    w->unk5C |= 4;
    w->unk68 = Player_State_Wheel2D;
    w->unk58 |= 0x10;
    w->unk58 |= 0x2000;
    w->unk58 |= 0x4000;
    w->unk5C &= ~1;
    w->unk5C &= ~0x200;
    w->unk330 |= 0x20000;
    w->unk32C |= 0x100000;
    w->unk2E = 0;
    w->unk30 = 0;
    w->unk32 = 0;
    w->unk1 = 0;
    w->unk32C &= ~3;
    w->unk34 = w->unk28 >> 4;
    if (w->unk34 < 0x40) {
        w->unk34 = 0x40;
    }
    if (w->unk28 < w->unk37C) {
        w->unk32C |= 1;
    }
    w->unk8 = 0;
    w->unk3F4 = 0;
    w->unkC = 0;
    w->unk3F8 = o->unk10 - w->unk10;
    w->unk3FC = o->unk14 - w->unk14;
    w->unk32C |= 0x2000;
    w->unk330 |= 0x10;
    Sound_PlaySeqArcEx(w->unk384, 0, 0x58);
}
