/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_022c197c[];
extern u8 sSysState[];
extern u8 data_020785c4[];
extern u8 data_02078af8[];
extern u8 data_02078b00[];

s32 WH_ChangeSysState(s32 p0) {
    s32 t0 = *(u32 *)data_022c197c;
    if (t0 != 0) {
        ((s32 (*)())t0)(data_02078af8, *(u32 *)((u8 *)data_020785c4 + (*(u32 *)sSysState << 2)), *(u32 *)sSysState, t0);
    }
    s32 t1 = *(u32 *)data_022c197c;
    *(u32 *)sSysState = p0;
    if (t1 == 0) {
        return (s32)data_022c197c;
    }
    s32 t2 = ((s32 (*)())t1)(data_02078b00, *(u32 *)((u8 *)data_020785c4 + ((s32)p0 << 2)), t1);
    return t2;
}
