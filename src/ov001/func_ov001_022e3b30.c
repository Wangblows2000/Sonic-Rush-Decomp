#include <nitro/types.h>

extern void *gMainHeap;
u32 func_02006abc(s32 i);
u8 *Heap_Alloc(void *heap, u32 size, s32 align);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void MIi_CpuCopy16(const void *src, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
typedef struct { u8 data[0x5c]; } UnkStruct_023029d4;
extern u8 data_ov001_0231c578[];
void Model_Release(void *p);
void func_02021310(UnkStruct_023029d4 *a, void *b, void *c);
void *func_02021264(UnkStruct_023029d4 *a, s32 i);
void func_020212e0(UnkStruct_023029d4 *a);
void func_0202566c(void *p);
BOOL ZoneMap_IsBlaze(void);

#pragma thumb on
void func_ov001_022e3b30(u8 *p) {
    UnkStruct_023029d4 arc;
    vu32 zero;
    if (*(void **)(p + 0x15c) != NULL) {
        u8 *blk;
        Model_Release(p + 0x24);
        func_02021310(&arc, data_ov001_0231c578, *(void **)(p + 0x15c));
        if (!ZoneMap_IsBlaze()) {
            func_0202566c(func_02021264(&arc, 0));
        } else {
            func_0202566c(func_02021264(&arc, 3));
        }
        func_020212e0(&arc);
        blk = *(u8 **)(p + 0x15c);
        if ((u8 *)func_02006abc(0) != blk) {
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
