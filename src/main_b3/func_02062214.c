#include <nitro/types.h>

typedef struct { u8 pad0[8]; s32 chr; s32 scr; s32 pal; u32 flags; } UnkRes_02062214;
typedef struct UnkNode_02062214 UnkNode_02062214;
extern UnkNode_02062214 *gPaletteTransferHead;
extern UnkNode_02062214 *gPaletteTransferTail;

void func_02061fec(void);
s32 func_020620f4(void *p);
u32 func_020620e8(void *p);
void func_02008648(void *src, void *dst);
void func_02008720(void *src, void *dst);
void func_020087f8(void *src, void *dst);
void *Compress_GetData(void *p);
void Mem_Copy(void *src, void *dst, u32 size);
void DC_StoreRange(void *p, u32 size);
u32 TalkDemo_GetFlags(s32 a);
void PaletteTransfer_QueueCopy(void *p, s32 a, s32 b, u32 c);
void PaletteTransfer_QueueLoad(void *p, u32 a, u32 b);
void PaletteTransfer_Flush(void);
void ScreenTransfer_QueueRect(void *p, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j, s32 k);
void func_02061840(UnkRes_02062214 *res);

#pragma thumb on
void func_02062214(UnkRes_02062214 *res) {
    u8 *p;

    if (res == NULL) {
        func_02061fec();
        return;
    }
    while ((s32)*(vu16 *)0x04000006 < 0xc0)
        ;
    p = (u8 *)res + res->chr;
    switch (func_020620f4(p)) {
    case 0x10:
        func_02008648(p, (void *)0x06008000);
        break;
    case 0x20:
        func_02008720(p, (void *)0x06008000);
        break;
    case 0x30:
        func_020087f8(p, (void *)0x06008000);
        break;
    default:
        Mem_Copy(Compress_GetData(p), (void *)0x06008000, func_020620e8(p));
        break;
    }
    DC_StoreRange((void *)0x06008000, func_020620e8(p));
    while ((s32)*(vu16 *)0x04000006 < 0xc0)
        ;
    p = (u8 *)res + res->pal;
    if (!(TalkDemo_GetFlags(0) & 1)) {
        PaletteTransfer_QueueCopy(p + 4, 0xe0, 0, 0x05000000);
    } else {
        UnkNode_02062214 *s0 = gPaletteTransferHead;
        UnkNode_02062214 *s1 = gPaletteTransferTail;
        gPaletteTransferHead = NULL;
        gPaletteTransferTail = NULL;
        PaletteTransfer_QueueLoad(p, 1, 0x4000);
        PaletteTransfer_Flush();
        gPaletteTransferHead = s0;
        gPaletteTransferTail = s1;
    }
    ScreenTransfer_QueueRect((u8 *)res + res->scr, 0, 0, 10, (res->flags & 0x1000) ? 1 : 0, 9, 0, 4, 0, 0, 10, 10);
    func_02061840(res);
    while ((s32)*(vu16 *)0x04000006 < 0xc0)
        ;
}
#pragma thumb reset
