#include <nitro/types.h>

extern u8 *data_ov001_0231f5bc;
void func_02032d2c(void *p);
void func_02032480(void *p, s32 a, s32 b, s32 c);

void func_ov001_023091a0(void) {
    u8 *g = data_ov001_0231f5bc;
    if (*(s32 *)(g + 0xbc4) >= 0x96 && *(s32 *)(g + 0xbc4) < 0x118) {
        func_02032d2c(g + 0x780);
        return;
    }
    if (*(s32 *)(g + 0xbc4) < 0x118) {
        return;
    }
    if (*(s32 *)(g + 0xbc4) > 0x168) {
        return;
    }
    func_02032480(g + 0x7c8, *(s32 *)(g + 0xbc0), *(s32 *)(g + 0xbc0), 0);
}
