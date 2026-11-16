#include <nitro/types.h>

u8 *func_02034a40(void *heap, u32 size, s32 align);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_020349b8(void *heap, void *p);

extern void *data_0208fe78;
extern u16 data_022c4994;

void func_020586d4(u8 *buf);
void func_0200f24c(u16 card);
void func_0200f23c(u16 card);
s32 func_0200f9a0(u32 ofs, void *buf, u32 size, s32 a, s32 b, s32 c);

static inline void Free_02058bbc(u8 *p) {
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
    func_020349b8(data_0208fe78, p);
}

BOOL func_02058bbc(s32 idx) {
    u16 card;
    u8 *buf;
    s32 r;
    vu32 z;

    card = data_022c4994;
    buf = func_02034a40(data_0208fe78, 0x340, 0x20);
    if ((u8 *)func_02006abc(0) != buf) {
        vu32 fill = 0xcccccccc;
        MIi_CpuClear32(fill, buf, 0x340);
        *(u32 *)buf = 0x340;
        *(u32 *)(buf + 0x340 - 4) = 0x02467531;
        DC_StoreRange(buf, 0x340);
        buf += 0x20;
    }
    if (buf == (u8 *)func_02006abc(0))
        return FALSE;
    z = 0;
    MIi_CpuClear32(z, buf, 0x314);
    func_020586d4(buf);
    func_0200f24c(card);
    r = func_0200f9a0(idx * 0x320 + 0xc, buf, 0x314, 0, 0, 0);
    func_0200f23c(card);
    Free_02058bbc(buf);
    return r ? TRUE : FALSE;
}
