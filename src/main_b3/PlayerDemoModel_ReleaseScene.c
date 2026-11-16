#include <nitro/types.h>

typedef struct { u8 pad0[0xa]; u8 unkA; u8 padB[0xd]; s32 unk18; s32 unk1C; s32 unk20; u8 pad24[0x114]; } UnkObj_02056428;
typedef struct {
    u8 pad0[0x18];
    void **names;            /* 0x18 */
    u8 pad1C[4];
    s8 count;                /* 0x20 */
    s8 unk21;
    u8 pad22[2];
    u8 *arc;                 /* 0x24 */
    void *anm[5];            /* 0x28 */
    UnkObj_02056428 objs[15];/* 0x3c */
    void *res[15];           /* 0x1284 */
    u32 flags[15];           /* 0x12c0 */
} UnkWork_02056428;
typedef struct { u8 data[0x5c]; } UnkArc_02056428;

extern void *gMainHeap;
extern char data_0207ba98[];
extern s32 data_02075704[];

u8 *File_Load(void *path, s32 b);
u8 *Heap_Alloc(void *heap, u32 size, s32 align);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
s32 PlayerDemoModel_GetCompressionType(void *p);
u32 PlayerDemoModel_GetUncompressedSize(void *p);
void MI_UncompressLZ16(void *src, void *dst);
void MI_UncompressHuffman(void *src, void *dst);
void MI_UncompressRL16(void *src, void *dst);
void *Compress_GetData(void *p);
void Mem_Copy(void *src, void *dst, u32 size);
BOOL NNS_FndMountArchive(UnkArc_02056428 *arc, const char *name, void *narc);
void *NNS_FndGetArchiveFileByIndex(UnkArc_02056428 *arc, s32 i);
BOOL NNS_FndUnmountArchive(UnkArc_02056428 *arc);
void PolyDemo_BuildModelPath(void *name, char *buf);
u32 func_02057038(void *name);
void Model_Release(UnkObj_02056428 *o);
void NNS_G3dResDefaultRelease(void *res);
void Model_Init(UnkObj_02056428 *o, s32 a);
void NNS_G3dResDefaultSetup(void *res);
void Model_SetResource(UnkObj_02056428 *o, void *res, s32 a);
void *NNS_G3dGetTex(void *res);
void Model_SetAnimation(UnkObj_02056428 *o, s32 a, void *b, s32 c, void *d);

#pragma thumb on
static inline u8 *Alloc_02056428(u32 size) {
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

static inline void Free_02056428(u8 *blk) {
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

void PlayerDemoModel_ReleaseScene(UnkWork_02056428 *w) {
    u8 i;

    for (i = 0; i < w->count; i++) {
        Model_Release(&w->objs[i]);
        if (w->res[i] != NULL && !(w->flags[i] & 1)) {
            NNS_G3dResDefaultRelease(w->res[i]);
            Free_02056428(w->res[i]);
        }
        w->res[i] = NULL;
        w->flags[i] = 0;
    }
    if (w->arc != NULL) {
        Free_02056428(w->arc);
        w->arc = NULL;
    }
}
#pragma thumb reset
