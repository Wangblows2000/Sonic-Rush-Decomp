#include <nitro/types.h>

extern u8 gGameState[];

BOOL func_ov000_022cc6d4(void) {
    return gGameState[0x1c] == 4;
}
