#include <nitro/types.h>

u32 Compress_GetType(u32 *p) { return *p & 0xf0; }
