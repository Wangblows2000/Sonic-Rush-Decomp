#include <nitro/types.h>

void MBi_CommChangeParentStateCallbackOnly(u32 a, u32 b, void *c);
void MBi_CommCallParentError(u32 a, u16 v) { u16 t = v; MBi_CommChangeParentStateCallbackOnly(a, 0xd, &t); }
