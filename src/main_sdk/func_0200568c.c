#include <nitro/types.h>

void func_02005838(u32 a, void *b, void (*c)(void));
void OSi_AllocateCardBus(void);
void func_0200568c(u32 id) { func_02005838(id, (void *)0x27fffe0, OSi_AllocateCardBus); }
