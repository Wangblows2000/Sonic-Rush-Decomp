#include <nitro/types.h>

typedef struct {
    u32 pad0;
    s32 idx;
    u8 pad8[0xd4];
    void (*fndc)(void);
    u8 pade0[0x1c];
    s32 cnt;
} UnkWork_02052e7c;

typedef struct {
    u8 pad0[0x20];
    u16 blendCnt;
    u16 blendAlpha;
} UnkDisp_02052e7c;

extern UnkDisp_02052e7c *data_02072c78[];
extern s32 data_02075664[];
extern u32 data_0207566c[];

void func_02052d90(UnkWork_02052e7c *w, s32 n);
void func_02052af8(UnkWork_02052e7c *w, s32 idx, s32 v);
void func_02036754(u32 *p, u32 a, u32 b);

void func_02052e7c(UnkWork_02052e7c *w) {
    s32 v;

    w->cnt++;
    if (w->cnt >= 60) {
        data_02072c78[1]->blendCnt = 0x40;
        data_02072c78[1]->blendAlpha = 0x1f00;
        w->fndc = NULL;
        func_02052d90(w, 2);
        func_02052af8(w, data_02075664[w->idx], 0xf0);
        func_02036754(0, data_0207566c[w->idx], 9);
    } else {
        v = (w->cnt * 31 / 60) & 0x1f;
        data_02072c78[1]->blendAlpha = v | ((31 - v) << 8);
    }
}
