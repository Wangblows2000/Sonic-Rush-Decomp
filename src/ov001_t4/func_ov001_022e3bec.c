#include <nitro/types.h>

typedef struct { u8 data[0x60]; } UnkArc_022e3bec;

typedef struct {
    s32 mode;
    s32 unk4;
    u8 pad8[0x10];
    s32 unk18;
    s32 unk1c;
    s32 unk20;
    u8 obj[0x130 - 0x24];
    u8 flags130;
    u8 pad131[7];
    s32 unk138;
    u8 pad13c[0x15c - 0x13c];
    u8 *buf;
    void *res160;
    void *res164;
    void *res168;
    void *res16c;
    void *res170;
} UnkWork_022e3bec;

extern void *data_0208fe80;
extern char data_ov001_023162e0[];
extern char data_ov001_0231c57c[];
extern char data_ov001_0231c580[];
extern char data_ov001_0231c584[];
extern void *data_ov001_023162ac;
extern void *data_ov001_023162c4;
extern void *data_ov001_023162b8;
extern void *data_ov001_023162a0;
extern u8 data_ov001_023162d0[];
extern u16 data_ov001_0231628c[3];

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
BOOL func_02021310(UnkArc_022e3bec *arc, const char *name, void *narc);
void *func_02021264(UnkArc_022e3bec *arc, s32 i);
BOOL func_020212e0(UnkArc_022e3bec *arc);
void func_020257f8(void *res);
void func_02031c1c(void *o, s32 a);
void func_02031bd0(void *o, void *res, s32 a);
void func_02031a68(void *o, s32 a, void *b, void *c, s32 d);
void func_02004084(void *p);
void func_02022270(u32 index, u32 value);

#pragma thumb on
static inline u8 *Alloc_022e3bec(u32 size) {
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

static inline void Free_022e3bec(u8 *blk) {
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

void func_ov001_022e3bec(UnkWork_022e3bec *w, s32 mode, void *narc) {
    UnkArc_022e3bec arc;
    u8 *f;
    u8 *p;

    w->mode = mode;
    w->unk4 = 1;
    f = func_02033fbc(data_ov001_023162e0, -1);
    w->buf = Alloc_022e3bec(((*(u32 *)f >> 8) + 0x43) & ~0x1f);
    p = w->buf;
    switch (func_ov001_022e3ecc(f)) {
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
        func_02033a68(func_02033a54(f), p, func_ov001_022e3ec0(f));
        break;
    }
    DC_StoreRange(p, func_ov001_022e3ec0(f));
    Free_022e3bec(f);
    if (mode == 0) {
        func_02021310(&arc, data_ov001_0231c57c, w->buf);
        func_020257f8(func_02021264(&arc, 0));
        w->res160 = func_02021264(&arc, 0);
        w->res164 = func_02021264(&arc, 1);
        w->res168 = func_02021264(&arc, 2);
        func_020212e0(&arc);
    } else {
        func_02021310(&arc, data_ov001_0231c580, w->buf);
        func_020257f8(func_02021264(&arc, 3));
        w->res160 = func_02021264(&arc, 3);
        w->res164 = func_02021264(&arc, 1);
        w->res168 = func_02021264(&arc, 2);
        func_020212e0(&arc);
    }
    func_02021310(&arc, data_ov001_0231c584, narc);
    w->res16c = func_02021264(&arc, 0);
    w->res170 = func_02021264(&arc, 1);
    func_020212e0(&arc);
    func_02031c1c(w->obj, 0);
    w->flags130 |= 4;
    if (mode == 0) {
        func_02031bd0(w->obj, w->res160, 0);
        func_02031a68(w->obj, 0, w->res164, data_ov001_023162ac, 0);
        func_02031a68(w->obj, 4, w->res168, data_ov001_023162c4, 0);
    } else {
        func_02031bd0(w->obj, w->res160, 0);
        func_02031a68(w->obj, 0, w->res164, data_ov001_023162b8, 0);
        func_02031a68(w->obj, 4, w->res168, data_ov001_023162a0, 0);
    }
    w->unk20 = 0;
    w->unk1c = w->unk20;
    w->unk138 = 0x1000;
    w->unk18 = 0x1f;
    func_02004084(data_ov001_023162d0);
    func_02022270(0, data_ov001_0231628c[0]);
    func_02022270(1, data_ov001_0231628c[1]);
    func_02022270(2, data_ov001_0231628c[2]);
}
#pragma thumb reset
