#include <nitro/types.h>

typedef struct { u8 pad0[8]; s32 chr; s32 scr; s32 pal; u32 flags; } UnkRes_02057af8;
typedef struct UnkNode_02057af8 UnkNode_02057af8;
extern UnkNode_02057af8 *gPaletteTransferHead;
extern UnkNode_02057af8 *gPaletteTransferTail;
extern UnkNode_02057af8 *gScreenTransferHead;
extern UnkNode_02057af8 *gScreenTransferTail;
extern u8 data_0207c5f0[];
extern void *gMainHeap;

UnkRes_02057af8 *File_LoadArchiveEntry(void *a, s32 b, s32 c);
s32 PolyDemoUnder2_GetCompressionType(void *p);
u32 PolyDemoUnder2_GetUncompressedSize(void *p);
void func_02008648(void *src, void *dst);
void func_02008720(void *src, void *dst);
void func_020087f8(void *src, void *dst);
void *Compress_GetData(void *p);
void Mem_Copy(void *src, void *dst, u32 size);
void DC_StoreRange(void *p, u32 size);
void PaletteTransfer_QueueCopy(void *p, s32 a, s32 b, u32 c);
void PaletteTransfer_Flush(void);
void ScreenTransfer_QueueRect(void *p, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j, s32 k);
void ScreenTransfer_Flush(void);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void Heap_Free(void *heap, void *p);

#pragma thumb on
static inline void LoadScr_02057af8(void *scr, UnkRes_02057af8 *res) {
    s32 flag = (res->flags & 0x1000) ? 1 : 0;
    UnkNode_02057af8 *s0 = gScreenTransferHead;
    UnkNode_02057af8 *s1 = gScreenTransferTail;
    gScreenTransferHead = NULL;
    gScreenTransferTail = NULL;
    ScreenTransfer_QueueRect(scr, 0, 0, 0x20, flag, 0xc, 0, 4, 0, 0, 0x20, 0x18);
    ScreenTransfer_Flush();
    gScreenTransferHead = s0;
    gScreenTransferTail = s1;
}

static inline void LoadPal_02057af8(void *pal) {
    UnkNode_02057af8 *s0 = gPaletteTransferHead;
    UnkNode_02057af8 *s1 = gPaletteTransferTail;
    gPaletteTransferHead = NULL;
    gPaletteTransferTail = NULL;
    PaletteTransfer_QueueCopy(pal, 0x100, 0, 0x05000400);
    PaletteTransfer_Flush();
    gPaletteTransferHead = s0;
    gPaletteTransferTail = s1;
}

static inline void Free_02057af8(u8 *blk) {
    if ((u8 *)func_02006abc(0) != blk) {
        u32 size;
        blk -= 0x20;
        size = *(u32 *)blk;
        if (*(u32 *)(blk + size - 4) == 0x02467531) {
            vu32 fill = 0xcccccccc;
            MIi_CpuClear32(fill, blk, size);
            DC_StoreRange(blk, size);
        }
    }
    Heap_Free(gMainHeap, blk);
}

void PolyDemoUnder2_LoadBg(void) {
    UnkRes_02057af8 *res = File_LoadArchiveEntry(data_0207c5f0, 1, -1);
    u8 *p = (u8 *)res + res->chr;

    switch (PolyDemoUnder2_GetCompressionType(p)) {
    case 0x10:
        func_02008648(p, (void *)0x06200000);
        break;
    case 0x20:
        func_02008720(p, (void *)0x06200000);
        break;
    case 0x30:
        func_020087f8(p, (void *)0x06200000);
        break;
    default:
        Mem_Copy(Compress_GetData(p), (void *)0x06200000, PolyDemoUnder2_GetUncompressedSize(p));
        break;
    }
    DC_StoreRange((void *)0x06200000, PolyDemoUnder2_GetUncompressedSize(p));
    LoadScr_02057af8((u8 *)res + res->scr, res);
    LoadPal_02057af8((u8 *)res + res->pal + 4);
    Free_02057af8((u8 *)res);
}
#pragma thumb reset
