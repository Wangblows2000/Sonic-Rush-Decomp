#include <ov000_b/stageobj.h>
#include <ov000_b/res.h>

typedef struct {
    u8 pad0[0x10];
} UnkStruct_0236a790;

typedef struct {
    u8 pad0[0x54];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x94 - 0x5E];
    UnkStruct_Spr spr0;
    UnkStruct_Spr spr1;
    u8 pad124[0x19C - 0x124];
} UnkStruct_023240f8;

extern void *data_ov000_02369ba0;
extern UnkStruct_Res *data_ov000_0236a6f0[];
extern UnkStruct_0236a790 data_ov000_0236a790[];
extern u8 data_ov000_023675e0[], data_ov000_023675f8[], data_ov000_0236a6f8[], data_ov000_0236a840[];

void func_ov000_023265d8(void);
s32 func_02002094(s32 a, s32 b);
void func_ov000_02326638(void *w, UnkStruct_ObjParam *p, s32 a, s32 b);

UnkStruct_023240f8 *func_ov000_023240f8(UnkStruct_ObjParam *p, s32 a, s32 b) {
    UnkStruct_Task *task;
    UnkStruct_023240f8 *w;
    vu16 zero;
    s32 i;

    task = func_020372d0(func_0206c3d0, func_ov000_023265d8, 0, 0x500, 4, sizeof(UnkStruct_023240f8));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_023240f8));
    func_ov000_02326638(w, p, a, b);
    switch (p->unk2) {
    default:
        func_02069f1c(w, data_ov000_023675e0, 0, data_ov000_0236a6f0, data_ov000_02369ba0);
        func_0206a054(w, 0, 0x39);
        if (p->unk2 < 0x83) {
            i = p->unk2 - 0x67;
            i -= func_02002094(i, 7) * 7;
            func_02069eb0(w, Res_GetElemVal(data_ov000_0236a6f0[0], i), &data_ov000_0236a790[i]);
            func_02069818(w, (u16)i);
            if (p->unk2 >= 0x6e)
                w->unk5C ^= 1;
            if (p->unk2 >= 0x75)
                w->unk5C ^= 2;
            if (p->unk2 >= 0x75 && p->unk2 <= 0x7b)
                w->unk5C ^= 1;
        } else {
            i = p->unk2 - 0x83;
            func_02069eb0(w, Res_GetElemVal(data_ov000_0236a6f0[0], i + 7), &data_ov000_0236a790[i + 7]);
            func_02069818(w, (u16)(i + 7));
        }
        if (p->unk2 == 0x84 || p->unk2 == 0x86)
            func_02069918(w, 5);
        else
            func_02069918(w, 6);
        func_020698f0(w, 0);
        break;
    case 0x91:
    case 0x92:
    case 0x93:
    case 0x94:
        func_02069f1c(w, data_ov000_023675f8, 0, data_ov000_0236a6f8, data_ov000_02369ba0);
        func_02069eb0(w, 0xffff, data_ov000_0236a840);
        if (p->unk2 < 0x93)
            func_0206a054(w, 0, 0x3e);
        else
            func_0206a054(w, 1, 0x3f);
        if (!(p->unk2 & 1))
            w->unk5C ^= 1;
        func_02069918(w, 0x16);
        break;
    }
    if (w->spr0.unk46 == 0)
        func_02069918(w, 0x16);
    w->unk58 |= 0x2100;
    w->unk54 |= 2;
    return w;
}
