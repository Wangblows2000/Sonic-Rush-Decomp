/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gTransferJobUsed[];
extern u8 gTransferJobFreeList[];
s32 func_02006abc();

s32 TransferJob_Alloc(void) {
    s32 t0 = *(u16 *)gTransferJobUsed;
    if ((u32)t0 < (u32)0x100) {
        *(u16 *)gTransferJobUsed = (t0 + 0x1);
        return *(u32 *)((u8 *)gTransferJobFreeList + (t0 << 2));
    }
    s32 t1 = func_02006abc(0, gTransferJobUsed);
    return t1;
}
