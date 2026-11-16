#include <nitro/types.h>

u8 *func_02034a40(void *heap, u32 size, s32 align);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_020349b8(void *heap, void *p);

typedef struct {
    u8 *buf;
} UnkObj_02068478;

extern void *data_0208fe80;

u8 *func_020687b8(UnkObj_02068478 *s, s32 a, s32 b);
void func_020257f8(void *p);
u32 func_020380d8(void *p);
void func_020380a0(void *p, void *buf);

static inline u8 *Alloc_02068478(u32 sz) {
    u32 size = sz;
    u8 *p = func_02034a40(data_0208fe80, size, 0x20);
    if ((u8 *)func_02006abc(0) != p) {
        vu32 fill = 0xcccccccc;
        MIi_CpuClear32(fill, p, size);
        *(u32 *)p = size;
        *(u32 *)(p + size - 4) = 0x02467531;
        DC_StoreRange(p, size);
        p += 0x20;
    }
    return p;
}

static inline void Free_02068478(u8 *p) {
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

void func_02068478(UnkObj_02068478 *s, s32 a) {
    u8 *res;

    res = s->buf;
    if (res == NULL)
        res = func_020687b8(s, a, 0);
    func_020257f8(res);
    s->buf = Alloc_02068478((func_020380d8(res) + 0x43) & ~0x1f);
    func_020380a0(res, s->buf);
    Free_02068478(res);
}
