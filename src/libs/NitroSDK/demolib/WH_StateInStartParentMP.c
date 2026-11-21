#include <nitro/types.h>

typedef struct {
    u8 pad0[0x16];
    u16 unk16;
} UnkCfg_0203ff68;

extern s32 sSysState;
extern UnkCfg_0203ff68 sParentParam;
extern u32 sSendBufferSize;
extern u8 data_022c1da0[];
extern u32 sRecvBufferSize;
extern u8 data_022c1fc0[];

void WH_ChangeSysState(s32 a);
void WH_SetError(s32 v);
s32 WM_StartMP(void (*cb)(void), void *recvBuf, u16 recvBufSize, void *sendBuf, u16 sendBufSize,
                  u16 mpFreq, u16 retry, BOOL a, BOOL b, BOOL c, BOOL d);
void WH_StateOutStartParentMP(void);

s32 WH_StateInStartParentMP(void) {
    s32 r;

    if ((u32)(sSysState - 4) <= 2)
        return 1;
    WH_ChangeSysState(4);
    r = WM_StartMP(WH_StateOutStartParentMP, data_022c1fc0, sRecvBufferSize, data_022c1da0, sSendBufferSize, sParentParam.unk16 == 0, 0, 0, 0, 0, 0);
    if (r == 2)
        return 1;
    WH_SetError(r);
    return 0;
}
