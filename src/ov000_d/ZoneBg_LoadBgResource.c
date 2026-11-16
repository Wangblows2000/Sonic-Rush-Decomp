#include <nitro/types.h>

typedef struct UnkStruct_023418c4 {
    u8 pad0[8];
    s32 chr;
    s32 scr;
    s32 pal;
    u32 flags;
} UnkStruct_023418c4;

extern u32 gPaletteTransferHead;
extern u32 gPaletteTransferTail;
extern u32 gScreenTransferHead;
extern u32 gScreenTransferTail;

u32 ZoneBg_GetCompressionType(void *res);
u32 ZoneBg_GetUncompressedSize(void *res);
void MI_UncompressLZ16(void *src, void *dst);
void MI_UncompressHuffman(void *src, void *dst);
void MI_UncompressRL16(void *src, void *dst);
void *Compress_GetData(void *res);
void Mem_Copy(void *src, void *dst, u32 size);
void DC_StoreRange(void *p, u32 size);
void PaletteTransfer_QueueLoad(void *res, s32 a, u32 b);
void PaletteTransfer_Flush(void);
void ScreenTransfer_QueueRect(void *res, s32 a, s32 b, s32 k, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j);
void ScreenTransfer_Flush(void);

static inline void UnkDecomp_023418c4(void *res, void *dst) {
    switch (ZoneBg_GetCompressionType(res)) {
    case 0x10:
        MI_UncompressLZ16(res, dst);
        break;
    case 0x20:
        MI_UncompressHuffman(res, dst);
        break;
    case 0x30:
        MI_UncompressRL16(res, dst);
        break;
    default:
        Mem_Copy(Compress_GetData(res), dst, ZoneBg_GetUncompressedSize(res));
        break;
    }
    DC_StoreRange(dst, ZoneBg_GetUncompressedSize(res));
}

static inline void UnkPal_023418c4(void *res, s32 a, u32 b) {
    u32 sa = gPaletteTransferHead;
    u32 sb = gPaletteTransferTail;
    gPaletteTransferHead = 0;
    gPaletteTransferTail = 0;
    PaletteTransfer_QueueLoad(res, a, b);
    PaletteTransfer_Flush();
    gPaletteTransferHead = sa;
    gPaletteTransferTail = sb;
}

static inline void UnkScr_023418c4(UnkStruct_023418c4 *r, s32 x) {
    void *res = (u8 *)r + r->scr;
    s32 flag = (r->flags & 0x1000) ? 1 : 0;
    u32 sa = gScreenTransferHead;
    u32 sb = gScreenTransferTail;
    gScreenTransferHead = 0;
    gScreenTransferTail = 0;
    ScreenTransfer_QueueRect(res, 0, 0, 0x40, flag, x, 0, 8, 0, 0, 0x40, 0x40);
    ScreenTransfer_Flush();
    gScreenTransferHead = sa;
    gScreenTransferTail = sb;
}

void ZoneBg_LoadBgResource(UnkStruct_023418c4 *r, s32 screen, u32 mask) {
    u32 type = r->flags & 0xf;

    if (screen == 0 || screen == 2) {
        if (mask & 2) {
            UnkDecomp_023418c4((u8 *)r + r->chr, (void *)0x6008000);
        }
        if (mask & 1) {
            switch (type) {
            case 1:
                UnkPal_023418c4((u8 *)r + r->pal, 0, 0x5000000);
                break;
            case 2:
                UnkPal_023418c4((u8 *)r + r->pal, 1, 0x6200);
                break;
            }
        }
        if (mask & 4) {
            UnkScr_023418c4(r, 3);
        }
    }
    if ((u32)(screen - 1) > 1) {
        return;
    }
    if (mask & 2) {
        UnkDecomp_023418c4((u8 *)r + r->chr, (void *)0x6208000);
    }
    if (mask & 1) {
        switch (type) {
        case 1:
            UnkPal_023418c4((u8 *)r + r->pal, 0, 0x5000400);
            break;
        case 2:
            UnkPal_023418c4((u8 *)r + r->pal, 3, 0x6200);
            break;
        }
    }
    if (mask & 4) {
        UnkScr_023418c4(r, 0xf);
    }
}
