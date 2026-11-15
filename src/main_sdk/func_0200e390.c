#include <nitro/types.h>

s32 func_0200e3cc(void (*cb)(u32, u32 *), u32 *out);
void func_0200e9f4(u32 v, u32 *p);
u32 func_0200e9fc(void);
s32 func_0200e390(void) { u32 r; s32 res = func_0200e3cc(func_0200e9f4, &r); if (res != 0) return res; func_0200e9fc(); return r; }
