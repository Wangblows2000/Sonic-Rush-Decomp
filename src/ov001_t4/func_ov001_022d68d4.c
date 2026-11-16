#include <nitro/types.h>

typedef struct { u8 data[0x60]; } UnkArc_022d68d4;

typedef struct {
    u8 pad0[0x10c];
    u8 flags;
    u8 pad10d[0x138 - 0x10d];
} UnkObj_022d68d4;

typedef struct {
    u8 pad0[0x14];
    UnkObj_022d68d4 obj1;
    UnkObj_022d68d4 obj2;
    void *res284;
    void *res288;
    void *res28c;
    void *res290;
    u8 *buf;
    u8 *file298;
} UnkWork_022d68d4;

extern void *data_0208fe80;
extern char data_ov001_0231b7fc[];
extern char data_ov001_0231b814[];
extern char data_ov001_0231b818[];
extern char data_ov001_0231b82c[];
extern u8 data_ov001_0231615c[];
extern u16 data_ov001_02316154[3];

u8 *func_02033fbc(void *path, s32 b);
u8 *func_02034a40(void *heap, u32 size, s32 align);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_020349b8(void *heap, void *p);
s32 func_ov001_022d6b50(void *p);
u32 func_ov001_022d6b44(void *p);
void func_02008648(void *src, void *dst);
void func_02008720(void *src, void *dst);
void func_020087f8(void *src, void *dst);
void *func_02033a54(void *p);
void func_02033a68(void *src, void *dst, u32 size);
BOOL func_02021310(UnkArc_022d68d4 *arc, const char *name, void *narc);
void *func_02021264(UnkArc_022d68d4 *arc, s32 i);
BOOL func_020212e0(UnkArc_022d68d4 *arc);
void func_020257f8(void *res);
void func_02031c1c(void *o, s32 a);
void func_02031bd0(void *o, void *res, s32 a);
void func_02031a68(void *o, s32 a, void *b, s32 c, s32 d);
void func_02004084(void *p);
void func_02022270(u32 index, u32 value);

#pragma thumb on
static inline u8 *Alloc_022d68d4(u32 size) {
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

static inline void Free_022d68d4(u8 *blk) {
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

void func_ov001_022d68d4(UnkWork_022d68d4 *w) {
    UnkArc_022d68d4 arc;
    u8 *f;
    u8 *p;

    f = func_02033fbc(data_ov001_0231b7fc, -1);
    w->buf = Alloc_022d68d4(((*(u32 *)f >> 8) + 0x43) & ~0x1f);
    p = w->buf;
    switch (func_ov001_022d6b50(f)) {
    case 0x10:
        func_02008648(f, p);
        break;
    case 0x20:
        func_02008720(f, p);
        break;
    case 0x30:
        func_020087f8(f, p);
        break;
    default:
        func_02033a68(func_02033a54(f), p, func_ov001_022d6b44(f));
        break;
    }
    DC_StoreRange(p, func_ov001_022d6b44(f));
    Free_022d68d4(f);
    func_02021310(&arc, data_ov001_0231b814, w->buf);
    func_020257f8(func_02021264(&arc, 0));
    w->res284 = func_02021264(&arc, 0);
    func_020257f8(func_02021264(&arc, 3));
    w->res288 = func_02021264(&arc, 3);
    func_020212e0(&arc);
    w->file298 = func_02033fbc(data_ov001_0231b818, -1);
    func_02021310(&arc, data_ov001_0231b82c, w->file298);
    w->res28c = func_02021264(&arc, 0);
    w->res290 = func_02021264(&arc, 1);
    func_020212e0(&arc);
    func_02031c1c(&w->obj1, 0);
    func_02031bd0(&w->obj1, w->res284, 0);
    func_02031a68(&w->obj1, 0, w->res28c, 1, 0);
    func_02031a68(&w->obj1, 4, w->res290, 1, 0);
    func_02031c1c(&w->obj2, 0);
    func_02031bd0(&w->obj2, w->res288, 0);
    func_02031a68(&w->obj2, 0, w->res28c, 3, 0);
    func_02031a68(&w->obj2, 4, w->res290, 3, 0);
    w->obj1.flags |= 1;
    w->obj2.flags |= 1;
    func_02004084(data_ov001_0231615c);
    func_02022270(0, data_ov001_02316154[0]);
    func_02022270(1, data_ov001_02316154[1]);
    func_02022270(2, data_ov001_02316154[2]);
}
#pragma thumb reset
