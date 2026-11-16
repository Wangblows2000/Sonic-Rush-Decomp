#include <nitro/types.h>

typedef struct UnkMtx33_022dedf4 {
    s32 m[9];
} UnkMtx33_022dedf4;

typedef struct UnkStruct_022dedf4_o {
    u8 pad0[4];
    s32 unk4;
    u8 pad8[0x20 - 8];
    s32 unk20;
    s32 unk24;
    u8 pad28[0x3c - 0x28];
    s32 unk3C;
    u8 pad40[0x50 - 0x40];
    u16 unk50;
    u16 unk52;
    u8 pad54[0x80 - 0x54];
    UnkMtx33_022dedf4 mtx;
    s32 unkA4;
    s32 unkA8;
} UnkStruct_022dedf4_o;

typedef struct UnkStruct_022dedf4 {
    u8 pad0[4];
    u32 unk4;
} UnkStruct_022dedf4;

extern vu16 gPad;
extern s16 data_0206e470[];

#define SIN(a) data_0206e470[(u32)((((a) & 0xffff) >> 4) << 1)]
#define COS(a) data_0206e470[(u32)(((((a) & 0xffff) >> 4) << 1) + 1)]

void MTX_RotX33_(UnkMtx33_022dedf4 *m, s32 s, s32 c);
void MTX_RotY33_(UnkMtx33_022dedf4 *m, s32 s, s32 c);
void MTX_Concat33(UnkMtx33_022dedf4 *a, UnkMtx33_022dedf4 *b, UnkMtx33_022dedf4 *ab);

void func_ov001_022dedf4(UnkStruct_022dedf4 *w, UnkStruct_022dedf4_o *o) {
    if (w->unk4 & 0x40000000) {
        UnkMtx33_022dedf4 rx;
        UnkMtx33_022dedf4 ry;
        u16 k;
        k = gPad;
        if (k & 0x40)
            o->unk24 += 0x1800;
        else if (k & 0x80)
            o->unk24 -= 0x1800;
        o->unkA8 = o->unk24;
        k = gPad;
        if (k & 0x20)
            o->unk20 -= 0x400;
        else if (k & 0x10)
            o->unk20 += 0x400;
        o->unkA4 = o->unk20;
        if (gPad & 0x200)
            o->unk52 -= 0x100;
        if (gPad & 0x100)
            o->unk52 += 0x100;
        MTX_RotX33_(&rx, SIN(o->unk50), COS(o->unk50));
        MTX_RotY33_(&ry, SIN(o->unk52), COS(o->unk52));
        MTX_Concat33(&rx, &ry, &o->mtx);
        o->unk4 = o->unkA8 + 0x5000;
    } else {
        UnkMtx33_022dedf4 rx;
        UnkMtx33_022dedf4 ry;
        if (o->unk24 < 0)
            o->unk3C -= 10;
        else
            o->unk3C += 10;
        o->unk24 += o->unk3C;
        o->unkA8 = o->unk24;
        MTX_RotX33_(&rx, SIN(o->unk50), COS(o->unk50));
        MTX_RotY33_(&ry, SIN(o->unk52), COS(o->unk52));
        MTX_Concat33(&rx, &ry, &o->mtx);
        o->unk4 = o->unkA8 + 0x5000;
    }
}
