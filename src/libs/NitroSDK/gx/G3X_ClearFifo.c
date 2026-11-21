#include <nitro/types.h>

void GXi_NopClearFifo128_(void *p);
void G3X_ClearFifo(void) { GXi_NopClearFifo128_((void *)0x4000400); while (*(vu32 *)0x4000600 & 0x8000000) {} }
