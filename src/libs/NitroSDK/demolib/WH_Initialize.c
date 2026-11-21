/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 sRecvBufferSize[];
extern u8 sParentParam[];
extern u8 sSendBufferSize[];
extern u8 data_022c1970[];
extern u8 sMyAid[];
extern u8 sErrCode[];
extern u8 data_022c1978[];
extern u8 sConnectBitmap[];
s32 WH_StateInInitialize();

s32 WH_Initialize(void) {
    *(u32 *)sRecvBufferSize = 0;
    *(u32 *)sSendBufferSize = 0;
    *(u32 *)data_022c1970 = 0;
    *(u16 *)sMyAid = 0;
    *(u16 *)sConnectBitmap = 1;
    *(u32 *)sErrCode = 0;
    *(u32 *)sParentParam = 0;
    *(u16 *)((u8 *)sParentParam + 0x4) = 0;
    *(u32 *)data_022c1978 = 0;
    s32 t0 = WH_StateInInitialize(data_022c1978, sParentParam, sErrCode, sConnectBitmap);
    return (t0 != 0);
}
