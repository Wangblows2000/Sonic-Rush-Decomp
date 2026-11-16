#include <nitro/types.h>

typedef struct {
    u16 a, b, c, d, e, f, g;
} UnkStruct_022b89e8;

extern UnkStruct_022b89e8 data_022b89e8;
BOOL MB_CommIsBootable(u16 id);

BOOL func_0203b634(void) {
    u16 i;
    if (data_022b89e8.b == 0)
        return FALSE;
    for (i = 1; i < 16; i++) {
        if ((data_022b89e8.b & (1 << i)) && MB_CommIsBootable(i) == 0)
            return FALSE;
    }
    return TRUE;
}
