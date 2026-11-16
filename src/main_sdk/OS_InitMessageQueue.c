#include <nitro/types.h>

typedef struct { u16 a; u16 b; u32 c; u32 d; u32 e; u32 f; } UnkStruct_02006548;
void OS_InitMessageQueue(UnkStruct_02006548 *p, u32 c, u32 d) { p->a = 0; p->b = 0; p->c = c; p->d = d; p->e = 0; p->f = 0; }
