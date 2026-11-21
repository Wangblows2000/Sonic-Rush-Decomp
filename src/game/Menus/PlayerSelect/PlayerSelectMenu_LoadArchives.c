#include <game/Menus/mdl.h>

extern void *gMainHeap;
extern const char *data_ov000_02368e00;
extern const char *data_ov000_02368dfc;
extern const char *data_ov000_02368df8;
extern u8 data_ov000_023690d8[];
extern u8 data_ov000_023690dc[];
extern u8 data_ov000_023690e0[];
extern u8 data_ov000_02364eb8[];
extern u16 data_ov000_02364eb0[];

void *File_Load(const char *path, void *dst);
void *Heap_Alloc(void *heap, u32 size, s32 align);
void *OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
u32 PlayerSelectMenu_GetCompressionType(void *res);
u32 PlayerSelectMenu_GetCompressedSize(void *res);
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

static inline void *UnkAlloc_02357458(u32 size, s32 align) {
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

static inline void UnkFree_02357458(void *p) {
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

void PlayerSelectMenu_LoadArchives(UnkStruct_Mdl *w, BOOL flag) {
    vu32 zero;
    void *res;
    void *dst;
    u8 buf0[0x5c];
    u8 buf1[0x5c];
    u8 buf2[0x5c];

    zero = 0;
    MIi_CpuClear32(zero, w, sizeof(UnkStruct_Mdl));
    res = File_Load(data_ov000_02368e00, (void *)-1);
    w->unk2F8 = UnkAlloc_02357458(*(u32 *)res >> 8, 0x20);
    dst = w->unk2F8;
    switch (PlayerSelectMenu_GetCompressionType(res)) {
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
        Mem_Copy(Compress_GetData(res), dst, PlayerSelectMenu_GetCompressedSize(res));
        break;
    }
    DC_StoreRange(dst, PlayerSelectMenu_GetCompressedSize(res));
    UnkFree_02357458(res);
    w->unk2FC = File_Load(data_ov000_02368dfc, (void *)-1);
    if (flag) {
        w->unk300 = File_Load(data_ov000_02368df8, (void *)-1);
    }
    NNS_FndMountArchive(buf0, data_ov000_023690d8, w->unk2F8);
    NNS_FndMountArchive(buf1, data_ov000_023690dc, w->unk2FC);
    if (flag) {
        NNS_FndMountArchive(buf2, data_ov000_023690e0, w->unk300);
    }
    NNS_FndGetArchiveFileByIndex(buf0, 0);
    NNS_G3dResDefaultSetup();
    w->slot[0].mdl14C = NNS_FndGetArchiveFileByIndex(buf0, 0);
    w->slot[0].anm150 = NNS_FndGetArchiveFileByIndex(buf1, 0);
    w->slot[0].anm154 = NNS_FndGetArchiveFileByIndex(buf1, 1);
    if (flag) {
        w->slot[0].anm158 = NNS_FndGetArchiveFileByIndex(buf2, 0);
        w->slot[0].anm15C = NNS_FndGetArchiveFileByIndex(buf2, 1);
    } else {
        w->slot[0].anm158 = NULL;
        w->slot[0].anm15C = NULL;
    }
    NNS_FndGetArchiveFileByIndex(buf0, 3);
    NNS_G3dResDefaultSetup();
    w->slot[1].mdl14C = NNS_FndGetArchiveFileByIndex(buf0, 3);
    w->slot[1].anm150 = NNS_FndGetArchiveFileByIndex(buf1, 0);
    w->slot[1].anm154 = NNS_FndGetArchiveFileByIndex(buf1, 1);
    if (flag) {
        w->slot[1].anm158 = NNS_FndGetArchiveFileByIndex(buf2, 0);
        w->slot[1].anm15C = NNS_FndGetArchiveFileByIndex(buf2, 1);
    } else {
        w->slot[1].anm158 = NULL;
        w->slot[1].anm15C = NULL;
    }
    if (flag) {
        NNS_FndUnmountArchive(buf2);
    }
    NNS_FndUnmountArchive(buf1);
    NNS_FndUnmountArchive(buf0);
    Model_Init(&w->slot[0].r, 0);
    Model_SetResource(&w->slot[0].r, w->slot[0].mdl14C, 0);
    Model_SetAnimation(&w->slot[0].r, 0, w->slot[0].anm150, 0, NULL);
    Model_SetAnimation(&w->slot[0].r, 4, w->slot[0].anm154, 0, NULL);
    Model_Init(&w->slot[1].r, 0);
    Model_SetResource(&w->slot[1].r, w->slot[1].mdl14C, 0);
    Model_SetAnimation(&w->slot[1].r, 0, w->slot[1].anm150, 4, NULL);
    Model_SetAnimation(&w->slot[1].r, 4, w->slot[1].anm154, 4, NULL);
    w->x = 0;
    w->y = 0;
    w->z = 0;
    w->scale = 0x1a00;
    w->rx = 0;
    w->ry = 0;
    w->rz = 0;
    w->mode = 0;
    w->unk2C = 0;
    w->sel = 0;
    w->prevSel = 0;
    w->slot[0].cur = 0;
    w->slot[0].prev = -1;
    w->slot[0].flag = 0;
    w->slot[0].angOfs = 0xb1c7;
    w->slot[1].cur = 4;
    w->slot[1].prev = -1;
    w->slot[1].flag = 0;
    w->slot[1].angOfs = 0xae38;
    w->slot[0].unk120 |= 4;
    w->slot[1].unk120 |= 4;
    G3X_SetEdgeColorTable(data_ov000_02364eb8);
    NNS_G3dGlbLightColor(0, data_ov000_02364eb0[0]);
    NNS_G3dGlbLightColor(1, data_ov000_02364eb0[1]);
    NNS_G3dGlbLightColor(2, data_ov000_02364eb0[2]);
}

#pragma thumb reset
