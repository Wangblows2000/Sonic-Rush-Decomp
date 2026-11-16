#include <nitro/types.h>

typedef struct {
    u8 pad0[0x20];
    u16 unk20;
    u16 unk22;
} UnkGfx_02053014;

typedef struct {
    u32 pad0;
    UnkGfx_02053014 *gfx;
} UnkCtx_02053014;

typedef struct {
    u8 pad0[0xdc];
    s32 unkdc;
    u8 pade0[0x1c];
    s32 timer;
} UnkWork_02053014;

extern UnkCtx_02053014 gDisplayTable;

void NaviMsg_State_FadeIn(UnkWork_02053014 *w) {
    s32 v;

    w->timer++;
    if (w->timer >= 30) {
        UnkGfx_02053014 *g = gDisplayTable.gfx;
        g->unk20 = 0x40;
        g->unk22 = 0;
        w->unkdc = 0;
        return;
    }
    v = (w->timer * 31) / 30;
    gDisplayTable.gfx->unk22 = (v & 0x1f) | ((31 - (v & 0x1f)) << 8);
}
