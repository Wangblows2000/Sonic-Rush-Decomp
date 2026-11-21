#include <nitro/types.h>

typedef struct { u8 pad[0x41]; u8 b; } UnkStruct_0201be98;
void NNS_SndPlayerSetVolume(UnkStruct_0201be98 **pp, u8 v) { UnkStruct_0201be98 *p = *pp; if (p != 0) p->b = v; }
