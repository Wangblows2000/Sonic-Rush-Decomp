#include <nitro/types.h>

typedef struct UnkStruct_0230d5bc_m {
    s32 m[12];
} UnkStruct_0230d5bc_m;

extern s16 data_0206e470[];

void MTX_TransApply43(const UnkStruct_0230d5bc_m *a, UnkStruct_0230d5bc_m *b, s32 x, s32 y, s32 z);
void MTX_RotY43_(UnkStruct_0230d5bc_m *m, s32 s, s32 c);
void MTX_Concat43(const UnkStruct_0230d5bc_m *a, const UnkStruct_0230d5bc_m *b, UnkStruct_0230d5bc_m *ab);

#define SIN(a) data_0206e470[(((a) & 0xffff) >> 4) * 2]
#define COS(a) data_0206e470[(((a) & 0xffff) >> 4) * 2 + 1]

void SpecialStage_RotateMtxYAroundPivot(UnkStruct_0230d5bc_m *m, s32 d, u16 ang) {
    UnkStruct_0230d5bc_m t;

    MTX_TransApply43(m, m, d, 0, 0);
    MTX_RotY43_(&t, SIN(ang), COS(ang));
    MTX_Concat43(&t, m, m);
    MTX_TransApply43(m, m, -d, 0, 0);
}
