#include <nitro/types.h>

void WmClearFifoRecvFlag(void) { u16 *p = (u16 *)0x27fff96; if (*p & 1) *p &= ~1; }
