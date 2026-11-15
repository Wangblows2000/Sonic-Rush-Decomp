#include <nitro/types.h>

typedef struct UnkStruct_022c6bc0_m {
    s32 m[9];
} UnkStruct_022c6bc0_m;

typedef struct UnkStruct_022c6bc0_4 {
    u8 pad[8];
    u16 unk8;
} UnkStruct_022c6bc0_4;

typedef struct UnkStruct_022c6bc0_5 {
    u8 pad[0x20c];
    u32 unk20C;
} UnkStruct_022c6bc0_5;

typedef struct UnkStruct_022c6bc0_2 {
    u8 unk0;
} UnkStruct_022c6bc0_2;

typedef struct UnkStruct_022c6bc0 {
    s32 unk0;
    s32 unk4;
    u8 pad[0x10];
    UnkStruct_022c6bc0_2 *unk18;
} UnkStruct_022c6bc0;

extern u32 data_0208fe70;
extern s16 data_0206e470[];
extern UnkStruct_022c6bc0_4 data_ov000_02369ac0;
extern UnkStruct_022c6bc0_5 *data_ov000_02369acc;

void func_ov000_022cbdf4(void);
void func_ov000_022cbdb0(void);
u8 *func_ov000_02328710(s32 a, s32 b, s32 c, s32 d);
void func_02000e60(void *m, s32 s, s32 c);
void MTX_RotY33_(void *m, s32 s, s32 c);
void func_02000f34(void *a, void *b, void *c);
void func_020537b4(s32 a);
void func_020533e0(s32 a, s32 b);
void func_020367b4(s32 a, s32 b);
void func_ov000_0234aa80(UnkStruct_022c6bc0_2 *p);

static inline u16 Rand(void) {
    data_0208fe70 = data_0208fe70 * 0x196225 + 0x3c6ef35f;
    return (u16)(data_0208fe70 >> 16);
}

void func_ov000_022c6bc0(void *a, UnkStruct_022c6bc0 *b) {
    UnkStruct_022c6bc0_m mtx;
    UnkStruct_022c6bc0_2 *p = b->unk18;
    u8 *obj;
    u16 ang;
    if (data_ov000_02369ac0.unk8 == 0)
        return;
    data_ov000_02369ac0.unk8--;
    if (data_ov000_02369ac0.unk8 != 0) {
        func_ov000_022cbdf4();
        obj = func_ov000_02328710(1, b->unk0 << 8, b->unk4 << 8, 0);
        ang = (Rand() & 0xfff) + 0xaaa;
        func_02000e60(obj + 0x158, data_0206e470[((ang & 0xffff) >> 4) * 2], data_0206e470[((ang & 0xffff) >> 4) * 2 + 1]);
        ang = Rand();
        MTX_RotY33_(&mtx, data_0206e470[((ang & 0xffff) >> 4) * 2], data_0206e470[((ang & 0xffff) >> 4) * 2 + 1]);
        func_02000f34(obj + 0x158, &mtx, obj + 0x158);
        *(s32 *)(obj + 0x248) = 0x1000;
        if (data_ov000_02369ac0.unk8 == 2) {
            func_020537b4(0xb);
            func_020533e0(0x12, 0xf0);
            func_020367b4(0, 0xf);
            data_ov000_02369acc->unk20C |= 2;
        } else {
            func_020537b4(0xb);
            func_020533e0(0x10, 0xf0);
        }
        if (p == NULL)
            return;
        if (p->unk0 != 1)
            return;
        func_ov000_0234aa80(b->unk18);
    } else {
        func_ov000_022cbdb0();
    }
}
