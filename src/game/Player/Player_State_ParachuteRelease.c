#include <nitro/types.h>

typedef struct UnkStruct_0234f054 UnkStruct_0234f054;

struct UnkStruct_0234f054 {
    u8 pad0;
    u8 unk1;
    u8 pad2[8 - 2];
    u32 unk8;
    s32 unkC;
    u8 pad10[0x30 - 0x10];
    s16 unk30;
    u8 pad32[0x36 - 0x32];
    s16 unk36;
    u8 pad38[0x40 - 0x38];
    s16 unk40;
    s16 unk42;
    u8 pad44[0x58 - 0x44];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x62 - 0x5E];
    s16 unk62;
    u8 pad64[0x328 - 0x64];
    s16 unk328;
    s16 unk32A;
    u32 unk32C;
    u8 pad330[0x334 - 0x330];
    void (*unk334)(UnkStruct_0234f054 *w);
    u8 pad338[0x390 - 0x338];
    u8 unk390[0x3E8 - 0x390];
    void *unk3E8;
    u8 pad3EC[0x3F0 - 0x3EC];
    s32 unk3F0;
    s32 unk3F4;
    u8 pad3F8[0x422 - 0x3F8];
    u16 unk422;
};

void Player_SetAnimation(UnkStruct_0234f054 *w, s32 state);
void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);
s32 ObjSpd_DownSet(s32 v, s32 a);
s32 ObjSpd_UpSet(s32 v, s32 a, s32 max);
void Player_ApplyAirInput(UnkStruct_0234f054 *w);
void Player_Action_TrickCombo(UnkStruct_0234f054 *w);
void Player_Action_LandOnGround(UnkStruct_0234f054 *w, u8 a);
void NNS_SndPlayerStopSeq(void *p, s32 a);
void NNS_SndHandleReleaseSeq(void *p);

void Player_State_ParachuteRelease(UnkStruct_0234f054 *w) {
    s32 lim = 0x400;

    switch (w->unk328) {
    case 0x13:
    default:
        if (w->unk422 & 3) {
            w->unk8 = w->unk422;
        }
        if (w->unk328 >= 0x39 && w->unk328 <= 0x3b) {
            w->unk32A++;
            if (w->unk32A > 0xff) {
                w->unk32A = 0xff;
            }
        }
        if (w->unk30 > -0x400) {
            if (w->unk328 != 0x13) {
                w->unk5C |= 0x400;
            }
            w->unk8 = 0;
            w->unk32A = 0;
            Player_SetAnimation(w, 0x14);
            NNS_SndPlayerStopSeq(w->unk390, 0x20);
            NNS_SndHandleReleaseSeq(w->unk390);
            return;
        }
        if (!(w->unk32C & 4) && (w->unk328 < 0x39 || w->unk32A > 0x18) && w->unk8 != 0) {
            Player_Action_TrickCombo(w);
        }
        break;
    case 0x14:
        if (w->unk5C & 8) {
            Player_SetAnimation(w, 0x15);
            w->unk5C |= 4;
            w->unk62 = 100;
            Sound_PlaySeqArcEx(NULL, 0, 0x90);
            w->unk40 = 8;
            w->unk42 = 0x1e0;
            w->unk30 = 0;
        }
        break;
    case 0x15:
        if (w->unk422 & 3) {
            w->unk8 = 0x10;
            w->unk62 = 0x20;
        }
        if (w->unk8 != 0) {
            w->unk8--;
            w->unk3F0 = ObjSpd_UpSet(w->unk3F0, 8, 0x80);
            lim <<= 2;
        } else {
            w->unk3F0 = ObjSpd_DownSet(w->unk3F0, 0x10);
        }
        w->unk40 = ((0x100 - w->unk3F0) << 3) >> 8;
        w->unk42 = ((0x100 - w->unk3F0) * 0x1e0) >> 8;
        if (w->unk32C & 1) {
            w->unk3F4 = ObjSpd_UpSet(w->unk3F4, -0x12, 0xa0);
        } else {
            w->unk3F4 = ObjSpd_UpSet(w->unk3F4, 0x12, 0xa0);
        }
        w->unkC += w->unk3F4;
        if (w->unkC > lim) {
            w->unk32C |= 1;
        }
        if (w->unkC < -lim) {
            w->unk32C &= ~1;
        }
        w->unk1 = w->unkC >> 8;
        w->unk36 -= w->unkC >> 4;
        Player_ApplyAirInput(w);
        break;
    }
    if (!(w->unk58 & 1)) {
        return;
    }
    w->unk3E8 = NULL;
    Player_Action_LandOnGround(w, 0);
    w->unk334(w);
}
