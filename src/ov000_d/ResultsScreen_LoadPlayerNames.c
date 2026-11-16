#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    s32 unk8;
} UnkStruct_0235d1a4_Res;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    u8 pad14[0xC1C - 0x14];
    u8 tiles[2][7][0x20];
    void *unkDDC[2][7];
    u8 padE14[0xE30 - 0xE14];
    UnkStruct_0235d1a4_Res *unkE30;
} UnkStruct_0235d1a4;

typedef struct {
    u8 pad0[0x18];
    u16 unk18;
} UnkStruct_02072cb8;

extern void *gSystemHeap;
extern UnkStruct_02072cb8 gGameLanguage;
extern u16 data_02072cbc[];
extern u32 gPaletteTransferHead;
extern u32 gPaletteTransferTail;
extern void *gVRAMTransferHead;
extern void *gVRAMTransferTail;

void ResultsScreen_GetNameGlyphs(u32 *out, u16 *src, s32 n);
s32 func_02051300(u16 *buf, s32 n);
void *Heap_Alloc(void *heap, u32 size, s32 align);
void *OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void MIi_CpuCopy32(const void *src, void *dst, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
u32 ResultsScreen_GetCompressionType(void *res);
u32 ResultsScreen_GetCompressedSize(void *res);
void MI_UncompressLZ16(void *src, void *dst);
void MI_UncompressHuffman(void *src, void *dst);
void MI_UncompressRL16(void *src, void *dst);
void *Compress_GetData(void *res);
void Mem_Copy(void *src, void *dst, u32 size);
void *ObjVram_Alloc(s32 a, s32 b);
void PaletteTransfer_QueueCopy(void *src, s32 a, s32 b, u32 dst);
void PaletteTransfer_Flush(void);
void VRAMTransfer_QueueCopy(void *src, s32 size, s32 a, void *dst);
void VRAMTransfer_Flush(void);

static inline void *UnkAlloc_0235d1a4(u32 size, s32 align) {
    u8 *p;
    size = (size + 0x43) & ~0x1f;
    p = Heap_Alloc(gSystemHeap, size, align);
    if (OS_GetArenaLo(0) != p) {
        vu32 fill = 0xcccccccc;
        MIi_CpuClear32(fill, p, size);
        *(u32 *)p = size;
        *(u32 *)(p + size - 4) = 0x2467531;
        DC_StoreRange(p, size);
        p += 0x20;
    }
    return p;
}

static inline void UnkFree_0235d1a4(void *p) {
    u32 size;
    if (OS_GetArenaLo(0) != p) {
        p = (u8 *)p - 0x20;
        size = *(u32 *)p;
        if (*(u32 *)((u8 *)p + size - 4) == 0x2467531) {
            vu32 fill = 0xcccccccc;
            MIi_CpuClear32(fill, p, size);
            DC_StoreRange(p, size);
        }
    }
    Heap_Free(gSystemHeap, p);
}

static inline void UnkLoad_0235d1a4(void *src, void *dst) {
    void *pa = gVRAMTransferHead;
    void *pb = gVRAMTransferTail;
    gVRAMTransferHead = NULL;
    gVRAMTransferTail = NULL;
    VRAMTransfer_QueueCopy(src, 0x20, 0, dst);
    VRAMTransfer_Flush();
    gVRAMTransferHead = pa;
    gVRAMTransferTail = pb;
}

#pragma thumb on

void ResultsScreen_LoadPlayerNames(UnkStruct_0235d1a4 *w) {
    void *dst;
    s32 j;
    s32 i;
    u32 sa;
    u32 sb;
    vu16 zero;
    u16 name[8];
    u32 idx[2][7];
    void *src;

    if (w->unk10) {
        ResultsScreen_GetNameGlyphs(idx[0], data_02072cbc, gGameLanguage.unk18);
        ResultsScreen_GetNameGlyphs(idx[1], name, (u16)func_02051300(name, 7));
    } else {
        ResultsScreen_GetNameGlyphs(idx[1], data_02072cbc, gGameLanguage.unk18);
        ResultsScreen_GetNameGlyphs(idx[0], name, (u16)func_02051300(name, 7));
    }
    src = (u8 *)w->unkE30 + w->unkE30->unk8;
    {
        u32 size = ((*(u32 *)src >> 8) + 0x43) & ~0x1f;
        dst = Heap_Alloc(gSystemHeap, size, -0x20);
        if (OS_GetArenaLo(0) != dst) {
            vu32 fill = 0xcccccccc;
            MIi_CpuClear32(fill, dst, size);
            *(u32 *)dst = size;
            *(u32 *)((u8 *)dst + size - 4) = 0x2467531;
            DC_StoreRange(dst, size);
            dst = (u8 *)dst + 0x20;
        }
    }
    switch (ResultsScreen_GetCompressionType(src)) {
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
        Mem_Copy(Compress_GetData(src), dst, ResultsScreen_GetCompressedSize(src));
        break;
    }
    DC_StoreRange(dst, ResultsScreen_GetCompressedSize(src));
    for (j = 0; j < 2; j++) {
        for (i = 0; i < 7; i++) {
            MIi_CpuCopy32((u8 *)dst + (idx[j][i] << 5), w->tiles[j][i], 0x20);
        }
    }
    for (j = 0; j < 2; j++) {
        for (i = 0; i < 7; i++) {
            w->unkDDC[j][i] = ObjVram_Alloc(0, 1);
        }
    }
    zero = 0;
    sa = gPaletteTransferHead;
    sb = gPaletteTransferTail;
    gPaletteTransferHead = 0;
    gPaletteTransferTail = 0;
    PaletteTransfer_QueueCopy((void *)&zero, 1, 0, 0x500039e);
    PaletteTransfer_Flush();
    gPaletteTransferHead = sa;
    gPaletteTransferTail = sb;
    for (j = 0; j < 2; j++) {
        for (i = 0; i < 7; i++) {
            UnkLoad_0235d1a4(w->tiles[j][i], w->unkDDC[j][i]);
        }
    }
    UnkFree_0235d1a4(dst);
}

#pragma thumb reset
