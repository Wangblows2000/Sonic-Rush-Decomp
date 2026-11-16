#include <ov000_d/mdl2.h>

extern void *gMainHeap;
extern u8 data_ov000_023692f4[];

void Model_Release(UnkStruct_MdlRender *r);
BOOL WirelessSession_IsDownloadPlay(void);
void *OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
void NNS_FndMountArchive(void *buf, void *a, void *b);
void NNS_FndGetArchiveFileByIndex(void *buf, s32 a);
void NNS_G3dResDefaultRelease(void);
void NNS_FndUnmountArchive(void *buf);

static inline void UnkFree_0235d958(void **pp) {
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

void ResultsScreen_ReleaseArchives(UnkStruct_Mdl2 *w) {
    u8 buf[0x5c];

    if (WirelessSession_IsDownloadPlay()) {
        Model_Release(&w->rA);
        Model_Release(&w->rB);
        UnkFree_0235d958(&w->unk90C);
        return;
    }
    if (w->unk908 != NULL) {
        Model_Release(&w->rA);
        Model_Release(&w->rB);
        NNS_FndMountArchive(buf, data_ov000_023692f4, w->unk908);
        NNS_FndGetArchiveFileByIndex(buf, 0);
        NNS_G3dResDefaultRelease();
        NNS_FndGetArchiveFileByIndex(buf, 3);
        NNS_G3dResDefaultRelease();
        NNS_FndUnmountArchive(buf);
        UnkFree_0235d958(&w->unk908);
        UnkFree_0235d958(&w->unk90C);
    }
}

#pragma thumb reset
