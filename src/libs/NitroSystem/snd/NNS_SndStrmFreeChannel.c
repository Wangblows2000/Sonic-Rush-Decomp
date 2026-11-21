#include <nitro/types.h>

void NNS_SndUnlockChannel(void *p);

typedef struct { u8 unk0[0x44]; void *unk44; u32 unk48; } UnkStruct_0201c6b4;

void NNS_SndStrmFreeChannel(UnkStruct_0201c6b4 *s) {
    if (s->unk44 == NULL) return;
    NNS_SndUnlockChannel(s->unk44);
    s->unk44 = NULL;
    s->unk48 = 0;
}
