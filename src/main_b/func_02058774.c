#include <nitro/types.h>

extern u16 data_022c4994;
extern u8 data_02076b3c;
void func_0200f24c(u16 id);
s32 func_0200f9a0(s32 a, void *b, s32 c, s32 d, s32 e, s32 f);
void func_0200f23c(u16 id);
BOOL func_02058774(void) { u16 id = data_022c4994; s32 r; func_0200f24c(id); r = func_0200f9a0(0, &data_02076b3c, 12, 0, 0, 0); func_0200f23c(id); return r != 0; }
