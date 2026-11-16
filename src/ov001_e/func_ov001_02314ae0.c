#include <nitro/types.h>

typedef struct UnkStruct_02314ae0_v {
    s32 x;
    s32 y;
    s32 z;
} UnkStruct_02314ae0_v;

typedef struct UnkStruct_02314ae0_e {
    u8 pad0[0x20];
    UnkStruct_02314ae0_v pos;
    UnkStruct_02314ae0_v target;
    u8 pad38[0x58 - 0x38];
} UnkStruct_02314ae0_e;

extern UnkStruct_02314ae0_e *data_ov001_0231f6a4;

void func_ov001_02314f70(UnkStruct_02314ae0_e *e);

void func_ov001_02314ae0(s32 idx, UnkStruct_02314ae0_v v) {
    UnkStruct_02314ae0_e *e = &data_ov001_0231f6a4[idx];

    e->pos = v;
    func_ov001_02314f70(e);
}
