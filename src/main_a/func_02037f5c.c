#include <nitro/types.h>

typedef struct { u8 unk0[0x10]; u32 *unk10; } UnkStruct_02037f5c;
extern UnkStruct_02037f5c *gTaskCurrent;
void NNS_G3dResDefaultSetup(u32 a);
void Task_Destroy(void *a);

void func_02037f5c(void) {
    NNS_G3dResDefaultSetup(*gTaskCurrent->unk10);
    Task_Destroy(gTaskCurrent);
}
