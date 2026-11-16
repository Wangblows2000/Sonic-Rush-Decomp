#include <nitro/types.h>

typedef struct { u8 pad0[0x34]; void *unk34; u8 pad38[0x10]; } UnkSpr_02061484;
typedef struct {
    u8 pad0[0x10];
    UnkSpr_02061484 spr[2]; /* 0x10 */
    u8 *res[2];             /* 0xa0 */
    u16 pal[0x10];          /* 0xa8 */
} UnkWork_02061484;
extern void *gMainHeap;

void MIi_CpuCopy16(const void *src, void *dst, u32 size);
void ObjVram_Free(s32 a, void *b);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);

#pragma thumb on
static inline void Free_02061484(u8 *blk) {
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

void func_02061484(UnkWork_02061484 *w) {
    UnkSpr_02061484 *spr = w->spr;
    u8 i;

    MIi_CpuCopy16(w->pal, (void *)0x050003e0, sizeof(w->pal));
    for (i = 0; i < 2; i++) {
        if (spr[i].unk34 != NULL)
            ObjVram_Free(0, spr[i].unk34);
        if (w->res[i] != NULL)
            Free_02061484(w->res[i]);
    }
}
#pragma thumb reset
