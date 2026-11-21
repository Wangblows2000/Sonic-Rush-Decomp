#include <game/Stage/stageobj_alt.h>

typedef struct {
    UnkStruct_StageObj base;
    s8 unk20C;
    s8 unk20D;
} UnkWork_0231b9f0;

typedef struct {
    u8 pad0[0x3a];
    s8 unk3A;
    u8 pad3B[0x9e - 0x3b];
    s8 unk9E;
    u8 pad9F[0xc8 - 0x9f];
    u32 unkC8;
} UnkStruct_0236aad8;

extern UnkStruct_0236aad8 gCamera;

void WaterLevelTrigger_State_SetBySide(void);

void WaterLevelTrigger_Action_InitBySide(UnkWork_0231b9f0 *w) {
    if (gCamera.unkC8 & 1) {
        w->unk20C = gCamera.unk3A;
        w->unk20D = gCamera.unk3A;
    } else {
        w->unk20C = gCamera.unk3A;
        w->unk20D = gCamera.unk9E;
    }
    w->base.unk68 = WaterLevelTrigger_State_SetBySide;
}
