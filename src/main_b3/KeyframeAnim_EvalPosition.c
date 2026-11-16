#include <nitro/types.h>

typedef struct { u32 unk0; s32 frame; u8 pad8[0x18]; } UnkKey_02040b14;
typedef struct { u8 pad0[0x10]; u32 flags; u8 pad14[4]; s32 frame; s32 speed; s32 cur; u8 pad24[4]; u8 *base; } UnkAnim_02040b14;
typedef struct { u32 flags; u8 pad4[4]; s32 count; s32 length; s32 offset; } UnkTrack_02040b14;

void KeyframeAnim_DefaultPosition(void *out);
void KeyframeAnim_LerpPosition(UnkKey_02040b14 *k0, UnkKey_02040b14 *k1, s32 ratio, void *out, u32 flags);
s32 FX_Div(s32 a, s32 b);

#pragma thumb on
void KeyframeAnim_EvalPosition(UnkAnim_02040b14 *a, UnkTrack_02040b14 *tr, void *out) {
    UnkKey_02040b14 *kn;
    s32 n;
    s32 t;
    BOOL rev;
    s32 step;
    s32 i;
    s32 j;
    UnkKey_02040b14 *keys;
    UnkKey_02040b14 *k;
    UnkKey_02040b14 *k0;
    UnkKey_02040b14 *k1;
    s32 frame;
    s32 f0;
    s32 f1;
    s32 len;
    s32 fr;

    n = tr->count;
    if (n == 0) {
        KeyframeAnim_DefaultPosition(out);
        return;
    }
    if (n == 1) {
        keys = (UnkKey_02040b14 *)(a->base + tr->offset);
        KeyframeAnim_LerpPosition(keys, keys, 0, out, a->flags);
        return;
    }
    frame = a->frame;
    t = frame >> 12;
    if ((t << 12) < frame)
        t++;
    if (tr->flags & 1) {
        t = t % tr->length;
    } else if (t < 0) {
        t = 0;
    } else if (t >= tr->length) {
        t = tr->length - 1;
    }
    if (a->flags & 8) {
        if (a->speed < 0)
            rev = FALSE;
        else
            rev = TRUE;
    } else {
        if (a->speed < 0)
            rev = TRUE;
        else
            rev = FALSE;
    }
    if (rev)
        step = -1;
    else
        step = 1;
    i = a->cur;
    if (i >= n)
        i = n - 1;
    if (i < 0) {
        if (rev)
            i = n - 1;
        else
            i = 0;
    }
    k = &((UnkKey_02040b14 *)(a->base + tr->offset))[i];
    if (rev) {
        if (k->frame <= t)
            k = NULL;
    } else {
        if (k->frame > t)
            k = NULL;
    }
    if (k == NULL) {
        if (rev)
            i = n - 1;
        else
            i = 0;
    }
    for (;;) {
        k0 = &((UnkKey_02040b14 *)(a->base + tr->offset))[i];
        j = i + step;
        if (j < 0)
            j = n - 1;
        if (j >= n)
            j = 0;
        k1 = &((UnkKey_02040b14 *)(a->base + tr->offset))[j];
        kn = k1;
        f0 = k0->frame;
        f1 = k1->frame;
        if (rev) {
            if (f0 < f1)
                f0 += tr->length;
            if (f0 >= t && f1 <= t)
                break;
        } else {
            if (f0 > f1)
                f1 += tr->length;
            if (f0 <= t && f1 >= t)
                break;
        }
        i += step;
        if (i < 0)
            i = n - 1;
        if (i >= n)
            i = 0;
    }
    if (rev) {
        kn = k0;
        k0 = k1;
    }
    len = tr->length << 12;
    fr = frame - (frame / len) * len;
    f0 = k0->frame << 12;
    f1 = kn->frame << 12;
    if (f0 > f1)
        f1 += len;
    if (f0 > fr)
        fr += len;
    if (fr > f1)
        fr = f1;
    if (f0 > fr)
        f0 = fr;
    KeyframeAnim_LerpPosition(k0, kn, FX_Div(fr - f0, f1 - f0), out, *(u32 *)&a->flags);
    a->cur = i;
}
#pragma thumb reset
