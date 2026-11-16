#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    u32 unk8;
    u32 unkC;
    u32 unk10;
    u32 unk14;
} UnkRes_02341d00;

extern u32 gPaletteTransferTail;
extern u32 gPaletteTransferHead;
extern u32 gScreenTransferTail;
extern u32 gScreenTransferHead;

s32 func_ov000_023418b8(void *p);
u32 func_ov000_023418ac(void *p);
void func_02008648(void *src, void *dst);
void func_02008720(void *src, void *dst);
void func_020087f8(void *src, void *dst);
void *Compress_GetData(void *p);
void Mem_Copy(void *src, void *dst, u32 size);
void DC_StoreRange(void *p, u32 size);
void PaletteTransfer_QueueLoad(void *p, s32 a, s32 b);
void PaletteTransfer_Flush(void);
void ScreenTransfer_QueueRect(void *p, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j, s32 k);
void ScreenTransfer_Flush(void);

void func_ov000_02341d00(UnkRes_02341d00 *res, s32 mode, u32 flags) {
    u8 *p;
    void *src;
    u32 a;
    u32 b;
    s32 f;

    if (mode == 1) {
        return;
    }
    if (flags & 2) {
        p = (u8 *)res + res->unk8;
        switch (func_ov000_023418b8(p)) {
        case 0x10:
            func_02008648(p, (void *)0x6008000);
            break;
        case 0x20:
            func_02008720(p, (void *)0x6008000);
            break;
        case 0x30:
            func_020087f8(p, (void *)0x6008000);
            break;
        default:
            src = Compress_GetData(p);
            Mem_Copy(src, (void *)0x6008000, func_ov000_023418ac(p));
            break;
        }
        DC_StoreRange((void *)0x6008000, func_ov000_023418ac(p));
    }
    if (flags & 1) {
        p = (u8 *)res + res->unk10;
        a = gPaletteTransferHead;
        b = gPaletteTransferTail;
        gPaletteTransferHead = 0;
        gPaletteTransferTail = 0;
        PaletteTransfer_QueueLoad(p, 0, 0x5000000);
        PaletteTransfer_Flush();
        gPaletteTransferHead = a;
        gPaletteTransferTail = b;
    }
    if (flags & 4) {
        p = (u8 *)res + res->unkC;
        f = (res->unk14 & 0x1000) ? 1 : 0;
        a = gScreenTransferHead;
        b = gScreenTransferTail;
        gScreenTransferHead = 0;
        gScreenTransferTail = 0;
        ScreenTransfer_QueueRect(p, 0, 0, 0x40, f, 3, 0, 8, 0, 0, 0x40, 0x40);
        ScreenTransfer_Flush();
        gScreenTransferHead = a;
        gScreenTransferTail = b;
    }
}
