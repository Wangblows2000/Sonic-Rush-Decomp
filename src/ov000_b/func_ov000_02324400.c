#include <ov000_b/stageobj.h>
#include <ov000_b/res.h>

typedef struct {
    u8 pad0[0x10];
} UnkStruct_0236a790;

typedef struct {
    u8 pad0[8];
    s32 unk8;
    u8 padC[0x54 - 0xC];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x68 - 0x5E];
    void *unk68;
    u8 pad6C[0x94 - 0x6C];
    UnkStruct_Spr spr0;
    UnkStruct_Spr spr1;
    u8 pad124[0x134 - 0x124];
    UnkStruct_Coll coll0;
    u8 pad160[0x19C - 0x160];
} UnkStruct_02324400;

extern void *data_ov000_02369ba0;
extern UnkStruct_Res *data_ov000_0236a6f0[];
extern UnkStruct_0236a790 data_ov000_0236a790[];
extern u8 data_ov000_02367610[];
extern u8 data_ov000_0236762c[], data_ov000_0236a728[], data_ov000_0236a8d0[];
extern u8 data_ov000_02367648[], data_ov000_0236a718[], data_ov000_0236a8b0[];
extern u8 data_ov000_02367668[], data_ov000_0236a8c0[];
extern u8 data_ov000_02367688[], data_ov000_0236a6f8[], data_ov000_0236a840[], data_ov000_02364568[];
extern u8 data_ov000_023676a4[], data_ov000_0236a700[], data_ov000_0236a850[], data_ov000_0236454c[];
extern u8 data_ov000_023676c0[], data_ov000_0236a720[], data_ov000_0236a880[];
extern u8 data_ov000_023676d8[], data_ov000_0236a730[];
extern u8 data_ov000_023676f4[], data_ov000_0236a738[], data_ov000_0236a8f0[], data_ov000_0236a8e0[], data_ov000_02364558[];
extern u8 data_ov000_02367710[], data_ov000_0236a740[];
extern u8 data_ov000_0236772c[];

void func_ov000_023265d8(void);
void func_ov000_02322f3c(void);
void func_ov000_02320894(void);
void func_ov000_023208a4(void);
void func_ov000_02320014(void);
void func_ov000_02320168(void);
void func_ov000_02326638(void *w, UnkStruct_ObjParam *p, s32 a, s32 b);
void func_0204423c(s32 a, void *b, void *c, s32 d);
void func_ov000_02320744(UnkStruct_02324400 *w);
void func_ov000_023205bc(UnkStruct_02324400 *w);

UnkStruct_02324400 *func_ov000_02324400(UnkStruct_ObjParam *p, s32 a, s32 b) {
    UnkStruct_Task *task;
    UnkStruct_02324400 *w;
    vu16 zero;
    s32 idx = 0;

    task = func_020372d0(func_0206c3d0, func_ov000_023265d8, 0, 0x500, 4, sizeof(UnkStruct_02324400));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_02324400));
    func_ov000_02326638(w, p, a, b);
    switch (p->unk2) {
    default:
        func_02069f1c(w, data_ov000_02367610, 0, data_ov000_0236a6f0, data_ov000_02369ba0);
        if (p->unk2 < 0xb3) {
            if (p->unk2 == 0xb1)
                idx = 1;
            else if (p->unk2 == 0xb2)
                idx = 2;
            else if (!(p->unk2 & 1))
                w->unk5C ^= 1;
        } else {
            idx = ((p->unk2 - 0xb3) >> 1) + 3;
            if (!(p->unk2 & 1))
                w->unk5C ^= 1;
        }
        func_02069eb0(w, Res_GetElemVal(data_ov000_0236a6f0[0], idx), &data_ov000_0236a790[idx]);
        func_02069818(w, (u16)idx);
        func_0206a054(w, 0, 0x55);
        func_02069918(w, 6);
        func_020698f0(w, 0);
        w->unk68 = func_ov000_02322f3c;
        break;
    case 0xe4:
        w->unk5C ^= 1;
    case 0xe3:
        func_02069f1c(w, data_ov000_0236762c, 0, data_ov000_0236a728, data_ov000_02369ba0);
        func_02069eb0(w, 0xffff, data_ov000_0236a8d0);
        func_0206a054(w, 0, 0x55);
        func_02069918(w, 6);
        func_020698f0(w, 0);
        w->unk68 = func_ov000_02322f3c;
        break;
    case 0xe2:
        w->unk5C ^= 2;
    case 0xd9:
        func_02069f1c(w, data_ov000_02367648, 0, data_ov000_0236a718, data_ov000_02369ba0);
        func_02069eb0(w, 0x12, data_ov000_0236a8b0);
        func_0206a054(w, 0, 0x5d);
        break;
    case 0xdb:
        w->unk5C ^= 1;
    case 0xda:
        func_02069f1c(w, data_ov000_02367668, 0, data_ov000_0236a718, data_ov000_02369ba0);
        func_02069eb0(w, 0xa, data_ov000_0236a8c0);
        func_0206a054(w, 0, 0x5d);
        func_02069818(w, 1);
        break;
    case 0xc3:
        func_02069f1c(w, data_ov000_02367688, 0, data_ov000_0236a6f8, data_ov000_02369ba0);
        func_02069eb0(w, 0xffff, data_ov000_0236a840);
        func_0206a054(w, 0, 0x57);
        func_02069918(w, 6);
        func_020698f0(w, 0);
        func_0204423c(w->spr0.unk44, data_ov000_02364568, data_ov000_0236a6f0[2], 0);
        w->unk54 |= 0x800;
        break;
    case 0xc4:
        func_02069f1c(w, data_ov000_023676a4, 0, data_ov000_0236a700, data_ov000_02369ba0);
        func_02069eb0(w, 0xffff, data_ov000_0236a850);
        func_0206a054(w, 0, 0x58);
        func_02069918(w, 6);
        func_020698f0(w, 0);
        func_0204423c(w->spr0.unk44, data_ov000_0236454c, data_ov000_0236a6f0[4], 0);
        w->unk54 |= 0x800;
        break;
    case 0xd8:
        func_02069f1c(w, data_ov000_023676c0, 0, data_ov000_0236a720, data_ov000_02369ba0);
        func_02069eb0(w, 0x31, data_ov000_0236a880);
        func_0206a054(w, 0, 0x5c);
        func_02069818(w, 0);
        func_ov000_02320744(w);
        func_ov000_023205bc(w);
        func_02069918(w, 0x16);
        func_0206e3c0(&w->coll0, -0x30, -8, 0x30, 0x40);
        func_0206e2b4(&w->coll0, 0, -0x10);
        w->coll0.unkA |= 0xc0;
        w->coll0.unk1C = func_ov000_02320894;
        w->coll0.unk18 = w;
        w->unk54 &= ~2;
        w->unk58 |= 0x2100;
        w->unk68 = func_ov000_023208a4;
        return w;
    case 0xe8:
        func_02069f1c(w, data_ov000_023676d8, 0xffff, data_ov000_0236a730, data_ov000_02369ba0);
        func_0206a054(w, 0, 0x5f);
        func_02069918(w, 0x16);
        w->unk5C |= 4;
        break;
    case 0xe9:
    case 0xea:
        func_02069f1c(w, data_ov000_023676f4, idx, data_ov000_0236a738, data_ov000_02369ba0);
        if (p->unk2 == 0xea) {
            func_02069eb0(w, 5, data_ov000_0236a8f0);
            func_02069818(w, 3);
        } else {
            func_02069eb0(w, 0xffff, data_ov000_0236a8e0);
        }
        func_0206a054(w, 0, 0x60);
        func_02069918(w, 0x16);
        func_0204423c(w->spr0.unk44, data_ov000_02364558, data_ov000_0236a6f0[0x12], 0);
        w->unk54 |= 0x800;
        break;
    case 0xeb:
        func_02069f1c(w, data_ov000_02367710, 0xffff, data_ov000_0236a740, data_ov000_02369ba0);
        func_0206a054(w, 0xc, 0x61);
        func_02069818(w, 0xc);
        break;
    case 0xed:
        func_02069f1c(w, data_ov000_0236772c, 0xffff, data_ov000_0236a740, data_ov000_02369ba0);
        func_0206a054(w, 0xd, 0x62);
        func_02069818(w, 0x19);
        break;
    case 0xec:
    case 0xee:
        w->unk68 = func_ov000_02320014;
        w->unk5C |= 0x20;
        break;
    case 0xef:
        w->unk8 = 0xffff;
        w->unk68 = func_ov000_02320168;
        w->unk5C |= 0x20;
        break;
    }
    if (w->spr0.unk46 == 0)
        func_02069918(w, 0x16);
    w->unk58 |= 0x2100;
    w->unk54 |= 2;
    return w;
}
