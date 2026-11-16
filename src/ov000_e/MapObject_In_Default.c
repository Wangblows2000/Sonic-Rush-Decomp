#include <nitro/types.h>

typedef struct UnkStruct_022fb2f8 {
    u8 pad0[0x54];
    u32 unk54;
    u8 pad58[0x5C - 0x58];
    u16 unk5C;
    u8 pad5E[0x158 - 0x5E];
    u32 unk158;
    u8 pad15C[0x180 - 0x15C];
    u32 unk180;
    u8 pad184[0x1F0 - 0x184];
    s16 unk1F0;
    u8 pad1F2[0x204 - 0x1F2];
    struct UnkStruct_022fb2f8 *unk204;
} UnkStruct_022fb2f8;

typedef struct {
    u8 pad[0x10];
    UnkStruct_022fb2f8 *unk10;
} UnkStruct_022b4574;

extern UnkStruct_022b4574 *gTaskCurrent;

void MapObject_Unk_ov000_022faa74(UnkStruct_022fb2f8 *w);
BOOL StageObj_PauseCheck(u32 v);

void MapObject_In_Default(void) {
    UnkStruct_022fb2f8 *w = gTaskCurrent->unk10;

    if (w->unk204 != NULL && (w->unk204->unk54 & 4))
        w->unk204 = NULL;
    MapObject_Unk_ov000_022faa74(w);
    if (StageObj_PauseCheck(w->unk54))
        return;
    if (w->unk1F0 == 0)
        return;
    w->unk1F0--;
    if (w->unk1F0 & 2)
        w->unk5C |= 0x20;
    else
        w->unk5C &= ~0x20;
    w->unk158 |= 5;
    w->unk180 &= ~7;
    if (w->unk1F0 != 0)
        return;
    w->unk5C &= ~0x20;
    w->unk158 = 0xa;
    w->unk180 |= 3;
}
