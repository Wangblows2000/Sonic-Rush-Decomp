#include <nitro/types.h>

typedef struct UnkStruct_0235f8cc {
    u8 pad[0x8];
    u16 unk8;
} UnkStruct_0235f8cc;

extern UnkStruct_0235f8cc data_0208922c;
extern UnkStruct_0235f8cc data_02087d04;

#pragma thumb on
void func_ov000_0235f8cc(void) {
    data_0208922c.unk8 += 2;
    data_02087d04.unk8 += 2;
}
#pragma thumb reset
