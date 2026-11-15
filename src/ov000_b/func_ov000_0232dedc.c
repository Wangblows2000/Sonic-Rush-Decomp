#include <ov000_b/stageobj.h>

extern void *data_022c448c;
extern u8 data_ov000_02368100[], data_ov000_0236aa00[];
extern u8 data_ov000_0236aa38[], data_ov000_0236aa48[], data_ov000_0236aa58[], data_ov000_0236aa68[];

void func_020693f0(s16 *x, s16 *y, u8 a);
void func_ov000_0232dec0(UnkStruct_EffObj *w);

UnkStruct_EffObj *func_ov000_0232dedc(UnkStruct_StageObj *arg, s16 x, s16 y, s16 vx, s16 vy, u8 type) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;
    BOOL flag = FALSE;

    task = func_020372d0(func_0206c3d0, func_0206bef4, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    func_02069f1c(w, data_ov000_02368100, 0, data_ov000_0236aa00, data_022c448c);
    func_0206a054(w, 0, 0xc);
    if (type >= 4) {
        if (type != 8)
            flag = TRUE;
        if (type <= 7)
            type -= 4;
        else
            type = Rand() & 3;
    }
    switch (type) {
    case 0:
        func_02069eb0(w, 2, data_ov000_0236aa38);
        func_02069818(w, type);
        break;
    case 1:
        func_02069eb0(w, 2, data_ov000_0236aa48);
        func_02069818(w, type);
        break;
    case 2:
        func_02069eb0(w, 2, data_ov000_0236aa58);
        func_02069818(w, type);
        break;
    case 3:
        func_02069eb0(w, 2, data_ov000_0236aa68);
        func_02069818(w, type);
        break;
    }
    func_02069918(w, 9);
    if (arg->unk5C & 1)
        w->unk5C |= 1;
    w->unk10 = arg->unk10;
    w->unk14 = arg->unk14;
    w->unk5 = arg->unk5;
    func_020693f0(&x, &y, arg->unk5);
    if (flag) {
        w->unk2E = vx;
        w->unk30 = vy;
        if (!(w->unk5C & 1)) {
            w->unk10 += x << 8;
        } else {
            w->unk10 -= x << 8;
            w->unk2E = -w->unk2E;
        }
        w->unk14 += y << 8;
        w->unk2E += (s16)(0x400 - (Rand() & 0x7ff));
        w->unk30 += (s16)-(Rand() & 0x3ff);
        w->unk10 += (s16)(0x1000 - (Rand() & 0x1fff));
        w->unk14 += (s16)(0x1000 - (Rand() & 0x1fff));
    } else {
        w->unk2E = vx;
        w->unk30 = vy;
        if (!(w->unk5C & 1)) {
            w->unk10 += x << 8;
        } else {
            w->unk10 -= x << 8;
            w->unk2E = -w->unk2E;
        }
        w->unk14 += y << 8;
    }
    w->unk40 = 0x2a;
    w->unk42 = 0xf00;
    if (arg->unk54 & 1)
        w->unk54 |= 1;
    func_02068e40(w, -2, -4, 2, 4);
    w->unk58 |= 0x280;
    w->unk54 |= 2;
    w->unk5C |= 0x100;
    func_ov000_0232dec0(w);
    return w;
}
