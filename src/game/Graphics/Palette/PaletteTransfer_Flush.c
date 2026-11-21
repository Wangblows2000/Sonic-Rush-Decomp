#include <nitro/types.h>

typedef struct UnkJob_02035754 {
    struct UnkJob_02035754 *next;
    u32 type;
    u32 dstType;
    u32 dst;
    union {
        u32 src;
        u16 arg;
    } u;
    u16 len;
} UnkJob_02035754;

extern UnkJob_02035754 *gPaletteTransferHead;
extern UnkJob_02035754 *gPaletteTransferTail;
extern u32 gDisplayMain[];
extern u32 gDisplaySub[];
extern u32 data_0208a754[];

u32 GX_ResetBankForBGExtPltt(void);
u32 GX_ResetBankForOBJExtPltt(void);
u32 GX_ResetBankForSubBGExtPltt(void);
u32 GX_ResetBankForSubOBJExtPltt(void);
u32 GX_ResetBankForTexPltt(void);
void GX_SetBankForBGExtPltt(u32 s);
void GX_SetBankForOBJExtPltt(u32 s);
void GX_SetBankForSubBGExtPltt(u32 s);
void GX_SetBankForSubOBJExtPltt(u32 s);
void GX_SetBankForTexPltt(u32 s);
void MIi_CpuCopyFast(const void *src, void *dst, u32 size);
void MIi_CpuCopy32(const void *src, void *dst, u32 size);
void MIi_CpuCopy16(const void *src, void *dst, u32 size);
void MI_CpuCopy8(const void *src, void *dst, u32 size);
u32 PaletteTransfer_GetCompressionType(u32 src);
u32 PaletteTransfer_GetDataSize(u32 src);
u32 Compress_GetData(u32 src);
void Mem_Copy(u32 src, u32 dst, u32 size);
void MI_UncompressLZ16(u32 src, u32 dst);
void MI_UncompressHuffman(u32 src, u32 dst);
void MI_UncompressRL16(u32 src, u32 dst);
void DC_StoreRange(u32 addr, u32 size);
void Palette_Invert(u32 dst, u16 n);
void Palette_InvertGrayscale(u32 dst, u16 n);
void Palette_StepDownToward(u32 dst, u32 a, u16 n);
void Palette_BrightenStep(u32 dst, u16 n);
void Palette_StepUpToward(u32 dst, u32 a, u16 n);
void Palette_DarkenStep(u32 dst, u16 n);
void TransferJob_Free(UnkJob_02035754 *job);

static inline void UnkCopy_02035754(const u8 *src, u8 *d, u32 size) {
    u32 n;
    if ((((u32)src | (u32)d) & 3) == 0) {
        n = size & ~0x1f;
        if (n != 0) {
            MIi_CpuCopyFast(src, d, n);
            src += n;
            d += n;
            size &= 0x1f;
        }
        n = size & ~3;
        if (n != 0) {
            MIi_CpuCopy32(src, d, n);
            src += n;
            d += n;
            size &= 3;
        }
    }
    if ((((u32)src | (u32)d) & 1) == 0) {
        n = size & ~1;
        if (n != 0) {
            MIi_CpuCopy16(src, d, n);
            src += n;
            d += n;
            size &= 1;
        }
    }
    if (size != 0) {
        MI_CpuCopy8(src, d, size);
    }
}

void PaletteTransfer_Flush(void) {
    u32 saved[5];
    u32 flags = 0;
    while (gPaletteTransferHead != NULL) {
        UnkJob_02035754 *job = gPaletteTransferHead;
        u32 dst = 0;
        switch (job->dstType) {
        case 0:
            dst = job->dst;
            break;
        case 1: {
            u32 a = job->dst;
            u32 page = a >> 13;
            dst = a + gDisplayMain[page + 31] - (page << 13);
            if (!(flags & 2)) {
                saved[0] = GX_ResetBankForBGExtPltt();
                flags |= 2;
            }
            break;
        }
        case 2: {
            u32 a = job->dst;
            u32 page = a >> 13;
            dst = a + gDisplayMain[page + 35] - (page << 13);
            if (!(flags & 4)) {
                saved[1] = GX_ResetBankForOBJExtPltt();
                flags |= 4;
            }
            break;
        }
        case 3: {
            u32 a = job->dst;
            u32 page = a >> 13;
            dst = a + gDisplaySub[page + 31] - (page << 13);
            if (!(flags & 8)) {
                saved[2] = GX_ResetBankForSubBGExtPltt();
                flags |= 8;
            }
            break;
        }
        case 4: {
            u32 a = job->dst;
            u32 page = a >> 13;
            dst = a + gDisplaySub[page + 35] - (page << 13);
            if (!(flags & 0x10)) {
                saved[3] = GX_ResetBankForSubOBJExtPltt();
                flags |= 0x10;
            }
            break;
        }
        case 5: {
            u32 a = job->dst;
            u32 page = a >> 14;
            dst = a + data_0208a754[page + 4] - (page << 14);
            if (!(flags & 0x20)) {
                saved[4] = GX_ResetBankForTexPltt();
                flags |= 0x20;
            }
            break;
        }
        }
        switch (job->type) {
        case 0:
            UnkCopy_02035754((u8 *)job->u.src, (u8 *)dst, job->len * 2);
            break;
        case 1: {
            u32 src = job->u.src;
            switch (PaletteTransfer_GetCompressionType(src)) {
            case 0x10:
                MI_UncompressLZ16(src, dst);
                break;
            case 0x20:
                MI_UncompressHuffman(src, dst);
                break;
            case 0x30:
                MI_UncompressRL16(src, dst);
                break;
            default:
                Mem_Copy(Compress_GetData(src), dst, PaletteTransfer_GetDataSize(src));
                break;
            }
            DC_StoreRange(dst, PaletteTransfer_GetDataSize(src));
            break;
        }
        case 2:
            Palette_Invert(dst, job->u.arg);
            break;
        case 3:
            Palette_InvertGrayscale(dst, job->u.arg);
            break;
        case 4:
            Palette_StepDownToward(dst, job->u.src, job->len);
            break;
        case 5:
            Palette_BrightenStep(dst, job->u.arg);
            break;
        case 6:
            Palette_StepUpToward(dst, job->u.src, job->len);
            break;
        case 7:
            Palette_DarkenStep(dst, job->u.arg);
            break;
        }
        TransferJob_Free(gPaletteTransferHead);
        gPaletteTransferHead = gPaletteTransferHead->next;
        if (gPaletteTransferHead == NULL) {
            gPaletteTransferTail = NULL;
        }
    }
    if (flags & 2) {
        GX_SetBankForBGExtPltt(saved[0]);
    }
    if (flags & 4) {
        GX_SetBankForOBJExtPltt(saved[1]);
    }
    if (flags & 8) {
        GX_SetBankForSubBGExtPltt(saved[2]);
    }
    if (flags & 0x10) {
        GX_SetBankForSubOBJExtPltt(saved[3]);
    }
    if (flags & 0x20) {
        GX_SetBankForTexPltt(saved[4]);
    }
}
