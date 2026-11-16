#include <nitro/types.h>

typedef struct UnkStruct_0230cc54_m {
    s32 m[12];
} UnkStruct_0230cc54_m;

extern s16 data_02071470[];

void MTX_TransApply43(const UnkStruct_0230cc54_m *a, UnkStruct_0230cc54_m *b, s32 x, s32 y, s32 z);
void MTX_RotY43_(UnkStruct_0230cc54_m *m, s32 s, s32 c);
void MTX_Concat43(const UnkStruct_0230cc54_m *a, const UnkStruct_0230cc54_m *b, UnkStruct_0230cc54_m *ab);

void func_ov001_0230cc54(UnkStruct_0230cc54_m *m) {
    UnkStruct_0230cc54_m t;

    MTX_RotY43_(&t, data_02071470[0], data_02071470[1]);
    MTX_Concat43(&t, m, m);
    MTX_TransApply43(m, m, -0xbf000, 0, 0);
}
