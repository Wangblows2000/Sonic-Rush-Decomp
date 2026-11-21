#include <game/Stage/stageobj.h>

typedef struct {
    u8 pad[0x20];
    u32 unk20;
} UnkStruct_022c4560;

extern UnkStruct_022c4560 gGameState;

void EffZakoBombPuff_Create(UnkStruct_StageObj *w, s16 x, s16 y);
s16 ObjSpd_UpSet(s32 v, s32 a, s32 b);

void JetCatapult_State_Fly(UnkStruct_StageObj *w) {
    w->unk1 += 8;
    if ((gGameState.unk20 & 7) == 0)
        EffZakoBombPuff_Create(w, 15 - (Rand() & 0x1e), 7 - (Rand() & 0xe));
    if (w->unk58 & 4) {
        w->unk34 = 0;
        return;
    }
    w->unk34 = ObjSpd_UpSet(w->unk34, 0x18, 0xf00);
}
