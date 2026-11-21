#include <game/Stage/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x43C - 0x20C];
    void *unk43C;
    u8 pad440[0x454 - 0x440];
    u16 unk454;
} UnkStruct_022ea584;

void Boss7PartD5_InputState_Decide(void);
void Boss7PartD5_InputState_AirSteer(void);

s32 Boss7PartD5_InputState_HoldJump(UnkStruct_022ea584 *w) {
    if (w->unk454++ > 4) {
        if (w->base.unk58 & 1)
            w->unk43C = Boss7PartD5_InputState_Decide;
        else
            w->unk43C = Boss7PartD5_InputState_AirSteer;
        w->unk454 = (Rand() + 0x14) & 0x1f;
    }
    return 1;
}
