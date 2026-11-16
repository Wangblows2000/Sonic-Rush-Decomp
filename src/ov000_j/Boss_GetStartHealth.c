#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_022c4560;

extern UnkStruct_022c4560 gGameState;

s32 Boss_GetStartHealth(void) {
    BOOL b;
    s32 idx;

    b = gGameState.unk10 == 2 || gGameState.unk10 == 3;
    if (b)
        idx = 1;
    else
        idx = gGameState.unk14;
    if (idx == 1)
        return 8;
    return 6;
}
