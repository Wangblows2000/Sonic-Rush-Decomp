#include <game/Stage/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x440 - 0x20C];
    void *unk440;
    u8 pad444[0x464 - 0x444];
    s32 unk464;
    s32 unk468;
    s32 unk46C;
    s32 unk470;
    s32 unk474;
    s32 unk478;
    u16 unk47C;
} UnkStruct_022e75b0;

extern UnkStruct_StageObj *gPlayers;
extern u8 gBGMSoundHandle[];

void Boss7_SetRivalAnimation(UnkStruct_022e75b0 *w, s32 a);
void Boss7_SetCollidersEnabled(UnkStruct_022e75b0 *w, s32 a, s32 b);
void NNS_SndPlayerMoveVolume(void *a, s32 b, s32 c);
void Boss7PartD5_MoveState_DefeatScene(void);

#define ABS(x) ((x) < 0 ? -(x) : (x))
#define CLAMP(x, lo, hi) ((x) < (lo) ? (lo) : ((x) > (hi) ? (hi) : (x)))

void Boss7PartD5_MoveState_WaitDefeatScene(UnkStruct_022e75b0 *w) {
    if (!(w->base.unk58 & 1))
        return;
    if (!(gPlayers->unk58 & 1))
        return;
    if ((s16)w->base.unk1F0 > 0)
        return;
    Boss7_SetRivalAnimation(w, 0x3c);
    Boss7_SetCollidersEnabled(w, 0, 0);
    w->base.unk5C |= 4;
    gPlayers->unk5C &= ~1;
    w->base.unk5C |= 1;
    w->unk464 = gPlayers->unk10 << 4;
    w->unk468 = gPlayers->unk14 << 4;
    w->unk470 = w->base.unk10 << 4;
    w->unk474 = w->base.unk14 << 4;
    w->unk47C = 0;
    w->unk46C = (gPlayers->unk10 >> 8) - 0x40;
    w->unk46C = ABS(w->unk46C);
    w->unk46C >>= 1;
    w->unk46C = CLAMP(w->unk46C, 0x10, 0x28);
    w->unk478 = (w->base.unk10 >> 8) - 0x100;
    w->unk478 = ABS(w->unk478);
    w->unk478 >>= 1;
    w->unk478 = CLAMP(w->unk478, 0x10, 0x28);
    NNS_SndPlayerMoveVolume(gBGMSoundHandle, 0x20, 0xb4);
    w->unk440 = Boss7PartD5_MoveState_DefeatScene;
}
