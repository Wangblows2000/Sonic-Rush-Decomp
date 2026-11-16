#include <nitro/types.h>

typedef struct {
    u8 pad0[0x28];
    s32 n;
    u8 pad2c[0x7fc - 0x2c];
    u8 *res;
    u8 chr[11][7][0x20];
    s32 pal[11][7];
} UnkStruct_022ecae4;

typedef struct {
    u16 name[0x1c / 2];
    u8 len;
    u8 pad1d[3];
} UnkEntry_022ecae4;

typedef struct {
    u8 pad0[0x1b4];
    UnkEntry_022ecae4 e[10];
} UnkTbl_022ecae4;

typedef struct {
    u8 pad0[0x18];
    u16 len;
} UnkHdr_022ecae4;

extern u16 data_02072cbc[];
extern UnkHdr_022ecae4 gGameLanguage;
extern UnkTbl_022ecae4 data_022c46a0;
extern UnkEntry_022ecae4 data_022c4854[];
extern void *gMainHeap;
extern u32 gPaletteTransferHead;
extern u32 gPaletteTransferTail;
extern u32 gVRAMTransferHead;
extern u32 gVRAMTransferTail;

void func_ov001_022ec980(s32 *out, u16 *ids, s32 n);
u8 *Heap_Alloc(void *heap, u32 size, s32 align);
void *func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void MIi_CpuCopy32(const void *src, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
u32 func_ov001_022ecdc0(void *p);
u32 func_ov001_022ecdb4(void *p);
void func_02008648(void *src, void *dst);
void func_02008720(void *src, void *dst);
void func_020087f8(void *src, void *dst);
void *Compress_GetData(void *s);
void Mem_Copy(void *a, void *b, u32 c);
void Heap_Free(void *heap, void *p);
s32 ObjVram_Alloc(s32 a, s32 b);
void PaletteTransfer_QueueCopy(void *src, s32 a, s32 b, u32 dst);
void PaletteTransfer_Flush(void);
void VRAMTransfer_QueueCopy(void *src, s32 size, s32 a, s32 dst);
void VRAMTransfer_Flush(void);

static inline void LoadPal_022ecae4(void *src, s32 dst) {
    u32 s0;
    u32 s1;

    s0 = gVRAMTransferHead;
    s1 = gVRAMTransferTail;
    gVRAMTransferHead = 0;
    gVRAMTransferTail = 0;
    VRAMTransfer_QueueCopy(src, 0x20, 0, dst);
    VRAMTransfer_Flush();
    gVRAMTransferHead = s0;
    gVRAMTransferTail = s1;
}

#pragma thumb on
void func_ov001_022ecae4(UnkStruct_022ecae4 *p) {
    u8 *blk;
    vu32 fill1;
    vu32 fill2;
    u16 col;
    s32 i;
    s32 j;
    u8 *hdr;
    u32 size;
    u32 s0;
    u32 s1;
    s32 tbl[11][7];
    u8 *q;

    func_ov001_022ec980(tbl[0], data_02072cbc, gGameLanguage.len);
    for (i = 1, q = (u8 *)&data_022c46a0; i < 11; i++) {
        if (i <= p->n) {
            func_ov001_022ec980(tbl[i], data_022c4854[i - 1].name, q[0x1d0]);
        }
        q += 0x20;
    }
    hdr = p->res + *(u32 *)(p->res + 8);
    size = ((*(u32 *)hdr >> 8) + 0x43) & ~0x1f;
    blk = Heap_Alloc(gMainHeap, size, -0x20);
    if ((u8 *)func_02006abc(0) != blk) {
        fill1 = 0xcccccccc;
        MIi_CpuClear32(fill1, blk, size);
        *(u32 *)blk = size;
        *(u32 *)(blk + size - 4) = 0x02467531;
        DC_StoreRange(blk, size);
        blk += 0x20;
    }
    switch (func_ov001_022ecdc0(hdr)) {
    case 0x10:
        func_02008648(hdr, blk);
        break;
    case 0x20:
        func_02008720(hdr, blk);
        break;
    case 0x30:
        func_020087f8(hdr, blk);
        break;
    default:
        Mem_Copy(Compress_GetData(hdr), blk, func_ov001_022ecdb4(hdr));
        break;
    }
    DC_StoreRange(blk, func_ov001_022ecdb4(hdr));
    for (i = 0; i < p->n + 1; i++) {
        for (j = 0; j < 7; j++) {
            MIi_CpuCopy32(blk + tbl[i][j] * 0x20, p->chr[i][j], 0x20);
        }
    }
    for (i = 0; i < 11; i++) {
        for (j = 0; j < 7; j++) {
            p->pal[i][j] = ObjVram_Alloc(0, 1);
        }
    }
    col = 0;
    s0 = gPaletteTransferHead;
    s1 = gPaletteTransferTail;
    gPaletteTransferHead = 0;
    gPaletteTransferTail = 0;
    PaletteTransfer_QueueCopy(&col, 1, 0, 0x500039e);
    PaletteTransfer_Flush();
    col = 0x7fff;
    gPaletteTransferHead = 0;
    gPaletteTransferTail = 0;
    PaletteTransfer_QueueCopy(&col, 1, 0, 0x50003be);
    PaletteTransfer_Flush();
    gPaletteTransferHead = s0;
    gPaletteTransferTail = s1;
    for (i = 0; i < p->n + 1; i++) {
        for (j = 0; j < 7; j++) {
            LoadPal_022ecae4(p->chr[i][j], p->pal[i][j]);
        }
    }
    if ((u8 *)func_02006abc(0) != blk) {
        blk -= 0x20;
        size = *(u32 *)blk;
        if (*(u32 *)(blk + size - 4) == 0x02467531) {
            fill2 = 0xcccccccc;
            MIi_CpuClear32(fill2, blk, size);
            DC_StoreRange(blk, size);
        }
    }
    Heap_Free(gMainHeap, blk);
}
#pragma thumb reset
