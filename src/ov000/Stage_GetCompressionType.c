#include <nitro/types.h>

u32 Stage_GetCompressionType(u32 *p) {
    return *p & 0xf0;
}
