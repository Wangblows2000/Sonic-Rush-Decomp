#include <nitro/types.h>

typedef struct { u8 pad0[0x34]; void *unk34; u8 pad38[0x10]; } UnkSpr_020577c4;
typedef struct {
    u8 pad0[0x14];
    UnkSpr_020577c4 spr[7]; /* 0x14 */
    u8 pad20C[4];
    u8 *res1;               /* 0x210 */
    u8 *res2;               /* 0x214 */
} UnkWork_020577c4;
extern void *gMainHeap;

void ObjVram_Free(s32 a, void *b);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);

#pragma thumb on
static inline void Free_020577c4(u8 *blk) {
    if ((u8 *)func_02006abc(0) != blk) {
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

void func_020577c4(UnkWork_020577c4 *w) {
    u8 i;

    for (i = 0; i < 7; i++) {
        if (w->spr[i].unk34 != NULL)
            ObjVram_Free(1, w->spr[i].unk34);
    }
    if (w->res1 != NULL)
        Free_020577c4(w->res1);
    if (w->res2 != NULL)
        Free_020577c4(w->res2);
}
#pragma thumb reset
