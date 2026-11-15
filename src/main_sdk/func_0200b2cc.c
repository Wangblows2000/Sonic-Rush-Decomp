// cc: 1.2/sp2p3
#include <nitro/types.h>

void MI_CpuFill8(void *dst, u8 v, u32 size);
typedef struct { u8 pad[0xc]; u16 a; u16 b; } UnkStruct_0200b2cc;
void func_0200b2cc(UnkStruct_0200b2cc *p) { MI_CpuFill8(p, 0, 0x50); p->a = 0; p->b = 0; }
