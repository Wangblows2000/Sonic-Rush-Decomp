#include <nitro/types.h>

typedef struct UnkStruct_022c798c UnkStruct_022c798c;
struct UnkStruct_022c798c {
    u8 pad[0x68];
    void (*func)(UnkStruct_022c798c *s);
    u8 pad1[0x20c - 0x6c];
    u16 unk20C;
    u8 pad2[0x214 - 0x20e];
    s32 unk214;
};

typedef struct UnkStruct_022c798c_2 {
    u8 pad[0x21c];
    u16 unk21C;
    u16 unk21E;
    u8 pad1[0x22a - 0x220];
    u16 unk22A;
} UnkStruct_022c798c_2;

typedef struct UnkStruct_022c798c_3 {
    u8 pad[0x10];
    s32 unk10;
} UnkStruct_022c798c_3;

typedef struct UnkStruct_022c798c_v {
    s32 x;
    s32 y;
    s32 z;
} UnkStruct_022c798c_v;

extern s16 data_0206e470[];
extern UnkStruct_022c798c_2 *data_ov000_02369acc;
extern s16 data_ov000_02362b94[];
extern u16 data_ov000_02362b84[];

void *func_ov000_022fad3c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i);
void func_ov000_022cc530(UnkStruct_022c798c_v *v, s32 a);
void func_ov000_022ca484(void *obj, s32 x, s32 y, s32 z, s32 a, s32 b, s32 c, s32 d);
u8 *func_ov000_023282b8(s32 a, s32 x, s32 y, s32 z);
void MTX_RotY33_(void *m, s32 s, s32 c);
void func_02036754(s32 a, s32 b, s32 c);
void func_ov000_022c7630(UnkStruct_022c798c *s);
void func_ov000_022c7be4(UnkStruct_022c798c *s);

void func_ov000_022c798c(UnkStruct_022c798c *s) {
    void *obj;
    UnkStruct_022c798c_v v;
    UnkStruct_022c798c_2 *g;
    u8 *p;
    u16 a;
    obj = func_ov000_022fad3c(0xd3, 0, 0, 0, 0, 0, 0, 0, 0);
    func_ov000_022cc530(&v, s->unk20C & 1);
    func_ov000_022ca484(obj, v.x, v.y, v.z, s->unk214 << 4, -0x200000, 0, data_ov000_02362b94[data_ov000_02369acc->unk22A]);
    g = data_ov000_02369acc;
    p = func_ov000_023282b8(1, v.x >> 4, -v.y >> 4, v.z >> 4);
    if (s->unk20C & 1)
        a = g->unk21C;
    else
        a = g->unk21E;
    MTX_RotY33_(p + 0x158, data_0206e470[((a & 0xffff) >> 4) * 2], data_0206e470[((a & 0xffff) >> 4) * 2 + 1]);
    func_02036754(0, 0, 0x9c);
    s->unk20C++;
    if (data_ov000_02362b84[data_ov000_02369acc->unk22A] <= s->unk20C)
        s->func = func_ov000_022c7630;
    else
        s->func = func_ov000_022c7be4;
}
