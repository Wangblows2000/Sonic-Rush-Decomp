#include <nitro/types.h>

extern void *data_022c567c[][0x30];
extern void *data_022c597c[][0x30];
extern u8 data_022c5674[];
extern u8 data_022c5678[];
extern u8 data_022c5670[];

void func_0206e1f8(void) {
    u8 i;
    u8 j;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < data_022c5674[i]; j++) {
            data_022c597c[i][j] = NULL;
            data_022c567c[i][j] = NULL;
        }
    }
    for (i = 0; i < 4; i++) {
        data_022c5678[i] = 0;
        data_022c5674[i] = 0;
        data_022c5670[i] = 0;
        data_022c5670[i] = 0;
    }
}
