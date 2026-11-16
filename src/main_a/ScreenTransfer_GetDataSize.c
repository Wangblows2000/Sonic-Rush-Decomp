#include <nitro/types.h>

u32 ScreenTransfer_GetDataSize(u32 *p) {
    return *p >> 8;
}
