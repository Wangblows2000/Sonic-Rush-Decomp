#include <nitro/types.h>

extern u32 data_ov000_0236ac58[2];
extern u32 data_ov000_0236ac50[2];
extern u32 data_ov000_0236ac48[2];
extern u32 data_ov000_0236ac40[2];
extern u32 data_ov000_0236ac38[2];
extern void *data_ov000_0236ac28;
void ObjData_Release(void *p);
extern void *gMainHeap;

void *OS_GetArenaLo(u32 arg);
void MIi_CpuClear32(u32 data, void *dst, u32 size);
void DC_StoreRange(const void *addr, u32 size);
void Heap_Free(void *heap, void *p);

static inline void UnkDbgFree(void *ptr) {
    u8 *p = ptr;
    u32 sz;
    if (OS_GetArenaLo(0) != p) {
        p -= 0x20;
        sz = *(u32 *)p;
        if (*(u32 *)(p + sz - 4) == 0x2467531) {
            vu32 fill = 0xcccccccc;
            MIi_CpuClear32(fill, p, sz);
            DC_StoreRange(p, sz);
        }
    }
    Heap_Free(gMainHeap, p);
}

void func_ov000_0234dee0(void) {
    void *p;

    ObjData_Release(&data_ov000_0236ac58);
    ObjData_Release(&data_ov000_0236ac50);
    ObjData_Release(&data_ov000_0236ac48);
    ObjData_Release(&data_ov000_0236ac40);
    ObjData_Release(&data_ov000_0236ac38);
    p = data_ov000_0236ac28;
    if (p != NULL)
        UnkDbgFree(p);
    data_ov000_0236ac28 = NULL;
}
