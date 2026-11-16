#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s32 unk4;
    u8 pad8[2];
    u16 unkA;
    u8 padC[0x18 - 0xC];
    void *unk18;
    void *unk1C;
    u32 unk20;
    u8 pad24[0x2C - 0x24];
} UnkStruct_022de860_C;

typedef struct {
    u8 pad0[0x134];
    UnkStruct_022de860_C coll0;
    UnkStruct_022de860_C coll1;
    UnkStruct_022de860_C coll2;
} UnkStruct_022de860_O;

typedef struct {
    u8 pad0[0x210];
    UnkStruct_022de860_O *unk210;
    u8 pad214[0x4E0 - 0x214];
    u8 unk4E0[0xBD0 - 0x4E0];
    s32 unkBD0;
    u8 padBD4[0xBF0 - 0xBD4];
    s32 unkBF0;
} UnkStruct_022de860_M;

typedef struct {
    u8 pad[4];
    s32 unk4;
} UnkStruct_02369ac0;

extern UnkStruct_022de860_M *data_ov000_02369b2c;
extern UnkStruct_02369ac0 data_ov000_02369ac0;

void func_ov000_022db7ec(void *v, UnkStruct_022de860_O *o);
void func_0206e3c0(UnkStruct_022de860_C *c, s16 a, s16 b, s16 d, s16 e);
void func_02069090(void *w, void *a);
void func_ov000_022db894(UnkStruct_022de860_C *h, UnkStruct_022de860_C *out);
void func_ov000_022ded44(void);

void func_ov000_022de860(void) {
    UnkStruct_022de860_M *m = data_ov000_02369b2c;
    UnkStruct_022de860_O *o = m->unk210;
    UnkStruct_022de860_C *c;

    func_ov000_022db7ec(m->unk4E0, o);
    if (m->unkBD0 != 0 || data_ov000_02369ac0.unk4 == 0x18) {
        c = &o->coll1;
        func_0206e3c0(c, -0x4b, -0x28, 0x4b, 0x28);
        c->unk1C = NULL;
        if (m->unkBF0 != 0)
            c->unk20 |= 0x20;
        else
            c->unk20 &= ~0x20;
    } else {
        c = &o->coll0;
        func_0206e3c0(c, -0x32, -0x23, 0x32, 0x23);
        c->unk1C = func_ov000_022ded44;
    }
    c->unk18 = o;
    c->unkA |= 4;
    func_02069090(o, c);
    c->unk4 += 0x64;
    func_ov000_022db894(c, &o->coll2);
    o->coll2.unk4 = c->unk4;
}
