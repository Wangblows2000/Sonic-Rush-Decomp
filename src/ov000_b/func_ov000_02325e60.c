#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x54];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x94 - 0x5E];
    UnkStruct_Spr spr0;
    UnkStruct_Spr spr1;
    u8 pad124[0x19C - 0x124];
} UnkStruct_02325e60;

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02367a84[], data_ov000_0236a6f0[];
extern u8 data_ov000_0236a790[], data_ov000_0236a7a0[], data_ov000_0236a7b0[], data_ov000_0236a7c0[], data_ov000_0236a7d0[];
extern u8 data_ov000_02367a9c[], data_ov000_0236a6f8[];
extern u8 data_ov000_0236a7e0[], data_ov000_0236a7f0[], data_ov000_0236a800[], data_ov000_0236a810[], data_ov000_0236a820[], data_ov000_0236a830[];
extern u8 data_ov000_02367ab4[], data_ov000_0236a710[];
extern u8 data_ov000_02367ad0[], data_ov000_0236a8e0[], data_ov000_0236a8f0[], data_ov000_0236a900[];

void func_ov000_023265d8(void);
void func_ov000_02326638(void *w, UnkStruct_ObjParam *p, s32 a, s32 b);

UnkStruct_02325e60 *func_ov000_02325e60(UnkStruct_ObjParam *p, s32 a, s32 b) {
    UnkStruct_Task *task;
    UnkStruct_02325e60 *w;
    vu16 zero;
    u16 idx;
    s32 t;

    task = func_020372d0(func_0206c3d0, func_ov000_023265d8, 0, 0x500, 4, sizeof(UnkStruct_02325e60));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_02325e60));
    func_ov000_02326638(w, p, a, b);
    idx = p->unk2 - 0xf;
    if (p->unk2 >= 0xf && p->unk2 <= 0x18) {
        if (p->unk2 >= 0x14)
            idx -= 5;
        func_02069f1c(w, data_ov000_02367a84, 0, data_ov000_0236a6f0, data_ov000_02369ba0);
        func_0206a054(w, 0, 0x16);
        switch (p->unk2) {
        default:
        case 0xf:
        case 0x14:
            func_02069eb0(w, 0x20, data_ov000_0236a790);
            break;
        case 0x10:
        case 0x15:
            func_02069eb0(w, 6, data_ov000_0236a7a0);
            break;
        case 0x11:
        case 0x16:
            func_02069eb0(w, 2, data_ov000_0236a7b0);
            break;
        case 0x12:
        case 0x17:
            func_02069eb0(w, 1, data_ov000_0236a7c0);
            break;
        case 0x13:
        case 0x18:
            func_02069eb0(w, 1, data_ov000_0236a7d0);
            break;
        }
        func_02069818(w, idx);
        func_02069918(w, 6);
        func_020698f0(w, 0);
        if (p->unk2 >= 0x14)
            w->unk5C &= ~1;
    } else if (p->unk2 >= 0x19 && p->unk2 <= 0x34) {
        func_02069f1c(w, data_ov000_02367a9c, 0, data_ov000_0236a6f8, data_ov000_02369ba0);
        func_0206a054(w, 0, 5);
        switch (p->unk2) {
        default:
        case 0x19:
        case 0x1a:
        case 0x1b:
        case 0x1c:
            func_02069eb0(w, 0x2a, data_ov000_0236a7e0);
            func_02069818(w, 0);
            break;
        case 0x1d:
        case 0x1e:
        case 0x1f:
        case 0x20:
            func_02069eb0(w, 0x14, data_ov000_0236a7f0);
            func_02069818(w, 3);
            break;
        case 0x21:
        case 0x22:
        case 0x23:
        case 0x24:
            func_02069eb0(w, 9, data_ov000_0236a800);
            func_02069818(w, 2);
            break;
        case 0x25:
        case 0x26:
        case 0x27:
        case 0x28:
            func_02069eb0(w, 0x34, data_ov000_0236a810);
            func_02069818(w, 1);
            break;
        case 0x29:
        case 0x2a:
        case 0x2b:
        case 0x2c:
            func_02069eb0(w, 0x14, data_ov000_0236a820);
            func_02069818(w, 5);
            break;
        case 0x2d:
        case 0x2e:
        case 0x2f:
        case 0x30:
            func_02069eb0(w, 0xc, data_ov000_0236a830);
            func_02069818(w, 4);
            break;
        }
        if (p->unk2 & 1)
            w->unk5C |= 1;
        else
            w->unk5C &= ~1;
        t = p->unk2 & 3;
        if (t == 0 || t == 3) {
            func_02069918(w, 6);
            func_020698f0(w, 1);
        }
    } else if (p->unk2 == 0x3a) {
        func_02069f1c(w, data_ov000_02367ab4, 4, data_ov000_0236a710, data_ov000_02369ba0);
        func_0206a054(w, 0, 5);
        func_02069918(w, 0x16);
    } else if (p->unk2 >= 0x3b && p->unk2 <= 0x40) {
        func_02069f1c(w, data_ov000_02367ad0, 0, data_ov000_0236a6f8, data_ov000_02369ba0);
        func_0206a054(w, 8, 6);
        switch (p->unk2) {
        default:
        case 0x3c:
            w->unk5C ^= 1;
        case 0x3b:
            func_02069eb0(w, 0x10, data_ov000_0236a8e0);
            func_02069818(w, 8);
            break;
        case 0x3e:
            w->unk5C ^= 1;
        case 0x3d:
            func_02069eb0(w, 4, data_ov000_0236a8f0);
            func_02069818(w, 9);
            break;
        case 0x40:
            w->unk5C ^= 1;
        case 0x3f:
            func_02069eb0(w, 8, data_ov000_0236a900);
            func_02069818(w, 0xa);
            break;
        }
        func_020698f0(w, 0);
    }
    if (w->spr0.unk46 == 0)
        func_02069918(w, 0x16);
    w->unk58 |= 0x2100;
    w->unk54 |= 2;
    return w;
}
