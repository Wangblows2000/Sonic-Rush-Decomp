#include <nitro/types.h>

void ForceStopStrm_sndarc_stream(void *s);
void NNSi_SndFaderSet(void *f, s32 target, s32 frame);

typedef struct { u8 unk0[0xE4]; u8 fader[0x28]; u32 unk10C_0 : 1; s32 unk10C_1 : 1; u32 unk10C_2 : 1; u32 unk10C_3 : 1; u32 unk10C_4 : 28; u8 unk110[0x3C]; u32 unk14C; } UnkStruct_0201fc60;

void StopStrm(UnkStruct_0201fc60 *s, s32 frames) {
    if (!s->unk10C_1) {
        ForceStopStrm_sndarc_stream(s);
        return;
    }
    if (frames == 0) {
        ForceStopStrm_sndarc_stream(s);
        return;
    }
    NNSi_SndFaderSet(s->fader, 0, frames);
    s->unk10C_3 = 1;
    s->unk14C = 0;
}
