#include <nitro/types.h>

extern void *data_ov000_0236aab8;
extern u8 data_ov000_02368804[];
extern u32 gPaletteTransferHead;
extern u32 gPaletteTransferTail;

void NNS_FndMountArchive(void *f, void *path, void *arc);
void *NNS_FndGetArchiveFileByIndex(void *f, s32 i);
void NNS_FndUnmountArchive(void *f);
s32 StageLoader_GetCompressionType(void *res);
u32 StageLoader_GetUncompressedSize(void *res);
void MI_UncompressLZ16(void *src, void *dst);
void MI_UncompressHuffman(void *src, void *dst);
void MI_UncompressRL16(void *src, void *dst);
void *Compress_GetData(void *res);
void Mem_Copy(void *src, void *dst, u32 size);
void DC_StoreRange(void *p, u32 size);
void PaletteTransfer_QueueLoad(void *res, s32 a, u32 b);
void PaletteTransfer_Flush(void);
void StageLoader_UnpackMapArchive(void *f);

void StageLoader_LoadBossMapGraphics(void) {
    u8 f[0x5C];
    void *res;
    u32 sa;
    u32 sb;

    NNS_FndMountArchive(f, data_ov000_02368804, data_ov000_0236aab8);
    res = NNS_FndGetArchiveFileByIndex(f, 3);
    switch (StageLoader_GetCompressionType(res)) {
    case 0x10:
        MI_UncompressLZ16(res, (void *)0x6010000);
        break;
    case 0x20:
        MI_UncompressHuffman(res, (void *)0x6010000);
        break;
    case 0x30:
        MI_UncompressRL16(res, (void *)0x6010000);
        break;
    default:
        Mem_Copy(Compress_GetData(res), (void *)0x6010000, StageLoader_GetUncompressedSize(res));
        break;
    }
    DC_StoreRange((void *)0x6010000, StageLoader_GetUncompressedSize(res));
    res = NNS_FndGetArchiveFileByIndex(f, 6);
    sa = gPaletteTransferHead;
    sb = gPaletteTransferTail;
    gPaletteTransferHead = 0;
    gPaletteTransferTail = 0;
    PaletteTransfer_QueueLoad(res, 0, 0x5000000);
    PaletteTransfer_Flush();
    gPaletteTransferHead = sa;
    gPaletteTransferTail = sb;
    StageLoader_UnpackMapArchive(f);
    NNS_FndUnmountArchive(f);
}
