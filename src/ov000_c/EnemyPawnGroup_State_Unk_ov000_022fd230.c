#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_022c4560;

typedef struct {
    u8 pad0[8];
    u32 unk8;
} UnkStruct_022c4680;

extern UnkStruct_022c4560 gGameState;
extern UnkStruct_022c4680 gSaveData;
extern UnkStruct_StageObj *gPlayers;

void Sound_PlaySeqArcEx(s32 a, s32 b, s32 c);
void Sound_SetPositional(s32 a, UnkStruct_StageObj *w, UnkStruct_StageObj *p);
void func_ov000_022fc5b8(UnkStruct_StageObj *w, s32 a, s32 b, s32 c);
void EnemyPawnGroup_Action_Unk_ov000_022fe7e8(UnkStruct_StageObj *w);

void EnemyPawnGroup_State_Unk_ov000_022fd230(UnkStruct_StageObj *w) {
    if ((w->spr0.unkE >> 4) == 6) {
        if (!(w->unk1F4 & 4)) {
            Sound_PlaySeqArcEx(w->unk1DC, 0, 0x77);
            Sound_SetPositional(w->unk1DC, w, gPlayers);
            func_ov000_022fc5b8(w, 0xe6, 0x1c, -0x20);
        }
        w->unk1F4 |= 4;
    }
    if ((w->unk5C & 0x10) && w->unkC != 0) {
        w->unkC--;
        if (w->unkC == 0) {
            w->unk5C &= ~0x10;
            w->unk1F4 |= 1;
            w->unkC = 0x20;
            if ((gSaveData.unk8 & 1) && gGameState.unk10 != 2 && gGameState.unk10 != 3)
                w->unkC += w->unkC >> 1;
        }
    }
    if (!(w->unk5C & 8))
        return;
    w->unkC--;
    if (w->unkC == 0) {
        w->unk1F4 &= ~5;
        EnemyPawnGroup_Action_Unk_ov000_022fe7e8(w);
    }
}
