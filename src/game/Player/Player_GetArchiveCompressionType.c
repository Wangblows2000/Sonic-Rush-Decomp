#include <nitro/types.h>

u32 Player_GetArchiveCompressionType(u32 *p) { return *p & 0xf0; }
