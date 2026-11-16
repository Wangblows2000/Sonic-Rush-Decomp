#include <nitro/types.h>

typedef struct { u8 data[0x60]; } UnkArc_022d68d4;

typedef struct {
    u8 pad0[0x10c];
    u8 flags;
    u8 pad10d[0x138 - 0x10d];
} UnkObj_022d68d4;

typedef struct {
    u8 pad0[0x14];
    UnkObj_022d68d4 obj1;
    UnkObj_022d68d4 obj2;
    void *res284;
    void *res288;
    void *res28c;
    void *res290;
    u8 *buf;
    u8 *file298;
} UnkWork_022d68d4;

extern void *gMainHeap;
extern char data_ov001_0231b7fc[];
extern char data_ov001_0231b814[];
extern char data_ov001_0231b818[];
extern char data_ov001_0231b82c[];
extern u8 data_ov001_0231615c[];
extern u16 data_ov001_02316154[3];

u8 *File_Load(void *path, s32 b);
u8 *Heap_Alloc(void *heap, u32 size, s32 align);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
s32 func_ov001_022d6b50(void *p);
u32 func_ov001_022d6b44(void *p);
void MI_UncompressLZ16(void *src, void *dst);
void MI_UncompressHuffman(void *src, void *dst);
void MI_UncompressRL16(void *src, void *dst);
void *Compress_GetData(void *p);
void Mem_Copy(void *src, void *dst, u32 size);
BOOL func_02021310(UnkArc_022d68d4 *arc, const char *name, void *narc);
void *func_02021264(UnkArc_022d68d4 *arc, s32 i);
BOOL func_020212e0(UnkArc_022d68d4 *arc);
void NNS_G3dResDefaultSetup(void *res);
void Model_Init(void *o, s32 a);
void Model_SetResource(void *o, void *res, s32 a);
void Model_SetAnimation(void *o, s32 a, void *b, s32 c, s32 d);
void G3X_SetEdgeColorTable(void *p);
void func_02022270(u32 index, u32 value);

#pragma thumb on
static inline u8 *Alloc_022d68d4(u32 size) {
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

static inline void Free_022d68d4(u8 *blk) {
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

void TitleDemo_LoadArchives(UnkWork_022d68d4 *w) {
    UnkArc_022d68d4 arc;
    u8 *f;
    u8 *p;

    f = File_Load(data_ov001_0231b7fc, -1);
    w->buf = Alloc_022d68d4(((*(u32 *)f >> 8) + 0x43) & ~0x1f);
    p = w->buf;
    switch (func_ov001_022d6b50(f)) {
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
        Mem_Copy(Compress_GetData(f), p, func_ov001_022d6b44(f));
        break;
    }
    DC_StoreRange(p, func_ov001_022d6b44(f));
    Free_022d68d4(f);
    func_02021310(&arc, data_ov001_0231b814, w->buf);
    NNS_G3dResDefaultSetup(func_02021264(&arc, 0));
    w->res284 = func_02021264(&arc, 0);
    NNS_G3dResDefaultSetup(func_02021264(&arc, 3));
    w->res288 = func_02021264(&arc, 3);
    func_020212e0(&arc);
    w->file298 = File_Load(data_ov001_0231b818, -1);
    func_02021310(&arc, data_ov001_0231b82c, w->file298);
    w->res28c = func_02021264(&arc, 0);
    w->res290 = func_02021264(&arc, 1);
    func_020212e0(&arc);
    Model_Init(&w->obj1, 0);
    Model_SetResource(&w->obj1, w->res284, 0);
    Model_SetAnimation(&w->obj1, 0, w->res28c, 1, 0);
    Model_SetAnimation(&w->obj1, 4, w->res290, 1, 0);
    Model_Init(&w->obj2, 0);
    Model_SetResource(&w->obj2, w->res288, 0);
    Model_SetAnimation(&w->obj2, 0, w->res28c, 3, 0);
    Model_SetAnimation(&w->obj2, 4, w->res290, 3, 0);
    w->obj1.flags |= 1;
    w->obj2.flags |= 1;
    G3X_SetEdgeColorTable(data_ov001_0231615c);
    func_02022270(0, data_ov001_02316154[0]);
    func_02022270(1, data_ov001_02316154[1]);
    func_02022270(2, data_ov001_02316154[2]);
}
#pragma thumb reset
