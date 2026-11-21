#include <game/Stage/stageobj.h>

typedef struct {
    u8 pad[0x20C];
    u32 unk20C;
    u8 pad210[0x216 - 0x210];
    s16 unk216;
    s16 unk218;
    s16 unk21A;
    s16 unk21C;
} UnkStruct_02308810_T;

void *Gimmick_GetRoomIndex(void);
UnkStruct_02308810_T *EnemyRoom_Get(s32 a, void *b);
BOOL Gimmick_CheckRoomResetOnDeath(void *a, s32 b);
void MapObj8F_Action_RoomBoundDone(UnkStruct_StageObj *s);

void MapObj8F_State_SetRoomBound(UnkStruct_StageObj *s) {
    UnkStruct_02308810_T *t = EnemyRoom_Get(1, Gimmick_GetRoomIndex());

    if (Gimmick_CheckRoomResetOnDeath(t, 0)) {
        s->unk1F4 &= ~0x10;
        s->unk54 |= 4;
        return;
    }
    if (t == NULL)
        return;
    if (!(s->unk1E0->unk4 & 2)) {
        t->unk216 = s->unk10 >> 8;
        t->unk218 = s->unk14 >> 8;
    } else {
        t->unk21A = s->unk10 >> 8;
        t->unk21C = s->unk14 >> 8;
    }
    if (t->unk20C & 8)
        MapObj8F_Action_RoomBoundDone(s);
}
