#include <nitro/types.h>

void func_020057a8(u32 a, void *b, void (*c)(void), u32 d);
void OSi_FreeCartridgeBus(void);
void func_020056f8(u32 id) { func_020057a8(id, (void *)0x27fffe8, OSi_FreeCartridgeBus, 1); }
