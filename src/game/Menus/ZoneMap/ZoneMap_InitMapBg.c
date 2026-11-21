#include <nitro/types.h>

typedef struct {
    u8 pad0[0xc];
    u32 unkc;
    u8 pad10[8];
    u16 w;
    u16 h;
} UnkRes_022e5684;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkc;
    u8 bg[0x40];
    u8 *buf;
    u8 pad54[4];
    s32 unk58;
    u8 pad5c[8];
    s32 unk64;
    s32 unk68;
    s32 unk6c;
    u8 pad70[0x670 - 0x70];
} UnkWork_022e5684;

extern void *gMainHeap;
u8 *File_Load(void *path, s32 b);
u8 *Heap_Alloc(void *heap, u32 size, s32 align);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
s32 ZoneMap_GetCompressionType(void *p);
u32 ZoneMap_GetUncompressedSize(void *p);
void MI_UncompressLZ16(void *src, void *dst);
void MI_UncompressHuffman(void *src, void *dst);
void MI_UncompressRL16(void *src, void *dst);
void *Compress_GetData(void *p);
void Mem_Copy(void *src, void *dst, u32 size);
void Background_Init(void *bg, void *res, s32 a, s32 b, s32 c, u16 w, u16 h);
void Background_Draw(void *bg);
void ZoneMap_StartBg3XHdma(UnkWork_022e5684 *w);

#pragma thumb on
static inline u8 *Alloc_022e5684(u32 size) {
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

void ZoneMap_InitMapBg(UnkWork_022e5684 *w, UnkRes_022e5684 *res) {
    u8 *p;
    vu32 z;

    z = 0;
    MIi_CpuClear32(z, w, 0x670);
    w->unk0 = 8;
    w->unk4 = 0;
    w->unk64 = 1;
    w->unk68 = 1;
    w->unk6c = 0xffff;
    w->unk58 = 1;
    Background_Init(w->bg, res, 0x1c, 0, 3, 0x40, res->h);
    Background_Draw(w->bg);
    w->unk8 = res->w << 3;
    w->unkc = res->h << 3;
    w->buf = Alloc_022e5684(((*(u32 *)((u8 *)res + res->unkc) >> 8) + 0x43) & ~0x1f);
    p = w->buf;
    res = (UnkRes_022e5684 *)((u8 *)res + res->unkc);
    switch (ZoneMap_GetCompressionType(res)) {
    case 0x10:
        MI_UncompressLZ16(res, p);
        break;
    case 0x20:
        MI_UncompressHuffman(res, p);
        break;
    case 0x30:
        MI_UncompressRL16(res, p);
        break;
    default:
        Mem_Copy(Compress_GetData(res), p, ZoneMap_GetUncompressedSize(res));
        break;
    }
    DC_StoreRange(p, ZoneMap_GetUncompressedSize(res));
    ZoneMap_StartBg3XHdma(w);
}
#pragma thumb reset
