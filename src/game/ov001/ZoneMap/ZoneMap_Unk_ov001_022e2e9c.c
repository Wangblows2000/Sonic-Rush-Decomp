#include <nitro/types.h>

#pragma thumb on
BOOL ZoneMap_Unk_ov001_022e2e9c(s32 id) {
    switch (id) {
    case 5:
    case 0xb:
    case 0x11:
    case 0x13:
    case 0x16:
    case 0x18:
    case 0x1b:
    case 0x1e:
    case 0x21:
    case 0x22:
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset
