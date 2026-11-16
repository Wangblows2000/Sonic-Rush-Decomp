/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 WH_SetError();
s32 WH_ChangeSysState();
extern u8 sTraceFunc[];
extern u8 data_02078850[];
s32 WH_StateInStartChildMP();
extern u8 data_02078864[];
extern u8 sMyAid[];
extern u8 sConnectMode[];
s32 WFS_End();
extern u8 data_02078884[];
s32 WH_GetWMStateCodeName();
extern u8 data_020788a0[];

void WH_StateOutStartChild(u8 *p0, s32 p1) {
    s32 t0 = *(u16 *)((u8 *)p0 + 0x2);
    if (t0 != 0) {
        WH_SetError(t0);
        s32 t1 = *(u16 *)((u8 *)p0 + 0x2);
        if (t1 == 0xc) {
            WH_ChangeSysState(9);
            return;
        }
        if (t1 == 0xb) {
            WH_ChangeSysState(9);
            return;
        }
        if (t1 == 1) {
            WH_ChangeSysState(8);
            return;
        }
        WH_ChangeSysState(9);
        return;
    }
    if (*(u16 *)((u8 *)p0 + 0x8) == 8) {
        return;
    }
    s32 t2 = *(u16 *)((u8 *)p0 + 0x8);
    if (t2 == 7) {
        s32 t3 = *(u32 *)sTraceFunc;
        if (t3 != 0) {
            ((s32 (*)())t3)(data_02078850, t3);
        }
        WH_ChangeSysState(4);
        s32 t4 = WH_StateInStartChildMP();
        if (t4 == 0) {
            s32 t5 = *(u32 *)sTraceFunc;
            if (t5 != 0) {
                ((s32 (*)())t5)(data_02078864, t5);
            }
            WH_ChangeSysState(3);
            return;
        }
        *(u16 *)sMyAid = *(u16 *)((u8 *)p0 + 0xa);
        return;
    }
    if (t2 == 6) {
        return;
    }
    if (t2 == 9) {
        s32 t6 = *(u32 *)sConnectMode;
        if (t6 == 7) {
            WFS_End(t6, p1);
        }
        s32 t7 = *(u32 *)sTraceFunc;
        if (t7 != 0) {
            ((s32 (*)())t7)(data_02078884, t7);
        }
        WH_SetError(0x14);
        WH_ChangeSysState(9);
        return;
    }
    s32 t8 = *(u32 *)sTraceFunc;
    if (t8 != 0) {
        s32 t9 = WH_GetWMStateCodeName(t2, t8);
        ((s32 (*)())*(u32 *)sTraceFunc)(data_020788a0, *(u16 *)((u8 *)p0 + 0x8), t9, *(u32 *)sTraceFunc);
    }
    WH_ChangeSysState(9);
}
