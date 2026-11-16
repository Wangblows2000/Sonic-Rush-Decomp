#include <nitro/types.h>

extern void *gMainHeap;
extern void *gSpecialStageTrick;

u8 *Heap_Alloc(void *heap, u32 size, s32 align);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void MI_CpuFill8(void *dest, u8 data, u32 size);
void func_ov001_023145d8(void);
void Touch_Enable(void);

static inline u8 *Alloc(u32 sz) {
    u32 size = sz;
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

void SpecialStageTrick_Init(void) {
    u8 *p = Alloc(0x140);

    gSpecialStageTrick = p;
    MI_CpuFill8(p, 0, 0x114);
    func_ov001_023145d8();
    Touch_Enable();
}
