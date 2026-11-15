#include <nitro/types.h>

extern u8 *data_ov001_0231f5bc;
void func_02032d2c(void *p);
void func_ov001_02308430(s32 a);
void func_ov001_023081d8(s32 a);

void func_ov001_02308fc8(void) {
    u8 *g = data_ov001_0231f5bc;
    if (*(s32 *)(g + 0xbc8) >= 1 && *(s32 *)(g + 0xbc8) < 0x78) {
        func_02032d2c(g + 0x7c8);
    } else if (*(s32 *)(g + 0xbc8) >= 0xf0 && *(s32 *)(g + 0xbc8) < 0x168) {
        func_02032d2c(g + 0x780);
    }
    func_ov001_02308430(*(s32 *)(g + 0xbc8) - 1);
    func_ov001_023081d8(*(s32 *)(g + 0xbc8) - 0xb4);
}
