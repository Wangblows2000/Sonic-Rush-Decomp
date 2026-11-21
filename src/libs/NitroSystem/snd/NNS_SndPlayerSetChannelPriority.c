#include <nitro/types.h>

typedef struct { u8 pad[0x3c]; u8 h; } UnkStruct_0201be10;
void SND_SetPlayerChannelPriority(u32 a, u32 b);
void NNS_SndPlayerSetChannelPriority(UnkStruct_0201be10 **pp, u32 b) { if (*pp == 0) return; SND_SetPlayerChannelPriority((*pp)->h, b); }
