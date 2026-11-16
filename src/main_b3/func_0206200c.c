#include <nitro/types.h>

typedef struct { u8 pad0[8]; s32 chr; s32 scr; s32 pal; u32 flags; } UnkRes_0206200c;
typedef struct UnkNode_0206200c UnkNode_0206200c;
extern UnkNode_0206200c *gPaletteTransferHead;
extern UnkNode_0206200c *gPaletteTransferTail;

void func_02061fc8(void);
s32 func_020620f4(void *p);
u32 func_020620e8(void *p);
void MI_UncompressLZ16(void *src, void *dst);
void MI_UncompressHuffman(void *src, void *dst);
void MI_UncompressRL16(void *src, void *dst);
void *Compress_GetData(void *p);
void Mem_Copy(void *src, void *dst, u32 size);
void DC_StoreRange(void *p, u32 size);
u32 TalkDemo_GetFlags(s32 a);
void PaletteTransfer_QueueCopy(void *p, s32 a, s32 b, u32 c);
void PaletteTransfer_QueueLoad(void *p, u32 a, u32 b);
void PaletteTransfer_Flush(void);
void ScreenTransfer_QueueRect(void *p, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j, s32 k);
void func_02061840(UnkRes_0206200c *res);

#pragma thumb on
void func_0206200c(UnkRes_0206200c *res) {
    u8 *p;

    if (res == NULL) {
        func_02061fc8();
        return;
    }
    while ((s32)*(vu16 *)0x04000006 < 0xc0)
        ;
    p = (u8 *)res + res->chr;
    switch (func_020620f4(p)) {
    case 0x10:
        MI_UncompressLZ16(p, (void *)0x06006000);
        break;
    case 0x20:
        MI_UncompressHuffman(p, (void *)0x06006000);
        break;
    case 0x30:
        MI_UncompressRL16(p, (void *)0x06006000);
        break;
    default:
        Mem_Copy(Compress_GetData(p), (void *)0x06006000, func_020620e8(p));
        break;
    }
    DC_StoreRange((void *)0x06006000, func_020620e8(p));
    while ((s32)*(vu16 *)0x04000006 < 0xc0)
        ;
    ScreenTransfer_QueueRect((u8 *)res + res->scr, 0, 0, 0x10, (res->flags & 0x1000) ? 1 : 0, 1, 0, 6, 3, 1, 0x10, 7);
    func_02061840(res);
    while ((s32)*(vu16 *)0x04000006 < 0xc0)
        ;
}
#pragma thumb reset
