#include <nitro/types.h>

extern void *gTaskCurrent;
void Task_Destroy(void *t);

void func_ov001_022c689c(void) {
    {
        u32 v = (*(vu32 *)0x4000000 & 0x1f00) >> 8;
        *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | ((v & ~1) << 8);
    }
    Task_Destroy(gTaskCurrent);
}
