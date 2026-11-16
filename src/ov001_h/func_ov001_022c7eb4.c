#include <nitro/types.h>

typedef struct UnkStruct_022c7eb4 {
    u8 pad0[0x10];
    void *res[0x13];
    u8 bgs[0x13][0x40];
    void *hdr[2];
} UnkStruct_022c7eb4;

extern void *gMainHeap;

void *func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
void func_0201b348(void);
void Sound_ResetHeap(void);

static inline void Free_022c7eb4(void *p) {
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

void func_ov001_022c7eb4(UnkStruct_022c7eb4 *w) {
    s32 i;

    for (i = 0; i < 0x13; i++) {
        Free_022c7eb4(w->res[i]);
        w->res[i] = NULL;
    }
    for (i = 0; i < 2; i++) {
        Free_022c7eb4(w->hdr[i]);
        w->hdr[i] = NULL;
    }
    func_0201b348();
    Sound_ResetHeap();
}
