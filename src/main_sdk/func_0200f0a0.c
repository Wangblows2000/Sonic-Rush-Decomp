#include <nitro/types.h>

s32 func_0200f0e4(u32 a, void (*cb)(u32), u32 c);
void func_0200ea7c(u32 v);
void func_0200ea64(void);
typedef struct { u8 pad[0x20]; s32 x20; } UnkStruct_0207f0e4;
extern UnkStruct_0207f0e4 data_0207f0e4;
s32 func_0200f0a0(u32 a) { s32 r = func_0200f0e4(a, func_0200ea7c, 0); data_0207f0e4.x20 = r; if (r == 0) func_0200ea64(); return data_0207f0e4.x20; }
