#include <nitro/types.h>

extern u32 data_02250460;
extern u32 data_02250468[];
extern u8 data_0225045c[];
void func_0201bf1c(void *p);

void func_020365bc(u32 *p) {
    u32 i;
    if (&data_02250460 == p) {
        func_0201bf1c(p);
        return;
    }
    for (i = 0; i < 0x20; i++) {
        if (&data_02250468[i] == p) {
            func_0201bf1c(&data_02250468[i]);
            data_0225045c[i >> 3] &= ~(1 << (i & 7));
            return;
        }
    }
}
