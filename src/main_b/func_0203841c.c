#include <nitro/types.h>

s32 func_020026f8(void *a, void *b);
void func_02002300(s32 a, void *b, void *c, void *d);
void func_0203841c(u8 *s, void *b, void *c) { s32 x = func_020026f8(s, b); func_02002300(x - func_020026f8(s, s + 0xc), s, s + 0xc, c); }
