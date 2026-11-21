#include <nitro/types.h>

extern u8 *gDisplayTable[];
extern void *gTaskCurrent;
void MIi_CpuClear16(u16 data, void *dst, u32 size);
void Task_Destroy(void *p);
void OamUnfreezeTask_MainB(void) { u8 *p = gDisplayTable[1]; vu16 v = 0x200; MIi_CpuClear16(v, p + 0xca8, 0x400); *(u16 *)(p + 0x8a0) = 0; Task_Destroy(gTaskCurrent); }
