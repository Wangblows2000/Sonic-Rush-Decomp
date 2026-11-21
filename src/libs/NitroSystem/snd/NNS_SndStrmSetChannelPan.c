#include <nitro/types.h>

void SND_SetChannelPan(u32 mask, u32 v);

typedef struct { u8 unk0[0x48]; s32 unk48; u8 unk4C[1]; } UnkStruct_0201c3a0;

void NNS_SndStrmSetChannelPan(UnkStruct_0201c3a0 *s, s32 idx, u32 v) {
    if (idx > s->unk48 - 1) return;
    SND_SetChannelPan(1 << s->unk4C[idx], v);
}
