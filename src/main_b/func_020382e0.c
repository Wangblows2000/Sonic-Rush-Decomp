#include <nitro/types.h>

typedef struct { s32 m[9]; } UnkMtx_020382e0;
typedef struct { s32 m[9]; } UnkOut_020382e0;
void func_020382e0(const UnkMtx_020382e0 *a, UnkOut_020382e0 *o) { o->m[0] = a->m[0]; o->m[1] = a->m[3]; o->m[2] = a->m[6]; o->m[3] = 0; o->m[4] = 0x1000; o->m[5] = 0; o->m[6] = a->m[2]; o->m[7] = a->m[5]; o->m[8] = a->m[8]; }
