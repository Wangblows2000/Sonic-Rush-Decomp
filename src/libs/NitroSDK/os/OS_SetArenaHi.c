#include <nitro/types.h>

void OS_SetArenaHi(u32 id, void *hi) { ((void **)0x027FFDC4)[id] = hi; }
