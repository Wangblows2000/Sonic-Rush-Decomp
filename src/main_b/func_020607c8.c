#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x2c]; u8 unk2C[1]; } UnkStruct_020607c8;
void NNS_SndPlayerStopSeq(void *p, s32 n);
void func_020607c8(UnkStruct_020607c8 *s) { NNS_SndPlayerStopSeq(s->unk2C, 0x1e); }

#pragma thumb reset
