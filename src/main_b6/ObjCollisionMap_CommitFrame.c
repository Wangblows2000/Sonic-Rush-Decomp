#include <nitro/types.h>

extern u8 data_022c4de0;
extern u32 data_022c4de4[];
extern u32 data_022c4e64[];
extern u8 data_022c4ddc;

void ObjCollisionMap_CommitFrame(void) {
    u8 i = 0;
    u8 n = data_022c4de0;

    for (; i < n; i++)
        data_022c4e64[i] = data_022c4de4[i];
    for (; i < 32; i++)
        data_022c4de4[i] = 0;
    data_022c4ddc = n;
    data_022c4de0 = 0;
}
