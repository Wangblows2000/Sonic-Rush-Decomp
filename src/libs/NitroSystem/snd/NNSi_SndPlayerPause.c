#include <nitro/types.h>

typedef struct { u8 pad[0x2e]; u8 x2e; u8 pad2[0xd]; u8 x3c; } UnkStruct_0201b944;
void SND_PauseSeq(u32 a, u32 b);
void NNSi_SndPlayerPause(UnkStruct_0201b944 *p, u32 v) { if (p == 0) return; if (v == p->x2e) return; SND_PauseSeq(p->x3c, v); p->x2e = v; }
