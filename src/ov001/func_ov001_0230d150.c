#include <nitro/types.h>

extern u8 *data_ov001_0231f654;

BOOL func_ov001_0230d150(void) {
    return *(u8 **)(data_ov001_0231f654 + 0xa80) == data_ov001_0231f654 + 0xa88;
}
