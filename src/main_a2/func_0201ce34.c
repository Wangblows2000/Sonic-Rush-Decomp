#include <nitro/types.h>

typedef struct { s32 origin; s32 target; s32 counter; s32 frame; } NNSSndFader;

typedef struct {
    u32 unk0;
    s32 type;
    u8 unk8[0x1C];
    s32 outCh;
    u8 unk28[0x14];
    NNSSndFader fader;
    s32 unk4C;
    s32 volume;
} UnkStruct_0201ce34;

extern UnkStruct_0201ce34 data_02083e10;

void NNSi_SndFaderUpdate(NNSSndFader *f);
BOOL NNSi_SndFaderIsFinished(NNSSndFader *f);
s32 func_02020564(NNSSndFader *f);
void func_0201c9d0(void);
void func_02008b64(u32 mask, s32 vol, s32 shift);

void func_0201ce34(void) {
    UnkStruct_0201ce34 *p = &data_02083e10;
    NNSSndFader *f;
    s32 vol;
    if (p->unk0 == 0) return;
    if (p->type != 0) return;
    f = &p->fader;
    NNSi_SndFaderUpdate(f);
    if (p->unk4C && NNSi_SndFaderIsFinished(f)) {
        func_0201c9d0();
        return;
    }
    vol = func_02020564(f) >> 8;
    if (vol == p->volume) return;
    func_02008b64(p->outCh, vol, 0);
    p->volume = vol;
}
