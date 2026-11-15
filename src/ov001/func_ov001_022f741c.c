#include <nitro/types.h>

extern u8 *data_ov001_0231f084;
void func_02032d2c(void *p);

void func_ov001_022f741c(void) {
    u8 *q = data_ov001_0231f084 + 0xb10;
    *(u16 *)(q + 4) = 0x50;
    *(u16 *)(q + 6) = 0x8a;
    func_02032d2c(q);
}
