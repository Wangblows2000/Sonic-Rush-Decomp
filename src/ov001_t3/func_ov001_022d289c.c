#include <nitro/types.h>

typedef struct {
    u8 pad0[0xc];
    u8 unkc[0x18 - 0xc];
    void *res[15];
    u8 pad54[0x414 - 0x54];
    void *res2[3];
} UnkStruct_022d289c;

extern void *gMainHeap;

void *func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
void func_0201b348(void);
void func_02020198(void *p);
void Sound_ResetHeap(void);

#pragma thumb on
static inline void Free_022d289c(void *p) {
    u32 size;
    if (func_02006abc(0) != p) {
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

void func_ov001_022d289c(UnkStruct_022d289c *p) {
    s32 i;

    for (i = 0; i < 15; i++) {
        if (p->res[i] != NULL) {
            Free_022d289c(p->res[i]);
            p->res[i] = NULL;
        }
    }
    for (i = 0; i < 3; i++) {
        if (p->res2[i] != NULL) {
            Free_022d289c(p->res2[i]);
            p->res2[i] = NULL;
        }
    }
    func_0201b348();
    func_02020198(p->unkc);
    Sound_ResetHeap();
}
#pragma thumb reset
