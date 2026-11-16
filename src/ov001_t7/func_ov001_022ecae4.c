#include <nitro/types.h>

typedef struct {
    u8 pad0[0x28];
    s32 n;
    u8 pad2c[0x7fc - 0x2c];
    u8 *res;
    u8 chr[11][7][0x20];
    s32 pal[11][7];
} UnkStruct_022ecae4;

typedef struct {
    u16 name[0x1c / 2];
    u8 len;
    u8 pad1d[3];
} UnkEntry_022ecae4;

typedef struct {
    u8 pad0[0x1b4];
    UnkEntry_022ecae4 e[10];
} UnkTbl_022ecae4;

typedef struct {
    u8 pad0[0x18];
    u16 len;
} UnkHdr_022ecae4;

extern u16 data_02072cbc[];
extern UnkHdr_022ecae4 data_02072cb8;
extern UnkTbl_022ecae4 data_022c46a0;
extern UnkEntry_022ecae4 data_022c4854[];
extern void *data_0208fe80;
extern u32 data_02250438;
extern u32 data_02250434;
extern u32 data_0208f7e0;
extern u32 data_0208f7dc;

void func_ov001_022ec980(s32 *out, u16 *ids, s32 n);
u8 *func_02034a40(void *heap, u32 size, s32 align);
void *func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void MIi_CpuCopy32(const void *src, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
u32 func_ov001_022ecdc0(void *p);
u32 func_ov001_022ecdb4(void *p);
void func_02008648(void *src, void *dst);
void func_02008720(void *src, void *dst);
void func_020087f8(void *src, void *dst);
void *func_02033a54(void *s);
void func_02033a68(void *a, void *b, u32 c);
void func_020349b8(void *heap, void *p);
s32 func_02039c18(s32 a, s32 b);
void func_02035704(void *src, s32 a, s32 b, u32 dst);
void func_02035754(void);
void func_020337ec(void *src, s32 size, s32 a, s32 dst);
void func_0203383c(void);

static inline void LoadPal_022ecae4(void *src, s32 dst) {
    u32 s0;
    u32 s1;

    s0 = data_0208f7e0;
    s1 = data_0208f7dc;
    data_0208f7e0 = 0;
    data_0208f7dc = 0;
    func_020337ec(src, 0x20, 0, dst);
    func_0203383c();
    data_0208f7e0 = s0;
    data_0208f7dc = s1;
}

#pragma thumb on
void func_ov001_022ecae4(UnkStruct_022ecae4 *p) {
    u8 *blk;
    vu32 fill1;
    vu32 fill2;
    u16 col;
    s32 i;
    s32 j;
    u8 *hdr;
    u32 size;
    u32 s0;
    u32 s1;
    s32 tbl[11][7];
    u8 *q;

    func_ov001_022ec980(tbl[0], data_02072cbc, data_02072cb8.len);
    for (i = 1, q = (u8 *)&data_022c46a0; i < 11; i++) {
        if (i <= p->n) {
            func_ov001_022ec980(tbl[i], data_022c4854[i - 1].name, q[0x1d0]);
        }
        q += 0x20;
    }
    hdr = p->res + *(u32 *)(p->res + 8);
    size = ((*(u32 *)hdr >> 8) + 0x43) & ~0x1f;
    blk = func_02034a40(data_0208fe80, size, -0x20);
    if ((u8 *)func_02006abc(0) != blk) {
        fill1 = 0xcccccccc;
        MIi_CpuClear32(fill1, blk, size);
        *(u32 *)blk = size;
        *(u32 *)(blk + size - 4) = 0x02467531;
        DC_StoreRange(blk, size);
        blk += 0x20;
    }
    switch (func_ov001_022ecdc0(hdr)) {
    case 0x10:
        func_02008648(hdr, blk);
        break;
    case 0x20:
        func_02008720(hdr, blk);
        break;
    case 0x30:
        func_020087f8(hdr, blk);
        break;
    default:
        func_02033a68(func_02033a54(hdr), blk, func_ov001_022ecdb4(hdr));
        break;
    }
    DC_StoreRange(blk, func_ov001_022ecdb4(hdr));
    for (i = 0; i < p->n + 1; i++) {
        for (j = 0; j < 7; j++) {
            MIi_CpuCopy32(blk + tbl[i][j] * 0x20, p->chr[i][j], 0x20);
        }
    }
    for (i = 0; i < 11; i++) {
        for (j = 0; j < 7; j++) {
            p->pal[i][j] = func_02039c18(0, 1);
        }
    }
    col = 0;
    s0 = data_02250438;
    s1 = data_02250434;
    data_02250438 = 0;
    data_02250434 = 0;
    func_02035704(&col, 1, 0, 0x500039e);
    func_02035754();
    col = 0x7fff;
    data_02250438 = 0;
    data_02250434 = 0;
    func_02035704(&col, 1, 0, 0x50003be);
    func_02035754();
    data_02250438 = s0;
    data_02250434 = s1;
    for (i = 0; i < p->n + 1; i++) {
        for (j = 0; j < 7; j++) {
            LoadPal_022ecae4(p->chr[i][j], p->pal[i][j]);
        }
    }
    if ((u8 *)func_02006abc(0) != blk) {
        blk -= 0x20;
        size = *(u32 *)blk;
        if (*(u32 *)(blk + size - 4) == 0x02467531) {
            fill2 = 0xcccccccc;
            MIi_CpuClear32(fill2, blk, size);
            DC_StoreRange(blk, size);
        }
    }
    func_020349b8(data_0208fe80, blk);
}
#pragma thumb reset
