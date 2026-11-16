#include <nitro/types.h>

typedef struct UnkStruct_02348860 UnkStruct_02348860;
struct UnkStruct_02348860 {
    u8 unk0;
    u8 unk1;
    u8 pad2[8 - 2];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u8 pad18[0x2E - 0x18];
    s16 unk2E;
    s16 unk30;
    u8 pad32[0x34 - 0x32];
    s16 unk34;
    u8 pad36[0x58 - 0x36];
    u32 unk58;
    u16 unk5C;
    u16 unk5E;
    u16 unk60;
    u8 pad62[0x68 - 0x62];
    void (*state)(UnkStruct_02348860 *w);
    u8 pad6C[0x2CC - 0x6C];
    u8 unk2CC[0x324 - 0x2CC];
    u8 unk324;
    u8 pad325[0x32A - 0x325];
    s16 unk32A;
    u32 unk32C;
    u8 pad330[0x384 - 0x330];
    u8 pad384[0x388 - 0x384];
    u8 unk388[0x3CF - 0x388];
    u8 unk3CF;
    u8 pad3D0[0x3E8 - 0x3D0];
    UnkStruct_02348860 *other;
    u8 pad3EC[0x3F0 - 0x3EC];
    s32 unk3F0;
    s32 unk3F4;
    s32 unk3F8;
    u8 pad3FC[0x410 - 0x3FC];
    u8 unk410;
    u8 unk411;
};

void func_ov000_02348350(UnkStruct_02348860 *w);
void Player_ResetGimmickState(UnkStruct_02348860 *w);
void Player_SetAnimation(UnkStruct_02348860 *w, s32 a);
void func_ov000_0234a064(UnkStruct_02348860 *w, s32 a);
void EffGrind_Create(UnkStruct_02348860 *w, s32 a, s32 b);
void NNS_SndPlayerStopSeq(void *p, s32 a);
void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);

void func_ov000_02348860(UnkStruct_02348860 *w) {
    s32 a;
    s32 b;
    s16 v;

    w->unk410 &= ~0x80;
    Player_ResetGimmickState(w);
    Player_SetAnimation(w, 0x1A);
    w->unk5C |= 4;
    w->unk58 &= ~0x10;
    w->unk32C &= ~0xC000;
    w->unk410 |= 0x80;
    w->unk411 = 0;
    w->state = func_ov000_02348350;
    if (w->unk58 & 1) {
        a = w->unk34;
        b = w->unk2E;
        a = a < 0 ? -a : a;
        b = b < 0 ? -b : b;
        if (b < a)
            w->unk2E = w->unk34;
    }
    func_ov000_0234a064(w, 0);
    v = w->unk34;
    if (v > -0x200 && v < 0x200) {
        if (v < 0)
            w->unk34 = -0x200;
        else if (v > 0)
            w->unk34 = 0x200;
    }
    NNS_SndPlayerStopSeq(w->unk388, 0);
    Sound_PlaySeqArcEx(w->unk388, 0, 0x3E);
    w->unkC = 0;
    w->unk8 = 0;
    w->unk3CF = 0;
    EffGrind_Create(w, 0, 0x12);
}
