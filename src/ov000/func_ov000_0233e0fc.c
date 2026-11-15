#include <nitro/types.h>

typedef struct UnkStruct_0233e0fc {
    u8 pad[0x24];
    u16 unk24;
} UnkStruct_0233e0fc;

extern UnkStruct_0233e0fc *data_02072c78[];

void func_ov000_0233e0fc(void) {
    data_02072c78[0]->unk24 = 0;
    data_02072c78[1]->unk24 = 0;
}
