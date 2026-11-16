#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
} UnkCfg_02343228;

extern UnkCfg_02343228 data_022c4560;
void Sound_PlaySeqArcEx(s32 a, s32 b, s32 c);
void func_ov000_023431c0(u8 *w, s32 a);

#define CLAMP(v, lo, hi) ((v) < (lo) ? (lo) : ((v) > (hi) ? (hi) : (v)))

void func_ov000_02343228(u8 *w, s16 d) {
    s16 old = *(s16 *)(w + 0x3ae);
    s16 t;
    s32 v;

    *(s16 *)(w + 0x3ae) = old + d;
    v = *(s16 *)(w + 0x3ae);
    *(s16 *)(w + 0x3ae) = CLAMP(v, 0, 999);
    *(s16 *)(w + 0x3b0) += d;
    v = *(s16 *)(w + 0x3b0);
    *(s16 *)(w + 0x3b0) = CLAMP(v, 0, 9999);
    Sound_PlaySeqArcEx(0, 0, 4);
    if (data_022c4560.unk10 - 1U <= 1)
        return;
    for (t = 100; t <= 900; t += 100) {
        if (old < t && *(s16 *)(w + 0x3ae) >= t)
            func_ov000_023431c0(w, 1);
    }
}
