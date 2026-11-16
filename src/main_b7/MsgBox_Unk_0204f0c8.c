#include <nitro/types.h>

#pragma thumb on
u32 MsgBox_Unk_0204f0c8(u32 screen, u32 bg) {
    switch (bg) {
    case 0:
        switch (screen) {
        case 0:
            return *(vu16 *)0x04000008 & 3;
        case 1:
            return *(vu16 *)0x04001008 & 3;
        }
        break;
    case 1:
        switch (screen) {
        case 0:
            return *(vu16 *)0x0400000a & 3;
        case 1:
            return *(vu16 *)0x0400100a & 3;
        }
        break;
    case 2:
        switch (screen) {
        case 0:
            return *(vu16 *)0x0400000c & 3;
        case 1:
            return *(vu16 *)0x0400100c & 3;
        }
        break;
    case 3:
        switch (screen) {
        case 0:
            return *(vu16 *)0x0400000e & 3;
        case 1:
            return *(vu16 *)0x0400100e & 3;
        }
        break;
    }
    return 0;
}
#pragma thumb reset
