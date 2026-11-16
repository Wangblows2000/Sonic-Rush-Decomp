#include <nitro/types.h>

typedef struct { u8 pad0[0x550]; u8 *buf[0xd]; } UnkWork_0204f454;
extern s32 data_022c4600[];
extern void *gMainHeap;

u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);

#pragma thumb on
static inline void Free_0204f454(u8 *blk) {
    if ((u8 *)OS_GetArenaLo(0) != blk) {
        u32 size;
        blk -= 0x20;
        size = *(u32 *)blk;
        if (*(u32 *)(blk + size - 4) == 0x02467531) {
            vu32 fill = 0xcccccccc;
            MIi_CpuClear32(fill, blk, size);
            DC_StoreRange(blk, size);
        }
    }
    Heap_Free(gMainHeap, blk);
}

void MsgBox_Unk_0204f454(UnkWork_0204f454 *w, s32 i) {
    u8 **p = &w->buf[i];

    if (w->buf[i] != NULL) {
        if (data_022c4600[i] != 0)
            *p = NULL;
        if (*p != NULL) {
            Free_0204f454(*p);
            *p = NULL;
        }
    }
}
#pragma thumb reset
