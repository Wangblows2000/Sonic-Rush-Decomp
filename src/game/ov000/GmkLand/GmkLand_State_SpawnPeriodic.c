#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad[0x20];
    s32 unk20;
} UnkStruct_022c4560;

extern UnkStruct_022c4560 gGameState;

UnkStruct_StageObj *MapObject_Spawn(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i);

void GmkLand_State_SpawnPeriodic(UnkStruct_StageObj *w) {
    s32 t = (w->unk1E0->unk8 + 1) << 6;

    if (gGameState.unk20 - w->unkC < t)
        return;
    w->unkC += t;
    MapObject_Spawn(0xef, w->unk10, w->unk14, 0, 0, w->unk1E0->unk7, 0, 0, 0);
}
