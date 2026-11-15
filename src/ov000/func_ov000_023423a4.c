#include <nitro/types.h>

extern s32 data_ov000_0236abc4[];

BOOL func_ov000_023423a4(s32 unused, s32 v, s32 i) {
    data_ov000_0236abc4[i] = v;
    return TRUE;
}
