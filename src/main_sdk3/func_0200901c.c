#include <nitro/types.h>

extern void func_0200a1b4(u32 fifotag, void (*callback)(u32, u32, BOOL));
extern void func_02009088(u32 tag, u32 data, BOOL err);
extern BOOL func_02008f48(void);
extern BOOL PXI_IsCallbackReady(u32 fifotag, u32 proc);
extern void OS_SpinWait(u32 cycles);

void func_0200901c(void)
{
    func_0200a1b4(7, func_02009088);
    if (!func_02008f48()) {
        return;
    }
    while (!PXI_IsCallbackReady(7, 1)) {
        OS_SpinWait(100);
    }
}
