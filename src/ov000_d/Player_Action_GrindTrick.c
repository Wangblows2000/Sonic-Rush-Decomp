#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    s32 unk8;
    s32 unkC;
    u8 pad10[0x34 - 0x10];
    s16 unk34;
    u8 pad36[0x5C - 0x36];
    u16 unk5C;
    u8 pad5E[0x324 - 0x5E];
    u8 unk324;
    u8 pad325[3];
    s16 unk328;
    s16 unk32A;
    u32 unk32C;
    u8 pad330[0x350 - 0x330];
    s16 unk350;
    u8 pad352[0x384 - 0x352];
    u8 unk384[0x390 - 0x384];
    u8 unk390[0x3D0 - 0x390];
    u8 unk3D0;
} UnkStruct_0234801c;

extern u32 gRandSeed;

static inline u16 Rand_0234801c(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

void Player_SetAnimation(UnkStruct_0234801c *w, s32 a);
void Player_AddTension(UnkStruct_0234801c *w, s32 a);
void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);
void NNS_SndPlayerStopSeq(void *p, s32 a);
void Player_AddTrickScore(UnkStruct_0234801c *w, s32 a);
void func_ov000_0232c7b0(u8 a);

void Player_Action_GrindTrick(UnkStruct_0234801c *w) {
    s16 v;

    w->unk3D0 = 0;
    if (w->unk328 == 0x1c) {
        Player_SetAnimation(w, 0x1d);
        Player_AddTension(w, 0xa0);
        if (w->unk324 != 1) {
            if (Rand_0234801c() & 1) {
                Sound_PlaySeqArcEx(w->unk384, 0, 0x1e);
            } else {
                Sound_PlaySeqArcEx(w->unk384, 0, 0x25);
            }
            Sound_PlaySeqArcEx(w->unk384, 0, 0xc);
        } else {
            if (Rand_0234801c() & 1) {
                Sound_PlaySeqArcEx(w->unk384, 0, 0x27);
            } else {
                Sound_PlaySeqArcEx(w->unk384, 0, 0x2e);
            }
            Sound_PlaySeqArcEx(w->unk384, 0, 0x19);
        }
        Sound_PlaySeqArcEx(w->unk384, 0, 0x30);
    } else if (w->unk328 == 0x1d) {
        Player_SetAnimation(w, 0x1e);
        Player_AddTension(w, 0x140);
        if (w->unk324 != 1) {
            if (Rand_0234801c() & 1) {
                Sound_PlaySeqArcEx(w->unk384, 0, 0x1f);
            } else {
                Sound_PlaySeqArcEx(w->unk384, 0, 0x20);
            }
            Sound_PlaySeqArcEx(w->unk384, 0, 0xd);
        } else {
            if (Rand_0234801c() & 1) {
                Sound_PlaySeqArcEx(w->unk384, 0, 0x28);
            } else {
                Sound_PlaySeqArcEx(w->unk384, 0, 0x29);
            }
            Sound_PlaySeqArcEx(w->unk384, 0, 0x1a);
        }
        NNS_SndPlayerStopSeq(w->unk390, 0);
        Sound_PlaySeqArcEx(w->unk390, 0, 0x31);
        if (w->unk5C & 1) {
            w->unk34 -= 0x200;
        } else {
            w->unk34 += 0x200;
        }
        w->unk8 = 0;
        Player_AddTrickScore(w, 200);
    } else {
        Player_SetAnimation(w, 0x1c);
        Player_AddTension(w, 0xa0);
        if (w->unk324 != 1) {
            Sound_PlaySeqArcEx(w->unk384, 0, 0x1d);
            Sound_PlaySeqArcEx(w->unk384, 0, 0xc);
        } else {
            Sound_PlaySeqArcEx(w->unk384, 0, 0x26);
            Sound_PlaySeqArcEx(w->unk384, 0, 0x19);
        }
        Sound_PlaySeqArcEx(w->unk384, 0, 0x2f);
    }
    w->unk32C |= 0x8000;
    v = w->unk350;
    if (v < 0) {
        v = 0;
    } else if (v > 0x12c0) {
        v = 0x12c0;
    }
    w->unk350 = v;
    func_ov000_0232c7b0(w->unk3D0);
    Player_AddTrickScore(w, 100);
    w->unkC = 0;
    w->unk32A = 0;
}
