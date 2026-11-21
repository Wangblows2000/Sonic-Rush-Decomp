#include <nitro/types.h>

void NNS_SndStrmFreeChannel(void *p);

typedef struct { u8 unk0[0x11C]; u32 unk11C; } UnkStruct_0201fac8;

void FreeChannel(UnkStruct_0201fac8 *s) {
    if (s->unk11C == 0) return;
    s->unk11C--;
    if (s->unk11C != 0) return;
    NNS_SndStrmFreeChannel(s);
}
