#include <nitro/types.h>

s32 VEC_DotProduct(void *a, void *b);
void VEC_MultAdd(s32 a, void *b, void *c, void *d);
void func_0203841c(u8 *s, void *b, void *c) { s32 x = VEC_DotProduct(s, b); VEC_MultAdd(x - VEC_DotProduct(s, s + 0xc), s, s + 0xc, c); }
