#include <nitro/types.h>

typedef struct UnkStruct_022c736c_3 {
    u8 pad[0xa];
    u8 unkA;
    u8 unkB;
    u8 unkC;
    u8 pad1[0x48 - 0xd];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
} UnkStruct_022c736c_3;

typedef struct UnkStruct_022c736c {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad1[0x5c - 0x1c];
    u16 unk5C;
    u8 pad2[0x2d0 - 0x5e];
    u8 unk2D0[0x2f4 - 0x2d0];
    s32 unk2F4;
    s32 unk2F8;
    s32 unk2FC;
    u8 pad3[0x30c - 0x300];
    UnkStruct_022c736c_3 unk30C;
} UnkStruct_022c736c;

typedef struct UnkStruct_022c736c_2 {
    u8 pad[0x10];
    UnkStruct_022c736c *unk10;
} UnkStruct_022c736c_2;

typedef struct UnkStruct_022c736c_4 {
    u8 pad[0x20c];
    u32 unk20C;
} UnkStruct_022c736c_4;

extern UnkStruct_022c736c_2 *data_022b4574;
extern UnkStruct_022c736c_4 *data_ov000_02369acc;

void func_020316f4(UnkStruct_022c736c_3 *p);
void func_020315e0(UnkStruct_022c736c_3 *p);
void func_02025268(s32 a, void *p, s32 n);
void func_02025a94(void *p, s32 a);

void func_ov000_022c736c(void) {
    UnkStruct_022c736c *o = data_022b4574->unk10;
    UnkStruct_022c736c_3 *p = &o->unk30C;
    if (o->unk5C & 0x20)
        return;
    if (data_ov000_02369acc->unk20C & 1) {
        p->unkA = 3;
        p->unkB = 0;
        p->unk48 = o->unk10 << 4;
        p->unk4C = -(o->unk14 << 4);
        p->unk50 = o->unk18 << 4;
        func_020316f4(p);
        func_020315e0(p);
        return;
    }
    p->unkA = 4;
    p->unkB = 2;
    p->unkC = 0;
    p->unk48 = 0;
    p->unk4C = 0;
    p->unk50 = 0;
    func_020316f4(p);
    func_020315e0(p);
    {
        u32 v = 2;
        func_02025268(0x10, &v, 1);
    }
    {
        u32 v = 0x1e;
        func_02025268(0x14, &v, 1);
    }
    func_02025a94(o->unk2D0, 0);
    o->unk10 = o->unk2F4 >> 4;
    o->unk14 = -o->unk2F8 >> 4;
    o->unk18 = o->unk2FC >> 4;
}
