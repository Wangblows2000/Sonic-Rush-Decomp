#include <nitro/types.h>

typedef struct UnkStruct_0230ccb0_m {
    s32 m[12];
} UnkStruct_0230ccb0_m;

extern s16 data_0206f470[];

void MTX_TransApply43(const UnkStruct_0230ccb0_m *a, UnkStruct_0230ccb0_m *b, s32 x, s32 y, s32 z);
void MTX_RotY43_(UnkStruct_0230ccb0_m *m, s32 s, s32 c);
void MTX_Concat43(const UnkStruct_0230ccb0_m *a, const UnkStruct_0230ccb0_m *b, UnkStruct_0230ccb0_m *ab);

void SpecialStageMap_Curve1Advance(UnkStruct_0230ccb0_m *m) {
    UnkStruct_0230ccb0_m t;

    MTX_RotY43_(&t, data_0206f470[0], data_0206f470[1]);
    MTX_Concat43(&t, m, m);
    MTX_TransApply43(m, m, 0xbf000, 0, 0);
}
