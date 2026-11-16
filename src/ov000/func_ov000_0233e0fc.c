#include <nitro/types.h>

typedef struct UnkStruct_0233e0fc {
    u8 pad[0x24];
    u16 unk24;
} UnkStruct_0233e0fc;

extern UnkStruct_0233e0fc *gDisplayTable[];

void func_ov000_0233e0fc(void) {
    gDisplayTable[0]->unk24 = 0;
    gDisplayTable[1]->unk24 = 0;
}
