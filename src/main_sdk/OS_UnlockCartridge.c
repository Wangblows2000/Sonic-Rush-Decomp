#include <nitro/types.h>

void OSi_DoUnlockByWord(u32 a, void *b, void (*c)(void), u32 d);
void OSi_FreeCartridgeBus(void);
void OS_UnlockCartridge(u32 id) { OSi_DoUnlockByWord(id, (void *)0x27fffe8, OSi_FreeCartridgeBus, 1); }
