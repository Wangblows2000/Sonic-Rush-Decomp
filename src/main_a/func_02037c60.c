#include <nitro/types.h>

typedef struct { u8 unk0[0x10]; u32 unk10; } UnkStruct_02037c60;
extern UnkStruct_02037c60 data_022b65b4;
void func_0203787c(void);
void func_02037a8c(void);

void func_02037c60(void) {
    u32 f = data_022b65b4.unk10;
    if (!(f & 1)) return;
    if (f & 2) {
        func_0203787c();
    } else {
        func_02037a8c();
    }
}
