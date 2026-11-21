#include <nitro/types.h>

typedef struct { u8 pad[0x40]; u8 a; u8 b; } UnkStruct_0201be88;
void NNS_SndPlayerSetInitialVolume(UnkStruct_0201be88 **pp, u8 v) { UnkStruct_0201be88 *p = *pp; if (p != 0) p->a = v; }
