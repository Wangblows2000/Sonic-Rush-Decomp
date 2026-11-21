#include <nitro/types.h>

typedef struct UnkStruct_0230d048 {
    s32 unk0;
    u8 pad4[4];
    u8 mtx[0x30];
} UnkStruct_0230d048;

extern UnkStruct_0230d048 *gSpecialStageMap;

u16 SpecialStage_ArcLengthToAngle(s32 a, s32 b);
void SpecialStage_RotateMtxXAroundPivot(void *m, s32 d, u16 ang);

void SpecialStageMap_ScrollCurve3(void) {
    UnkStruct_0230d048 *g = gSpecialStageMap;
    u16 a = -SpecialStage_ArcLengthToAngle(0xbf000, g->unk0);

    SpecialStage_RotateMtxXAroundPivot(g->mtx, 0xbf000, a);
}
