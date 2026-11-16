#include <nitro/types.h>

typedef struct UnkStruct_022d8db0_p {
    u8 unk0;
    u8 pad[0x2e - 1];
    s16 unk2E;
    u8 pad1[0x20c - 0x30];
    u32 unk20C;
    u8 pad2[0x2d8 - 0x210];
    s32 unk2D8;
    s32 unk2DC;
    s32 unk2E0;
} UnkStruct_022d8db0_p;

typedef struct UnkStruct_022d8db0_o {
    u8 pad[0x5c];
    u16 unk5C;
} UnkStruct_022d8db0_o;

typedef struct UnkStruct_022d8db0_h {
    u8 pad[0xa];
    u16 unkA;
    u8 pad1[0x18 - 0xc];
    void *unk18;
} UnkStruct_022d8db0_h;

typedef struct UnkStruct_022d8db0_q {
    u8 pad[0x158];
    u8 unk158[0x24];
} UnkStruct_022d8db0_q;

typedef struct UnkStruct_022d8db0_7 {
    u8 pad[8];
    u16 unk8;
} UnkStruct_022d8db0_7;

typedef struct UnkStruct_022d8db0_b {
    u8 pad[0x20c];
    u32 unk20C;
} UnkStruct_022d8db0_b;

extern UnkStruct_022d8db0_7 data_ov000_02369ac0;
extern UnkStruct_022d8db0_b *data_ov000_02369b14;
extern u32 data_0208fe70;
extern s16 data_0206e470[];

void func_0206d8bc(UnkStruct_022d8db0_h *a, UnkStruct_022d8db0_h *b);
void func_ov000_022daab0(void);
void func_ov000_022daaf4(void);
UnkStruct_022d8db0_q *func_ov000_02328710(s32 a, s32 b, s32 c, s32 d);
void func_02000e60(void *m, s32 s, s32 c);
void MTX_RotY33_(void *m, s32 s, s32 c);
void func_02000f34(void *a, void *b, void *c);
void func_020537b4(s32 a);
void func_020533e0(s32 a, s32 b);
void func_020367b4(s32 a, s32 b);

static inline u16 Rand(void) {
    data_0208fe70 = data_0208fe70 * 0x196225 + 0x3c6ef35f;
    return (u16)(data_0208fe70 >> 16);
}

void func_ov000_022d8db0(UnkStruct_022d8db0_h *a, UnkStruct_022d8db0_h *b) {
    UnkStruct_022d8db0_o *o = a->unk18;
    UnkStruct_022d8db0_p *p = b->unk18;
    UnkStruct_022d8db0_q *q;
    u8 mtx[0x24];
    u16 ang;
    if ((void *)o == (void *)p) {
        func_0206d8bc(a, b);
        return;
    }
    if (p->unk0 == 1) {
        func_0206d8bc(a, b);
        return;
    }
    if (!(p->unk20C & 0x40)) {
        func_0206d8bc(a, b);
        return;
    }
    b->unkA |= 0x1000;
    data_ov000_02369ac0.unk8--;
    if (data_ov000_02369ac0.unk8 != 0) {
        if (p->unk2E > 0)
            o->unk5C |= 1;
        else
            o->unk5C &= ~1;
        func_ov000_022daaf4();
        q = func_ov000_02328710(1, p->unk2D8 >> 4, -p->unk2DC >> 4, p->unk2E0 >> 4);
        ang = (Rand() & 0xfff) + 0xaaa;
        func_02000e60(q->unk158, data_0206e470[((ang & 0xffff) >> 4) * 2], data_0206e470[((ang & 0xffff) >> 4) * 2 + 1]);
        ang = Rand();
        MTX_RotY33_(mtx, data_0206e470[((ang & 0xffff) >> 4) * 2], data_0206e470[((ang & 0xffff) >> 4) * 2 + 1]);
        func_02000f34(q->unk158, mtx, q->unk158);
        if (data_ov000_02369ac0.unk8 == 2) {
            func_020537b4(0xb);
            func_020533e0(0x12, 0xf0);
            func_020367b4(0, 0xf);
            data_ov000_02369b14->unk20C |= 1;
        } else {
            func_020537b4(0xb);
            func_020533e0(0x10, 0xf0);
        }
    } else {
        func_ov000_022daab0();
    }
}
