#include <nitro/types.h>

typedef struct { u8 pad[0x70]; u32 priority; } OSThread;
u32 OS_GetThreadPriority(const OSThread *t) { return t->priority; }
