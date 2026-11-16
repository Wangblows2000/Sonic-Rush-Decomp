#include <nitro/types.h>

u32 ExtraBoss_GetUncompressedSize(u32 *p) {
    return *p >> 8;
}
