#include <nitro/types.h>

u32 SpecialStagePlayerArchive_GetUncompressedSize(u32 *p) {
    return *p >> 8;
}
