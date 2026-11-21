#include <ov000_d/mdl.h>

extern void *gMainHeap;
extern u8 data_ov000_023690d4[];

void Model_Release(UnkStruct_MdlRender *r);
void *OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
void NNS_FndMountArchive(void *buf, void *a, void *b);
void NNS_FndGetArchiveFileByIndex(void *buf, s32 a);
void NNS_G3dResDefaultRelease(void);
void NNS_FndUnmountArchive(void *buf);

static inline void UnkFree_02357308(void **pp) {
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

void PlayerSelectMenu_ReleaseArchives(UnkStruct_Mdl *w) {
    vu32 zero;
    u8 buf[0x60];

    if (w->unk2F8 != NULL) {
        Model_Release(&w->slot[0].r);
        Model_Release(&w->slot[1].r);
        NNS_FndMountArchive(buf, data_ov000_023690d4, w->unk2F8);
        NNS_FndGetArchiveFileByIndex(buf, 0);
        NNS_G3dResDefaultRelease();
        NNS_FndGetArchiveFileByIndex(buf, 3);
        NNS_G3dResDefaultRelease();
        NNS_FndUnmountArchive(buf);
        UnkFree_02357308(&w->unk2F8);
        UnkFree_02357308(&w->unk2FC);
        if (w->unk300 != NULL) {
            UnkFree_02357308(&w->unk300);
        }
    }
    zero = 0;
    MIi_CpuClear32(zero, w, sizeof(UnkStruct_Mdl));
}

#pragma thumb reset
