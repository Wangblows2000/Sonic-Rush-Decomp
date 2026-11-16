#include <nitro/types.h>

extern u8 *data_02080f1c;
void MBi_CommChangeParentStateCallbackOnly(u32 a, u32 b, void *c) { void (*fn)(u32, u32, void *) = *(void (**)(u32, u32, void *))(data_02080f1c + 0x14e4); if (fn == 0) return; fn(a, b, c); }
