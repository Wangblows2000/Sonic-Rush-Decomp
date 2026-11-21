#include <nitro/types.h>

u32 Stage_GetUncompressedSize(u32 *p) {
    return *p >> 8;
}
