#include <nitro/types.h>

extern u32 data_02080f5c;
extern u32 data_02080f60;

void func_0201ac80(void);
void func_02009fa8(void);
BOOL PXI_IsCallbackReady(u32 fifo, u32 proc);
void func_0200a1b4(u32 fifo, void (*cb)(u32, u32, BOOL));
void func_0201ad40(u32 tag, u32 data, BOOL err);
void func_0201ace0(u32 tag, u32 data, BOOL err);
void func_0201ad7c(void);

void func_0201af48(void)
{
    if (data_02080f5c != 0) {
        return;
    }
    data_02080f5c = 1;
    func_0201ac80();
    func_02009fa8();
    while (!PXI_IsCallbackReady(13, 1)) {
    }
    func_0200a1b4(13, func_0201ad40);
    func_0201ad7c();
    func_0200a1b4(13, NULL);
    func_0200a1b4(13, func_0201ace0);
    data_02080f60 = 0;
}
