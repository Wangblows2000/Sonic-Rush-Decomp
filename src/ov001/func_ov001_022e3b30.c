#include <nitro/types.h>

extern void *gMainHeap;
u32 OS_GetArenaLo(s32 i);
u8 *Heap_Alloc(void *heap, u32 size, s32 align);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void MIi_CpuCopy16(const void *src, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
typedef struct { u8 data[0x5c]; } UnkStruct_023029d4;
extern u8 data_ov001_0231c578[];
void Model_Release(void *p);
void NNS_FndMountArchive(UnkStruct_023029d4 *a, void *b, void *c);
void *NNS_FndGetArchiveFileByIndex(UnkStruct_023029d4 *a, s32 i);
void NNS_FndUnmountArchive(UnkStruct_023029d4 *a);
void NNS_G3dResDefaultRelease(void *p);
BOOL ZoneMap_IsBlaze(void);

#pragma thumb on
void func_ov001_022e3b30(u8 *p) {
    UnkStruct_023029d4 arc;
    vu32 zero;
    if (*(void **)(p + 0x15c) != NULL) {
        u8 *blk;
        Model_Release(p + 0x24);
        NNS_FndMountArchive(&arc, data_ov001_0231c578, *(void **)(p + 0x15c));
        if (!ZoneMap_IsBlaze()) {
            NNS_G3dResDefaultRelease(NNS_FndGetArchiveFileByIndex(&arc, 0));
        } else {
            NNS_G3dResDefaultRelease(NNS_FndGetArchiveFileByIndex(&arc, 3));
        }
        NNS_FndUnmountArchive(&arc);
        blk = *(u8 **)(p + 0x15c);
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
        *(void **)(p + 0x15c) = NULL;
    }
    zero = 0;
    MIi_CpuClear32(zero, p, 0x174);
}
#pragma thumb reset
