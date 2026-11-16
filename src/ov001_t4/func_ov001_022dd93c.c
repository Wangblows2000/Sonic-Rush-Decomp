#include <nitro/types.h>

typedef struct {
    u32 unk0;
    u32 unk4;
    u32 unk8;
} UnkFile_022dd93c;

typedef struct {
    u8 pad0[0x20];
    s32 h[2];
} UnkA_022dd93c;

typedef struct {
    s32 idx;
    u8 pad4[0x1c];
    s32 unk20;
    s32 unk24;
    s32 unk28;
    s32 h[2];
} UnkB_022dd93c;

typedef struct {
    u8 pad0[0x20];
    UnkA_022dd93c a[2];
    UnkB_022dd93c b[8];
    u8 pad210[0xdac - 0x210];
    u8 *buf;
    u8 padxx[0xdc4 - 0xdb0];
    UnkFile_022dd93c *res;
} UnkWork_022dd93c;

typedef struct {
    u8 pad0[0x18];
    u16 count;
} UnkInfo_022dd93c;

extern void *gMainHeap;
extern UnkInfo_022dd93c gGameLanguage;
extern u16 data_02072cbc[];

u8 *Heap_Alloc(void *heap, u32 size, s32 align);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void MIi_CpuCopy32(const void *src, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
s32 func_ov001_022ddac0(void *p);
u32 func_ov001_022ddab4(void *p);
void MI_UncompressLZ16(void *src, void *dst);
void MI_UncompressHuffman(void *src, void *dst);
void MI_UncompressRL16(void *src, void *dst);
void *Compress_GetData(void *p);
void Mem_Copy(void *src, void *dst, u32 size);
s32 ObjVram_Alloc(s32 a, s32 b);
void func_ov001_022d9a14(UnkWork_022dd93c *w, s32 a);
void func_ov001_022d983c(UnkWork_022dd93c *w, s32 i);
void func_ov001_022d9814(UnkWork_022dd93c *w, s32 i);
void func_ov001_022d9a54(void *p, u16 *tbl, u16 n);

#pragma thumb on
static inline u8 *Alloc_022dd93c(u32 size) {
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

void func_ov001_022dd93c(UnkWork_022dd93c *w) {
    u8 *f;
    u8 *p;
    s32 i;
    s32 j;

    f = (u8 *)w->res + w->res->unk8;
    w->buf = Alloc_022dd93c(((*(u32 *)f >> 8) + 0x43) & ~0x1f);
    p = w->buf;
    switch (func_ov001_022ddac0(f)) {
    case 0x10:
        MI_UncompressLZ16(f, p);
        break;
    case 0x20:
        MI_UncompressHuffman(f, p);
        break;
    case 0x30:
        MI_UncompressRL16(f, p);
        break;
    default:
        Mem_Copy(Compress_GetData(f), p, func_ov001_022ddab4(f));
        break;
    }
    DC_StoreRange(p, func_ov001_022ddab4(f));
    func_ov001_022d9a14(w, 0);
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            w->a[i].h[j] = ObjVram_Alloc(1, 1);
        }
        func_ov001_022d983c(w, i);
    }
    for (i = 0; i < 8; i++) {
        w->b[i].unk20 = 0;
        w->b[i].idx = i;
        w->b[i].unk24 = 2;
        w->b[i].unk28 = 0;
        for (j = 0; j < 2; j++) {
            w->b[i].h[j] = ObjVram_Alloc(1, 1);
        }
        func_ov001_022d9814(w, i);
    }
    func_ov001_022d9a54((u8 *)w + 0x24, data_02072cbc, gGameLanguage.count);
}
#pragma thumb reset
