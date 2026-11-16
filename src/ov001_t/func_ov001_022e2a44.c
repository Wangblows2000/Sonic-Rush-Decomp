#include <nitro/types.h>

typedef struct {
    u8 pad0[0xc];
    u16 unkc;
} UnkStruct_022e2a44;

extern UnkStruct_022e2a44 data_022c4680;

#pragma thumb on
s32 func_ov001_022e2a44(void) {
    switch (data_022c4680.unkc) {
    case 2:
        return 0;
    case 3:
    case 4:
        return 5;
    case 5:
        return 6;
    case 7:
    case 8:
        return 11;
    case 9:
        return 12;
    case 11:
    case 12:
        return 17;
    case 13:
        return 18;
    case 14:
        return 19;
    case 15:
    case 16:
        return 22;
    case 17:
        return 23;
    case 18:
        return 24;
    case 19:
    case 20:
        return 27;
    case 21:
        return 28;
    case 22:
    case 23:
        return 30;
    case 24:
        return 31;
    case 25:
        return 33;
    case 26:
        return 34;
    }
    return 0;
}
#pragma thumb reset
