#include <nitro/types.h>

u32 TimeSelectMenu_GetCompressedSize(u32 *p) {
    return *p >> 8;
}
