/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gTaskPoolUsed[];
extern u8 gTaskFreeList[];
s32 func_02006abc();

s32 Task_AllocNode(void) {
    s32 t0 = *(u32 *)gTaskPoolUsed;
    if (t0 < 0x100) {
        *(u32 *)gTaskPoolUsed = (t0 + 0x1);
        return *(u32 *)((u8 *)gTaskFreeList + (t0 << 2));
    }
    s32 t1 = func_02006abc(0, gTaskPoolUsed);
    return t1;
}
