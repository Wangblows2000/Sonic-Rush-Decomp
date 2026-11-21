#include <nitro/types.h>

void OS_SetIrqFunction(u32 m, void (*fn)(void));
void GX_SetVCountEqVal(s32 v);
u32 OS_EnableIrqMask(u32 m);
void OSi_VAlarmHandler(void);
typedef struct { u8 pad[0x10]; s16 line; } UnkStruct_02007374;
void OSi_SetNextVAlarm(UnkStruct_02007374 *p) { OS_SetIrqFunction(4, OSi_VAlarmHandler); GX_SetVCountEqVal(p->line); *(vu16 *)0x4000004 |= 0x20; OS_EnableIrqMask(4); }
