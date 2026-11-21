#include <nitro/types.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
extern s32 OSi_PreviousVCount;
extern u32 OSi_VFrameCount;
u32 OSi_GetVFrame(s32 v) { u32 e = OS_DisableInterrupts(); if (v < OSi_PreviousVCount) OSi_VFrameCount++; OSi_PreviousVCount = v; OS_RestoreInterrupts(e); return OSi_VFrameCount; }
