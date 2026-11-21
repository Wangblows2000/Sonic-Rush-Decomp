#include <nitro/types.h>

typedef struct { u8 pad[0x3c]; u8 h; } UnkStruct_0201bde0;
void SND_SetTrackPitch(u32 a, u32 b, u32 c);
void NNS_SndPlayerSetTrackPitch(UnkStruct_0201bde0 **pp, u32 b, u32 c) { if (*pp == 0) return; SND_SetTrackPitch((*pp)->h, b, c); }
