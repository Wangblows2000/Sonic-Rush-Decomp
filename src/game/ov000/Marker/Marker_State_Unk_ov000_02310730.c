#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x15];
    u8 unk15;
} UnkStruct_022c4590;

typedef struct {
    u8 pad0[0x9c];
    u16 unk9C;
} UnkView_02310730;

extern UnkStruct_022c4590 data_022c4590;

void Marker_State_Unk_ov000_02310730(UnkStruct_StageObj *w) {
    UnkStruct_022c4590 *g = &data_022c4590;

    if (((UnkView_02310730 *)w)->unk9C == 0 && (w->unk1E0->unk4 & 0xff) <= g->unk15) {
        StageObj_SetAnimation(w, 2);
        w->unk5C |= 4;
        w->unk54 |= 2;
    }
    if (((UnkView_02310730 *)w)->unk9C == 1 && (w->unk5C & 8)) {
        StageObj_SetAnimation(w, 2);
        w->unk5C |= 4;
    }
}
