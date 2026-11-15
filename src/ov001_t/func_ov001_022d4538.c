#include <nitro/types.h>

typedef struct {
    u8 pad0[0xc];
    s32 mode;
    u8 idx;
    u8 pad11[3];
    void *dest;
    u16 *pal;
} UnkSub_022d4538;

typedef struct {
    s32 timer;
    UnkSub_022d4538 *sub;
    s32 duration;
    s32 offset;
    s32 count;
    u16 palA[0x10];
    u16 palB[0x10];
    u16 palC[0x10];
    u16 out[0x10];
} UnkWork_022d4538;

typedef struct {
    u8 pad[0x10];
    UnkWork_022d4538 *work;
} UnkTask_022d4538;

typedef struct {
    u16 x;
    u16 y;
} UnkScroll_022d4538;

extern UnkTask_022d4538 *data_022b4574;
extern UnkScroll_022d4538 data_0208922c[];
extern UnkScroll_022d4538 data_02087d04[];

void func_02035704(u16 *src, u16 count, void *dest, u16 *pal);

#pragma thumb on
void func_ov001_022d4538(void) {
    UnkWork_022d4538 *w = data_022b4574->work;
    s32 i;
    s32 half, q, range, t, pos;
    BOOL dir, flag;
    s32 b;
    u16 *src, *dst;

    if (w->sub->mode == 0) {
        data_0208922c[w->sub->idx].x += 2;
    } else {
        data_02087d04[w->sub->idx].x += 2;
    }
    half = w->duration / 2;
    q = half * 12 / 16;
    range = half - q;
    t = w->timer;
    if (t >= half) {
        t -= half;
        dir = FALSE;
    } else {
        dir = TRUE;
    }
    if (t < q) {
        pos = 0;
    } else {
        pos = t - q;
    }
    if (pos < range / 2) {
        flag = TRUE;
        range = range / 2;
    } else {
        flag = FALSE;
        pos -= range / 2;
        range -= range / 2;
    }
    if (dir) {
        if (flag) {
            src = w->palA;
            dst = w->palC;
        } else {
            src = w->palC;
            dst = w->palB;
        }
    } else {
        if (flag) {
            src = w->palB;
            dst = w->palC;
        } else {
            src = w->palC;
            dst = w->palA;
        }
    }
    for (i = 0; i < w->count; i++) {
        s32 r, g;
        u16 d;
        r = *src & 0x1f;
        g = (*src >> 5) & 0x1f;
        b = (*src >> 10) & 0x1f;
        d = *dst;
        r += pos * ((d & 0x1f) - r) / range;
        g += pos * (((d >> 5) & 0x1f) - g) / range;
        b += pos * (((d >> 10) & 0x1f) - b) / range;
        w->out[i] = (r & 0x1f) | ((g & 0x1f) << 5) | ((b & 0x1f) << 10);
        src++;
        dst++;
    }
    func_02035704(w->out, w->count, w->sub->dest, w->sub->pal + w->offset);
    w->timer++;
    if (w->timer >= w->duration) {
        w->timer = 0;
    }
}
#pragma thumb reset
