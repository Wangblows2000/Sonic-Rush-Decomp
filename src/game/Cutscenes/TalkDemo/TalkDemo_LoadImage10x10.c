#include <nitro/types.h>

typedef struct { u8 pad0[8]; s32 chr; s32 scr; s32 pal; u32 flags; } UnkRes_02062214;
typedef struct UnkNode_02062214 UnkNode_02062214;
extern UnkNode_02062214 *gPaletteTransferHead;
extern UnkNode_02062214 *gPaletteTransferTail;

void TalkDemo_ClearImage10x10(void);
s32 TalkDemo_GetCompressionType(void *p);
u32 TalkDemo_GetUncompressedSize(void *p);
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
void TalkDemo_CreateFreeResTask(UnkRes_02062214 *res);

#pragma thumb on
void TalkDemo_LoadImage10x10(UnkRes_02062214 *res) {
    u8 *p;

    if (res == NULL) {
        TalkDemo_ClearImage10x10();
        return;
    }
    while ((s32)*(vu16 *)0x04000006 < 0xc0)
        ;
    p = (u8 *)res + res->chr;
    switch (TalkDemo_GetCompressionType(p)) {
    case 0x10:
        MI_UncompressLZ16(p, (void *)0x06008000);
        break;
    case 0x20:
        MI_UncompressHuffman(p, (void *)0x06008000);
        break;
    case 0x30:
        MI_UncompressRL16(p, (void *)0x06008000);
        break;
    default:
        Mem_Copy(Compress_GetData(p), (void *)0x06008000, TalkDemo_GetUncompressedSize(p));
        break;
    }
    DC_StoreRange((void *)0x06008000, TalkDemo_GetUncompressedSize(p));
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
    TalkDemo_CreateFreeResTask(res);
    while ((s32)*(vu16 *)0x04000006 < 0xc0)
        ;
}
#pragma thumb reset
