#include <ov000_b/stageobj.h>

void MapObject_Destructor(void);
void StageEvent_SpawnInRect(u16 a, u16 b, u16 c, u16 d);

UnkStruct_StageObj *MapObjC2_Create(s32 a, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_StageObj *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, MapObject_Destructor, 0, 0xf6, 2, sizeof(UnkStruct_StageObj));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_StageObj));
    MapObject_Init(w, a, b, c);
    w->unk58 |= 0x2100;
    w->unk58 |= 0x100;
    w->unk54 |= 0x20;
    StageEvent_SpawnInRect(w->unk1E8 - 0x80 + (w->unk1E0->unk6 << 8), w->unk1EC - 0x120 + w->unk1E0->unk7 * 0xc0,
                        w->unk1E8 + 0x80 + (w->unk1E0->unk6 << 8), w->unk1EC + 0x120 + w->unk1E0->unk7 * 0xc0);
    return w;
}
