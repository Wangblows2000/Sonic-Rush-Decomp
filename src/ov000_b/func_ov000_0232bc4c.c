#include <ov000_b/stageobj.h>

extern void *gActComArchive;
extern u8 data_ov000_02367ff8[], data_ov000_0236aa18[], data_ov000_02368010[], data_ov000_02368028[];

void func_ov000_0232bc28(UnkStruct_EffObj *w);

void func_ov000_0232bc4c(s32 type, s32 x, s32 y, s16 vx, s16 vy, s16 c) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == OS_GetArenaLo(0))
        return;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    switch (type) {
    default:
        StageObj_Load2dBAC(w, data_ov000_02367ff8, 9, data_ov000_0236aa18, gActComArchive);
        StageObj_AllocSpritePalette(w, 0, 2);
        StageObj_SetAnimation(w, 6);
        break;
    case 1:
        StageObj_Load2dBAC(w, data_ov000_02368010, 0xa, data_ov000_0236aa18, gActComArchive);
        w->spr0.unk44 = 1;
        w->spr1.unk44 = 1;
        w->spr0.unk2C |= 0x10;
        w->spr1.unk2C |= 0x10;
        StageObj_SetAnimation(w, 8);
        break;
    case 2:
        StageObj_Load2dBAC(w, data_ov000_02368028, 0xc, data_ov000_0236aa18, gActComArchive);
        StageObj_AllocSpritePalette(w, 0, 2);
        StageObj_SetAnimation(w, 0xa);
        break;
    }
    StageObj_SetOAMOrder(w, 9);
    StageObj_SetOAMPriority(w, 0);
    w->unk5C |= 0x80;
    w->unk54 |= 0x10;
    w->unk10 = (x + 0x80) << 8;
    w->unk14 = (y + 0xc0) << 8;
    w->unk58 |= 0x180;
    w->unk54 |= 2;
    w->unk5C |= 4;
    w->unk2E = vx;
    w->unk30 = vy;
    w->unkC = c;
    w->unk40 = 0x54;
    w->unk42 = 0x1e00;
    func_ov000_0232bc28(w);
}
