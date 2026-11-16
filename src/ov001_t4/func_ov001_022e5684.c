#include <nitro/types.h>

typedef struct {
    u8 pad0[0xc];
    u32 unkc;
    u8 pad10[8];
    u16 w;
    u16 h;
} UnkRes_022e5684;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkc;
    u8 bg[0x40];
    u8 *buf;
    u8 pad54[4];
    s32 unk58;
    u8 pad5c[8];
    s32 unk64;
    s32 unk68;
    s32 unk6c;
    u8 pad70[0x670 - 0x70];
} UnkWork_022e5684;

extern void *data_0208fe80;
u8 *func_02033fbc(void *path, s32 b);
u8 *func_02034a40(void *heap, u32 size, s32 align);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_020349b8(void *heap, void *p);
s32 func_ov001_022e3ecc(void *p);
u32 func_ov001_022e3ec0(void *p);
void func_02008648(void *src, void *dst);
void func_02008720(void *src, void *dst);
void func_020087f8(void *src, void *dst);
void *func_02033a54(void *p);
void func_02033a68(void *src, void *dst, u32 size);
void func_0203473c(void *bg, void *res, s32 a, s32 b, s32 c, u16 w, u16 h);
void func_0203438c(void *bg);
void func_ov001_022e833c(UnkWork_022e5684 *w);

#pragma thumb on
static inline u8 *Alloc_022e5684(u32 size) {
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

void func_ov001_022e5684(UnkWork_022e5684 *w, UnkRes_022e5684 *res) {
    u8 *p;
    vu32 z;

    z = 0;
    MIi_CpuClear32(z, w, 0x670);
    w->unk0 = 8;
    w->unk4 = 0;
    w->unk64 = 1;
    w->unk68 = 1;
    w->unk6c = 0xffff;
    w->unk58 = 1;
    func_0203473c(w->bg, res, 0x1c, 0, 3, 0x40, res->h);
    func_0203438c(w->bg);
    w->unk8 = res->w << 3;
    w->unkc = res->h << 3;
    w->buf = Alloc_022e5684(((*(u32 *)((u8 *)res + res->unkc) >> 8) + 0x43) & ~0x1f);
    p = w->buf;
    res = (UnkRes_022e5684 *)((u8 *)res + res->unkc);
    switch (func_ov001_022e3ecc(res)) {
    case 0x10:
        func_02008648(res, p);
        break;
    case 0x20:
        func_02008720(res, p);
        break;
    case 0x30:
        func_020087f8(res, p);
        break;
    default:
        func_02033a68(func_02033a54(res), p, func_ov001_022e3ec0(res));
        break;
    }
    DC_StoreRange(p, func_ov001_022e3ec0(res));
    func_ov001_022e833c(w);
}
#pragma thumb reset
