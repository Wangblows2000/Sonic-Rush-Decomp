#include <nitro/types.h>

typedef struct {
    u8 pad0[0x1c];
    u8 unk1C;
    u8 unk1D;
} UnkCfg_02341f78;

typedef void (*UnkFunc_02341f78)(void *p);

extern UnkCfg_02341f78 gGameState;
extern u32 gSystemFlags;
extern UnkFunc_02341f78 data_ov000_023689c4[][3];
extern void *data_ov000_0236ac1c;
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

void ZoneBg_Release(void) {
    UnkFunc_02341f78 f;
    void *p;

    gSystemFlags &= ~0xc00;
    f = data_ov000_023689c4[gGameState.unk1C][gGameState.unk1D];
    if (f != NULL)
        f(data_ov000_0236ac1c);
    p = data_ov000_0236ac1c;
    if (p == NULL)
        return;
    UnkDbgFree(p);
    data_ov000_0236ac1c = NULL;
}
