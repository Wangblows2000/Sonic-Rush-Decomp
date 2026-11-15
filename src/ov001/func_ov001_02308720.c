#include <nitro/types.h>

extern u8 *data_ov001_0231f5bc;
u8 *func_ov001_0230c0a4(void);
void func_02032d2c(void *p);

void func_ov001_02308720(void) {
    u8 *g = data_ov001_0231f5bc;
    s32 v = *(s32 *)(func_ov001_0230c0a4() + 0x148);
    if (v >= 100) {
        func_02032d2c(g + 0x198);
        func_02032d2c(g + 0x1e0);
        func_02032d2c(g + 0x228);
    } else if (v >= 10) {
        func_02032d2c(g + 0x1e0);
        func_02032d2c(g + 0x228);
    } else {
        func_02032d2c(g + 0x228);
    }
}
