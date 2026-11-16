#include <ov000_b/stageobj.h>

UnkStruct_EffObj *FixMoji3D_Create(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void func_ov000_0232a534(void);

void func_ov000_0232a780(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f) {
    UnkStruct_EffObj *w = FixMoji3D_Create(a, b, c, d, e, f);

    if (w != NULL)
        w->unk68 = func_ov000_0232a534;
}
