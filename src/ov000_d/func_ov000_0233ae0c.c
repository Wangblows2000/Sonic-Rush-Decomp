#include <nitro/types.h>

extern void *data_ov000_0236aab8;
extern u8 data_ov000_02368808[];
extern u32 gPaletteTransferHead;
extern u32 gPaletteTransferTail;

void func_02021310(void *buf, void *a, void *b);
void *func_02021264(void *buf, s32 a);
void func_020212e0(void *buf);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
u32 func_ov000_0233ac84(void *res);
u32 func_ov000_0233ac78(void *res);
void func_02008648(void *src, void *dst);
void func_02008720(void *src, void *dst);
void func_020087f8(void *src, void *dst);
void *Compress_GetData(void *res);
void Mem_Copy(void *src, void *dst, u32 size);
void VBlankIntrWait(void);
void PaletteTransfer_QueueLoad(void *res, s32 a, s32 b);
void PaletteTransfer_Flush(void);
void func_ov000_0233a4c8(void *arc);

static inline void UnkDecomp_0233ae0c(void *res, void *dst) {
    switch (func_ov000_0233ac84(res)) {
    case 0x10:
        func_02008648(res, dst);
        break;
    case 0x20:
        func_02008720(res, dst);
        break;
    case 0x30:
        func_020087f8(res, dst);
        break;
    default:
        Mem_Copy(Compress_GetData(res), dst, func_ov000_0233ac78(res));
        break;
    }
    DC_StoreRange(dst, func_ov000_0233ac78(res));
}

void func_ov000_0233ae0c(void) {
    vu32 z0;
    vu32 z1;
    u8 buf[0x60];
    void *res;
    u32 sa;
    u32 sb;

    func_02021310(buf, data_ov000_02368808, data_ov000_0236aab8);
    z0 = 0;
    MIi_CpuClear32(z0, (void *)0x6010000, 0x10000);
    z1 = 0;
    MIi_CpuClear32(z1, (void *)0x6210000, 0x10000);
    res = func_02021264(buf, 3);
    UnkDecomp_0233ae0c(res, (void *)0x6010000);
    UnkDecomp_0233ae0c(res, (void *)0x6210000);
    VBlankIntrWait();
    res = func_02021264(buf, 6);
    sa = gPaletteTransferHead;
    sb = gPaletteTransferTail;
    gPaletteTransferHead = 0;
    gPaletteTransferTail = 0;
    PaletteTransfer_QueueLoad(res, 1, 0x4000);
    PaletteTransfer_Flush();
    gPaletteTransferHead = 0;
    gPaletteTransferTail = 0;
    PaletteTransfer_QueueLoad(res, 1, 0x6000);
    PaletteTransfer_Flush();
    gPaletteTransferHead = 0;
    gPaletteTransferTail = 0;
    PaletteTransfer_QueueLoad(res, 3, 0x4000);
    PaletteTransfer_Flush();
    gPaletteTransferHead = 0;
    gPaletteTransferTail = 0;
    PaletteTransfer_QueueLoad(res, 3, 0x6000);
    PaletteTransfer_Flush();
    gPaletteTransferHead = sa;
    gPaletteTransferTail = sb;
    func_ov000_0233a4c8(buf);
    func_020212e0(buf);
}
