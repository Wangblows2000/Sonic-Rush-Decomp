#include <nitro/types.h>

u32 Records_GetUncompressedSize(u32 *p) {
    return *p >> 8;
}
