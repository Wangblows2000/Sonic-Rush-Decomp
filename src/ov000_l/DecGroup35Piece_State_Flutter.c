#include <ov000_b/stageobj.h>

void DecGroup35Piece_State_Flutter(UnkStruct_EffObj *w) {
    if (!(Rand() & 0x1f)) {
        w->unk2E = -w->unk2E;
        if (!(Rand() & 1))
            w->unk5C ^= 1;
    }
    if (!(Rand() & 0x1f))
        w->unk30 = 0;
}
