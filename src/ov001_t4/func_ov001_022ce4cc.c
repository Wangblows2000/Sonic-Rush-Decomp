#include <nitro/types.h>

typedef struct {
    u32 unk0;
    u32 unk4;
    u32 unk8;
} UnkFile_022ce4cc;

typedef struct {
    u8 pad0[0x1d08];
    u8 tiles[7][0x20];
    u8 pad1de8[0x1e40 - 0x1de8];
    UnkFile_022ce4cc *res;
} UnkWork_022ce4cc;

typedef struct {
    u8 pad0[0x18];
    u16 count;
} UnkInfo_022ce4cc;

extern void *gMainHeap;
extern UnkInfo_022ce4cc gGameLanguage;
extern u16 data_02072cbc[];

u8 *Heap_Alloc(void *heap, u32 size, s32 align);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void MIi_CpuCopy32(const void *src, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
s32 Options_GetCompressionType(void *p);
u32 Options_GetUncompressedSize(void *p);
void MI_UncompressLZ16(void *src, void *dst);
void MI_UncompressHuffman(void *src, void *dst);
void MI_UncompressRL16(void *src, void *dst);
void *Compress_GetData(void *p);
void Mem_Copy(void *src, void *dst, u32 size);
s32 Font_GetGlyphIndex(u16 c);

#pragma thumb on
static inline u8 *Alloc_022ce4cc(u32 size) {
    u8 *p = Heap_Alloc(gMainHeap, size, 0x20);
    if ((u8 *)OS_GetArenaLo(0) != p) {
        vu32 fill = 0xcccccccc;
        MIi_CpuClear32(fill, p, size);
        *(u32 *)p = size;
        *(u32 *)(p + size - 4) = 0x02467531;
        DC_StoreRange(p, size);
        p += 0x20;
    }
    return p;
}

static inline void Free_022ce4cc(u8 *blk) {
    if ((u8 *)OS_GetArenaLo(0) != blk) {
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

void func_ov001_022ce4cc(UnkWork_022ce4cc *w) {
    u8 *f;
    u16 n;
    s32 i;
    u8 *buf;
    u16 *c;
    u8 *t;

    f = (u8 *)w->res + w->res->unk8;
    buf = Alloc_022ce4cc(((*(u32 *)f >> 8) + 0x43) & ~0x1f);
    switch (Options_GetCompressionType(f)) {
    case 0x10:
        MI_UncompressLZ16(f, buf);
        break;
    case 0x20:
        MI_UncompressHuffman(f, buf);
        break;
    case 0x30:
        MI_UncompressRL16(f, buf);
        break;
    default:
        Mem_Copy(Compress_GetData(f), buf, Options_GetUncompressedSize(f));
        break;
    }
    DC_StoreRange(buf, Options_GetUncompressedSize(f));
    n = gGameLanguage.count;
    i = 0;
    c = data_02072cbc;
    t = w->tiles[0];
    for (; i < n && i < 6; i++) {
        MIi_CpuCopy32(buf + Font_GetGlyphIndex(*c) * 0x20, t, 0x20);
        c++;
        t += 0x20;
    }
    if (n <= 6) {
        for (; i < 7; i++) {
            MIi_CpuCopy32(buf + Font_GetGlyphIndex(0x20) * 0x20, w->tiles[i], 0x20);
        }
    } else {
        MIi_CpuCopy32(buf + Font_GetGlyphIndex(0x2026) * 0x20, w->tiles[i], 0x20);
    }
    Free_022ce4cc(buf);
}
#pragma thumb reset
