#include <nitro/types.h>

extern s32 sSysState;
extern u16 sParentPacketSize;
extern u16 sChildPacketSize;
extern u16 gWHMaxChildCount;
extern u16 gWHPacketSize;
extern s32 sSendBufferSize;
extern s32 sConnectMode;
extern s32 sRecvBufferSize;
extern s32 data_022c1994;
extern void (*sTraceFunc)(const char *fmt, ...);
extern u8 data_022c1ae0[];
extern char data_0207871c[];
extern char data_02078730[];

void OS_Terminate(void);
void WH_ChangeSysState(s32 a);
void WH_Alloc(void);
void WFS_InitChild(s32 a, s32 b, void (*cb)(void), s32 d);
void WFS_SetDebugMode(s32 a);
void MI_CpuCopy8(const void *src, void *dst, u32 size);
void DC_FlushRange(void *p, u32 size);
BOOL WH_StateInSetChildWEPKey(void);
BOOL WH_StateInStartChild(void);

BOOL WH_ChildConnect(s32 mode, void *bss) {
    s32 x;
    s32 y;

    if (sSysState != 1)
        OS_Terminate();
    if (mode == 7) {
        x = (sParentPacketSize + 0x23) & ~0x1f;
        y = (sChildPacketSize + 0x21) & ~0x1f;
        if (x <= y)
            x = y;
        sSendBufferSize = x;
        x = (((sChildPacketSize + 0xe) * gWHMaxChildCount + 0x29) & ~0x1f) * 2;
        y = ((sParentPacketSize + 0x55) & ~0x1f) * 2;
        if (x <= y)
            x = y;
        sRecvBufferSize = x;
    } else {
        sRecvBufferSize = (((u32)(gWHPacketSize * (gWHMaxChildCount + 1) + 4) + 0x55) & ~0x1f) * 2;
        sSendBufferSize = (gWHPacketSize + 0x21) & ~0x1f;
    }
    sConnectMode = mode;
    WH_ChangeSysState(3);
    switch (mode) {
    case 7:
        if (sTraceFunc != NULL)
            sTraceFunc(data_0207871c);
        WFS_InitChild(1, 0, WH_Alloc, 0);
        WFS_SetDebugMode(1);
    case 1:
    case 3:
    case 5:
        MI_CpuCopy8(bss, data_022c1ae0, 0xc0);
        DC_FlushRange(data_022c1ae0, 0xc0);
        if (data_022c1994 != 0)
            return WH_StateInSetChildWEPKey();
        return WH_StateInStartChild();
    default:
        if (sTraceFunc != NULL)
            sTraceFunc(data_02078730, mode);
        return FALSE;
    }
}
