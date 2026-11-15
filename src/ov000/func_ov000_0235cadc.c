#include <nitro/types.h>

typedef struct UnkStruct_0235cadc {
    u8 pad[0xc];
    u16 unkc;
} UnkStruct_0235cadc;

extern UnkStruct_0235cadc data_0208922c;
extern UnkStruct_0235cadc data_02087d04;

#pragma thumb on
void func_ov000_0235cadc(void) {
    data_0208922c.unkc += 2;
    data_02087d04.unkc += 2;
}
#pragma thumb reset
