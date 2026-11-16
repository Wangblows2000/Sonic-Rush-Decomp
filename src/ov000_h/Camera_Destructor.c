#include <nitro/types.h>

typedef struct {
    u8 pad0[0x24];
    void *unk24;
    void *unk28;
    u8 pad2C[0x64 - 0x2C];
} UnkPlayer_02339eec;

extern UnkPlayer_02339eec gCamera[];
extern u32 data_ov000_0236aab4;
extern void *gMainHeap;

void *OS_GetArenaLo(u32 arg);
void MIi_CpuClear32(u32 data, void *dst, u32 size);
void DC_StoreRange(const void *addr, u32 size);
void Heap_Free(void *heap, void *p);
void ZoneFx_Release(void);
void ZoneBg_Release(void);

static inline void UnkFree_02339eec(void *p) {
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

void Camera_Destructor(void) {
    u32 i;

    for (i = 0; i < 2; i++) {
        UnkFree_02339eec(gCamera[i].unk24);
        UnkFree_02339eec(gCamera[i].unk28);
    }
    ZoneFx_Release();
    ZoneBg_Release();
    data_ov000_0236aab4 = 0;
}
