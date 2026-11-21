#include <nitro/types.h>

#pragma thumb on
BOOL ZoneMap_Unk_ov001_022e2df0(s32 id) {
    switch (id) {
    case 0:
    case 6:
    case 9:
    case 12:
    case 15:
    case 28:
    case 31:
        return TRUE;
    case 7:
        break;
    }
    return FALSE;
}
#pragma thumb reset
