#include <nitro/types.h>

typedef struct UnkStruct_0230cac8_m {
    s32 m[12];
} UnkStruct_0230cac8_m;

extern s16 data_0206f470[];

void MTX_TransApply43(const UnkStruct_0230cac8_m *a, UnkStruct_0230cac8_m *b, s32 x, s32 y, s32 z);
void MTX_RotY43_(UnkStruct_0230cac8_m *m, s32 s, s32 c);
void MTX_Concat43(const UnkStruct_0230cac8_m *a, const UnkStruct_0230cac8_m *b, UnkStruct_0230cac8_m *ab);

void func_ov001_0230cac8(UnkStruct_0230cac8_m *m) {
    UnkStruct_0230cac8_m t;

    MTX_TransApply43(m, m, 0xbf000, 0, 0);
    MTX_RotY43_(&t, data_0206f470[0], data_0206f470[1]);
    MTX_Concat43(&t, m, m);
}
