#include <ov000_b/stageobj.h>

void func_ov000_0231de44(void);

void func_ov000_0231dea0(UnkStruct_EffObj *w) {
    if (w->unk68 != func_ov000_0231de44)
        StageObj_SetAnimation(w, 2);
    w->unk5C |= 4;
    w->unk68 = func_ov000_0231de44;
}
