#include <nitro/types.h>

typedef struct UnkStruct_022ca11c_3 {
    u8 pad[0xa];
    u8 unkA;
    u8 unkB;
    u8 unkC;
    u8 pad1[0x48 - 0xd];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
} UnkStruct_022ca11c_3;

typedef struct UnkStruct_022ca11c {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad1[0x5c - 0x1c];
    u16 unk5C;
    u8 pad2[0x228 - 0x5e];
    UnkStruct_022ca11c_3 unk228;
} UnkStruct_022ca11c;

typedef struct UnkStruct_022ca11c_2 {
    u8 pad[0x10];
    UnkStruct_022ca11c *unk10;
} UnkStruct_022ca11c_2;

typedef struct UnkStruct_022ca11c_4 {
    u8 pad[0x20c];
    u32 unk20C;
} UnkStruct_022ca11c_4;

extern UnkStruct_022ca11c_2 *data_022b4574;
extern UnkStruct_022ca11c_4 *data_ov000_02369acc;

void func_020316f4(UnkStruct_022ca11c_3 *p);
void func_020315e0(UnkStruct_022ca11c_3 *p);
void func_02025268(s32 a, void *p, s32 n);
void func_02025a94(void *p, s32 a);

void func_ov000_022ca11c(void) {
    UnkStruct_022ca11c *o = data_022b4574->unk10;
    UnkStruct_022ca11c_3 *p = &o->unk228;
    if (o->unk5C & 0x20)
        return;
    p->unk48 = o->unk10 << 4;
    p->unk4C = -(o->unk14 << 4);
    p->unk50 = o->unk18 << 4;
    func_020316f4(p);
    func_020315e0(p);
}
