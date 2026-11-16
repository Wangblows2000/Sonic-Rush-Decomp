#ifndef OV000_D_PLAYER_H
#define OV000_D_PLAYER_H

#include <nitro/types.h>

typedef struct {
    u8 pad0[0xA];
    u8 unkA;
} UnkStruct_PlayerHit;

/* ov000 player object (only fields used by ov000_d functions) */
typedef struct {
    u8 pad0[8];
    u32 unk8;
    s32 unkC;
    u8 pad10[0x2E - 0x10];
    s16 unk2E;
    s16 unk30;
    u8 pad32[2];
    s16 unk34;
    u8 pad36[0x58 - 0x36];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x62 - 0x5E];
    u16 unk62;
    u8 pad64[0x68 - 0x64];
    void (*unk68)(void);
    u8 pad6C[0x2C4 - 0x6C];
    u32 unk2C4;
    u8 pad2C8[0x324 - 0x2C8];
    u8 unk324;
    u8 pad325[3];
    s16 unk328;
    s16 unk32A;
    u32 unk32C;
    u32 unk330;
    u8 pad334[0x350 - 0x334];
    s16 unk350;
    u8 pad352[0x376 - 0x352];
    s16 unk376;
    u8 pad378[0x37C - 0x378];
    s16 unk37C;
    u8 pad37E[0x384 - 0x37E];
    u8 unk384[0x390 - 0x384];
    u8 unk390[0x3AE - 0x390];
    s16 unk3AE;
    u8 pad3B0[2];
    s16 unk3B2;
    u8 pad3B4[0x3C4 - 0x3B4];
    s16 unk3C4;
    u8 pad3C6[0x3CF - 0x3C6];
    u8 unk3CF;
    u8 unk3D0;
    u8 pad3D1[0x3E4 - 0x3D1];
    UnkStruct_PlayerHit *unk3E4;
} UnkStruct_Player;

extern u32 gRandSeed;

static inline u16 Player_Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

void Player_SetAnimation(UnkStruct_Player *w, s32 a);
void func_ov000_0234307c(UnkStruct_Player *w, s16 a);
void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);
void NNS_SndPlayerStopSeq(void *p, s32 a);
void NNS_SndPlayerSetVolume(void *p, s32 vol);
void func_ov000_02345830(UnkStruct_Player *w, s32 a);
void func_ov000_0232c7b0(u8 a);

#endif
