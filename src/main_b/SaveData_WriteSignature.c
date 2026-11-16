#include <nitro/types.h>

extern u16 gSaveState;
extern u8 gSaveSignature;
void func_0200f24c(u16 id);
s32 func_0200f9a0(s32 a, void *b, s32 c, s32 d, s32 e, s32 f);
void func_0200f23c(u16 id);
BOOL SaveData_WriteSignature(void) { u16 id = gSaveState; s32 r; func_0200f24c(id); r = func_0200f9a0(0, &gSaveSignature, 12, 0, 0, 0); func_0200f23c(id); return r != 0; }
