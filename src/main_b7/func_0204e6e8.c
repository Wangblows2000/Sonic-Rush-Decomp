#include <nitro/types.h>

typedef struct {
    s32 unk0;
} UnkStruct_022c4560;

extern UnkStruct_022c4560 gGameState;
extern u8 data_02074bc4[];
extern u8 data_02074bd0[];

u8 func_0204e6e8(u8 v) {
    const u8 *tbl;
    u8 i;
    u8 n;
    if (gGameState.unk0 != 1) {
        tbl = data_02074bc4;
        n = 9;
    } else {
        n = 9;
        tbl = data_02074bd0;
    }
    for (i = 0; i < n; i++) {
        if (v == tbl[i])
            return i;
    }
    return 0xff;
}
