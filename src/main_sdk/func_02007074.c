#include <nitro/types.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
extern s32 data_0207d38c;
extern u32 data_0207d390;
u32 func_02007074(s32 v) { u32 e = OS_DisableInterrupts(); if (v < data_0207d38c) data_0207d390++; data_0207d38c = v; OS_RestoreInterrupts(e); return data_0207d390; }
