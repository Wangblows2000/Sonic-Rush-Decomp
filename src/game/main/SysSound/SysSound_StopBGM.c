#include <nitro/types.h>

#pragma thumb on

extern u8 *gSysSound;
void NNS_SndPlayerStopSeq(void *p, s32 n);
void SysSound_StopBGM(s32 n) { NNS_SndPlayerStopSeq(gSysSound + 0x14, n); }

#pragma thumb reset
