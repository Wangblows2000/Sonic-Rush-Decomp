#include <nitro/types.h>

void OS_LockByWord(u32 a, void *b, void (*c)(void));
void OSi_AllocateCardBus(void);
void OS_LockCard(u32 id) { OS_LockByWord(id, (void *)0x27fffe0, OSi_AllocateCardBus); }
