#include <nitro/types.h>

extern void *data_ov000_0236aab8;
extern u8 data_ov000_02368804[];
extern u32 gPaletteTransferHead;
extern u32 gPaletteTransferTail;

void func_02021310(void *f, void *path, void *arc);
void *func_02021264(void *f, s32 i);
void func_020212e0(void *f);
s32 func_ov000_0233ac84(void *res);
u32 func_ov000_0233ac78(void *res);
void func_02008648(void *src, void *dst);
void func_02008720(void *src, void *dst);
void func_020087f8(void *src, void *dst);
void *Compress_GetData(void *res);
void Mem_Copy(void *src, void *dst, u32 size);
void DC_StoreRange(void *p, u32 size);
void PaletteTransfer_QueueLoad(void *res, s32 a, u32 b);
void PaletteTransfer_Flush(void);
void func_ov000_0233a4c8(void *f);

void func_ov000_0233ace4(void) {
    u8 f[0x5C];
    void *res;
    u32 sa;
    u32 sb;

    func_02021310(f, data_ov000_02368804, data_ov000_0236aab8);
    res = func_02021264(f, 3);
    switch (func_ov000_0233ac84(res)) {
    case 0x10:
        func_02008648(res, (void *)0x6010000);
        break;
    case 0x20:
        func_02008720(res, (void *)0x6010000);
        break;
    case 0x30:
        func_020087f8(res, (void *)0x6010000);
        break;
    default:
        Mem_Copy(Compress_GetData(res), (void *)0x6010000, func_ov000_0233ac78(res));
        break;
    }
    DC_StoreRange((void *)0x6010000, func_ov000_0233ac78(res));
    res = func_02021264(f, 6);
    sa = gPaletteTransferHead;
    sb = gPaletteTransferTail;
    gPaletteTransferHead = 0;
    gPaletteTransferTail = 0;
    PaletteTransfer_QueueLoad(res, 0, 0x5000000);
    PaletteTransfer_Flush();
    gPaletteTransferHead = sa;
    gPaletteTransferTail = sb;
    func_ov000_0233a4c8(f);
    func_020212e0(f);
}
