#include <ov000_b/stageobj.h>

typedef struct {
    s16 x;
    s16 y;
    s16 rangeX;
    s16 rangeY;
} UnkStruct_Valve_T;

typedef struct {
    u8 pad0[0xC];
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u8 pad18[0x2A4 - 0x18];
    s32 unk2A4;
} UnkStruct_Valve;

extern UnkStruct_Valve_T data_ov000_02364474[];

void Valve_DrawSprites(UnkStruct_Valve *w);
void Valve_SpawnSteamPuff(UnkStruct_Valve *w, s32 a, s32 x, s32 y);
void Valve_Action_End(UnkStruct_Valve *w);

void Valve_State_Active(UnkStruct_Valve *w) {
    Valve_DrawSprites(w);
    if (!(Rand() & 0xf)) {
        int i = w->unk2A4;
        s32 x = w->unk10 >> 8;
        s32 y = w->unk14 >> 8;
        Valve_SpawnSteamPuff(w, 0xf1, (x + data_ov000_02364474[i].x + Rand() % data_ov000_02364474[i].rangeX) << 8, (y + data_ov000_02364474[i].y + Rand() % data_ov000_02364474[i].rangeY) << 8);
    }
    w->unkC--;
    if (w->unkC == 0)
        Valve_Action_End(w);
}
