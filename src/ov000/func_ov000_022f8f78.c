#include <nitro/types.h>

typedef struct UnkStruct_022f8f78 {
    u8 pad[0x10];
    s32 unk10;
} UnkStruct_022f8f78;

extern UnkStruct_022f8f78 *data_ov000_0236ac60;

BOOL func_ov000_022f8f78(void) {
    return data_ov000_0236ac60->unk10 > 0x2fa00;
}
