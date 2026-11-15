#include <nitro/types.h>

void func_02003e9c(void *p);
void func_020042dc(void) { func_02003e9c((void *)0x4000400); while (*(vu32 *)0x4000600 & 0x8000000) {} }
