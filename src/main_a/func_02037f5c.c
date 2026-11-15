#include <nitro/types.h>

typedef struct { u8 unk0[0x10]; u32 *unk10; } UnkStruct_02037f5c;
extern UnkStruct_02037f5c *data_022b4574;
void func_020257f8(u32 a);
void func_020371c8(void *a);

void func_02037f5c(void) {
    func_020257f8(*data_022b4574->unk10);
    func_020371c8(data_022b4574);
}
