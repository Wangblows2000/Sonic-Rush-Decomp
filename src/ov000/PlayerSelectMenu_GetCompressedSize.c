#include <nitro/types.h>

u32 PlayerSelectMenu_GetCompressedSize(u32 *p) {
    return *p >> 8;
}
