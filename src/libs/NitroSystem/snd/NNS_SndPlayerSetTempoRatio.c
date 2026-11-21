#include <nitro/types.h>

typedef struct { u8 pad[0x3c]; u8 h; } UnkStruct_0201bd80;
void SND_SetPlayerTempoRatio(u32 a, u32 b);
void NNS_SndPlayerSetTempoRatio(UnkStruct_0201bd80 **pp, u32 b) { if (*pp == 0) return; SND_SetPlayerTempoRatio((*pp)->h, b); }
