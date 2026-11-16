#include <nitro/types.h>

#pragma thumb on
BOOL ZoneMap_Unk_ov001_022e2e40(s32 id) {
    switch (id) {
    case 3:
    case 0xb:
    case 0xd:
    case 0xf:
    case 0x11:
    case 0x13:
    case 0x15:
    case 0x17:
    case 0x19:
    case 0x1b:
    case 0x1d:
    case 0x20:
    case 0x21:
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset
