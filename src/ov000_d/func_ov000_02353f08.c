#include <nitro/types.h>

typedef struct UnkStruct_02353f08 {
    u8 unk0;
    u8 unk1;
    u8 pad2[0x2E - 2];
    s16 unk2E;
    s16 unk30;
    u8 pad32[2];
    s16 unk34;
    u8 pad36[0x58 - 0x36];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x32C - 0x5E];
    u32 unk32C;
    u8 pad330[0x334 - 0x330];
    void (*unk334)(struct UnkStruct_02353f08 *w);
    u8 pad338[0x388 - 0x338];
    u8 unk388[4];
} UnkStruct_02353f08;

typedef struct {
    u8 pad0[0x20];
    u32 unk20;
} UnkStruct_02353f08_4560;

extern UnkStruct_02353f08_4560 gGameState;
extern s16 data_0206e470[];

s16 ObjSpd_UpSet(s16 v, s32 a, s32 b);
void func_ov000_02328e6c(UnkStruct_02353f08 *w, s32 a, s32 b);
void NNS_SndPlayerStopSeq(void *p, s32 a);
void NNS_SndHandleReleaseSeq(void *p);
void Player_LoadPhysicsParams(UnkStruct_02353f08 *w);
void Player_Action_LandOnGround(UnkStruct_02353f08 *w, u8 a);

static inline s16 UnkCos_02353f08(u16 a) {
    s32 i = (s32)(a & 0xffff) >> 4;
    return data_0206e470[(u32)((i << 1) + 1)];
}

static inline s16 UnkSin_02353f08(u16 a) {
    s32 i = (s32)(a & 0xffff) >> 4;
    return data_0206e470[(u32)(i << 1)];
}

#define ABS_02353f08(x) ((x) < 0 ? -(x) : (x))

void func_ov000_02353f08(UnkStruct_02353f08 *w) {
    if (w->unk1 > 0x80) {
        w->unk5C |= 1;
    } else {
        w->unk5C &= ~1;
    }
    if (w->unk1 == 0 && ABS_02353f08(w->unk34) < 0x100) {
        w->unk34 = ObjSpd_UpSet(w->unk34, 0x10, 0x100);
    }
    if ((w->unk58 & 1) && !(gGameState.unk20 & 7)) {
        func_ov000_02328e6c(w, 0, 0xc);
    }
    if (w->unk58 & 0x10) {
        if (!(w->unk58 & 1)) {
            return;
        }
        NNS_SndPlayerStopSeq(w->unk388, 0x10);
        NNS_SndHandleReleaseSeq(w->unk388);
        Player_LoadPhysicsParams(w);
        w->unk58 |= 0x40000;
        w->unk32C &= ~0x100000;
        w->unk2E = w->unk34;
        Player_Action_LandOnGround(w, 0);
        w->unk334(w);
    } else {
        if (w->unk58 & 1) {
            return;
        }
        w->unk58 |= 0x10;
        w->unk58 |= 0x8000;
        w->unk2E = (w->unk34 * UnkCos_02353f08(w->unk1 << 8)) >> 12;
        w->unk30 = (w->unk34 * UnkSin_02353f08(w->unk1 << 8)) >> 12;
    }
}
