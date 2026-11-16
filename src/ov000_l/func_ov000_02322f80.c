#include <ov000_b/stageobj.h>

void func_ov000_02322f80(UnkStruct_EffObj *w) {
    if (w->unk88 == NULL)
        return;
    if (((UnkStruct_EffObj *)w->unk88)->unk18 <= 0) {
        StageObj_SetOAMPriority(w, 1);
        return;
    }
    StageObj_SetOAMPriority(w, 2);
}
