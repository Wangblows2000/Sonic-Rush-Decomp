#include <nitro/types.h>

typedef struct UnkStruct_02355414 UnkStruct_02355414;

struct UnkStruct_02355414 {
    u8 pad0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 pad4[0x8 - 4];
    u32 unk8;
    u8 padC[0x2E - 0xC];
    s16 unk2E;
    u8 pad30[0x34 - 0x30];
    s16 unk34;
    u8 pad36[0x54 - 0x36];
    u32 unk54;
    u32 unk58;
    u8 pad5C[0x328 - 0x5C];
    s16 unk328;
    s16 unk32A;
    u8 pad32C[0x334 - 0x32C];
    void (*unk334)(UnkStruct_02355414 *w);
    u8 pad338[0x340 - 0x338];
    void (*unk340)(UnkStruct_02355414 *w);
    u8 pad344[0x37A - 0x344];
    s16 unk37A;
    u8 pad37C[0x3F8 - 0x37C];
    s32 unk3F8;
    u8 pad3FC[0x422 - 0x3FC];
    u16 unk422;
};

#define ABS(x) ((x) < 0 ? -(x) : (x))

BOOL Player_CheckFallOffGround(UnkStruct_02355414 *w);
void Player_Action_Fall(UnkStruct_02355414 *w);
void Player_Action_LandOnGround(UnkStruct_02355414 *w, u8 ang);
void Player_Action_Grind(UnkStruct_02355414 *w);
void Player_SetAnimSpeedFromVelocity(UnkStruct_02355414 *w, s16 spd);
void Player_UpdateRunAnimation(UnkStruct_02355414 *w);
void Player_ApplyGroundInput(UnkStruct_02355414 *w);
void Player_MoveAlongCorkscrew(UnkStruct_02355414 *w, s32 a, s32 b, s32 c);
void func_ov000_02354de4(UnkStruct_02355414 *w, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

void Player_State_CorkscrewPath(UnkStruct_02355414 *w) {
    if (w->unk3F8 != 2 && ABS(w->unk34) < w->unk37A && Player_CheckFallOffGround(w)) {
        w->unk1 = 0;
        w->unk2 = 0;
        w->unk3 = 0;
        w->unk58 &= ~0x2000;
        Player_Action_Fall(w);
        return;
    }
    if ((w->unk422 & 3) && w->unk340 != NULL) {
        if (w->unk3F8 == 2) {
            w->unk54 |= 1;
        }
        w->unk1 = 0;
        w->unk2 = 0;
        w->unk3 = 0;
        w->unk58 &= ~0x2000;
        w->unk340(w);
        return;
    }
    if (w->unk32A != 0) {
        w->unk32A--;
    } else {
        if (w->unk8 & 2) {
            w->unk58 &= ~0x10;
        }
        if (w->unk58 & 0xd) {
            w->unk1 = 0;
            w->unk2 = 0;
            w->unk3 = 0;
            w->unk58 &= ~0x2000;
            w->unk2E = w->unk34;
            Player_Action_LandOnGround(w, 0);
            if (w->unk3F8 == 2) {
                Player_Action_Grind(w);
                return;
            }
            w->unk334(w);
            return;
        }
    }
    if (w->unk3F8 != 2) {
        Player_SetAnimSpeedFromVelocity(w, w->unk34);
        if (w->unk328 != 0x31) {
            Player_UpdateRunAnimation(w);
        }
    }
    if (w->unk3F8 != 2) {
        Player_ApplyGroundInput(w);
    }
    switch (w->unk3F8) {
    case 0:
    default:
        Player_MoveAlongCorkscrew(w, 0x1759d, 0x120, 0x26);
        break;
    case 1:
        func_ov000_02354de4(w, 0x21e52, 0x54, 0x80, 0x83f0, 0x80, 0x20);
        break;
    case 2:
        Player_MoveAlongCorkscrew(w, 0x13d9d, 0x100, 0x1e);
        break;
    }
}
