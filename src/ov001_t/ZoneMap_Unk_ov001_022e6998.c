#include <nitro/types.h>

#pragma thumb on
s32 ZoneMap_Unk_ov001_022e6998(s32 a, s32 b, s32 c) {
    switch (a) {
    case 0:
        switch (b) {
        case 1:
            if (c == 2) return 0;
            break;
        case 2:
            if (c == 1) return 0;
            if (c == 15) return 9;
            if (c == 3) return 1;
            break;
        case 3:
            if (c == 2) return 1;
            if (c == 6) return 2;
            if (c == 16) return 10;
            break;
        case 6:
            if (c == 3) return 2;
            if (c == 7) return 3;
            break;
        case 8:
            if (c == 7) return 4;
            if (c == 9) return 5;
            break;
        case 12:
            if (c == 9) return 6;
            if (c == 13) return 7;
            break;
        case 13:
            if (c == 12) return 7;
            if (c == 14) return 8;
            break;
        case 14:
            if (c == 13) return 8;
            break;
        case 15:
            if (c == 2) return 9;
            break;
        case 16:
            if (c == 3) return 10;
            break;
        case 7:
            if (c == 6) return 3;
            if (c == 8) return 4;
            break;
        case 9:
            if (c == 8) return 5;
            if (c == 12) return 6;
            break;
        }
        break;
    case 1:
        switch (b) {
        case 1:
            if (c == 0) return 11;
            if (c == 16) return 12;
            break;
        case 2:
            if (c == 16) return 13;
            if (c == 3) return 14;
            break;
        case 3:
            if (c == 2) return 14;
            if (c == 4) return 15;
            if (c == 5) return 15;
            break;
        case 6:
            if (c == 4) return 16;
            if (c == 5) return 16;
            break;
        case 8:
            if (c == 4) return 17;
            if (c == 5) return 17;
            if (c == 11) return 18;
            if (c == 10) return 18;
            break;
        case 12:
            if (c == 11) return 19;
            if (c == 10) return 19;
            break;
        case 13:
            if (c == 11) return 20;
            if (c == 10) return 20;
            if (c == 14) return 21;
            break;
        case 14:
            if (c == 13) return 21;
            break;
        case 16:
            if (c == 1) return 12;
            if (c == 2) return 13;
            break;
        case 0:
            if (c == 1) return 11;
            break;
        case 4:
        case 5:
            if (c == 3) return 15;
            if (c == 6) return 16;
            if (c == 8) return 17;
            break;
        case 10:
        case 11:
            if (c == 8) return 18;
            if (c == 12) return 19;
            if (c == 13) return 20;
            break;
        }
        break;
    }
    return 23;
}
#pragma thumb reset
