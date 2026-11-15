#include <nitro/types.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
extern void *data_0207d3fc;
extern u32 data_0207d3f8;
extern u32 data_0207d414;
u32 func_020091ac(void) { u32 r; u32 e = OS_DisableInterrupts(); if (data_0207d3fc == 0) r = data_0207d3f8; else r = data_0207d414; OS_RestoreInterrupts(e); return r; }
