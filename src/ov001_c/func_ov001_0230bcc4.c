#include <nitro/types.h>

typedef struct UnkStruct_0230bcc4_o {
    void *res[7];
    void *res2[4];
    u8 *unk2C;
} UnkStruct_0230bcc4_o;

typedef struct UnkStruct_0230bcc4 {
    u8 pad0[0x1d0];
    void *heap;
    u8 *mem;
} UnkStruct_0230bcc4;

extern UnkStruct_0230bcc4 *gSpecialStageWork;
extern void *gMainHeap;

void *OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
void func_ov001_0230b09c(u8 *blk);
void NNS_SndStopSoundAll(void);
void NNS_SndHeapClear(void *h);
void func_0201db14(void *h);
void Sound_ResetHeap(void);

static inline void Free_0230bcc4(void *p) {
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

void func_ov001_0230bcc4(UnkStruct_0230bcc4_o *o) {
    UnkStruct_0230bcc4 *g = gSpecialStageWork;
    s32 i;

    for (i = 0; i < 7; i++) {
        Free_0230bcc4(o->res[i]);
        o->res[i] = NULL;
    }
    for (i = 0; i < 4; i++) {
        Free_0230bcc4(o->res2[i]);
        o->res2[i] = NULL;
    }
    func_ov001_0230b09c(o->unk2C);
    o->unk2C = NULL;
    NNS_SndStopSoundAll();
    NNS_SndHeapClear(g->heap);
    func_0201db14(g->heap);
    Free_0230bcc4(g->mem);
    Sound_ResetHeap();
}
