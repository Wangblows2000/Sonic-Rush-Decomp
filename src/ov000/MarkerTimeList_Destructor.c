#include <nitro/types.h>

typedef struct UnkStruct_0232aff8_2 {
    u8 pad[0xc8];
    s32 unkC8;
    u8 pad2[0x110 - 0xcc];
    s32 unk110;
} UnkStruct_0232aff8_2;

typedef struct UnkStruct_0232aff8 {
    u8 pad[0x10];
    UnkStruct_0232aff8_2 *unk10;
} UnkStruct_0232aff8;

extern s32 gMarkerTimeListTask;

void StageObj_Destructor(UnkStruct_0232aff8 *s);

void MarkerTimeList_Destructor(UnkStruct_0232aff8 *s) {
    UnkStruct_0232aff8_2 *p = s->unk10;
    p->unkC8 = 0;
    p->unk110 = 0;
    gMarkerTimeListTask = 0;
    StageObj_Destructor(s);
}
