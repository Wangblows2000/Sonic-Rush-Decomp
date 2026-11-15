#include <nitro/types.h>

typedef struct { u8 pad[8]; u32 x8; u32 xc; u8 pad2[0x1c]; u32 x2c; u16 x30; u16 x32; u32 x34; } UnkStruct_0200ac30;
void func_0200ad14(UnkStruct_0200ac30 *p, u32 a);
void func_0200ac30(UnkStruct_0200ac30 *p, u16 v) { p->xc |= 4; p->x2c = p->x8; p->x34 = 0; p->x32 = 0; p->x30 = v; func_0200ad14(p, 2); }
