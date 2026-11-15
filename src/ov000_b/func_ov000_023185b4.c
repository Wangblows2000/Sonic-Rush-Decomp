#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2C];
    u32 unk2C;
    u8 pad30[0x44 - 0x30];
    u16 unk44;
    u8 pad46[2];
} UnkStruct_023185b4_Spr;

typedef struct {
    u8 pad0[6];
    s8 unk6;
    u8 pad7;
    u8 unk8;
} UnkStruct_023185b4_Param;

typedef struct {
    u8 pad0[0xA];
    u16 unkA;
    u8 padC[0x1C - 0xC];
} UnkStruct_023185b4_Coll;

typedef struct {
    u8 pad0[0x58];
    u32 unk58;
    u8 pad5C[0x70 - 0x5C];
    void *unk70;
    u8 pad74[0x94 - 0x74];
    UnkStruct_023185b4_Spr spr0;
    UnkStruct_023185b4_Spr spr1;
    u8 pad124[0x1E0 - 0x124];
    UnkStruct_023185b4_Param *unk1E0;
    u8 pad1E4[0x20C - 0x1E4];
    UnkStruct_023185b4_Spr s0;
    UnkStruct_023185b4_Spr s1;
    UnkStruct_023185b4_Spr s2;
    UnkStruct_023185b4_Spr s3;
    UnkStruct_023185b4_Spr s4;
    UnkStruct_023185b4_Spr s5;
    UnkStruct_023185b4_Spr s6;
    UnkStruct_023185b4_Spr s7;
    UnkStruct_023185b4_Coll unk44C;
    void *unk468;
    u8 pad46C[0x474 - 0x46C];
    u8 unk474;
    u8 pad475[0x4A4 - 0x475];
    u32 unk4A4;
} UnkStruct_023185b4;

typedef struct {
    u8 pad0[0x10];
    UnkStruct_023185b4 *work;
} UnkStruct_023185b4_Task;

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02367020[];
extern u8 data_ov000_0236a26c[];
extern u8 data_ov000_0236703c[];
extern u8 data_ov000_0236a274[];
extern u8 data_ov000_02367058[];
extern u8 data_ov000_0236a284[];
extern u8 data_ov000_02367074[];
extern u8 data_ov000_0236a294[];
extern u8 data_ov000_02367090[];

void func_0206c3d0(void);
void func_ov000_023184e4(void);
void func_ov000_023180d8(void);
void func_ov000_02317db8(void);
UnkStruct_023185b4_Task *func_020372d0(void (*f)(void), void (*g)(void), s32 b, s32 c, s32 d, s32 e);
UnkStruct_023185b4_Task *func_02006abc(s32 i);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void func_ov000_022fba50(UnkStruct_023185b4 *w, s32 a, s32 b, s32 c);
void func_02069f1c(UnkStruct_023185b4 *w, void *a, s32 b, void *c, void *d);
void func_0206a054(UnkStruct_023185b4 *w, s32 a, s32 b);
void func_02069918(UnkStruct_023185b4 *w, s32 a);
void func_020698f0(UnkStruct_023185b4 *w, s32 a);
void func_02069d38(UnkStruct_023185b4_Spr *a, UnkStruct_023185b4_Spr *b, void *c, s32 d, void *e, void *f);
void func_02033550(UnkStruct_023185b4_Spr *a, s32 b);
void func_0206990c(UnkStruct_023185b4_Spr *a, UnkStruct_023185b4_Spr *b, s32 c);
void func_02069e68(UnkStruct_023185b4_Spr *a, UnkStruct_023185b4_Spr *b, s32 c, void *d);
void func_020698bc(UnkStruct_023185b4_Spr *a, UnkStruct_023185b4_Spr *b, s32 c);
void func_0206e3c0(UnkStruct_023185b4_Coll *c, s32 a, s32 b, s32 d, s32 e);
void func_0206e2b4(UnkStruct_023185b4_Coll *c, s32 a, s32 b);
void func_ov000_02318080(UnkStruct_023185b4 *w);

UnkStruct_023185b4 *func_ov000_023185b4(s32 a, s32 b, s32 c) {
    UnkStruct_023185b4_Task *task;
    UnkStruct_023185b4 *w;
    vu16 zero;

    task = func_020372d0(func_0206c3d0, func_ov000_023184e4, 0, 0xf6, 2, sizeof(UnkStruct_023185b4));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_023185b4));
    func_ov000_022fba50(w, a, b, c);
    func_02069f1c(w, data_ov000_02367020, 0xffff, data_ov000_0236a26c, data_ov000_02369ba0);
    func_0206a054(w, 0, 0x47);
    w->spr0.unk2C |= 0x200;
    w->spr1.unk2C |= 0x200;
    func_02069918(w, 6);
    func_020698f0(w, 1);
    w->unk70 = func_ov000_023180d8;

    func_02069d38(&w->s0, &w->s1, data_ov000_0236703c, 0, data_ov000_0236a26c, data_ov000_02369ba0);
    func_02033550(&w->s0, 3);
    func_02033550(&w->s1, 3);
    func_0206990c(&w->s0, &w->s1, 0x14);
    w->s0.unk44 = w->spr0.unk44;
    w->s1.unk44 = w->spr0.unk44;
    func_02069e68(&w->s0, &w->s1, 1, data_ov000_0236a274);

    func_02069d38(&w->s2, &w->s3, data_ov000_02367058, 0, data_ov000_0236a26c, data_ov000_02369ba0);
    func_02033550(&w->s2, 2);
    func_02033550(&w->s3, 2);
    func_0206990c(&w->s2, &w->s3, 0x14);
    w->s2.unk44 = w->spr0.unk44;
    w->s3.unk44 = w->spr0.unk44;
    func_02069e68(&w->s2, &w->s3, 1, data_ov000_0236a284);

    func_02069d38(&w->s4, &w->s5, data_ov000_02367074, 0, data_ov000_0236a26c, data_ov000_02369ba0);
    func_02033550(&w->s4, 4);
    func_02033550(&w->s5, 4);
    func_0206990c(&w->s4, &w->s5, 0x13);
    w->s4.unk44 = w->spr0.unk44;
    w->s5.unk44 = w->spr0.unk44;
    func_02069e68(&w->s4, &w->s5, 8, data_ov000_0236a294);
    w->s4.unk2C |= 0x200;
    w->s5.unk2C |= 0x200;

    func_02069d38(&w->s6, &w->s7, data_ov000_02367090, 8, data_ov000_0236a26c, data_ov000_02369ba0);
    func_02033550(&w->s6, 1);
    func_02033550(&w->s7, 1);
    func_0206990c(&w->s6, &w->s7, 5);
    func_020698bc(&w->s6, &w->s7, 1);
    w->s6.unk44 = w->spr0.unk44;
    w->s7.unk44 = w->spr0.unk44;

    func_0206e3c0(&w->unk44C, -4, -4, 4, 4);
    func_0206e2b4(&w->unk44C, 0, -0x10);
    w->unk474 = 0x12;
    w->unk468 = func_ov000_02317db8;
    w->unk44C.unkA |= 0x800;
    w->unk58 |= 0x2100;
    if (w->unk1E0->unk6 == 0)
        w->unk1E0->unk6 = 9;
    if (w->unk1E0->unk6 <= 2)
        w->unk1E0->unk6 = 3;
    if (w->unk1E0->unk6 >= 0x14)
        w->unk1E0->unk6 = 0x13;
    if (w->unk1E0->unk8 == 0)
        w->unk1E0->unk8 = 0xd0;
    w->unk4A4 = 0x38e38e38;
    func_ov000_02318080(w);
    return w;
}
