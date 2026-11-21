#include <nitro/types.h>

typedef struct {
    u8 pad0[0x1C];
    u8 unk1C;
    u8 unk1D;
} UnkStruct_0233e79c_4560;

typedef void (*UnkFunc_0233e79c)(void *w);

extern UnkStruct_0233e79c_4560 gGameState;
extern UnkFunc_0233e79c gZoneFxInitTable[][3];
extern void *gZoneFxWork;
extern void *gMainHeap;

void *Heap_Alloc(void *heap, u32 size, s32 align);
void *OS_GetArenaLo(u32 arg);
void MIi_CpuClear32(u32 data, void *dst, u32 size);
void MIi_CpuClear16(u16 data, void *dst, u32 size);
void DC_StoreRange(const void *addr, u32 size);

static inline void *UnkAlloc_0233e79c(u32 sz, s32 align) {
    u8 *p = Heap_Alloc(gMainHeap, sz, align);
    if (OS_GetArenaLo(0) != p) {
        vu32 fill = 0xcccccccc;
        MIi_CpuClear32(fill, p, sz);
        *(u32 *)p = sz;
        *(u32 *)(p + sz - 4) = 0x2467531;
        DC_StoreRange(p, sz);
        p += 0x20;
    }
    return p;
}

void ZoneFx_Init(void) {
    void *p;
    vu16 zero;

    if (gZoneFxInitTable[gGameState.unk1C][gGameState.unk1D] == NULL) {
        return;
    }
    p = UnkAlloc_0233e79c(0x980, 0x20);
    zero = 0;
    gZoneFxWork = p;
    MIi_CpuClear16(zero, p, 0x950);
    gZoneFxInitTable[gGameState.unk1C][gGameState.unk1D](gZoneFxWork);
}
