#include <nitro/types.h>

typedef struct UnkStruct_02342358 {
    u8 pad[0x10c];
    u8 unk10C;
} UnkStruct_02342358;

extern UnkStruct_02342358 data_ov000_0236aad8;
extern s32 data_ov000_0236abc4[];

s32 func_ov000_02342358(void) {
    u8 i = data_ov000_0236aad8.unk10C;
    if (i != 0xff)
        return data_ov000_0236abc4[i];
    return 0;
}
