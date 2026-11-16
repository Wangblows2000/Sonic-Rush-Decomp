#include <nitro/types.h>

typedef struct UnkStruct_022eee68_o {
    u8 pad0[0x18];
    s32 x;
    s32 y;
    u8 pad20[0xb8 - 0x20];
    u32 flagsB8;
    u8 padBC[0xf0 - 0xbc];
    u32 flagsF0;
    u8 padF4[0x128 - 0xf4];
    u32 flags128;
    u8 pad12C[0x160 - 0x12c];
    u32 flags160;
    u8 pad164[0x1dc - 0x164];
    struct UnkStruct_022eee68_w *work;
} UnkStruct_022eee68_o;

typedef struct UnkStruct_022eee68_w {
    u8 pad0[0xc];
    s32 unkC;
    u8 pad10[0x16 - 0x10];
    s16 unk16;
    UnkStruct_022eee68_o *unk18;
    u8 pad1C[0x1c0 - 0x1c];
    s32 unk1C0;
} UnkStruct_022eee68_w;

typedef struct UnkStruct_022eee68_c {
    u8 pad0[0x308];
    s32 lives;
} UnkStruct_022eee68_c;

extern u32 gRandSeed;
extern s32 data_ov001_023167e4[];

UnkStruct_022eee68_c *func_ov001_022ffce8(void);
void func_ov001_022fc8dc(s32 a, s32 x, s32 y, s32 b);
void func_ov001_022efcdc(UnkStruct_022eee68_o *p);
void func_ov001_022ef6c4(UnkStruct_022eee68_o *p);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void func_020366f4(s32 a, s32 b, s32 c);

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return gRandSeed >> 16;
}

void func_ov001_022eee68(UnkStruct_022eee68_o **self, UnkStruct_022eee68_o **other) {
    UnkStruct_022eee68_o *s = *self;
    UnkStruct_022eee68_o *p = *other;
    UnkStruct_022eee68_w *w = p->work;
    UnkStruct_022eee68_o *o = w->unk18;
    UnkStruct_022eee68_c *c = func_ov001_022ffce8();

    c->lives--;
    p->flagsB8 |= 0x200;
    p->flagsF0 |= 0x200;
    p->flags128 |= 0x200;
    p->flags160 |= 0x200;
    o->flags160 |= 0x200;
    o->flags128 |= 0x200;
    w->unkC = 0;
    w->unk16 = 0;
    func_ov001_022fc8dc(0, s->x, s->y, 0);
    w->unk1C0 = 0;
    if (c->lives > 0) {
        func_ov001_022efcdc(p);
        Sound_PlaySeqArc(0, 0, 0x7b);
    } else {
        func_ov001_022ef6c4(p);
        Sound_PlaySeqArc(0, 0, 0xba);
    }
    func_020366f4(0, 0, data_ov001_023167e4[Rand() & 1]);
}
