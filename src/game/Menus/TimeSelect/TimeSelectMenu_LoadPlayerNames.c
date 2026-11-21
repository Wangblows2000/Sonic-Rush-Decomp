#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    s32 unk8;
} UnkStruct_0235fd2c_Res;

typedef struct {
    u8 pad0[4];
    s32 unk10;
    u8 pad8[0xDFC - 0x8];
    u8 tiles[2][7][0x20];
    u8 padFBC[0xFD8 - 0xFBC];
    void *unkDDC[2][7];
    u8 pad1010[0x1038 - 0x1010];
    UnkStruct_0235fd2c_Res *unkE30;
} UnkStruct_0235fd2c;

typedef struct {
    u8 pad0[0x18];
    u16 unk18;
} UnkStruct_02072cb8;

extern void *gMainHeap;
extern UnkStruct_02072cb8 gGameLanguage;
extern u16 data_02072cbc[];
extern u32 gPaletteTransferHead;
extern u32 gPaletteTransferTail;
extern void *gVRAMTransferHead;
extern void *gVRAMTransferTail;

void TimeSelectMenu_GetNameGlyphs(u32 *out, u16 *src, s32 n);
s32 WirelessSession_GetPartnerName(u16 *buf, s32 n);
void *Heap_Alloc(void *heap, u32 size, s32 align);
void *OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void MIi_CpuCopy32(const void *src, void *dst, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
u32 TimeSelectMenu_GetCompressionType(void *res);
u32 TimeSelectMenu_GetCompressedSize(void *res);
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

static inline void *UnkAlloc_0235fd2c(u32 size, s32 align) {
    u8 *p;
    size = (size + 0x43) & ~0x1f;
    p = Heap_Alloc(gMainHeap, size, align);
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

static inline void UnkFree_0235fd2c(void *p) {
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
    Heap_Free(gMainHeap, p);
}

static inline void UnkLoad_0235fd2c(void *src, void *dst) {
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

void TimeSelectMenu_LoadPlayerNames(UnkStruct_0235fd2c *w) {
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
        TimeSelectMenu_GetNameGlyphs(idx[0], data_02072cbc, gGameLanguage.unk18);
        TimeSelectMenu_GetNameGlyphs(idx[1], name, (u16)WirelessSession_GetPartnerName(name, 7));
    } else {
        TimeSelectMenu_GetNameGlyphs(idx[1], data_02072cbc, gGameLanguage.unk18);
        TimeSelectMenu_GetNameGlyphs(idx[0], name, (u16)WirelessSession_GetPartnerName(name, 7));
    }
    src = (u8 *)w->unkE30 + w->unkE30->unk8;
    {
        u32 size = ((*(u32 *)src >> 8) + 0x43) & ~0x1f;
        dst = Heap_Alloc(gMainHeap, size, -0x20);
        if (OS_GetArenaLo(0) != dst) {
            vu32 fill = 0xcccccccc;
            MIi_CpuClear32(fill, dst, size);
            *(u32 *)dst = size;
            *(u32 *)((u8 *)dst + size - 4) = 0x2467531;
            DC_StoreRange(dst, size);
            dst = (u8 *)dst + 0x20;
        }
    }
    switch (TimeSelectMenu_GetCompressionType(src)) {
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
        Mem_Copy(Compress_GetData(src), dst, TimeSelectMenu_GetCompressedSize(src));
        break;
    }
    DC_StoreRange(dst, TimeSelectMenu_GetCompressedSize(src));
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
            UnkLoad_0235fd2c(w->tiles[j][i], w->unkDDC[j][i]);
        }
    }
    UnkFree_0235fd2c(dst);
}

#pragma thumb reset
