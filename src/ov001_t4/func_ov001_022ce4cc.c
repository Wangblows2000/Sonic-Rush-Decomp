#include <nitro/types.h>

typedef struct {
    u32 unk0;
    u32 unk4;
    u32 unk8;
} UnkFile_022ce4cc;

typedef struct {
    u8 pad0[0x1d08];
    u8 tiles[7][0x20];
    u8 pad1de8[0x1e40 - 0x1de8];
    UnkFile_022ce4cc *res;
} UnkWork_022ce4cc;

typedef struct {
    u8 pad0[0x18];
    u16 count;
} UnkInfo_022ce4cc;

extern void *data_0208fe80;
extern UnkInfo_022ce4cc data_02072cb8;
extern u16 data_02072cbc[];

u8 *func_02034a40(void *heap, u32 size, s32 align);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void MIi_CpuCopy32(const void *src, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_020349b8(void *heap, void *p);
s32 func_ov001_022ce664(void *p);
u32 func_ov001_022ce658(void *p);
void func_02008648(void *src, void *dst);
void func_02008720(void *src, void *dst);
void func_020087f8(void *src, void *dst);
void *func_02033a54(void *p);
void func_02033a68(void *src, void *dst, u32 size);
s32 func_0203422c(u16 c);

#pragma thumb on
static inline u8 *Alloc_022ce4cc(u32 size) {
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

static inline void Free_022ce4cc(u8 *blk) {
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
    func_020349b8(data_0208fe80, blk);
}

void func_ov001_022ce4cc(UnkWork_022ce4cc *w) {
    u8 *f;
    u16 n;
    s32 i;
    u8 *buf;
    u16 *c;
    u8 *t;

    f = (u8 *)w->res + w->res->unk8;
    buf = Alloc_022ce4cc(((*(u32 *)f >> 8) + 0x43) & ~0x1f);
    switch (func_ov001_022ce664(f)) {
    case 0x10:
        func_02008648(f, buf);
        break;
    case 0x20:
        func_02008720(f, buf);
        break;
    case 0x30:
        func_020087f8(f, buf);
        break;
    default:
        func_02033a68(func_02033a54(f), buf, func_ov001_022ce658(f));
        break;
    }
    DC_StoreRange(buf, func_ov001_022ce658(f));
    n = data_02072cb8.count;
    i = 0;
    c = data_02072cbc;
    t = w->tiles[0];
    for (; i < n && i < 6; i++) {
        MIi_CpuCopy32(buf + func_0203422c(*c) * 0x20, t, 0x20);
        c++;
        t += 0x20;
    }
    if (n <= 6) {
        for (; i < 7; i++) {
            MIi_CpuCopy32(buf + func_0203422c(0x20) * 0x20, w->tiles[i], 0x20);
        }
    } else {
        MIi_CpuCopy32(buf + func_0203422c(0x2026) * 0x20, w->tiles[i], 0x20);
    }
    Free_022ce4cc(buf);
}
#pragma thumb reset
