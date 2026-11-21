#include <nitro/types.h>

u32 StageLoader_GetUncompressedSize(u32 *p) {
    return *p >> 8;
}
