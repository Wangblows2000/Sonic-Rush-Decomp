#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    u8 unk10[0x5c - 0x10];
    u16 unk5C;
    u8 pad5E[0x158 - 0x5e];
    u8 unk158[0x24];
    u8 unk17C[0x328 - 0x17c];
    s16 unk328;
} UnkStruct_022de768;

typedef struct {
    u8 pad0[4];
    s32 unk4;
} UnkStruct_02369ac0;

typedef struct {
    u8 pad0[0xd6];
    u16 unkD6;
} UnkStruct_0236a444;

extern UnkStruct_022de768 *data_ov000_0236ac60;
extern UnkStruct_02369ac0 data_ov000_02369ac0;
extern UnkStruct_0236a444 data_ov000_0236a444;
extern s16 data_0206e470[];

void func_ov000_022db6c8(void *a);
void func_ov000_022db7d4(void *a, u16 *b, void *c);
void MTX_RotY33_(void *m, s32 s, s32 c);
void func_ov000_022de370(void);

void func_ov000_022de768(void) {
    u16 ang[4];
    UnkStruct_022de768 *p;
    s32 st;
    s32 off;

    p = data_ov000_0236ac60;
    if (p->unk5C & 0x20)
        return;
    func_ov000_022db6c8(p->unk10);
    func_ov000_022db7d4(p->unk17C, ang, p);
    if (p->unk5C & 1)
        off = 0xc000;
    else
        off = 0x4000;
    MTX_RotY33_(p->unk158, data_0206e470[(((u16)(off + ang[0]) & 0xffff) >> 4) * 2], data_0206e470[(((u16)(off + ang[0]) & 0xffff) >> 4) * 2 + 1]);
    st = data_ov000_02369ac0.unk4;
    if (st == 0x1a)
        return;
    data_ov000_0236a444.unkD6 = ang[0];
    if (st == 0x18)
        return;
    if (p->unk328 == 0x30)
        return;
    func_ov000_022de370();
}
