#include <nitro/types.h>

s32 func_02005718(u32 a, void *b, void *c, u32 d);
void func_02005628(void);
void func_02005848(u32 a, void *b, void *c, u32 d) {
    if (func_02005718(a, b, c, d) <= 0) return;
    do { func_02005628(); } while (func_02005718(a, b, c, d) > 0);
}
