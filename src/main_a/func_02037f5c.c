#include <nitro/types.h>

typedef struct { u8 unk0[0x10]; u32 *unk10; } UnkStruct_02037f5c;
extern UnkStruct_02037f5c *gTaskCurrent;
void func_020257f8(u32 a);
void Task_Destroy(void *a);

void func_02037f5c(void) {
    func_020257f8(*gTaskCurrent->unk10);
    Task_Destroy(gTaskCurrent);
}
