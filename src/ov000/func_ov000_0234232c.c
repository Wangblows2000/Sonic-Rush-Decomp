#include <nitro/types.h>

typedef struct UnkStruct_0234232c {
    u8 pad[0x10c];
    u8 unk10C;
} UnkStruct_0234232c;

extern UnkStruct_0234232c data_ov000_0236aad8;

void func_ov000_0234232c(u8 v) {
    if (data_ov000_0236aad8.unk10C == v)
        data_ov000_0236aad8.unk10C = 0xff;
}
