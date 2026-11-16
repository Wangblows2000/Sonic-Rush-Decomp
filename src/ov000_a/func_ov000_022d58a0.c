#include <nitro/types.h>

typedef struct UnkStruct_022d58a0_v {
    s32 x;
    s32 y;
    s32 z;
} UnkStruct_022d58a0_v;

typedef struct UnkStruct_022d58a0 UnkStruct_022d58a0;
struct UnkStruct_022d58a0 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad0[0x54 - 0x18];
    u32 unk54;
    u8 pad1[0x5c - 0x58];
    u16 unk5C;
    u8 pad2[0x68 - 0x5e];
    void (*func)(UnkStruct_022d58a0 *s);
    u8 pad3[0x13e - 0x6c];
    u16 unk13E;
    u8 pad4[0x16a - 0x140];
    u16 unk16A;
    u8 pad5[0x20c - 0x16c];
    u32 unk20C;
    u16 unk210;
    u8 pad6[0x214 - 0x212];
    UnkStruct_022d58a0_v unk214;
    u8 pad7[0x22c - 0x220];
    u16 unk22C;
    u8 pad8[0x234 - 0x22e];
    u16 unk234;
    u8 pad9[0x2b4 - 0x236];
    u8 unk2B4[0x24];
    UnkStruct_022d58a0_v unk2D8;
};

extern s16 data_0206e470[];

void VEC_Add(void *a, void *b, void *c);
s32 VEC_Mag(void *v);
u16 func_ov000_022c5e10(s32 x, s32 z);
void MTX_RotX33_(void *m, s32 s, s32 c);
void MTX_RotY33_(void *m, s32 s, s32 c);
void MTX_Concat33(void *a, void *b, void *c);
void func_ov000_022d57b8(UnkStruct_022d58a0 *s);

void func_ov000_022d58a0(UnkStruct_022d58a0 *s) {
    u8 mtx[0x24];
    UnkStruct_022d58a0_v v;
    u16 ang;
    BOOL flag = FALSE;
    VEC_Add(&s->unk2D8, &s->unk214, &s->unk2D8);
    s->unk214.y -= 0x199;
    ang = func_ov000_022c5e10(s->unk214.x, s->unk214.z);
    s->unk210 += (u16)(VEC_Mag(&s->unk214) >> 4);
    MTX_RotX33_(s->unk2B4, data_0206e470[((s->unk210 & 0xffff) >> 4) * 2], data_0206e470[((s->unk210 & 0xffff) >> 4) * 2 + 1]);
    MTX_RotY33_(mtx, data_0206e470[((ang & 0xffff) >> 4) * 2], data_0206e470[((ang & 0xffff) >> 4) * 2 + 1]);
    MTX_Concat33(s->unk2B4, mtx, s->unk2B4);
    v.x = s->unk2D8.x;
    v.y = 0;
    v.z = s->unk2D8.z;
    if (VEC_Mag(&v) > 0xf42f8 || s->unk2D8.y < -0x200000)
        flag = TRUE;
    if (flag)
        s->func = func_ov000_022d57b8;
}
