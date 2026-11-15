#include <nitro/types.h>

void MIi_CpuClear32(u32 data, void *dest, u32 size);

typedef struct { u32 head; u32 tail; } UnkFrm_02021118;

void *func_02021118(UnkFrm_02021118 *hd, u32 size, s32 align) {
    u32 block = (hd->tail - size) & ~(align - 1);
    if (block < hd->head) return NULL;
    {
        u32 sz = hd->tail - block;
        if ((u16)(((u32 *)hd)[-1] & 0xFF) & 1) {
            vu32 zero = 0;
            MIi_CpuClear32(zero, (void *)block, sz);
        }
    }
    hd->tail = block;
    return (void *)block;
}
