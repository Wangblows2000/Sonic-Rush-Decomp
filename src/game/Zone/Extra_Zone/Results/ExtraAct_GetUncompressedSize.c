#include <nitro/types.h>

u32 ExtraAct_GetUncompressedSize(u32 *p) {
    return *p >> 8;
}
