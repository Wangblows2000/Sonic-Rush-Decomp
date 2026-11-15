#include <nitro/types.h>

extern s32 data_022c4560;

#pragma thumb on
BOOL func_ov001_022e2ad8(void) {
    switch (data_022c4560) {
    case 0:
        return FALSE;
    case 1:
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset
