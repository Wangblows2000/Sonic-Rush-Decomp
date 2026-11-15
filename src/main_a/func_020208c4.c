#include <nitro/types.h>

extern u8 data_02084d2c[];
u8 *func_020208f0(void *list, void *p);

void *func_020208c4(void *p) {
    u8 *r = data_02084d2c;
    u8 *x = func_020208f0(r, p);
    if (x != NULL) r = x + 0xC;
    return r;
}
