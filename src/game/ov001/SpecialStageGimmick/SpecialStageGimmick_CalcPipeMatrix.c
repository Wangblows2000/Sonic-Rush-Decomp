#include <nitro/types.h>

typedef struct UnkStruct_0230d28c_m {
    s32 m[12];
} UnkStruct_0230d28c_m;

extern s16 data_0206e470[];

#define SIN_IDX(i) data_0206e470[(u32)((i) << 1)]
#define COS_IDX(i) data_0206e470[(u32)(((i) << 1) + 1)]

void MTX_Identity43_(UnkStruct_0230d28c_m *m);
void MTX_RotZ43_(UnkStruct_0230d28c_m *m, s32 s, s32 c);
void MTX_Concat43(const UnkStruct_0230d28c_m *a, const UnkStruct_0230d28c_m *b, UnkStruct_0230d28c_m *ab);
void MTX_TransApply43(const UnkStruct_0230d28c_m *a, UnkStruct_0230d28c_m *b, s32 x, s32 y, s32 z);
u16 SpecialStage_ArcLengthToAngle(s32 a, s32 b);
void SpecialStage_RotateMtxYAroundPivot(UnkStruct_0230d28c_m *m, s32 a, u16 b);
void SpecialStage_RotateMtxXAroundPivot(UnkStruct_0230d28c_m *m, s32 a, u16 b);

void SpecialStageGimmick_CalcPipeMatrix(UnkStruct_0230d28c_m *m, s32 mode, s32 flag, s32 force, u32 ang, s32 y, s32 z) {
    UnkStruct_0230d28c_m t;
    UnkStruct_0230d28c_m t2;
    s32 idx;

    if (force == 1 || (flag != 0 && (ang < 0x38e3 || ang > 0xc71c))) {
        MTX_Identity43_(m);
        idx = (s32)(ang & 0xffff) >> 4;
        MTX_RotZ43_(&t, SIN_IDX(idx), COS_IDX(idx));
        MTX_Concat43(&t, m, m);
        MTX_TransApply43(m, m, 0, y - 0x1e000, 0);
    } else {
        MTX_Identity43_(m);
        MTX_TransApply43(m, m, 0, y, 0);
        idx = (s32)(ang & 0xffff) >> 4;
        MTX_RotZ43_(&t, SIN_IDX(idx), COS_IDX(idx));
        MTX_Concat43(&t, m, m);
        MTX_TransApply43(m, m, 0, -0x1e000, 0);
    }
    switch (mode) {
    case 0:
    case 5:
    case 6:
        MTX_TransApply43(m, m, 0, 0, -z);
        break;
    case 1:
        MTX_Identity43_(&t2);
        SpecialStage_RotateMtxYAroundPivot(&t2, -0xbf000, SpecialStage_ArcLengthToAngle(0xbf000, z));
        MTX_Concat43(m, &t2, m);
        break;
    case 2:
        MTX_Identity43_(&t2);
        SpecialStage_RotateMtxYAroundPivot(&t2, 0xbf000, -SpecialStage_ArcLengthToAngle(0xbf000, z));
        MTX_Concat43(m, &t2, m);
        break;
    case 3:
        MTX_Identity43_(&t2);
        SpecialStage_RotateMtxXAroundPivot(&t2, 0xbf000, SpecialStage_ArcLengthToAngle(0xbf000, z));
        MTX_Concat43(m, &t2, m);
        break;
    case 4:
        MTX_Identity43_(&t2);
        SpecialStage_RotateMtxXAroundPivot(&t2, -0xbf000, -SpecialStage_ArcLengthToAngle(0xbf000, z));
        MTX_Concat43(m, &t2, m);
        break;
    }
}
