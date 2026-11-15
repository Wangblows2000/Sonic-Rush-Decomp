#include <nitro/types.h>

void MIi_CpuClear32(u32 data, void *dest, u32 size);

typedef struct { u32 head; u32 tail; } UnkFrm_02021194;

void *func_02021194(UnkFrm_02021194 *hd, u32 size, s32 align) {
    u32 block = ((align - 1) + hd->head) & ~(align - 1);
    u32 end = size + block;
    if (end > hd->tail) return NULL;
    {
        u32 sz = end - hd->head;
        if ((u16)(((u32 *)hd)[-1] & 0xFF) & 1) {
            vu32 zero = 0;
            MIi_CpuClear32(zero, (void *)hd->head, sz);
        }
    }
    hd->head = end;
    return (void *)block;
}
