#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x54];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x68 - 0x5E];
    void *unk68;
    u8 pad6C[0x88 - 0x6C];
    void *unk88;
    u8 pad8C[0x94 - 0x8C];
    UnkStruct_Spr spr0;
    UnkStruct_Spr spr1;
    u8 pad124[0x134 - 0x124];
    UnkStruct_Coll coll0;
    u8 pad160[0x184 - 0x160];
    UnkStruct_ObjParam *unk184;
    u8 pad188[0x19C - 0x188];
} UnkStruct_023254dc;

extern void *data_ov000_02369ba0;
extern void *data_ov000_0236ac60;
extern u8 data_ov000_02367910[], data_ov000_0236a6f0[];
extern u8 data_ov000_0236792c[], data_ov000_0236a6f8[];
extern u8 data_ov000_02367948[], data_ov000_0236a7a0[], data_ov000_02367964[], data_ov000_0236a7b0[];
extern u8 data_ov000_02367980[], data_ov000_0236799c[], data_ov000_0236a7c0[];
extern u8 data_ov000_023679b8[], data_ov000_0236a700[], data_ov000_0236a7d0[];
extern u8 data_ov000_023679d0[], data_ov000_0236a7e0[];
extern u8 data_ov000_023679e8[], data_ov000_0236a708[], data_ov000_02367a04[], data_ov000_02367a20[];
extern u8 data_ov000_02367a3c[], data_ov000_0236a718[], data_ov000_0236a810[];
extern u8 data_ov000_02367a54[], data_ov000_0236a720[], data_ov000_0236a820[];
extern u8 data_ov000_02367a6c[], data_ov000_0236a728[];

void func_ov000_023265d8(void);
void func_ov000_02322f68(void);
void func_ov000_02322f80(void);
void func_ov000_0232112c(void);
void func_ov000_02326638(void *w, UnkStruct_ObjParam *p, s32 a, s32 b);
void *func_ov000_02322d7c(void *a, s32 b, s32 c, s32 d, s32 e);
void func_ov000_02320fec(UnkStruct_023254dc *w, s32 a, s32 b);
void func_ov000_02321430(UnkStruct_023254dc *w);
void func_ov000_02321824(UnkStruct_023254dc *w);

UnkStruct_023254dc *func_ov000_023254dc(UnkStruct_ObjParam *p, s32 a, s32 b) {
    UnkStruct_Task *task;
    UnkStruct_023254dc *w;
    vu16 zero;

    task = func_020372d0(func_0206c3d0, func_ov000_023265d8, 0, 0x500, 4, sizeof(UnkStruct_023254dc));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_023254dc));
    func_ov000_02326638(w, p, a, b);
    switch (p->unk2) {
    case 0x41:
        func_02069f1c(w, data_ov000_02367910, 0xffff, data_ov000_0236a6f0, data_ov000_02369ba0);
        func_0206a054(w, 0, 0x19);
        w->unk5C |= 4;
        func_02069918(w, 0x16);
        break;
    case 0x42:
    case 0x43:
    case 0x44:
    case 0xf1:
        func_02069f1c(w, data_ov000_0236792c, 0, data_ov000_0236a6f8, data_ov000_02369ba0);
        switch (p->unk2) {
        case 0x42:
            w->unk88 = func_ov000_02322d7c(data_ov000_02367948, 8, (u16)(p->unk2 - 0x42), 1, 1);
            func_02069eb0(w, 8, data_ov000_0236a7a0);
            break;
        case 0x43:
            w->unk88 = func_ov000_02322d7c(data_ov000_02367964, 0x10, (u16)(p->unk2 - 0x42), 1, 2);
            func_02069eb0(w, 0x10, data_ov000_0236a7b0);
            break;
        case 0xf1:
            w->unk5C ^= 1;
        case 0x44:
            if (p->unk2 == 0xf1)
                w->unk88 = func_ov000_02322d7c(data_ov000_02367980, 3, 2, 1, 3);
            else
                w->unk88 = func_ov000_02322d7c(data_ov000_0236799c, 3, (u16)(p->unk2 - 0x42), 1, 3);
            func_02069eb0(w, 3, data_ov000_0236a7c0);
            break;
        }
        func_0206a054(w, 0, 0x19);
        if (p->unk2 == 0xf1)
            func_02069818(w, 2);
        else
            func_02069818(w, (u16)(p->unk2 - 0x42));
        w->unk5C |= 4;
        func_02069918(w, 6);
        func_020698f0(w, 1);
        w->spr0.unk2C |= 8;
        w->spr1.unk2C |= 8;
        w->unk54 |= 0x200;
        w->unk68 = func_ov000_02322f68;
        break;
    case 0x4d:
        func_02069f1c(w, data_ov000_023679b8, 0, data_ov000_0236a700, data_ov000_02369ba0);
        func_02069eb0(w, 0x10, data_ov000_0236a7d0);
        func_02069818(w, 0);
        func_0206a054(w, 0, 0x26);
        func_02069918(w, 0x16);
        break;
    case 0x4e:
        func_02069f1c(w, data_ov000_023679d0, 0, data_ov000_0236a700, data_ov000_02369ba0);
        func_02069eb0(w, 8, data_ov000_0236a7e0);
        func_02069818(w, 1);
        func_0206a054(w, 0, 0x26);
        func_02069918(w, 0x16);
        w->unk88 = data_ov000_0236ac60;
        w->unk54 |= 0x200;
        w->unk68 = func_ov000_02322f80;
        break;
    case 0x4f:
        func_02069f1c(w, data_ov000_023679e8, 0x14, data_ov000_0236a708, data_ov000_02369ba0);
        func_02069818(w, 3);
        func_0206a054(w, 0, 7);
        w->unk5C |= 4;
        func_02069918(w, 5);
        func_020698f0(w, 1);
        break;
    case 0x50:
        func_02069f1c(w, data_ov000_02367a04, 0x10, data_ov000_0236a708, data_ov000_02369ba0);
        func_02069818(w, 4);
        func_0206a054(w, 0, 7);
        w->unk5C |= 4;
        func_020698f0(w, 1);
        func_02069918(w, 5);
        break;
    case 0x90:
        func_02069f1c(w, data_ov000_02367a20, 5, data_ov000_0236a708, data_ov000_02369ba0);
        func_02069818(w, 6);
        func_0206a054(w, 0, 7);
        w->unk5C |= 4;
        func_020698f0(w, 1);
        func_02069918(w, 5);
        break;
    case 0x9d:
    case 0x9e:
    case 0x9f:
    case 0xa0:
        func_02069f1c(w, data_ov000_02367a3c, 0, data_ov000_0236a718, data_ov000_02369ba0);
        func_02069eb0(w, 0xffff, data_ov000_0236a810);
        func_0206a054(w, 0, 0x54);
        if (w->unk184->unk2 < 0x9f) {
            func_02069918(w, 0x16);
        } else {
            func_020698f0(w, 1);
            func_02069918(w, 5);
        }
        if (!(w->unk184->unk2 & 1))
            w->unk5C ^= 1;
        break;
    case 0xa1:
    case 0xa2:
    case 0xa3:
    case 0xa4:
        func_02069f1c(w, data_ov000_02367a54, 0, data_ov000_0236a720, data_ov000_02369ba0);
        func_02069eb0(w, 0xffff, data_ov000_0236a820);
        func_0206a054(w, 0, 0x53);
        if (w->unk184->unk2 < 0xa3) {
            func_020698f0(w, 3);
            func_02069918(w, 0x16);
        } else {
            func_020698f0(w, 1);
            func_02069918(w, 5);
        }
        if (!(w->unk184->unk2 & 1))
            w->unk5C ^= 1;
        break;
    case 0xa5:
    case 0xa6:
    case 0xa7:
    case 0xa8:
    case 0xa9:
    case 0xaa:
    case 0xab:
    case 0xac:
    case 0xad:
    case 0xae:
        func_02069f1c(w, data_ov000_02367a6c, 0xffff, data_ov000_0236a728, data_ov000_02369ba0);
        func_0206a054(w, 0, 0x54);
        func_020698f0(w, 3);
        func_02069918(w, 0x16);
        if (w->unk184->unk2 >= 0xab) {
            func_02069918(w, 0x18);
            func_02069818(w, 4);
        } else if (w->unk184->unk2 >= 0xa8) {
            func_02069818(w, 2);
        }
        if (w->unk184->unk2 >= 0xad) {
            w->unk5C |= 0x20;
            func_ov000_02320fec(w, 4, 4);
            func_ov000_02320fec(w, -8, 2);
            func_ov000_02320fec(w, 8, 0x10);
            func_ov000_02320fec(w, 2, -8);
            func_ov000_02320fec(w, -0xc, 6);
            func_ov000_02320fec(w, 8, 8);
            func_ov000_02320fec(w, -6, -4);
            func_ov000_02320fec(w, -2, -10);
        }
        if (w->unk184->unk2 == 0xa7 || w->unk184->unk2 == 0xaa || w->unk184->unk2 == 0xac)
            func_ov000_02321430(w);
        else
            func_ov000_02321824(w);
        w->unk54 |= 2;
        if (w->unk184->unk2 == 0xa6 || w->unk184->unk2 == 0xa9 || w->unk184->unk2 == 0xae) {
            func_0206e3c0(&w->coll0, -0x20, -0x20, 0x20, 0x20);
            func_0206e2b4(&w->coll0, 0, -0x10);
            w->coll0.unkA |= 0xc0;
            w->coll0.unk1C = func_ov000_0232112c;
            w->coll0.unk18 = w;
            w->unk54 &= ~2;
        }
        return w;
    }
    if (w->spr0.unk46 == 0)
        func_02069918(w, 0x16);
    w->unk58 |= 0x2100;
    w->unk54 |= 2;
    return w;
}
