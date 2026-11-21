#include <nitro/types.h>

typedef struct {
    u8 pad0[0x24];
    u16 n;
} UnkStruct_022e22d0;

extern UnkStruct_022e22d0 data_022c46c0;

void SaveData_SetBlazeProgress(s32 id);

#pragma thumb on
void ZoneMap_SetBlazeProgressFromStep(u32 a) {
    s32 n = data_022c46c0.n;
    s32 id;

    if (n == 2 || n == 6 || n == 9 || n == 13 || n == 17 || n == 21 || n == 25) return;
    switch (a) {
    case 0:
        id = 0;
        break;
    case 3:
        id = 1;
        break;
    case 4:
        id = 3;
        break;
    case 11:
        id = 5;
        break;
    case 12:
        id = 7;
        break;
    case 13:
        id = 8;
        break;
    case 14:
        id = 10;
        break;
    case 15:
        id = 11;
        break;
    case 17:
        id = 12;
        break;
    case 18:
        id = 14;
        break;
    case 19:
        id = 15;
        break;
    case 21:
        id = 16;
        break;
    case 22:
        id = 18;
        break;
    case 23:
        id = 19;
        break;
    case 25:
        id = 20;
        break;
    case 26:
        id = 22;
        break;
    case 27:
        id = 23;
        break;
    case 29:
        id = 24;
        break;
    case 30:
        id = 26;
        break;
    case 32:
        id = 27;
        break;
    case 33:
        id = 28;
        break;
    default:
        return;
    }
    SaveData_SetBlazeProgress(id);
}
#pragma thumb reset
