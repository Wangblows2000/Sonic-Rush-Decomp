#include <ov000_d/mdl.h>

typedef struct {
    u8 pad0[0xB68];
    UnkStruct_MdlRender rA;
    u8 padC00[0xCB0 - 0xB68 - sizeof(UnkStruct_MdlRender)];
    UnkStruct_MdlRender rB;
    u8 padD48[0xDF8 - 0xCB0 - sizeof(UnkStruct_MdlRender)];
    void *unkDF8;
} UnkStruct_02360324;

extern void *gMainHeap;
extern u8 data_ov000_02369a84[];

BOOL func_ov000_0235faf0(void);
void Model_Release(UnkStruct_MdlRender *r);
void *OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
void NNS_FndMountArchive(void *buf, void *a, void *b);
void NNS_FndGetArchiveFileByIndex(void *buf, s32 a);
void NNS_G3dResDefaultRelease(void);
void NNS_FndUnmountArchive(void *buf);

static inline void UnkFree_02360324(void **pp) {
    u32 size;
    void *p = *pp;
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
    *pp = NULL;
}

#pragma thumb on

void TimeSelectMenu_ReleaseArchives(UnkStruct_02360324 *w) {
    vu32 zero;
    u8 buf[0x60];

    if (func_ov000_0235faf0()) {
        Model_Release(&w->rA);
        Model_Release(&w->rB);
        zero = 0;
        MIi_CpuClear32(zero, &w->rA, 0x290);
        return;
    }
    if (w->unkDF8 != NULL) {
        Model_Release(&w->rA);
        Model_Release(&w->rB);
        NNS_FndMountArchive(buf, data_ov000_02369a84, w->unkDF8);
        NNS_FndGetArchiveFileByIndex(buf, 0);
        NNS_G3dResDefaultRelease();
        NNS_FndGetArchiveFileByIndex(buf, 3);
        NNS_G3dResDefaultRelease();
        NNS_FndUnmountArchive(buf);
        UnkFree_02360324(&w->unkDF8);
    }
}

#pragma thumb reset
