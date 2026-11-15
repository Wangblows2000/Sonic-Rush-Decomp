#include <nitro/types.h>

extern s32 *data_ov001_0231f530;

BOOL func_ov001_02300c68(void) {
    if (data_ov001_0231f530 == NULL) {
        return FALSE;
    }
    return data_ov001_0231f530[1] != 0;
}
