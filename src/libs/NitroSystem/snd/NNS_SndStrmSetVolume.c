#include <nitro/types.h>

typedef struct { u32 unk0; s32 unk4; } UnkEntry_0201c3e4;
extern UnkEntry_0201c3e4 data_02083d74[];
s32 SND_CalcChannelVolume(s32 db);
void SND_SetChannelVolume(u32 mask, u32 vol, s32 shift);

typedef struct { u8 unk0[0x3C]; s32 unk3C; u8 unk40[8]; s32 count; u8 ids[1]; } UnkStruct_0201c3e4;

void NNS_SndStrmSetVolume(UnkStruct_0201c3e4 *s, s32 v) {
    u8 id;
    s32 i;
    s->unk3C = v;
    for (i = 0; i < s->count; i++) {
        id = s->ids[i];
        s32 vol = SND_CalcChannelVolume(s->unk3C + data_02083d74[id].unk4);
        SND_SetChannelVolume(1 << id, vol & 0xFF, vol >> 8);
    }
}
