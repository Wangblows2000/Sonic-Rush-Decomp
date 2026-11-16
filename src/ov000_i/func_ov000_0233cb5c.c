#include <nitro/types.h>

void ObjDraw_ReleaseSpritePalette(u32 a);
void ObjVram_Free(s32 a, void *p);
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

void func_ov000_0233cb5c(u32 *w) {
    u8 *s = (u8 *)w + 0x8bc;

    ObjDraw_ReleaseSpritePalette(*(u16 *)(s + 0x44) & 0xff);
    if (*(void **)(s + 0x34) != NULL)
        ObjVram_Free(0, *(void **)(s + 0x34));
    if (*(void **)(s + 0x7c) != NULL)
        ObjVram_Free(1, *(void **)(s + 0x7c));
    if (!(*w & 0x100))
        return;
    UnkDbgFree((void *)w[0x94c / 4]);
}
