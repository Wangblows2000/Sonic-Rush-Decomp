#include <game/Menus/mdl.h>

typedef struct {
    UnkStruct_MdlRender r;
    u8 pad98[0x10C - sizeof(UnkStruct_MdlRender)];
    u8 flag;
    u8 pad10D[0x138 - 0x10D];
    void *mdl;
    void *anm0;
    void *anm1;
    u8 pad144[4];
} UnkStruct_02360408_Slot;

typedef struct {
    u8 pad0[0xB68];
    UnkStruct_02360408_Slot slot[2];
    void *unkDF8;
} UnkStruct_02360408;

typedef struct {
    void *unk0;
    u8 pad4[4];
    void *unk8;
} UnkStruct_0236ac50;

extern void *gMainHeap;
extern char data_ov000_02369a88[];
extern u8 data_ov000_02369aa0[];
extern UnkStruct_0236ac50 data_ov000_0236ac50;
extern void *data_ov000_0236ac40;
extern void *data_ov000_0236ac48;
extern u8 data_ov000_02364ee8[];
extern u16 data_ov000_02364ee0[];

BOOL TimeSelectMenu_PollDownloadPlay(void);
void *File_Load(const char *path, void *dst);
void *Heap_Alloc(void *heap, u32 size, s32 align);
void *OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
u32 TimeSelectMenu_GetCompressionType(void *res);
u32 TimeSelectMenu_GetCompressedSize(void *res);
void MI_UncompressLZ16(void *src, void *dst);
void MI_UncompressHuffman(void *src, void *dst);
void MI_UncompressRL16(void *src, void *dst);
void *Compress_GetData(void *res);
void Mem_Copy(void *src, void *dst, u32 size);
void NNS_FndMountArchive(void *buf, void *a, void *b);
void *NNS_FndGetArchiveFileByIndex(void *buf, s32 a);
void NNS_G3dResDefaultSetup(void);
void NNS_FndUnmountArchive(void *buf);
void Model_Init(UnkStruct_MdlRender *r, s32 a);
void Model_SetResource(UnkStruct_MdlRender *r, void *mdl, s32 a);
void G3X_SetEdgeColorTable(void *p);
void NNS_G3dGlbLightColor(s32 idx, u16 val);

static inline void *UnkAlloc_02360408(u32 size, s32 align) {
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

static inline void UnkFree_02360408(void *p) {
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

#pragma thumb on

void TimeSelectMenu_LoadArchives(UnkStruct_02360408 *w) {
    vu32 zero;
    void *res;
    void *dst;
    u8 buf[0x5c];

    if (TimeSelectMenu_PollDownloadPlay()) {
        zero = 0;
        MIi_CpuClear32(zero, w->slot, sizeof(w->slot));
        w->slot[0].mdl = data_ov000_0236ac50.unk0;
        w->slot[0].anm0 = data_ov000_0236ac40;
        w->slot[0].anm1 = data_ov000_0236ac48;
        w->slot[1].mdl = data_ov000_0236ac50.unk8;
        w->slot[1].anm0 = data_ov000_0236ac40;
        w->slot[1].anm1 = data_ov000_0236ac48;
    } else {
        res = File_Load(data_ov000_02369a88, (void *)-1);
        w->unkDF8 = UnkAlloc_02360408(*(u32 *)res >> 8, 0x20);
        dst = w->unkDF8;
        switch (TimeSelectMenu_GetCompressionType(res)) {
        case 0x10:
            MI_UncompressLZ16(res, dst);
            break;
        case 0x20:
            MI_UncompressHuffman(res, dst);
            break;
        case 0x30:
            MI_UncompressRL16(res, dst);
            break;
        default:
            Mem_Copy(Compress_GetData(res), dst, TimeSelectMenu_GetCompressedSize(res));
            break;
        }
        DC_StoreRange(dst, TimeSelectMenu_GetCompressedSize(res));
        UnkFree_02360408(res);
        NNS_FndMountArchive(buf, data_ov000_02369aa0, w->unkDF8);
        NNS_FndGetArchiveFileByIndex(buf, 0);
        NNS_G3dResDefaultSetup();
        w->slot[0].mdl = NNS_FndGetArchiveFileByIndex(buf, 0);
        w->slot[0].anm0 = NNS_FndGetArchiveFileByIndex(buf, 1);
        w->slot[0].anm1 = NNS_FndGetArchiveFileByIndex(buf, 2);
        NNS_FndGetArchiveFileByIndex(buf, 3);
        NNS_G3dResDefaultSetup();
        w->slot[1].mdl = NNS_FndGetArchiveFileByIndex(buf, 3);
        w->slot[1].anm0 = NNS_FndGetArchiveFileByIndex(buf, 1);
        w->slot[1].anm1 = NNS_FndGetArchiveFileByIndex(buf, 2);
        NNS_FndUnmountArchive(buf);
    }
    Model_Init(&w->slot[0].r, 0);
    Model_SetResource(&w->slot[0].r, w->slot[0].mdl, 0);
    Model_SetAnimation(&w->slot[0].r, 0, w->slot[0].anm0, 0, NULL);
    Model_SetAnimation(&w->slot[0].r, 4, w->slot[0].anm1, 1, NULL);
    Model_Init(&w->slot[1].r, 0);
    Model_SetResource(&w->slot[1].r, w->slot[1].mdl, 0);
    Model_SetAnimation(&w->slot[1].r, 0, w->slot[1].anm0, 0x37, NULL);
    Model_SetAnimation(&w->slot[1].r, 4, w->slot[1].anm1, 1, NULL);
    w->slot[0].flag |= 4;
    w->slot[1].flag |= 4;
    G3X_SetEdgeColorTable(data_ov000_02364ee8);
    NNS_G3dGlbLightColor(0, data_ov000_02364ee0[0]);
    NNS_G3dGlbLightColor(1, data_ov000_02364ee0[1]);
    NNS_G3dGlbLightColor(2, data_ov000_02364ee0[2]);
}

#pragma thumb reset
