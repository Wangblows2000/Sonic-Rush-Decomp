#include <nitro/types.h>

extern s32 sSysState;
extern void (*sTraceFunc)(const char *s, ...);
extern char data_020786a4[];
extern char data_020786c0[];
extern s32 sConnectMode;

void WH_ChangeSysState(s32 a);
void WH_Reset(void);
BOOL WH_StateInEndChildKeyShare(void);
void WFS_End(void);
BOOL WH_StateInEndChildMP(void);
BOOL WH_StateInEndParentKeyShare(void);
BOOL WH_StateInEndParentMP(void);

void WH_Finalize(void) {
    if (sSysState == 1) {
        if (sTraceFunc != NULL)
            sTraceFunc(data_020786a4);
        return;
    }
    if (sTraceFunc != NULL)
        sTraceFunc(data_020786c0, sSysState);
    if (sSysState != 6 && sSysState != 5 && sSysState != 4) {
        WH_ChangeSysState(3);
        WH_Reset();
        return;
    }
    WH_ChangeSysState(3);
    switch (sConnectMode) {
    case 3:
        if (!WH_StateInEndChildKeyShare())
            WH_Reset();
        break;
    case 7:
        WFS_End();
    case 1:
    case 5:
        if (!WH_StateInEndChildMP())
            WH_Reset();
        break;
    case 2:
        if (!WH_StateInEndParentKeyShare())
            WH_Reset();
        break;
    case 6:
        WFS_End();
    case 0:
    case 4:
        if (!WH_StateInEndParentMP())
            WH_Reset();
        break;
    }
}
