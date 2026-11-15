#include <nitro/types.h>

void VEC_Subtract(const void *a, const void *b, void *ab);
void func_020024b4(const void *a, void *b);
s32 func_020026f8(const void *a, const void *b);
void func_02002300(s32 a, const void *b, const void *c, void *d);
void func_02038458(const s32 *p, const s32 *q, s32 *o) { VEC_Subtract(q, p, o); func_020024b4(o, o); func_02002300(-func_020026f8(o, p), o, p, o + 3); }
