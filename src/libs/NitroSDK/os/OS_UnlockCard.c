#include <nitro/types.h>

void OS_UnlockByWord(u32 a, void *b, void (*c)(void));
void OSi_FreeCardBus(void);
void OS_UnlockCard(u32 id) { OS_UnlockByWord(id, (void *)0x27fffe0, OSi_FreeCardBus); }
