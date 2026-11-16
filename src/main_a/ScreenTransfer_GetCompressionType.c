#include <nitro/types.h>

u32 ScreenTransfer_GetCompressionType(u32 *p) {
    return *p & 0xF0;
}
