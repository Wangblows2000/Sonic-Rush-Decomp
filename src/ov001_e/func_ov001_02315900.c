#include <nitro/types.h>

typedef struct UnkStruct_02315900_v {
    s32 x;
    s32 y;
    s32 z;
} UnkStruct_02315900_v;

typedef struct UnkStruct_02315900_e {
    u8 pad0[0x20];
    UnkStruct_02315900_v pos;
    UnkStruct_02315900_v target;
    UnkStruct_02315900_v up;
    u8 pad44[0x58 - 0x44];
} UnkStruct_02315900_e;

typedef struct UnkStruct_02315900_g {
    u8 pad0[0x34];
    s32 cur;
} UnkStruct_02315900_g;

extern UnkStruct_02315900_g data_ov001_0231f6ac;
extern UnkStruct_02315900_e *data_ov001_0231f6a4;

void func_ov001_02315900(UnkStruct_02315900_v *pos, UnkStruct_02315900_v *target, UnkStruct_02315900_v *up) {
    UnkStruct_02315900_g *c = &data_ov001_0231f6ac;

    if (pos != NULL)
        *pos = data_ov001_0231f6a4[c->cur].pos;
    if (target != NULL)
        *target = data_ov001_0231f6a4[c->cur].target;
    if (up != NULL)
        *up = data_ov001_0231f6a4[c->cur].up;
}
