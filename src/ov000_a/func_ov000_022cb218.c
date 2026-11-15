#include <nitro/types.h>

typedef struct UnkStruct_022cb218_3 {
    u8 pad[0x5c];
    u16 unk5C;
} UnkStruct_022cb218_3;

typedef struct UnkStruct_022cb218 {
    u8 pad[0x5c];
    u16 unk5C;
    u8 pad1[0x210 - 0x5e];
    UnkStruct_022cb218_3 *unk210;
    UnkStruct_022cb218_3 *unk214;
    u8 pad2[0x238 - 0x218];
    s32 unk238;
} UnkStruct_022cb218;

typedef struct UnkStruct_022cb218_v {
    s32 x;
    s32 y;
    s32 z;
} UnkStruct_022cb218_v;

typedef struct UnkStruct_022cb218_c {
    u8 pad[0x18];
    UnkStruct_022cb218_v unk18;
    UnkStruct_022cb218_v unk24;
    u8 pad1[0x54 - 0x30];
    UnkStruct_022cb218_v unk54;
    UnkStruct_022cb218_v unk60;
} UnkStruct_022cb218_c;

typedef struct UnkStruct_022cb218_a {
    u8 pad[0x18];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
} UnkStruct_022cb218_a;

typedef struct UnkStruct_022cb218_6 {
    u32 unk0;
} UnkStruct_022cb218_6;

extern UnkStruct_022cb218_6 data_ov000_02369ac0;
extern UnkStruct_022cb218_6 data_ov000_0236a444;
extern UnkStruct_022cb218_a data_ov000_0236a524;
extern UnkStruct_022cb218_c data_ov000_0236a48c;

void func_02031c1c(void *p, s32 a);
void func_02031bd0(void *p, s32 a, s32 b);
void func_ov000_022cc224(void);
void func_020537b4(s32 a);
void func_020533e0(s32 a, s32 b);
void func_ov000_022cbfc8(void);

void func_ov000_022cb218(UnkStruct_022cb218 *s) {
    UnkStruct_022cb218_a *a;
    UnkStruct_022cb218_c *c;
    UnkStruct_022cb218_3 *p1;
    UnkStruct_022cb218_3 *p2;
    if (!(data_ov000_02369ac0.unk0 & 1))
        return;
    a = (UnkStruct_022cb218_a *)(u32)&data_ov000_0236a524;
    p1 = s->unk210;
    p2 = s->unk214;
    s->unk5C &= ~0x20;
    p1->unk5C &= ~0x20;
    p2->unk5C &= ~0x20;
    data_ov000_0236a444.unk0 |= 1;
    func_02031c1c(a, 0);
    func_02031bd0(a, s->unk238, 0);
    a->unk18 = a->unk1C = a->unk20 = 0x1355b50;
    c = (UnkStruct_022cb218_c *)(u32)&data_ov000_0236a48c;
    func_ov000_022cc224();
    c->unk24.x = 0xc0000;
    c->unk24.y = -0x1d4000;
    c->unk24.z = 0xa0000;
    c->unk18 = c->unk24;
    c->unk60.x = 0xc0000;
    c->unk60.y = -0x1c0000;
    c->unk60.z = 0;
    c->unk54 = c->unk60;
    func_020537b4(9);
    func_020533e0(0xf, 0xf0);
    func_ov000_022cbfc8();
}
