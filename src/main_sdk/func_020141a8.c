#include <nitro/types.h>

typedef struct { u8 pad[0xc]; s32 last; u32 count; u32 *bits; } UnkStruct_020141a8;
void func_020141a8(UnkStruct_020141a8 *p, s32 n) { p->count++; p->last = n; p->bits[n >> 5] |= 1 << (n & 0x1f); }
