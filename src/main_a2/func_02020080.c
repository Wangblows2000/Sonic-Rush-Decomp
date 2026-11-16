#include <nitro/types.h>
#include <main_a2/strm.h>

extern UnkStrm data_020847b8[];
extern s16 data_020725e0[];

void func_0201fbc0(UnkStrm *s);
void func_0201c488(UnkStrm *s);
void NNSi_SndFaderUpdate(NNSSndFader *f);
BOOL NNSi_SndFaderIsFinished(NNSSndFader *f);
s32 func_02020564(NNSSndFader *f);
void NNS_SndStrmSetVolume(UnkStrm *s, s32 vol);

void func_02020080(void) {
    s32 i;
    s32 base;
    s32 vol;
    UnkStrm *s;
    s = data_020847b8;
    for (i = 0; i < 4; i++, s++) {
        if (!s->unk10C_0) continue;
        if (s->unk110 == 0) {
            func_0201fbc0(s);
            continue;
        }
        if (s->unk10C_2 && s->unk114 != 0) {
            func_0201c488(s);
            s->unk10C_1 = 1;
            s->unk10C_2 = 0;
        }
        if (s->unk10C_1) {
            NNSi_SndFaderUpdate(&s->fader);
            base = data_020725e0[s->unk150];
            vol = data_020725e0[func_02020564(&s->fader) >> 8];
            vol += base;
            if (vol != s->unk154) {
                NNS_SndStrmSetVolume(s, vol);
                s->unk154 = vol;
            }
            if (s->unk10C_3 && NNSi_SndFaderIsFinished(&s->fader)) {
                func_0201fbc0(s);
            }
        }
    }
}
