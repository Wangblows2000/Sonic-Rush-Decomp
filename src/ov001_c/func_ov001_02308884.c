#include <nitro/types.h>

typedef struct UnkStruct_02308884_o {
    u8 pad0[0x128];
    s32 unk128;
    u8 pad12C[0x14e - 0x12c];
    u16 unk14E;
    s16 unk150;
} UnkStruct_02308884_o;

typedef struct UnkStruct_02308884_d {
    u8 pad0[0x20];
    u16 unk20;
    u16 unk22;
} UnkStruct_02308884_d;

typedef struct UnkStruct_02308884 {
    u8 pad0[0x810];
    u8 objs[0x5][0x48];
    u8 pad978[0xa50 - 0x978];
    u8 objA50[0xae0 - 0xa50];
    u32 unkAE0;
} UnkStruct_02308884;

extern UnkStruct_02308884 *data_ov001_0231f5bc;
extern u16 data_ov001_02317090[];

UnkStruct_02308884_o *func_ov001_0230c0a4(void);
UnkStruct_02308884_d *func_ov001_02314664(s32 a);
float func_0202f1bc(u32 v);
void AnimatorSprite_Draw(void *p);
void func_ov001_02315714(void);
void func_02025268(u32 op, const u32 *args, u32 n);
void func_ov001_0231402c(void);
void func_ov001_02314058(s32 a);
s32 func_ov001_02314614(s32 a, u16 b);
void func_ov001_023156a8(void);

void func_ov001_02308884(void) {
    UnkStruct_02308884 *g = data_ov001_0231f5bc;
    UnkStruct_02308884_o *o;
    UnkStruct_02308884_d *d;
    s32 i;
    float f;
    s32 k;
    u16 w;
    u32 a0;
    u32 a1;
    u32 a2[2];
    u32 a3[2];
    u32 a4[2];
    u32 a5[2];
    u32 a6;
    u32 a7;
    u32 a8;
    u32 a9[2];
    u32 a10[2];
    u32 a11[2];
    u32 a12[2];
    u32 a13;

    if (func_ov001_0230c0a4()->unk128 == 0)
        return;
    o = func_ov001_0230c0a4();
    d = func_ov001_02314664(o->unk150);
    f = 1.0f - func_0202f1bc(o->unk14E) / func_0202f1bc(d->unk20);
    for (i = 0; i < d->unk22; i++)
        AnimatorSprite_Draw(g->objs[i]);
    func_ov001_02315714();
    func_02025268(0x11, 0, 0);
    func_ov001_0231402c();
    func_ov001_02314058(0x1f);
    a0 = 0;
    func_02025268(0x20, &a0, 1);
    a1 = 1;
    func_02025268(0x40, &a1, 1);
    a2[0] = 0xb000200;
    a2[1] = 0;
    func_02025268(0x23, a2, 2);
    a3[0] = 0xb800200;
    a3[1] = 0;
    func_02025268(0x23, a3, 2);
    a4[0] = 0xb800e00;
    a4[1] = 0;
    func_02025268(0x23, a4, 2);
    a5[0] = 0xb000e00;
    a5[1] = 0;
    func_02025268(0x23, a5, 2);
    k = func_ov001_02314614(o->unk150, o->unk14E);
    a6 = data_ov001_02317090[k];
    func_02025268(0x20, &a6, 1);
    if (k == 0 && (g->unkAE0 & 4)) {
        a7 = data_ov001_02317090[2];
        func_02025268(0x20, &a7, 1);
    }
    a8 = 1;
    func_02025268(0x40, &a8, 1);
    a9[0] = 0xb000200;
    a9[1] = 0x1000;
    func_02025268(0x23, a9, 2);
    a10[0] = 0xb800200;
    a10[1] = 0x1000;
    func_02025268(0x23, a10, 2);
    w = (s32)(512.0f + 786432.0f * f / 256.0f);
    a11[0] = w | 0xb800000;
    a11[1] = 0x1000;
    func_02025268(0x23, a11, 2);
    a12[0] = w | 0xb000000;
    a12[1] = 0x1000;
    func_02025268(0x23, a12, 2);
    func_02025268(0x41, 0, 0);
    a13 = 1;
    func_02025268(0x12, &a13, 1);
    func_ov001_023156a8();
    AnimatorSprite_Draw(g->objA50);
}
