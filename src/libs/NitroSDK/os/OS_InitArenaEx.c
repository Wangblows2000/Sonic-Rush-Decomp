/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 OS_GetInitArenaHi();
s32 OS_SetArenaHi();
s32 OS_GetInitArenaLo();
s32 OS_SetArenaLo();
extern u8 data_0207d2d8[];
s32 OS_GetConsoleType();
s32 OS_SetProtectionRegion1();
s32 OS_SetProtectionRegion2();

void OS_InitArenaEx(void) {
    s32 t0 = OS_GetInitArenaHi(2);
    OS_SetArenaHi(2, t0);
    s32 t1 = OS_GetInitArenaLo(2);
    OS_SetArenaLo(2, t1);
    if (*(u32 *)data_0207d2d8 != 0) {
        s32 t2 = *(u16 *)0x27ffc40;
        if (t2 != 2) {
            s32 t3 = OS_GetConsoleType(t2);
            if ((t3 & 3) != 1) {
                return;
            }
        }
    }
    OS_SetProtectionRegion1(0x200002b);
    OS_SetProtectionRegion2(0x23e0021);
}
