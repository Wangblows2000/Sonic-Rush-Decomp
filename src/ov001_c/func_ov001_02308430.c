#include <nitro/types.h>

extern u8 *data_ov001_0231f5bc;
u8 *func_ov001_0230c0a4(void);
void func_02032480(void *o, s32 a, s32 b, s32 c);

void func_ov001_02308430(s32 t) {
    u8 *g = data_ov001_0231f5bc;
    s32 v = *(s32 *)(func_ov001_0230c0a4() + 0x148);
    s32 s = 0x1000;

    if (t >= 0 && t <= 0x14)
        s += 1228.8f * (0x14 - t) / 20.0f;
    else if (t > 0x14 && t <= 0x28)
        s += 1228.8f * (0x28 - t) / 20.0f;
    else if (t > 0x28 && t <= 0x3c)
        s += 1228.8f * (0x3c - t) / 20.0f;
    if (v >= 100) {
        func_02032480(g + 0x198, s, s, 0);
        func_02032480(g + 0x1e0, s, s, 0);
        func_02032480(g + 0x228, s, s, 0);
    } else if (v >= 10) {
        func_02032480(g + 0x1e0, s, s, 0);
        func_02032480(g + 0x228, s, s, 0);
    } else {
        func_02032480(g + 0x228, s, s, 0);
    }
}
