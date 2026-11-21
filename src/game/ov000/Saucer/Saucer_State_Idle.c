#include <ov000_b/stageobj.h>

typedef struct {
    u8 unk0;
    u8 pad1[0x80 - 1];
    void *unk80;
} UnkStruct_02315928_R;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0x2E - 2];
    s16 unk2E;
    s16 unk30;
    u8 pad32[0x58 - 0x32];
    u32 unk58;
    u8 pad5C[0x1BC - 0x5C];
    UnkStruct_02315928_R *unk1BC;
    u8 pad1C0[0x1DC - 0x1C0];
    void *unk1DC;
    UnkStruct_ObjParam *unk1E0;
    u8 pad1E4[0x204 - 0x1E4];
    UnkStruct_02315928_R *unk204;
} UnkStruct_02315928;

typedef struct {
    u8 pad0[0x20];
    u32 unk20;
} UnkStruct_02315928_Sys;

extern UnkStruct_02315928_Sys gGameState;
extern void *gPlayers;

void NNS_SndPlayerStopSeq(void *p, s32 a);
void Saucer_Explode(UnkStruct_02315928 *w);
s16 ObjSpd_DownSet(s16 a, s32 b);
s16 ObjSpd_UpSet(s16 a, s32 b, s32 c);
u8 ObjAngle_MoveToward8(u8 a, s32 b, s32 c);
void Player_Action_Saucer(void *a, void *b);
void Saucer_Action_Ridden(UnkStruct_02315928 *w);
void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);
void Sound_SetPositional(void *a, void *b, void *c);

void Saucer_State_Idle(UnkStruct_02315928 *w) {
    UnkStruct_02315928_R *r;
    s16 acc = -8;
    s16 max = 0x1c0;

    if (w->unk58 & 0xf) {
        NNS_SndPlayerStopSeq(w->unk1DC, 0);
        Saucer_Explode(w);
        return;
    }
    if (w->unk1E0->unk4 & 1) {
        acc <<= 1;
        max <<= 1;
    }
    w->unk2E = ObjSpd_DownSet(w->unk2E, 0x10);
    w->unk30 = ObjSpd_UpSet(w->unk30, acc, max);
    w->unk1 = ObjAngle_MoveToward8(w->unk1, 0, 1);
    r = w->unk1BC;
    if (r != NULL && r->unk80 == w && r->unk0 == 1) {
        w->unk204 = r;
        Player_Action_Saucer(r, w);
        Saucer_Action_Ridden(w);
        return;
    }
    if (w->unk58 & 0x2000)
        return;
    if (!(gGameState.unk20 & 7))
        Sound_PlaySeqArcEx(w->unk1DC, 0, 0x6c);
    Sound_SetPositional(w->unk1DC, w, gPlayers);
}
