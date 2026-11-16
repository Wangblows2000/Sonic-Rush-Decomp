#include <nitro/types.h>

extern u8 *data_0224ff44[];
extern u16 data_022c1948;
extern u8 data_0207cfb4[];

void func_0206caf0(void) {
    u16 i;
    u8 *p;

    if (data_0224ff44[0][1] == 0xff)
        return;
    for (i = 0; i < data_022c1948 + 1; i++) {
        p = data_0224ff44[i];
        if (*p != 0x55)
            return;
        if (p[1] != 0xff)
            data_0207cfb4[i] = p[1];
        p[1] = 0xff;
    }
}
