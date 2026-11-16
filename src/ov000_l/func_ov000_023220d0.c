#include <ov000_b/stageobj.h>

void func_ov000_023220d0(UnkStruct_EffObj *w) {
    if (!(Rand() & 0x1f)) {
        w->unk2E = -w->unk2E;
        if (!(Rand() & 1))
            w->unk5C ^= 1;
    }
    if (!(Rand() & 0x1f))
        w->unk30 = 0;
}
