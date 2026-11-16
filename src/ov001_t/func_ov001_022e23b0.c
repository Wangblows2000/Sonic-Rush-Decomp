#include <nitro/types.h>

typedef struct {
    u8 pad0[0xc];
    u16 n;
} UnkStruct_022e23b0;

extern UnkStruct_022e23b0 gSaveData;

void func_02059bdc(s32 id);

#pragma thumb on
void func_ov001_022e23b0(u32 a) {
    s32 n = gSaveData.n;
    s32 id;

    if (n == 1 || n == 4 || n == 8 || n == 12 || n == 16 || n == 20 || n == 23) return;
    switch (a) {
    case 0:
        id = 2;
        break;
    case 5:
        id = 3;
        break;
    case 6:
        id = 5;
        break;
    case 11:
        id = 7;
        break;
    case 12:
        id = 9;
        break;
    case 17:
        id = 11;
        break;
    case 18:
        id = 13;
        break;
    case 19:
        id = 14;
        break;
    case 22:
        id = 15;
        break;
    case 23:
        id = 17;
        break;
    case 24:
        id = 18;
        break;
    case 27:
        id = 19;
        break;
    case 28:
        id = 21;
        break;
    case 30:
        id = 22;
        break;
    case 31:
        id = 24;
        break;
    case 33:
        id = 25;
        break;
    case 34:
        id = 26;
        break;
    default:
        return;
    }
    func_02059bdc(id);
}
#pragma thumb reset
