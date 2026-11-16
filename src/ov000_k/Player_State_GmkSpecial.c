#include <nitro/types.h>

typedef struct UnkStruct_023504e8 UnkStruct_023504e8;

typedef struct {
    u8 pad0[0x10];
    s32 x;
    s32 y;
} UnkStruct_023504e8_P;

struct UnkStruct_023504e8 {
    u8 pad0;
    u8 unk1;
    u8 pad2[5 - 2];
    u8 unk5;
    u8 pad6[0xC - 6];
    s32 unkC;
    s32 x;
    s32 y;
    u8 pad18[0x1C - 0x18];
    s32 unk1C;
    s32 unk20;
    u8 pad24[0x28 - 0x24];
    s16 unk28;
    s16 unk2A;
    u8 pad2C[0x34 - 0x2C];
    s16 unk34;
    u8 pad36[0x54 - 0x36];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x32C - 0x5E];
    u32 unk32C;
    u32 unk330;
    void (*unk334)(UnkStruct_023504e8 *w);
    u8 pad338[0x340 - 0x338];
    void (*unk340)(UnkStruct_023504e8 *w);
    u8 pad344[0x366 - 0x344];
    s16 unk366;
    s16 unk368;
    u8 pad36A[0x388 - 0x36A];
    u8 unk388[0x3E8 - 0x388];
    UnkStruct_023504e8_P *unk3E8;
    u8 pad3EC[0x3F0 - 0x3EC];
    s32 unk3F0;
    s32 unk3F4;
    s32 unk3F8;
    u8 pad3FC[0x422 - 0x3FC];
    u16 unk422;
};

typedef struct {
    u8 pad0[0x20];
    u32 unk20;
} UnkStruct_023504e8_4560;

extern UnkStruct_023504e8_4560 gGameState;
extern u32 gRandSeed;
extern s16 data_0206e470[];

#define SIN_023504e8(a) data_0206e470[(((a) & 0xffff) >> 4) * 2]
#define COS_023504e8(a) data_0206e470[(((a) & 0xffff) >> 4) * 2 + 1]

void NNS_SndPlayerStopSeq(void *p, s32 a);
void NNS_SndHandleReleaseSeq(void *p);
void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);
void EffSpecial_Create(UnkStruct_023504e8 *w);
void EffTrick_Unk_ov000_02330098(UnkStruct_023504e8 *w, s16 a, s16 b);
void Player_Action_GmkSpecialEnter(UnkStruct_023504e8 *w);
s16 ObjSpd_UpSet(s32 v, s32 a, s32 max);
s16 ObjSpd_DownSet(s32 v, s32 a);
void Player_Action_LandOnGround(UnkStruct_023504e8 *w, s32 a);

static inline u16 Rand_023504e8(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

void Player_State_GmkSpecial(UnkStruct_023504e8 *w) {
    s32 a;
    s32 r;

    if (w->unk3E8 != NULL) {
        w->unk5C &= ~1;
        if (w->unk32C & 0x80) {
            w->unk3F4++;
            if (w->unkC == 0) {
                NNS_SndPlayerStopSeq(w->unk388, 0x10);
                NNS_SndHandleReleaseSeq(w->unk388);
                Sound_PlaySeqArcEx(w->unk388, 0, 0x47);
                w->unkC++;
                EffSpecial_Create(w);
            }
            if (w->unk3F4 > 0x60) {
                if (!(gGameState.unk20 & 3)) {
                    EffTrick_Unk_ov000_02330098(w, 0xf - (Rand_023504e8() & 0x1e), 0xf - (Rand_023504e8() & 0x1e));
                }
                if (w->unkC == 1) {
                    NNS_SndPlayerStopSeq(w->unk388, 0x10);
                    NNS_SndHandleReleaseSeq(w->unk388);
                    Sound_PlaySeqArcEx(w->unk388, 0, 0x48);
                    w->unkC++;
                }
            }
            if (w->unk3F4 > 0xc0) {
                if (w->unkC == 2) {
                    NNS_SndPlayerStopSeq(w->unk388, 0x10);
                    NNS_SndHandleReleaseSeq(w->unk388);
                    Sound_PlaySeqArcEx(w->unk388, 0, 0x49);
                    w->unkC++;
                }
                Player_Action_GmkSpecialEnter(w);
                return;
            }
            if (w->unk34 > 0) {
                w->unk34 = ObjSpd_UpSet(w->unk34, w->unk366, w->unk368 << 1);
            } else {
                w->unk34 = ObjSpd_UpSet(w->unk34, -w->unk366, w->unk368 << 1);
            }
        } else {
            w->unkC = 0;
            w->unk3F4 = 0;
            if (w->unk3F8 < 0x4000) {
                w->unk3F8 += 0x10000;
            }
            if (w->unk3F8 > 0x14000) {
                w->unk3F8 -= 0x10000;
            }
            if (w->unk3F8 < 0xc000) {
                if (w->unk34 > 0) {
                    w->unk34 = ObjSpd_DownSet(w->unk34, 0x20);
                }
                w->unk34 = ObjSpd_UpSet(w->unk34, -0x20, 0xf00);
            } else {
                if (w->unk34 < 0) {
                    w->unk34 = ObjSpd_DownSet(w->unk34, 0x20);
                }
                w->unk34 = ObjSpd_UpSet(w->unk34, 0x20, 0xf00);
            }
            a = 0xc000 - w->unk3F8;
            if (a < 0) {
                a = -a;
            }
            if (a < 0x100) {
                a = w->unk34;
                if (a < 0) {
                    a = -a;
                }
                if (a < 0x80) {
                    w->unk3F8 = 0xc000;
                    w->unk34 = 0;
                }
            }
        }
        w->unk3F8 -= w->unk34;
        w->unk1 = w->unk3F8 >> 8;
        w->unk1C = w->x;
        w->unk20 = w->y;
        a = (u16)((w->unk1 + w->unk5) << 8);
        r = w->unk3F0;
        w->x = w->unk3E8->x + (s16)((r * data_0206e470[(u32)((((a & 0xffff) >> 4) << 1) + 1)]) >> 12);
        w->y = w->unk3E8->y + (s16)((r * data_0206e470[(u32)(((a & 0xffff) >> 4) << 1)]) >> 12);
        w->unk28 = w->x - w->unk1C;
        w->unk2A = w->y - w->unk20;
    }
    if (w->unk3E8 != NULL && !(w->unk422 & 3)) {
        return;
    }
    if (w->unk340 == NULL) {
        return;
    }
    if (w->unk334 == NULL) {
        return;
    }
    NNS_SndPlayerStopSeq(w->unk388, 0x10);
    NNS_SndHandleReleaseSeq(w->unk388);
    w->unk34 = 0;
    w->unk3E8 = NULL;
    w->unk58 |= 0xc0;
    w->unk58 &= ~0x2000;
    w->unk58 &= ~0x4000;
    w->unk58 &= ~0x100;
    w->unk54 &= ~2;
    w->unk32C &= ~0x2000;
    w->unk330 &= ~8;
    w->unk1 = 0;
    if (w->unk422 & 3) {
        w->unk340(w);
        return;
    }
    Player_Action_LandOnGround(w, 0);
    w->unk334(w);
}
