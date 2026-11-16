#include <nitro/types.h>

extern void *gMainHeap;
extern void *data_ov000_0236aa8c;

void *OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);

void Stage_FreeZoneFile(void) {
    void *p = data_ov000_0236aa8c;
    u32 size;

    if (p == NULL)
        return;
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
    data_ov000_0236aa8c = NULL;
}
