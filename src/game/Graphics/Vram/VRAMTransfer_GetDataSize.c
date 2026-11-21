#include <nitro/types.h>

u32 VRAMTransfer_GetDataSize(u32 *p) {
    return *p >> 8;
}
