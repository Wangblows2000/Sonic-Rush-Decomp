#include <nitro/types.h>

typedef struct UnkStruct_022f8f00 {
    u8 pad[0x328];
    s16 unk328;
} UnkStruct_022f8f00;

extern UnkStruct_022f8f00 *data_ov000_0236ac60;

BOOL func_ov000_022f8f00(void) {
    return (u16)(s16)(data_ov000_0236ac60->unk328 - 3) <= 1;
}
