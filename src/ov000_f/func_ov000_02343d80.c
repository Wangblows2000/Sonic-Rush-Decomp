#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 x;
    s32 y;
} UnkStruct_02343d80_Obj;

typedef struct {
    u8 pad0[0x18];
    s32 z;
    u8 pad1C[0x32 - 0x1C];
    s16 unk32;
    u8 pad34[0x40 - 0x34];
    s16 unk40;
    s16 unk42;
    u8 pad44[0x54 - 0x44];
    u32 unk54;
    u32 unk58;
    u8 pad5C[0x324 - 0x5C];
    u8 unk324;
    u8 unk325;
    u8 pad326[0x32C - 0x326];
    u32 unk32C;
    u32 unk330;
    u8 pad334[0x352 - 0x334];
    s16 unk352;
    u8 pad354[0x3B6 - 0x354];
    s16 unk3B6;
    u8 pad3B8[0x3E8 - 0x3B8];
    UnkStruct_02343d80_Obj *unk3E8;
} UnkStruct_02343d80;

typedef struct {
    u8 pad0[0x10];
    UnkStruct_02343d80 *ply;
} UnkStruct_02343d80_Task;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    u8 pad14[0x18 - 0x14];
    u32 unk18;
    u8 unk1C;
    u8 unk1D;
    u8 pad1E[0x20 - 0x1E];
    u32 unk20;
} UnkStruct_02343d80_4560;

typedef struct {
    u8 pad0[0x40];
    s32 unk40;
} UnkStruct_02343d80_4590;

typedef struct {
    u16 v;
    u8 pad2[4];
} UnkStruct_02343d80_Bgm;

typedef struct {
    s16 v;
    u8 pad2[0x2E];
} UnkStruct_02343d80_Tbl;

extern UnkStruct_02343d80_Task *data_022b4574;
extern UnkStruct_02343d80_4560 data_022c4560;
extern UnkStruct_02343d80_4590 data_022c4590;
extern UnkStruct_02343d80_Bgm data_02074960[][3];
extern UnkStruct_02343d80_Bgm data_02074962[][3];
extern UnkStruct_02343d80_Bgm data_02074964[][3];
extern UnkStruct_02343d80_Tbl data_ov000_02368ba4[];
extern UnkStruct_02343d80_Tbl data_ov000_02368ba6[];

void func_ov000_0234307c(UnkStruct_02343d80 *w, s16 v);
void func_ov000_0235bafc(UnkStruct_02343d80 *w);
void func_ov000_02345aa8(UnkStruct_02343d80 *w);
void func_ov000_023458cc(UnkStruct_02343d80 *w);
BOOL func_02043a58(void);
void func_0204d5f8(void (*fn)(void));
void func_0204d684(void (*fn)(void), s32 a);
void func_ov000_0234cfc8(void);
void func_ov000_0234cfbc(void);
void func_02022270(s32 a, u16 b);
s32 func_02068db4(s32 v, s32 a);
void func_ov000_023355e0(u16 a, u16 b, u16 c, u16 d);
void func_ov000_023526cc(UnkStruct_02343d80 *w, UnkStruct_02343d80_Obj *o);
void func_ov000_02345158(UnkStruct_02343d80 *w);
void func_ov000_02343ac0(UnkStruct_02343d80 *w);

void func_ov000_02343d80(void) {
    UnkStruct_02343d80 *w = data_022b4574->ply;
    u32 f;
    s32 bottom;
    s32 right;
    s32 top;
    s32 left;

    w->unk32C &= ~0x10000;
    if (w->unk3B6 == 0 && (w->unk32C & 0x40000)) {
        func_ov000_0234307c(w, -0x640);
        w->unk32C &= ~0x40000;
    }
    if (w->unk32C & 0x20000000) {
        func_ov000_0235bafc(w);
    }
    func_ov000_02345aa8(w);
    func_ov000_023458cc(w);
    if ((w->unk58 & 4) && !(w->unk58 & 0x4000)) {
        w->unk352 = 0;
    }
    if (w->unk3E8 == NULL || (w->unk330 & 8)) {
        w->unk58 &= ~0x4000;
    }
    if (data_022c4560.unk10 == 1 && w->unk325 == 0 && data_022c4560.unk20 >= 0x8c9f && func_02043a58() &&
        data_022c4590.unk40 >= 3) {
        func_0204d5f8(func_ov000_0234cfc8);
        return;
    }
    f = w->unk32C;
    if (f & 0x800000) {
        func_0204d684(func_ov000_0234cfc8, 3);
        return;
    }
    if (f & 0x40000000) {
        data_022c4560.unk18 |= 0x40;
        func_0204d684(func_ov000_0234cfbc, 3);
        return;
    }
    if (f & 0x4000000) {
        func_02022270(0, data_02074960[data_022c4560.unk1C][data_022c4560.unk1D].v);
        func_02022270(1, data_02074962[data_022c4560.unk1C][data_022c4560.unk1D].v);
        func_02022270(2, data_02074964[data_022c4560.unk1C][data_022c4560.unk1D].v);
    }
    f = w->unk330;
    if (f & 0x1000) {
        w->unk330 = f & ~0x1000;
        w->unk330 |= 0x80000;
        w->unk58 |= 0x80;
        w->unk58 &= ~0x100;
    } else if (f & 0x80000) {
        w->unk330 = f & ~0x100000;
        if (!(w->unk58 & 0x10)) {
            w->z = func_02068db4(w->z, 0x200);
            w->unk32 = func_02068db4(w->unk32, 0x80);
        }
    }
    if (w->unk32C & 0x2000000) {
        w->unk40 = data_ov000_02368ba4[w->unk324].v;
        w->unk42 = data_ov000_02368ba6[w->unk324].v;
        w->unk32C &= ~0x2000000;
        if (w->unk32C & 0x4000000) {
            w->unk40 >>= 1;
        }
    }
    if (w->unk330 & 0x2000) {
        w->unk330 &= ~0x2000;
        w->unk54 |= 1;
    }
    if (w->unk330 & 0x10000) {
        w->unk330 &= ~0x10000;
        w->unk54 |= 1;
    }
    if (w->unk330 & 0x40000) {
        if ((data_022c4560.unk18 & 0x20) && w->unk325 == 0) {
            left = (w->unk3E8->x >> 8) - 0xbc;
            top = (w->unk3E8->y >> 8) - 0x1d0;
            right = (w->unk3E8->x >> 8) + 0xbc;
            bottom = (w->unk3E8->y >> 8) + 0x110;
            if (left < 0) {
                left = 0;
            } else if (left > 0xffff) {
                left = 0xffff;
            }
            if (top < 0) {
                top = 0;
            } else if (top > 0xffff) {
                top = 0xffff;
            }
            if (right < 0) {
                right = 0;
            } else if (right > 0xffff) {
                right = 0xffff;
            }
            if (bottom < 0) {
                bottom = 0;
            } else if (bottom > 0xffff) {
                bottom = 0xffff;
            }
            func_ov000_023355e0(left, top, right, bottom);
        }
        func_ov000_023526cc(w, w->unk3E8);
    }
    func_ov000_02345158(w);
    w->unk32C &= ~0x20000;
    func_ov000_02343ac0(w);
}
