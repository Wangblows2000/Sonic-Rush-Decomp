#include <nitro/types.h>

extern s32 data_022c4560;
extern u8 data_ov001_023166c4[];
extern u8 data_ov001_023166bc[];

void func_020521b4(s32 mode);
void func_02052424(s32 a, s32 b);
void func_ov001_022e9664(s32 mode);

#pragma thumb on
void func_ov001_022e96a8(s32 mode, s32 id, s32 b) {
    switch (mode) {
    case 0:
    case 1:
        func_020521b4(mode);
        if (id < 7) {
            if (data_022c4560 == 0) {
                id = data_ov001_023166c4[id];
            } else {
                id = data_ov001_023166bc[id];
            }
        }
        func_02052424(id, b);
        return;
    case 2:
    case 3:
    default:
        func_ov001_022e9664(mode);
        break;
    }
}
#pragma thumb reset
