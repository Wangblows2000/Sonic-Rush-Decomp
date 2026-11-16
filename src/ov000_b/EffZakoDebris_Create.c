#include <ov000_b/stageobj.h>

extern void *gActComArchive;
extern u8 data_ov000_02368100[], data_ov000_0236aa00[];
extern u8 data_ov000_0236aa38[], data_ov000_0236aa48[], data_ov000_0236aa58[], data_ov000_0236aa68[];

void Math_RotateVec2S16(s16 *x, s16 *y, u8 a);
void func_ov000_0232dec0(UnkStruct_EffObj *w);

UnkStruct_EffObj *EffZakoDebris_Create(UnkStruct_StageObj *arg, s16 x, s16 y, s16 vx, s16 vy, u8 type) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;
    BOOL flag = FALSE;

    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    StageObj_Load2dBAC(w, data_ov000_02368100, 0, data_ov000_0236aa00, gActComArchive);
    StageObj_AllocSpritePalette(w, 0, 0xc);
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
        StageObj_SetSharedSpriteVram(w, 2, data_ov000_0236aa38);
        StageObj_SetAnimation(w, type);
        break;
    case 1:
        StageObj_SetSharedSpriteVram(w, 2, data_ov000_0236aa48);
        StageObj_SetAnimation(w, type);
        break;
    case 2:
        StageObj_SetSharedSpriteVram(w, 2, data_ov000_0236aa58);
        StageObj_SetAnimation(w, type);
        break;
    case 3:
        StageObj_SetSharedSpriteVram(w, 2, data_ov000_0236aa68);
        StageObj_SetAnimation(w, type);
        break;
    }
    StageObj_SetOAMOrder(w, 9);
    if (arg->unk5C & 1)
        w->unk5C |= 1;
    w->unk10 = arg->unk10;
    w->unk14 = arg->unk14;
    w->unk5 = arg->unk5;
    Math_RotateVec2S16(&x, &y, arg->unk5);
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
