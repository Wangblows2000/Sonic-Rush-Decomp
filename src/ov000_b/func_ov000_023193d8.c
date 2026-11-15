#include <ov000_b/stageobj.h>

extern void *data_ov000_02369ba0;
extern void *data_ov000_0236a2b4;
extern void *data_ov000_0236a2a4;
extern u8 data_ov000_023670c4[], data_ov000_0236a2ac[], data_ov000_023670dc[], data_ov000_023670f4[];

void func_ov000_02319390(void);
void func_020687b8(void **out, void *a, void *b);
void func_ov000_02318cc4(UnkStruct_StageObj *w, s32 a);
void func_ov000_023192cc(UnkStruct_StageObj *w);

UnkStruct_StageObj *func_ov000_023193d8(s32 a, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_StageObj *w;
    vu16 zero;

    task = func_020372d0(func_0206c3d0, func_ov000_02319390, 0, 0x500, 2, sizeof(UnkStruct_StageObj));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_StageObj));
    func_ov000_022fba50(w, a, b, c);
    func_02069f1c(w, data_ov000_023670c4, 0xffff, data_ov000_0236a2ac, data_ov000_02369ba0);
    func_0206a054(w, 0, 0x48);
    w->spr0.unk46 = 0x14;
    w->spr1.unk46 = 0x14;
    if (w->unk1E0->unk2 == 0xbf)
        func_02069818(w, 2);
    if (w->unk1E0->unk2 == 0xbe) {
        func_020687b8(&data_ov000_0236a2b4, data_ov000_023670dc, data_ov000_02369ba0);
        w->unk1C8 = data_ov000_0236a2b4;
        w->unk1B8 = w;
        w->unk1D8 = 4;
        w->unk1DA = 2;
        w->unk1D4 = -0xf;
        w->unk1D6 = -7;
        func_02069818(w, 1);
        if (w->unk1E0->unk4 & 1)
            w->unk5C ^= 2;
        func_ov000_02318cc4(w, 0);
    } else {
        func_020687b8(&data_ov000_0236a2a4, data_ov000_023670f4, data_ov000_02369ba0);
        w->unk1C8 = data_ov000_0236a2a4;
        w->unk1B8 = w;
        w->unk1D8 = 2;
        w->unk1DA = 4;
        w->unk1D4 = -8;
        w->unk1D6 = -0x10;
        func_02069818(w, 3);
        if (w->unk1E0->unk4 & 1)
            w->unk5C ^= 1;
        func_ov000_02318cc4(w, 1);
    }
    w->unk58 |= 0x100;
    func_ov000_023192cc(w);
    return w;
}
