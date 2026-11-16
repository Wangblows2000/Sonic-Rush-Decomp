#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x54];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x94 - 0x5E];
    UnkStruct_Spr spr0;
    u8 padDC[0x19C - 0xDC];
} UnkStruct_023234b8;

extern void *data_ov000_02369ba0;
extern void *data_ov000_0236a6f0[];
extern u8 data_ov000_02367450[], data_ov000_0236a790[], data_ov000_0236a7a0[], data_ov000_0236a7b0[];
extern u8 data_ov000_0236457c[];

void func_ov000_023265d8(void);
void func_ov000_02326638(void *w, void *p, s32 a, s32 b);
void func_0204423c(u16 a, void *b, void *c, s32 d);

UnkStruct_023234b8 *func_ov000_023234b8(UnkStruct_ObjParam *p, s32 a, s32 b) {
    UnkStruct_Task *task;
    UnkStruct_023234b8 *w;
    vu16 zero;

    task = func_020372d0(func_0206c3d0, func_ov000_023265d8, 0, 0x500, 4, sizeof(UnkStruct_023234b8));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_023234b8));
    func_ov000_02326638(w, p, a, b);
    func_02069f1c(w, data_ov000_02367450, 0, data_ov000_0236a6f0, data_ov000_02369ba0);
    func_0206a054(w, 0, 0x3a);
    if (p->unk2 <= 0x89) {
        func_02069eb0(w, 0xffff, data_ov000_0236a790);
        func_02069818(w, 0);
    } else if (p->unk2 <= 0x8b) {
        func_02069eb0(w, 0xffff, data_ov000_0236a7a0);
        func_02069818(w, 1);
    } else {
        func_02069eb0(w, 0xffff, data_ov000_0236a7b0);
        func_02069818(w, 2);
    }
    if (p->unk2 == 0x89 || p->unk2 == 0x8d || p->unk2 == 0x8f)
        w->unk5C ^= 1;
    if (p->unk2 == 0x8b || p->unk2 >= 0x8e)
        w->unk5C ^= 2;
    func_02069918(w, 0x16);
    func_0204423c(w->spr0.unk44, data_ov000_0236457c, data_ov000_0236a6f0[0], 0);
    w->unk54 |= 0x800;
    if (w->spr0.unk46 == 0)
        func_02069918(w, 0x16);
    w->unk58 |= 0x2100;
    w->unk54 |= 2;
    return w;
}
