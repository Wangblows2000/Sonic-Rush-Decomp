#include <nitro/types.h>

typedef struct { u32 a; u32 b; } SNDInstPos;
SNDInstPos SND_GetFirstInstDataPos(const void *bank) { SNDInstPos pos; pos.a = 0; pos.b = 0; return pos; }
