#include <nitro/types.h>

void OSi_DoTryLockByWord(u32 a, void *b, void (*c)(void), u32 d);
void OSi_AllocateCartridgeBus(void);
void OS_TryLockCartridge(u32 id) { OSi_DoTryLockByWord(id, (void *)0x27fffe8, OSi_AllocateCartridgeBus, 1); }
