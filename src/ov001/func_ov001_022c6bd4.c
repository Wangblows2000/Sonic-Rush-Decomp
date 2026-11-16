#include <nitro/types.h>

extern void *gTaskCurrent;
void Task_Destroy(void *p);

void func_ov001_022c6bd4(void) {
    *(vu32 *)0x04000000 = (*(vu32 *)0x04000000 & ~0x1f00) | 0x700;
    Task_Destroy(gTaskCurrent);
}
