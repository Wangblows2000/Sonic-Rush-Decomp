#include <nitro/types.h>

extern u16 gSaveState;
void func_0200f24c(u16 id);
s32 func_0200f8b8(u32 a);
void func_0200f23c(u16 id);
s32 SaveData_IdentifyBackup(void) { u16 id = gSaveState; s32 r; func_0200f24c(id); r = func_0200f8b8(0xd01); func_0200f23c(id); return r; }
