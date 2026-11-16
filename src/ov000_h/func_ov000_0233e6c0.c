#include <nitro/types.h>

typedef struct {
    u8 pad0[0x1C];
    u8 unk1C;
    u8 unk1D;
} UnkStruct_0233e6c0_4560;

typedef void (*UnkFunc_0233e6c0)(void *w);

extern UnkStruct_0233e6c0_4560 gGameState;
extern UnkFunc_0233e6c0 data_ov000_023688c8[][3];
extern void *data_ov000_0236ac18;
extern void *gMainHeap;

void *OS_GetArenaLo(u32 arg);
void MIi_CpuClear32(u32 data, void *dst, u32 size);
void DC_StoreRange(const void *addr, u32 size);
void Heap_Free(void *heap, void *p);

void func_ov000_0233e6c0(void) {
    UnkFunc_0233e6c0 f;
    void *p;
    u32 size;

    f = data_ov000_023688c8[gGameState.unk1C][gGameState.unk1D];
    if (f != NULL) {
        f(data_ov000_0236ac18);
    }
    p = data_ov000_0236ac18;
    if (p == NULL) {
        return;
    }
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
    data_ov000_0236ac18 = NULL;
}
