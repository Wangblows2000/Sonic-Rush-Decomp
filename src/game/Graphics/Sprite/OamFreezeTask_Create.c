#include <nitro/types.h>

void OamFreezeTask_MainA(void);
void OamFreezeTask_MainB(void);
void *Task_Create(void (*fn)(void), s32 a, s32 b, s32 c, s32 d, s32 e);
void OamFreezeTask_Create(BOOL on) { Task_Create(on == FALSE ? OamFreezeTask_MainA : OamFreezeTask_MainB, 0, 0x30, 0xefff, 0xfffe, 0); }
