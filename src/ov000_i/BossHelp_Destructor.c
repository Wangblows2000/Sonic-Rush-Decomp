#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    u8 *unk10;
} UnkTask_02338538;

extern void *data_ov000_0236aaac;
void ObjDraw_ReleaseSpritePalette(u32 a);
void ObjVram_Free(s32 a, void *p);
extern void *gMainHeap;

void *func_02006abc(u32 arg);
void MIi_CpuClear32(u32 data, void *dst, u32 size);
void DC_StoreRange(const void *addr, u32 size);
void Heap_Free(void *heap, void *p);

static inline void UnkDbgFree(void *ptr) {
    u8 *p = ptr;
    u32 sz;
    if (func_02006abc(0) != p) {
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

void BossHelp_Destructor(UnkTask_02338538 *t) {
    u8 *w = t->unk10;

    ObjDraw_ReleaseSpritePalette(*(u16 *)(w + 0x44) & 0xff);
    ObjVram_Free(0, *(void **)(w + 0x34));
    UnkDbgFree(*(void **)(w + 0x48));
    data_ov000_0236aaac = NULL;
}
