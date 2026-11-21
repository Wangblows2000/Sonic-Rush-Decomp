#include <nitro/types.h>

u32 PaletteTransfer_GetCompressionType(u32 *p) {
    return *p & 0xF0;
}
