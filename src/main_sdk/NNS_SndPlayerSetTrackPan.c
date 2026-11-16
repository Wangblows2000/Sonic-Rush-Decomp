#include <nitro/types.h>

typedef struct { u8 pad[0x3c]; u8 h; } UnkStruct_0201bdb0;
void SND_SetTrackPan(u32 a, u32 b, u32 c);
void NNS_SndPlayerSetTrackPan(UnkStruct_0201bdb0 **pp, u32 b, u32 c) { if (*pp == 0) return; SND_SetTrackPan((*pp)->h, b, c); }
