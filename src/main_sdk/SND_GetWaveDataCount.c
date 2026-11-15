#include <nitro/types.h>

typedef struct { u8 pad[0x38]; u32 count; } SNDWaveArc;
u32 SND_GetWaveDataCount(const SNDWaveArc *w) { return w->count; }
