#include <nitro/types.h>

u32 VsMenu_GetUncompressedSize(u32 *p) {
    return *p >> 8;
}
