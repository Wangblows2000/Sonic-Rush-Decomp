#include <nitro/types.h>

u32 SpecialStageActArchive_GetUncompressedSize(u32 *p) {
    return *p >> 8;
}
