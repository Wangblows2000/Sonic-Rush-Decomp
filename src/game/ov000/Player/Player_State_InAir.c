#include <nitro/types.h>

typedef struct UnkStruct_0234a288 UnkStruct_0234a288;

struct UnkStruct_0234a288 {
    u8 pad0[2];
    u8 unk2;
    u8 pad3[8 - 3];
    u32 unk8;
    s32 unkC;
    u8 pad10[0x2E - 0x10];
    s16 unk2E;
    s16 unk30;
    s16 unk32;
    u8 pad34[0x58 - 0x34];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x62 - 0x5E];
    s16 unk62;
    u8 pad64[0x324 - 0x64];
    u8 unk324;
    u8 pad325[0x328 - 0x325];
    s16 unk328;
    s16 unk32A;
    u32 unk32C;
    u32 unk330;
    void (*unk334)(UnkStruct_0234a288 *w);
    u8 pad338[0x344 - 0x338];
    void (*unk344)(UnkStruct_0234a288 *w);
    u8 pad348[0x3C4 - 0x348];
    s16 unk3C4;
    s16 unk3C6;
    u8 pad3C8[0x3E2 - 0x3C8];
    s16 unk3E2;
    u8 pad3E4[0x420 - 0x3E4];
    u16 unk420;
    u16 unk422;
};

typedef struct {
    u8 pad0[0x18];
    u32 unk18;
    u8 pad1C[0x20 - 0x1C];
    u32 unk20;
} UnkStruct_0234a288_4560;

extern UnkStruct_0234a288_4560 gGameState;
extern u32 gObjManagerFlags;
extern u32 gRandSeed;

void Player_SettleAirRotation(UnkStruct_0234a288 *w);
void Player_ApplyAirInput(UnkStruct_0234a288 *w);
void Player_SetAnimation(UnkStruct_0234a288 *w, s32 state);
void EffBlazeSpinFlame_Spawn(UnkStruct_0234a288 *w);
void EffBlazeTrickFlame_Create(UnkStruct_0234a288 *w, s32 a);
void EffTrick_CreateAtOffset(UnkStruct_0234a288 *w, s16 a, s16 b);
void Player_Action_LandOnGround(UnkStruct_0234a288 *w, s32 a);
void Player_Action_TrickUp(UnkStruct_0234a288 *w);
void Player_Action_TrickForward(UnkStruct_0234a288 *w);
void Player_Action_TrickCombo(UnkStruct_0234a288 *w);

static inline u16 Rand_0234a288(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

void Player_State_InAir(UnkStruct_0234a288 *w) {
    s16 v = w->unk30;

    if (w->unk330 & 0x1000) {
        v = w->unk32;
        if (w->unk2 > 0x80) {
            v = -v;
        }
    }
    Player_SettleAirRotation(w);
    Player_ApplyAirInput(w);
    if (w->unkC != 0) {
        w->unkC--;
        if (w->unkC == 0) {
            w->unk58 |= 0x80;
        }
    }
    if (!(w->unk32C & 1) && !(w->unk420 & 3) && v < -0x40) {
        if (w->unk330 & 0x1000) {
            w->unk32 = 0;
        } else {
            w->unk30 = -0x40;
        }
    }
    if (v > 0) {
        w->unk32C |= 1;
    }
    switch (w->unk328) {
    case 0x30:
        if (!(gObjManagerFlags & 0x100) && w->unk324 == 1 && (gGameState.unk18 & 1) && !(gGameState.unk20 & 7)) {
            EffBlazeSpinFlame_Spawn(w);
        }
        break;
    case 1:
        if (w->unk5C & 8) {
            Player_SetAnimation(w, 2);
            w->unk5C |= 4;
        }
        break;
    case 0x13:
        if (v > 0x40) {
            Player_SetAnimation(w, 0x14);
        }
        break;
    case 0x14:
        if (w->unk5C & 8) {
            Player_SetAnimation(w, 0x15);
            w->unk5C |= 4;
        }
        break;
    case 0x16:
        if (w->unk5C & 8) {
            w->unk5C |= 0x400;
            Player_SetAnimation(w, 0x17);
            w->unk5C |= 4;
        }
        break;
    case 0x18:
        if (v > 0x1c0) {
            w->unk5C |= 0x400;
            Player_SetAnimation(w, 0x19);
            w->unk5C |= 4;
        }
        break;
    case 0x35:
        if (w->unk324 == 1) {
            if (!(w->unk3E2 & 3)) {
                EffBlazeTrickFlame_Create(w, w->unk3E2);
            }
            w->unk3E2++;
        }
        if (w->unk5C & 8) {
            Player_SetAnimation(w, 0x36);
        }
        break;
    case 0x36:
        if (w->unk324 == 1) {
            if (v < -0x200 && !(w->unk3E2 & 3)) {
                EffBlazeTrickFlame_Create(w, w->unk3E2);
            }
            w->unk3E2++;
        }
        if (v > 0x40 && (w->unk5C & 8)) {
            w->unk5C |= 0x400;
            Player_SetAnimation(w, 2);
            w->unk5C |= 4;
        }
        break;
    case 0x38:
        if (!(gGameState.unk20 & 7)) {
            EffTrick_CreateAtOffset(w, 0xf - (Rand_0234a288() & 0x1e), 0xf - (Rand_0234a288() & 0x1e));
        }
        if (w->unk5C & 8) {
            Player_SetAnimation(w, 2);
            w->unk5C |= 4;
        }
        break;
    case 0x37:
        if (w->unk324 == 1 && w->unk3C4 != 0) {
            w->unk5C ^= 0x20;
            w->unk30 = 0;
            w->unk3C4--;
            if (w->unk3C4 == 0) {
                w->unk5C &= ~0x20;
                w->unk2E = w->unk3C6;
                w->unk62 = 0x44;
            }
        }
        break;
    case 0x39:
        break;
    case 0x3a:
        break;
    case 0x3b:
        break;
    }
    if (w->unk58 & 1) {
        Player_Action_LandOnGround(w, 0);
        w->unk334(w);
        return;
    }
    if (w->unk32C & 2) {
        if (!(w->unk32C & 8) && (w->unk328 < 0x35 || w->unk328 > 0x37)) {
            if ((w->unk422 & 0x100) && (w->unk420 & 0x40)) {
                Player_Action_TrickUp(w);
                return;
            }
            if (w->unk422 & 0x100) {
                Player_Action_TrickForward(w);
                return;
            }
        }
        if (w->unk422 & 3) {
            w->unk8 = w->unk422;
        }
        if (w->unk328 >= 0x37 && w->unk328 <= 0x3b) {
            if (w->unk328 == 0x37 && w->unk324 == 1 && v < 0) {
                w->unk32A = 0;
            }
            w->unk32A++;
            if (w->unk32A > 0xff) {
                w->unk32A = 0xff;
            }
        }
        if (w->unk32C & 4) {
            return;
        }
        if (w->unk328 >= 0x37 && w->unk32A <= 0x18) {
            return;
        }
        if (w->unk8 == 0) {
            return;
        }
        Player_Action_TrickCombo(w);
    } else {
        if (w->unk328 >= 0x18 && w->unk328 != 0x30) {
            return;
        }
        if (!(w->unk422 & 0x100)) {
            return;
        }
        if (w->unk344 == NULL) {
            return;
        }
        if (!(w->unk330 & 0x1000)) {
            w->unk58 |= 0x80;
        }
        w->unk344(w);
    }
}
