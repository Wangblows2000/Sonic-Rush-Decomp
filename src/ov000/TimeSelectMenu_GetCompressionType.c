#include <nitro/types.h>

u32 TimeSelectMenu_GetCompressionType(u32 *p) {
    return *p & 0xf0;
}
