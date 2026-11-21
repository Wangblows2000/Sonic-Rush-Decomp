#include <nitro/types.h>

#pragma thumb on
vu16 *MsgBox_GetBgVOfsReg(u32 screen, u32 bg) {
    switch (bg) {
    case 0:
        switch (screen) {
        case 0:
            return (vu16 *)0x04000012;
        case 1:
            return (vu16 *)0x04001012;
        }
        break;
    case 1:
        switch (screen) {
        case 0:
            return (vu16 *)0x04000016;
        case 1:
            return (vu16 *)0x04001016;
        }
        break;
    case 2:
        switch (screen) {
        case 0:
            return (vu16 *)0x0400001a;
        case 1:
            return (vu16 *)0x0400101a;
        }
        break;
    case 3:
        switch (screen) {
        case 0:
            return (vu16 *)0x0400001e;
        case 1:
            return (vu16 *)0x0400101e;
        }
        break;
    }
    return (vu16 *)0x04000012;
}
#pragma thumb reset
