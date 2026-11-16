#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x20];
    u32 unk20;
} UnkStruct_022c4560;

extern UnkStruct_022c4560 gGameState;

typedef struct {
    u8 pad0[0x9c];
    u16 unk9C;
} UnkView_0232053c;

void func_ov000_0232053c(UnkStruct_EffObj *w) {
    if (w->unk88 == NULL)
        return;
    w->unk10 = ((UnkStruct_EffObj *)w->unk88)->unk10;
    w->unk14 = ((UnkStruct_EffObj *)w->unk88)->unk14;
    if (((UnkStruct_EffObj *)w->unk88)->unkC != 0) {
        if (gGameState.unk20 & 1)
            w->unk5C &= ~0x20;
        else
            w->unk5C |= 0x20;
        if (((UnkView_0232053c *)w)->unk9C == 1)
            w->unk5C ^= 0x20;
        return;
    }
    w->unk5C |= 0x20;
}
