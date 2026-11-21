#include <nitro/types.h>

typedef struct {
    u8 pad0[0x28];
    fx32 levelL;
    fx32 levelR;
    u8 pad30[0x184 - 0x30];
    s32 histL[4];
    s32 histR[4];
} UnkSoundTest_022c8f30;

static inline fx32 FX_Mul_022c8f30(fx32 a, fx32 b) {
    return (fx32)(((s64)a * b + 0x800) >> 12);
}

#pragma thumb on
#pragma opt_lifetimes off
void SoundTest_SamplingCallback(s16 *l, s16 *r, u32 len, int format, void *arg)
{
    UnkSoundTest_022c8f30 *work = (UnkSoundTest_022c8f30 *)arg;
    fx32 *pL = &work->levelL;
    fx32 *pR = &work->levelR;
    s32 volR;
    s32 volL;
    u32 i;
    s32 j;
    s32 v;
    fx32 fl;
    fx32 fr;

    if (format != 1)
        len >>= 1;
    volR = 0;
    volL = 0;
    for (i = 0; i < len; i++) {
        v = *l;
        if (v < 0)
            v = -v;
        volL += v;
        v = *r;
        if (v < 0)
            v = -v;
        volR += v;
        l++;
        r++;
    }
    volL = (u32)volL / len;
    volR = (u32)volR / len;
    work->histL[3] = work->histL[2];
    work->histL[2] = work->histL[1];
    work->histL[1] = work->histL[0];
    work->histL[0] = volL;
    work->histR[3] = work->histR[2];
    work->histR[2] = work->histR[1];
    work->histR[1] = work->histR[0];
    work->histR[0] = volR;
    for (j = 0; j < 4; j++) {
        if (work->histL[j] == 0)
            volL = 0;
        if (work->histR[j] == 0)
            volR = 0;
    }
    if (volL < 1000)
        volL = 0;
    if (volR < 1000)
        volR = 0;
    if (volL > 16000)
        volL = 16000;
    if (volR > 16000)
        volR = 16000;
    fl = FX_Mul_022c8f30((volL << 12) / 16000, 0x400) + 0x1000;
    fr = FX_Mul_022c8f30((volR << 12) / 16000, 0x400) + 0x1000;
    if (fl > *pL)
        *pL = fl;
    if (fr > *pR)
        *pR = fr;
}
#pragma thumb reset
