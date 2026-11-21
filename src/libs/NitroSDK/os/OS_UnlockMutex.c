/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 OS_DisableInterrupts();
extern u8 data_0207d090[];
s32 OSi_DequeueItem();
s32 OS_WakeupThread();
s32 OS_RestoreInterrupts();

void OS_UnlockMutex(u8 *p0) {
    s32 t0 = OS_DisableInterrupts();
    s32 t1 = *(u32 *)((u8 *)data_0207d090 + 0x8);
    if (*(u32 *)((u8 *)p0 + 0x4) == t1) {
        *(u32 *)((u8 *)p0 + 0x8) = (*(u32 *)((u8 *)p0 + 0x8) - 0x1);
        if (*(u32 *)((u8 *)p0 + 0x8) == 0) {
            OSi_DequeueItem(t1, p0);
            *(u32 *)((u8 *)p0 + 0x4) = 0;
            OS_WakeupThread(p0, 0);
        }
    }
    OS_RestoreInterrupts(t0);
}
