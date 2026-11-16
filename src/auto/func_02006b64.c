/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_0207d2d4[];
s32 func_02006990();
s32 OS_SetArenaHi();
s32 func_020068a8();
s32 OS_SetArenaLo();

void func_02006b64(void) {
    if (*(u32 *)data_0207d2d4 != 0) {
        return;
    }
    *(u32 *)data_0207d2d4 = 1;
    s32 t0 = func_02006990(0, data_0207d2d4, 1);
    OS_SetArenaHi(0, t0);
    s32 t1 = func_020068a8(0);
    OS_SetArenaLo(0, t1);
    OS_SetArenaLo(2, 0);
    OS_SetArenaHi(2, 0);
    s32 t2 = func_02006990(3);
    OS_SetArenaHi(3, t2);
    s32 t3 = func_020068a8(3);
    OS_SetArenaLo(3, t3);
    s32 t4 = func_02006990(4);
    OS_SetArenaHi(4, t4);
    s32 t5 = func_020068a8(4);
    OS_SetArenaLo(4, t5);
    s32 t6 = func_02006990(5);
    OS_SetArenaHi(5, t6);
    s32 t7 = func_020068a8(5);
    OS_SetArenaLo(5, t7);
    s32 t8 = func_02006990(6);
    OS_SetArenaHi(6, t8);
    s32 t9 = func_020068a8(6);
    OS_SetArenaLo(6, t9);
}
