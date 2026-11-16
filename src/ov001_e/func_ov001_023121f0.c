#include <nitro/types.h>

typedef struct UnkStruct_023121f0_v {
    s32 x;
    s32 y;
    s32 z;
} UnkStruct_023121f0_v;

extern UnkStruct_023121f0_v data_ov001_0231eea8;

s32 func_02002278(UnkStruct_023121f0_v *a, UnkStruct_023121f0_v *b);

BOOL func_ov001_023121f0(s32 x, s32 y) {
    UnkStruct_023121f0_v c = data_ov001_0231eea8;
    UnkStruct_023121f0_v v;

    v.x = x << 12;
    v.y = y << 12;
    v.z = 0;
    if (func_02002278(&c, &v) < 0x4b000)
        return TRUE;
    return FALSE;
}
