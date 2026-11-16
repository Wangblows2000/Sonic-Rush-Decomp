#include <nitro/types.h>

extern u32 data_0207d014;
extern u32 data_0207d004;
extern u32 data_0207d018;
extern u32 data_02077964;

void func_02007da4(u32 dmaNo, const void *src, void *dest, u32 size);
void func_02007bac(u32 dmaNo, const void *src, void *dest, u32 size, void *callback, void *arg);
void MIi_CpuCopy32(const void *src, void *dest, u32 size);

static inline void func_02004fec_DmaCopy32(u32 dmaNo, const void *src, void *dest, u32 size)
{
    if (dmaNo != (u32)-1 && size > 0x30) {
        func_02007da4(dmaNo, src, dest, size);
    } else {
        MIi_CpuCopy32(src, dest, size);
    }
}

static inline void func_02004fec_DmaCopy32Async(u32 dmaNo, const void *src, void *dest, u32 size, void *callback, void *arg)
{
    if (dmaNo != (u32)-1) {
        func_02007bac(dmaNo, src, dest, size, callback, arg);
    } else {
        MIi_CpuCopy32(src, dest, size);
    }
}

void func_02004fec(const void *pSrc, u32 destSlotAddr, u32 szByte)
{
    void *pDst;
    u32 base = data_0207d014;

    if (!base) {
        pDst = (void *)(data_0207d004 + destSlotAddr);
    } else {
        if ((destSlotAddr + szByte) < data_0207d018) {
            pDst = (void *)(data_0207d004 + destSlotAddr);
        } else if (destSlotAddr >= data_0207d018) {
            pDst = (void *)(base + destSlotAddr - data_0207d018);
        } else {
            void *pDst2 = (void *)base;
            u32 sz = data_0207d018 - destSlotAddr;
            pDst = (void *)(data_0207d004 + destSlotAddr);
            func_02004fec_DmaCopy32(data_02077964, pSrc, pDst, sz);
            func_02004fec_DmaCopy32Async(data_02077964, (void *)((u8 *)pSrc + sz), pDst2, szByte - sz, NULL, NULL);
            return;
        }
    }
    func_02004fec_DmaCopy32Async(data_02077964, pSrc, pDst, szByte, NULL, NULL);
}
