#include <ov000_b/stageobj.h>

void func_ov000_0231b564(void);

void func_ov000_0231b568(UnkStruct_StageObj *w) {
    u16 f = w->unk1E0->unk4;

    switch (f & 3) {
    case 0:
        StageObj_SetAnimation(w, 0);
        break;
    case 1:
        StageObj_SetAnimation(w, 1);
        break;
    case 2:
    default:
        StageObj_SetAnimation(w, 2);
        break;
    }
    w->unk68 = func_ov000_0231b564;
}
