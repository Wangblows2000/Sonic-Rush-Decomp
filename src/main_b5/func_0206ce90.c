#include <nitro/types.h>

u8 *func_02034a40(void *heap, u32 size, s32 align);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_020349b8(void *heap, void *p);

extern void *data_0208fe80;

static inline void Free_0206ce90(u8 *p) {
    if ((u8 *)func_02006abc(0) != p) {
        u32 size;
        p -= 0x20;
        size = *(u32 *)p;
        if (*(u32 *)(p + size - 4) == 0x02467531) {
            vu32 fill = 0xcccccccc;
            MIi_CpuClear32(fill, p, size);
            DC_StoreRange(p, size);
        }
    }
    func_020349b8(data_0208fe80, p);
}

typedef struct {
    u8 *buf0;
    u8 *buf1;
} UnkBufs_0206ce90;

void func_0206ce90(UnkBufs_0206ce90 *s) {
    if (s->buf0 != NULL)
        Free_0206ce90(s->buf0);
    if (s->buf1 != NULL)
        Free_0206ce90(s->buf1);
}
