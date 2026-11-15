#include <nitro/types.h>

typedef struct { u8 pad[0x24]; s32 v; } UnkStruct_02314694;
extern u8 *data_ov001_0231f6a0;

s32 func_ov001_02314694(s32 i) {
    return (*(UnkStruct_02314694 **)(data_ov001_0231f6a0 + 0x10c))[i].v;
}
