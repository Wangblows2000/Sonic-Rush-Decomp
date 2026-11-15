#include <nitro/types.h>

typedef struct UnkStruct_022ee420 {
    u8 pad[0x220];
    s32 unk220;
} UnkStruct_022ee420;

extern UnkStruct_022ee420 *data_ov000_02369b78;

void func_ov000_022ee420(void) {
    data_ov000_02369b78->unk220++;
}
