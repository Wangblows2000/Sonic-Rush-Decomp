#include <nitro/types.h>

u32 ZoneActSelect_GetUncompressedSize(u32 *p) {
    return *p >> 8;
}
