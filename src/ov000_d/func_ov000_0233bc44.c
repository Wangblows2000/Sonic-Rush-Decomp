#include <nitro/types.h>

extern void *gMainHeap;
extern void *data_ov000_0236aabc[];
extern u16 data_ov000_0236aab0;

void *OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);

static inline void UnkFree_0233bc44(void **pp) {
    void *p = *pp;
    u32 size;
    if (p != NULL) {
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
    *pp = NULL;
}

void func_ov000_0233bc44(void) {
    UnkFree_0233bc44(&data_ov000_0236aabc[0]);
    UnkFree_0233bc44(&data_ov000_0236aabc[1]);
    UnkFree_0233bc44(&data_ov000_0236aabc[2]);
    UnkFree_0233bc44(&data_ov000_0236aabc[3]);
    UnkFree_0233bc44(&data_ov000_0236aabc[4]);
    UnkFree_0233bc44(&data_ov000_0236aabc[5]);
    UnkFree_0233bc44(&data_ov000_0236aabc[6]);
    data_ov000_0236aab0 = 0;
}
