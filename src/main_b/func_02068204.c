#include <nitro/types.h>

typedef struct { u8 unk0[0x10]; s32 x; s32 y; } UnkPos;
s32 func_02068194(s32 x0, s32 y0, s32 x1, s32 y1);
void func_0201be98(void *a, s32 v);
s32 func_020681e0(s32 a, s32 b);
void func_0201bdb0(void *a, s32 b, s32 c);
void func_02068204(void *snd, UnkPos *a, UnkPos *b) { func_0201be98(snd, func_02068194(a->x, a->y, b->x, b->y)); func_0201bdb0(snd, 0xffff, func_020681e0(a->x, b->x)); }
