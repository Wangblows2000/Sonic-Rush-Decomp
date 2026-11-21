/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_0207f2c0[];
s32 OS_WakeupThreadDirect();

void CARDi_OnFifoRecv(s32 p0, s32 p1, s32 p2) {
    if ((s32)p0 != 0xb) {
        return;
    }
    if ((s32)p2 == 0) {
        return;
    }
    *(u32 *)((u8 *)data_0207f2c0 + 0x34) = (*(u32 *)((u8 *)data_0207f2c0 + 0x34) & ~0x20);
    OS_WakeupThreadDirect(*(u32 *)((u8 *)data_0207f2c0 + 0xd0), data_0207f2c0);
}
