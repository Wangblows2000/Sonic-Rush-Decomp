#include <nitro/types.h>

typedef struct { u8 unk0[0x10]; u32 unk10; u8 unk14; } UnkStruct_02037cac;
extern UnkStruct_02037cac data_022b65b4;
extern u32 data_02087cb8;
void func_0200d554(void);
void func_0200d18c(u32 a);
BOOL func_0200d178(u32 a);

void func_02037cac(void) {
    if (data_022b65b4.unk10 & 2) {
        func_0200d554();
        func_0200d18c(4);
        if (func_0200d178(4)) return;
        data_022b65b4.unk10 &= ~1;
        data_022b65b4.unk10 &= ~2;
        data_022b65b4.unk14 = 0;
        data_02087cb8 &= ~0x8000;
    } else {
        data_022b65b4.unk10 &= ~1;
    }
}
