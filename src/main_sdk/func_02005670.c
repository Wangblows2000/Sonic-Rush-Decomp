#include <nitro/types.h>

void func_02005798(u32 a, void *b, void (*c)(void));
void OSi_FreeCardBus(void);
void func_02005670(u32 id) { func_02005798(id, (void *)0x27fffe0, OSi_FreeCardBus); }
