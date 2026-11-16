#include <nitro/types.h>

u32 StageLoader_GetCompressionType(u32 *p) {
    return *p & 0xf0;
}
