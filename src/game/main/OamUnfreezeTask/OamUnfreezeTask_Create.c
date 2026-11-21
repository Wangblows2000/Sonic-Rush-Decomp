#include <nitro/types.h>

void OamUnfreezeTask_MainA(void);
void OamUnfreezeTask_MainB(void);
void *Task_Create(void (*fn)(void), s32 a, s32 b, s32 c, s32 d, s32 e);
void OamUnfreezeTask_Create(BOOL on) { Task_Create(on == FALSE ? OamUnfreezeTask_MainA : OamUnfreezeTask_MainB, 0, 0x30, 0xefff, 0xfffe, 0); }
