#include <ov000_b/stageobj.h>

void StageObj_ReleaseSpriteVram(UnkStruct_EffObj *w);

void Effect_State_MoveUntilAnimEnd(UnkStruct_EffObj *w) {
    if (w->unkC != 0) {
        if (w->unk5C & 1)
            w->unk2E -= (s16)(w->unkC >> 16);
        else
            w->unk2E += (s16)(w->unkC >> 16);
        w->unk30 += (s16)w->unkC;
    }
    if (w->unk5C & 8) {
        w->unk54 |= 4;
        StageObj_ReleaseSpriteVram(w);
    }
}
