#include <nitro/types.h>

u8 *Heap_Alloc(void *heap, u32 size, s32 align);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);

typedef struct {
    u8 pad0[0x18];
    u32 flags;
} UnkStruct_022c4560;

typedef struct {
    u8 pad0[8];
    u8 *buf;
    u8 padc[0x5c];
    s32 unk68;
    s32 unk6c;
    u8 pad70[0x74];
    s32 unke4;
    s32 unke8;
} UnkWork_02048ad4;

typedef struct {
    u8 pad0[0x10];
    UnkWork_02048ad4 *work;
} UnkTask_02048ad4;

extern void *gMainHeap;
extern UnkStruct_022c4560 gGameState;

void ObjVram_Free(s32 a, s32 b);

static inline void Free_02048ad4(u8 *p) {
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

void DemoPlay_Destructor(UnkTask_02048ad4 *t) {
    UnkWork_02048ad4 *w = t->work;

    ObjVram_Free(0, w->unk68);
    ObjVram_Free(1, w->unk6c);
    ObjVram_Free(0, w->unke4);
    ObjVram_Free(1, w->unke8);
    Free_02048ad4(w->buf);
    gGameState.flags &= ~4;
    gGameState.flags &= ~8;
}
