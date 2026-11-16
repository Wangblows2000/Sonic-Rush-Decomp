#include <nitro/types.h>
#include "main_a2/dual.h"

#define REG_DIVCNT (*(vu16 *)0x04000280)
#define REG_DIV_NUMER (*(vs32 *)0x04000290)
#define REG_DIV_DENOM (*(volatile u64 *)0x04000298)
#define REG_DIV_RESULT (*(vs32 *)0x040002a0)
#define REG_DIVREM_RESULT (*(vs32 *)0x040002a8)

typedef struct {
    s16 x;
    s16 y;
    UnkDual d[5];
    s16 unk270;
    s16 unk272;
    u8 pad274[4];
    s32 scale[4];
    u8 pad288[0x10];
    s32 unk298;
} UnkSub_0233624c;

typedef struct {
    u32 flags;
    s16 unk4;
    s16 unk6;
    u8 pad8[0x680];
    UnkSub_0233624c sub;
} UnkWork_0233624c;

typedef struct {
    u8 pad0[0x20];
    u32 unk20;
} UnkStruct_022c4560;

typedef struct {
    u8 pad0[0x3b6];
    s16 unk3b6;
} UnkPlayer_0233624c;

extern UnkStruct_022c4560 data_022c4560;
extern u16 data_ov000_02364d60[];
extern UnkPlayer_0233624c *data_ov000_0236ac60;

void func_02035704(void *p, s32 a, s32 b, u32 c);
void func_020323c8(UnkDual *d, s32 scale, s32 b, s32 c);
void func_02032c8c(UnkDual *d);
void func_020332c4(UnkDual *d, s32 a, s32 b);
void func_02033544(UnkDual *d, u16 idx);

static inline void CP_SetDiv32_32(u32 numer, u32 denom) {
    REG_DIVCNT = 0;
    REG_DIV_NUMER = numer;
    REG_DIV_DENOM = denom;
}

void func_ov000_0233624c(UnkWork_0233624c *work) {
    u16 *p;
    UnkDual *d;
    UnkSub_0233624c *sub;
    s32 pal;
    u16 r;
    UnkDual *d2;
    u32 flags;
    s32 i;
    s32 n;
    s16 x;
    s32 q;
    s16 v;
    s16 y;

    sub = &work->sub;
    x = work->sub.x + work->unk4 + 4;
    y = sub->y + work->unk6 + 0xaa;
    CP_SetDiv32_32(sub->unk272, 100);
    while (REG_DIVCNT & 0x8000)
        ;
    q = REG_DIV_RESULT;
    while (REG_DIVCNT & 0x8000)
        ;
    n = REG_DIVREM_RESULT / 5;

    if (sub->unk270 >= sub->unk272 && q >= 1 && n == 0) {
        q--;
        n = 20;
    }

    if (sub->unk272 < 300) {
        pal = q;
    } else {
        flags = work->flags;
        if (data_022c4560.unk20 & 4)
            pal = 4;
        else
            pal = 5;
        if (flags & 0x20) {
            work->flags = flags | 0x80000000;
        } else if (flags & 0x80000000) {
            if (pal == 4)
                work->flags = flags & ~0x80000000;
        } else {
            pal = 4;
        }
    }

    p = &data_ov000_02364d60[pal];
    func_02035704(p, 1, 0, 0x5000222);
    func_02035704(p, 1, 0, 0x5000622);
    p = &data_ov000_02364d60[pal + 1];
    func_02035704(p, 1, 0, 0x5000224);
    func_02035704(p, 1, 0, 0x5000624);

    d = &sub->d[1];
    d2 = &sub->d[3];
    for (i = 0; i < 4 && n - i > 0; i++) {
        d->ab[0].a = x;
        d->ab[0].b = y - (n - i - 1) * 6;
        if (sub->unk270 < sub->unk272 && sub->unk298 != 0 && sub->unk298 == 1 && i == 0) {
            r = (data_022c4560.unk20 >> 1) * 0x196225 + 0x3c6ef35f;
            d->ab[0].a += 4 - (r & 7);
            d->ab[0].b += 4 - ((r * 0x196225 + 0x3c6ef35f) & 7);
        }
        d->ab[1].a = d->ab[0].a;
        d->ab[1].b = d->ab[0].b - 0xc0;
        func_020323c8(d, sub->scale[i], 0x1000, 0);
    }
    for (; (n - i) & 3; i++) {
        d->ab[0].a = x;
        d->ab[0].b = y - (n - i - 1) * 6;
        d->ab[1].a = d->ab[0].a;
        d->ab[1].b = d->ab[0].b - 0xc0;
        func_02032c8c(d);
    }
    for (; i < n; i += 4) {
        d2->ab[0].a = x;
        d2->ab[0].b = y - (n - i - 4) * 6;
        d2->ab[1].a = d2->ab[0].a;
        d2->ab[1].b = d2->ab[0].b - 0xc0;
        func_02032c8c(d2);
    }

    i = n;
    d = &sub->d[0];
    d2 = &sub->d[2];
    for (; i & 3; i++) {
        d->ab[0].a = x;
        d->ab[0].b = y - i * 6;
        d->ab[1].a = d->ab[0].a;
        d->ab[1].b = d->ab[0].b - 0xc0;
        func_02032c8c(d);
    }
    for (; i < 20; i += 4) {
        d2->ab[0].a = x;
        d2->ab[0].b = y - i * 6;
        d2->ab[1].a = d2->ab[0].a;
        d2->ab[1].b = d2->ab[0].b - 0xc0;
        func_02032c8c(d2);
    }

    d = &sub->d[4];
    if (q == 2 && n == 20) {
        v = data_ov000_0236ac60->unk3b6;
        if (d->sub.unk8 != 0x26)
            func_02033544(d, 0x26);
        if (v > 0) {
            d->sub.unk2a = v >> 2;
            if (d->sub.unk2a < 8)
                d->sub.unk2a = 8;
            d->sub.flags &= ~1;
        }
    } else if (q == 2) {
        if (d->sub.unk8 != 0x25) {
            func_02033544(d, 0x25);
            d->sub.unk2a = 0x10;
        }
        d->sub.flags &= ~1;
    } else if (q == 1) {
        if (d->sub.unk8 != 0x24) {
            func_02033544(d, 0x24);
            d->sub.unk2a = 0x10;
        }
        d->sub.flags &= ~1;
    } else {
        if (d->sub.unk8 != 0x23) {
            func_02033544(d, 0x23);
            d->sub.unk2a = 0x10;
        }
        d->sub.flags &= ~1;
    }
    d->ab[0].a = x;
    d->ab[0].b = y - 0x78;
    d->ab[1].a = sub->d[4].ab[0].a;
    d->ab[1].b = sub->d[4].ab[0].b - 0xc0;
    func_020332c4(d, 0, 0);
    func_02032c8c(d);
}
