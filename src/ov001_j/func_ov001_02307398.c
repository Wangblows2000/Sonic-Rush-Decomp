#include <nitro/types.h>

typedef struct UnkArc_02307398 {
    u8 pad[0x5c];
} UnkArc_02307398;

typedef struct UnkObj_02307398 {
    u8 pad0[0xdc];
    u32 flags;
    u8 padE0[0xf4 - 0xe0];
} UnkObj_02307398;

typedef struct UnkObj2_02307398 {
    u8 pad0[0x138];
} UnkObj2_02307398;

typedef struct UnkTask_02307398 {
    u8 pad0[0x1dc];
    void (*unk1DC)(void);
    void (*unk1E0)(void);
} UnkTask_02307398;

typedef struct UnkWork_02307398 {
    UnkObj_02307398 obj[32];
    u8 pad1E80[0x1f00 - 0x1e80];
    UnkObj2_02307398 obj2[32];
    u8 pad4600[0x4680 - 0x4600];
    u8 *data;
    u32 res0;
    u32 res2;
    u32 res1;
    u32 res3;
    u32 res4;
    u32 res5;
    u32 res6;
    UnkTask_02307398 *task;
} UnkWork_02307398;

typedef struct UnkHdr_02307398 {
    u8 pad0[0x16];
    u16 unk16;
    u32 unk18;
} UnkHdr_02307398;

typedef struct UnkRes_02307398 {
    u8 pad0[0x18];
    u32 hdrOfs;
} UnkRes_02307398;

typedef struct UnkResSet_02307398 {
    u8 pad0[0x28];
    UnkRes_02307398 *res;
} UnkResSet_02307398;

extern UnkResSet_02307398 *data_ov001_0231f5c4;
extern void *data_0208fe80;
extern UnkWork_02307398 *data_ov001_0231f5b8;
extern u8 data_ov001_0231df7c[];
extern char data_ov001_0231df94[];

u8 *func_02034a40(void *heap, u32 size, s32 align);
void *func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void MI_CpuFill8(void *dest, u8 data, u32 size);
u32 func_0203969c(u32 a, s32 b);
u32 func_02039428(u16 a, s32 b);
void func_02031878(UnkObj_02307398 *o, UnkRes_02307398 *res, s32 a, s32 b, u32 c, u32 d, s32 e, s32 f);
void func_02031c1c(UnkObj2_02307398 *o, s32 a);
u8 *func_02033fbc(void *p, s32 v);
u32 func_ov001_0230776c(u8 *p);
u32 func_ov001_02307760(u8 *p);
void func_02008648(void *src, void *dst);
void func_02008720(void *src, void *dst);
void func_020087f8(void *src, void *dst);
u32 *func_02033a54(void *s);
void func_02033a68(void *a, void *b, u32 c);
void func_020349b8(void *heap, void *p);
BOOL func_02021310(UnkArc_02307398 *arc, const char *name, void *data);
u32 func_02021264(UnkArc_02307398 *arc, u32 idx);
void func_020257f8(u32 p);
BOOL func_020212e0(UnkArc_02307398 *arc);
UnkTask_02307398 *func_ov001_02310d5c(s32 a);
void func_ov001_02306d04(void);
void func_ov001_02306a20(void);

static inline UnkHdr_02307398 *GetHdr(UnkRes_02307398 *r) { return (UnkHdr_02307398 *)((u8 *)r + r->hdrOfs); }

void func_ov001_02307398(void) {
    vu32 fill1;
    vu32 fill2;
    vu32 fill3;
    UnkArc_02307398 arc;
    u8 *blk;
    u8 *res;
    u8 *d;
    u32 size;
    UnkWork_02307398 *w;
    UnkRes_02307398 *r;
    u32 a;
    s32 i;
    UnkHdr_02307398 *hdr;

    r = data_ov001_0231f5c4->res;
    blk = func_02034a40(data_0208fe80, 0x46e0, 0x20);
    if ((u8 *)func_02006abc(0) != blk) {
        fill1 = 0xcccccccc;
        MIi_CpuClear32(fill1, blk, 0x46e0);
        *(u32 *)blk = 0x46e0;
        *(u32 *)(blk + 0x46dc) = 0x02467531;
        DC_StoreRange(blk, 0x46e0);
        blk += 0x20;
    }
    data_ov001_0231f5b8 = (UnkWork_02307398 *)blk;
    MI_CpuFill8(blk, 0, sizeof(UnkWork_02307398));
    w = data_ov001_0231f5b8;
    for (i = 0; i < 32; i++) {
        func_02031878(&w->obj[i], r, 0, 0, func_0203969c(GetHdr(r)->unk18, 0), func_02039428(GetHdr(r)->unk16, 0), 0, 0);
        w->obj[i].flags |= 0x8000;
    }
    for (i = 0; i < 32; i++)
        func_02031c1c(&w->obj2[i], 0);

    res = func_02033fbc(data_ov001_0231df7c, -1);
    size = ((*(u32 *)res >> 8) + 0x43) & ~0x1f;
    blk = func_02034a40(data_0208fe80, size, 0x20);
    if ((u8 *)func_02006abc(0) != blk) {
        fill2 = 0xcccccccc;
        MIi_CpuClear32(fill2, blk, size);
        *(u32 *)blk = size;
        *(u32 *)(blk + size - 4) = 0x02467531;
        DC_StoreRange(blk, size);
        blk += 0x20;
    }
    w->data = blk;
    d = w->data;
    switch (func_ov001_0230776c(res)) {
    case 0x10:
        func_02008648(res, d);
        break;
    case 0x20:
        func_02008720(res, d);
        break;
    case 0x30:
        func_020087f8(res, d);
        break;
    default:
        func_02033a68(func_02033a54(res), d, func_ov001_02307760(res));
        break;
    }
    DC_StoreRange(d, func_ov001_02307760(res));
    if ((u8 *)func_02006abc(0) != res) {
        res -= 0x20;
        size = *(u32 *)res;
        if (*(u32 *)(res + size - 4) == 0x02467531) {
            fill3 = 0xcccccccc;
            MIi_CpuClear32(fill3, res, size);
            DC_StoreRange(res, size);
        }
    }
    func_020349b8(data_0208fe80, res);
    func_02021310(&arc, data_ov001_0231df94, w->data);
    w->res0 = func_02021264(&arc, 0);
    w->res1 = func_02021264(&arc, 1);
    w->res2 = func_02021264(&arc, 2);
    w->res3 = func_02021264(&arc, 3);
    w->res4 = func_02021264(&arc, 4);
    w->res5 = func_02021264(&arc, 5);
    w->res6 = func_02021264(&arc, 6);
    func_020257f8(w->res0);
    func_020257f8(w->res3);
    func_020212e0(&arc);
    w->task = func_ov001_02310d5c(0x20);
    w->task->unk1DC = func_ov001_02306d04;
    w->task->unk1E0 = func_ov001_02306a20;
}
