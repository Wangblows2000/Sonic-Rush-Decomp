#include <nitro/types.h>

typedef struct UnkStruct_02342348 {
    u8 pad[0x10c];
    u8 unk10C;
} UnkStruct_02342348;

extern UnkStruct_02342348 data_ov000_0236aad8;

void func_ov000_02342348(u8 v) {
    data_ov000_0236aad8.unk10C = v;
}
