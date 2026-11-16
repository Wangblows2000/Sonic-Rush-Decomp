#include <nitro/types.h>

u8 *func_02034a40(void *heap, u32 size, s32 align);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_020349b8(void *heap, void *p);

typedef struct {
    u16 *dst;
    u16 *src;
    void *res;
} UnkPal_0206cf8c;

extern void *data_0208fe80;
extern u32 data_0208a764[];

u32 func_020030d0(void);
void func_020033d8(u32 e);
void *func_02025e8c(void *a);
u32 func_02021ad8(void *res);
u32 func_02038018(void *res, s32 a);
void MIi_CpuCopy16(const void *src, void *dest, u32 size);

static inline u8 *Alloc_0206cf8c(u32 sz) {
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

void func_0206cf8c(void *a, UnkPal_0206cf8c *p) {
    u32 e;
    u32 bank;
    void *addr;

    e = func_020030d0();
    p->res = func_02025e8c(a);
    p->dst = (u16 *)Alloc_0206cf8c((func_02021ad8(p->res) + 0x43) & ~0x1f);
    p->src = (u16 *)Alloc_0206cf8c((func_02021ad8(p->res) + 0x43) & ~0x1f);
    bank = func_02038018(p->res, 0) >> 14;
    addr = (void *)(data_0208a764[bank] + func_02038018(p->res, 0) - (bank << 14));
    MIi_CpuCopy16(addr, p->src, func_02021ad8(p->res));
    func_020033d8(e);
}
