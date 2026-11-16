#include <nitro/types.h>

typedef struct {
    u8 pad0[0x24];
    u16 unk24;
} UnkStruct_022e299c;

extern UnkStruct_022e299c data_022c46c0;

#pragma thumb on
s32 func_ov001_022e299c(void) {
    switch (data_022c46c0.unk24) {
    case 0:
        return 0;
    case 1:
    case 2:
        return 3;
    case 3:
        return 4;
    case 5:
    case 6:
        return 11;
    case 7:
        return 12;
    case 8:
    case 9:
        return 13;
    case 10:
        return 14;
    case 11:
        return 15;
    case 12:
    case 13:
        return 17;
    case 14:
        return 18;
    case 15:
        return 19;
    case 16:
    case 17:
        return 21;
    case 18:
        return 22;
    case 19:
        return 23;
    case 20:
    case 21:
        return 25;
    case 22:
        return 26;
    case 23:
        return 27;
    case 24:
    case 25:
        return 29;
    case 26:
        return 30;
    case 27:
        return 32;
    case 28:
        return 33;
    }
    return 0;
}
#pragma thumb reset
