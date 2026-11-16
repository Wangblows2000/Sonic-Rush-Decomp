/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 sSendBufferSize[];
extern u8 data_022c1da0[];
s32 DC_FlushRange();
void WH_StateOutSetMPData();
s32 func_02011c44();
extern u8 sTraceFunc[];
s32 WH_GetWMErrCodeName();
extern u8 data_020787b0[];

s32 WH_StateInSetMPData(s32 p0, s32 p1, s32 p2) {
    DC_FlushRange(data_022c1da0, *(u32 *)sSendBufferSize, p2, sSendBufferSize);
    s32 t0 = func_02011c44((u32)WH_StateOutSetMPData, p2, p0, p1, 0xffff, 0xe, 2);
    if (t0 != 2) {
        s32 t1 = *(u32 *)sTraceFunc;
        if (t1 != 0) {
            s32 t2 = WH_GetWMErrCodeName(t0, t1);
            ((s32 (*)())*(u32 *)sTraceFunc)(data_020787b0, t2, *(u32 *)sTraceFunc);
        }
        return 0;
    }
    return 1;
}
