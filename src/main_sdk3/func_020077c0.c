#include <nitro/types.h>

extern u16 data_0207d39c;

void func_02009fa8(void);
BOOL PXI_IsCallbackReady(u32 fifotag, u32 proc);
void func_0200a1b4(u32 tag, void (*fn)(u32, u32, BOOL));
void func_0200777c(u32 tag, u32 data, BOOL err);

void func_020077c0(void)
{
    if (data_0207d39c) {
        return;
    }
    data_0207d39c = TRUE;
    func_02009fa8();
    while (!PXI_IsCallbackReady(12, 1)) {
    }
    func_0200a1b4(12, func_0200777c);
}
