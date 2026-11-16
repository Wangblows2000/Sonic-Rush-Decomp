#include <nitro/types.h>

extern u8 data_022c5678[];
extern void *data_022c597c[][48];

void *func_0206de54(u32 mask, s16 idx) {
    s16 i;
    u8 *cnt = data_022c5678;

    for (i = 0; i < 4; i++, cnt++) {
        if (mask & (1 << i)) {
            if (idx < *cnt)
                return data_022c597c[i][idx];
            idx -= *cnt;
        }
    }
    return NULL;
}
