#include <nitro/types.h>

typedef struct {
    u32 unk0;
    u8 unk4[1];
} UnkScr_020363f0;

typedef struct UnkJob_020363f0 {
    struct UnkJob_020363f0 *next;
    u16 type;
    u16 unk6;
    u32 unk8;
    u32 unkC;
    u16 unk10;
    u16 unk12;
    UnkScr_020363f0 *src;
    u32 unk18;
    u32 unk1c;
} UnkJob_020363f0;

extern UnkJob_020363f0 *gScreenTransferHead;
extern UnkJob_020363f0 *gScreenTransferTail;

void Mem_CopyRect(u32 src, u32 srcStride, u32 dst, u32 dstStride, u16 size, u16 rows);
u32 ScreenTransfer_GetCompressionType(void *src);
u32 ScreenTransfer_GetDataSize(void *src);
u32 Compress_GetData(void *src);
void Mem_Copy(u32 src, u32 dst, u32 size);
void MI_UncompressLZ16(void *src, u32 dst);
void MI_UncompressHuffman(void *src, u32 dst);
void MI_UncompressRL16(void *src, u32 dst);
void DC_StoreRange(u32 addr, u32 size);
void TransferJob_Free(UnkJob_020363f0 *job);

void ScreenTransfer_Flush(void) {
    while (gScreenTransferHead != NULL) {
        UnkJob_020363f0 *j = gScreenTransferHead;
        switch (j->type) {
        case 0:
            Mem_CopyRect((u32)j->src + j->unk1c, j->unk18, j->unkC + j->unk8, j->unk6, j->unk10, j->unk12);
            break;
        case 1: {
            UnkScr_020363f0 *src = j->src;
            if (src->unk0 & 0xf0) {
                u32 dst = j->unkC + j->unk8;
                switch (ScreenTransfer_GetCompressionType(src)) {
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
                    Mem_Copy(Compress_GetData(src), dst, ScreenTransfer_GetDataSize(src));
                    break;
                }
                DC_StoreRange(dst, ScreenTransfer_GetDataSize(src));
            } else {
                Mem_CopyRect(j->unk1c + (u32)src->unk4, j->unk18, j->unkC + j->unk8, j->unk6, j->unk10, j->unk12);
            }
            break;
        }
        }
        TransferJob_Free(gScreenTransferHead);
        gScreenTransferHead = gScreenTransferHead->next;
        if (gScreenTransferHead == NULL) {
            gScreenTransferTail = NULL;
        }
    }
}
