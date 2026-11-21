#include <game/Stage/stageobj_alt.h>

typedef struct {
    s16 x;
    s16 y;
} UnkStruct_0231adac_T;

typedef struct {
    u8 pad0[0xC];
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u8 pad18[0x2A4 - 0x18];
    s32 unk2A4;
} UnkStruct_0231adac;

extern UnkStruct_0231adac_T data_ov000_02364444[];

void Valve_SpawnSteamPuff(UnkStruct_0231adac *w, s32 a, s32 x, s32 y);
void Valve_Action_Active(UnkStruct_0231adac *w);


void Valve_State_Puff(UnkStruct_0231adac *w) {
    if (!(Rand() & 0x1f) || w->unkC == 0x3c) {
        s32 i = w->unk2A4;
        s32 x = w->unk10;
        s32 y = w->unk14;
        s32 tx = data_ov000_02364444[i].x; s32 ty = data_ov000_02364444[i].y;
        Valve_SpawnSteamPuff(w, 0xf1, (tx + (x >> 8) + (3 - (Rand() & 6))) << 8, (ty + (y >> 8) + (3 - (Rand() & 6))) << 8);
    }
    w->unkC--;
    if (w->unkC == 0)
        Valve_Action_Active(w);
}
