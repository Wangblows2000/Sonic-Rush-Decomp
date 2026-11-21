#include <nitro/types.h>

typedef struct {
    u16 a, b, c, d, e, f, g;
} UnkStruct_022b89e8;

extern UnkStruct_022b89e8 sMbpState;
BOOL MB_CommIsBootable(u16 id);

BOOL MBP_IsBootableAll(void) {
    u16 i;
    if (sMbpState.b == 0)
        return FALSE;
    for (i = 1; i < 16; i++) {
        if ((sMbpState.b & (1 << i)) && MB_CommIsBootable(i) == 0)
            return FALSE;
    }
    return TRUE;
}
