#include <nitro/types.h>

#pragma thumb on
BOOL func_ov001_022e2dcc(s32 x) {
    switch (x) {
    case 1:
    case 4:
    case 7:
    case 0x1e:
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset
