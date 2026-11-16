#include <nitro/types.h>

typedef struct { u8 pad[0x2c]; u8 x2c; u8 pad2[0xf]; u8 x3c; } UnkStruct_0201b7a4;
void SND_SetPlayerVolume(u32 a, s32 v);
void SND_StopSeq(u32 a);
void ShutdownPlayer(UnkStruct_0201b7a4 *p);
void ForceStopSeq(UnkStruct_0201b7a4 *p) { if (p->x2c == 2) SND_SetPlayerVolume(p->x3c, -723); SND_StopSeq(p->x3c); ShutdownPlayer(p); }
