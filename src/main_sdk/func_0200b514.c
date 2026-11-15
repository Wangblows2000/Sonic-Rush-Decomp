#include <nitro/types.h>

typedef struct { u8 pad[8]; u32 x8; u8 pad2[4]; u32 x10; u32 x14; } UnkStruct_0200b514;
u32 func_0200ad14(UnkStruct_0200b514 *p, u32 a);
void func_0200aea4(UnkStruct_0200b514 *p, u32 r);
BOOL func_0200b600(u32 a);
void func_0200b55c(void);
BOOL func_0200b514(UnkStruct_0200b514 *p) { func_0200aea4(p, func_0200ad14(p, p->x10)); if (func_0200b600(p->x8)) func_0200b55c(); return p->x14 == 0; }
