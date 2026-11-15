#include <nitro/types.h>

s32 func_0200e420(u32 a, u32 b, void (*cb)(u32, u32 *), u32 *out);
void func_0200e9f4(u32 v, u32 *p);
u32 func_0200e9fc(void);
s32 func_0200e3e4(u32 a, u32 b) { u32 r; s32 res = func_0200e420(a, b, func_0200e9f4, &r); if (res != 0) return res; func_0200e9fc(); return r; }
