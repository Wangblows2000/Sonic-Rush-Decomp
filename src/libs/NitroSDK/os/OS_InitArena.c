/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_0207d2d4[];
s32 OS_GetInitArenaHi();
s32 OS_SetArenaHi();
s32 OS_GetInitArenaLo();
s32 OS_SetArenaLo();

void OS_InitArena(void) {
    if (*(u32 *)data_0207d2d4 != 0) {
        return;
    }
    *(u32 *)data_0207d2d4 = 1;
    s32 t0 = OS_GetInitArenaHi(0, data_0207d2d4, 1);
    OS_SetArenaHi(0, t0);
    s32 t1 = OS_GetInitArenaLo(0);
    OS_SetArenaLo(0, t1);
    OS_SetArenaLo(2, 0);
    OS_SetArenaHi(2, 0);
    s32 t2 = OS_GetInitArenaHi(3);
    OS_SetArenaHi(3, t2);
    s32 t3 = OS_GetInitArenaLo(3);
    OS_SetArenaLo(3, t3);
    s32 t4 = OS_GetInitArenaHi(4);
    OS_SetArenaHi(4, t4);
    s32 t5 = OS_GetInitArenaLo(4);
    OS_SetArenaLo(4, t5);
    s32 t6 = OS_GetInitArenaHi(5);
    OS_SetArenaHi(5, t6);
    s32 t7 = OS_GetInitArenaLo(5);
    OS_SetArenaLo(5, t7);
    s32 t8 = OS_GetInitArenaHi(6);
    OS_SetArenaHi(6, t8);
    s32 t9 = OS_GetInitArenaLo(6);
    OS_SetArenaLo(6, t9);
}
