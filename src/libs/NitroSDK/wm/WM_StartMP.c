#include <nitro/types.h>
#include <NitroSDK/wm.h>

void DC_InvalidateRange(void *p, u32 size);
s32 WM_GetMPReceiveBufferSize(void);
s32 WM_GetMPSendBufferSize(void);

s32 WM_StartMP(void *cb, u16 *recvBuf, u16 recvBufSize, u16 *sendBuf, u16 sendBufSize, u16 mpFreq,
                  u16 retry, BOOL minPoll, BOOL single, BOOL fixFreq, BOOL ignoreFatal)
{
    UnkWmWork *w = WMi_GetSystemWork();
    s32 r = WMi_CheckStateEx(2, 7, 8);
    u8 *p;
    if (r != 0) {
        return r;
    }
    DC_InvalidateRange(w->unk4 + 0x184, 2);
    DC_InvalidateRange(w->unk4 + 0xc2, 2);
    p = w->unk4;
    if (*(u16 *)(p + 0x184) != 0 && *(u16 *)(p + 0xc2) != 1) {
        return 3;
    }
    DC_InvalidateRange(p + 0xc, 4);
    if (*(u32 *)(w->unk4 + 0xc) == 1) {
        return 3;
    }
    if (recvBufSize < WM_GetMPReceiveBufferSize()) {
        return 6;
    }
    if (recvBufSize & 0x3f) {
        return 6;
    }
    {
        s32 s = sendBufSize;
        if (s < WM_GetMPSendBufferSize()) {
            return 6;
        }
        if (s & 0x1f) {
            return 6;
        }
    }
    WMi_SetCallbackTable(0xe, cb);
    r = WMi_SendCommand(0xe, 10, recvBuf, recvBufSize / 2, sendBuf, sendBufSize, mpFreq, retry, minPoll, single,
                      fixFreq, ignoreFatal);
    if (r == 0) {
        r = 2;
    }
    return r;
}
