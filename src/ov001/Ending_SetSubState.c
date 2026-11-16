#include <nitro/types.h>

void Ending_SetSubState(u8 *p, s32 v) {
    *(s32 *)(p + 8) = v;
    *(s32 *)(p + 0xc) = 0;
}
