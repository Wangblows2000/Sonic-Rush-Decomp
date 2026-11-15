#include <nitro/types.h>

typedef struct UnkStruct_022e73c4 {
    u8 pad[0x20c];
    u32 unk20C;
} UnkStruct_022e73c4;

extern UnkStruct_022e73c4 *data_ov000_02369b78;

void func_ov000_022e73c4(void) {
    data_ov000_02369b78->unk20C |= 2;
}
