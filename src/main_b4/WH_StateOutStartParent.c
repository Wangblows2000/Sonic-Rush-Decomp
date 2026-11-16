#include <nitro/types.h>

typedef struct {
    u16 unk0;
    u16 unk2;
    u8 pad4[4];
    u16 unk8;
    u8 pada[6];
    u16 unk10;
} UnkRes_02040048;

extern void (*sTraceFunc)(const char *fmt, ...);
extern BOOL (*data_022c1978)(UnkRes_02040048 *r);
extern s32 sConnectMode;
extern u16 sConnectBitmap;
extern char data_02078a60[];
extern char data_02078a8c[];
extern char data_02078ab8[];

void WH_SetError(s32 r);
void WH_ChangeSysState(s32 a);
s32 WM_Disconnect(s32 a, u16 b);
void WH_CallbackForWFS(UnkRes_02040048 *r);
BOOL WH_StateInStartParentMP(void);

void WH_StateOutStartParent(UnkRes_02040048 *r) {
    u16 bit = 1 << r->unk10;
    s32 ret;

    if (r->unk2 != 0) {
        WH_SetError(r->unk2);
        WH_ChangeSysState(9);
        return;
    }
    switch (r->unk8) {
    case 7:
        if (sTraceFunc != NULL)
            sTraceFunc(data_02078a60, r->unk10);
        if (data_022c1978 != NULL && !data_022c1978(r)) {
            ret = WM_Disconnect(0, r->unk10);
            if (ret == 2)
                return;
            WH_SetError(ret);
            WH_ChangeSysState(9);
            return;
        }
        if (sConnectMode == 6 || sConnectMode == 7)
            WH_CallbackForWFS(r);
        sConnectBitmap |= bit;
        return;
    case 9:
        if (sTraceFunc != NULL)
            sTraceFunc(data_02078a8c, r->unk10);
        sConnectBitmap &= ~bit;
        WH_CallbackForWFS(r);
        return;
    case 0:
        if (!WH_StateInStartParentMP())
            WH_ChangeSysState(9);
        return;
    case 2:
        return;
    default:
        if (sTraceFunc != NULL)
            sTraceFunc(data_02078ab8, r->unk8);
        return;
    }
}
