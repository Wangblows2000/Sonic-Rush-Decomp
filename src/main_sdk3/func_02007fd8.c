#include <nitro/types.h>

typedef struct {
    vu32 isBusy;  /* 0x00 */
    u32 dmaNo;    /* 0x04 */
    u8 pad8[8];
    void (*callback)(void *); /* 0x10 */
    void *arg;    /* 0x14 */
} UnkGXDma_02007fd8;

extern UnkGXDma_02007fd8 data_0207d3bc;

void func_02007a04(u32 dmaNo, u32 dmaType);
void func_02007994(u32 dmaNo, u32 src, u32 size, u32 dir);
void func_02007b3c(u32 dmaNo);
void func_02005448(u32 dmaNo, void (*callback)(void *), void *arg);
void func_01ff85cc(u32 dmaNo, u32 src, u32 dest, u32 ctrl);
void func_02007f98(void *arg);

void func_02007fd8(u32 dmaNo, const void *src, u32 commandLength, void (*callback)(void *), void *arg)
{
    if (commandLength == 0) {
        if (callback) {
            callback(arg);
        }
        return;
    }
    while (data_0207d3bc.isBusy) {
    }
    data_0207d3bc.isBusy = TRUE;
    data_0207d3bc.dmaNo = dmaNo;
    data_0207d3bc.callback = callback;
    data_0207d3bc.arg = arg;
    func_02007a04(dmaNo, 0x38000000);
    func_02007994(dmaNo, (u32)src, commandLength, 0);
    func_02007b3c(dmaNo);
    func_02005448(dmaNo, func_02007f98, NULL);
    func_01ff85cc(dmaNo, (u32)src, 0x04000400, 0xfc400000 | (commandLength >> 2));
}
