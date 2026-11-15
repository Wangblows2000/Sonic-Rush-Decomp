#include <nitro/types.h>

typedef struct { u8 unk0[0x10]; u32 unk10; u8 unk14; } UnkStruct_02037d40;
extern UnkStruct_02037d40 data_022b65b4;

void func_02037d40(void) {
    data_022b65b4.unk14 = 0;
    data_022b65b4.unk10 &= ~1;
}
