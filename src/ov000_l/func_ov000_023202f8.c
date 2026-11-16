#include <ov000_b/stageobj.h>

void func_ov000_023202f8(UnkStruct_EffObj *w) {
    UnkStruct_EffObj *p = w->unk88;

    if (p != NULL) {
        if (p->unk8 & (1 << w->unk8))
            w->unk5C &= ~0x20;
        else
            w->unk5C |= 0x20;
        return;
    }
    w->unk5C |= 0x20;
}
