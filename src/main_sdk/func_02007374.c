#include <nitro/types.h>

void func_02005520(u32 m, void (*fn)(void));
void func_02002be8(s32 v);
u32 OS_EnableIrqMask(u32 m);
void func_02007110(void);
typedef struct { u8 pad[0x10]; s16 line; } UnkStruct_02007374;
void func_02007374(UnkStruct_02007374 *p) { func_02005520(4, func_02007110); func_02002be8(p->line); *(vu16 *)0x4000004 |= 0x20; OS_EnableIrqMask(4); }
