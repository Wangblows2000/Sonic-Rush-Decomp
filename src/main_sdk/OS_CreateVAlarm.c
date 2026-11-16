#include <nitro/types.h>

typedef struct { u32 a; u32 pad; u32 b; u32 pad2[5]; u32 c; } UnkStruct_02007450;
void OS_CreateVAlarm(UnkStruct_02007450 *p) { p->a = 0; p->b = 0; p->c = 0; }
