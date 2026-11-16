#include <nitro/types.h>

typedef struct UnkStruct_022de240_b {
    void *unk0;
    u8 pad4[0x18 - 4];
    u8 objs[6][0x138];
    void *res[6];
} UnkStruct_022de240_b;

extern void *gMainHeap;

void Model_Release(void *o);
void NNS_G3dResDefaultRelease(void *r);
void *OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);

static inline void Free_022de240(void *p) {
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

void func_ov001_022de240(void *w, UnkStruct_022de240_b *b) {
    u8 i;

    for (i = 0; i < 6; i++) {
        Model_Release(b->objs[i]);
        if (b->res[i] != NULL) {
            NNS_G3dResDefaultRelease(b->res[i]);
            Free_022de240(b->res[i]);
        }
        b->res[i] = NULL;
    }
    if (b->unk0 != NULL) {
        Free_022de240(b->unk0);
        b->unk0 = NULL;
    }
}
