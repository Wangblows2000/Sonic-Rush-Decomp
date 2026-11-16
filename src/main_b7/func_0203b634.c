#include <nitro/types.h>

typedef struct {
    u16 a, b, c, d, e, f, g;
} UnkStruct_022b89e8;

extern UnkStruct_022b89e8 data_022b89e8;
BOOL func_02016a74(u16 id);

BOOL func_0203b634(void) {
    u16 i;
    if (data_022b89e8.b == 0)
        return FALSE;
    for (i = 1; i < 16; i++) {
        if ((data_022b89e8.b & (1 << i)) && func_02016a74(i) == 0)
            return FALSE;
    }
    return TRUE;
}
