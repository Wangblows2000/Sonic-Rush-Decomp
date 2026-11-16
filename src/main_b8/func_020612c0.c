#include <nitro/types.h>

typedef struct {
    u8 pad0[0xb8];
    void *unkb8;
    u8 padbc[0x10];
    u8 *unkcc;
} UnkWork_020612c0;

extern void *gMainHeap;

u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
void ObjVram_Free(s32 a, void *b);

#pragma thumb on
void func_020612c0(UnkWork_020612c0 *w) {
    u8 *p;

    if (w->unkb8 != NULL)
        ObjVram_Free(0, w->unkb8);
    p = w->unkcc;
    if (p != NULL) {
        if ((u8 *)OS_GetArenaLo(0) != p) {
            u32 size;
            p -= 0x20;
            size = *(u32 *)p;
            if (*(u32 *)(p + size - 4) == 0x02467531) {
                vu32 fill = 0xcccccccc;
                MIi_CpuClear32(fill, p, size);
                DC_StoreRange(p, size);
            }
        }
        Heap_Free(gMainHeap, p);
    }
}
#pragma thumb reset
