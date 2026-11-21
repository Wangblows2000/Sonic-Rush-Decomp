#include <nitro/types.h>

typedef struct {
    u8 pad0[0x20];
    u16 blendCnt;
    u16 blendAlpha;
} UnkDisp_02052f4c;

typedef struct {
    u8 pad0[0xd4];
    u32 flags;
    u8 padd8[4];
    s32 unkdc;
    u8 pade0[0x1c];
    s32 cnt;
} UnkWork_02052f4c;

extern UnkDisp_02052f4c *gDisplayTable[];

#define REG_DB_DISPCNT (*(vu32 *)0x04001000)

void NaviMsg_State_FadeOut(UnkWork_02052f4c *w) {
    u32 plane;
    s32 v;

    w->cnt++;
    if (w->cnt >= 30) {
        gDisplayTable[1]->blendCnt = 0x40;
        gDisplayTable[1]->blendAlpha = 0;
        plane = (REG_DB_DISPCNT & 0x1f00) >> 8;
        REG_DB_DISPCNT = (REG_DB_DISPCNT & ~0x1f00) | ((plane & ~2) << 8);
        w->flags &= ~1;
        w->unkdc = 0;
    } else {
        v = (w->cnt * 31 / 30) & 0x1f;
        gDisplayTable[1]->blendAlpha = (31 - v) | (v << 8);
    }
}
