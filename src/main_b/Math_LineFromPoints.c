#include <nitro/types.h>

void VEC_Subtract(const void *a, const void *b, void *ab);
void VEC_Normalize(const void *a, void *b);
s32 VEC_DotProduct(const void *a, const void *b);
void VEC_MultAdd(s32 a, const void *b, const void *c, void *d);
void Math_LineFromPoints(const s32 *p, const s32 *q, s32 *o) { VEC_Subtract(q, p, o); VEC_Normalize(o, o); VEC_MultAdd(-VEC_DotProduct(o, p), o, p, o + 3); }
