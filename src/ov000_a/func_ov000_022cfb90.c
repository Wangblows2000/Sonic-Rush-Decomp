#include <nitro/types.h>

typedef struct UnkStruct_022cfb90_a {
    u8 pad[0xb];
    u8 unkB;
    u8 pad0[0x4c - 0xc];
    s32 unk4C;
    u8 pad1[0x10c - 0x50];
    u8 unk10C;
    u8 pad2[0x114 - 0x10d];
    s32 unk114;
    u8 pad3[0x138 - 0x118];
} UnkStruct_022cfb90_a;

typedef struct UnkStruct_022cfb90 UnkStruct_022cfb90;
struct UnkStruct_022cfb90 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad0[0x54 - 0x1c];
    u32 unk54;
    u8 pad0b[0x68 - 0x58];
    void (*func)(UnkStruct_022cfb90 *s);
    u8 pad1[0x460 - 0x6c];
    s32 unk460;
    u8 pad1b[0x598 - 0x464];
    s32 unk598;
    u8 pad2[0xa9c - 0x59c];
    UnkStruct_022cfb90_a unkA9C;
    UnkStruct_022cfb90_a unkBD4;
    UnkStruct_022cfb90_a unkD0C;
    u8 pad3[0xf7c - 0xe44];
    s32 unkF7C;
    u8 pad4[0x12f4 - 0xf80];
    s32 unk12F4;
    u8 pad5[0x1304 - 0x12f8];
    u16 unk1304;
    u8 pad6[0x145c - 0x1306];
    s32 unk145C;
    s32 unk1460;
};

typedef struct UnkStruct_022cfb90_c {
    u8 pad[0x54];
    s32 unk54;
    s32 unk58;
    s32 unk5C;
    u8 pad1[0x78 - 0x60];
    s32 unk78;
    s32 unk7C;
    u8 pad2[0x84 - 0x80];
    s32 unk84;
    s32 unk88;
    u8 pad3[0x8e - 0x8c];
    u16 unk8E;
    u16 unk90;
} UnkStruct_022cfb90_c;

typedef struct UnkStruct_022cfb90_pl {
    u8 pad[0x5c];
    u16 unk5C;
    u8 pad1[0x32c - 0x5e];
    u32 unk32C;
} UnkStruct_022cfb90_pl;

typedef struct UnkStruct_022cfb90_5 {
    u8 pad[0x48];
    s16 unk48;
} UnkStruct_022cfb90_5;

extern UnkStruct_022cfb90_5 data_0208922c;
extern UnkStruct_022cfb90_c data_ov000_0236a48c;
extern UnkStruct_022cfb90_pl *data_ov000_0236ac60;
extern s32 data_ov000_023651fc[][3];

void func_ov000_022cf92c(UnkStruct_022cfb90 *s);
void func_ov000_022cfe5c(s32 a, s32 b);
void func_02036754(s32 a, s32 b, s32 c);
void func_02031a68(void *p, s32 a, s32 b, s32 c, s32 d);
void func_0206c7c0(void);
u8 *func_ov000_02328860(s32 a, s32 x, s32 y, s32 z);
void func_020537b4(s32 a);
void func_020533e0(s32 a, s32 b);

void func_ov000_022cfb90(UnkStruct_022cfb90 *s) {
    UnkStruct_022cfb90_c *c;
    UnkStruct_022cfb90_a *x;
    UnkStruct_022cfb90_a *y;
    UnkStruct_022cfb90_a *z;
    u32 v;
    u8 *obj;
    s->unk12F4--;
    if (data_0208922c.unk48 > 0)
        data_0208922c.unk48 -= 2;
    if (s->unk12F4 > 0)
        return;
    s->unk145C++;
    if (s->unk145C >= 3) {
    y = &s->unkBD4;
    s->func = func_ov000_022cf92c;
    z = &s->unkD0C;
    y->unkB = 0;
    z->unkB = 0;
    y->unk4C = -0x200000;
    z->unk4C = -0x200000;
    v = (*(vu32 *)0x4000000 & 0x1f00) >> 8;
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | ((v & ~4) << 8);
    x = &s->unkA9C;
    c = (UnkStruct_022cfb90_c *)(u32)&data_ov000_0236a48c;
    func_02031a68(x, 0, s->unkF7C, 0x22, 0);
    func_02031a68(y, 0, s->unkF7C, 0x3e, 0);
    func_02031a68(z, 0, s->unkF7C, 0x5e, 0);
    x->unk10C &= ~4;
    y->unk10C &= ~4;
    z->unk10C &= ~4;
    x->unk114 = y->unk114 = z->unk114 = 0x1000;
    s->unk1304 = 0;
    s->unk10 = 0;
    s->unk14 = 0x20000;
    s->unk18 = 0;
    func_0206c7c0();
    s->unk54 &= ~0x20;
    s->unk460 = s->unk598 = 0x1000;
    data_ov000_0236ac60->unk5C |= 0x20;
    data_ov000_0236ac60->unk32C |= 0x200000;
    data_0208922c.unk48 = 0x10;
    c->unk54 = 0;
    c->unk58 = -data_ov000_023651fc[s->unk1460][0] << 12;
    c->unk5C = 0;
    c->unk78 = c->unk7C = 0x12c000;
    c->unk84 = c->unk88 = -0x80000;
    c->unk8E = c->unk90 = 0;
    obj = func_ov000_02328860(0, 0, data_ov000_023651fc[s->unk1460][0] << 8, 0);
    *(s32 *)(obj + 0x248) = 0x800;
    func_02036754(0, 0, 0x7d);
    s->unk12F4 = 0x12c;
    func_020537b4(0xd);
    func_020533e0(0x13, 0xf0);
    } else {
        s->unk12F4 = 0x1e;
        func_ov000_022cfe5c(s->unk1460, s->unk145C);
        data_0208922c.unk48 = 0x10;
        func_02036754(0, 0, 0x7c);
    }
}
