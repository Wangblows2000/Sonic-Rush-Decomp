#include <nitro/types.h>

typedef struct UnkStruct_02305474_h {
    u8 pad0[0x18];
    u16 w;
    u16 h;
} UnkStruct_02305474_h;

typedef struct UnkStruct_02305474 {
    u8 pad0[4];
    u8 bg[0x40];
} UnkStruct_02305474;

extern void *data_0208fe80;
extern UnkStruct_02305474 *data_ov001_0231f54c;
extern UnkStruct_02305474_h *data_ov001_0231f550;

u8 *func_02034a40(void *heap, u32 size, s32 align);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_0203473c(void *bg, void *h, s32 a, s32 b, s32 c, u16 w, u16 hh);
void func_0203438c(void *bg);

static inline u8 *Alloc(u32 sz) {
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

void func_ov001_02305474(void) {
    u8 *p = Alloc(0xb80);
    vu16 zero;

    zero = 0;
    data_ov001_0231f54c = (UnkStruct_02305474 *)p;
    MIi_CpuClear16(zero, p, 0xb54);
    func_0203473c(data_ov001_0231f54c->bg, data_ov001_0231f550, 0x31, 0, 1, data_ov001_0231f550->w, data_ov001_0231f550->h);
    func_0203438c(data_ov001_0231f54c->bg);
}
