#include <nitro/types.h>

typedef struct {
    u8 pad0[0xc];
    u16 unkc;
    u16 unke;
    u16 unk10;
    u16 unk12;
    u16 unk14;
    u16 unk16;
    u16 unk18;
    u8 pad1a[0x18];
    u16 unk32;
    u16 unk34;
    u16 unk36;
} UnkStruct_022c1a00;

extern s32 sSysState;
extern u16 sParentPacketSize;
extern u16 sChildPacketSize;
extern u16 gWHMaxChildCount;
extern u16 gWHPacketSize;
extern s32 sSendBufferSize;
extern s32 sConnectMode;
extern s32 sRecvBufferSize;
extern void (*sTraceFunc)(const char *fmt, ...);
extern UnkStruct_022c1a00 sParentParam;
extern char data_0207874c[];
extern char data_02078764[];

void OS_Terminate(void);
void WH_ChangeSysState(s32 a);
u16 WM_GetDispersionBeaconPeriod(void);
void WH_Alloc(void);
void WFS_InitParent(s32 a, s32 b, void (*cb)(void), s32 d, u16 e, s32 f, s32 g);
void WFS_SetDebugMode(s32 a);
void WFS_EnableSync(s32 a);
BOOL WH_StateInSetParentParam(void);

BOOL WH_ParentConnect(s32 mode, u16 a, u16 b) {
    s32 x;
    s32 y;

    if (sSysState != 1)
        OS_Terminate();
    if (mode == 6) {
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
        sRecvBufferSize = (((gWHPacketSize + 0xe) * gWHMaxChildCount + 0x29) & ~0x1f) * 2;
        sSendBufferSize = (gWHPacketSize * (gWHMaxChildCount + 1) + 0x27) & ~0x1f;
    }
    sConnectMode = mode;
    WH_ChangeSysState(3);
    sParentParam.unkc = a;
    sParentParam.unk32 = b;
    sParentParam.unk18 = WM_GetDispersionBeaconPeriod();
    if (mode == 6) {
        sParentParam.unk34 = sParentPacketSize;
        sParentParam.unk36 = sChildPacketSize;
    } else {
        sParentParam.unk34 = gWHPacketSize * (gWHMaxChildCount + 1) + 4;
        sParentParam.unk36 = gWHPacketSize;
    }
    sParentParam.unk10 = gWHMaxChildCount;
    sParentParam.unk16 = (mode == 6) ? 1 : 0;
    sParentParam.unk12 = 0;
    sParentParam.unke = 1;
    sParentParam.unk14 = (mode == 2) ? 1 : 0;
    switch (mode) {
    case 6:
        if (sTraceFunc != NULL)
            sTraceFunc(data_0207874c);
        WFS_InitParent(1, 0, WH_Alloc, 0, sParentPacketSize, 0, 1);
        WFS_SetDebugMode(1);
        WFS_EnableSync(0);
    case 0:
    case 2:
    case 4:
        return WH_StateInSetParentParam();
    default:
        if (sTraceFunc != NULL)
            sTraceFunc(data_02078764, mode);
        return FALSE;
    }
}
