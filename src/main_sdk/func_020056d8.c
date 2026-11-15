#include <nitro/types.h>

void func_02005718(u32 a, void *b, void (*c)(void), u32 d);
void OSi_AllocateCartridgeBus(void);
void func_020056d8(u32 id) { func_02005718(id, (void *)0x27fffe8, OSi_AllocateCartridgeBus, 1); }
