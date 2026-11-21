#include <nitro/types.h>

typedef struct {
    u8 bg0 : 1;
    u8 bg1 : 1;
    u8 bg2 : 1;
    u8 bg3 : 1;
    u8 obj : 1;
    u8 eff : 1;
    u8 pad : 2;
} UnkWinCnt_0205ff2c;

typedef struct {
    u8 w0x2;
    u8 w0x1;
    u8 w1x2;
    u8 w1x1;
    u8 w0y2;
    u8 w0y1;
    u8 w1y2;
    u8 w1y1;
    UnkWinCnt_0205ff2c in0;
    UnkWinCnt_0205ff2c in1;
    UnkWinCnt_0205ff2c out;
    u8 padb;
    s32 unkc;
} UnkWindow_0205ff2c;

typedef struct {
    void (*state)(void *);
    u8 pad4[4];
    u16 unk8;
    u8 pada[0xc];
    u8 unk16;
    u8 unk17;
} UnkTask_0205ff2c;

extern UnkWindow_0205ff2c data_0208923c;

void TalkDemoWindow_State_WipeCenter(void *t);

#pragma thumb on
void TalkDemoWindow_State_InitCenter(UnkTask_0205ff2c *t) {
    UnkWindow_0205ff2c *w;
    u8 z;

    t->unk16 = 0x60;
    t->unk17 = 0x63;
    w = (void *)&data_0208923c;
    z = 0;

    w->w0x1 = 0xff;
    w->w0x2 = z;
    w->w0y1 = t->unk16;
    w->w0y2 = t->unk17;
    w->w1x1 = z;
    w->w1x2 = z;
    w->w1y1 = t->unk16;
    w->w1y2 = t->unk17;
    w->in0.bg0 = 0;
    w->in0.bg1 = 1;
    w->in0.bg2 = 0;
    w->in0.bg3 = 0;
    w->in0.obj = 0;
    w->in0.eff = 1;
    w->in1.bg0 = 0;
    w->in1.bg1 = 1;
    w->in1.bg2 = 0;
    w->in1.bg3 = 0;
    w->in1.obj = 0;
    w->in1.eff = 1;
    w->out.bg0 = 1;
    w->out.bg1 = 1;
    w->out.bg2 = 0;
    w->out.bg3 = 1;
    w->out.obj = 1;
    w->out.eff = 1;
    w->unkc = 3;
    *(vu16 *)0x05000000 = 0;
    t->unk8 = 0;
    t->state = (void (*)(void *))TalkDemoWindow_State_WipeCenter;
}
#pragma thumb reset
