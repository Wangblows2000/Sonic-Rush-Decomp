#include <nitro/types.h>

typedef struct {
    u8 bg0 : 1;
    u8 bg1 : 1;
    u8 bg2 : 1;
    u8 bg3 : 1;
    u8 obj : 1;
    u8 eff : 1;
    u8 pad : 2;
} UnkWinCnt_0205fad0;

typedef struct {
    u16 win0h;
    u16 win1h;
    u16 win0v;
    u16 win1v;
    UnkWinCnt_0205fad0 in0;
    UnkWinCnt_0205fad0 in1;
} UnkWinRegs_0205fad0;

typedef struct {
    u8 pad0[0xec];
    u16 color[4];
    u8 flag[4];
} UnkPal_0205fad0;

extern UnkPal_0205fad0 *data_022c4d9c;

#define REG_DISPSTAT_ (*(vu16 *)0x04000004)
#define WINREGS ((volatile UnkWinRegs_0205fad0 *)0x04000040)

#pragma thumb on
void TalkDemoWindow_OnVAlarmBackdropWin(u32 i) {
    UnkPal_0205fad0 *p = data_022c4d9c;

    if (p == NULL)
        return;
    while (!(REG_DISPSTAT_ & 2))
        ;
    *(vu16 *)0x05000000 = p->color[i];
    {
        volatile UnkWinRegs_0205fad0 *r = WINREGS;
        r->in0.bg2 = p->flag[i];
        r->in1.bg2 = p->flag[i];
    }
}
#pragma thumb reset
