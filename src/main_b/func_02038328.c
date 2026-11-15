#include <nitro/types.h>

typedef struct { s32 m[9]; } UnkMtx_02038328;
void func_02038328(const UnkMtx_02038328 *a, UnkMtx_02038328 *o) { o->m[0] = a->m[0]; o->m[1] = a->m[3]; o->m[2] = a->m[6]; o->m[3] = a->m[1]; o->m[4] = a->m[4]; o->m[5] = a->m[7]; o->m[6] = a->m[2]; o->m[7] = a->m[5]; o->m[8] = a->m[8]; }
