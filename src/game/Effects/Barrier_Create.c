#include <game/Effects/blkobj.h>

extern u8 data_ov000_0236a998[];
extern u8 data_ov000_023683a4[];
extern u8 data_ov000_023683bc[];
extern u8 data_ov000_023683d4[];
extern u8 data_ov000_023683ec[];
extern u8 data_ov000_02368404[];
extern u8 data_ov000_0236841c[];

void Barrier_State_Main(void);

UnkStruct_BlkObj *Barrier_Create(UnkStruct_BlkObjArg *arg) {
    UnkStruct_Task *task;
    UnkStruct_BlkObj *w;
    vu16 zero;

    if (Player_IsPlayer2(arg))
        return NULL;
    if (arg->unk0 != 1)
        return NULL;
    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_BlkObj));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_BlkObj));
    StageObj_LoadModelSimple(w, &w->blk[0], data_ov000_023683a4, 0, 3, 0, data_ov000_0236a998, gActComArchive);
    StageObj_LoadModelSimple(w, &w->blk[1], data_ov000_023683bc, 0, 4, 4, data_ov000_0236a998, gActComArchive);
    StageObj_LoadModelSimple(w, &w->blk[2], data_ov000_023683d4, 0, 5, 5, data_ov000_0236a998, gActComArchive);
    StageObj_LoadModelSimple(w, &w->blk[3], data_ov000_023683ec, 0, 2, 1, data_ov000_0236a998, gActComArchive);
    StageObj_LoadModelSimple(w, &w->blk[4], data_ov000_02368404, 0, 0, 2, data_ov000_0236a998, gActComArchive);
    StageObj_LoadModelSimple(w, &w->blk[5], data_ov000_0236841c, 0, 1, 3, data_ov000_0236a998, gActComArchive);
    w->blk[3].unkA8 |= 0x40;
    w->blk[4].unkA8 |= 0x40;
    w->blk[5].unkA8 |= 0x40;
    BlkObj_SetVec(&w->unk128->unk18, 0x34cc, 0x34cc, 0x34cc);
    w->unk10 = arg->unk10;
    w->unk14 = arg->unk14;
    w->unk88 = arg;
    w->unk58 |= 0x2100;
    w->unk54 |= 0x12;
    w->unk68 = Barrier_State_Main;
    return w;
}
