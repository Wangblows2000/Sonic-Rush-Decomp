#include <nitro/types.h>

u32 ZoneBg_GetUncompressedSize(u32 *p) {
    return *p >> 8;
}
