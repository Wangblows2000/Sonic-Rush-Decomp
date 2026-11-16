#include <nitro/types.h>

typedef struct UnkStruct_0235cadc {
    u8 pad[0xc];
    u16 unkc;
} UnkStruct_0235cadc;

extern UnkStruct_0235cadc gDisplayMain;
extern UnkStruct_0235cadc gDisplaySub;

#pragma thumb on
void func_ov000_0235cadc(void) {
    gDisplayMain.unkc += 2;
    gDisplaySub.unkc += 2;
}
#pragma thumb reset
