#include <nitro/types.h>

typedef struct {
    u8 pad0[0x8870];
    u32 arr[16];
} UnkCtx_0203c27c;

extern s32 data_022b8bc0;
extern UnkCtx_0203c27c data_022b8be0;

u32 WFS_GetCurrentBitmap(void);

u32 WFS_GetSentFileCount(void) {
    u32 min = 0xffffffff;
    u32 m;
    s32 i;

    if (data_022b8bc0 == 0 || *(s32 *)&data_022b8be0 == 0)
        return 0;
    m = WFS_GetCurrentBitmap();
    for (i = 1; i < 16; i++) {
        if (m & (1 << i)) {
            if (min > data_022b8be0.arr[i])
                min = data_022b8be0.arr[i];
        }
    }
    return min;
}
