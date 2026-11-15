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
    u8 pad124[0x19C - 0x124];
} UnkStruct_02324b9c;

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02367748[], data_ov000_0236a708[], data_ov000_0236a810[];
extern u8 data_ov000_02367760[], data_ov000_0236a730[], data_ov000_0236a870[];
extern u8 data_ov000_02367778[], data_ov000_0236a750[], data_ov000_0236a8a0[];
extern u8 data_ov000_02367790[], data_ov000_0236a8b0[];
extern u8 data_ov000_023677a8[], data_ov000_0236a710[], data_ov000_0236a820[];
extern u8 data_ov000_023677c0[], data_ov000_0236a718[], data_ov000_0236a830[];
extern u8 data_ov000_023677d4[], data_ov000_0236a720[], data_ov000_0236a840[];
extern u8 data_ov000_023677ec[], data_ov000_0236a850[];
extern u8 data_ov000_02367804[], data_ov000_0236a728[], data_ov000_0236a860[];
extern u8 data_ov000_02367818[], data_ov000_0236a738[];
extern u8 data_ov000_02367830[], data_ov000_0236a740[], data_ov000_0236a880[];
extern u8 data_ov000_02367848[], data_ov000_0236a890[];
extern u8 data_ov000_02367860[], data_ov000_0236a6f0[];
extern u8 data_ov000_0236a7d0[], data_ov000_0236a7e0[], data_ov000_0236a7f0[], data_ov000_0236a800[];
extern u8 data_ov000_02367878[], data_ov000_0236a6f8[];
extern u8 data_ov000_02367890[], data_ov000_0236a790[];
extern u8 data_ov000_023678a8[], data_ov000_0236a7a0[];
extern u8 data_ov000_023678c0[], data_ov000_0236a7b0[];
extern u8 data_ov000_023678d8[], data_ov000_0236a700[];
extern u8 data_ov000_023678f4[], data_ov000_0236a7c0[];

void func_ov000_023265d8(void);
void func_ov000_02322f68(void);
void func_ov000_02326638(void *w, UnkStruct_ObjParam *p, s32 a, s32 b);
void *func_ov000_02322d7c(void *a, s32 b, s32 c, s32 d, s32 e);

UnkStruct_02324b9c *func_ov000_02324b9c(UnkStruct_ObjParam *p, s32 a, s32 b) {
    UnkStruct_Task *task;
    UnkStruct_02324b9c *w;
    vu16 zero;

    task = func_020372d0(func_0206c3d0, func_ov000_023265d8, 0, 0x500, 4, sizeof(UnkStruct_02324b9c));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_02324b9c));
    func_ov000_02326638(w, p, a, b);
    switch (p->unk2) {
    case 0x54:
    case 0x55:
        func_02069f1c(w, data_ov000_02367748, 0, data_ov000_0236a708, data_ov000_02369ba0);
        func_02069eb0(w, 0xffff, data_ov000_0236a810);
        func_0206a054(w, 0, 0x20);
        if (p->unk2 & 1)
            w->unk5C ^= 1;
        break;
    case 0x5f:
    case 0x60:
        func_02069f1c(w, data_ov000_02367760, 0, data_ov000_0236a730, data_ov000_02369ba0);
        func_02069eb0(w, 0xffff, data_ov000_0236a870);
        func_0206a054(w, 1, 0x34);
        func_02069818(w, 1);
        func_02069918(w, 6);
        func_020698f0(w, 1);
        if (!(p->unk2 & 1))
            w->unk5C ^= 1;
        break;
    case 0x64:
    case 0x65:
        func_02069f1c(w, data_ov000_02367778, 0, data_ov000_0236a750, data_ov000_02369ba0);
        func_02069eb0(w, 0xffff, data_ov000_0236a8a0);
        func_0206a054(w, 0, 0x1e);
        if (p->unk2 & 1)
            w->unk5C ^= 1;
        break;
    case 0x66:
        func_02069f1c(w, data_ov000_02367790, 0, data_ov000_0236a750, data_ov000_02369ba0);
        func_02069eb0(w, 7, data_ov000_0236a8b0);
        func_02069818(w, 1);
        func_0206a054(w, 0, 0x1e);
        func_02069918(w, 0x15);
        break;
    case 0x56:
    case 0x57:
        func_02069f1c(w, data_ov000_023677a8, 0, data_ov000_0236a710, data_ov000_02369ba0);
        func_02069eb0(w, 0xffff, data_ov000_0236a820);
        func_0206a054(w, 0, 0x2f);
        if (p->unk2 & 1)
            w->unk5C ^= 1;
        break;
    case 0x58:
    case 0x59:
        func_02069f1c(w, data_ov000_023677c0, 0, data_ov000_0236a718, data_ov000_02369ba0);
        func_02069eb0(w, 0xffff, data_ov000_0236a830);
        func_0206a054(w, 0, 0x20);
        if (p->unk2 & 1)
            w->unk5C ^= 1;
        break;
    case 0x5a:
    case 0x5b:
        func_02069f1c(w, data_ov000_023677d4, 0, data_ov000_0236a720, data_ov000_02369ba0);
        func_02069eb0(w, 0xffff, data_ov000_0236a840);
        func_0206a054(w, 0, 0x20);
        if (p->unk2 & 1)
            w->unk5C ^= 1;
        break;
    case 0x5c:
    case 0x5d:
        func_02069f1c(w, data_ov000_023677ec, 0, data_ov000_0236a720, data_ov000_02369ba0);
        func_02069eb0(w, 0xffff, data_ov000_0236a850);
        func_0206a054(w, 0, 0x20);
        func_02069818(w, 1);
        if (p->unk2 & 1)
            w->unk5C ^= 1;
        break;
    case 0x5e:
        func_02069f1c(w, data_ov000_02367804, 0, data_ov000_0236a728, data_ov000_02369ba0);
        func_02069eb0(w, 0xffff, data_ov000_0236a860);
        func_0206a054(w, 0, 0x20);
        break;
    case 0x61:
        func_02069f1c(w, data_ov000_02367818, 0xffff, data_ov000_0236a738, data_ov000_02369ba0);
        func_0206a054(w, 0, 0x35);
        w->unk5C |= 4;
        break;
    case 0x62:
        func_02069f1c(w, data_ov000_02367830, 0, data_ov000_0236a740, data_ov000_02369ba0);
        func_02069eb0(w, 0xffff, data_ov000_0236a880);
        func_0206a054(w, 0, 0x20);
        func_02069918(w, 6);
        func_020698f0(w, 0);
        break;
    case 0x63:
        func_02069f1c(w, data_ov000_02367848, 0, data_ov000_0236a740, data_ov000_02369ba0);
        func_02069eb0(w, 0xffff, data_ov000_0236a890);
        func_02069818(w, 1);
        func_0206a054(w, 0, 0x20);
        func_02069918(w, 6);
        func_020698f0(w, 0);
        break;
    case 0x45:
    case 0x46:
    case 0x47:
    case 0x48:
    case 0x49:
        func_02069f1c(w, data_ov000_02367860, 0, data_ov000_0236a6f0, data_ov000_02369ba0);
        switch (p->unk2) {
        case 0x45:
            func_02069eb0(w, 2, data_ov000_0236a7d0);
            func_02069818(w, 0);
            break;
        case 0x49:
            w->unk5C ^= 1;
        case 0x46:
            func_02069eb0(w, 5, data_ov000_0236a7e0);
            func_02069818(w, 2);
            break;
        case 0x47:
            func_02069eb0(w, 2, data_ov000_0236a7f0);
            func_02069818(w, 3);
            break;
        case 0x48:
            func_02069eb0(w, 2, data_ov000_0236a800);
            func_02069818(w, 4);
            break;
        }
        func_0206a054(w, 0, 0x11);
        func_02069918(w, 0x16);
        break;
    case 0x4a:
    case 0x4b:
    case 0x4c:
        func_02069f1c(w, data_ov000_02367878, 0, data_ov000_0236a6f8, data_ov000_02369ba0);
        switch (p->unk2) {
        case 0x4a:
            w->unk88 = func_ov000_02322d7c(data_ov000_02367890, 2, 0, 1, 0);
            func_02069eb0(w, 2, data_ov000_0236a790);
            func_02069818(w, 0);
            break;
        case 0x4b:
            w->unk88 = func_ov000_02322d7c(data_ov000_023678a8, 8, 1, 1, 1);
            func_02069eb0(w, 8, data_ov000_0236a7a0);
            func_02069818(w, 1);
            break;
        case 0x4c:
            w->unk88 = func_ov000_02322d7c(data_ov000_023678c0, 8, 2, 1, 2);
            func_02069eb0(w, 8, data_ov000_0236a7b0);
            func_02069818(w, 2);
            break;
        }
        func_0206a054(w, 1, 0x25);
        func_02069918(w, 0x16);
        w->spr0.unk2C |= 8;
        w->spr1.unk2C |= 8;
        w->unk54 |= 0x200;
        w->unk68 = func_ov000_02322f68;
        break;
    case 0x53:
        func_02069f1c(w, data_ov000_023678d8, 0, data_ov000_0236a700, data_ov000_02369ba0);
        w->unk88 = func_ov000_02322d7c(data_ov000_023678f4, 0xffff, 0, 2, 3);
        func_02069eb0(w, 0xffff, data_ov000_0236a7c0);
        func_0206a054(w, 0, 0x22);
        func_02069918(w, 0x16);
        w->spr0.unk2C |= 8;
        w->spr1.unk2C |= 8;
        w->unk54 |= 0x200;
        w->unk68 = func_ov000_02322f68;
        break;
    }
    if (w->spr0.unk46 == 0)
        func_02069918(w, 0x16);
    w->unk58 |= 0x2100;
    w->unk54 |= 2;
    return w;
}
