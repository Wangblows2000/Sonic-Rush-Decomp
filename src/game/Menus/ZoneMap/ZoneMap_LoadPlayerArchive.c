#include <nitro/types.h>

typedef struct { u8 data[0x60]; } UnkArc_022e3bec;

typedef struct {
    s32 mode;
    s32 unk4;
    u8 pad8[0x10];
    s32 unk18;
    s32 unk1c;
    s32 unk20;
    u8 obj[0x130 - 0x24];
    u8 flags130;
    u8 pad131[7];
    s32 unk138;
    u8 pad13c[0x15c - 0x13c];
    u8 *buf;
    void *res160;
    void *res164;
    void *res168;
    void *res16c;
    void *res170;
} UnkWork_022e3bec;

extern void *gMainHeap;
extern char data_ov001_023162e0[];
extern char data_ov001_0231c57c[];
extern char data_ov001_0231c580[];
extern char data_ov001_0231c584[];
extern void *data_ov001_023162ac;
extern void *data_ov001_023162c4;
extern void *data_ov001_023162b8;
extern void *data_ov001_023162a0;
extern u8 data_ov001_023162d0[];
extern u16 data_ov001_0231628c[3];

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
BOOL NNS_FndMountArchive(UnkArc_022e3bec *arc, const char *name, void *narc);
void *NNS_FndGetArchiveFileByIndex(UnkArc_022e3bec *arc, s32 i);
BOOL NNS_FndUnmountArchive(UnkArc_022e3bec *arc);
void NNS_G3dResDefaultSetup(void *res);
void Model_Init(void *o, s32 a);
void Model_SetResource(void *o, void *res, s32 a);
void Model_SetAnimation(void *o, s32 a, void *b, void *c, s32 d);
void G3X_SetEdgeColorTable(void *p);
void NNS_G3dGlbLightColor(u32 index, u32 value);

#pragma thumb on
static inline u8 *Alloc_022e3bec(u32 size) {
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

static inline void Free_022e3bec(u8 *blk) {
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

void ZoneMap_LoadPlayerArchive(UnkWork_022e3bec *w, s32 mode, void *narc) {
    UnkArc_022e3bec arc;
    u8 *f;
    u8 *p;

    w->mode = mode;
    w->unk4 = 1;
    f = File_Load(data_ov001_023162e0, -1);
    w->buf = Alloc_022e3bec(((*(u32 *)f >> 8) + 0x43) & ~0x1f);
    p = w->buf;
    switch (ZoneMap_GetCompressionType(f)) {
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
        Mem_Copy(Compress_GetData(f), p, ZoneMap_GetUncompressedSize(f));
        break;
    }
    DC_StoreRange(p, ZoneMap_GetUncompressedSize(f));
    Free_022e3bec(f);
    if (mode == 0) {
        NNS_FndMountArchive(&arc, data_ov001_0231c57c, w->buf);
        NNS_G3dResDefaultSetup(NNS_FndGetArchiveFileByIndex(&arc, 0));
        w->res160 = NNS_FndGetArchiveFileByIndex(&arc, 0);
        w->res164 = NNS_FndGetArchiveFileByIndex(&arc, 1);
        w->res168 = NNS_FndGetArchiveFileByIndex(&arc, 2);
        NNS_FndUnmountArchive(&arc);
    } else {
        NNS_FndMountArchive(&arc, data_ov001_0231c580, w->buf);
        NNS_G3dResDefaultSetup(NNS_FndGetArchiveFileByIndex(&arc, 3));
        w->res160 = NNS_FndGetArchiveFileByIndex(&arc, 3);
        w->res164 = NNS_FndGetArchiveFileByIndex(&arc, 1);
        w->res168 = NNS_FndGetArchiveFileByIndex(&arc, 2);
        NNS_FndUnmountArchive(&arc);
    }
    NNS_FndMountArchive(&arc, data_ov001_0231c584, narc);
    w->res16c = NNS_FndGetArchiveFileByIndex(&arc, 0);
    w->res170 = NNS_FndGetArchiveFileByIndex(&arc, 1);
    NNS_FndUnmountArchive(&arc);
    Model_Init(w->obj, 0);
    w->flags130 |= 4;
    if (mode == 0) {
        Model_SetResource(w->obj, w->res160, 0);
        Model_SetAnimation(w->obj, 0, w->res164, data_ov001_023162ac, 0);
        Model_SetAnimation(w->obj, 4, w->res168, data_ov001_023162c4, 0);
    } else {
        Model_SetResource(w->obj, w->res160, 0);
        Model_SetAnimation(w->obj, 0, w->res164, data_ov001_023162b8, 0);
        Model_SetAnimation(w->obj, 4, w->res168, data_ov001_023162a0, 0);
    }
    w->unk20 = 0;
    w->unk1c = w->unk20;
    w->unk138 = 0x1000;
    w->unk18 = 0x1f;
    G3X_SetEdgeColorTable(data_ov001_023162d0);
    NNS_G3dGlbLightColor(0, data_ov001_0231628c[0]);
    NNS_G3dGlbLightColor(1, data_ov001_0231628c[1]);
    NNS_G3dGlbLightColor(2, data_ov001_0231628c[2]);
}
#pragma thumb reset
