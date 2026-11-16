#include <nitro/types.h>

typedef struct UnkStruct_022da24c_o {
    u8 pad[0x5c];
    u16 unk5C;
} UnkStruct_022da24c_o;

typedef struct UnkStruct_022da24c {
    u8 pad[0x5c];
    u16 unk5C;
    u8 pad1[0x214 - 0x5e];
    UnkStruct_022da24c_o *unk214;
    UnkStruct_022da24c_o *unk218;
    u8 pad2[0x238 - 0x21c];
    s32 unk238;
} UnkStruct_022da24c;

typedef struct UnkStruct_022da24c_p {
    u8 pad[0x5c];
    u16 unk5C;
    u8 pad1[0x70 - 0x5e];
    void (*unk70)(void);
} UnkStruct_022da24c_p;

typedef struct UnkStruct_022da24c_m {
    u8 pad[0x18];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
} UnkStruct_022da24c_m;

typedef struct UnkStruct_022da24c_v {
    s32 x;
    s32 y;
    s32 z;
} UnkStruct_022da24c_v;

typedef struct UnkStruct_022da24c_c {
    u8 pad[0x18];
    UnkStruct_022da24c_v unk18;
    UnkStruct_022da24c_v unk24;
    u8 pad1[0x54 - 0x30];
    UnkStruct_022da24c_v unk54;
    UnkStruct_022da24c_v unk60;
} UnkStruct_022da24c_c;

typedef struct UnkStruct_022da24c_7 {
    u32 unk0;
} UnkStruct_022da24c_7;

extern UnkStruct_022da24c_7 data_ov000_02369ac0;
extern UnkStruct_022da24c_p *data_ov000_0236ac60;
extern UnkStruct_022da24c_7 data_ov000_0236a444;
extern UnkStruct_022da24c_m data_ov000_0236a524;
extern UnkStruct_022da24c_c data_ov000_0236a48c;

void func_ov000_022da4b4(void);
void func_02031c1c(void *p, s32 a);
void func_02031bd0(void *p, s32 a, s32 b);
void func_ov000_022da6b4(void);
void func_020537b4(s32 a);
void func_020533e0(s32 a, s32 b);
void func_ov000_022dac60(void);

void func_ov000_022da24c(UnkStruct_022da24c *s) {
    UnkStruct_022da24c_o *a;
    UnkStruct_022da24c_o *b;
    UnkStruct_022da24c_m *m;
    UnkStruct_022da24c_c *c;
    if (!(data_ov000_02369ac0.unk0 & 1))
        return;
    a = s->unk214;
    b = s->unk218;
    s->unk5C &= ~0x20;
    a->unk5C &= ~0x20;
    if (b != NULL)
        b->unk5C &= ~0x20;
    m = (UnkStruct_022da24c_m *)(u32)&data_ov000_0236a524;
    data_ov000_0236ac60->unk70 = func_ov000_022da4b4;
    data_ov000_0236ac60->unk5C |= 0x2040;
    data_ov000_0236a444.unk0 |= 6;
    data_ov000_0236a444.unk0 |= 1;
    func_02031c1c(m, 0);
    func_02031bd0(m, s->unk238, 0);
    m->unk20 = 0x1355b50;
    m->unk1C = m->unk20;
    m->unk18 = m->unk1C;
    c = (UnkStruct_022da24c_c *)(u32)&data_ov000_0236a48c;
    func_ov000_022da6b4();
    c->unk24.x = 0xffedc800;
    c->unk24.y = -0x1d0000;
    c->unk24.z = -0x134000;
    c->unk18 = c->unk24;
    c->unk60.x = 0;
    c->unk60.y = -0x200000;
    c->unk60.z = 0;
    c->unk54 = c->unk60;
    func_020537b4(9);
    func_020533e0(0xf, 0xf0);
    func_ov000_022dac60();
}
