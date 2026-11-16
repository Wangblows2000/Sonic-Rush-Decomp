#include <nitro/types.h>

void func_0206d3fc(u32 a);
void func_02039bb0(s32 a, void *p);
extern void *data_0208fe80;

void *func_02006abc(u32 arg);
void MIi_CpuClear32(u32 data, void *dst, u32 size);
void DC_StoreRange(const void *addr, u32 size);
void func_020349b8(void *heap, void *p);

static inline void UnkDbgFree(void *ptr) {
    u8 *p = ptr;
    u32 sz;
    if (func_02006abc(0) != p) {
        p -= 0x20;
        sz = *(u32 *)p;
        if (*(u32 *)(p + sz - 4) == 0x2467531) {
            vu32 fill = 0xcccccccc;
            MIi_CpuClear32(fill, p, sz);
            DC_StoreRange(p, sz);
        }
    }
    func_020349b8(data_0208fe80, p);
}

void func_ov000_0233cb5c(u32 *w) {
    u8 *s = (u8 *)w + 0x8bc;

    func_0206d3fc(*(u16 *)(s + 0x44) & 0xff);
    if (*(void **)(s + 0x34) != NULL)
        func_02039bb0(0, *(void **)(s + 0x34));
    if (*(void **)(s + 0x7c) != NULL)
        func_02039bb0(1, *(void **)(s + 0x7c));
    if (!(*w & 0x100))
        return;
    UnkDbgFree((void *)w[0x94c / 4]);
}
