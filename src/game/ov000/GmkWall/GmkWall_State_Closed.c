#include <nitro/types.h>

typedef struct UnkStruct_02308b88_2 {
    u8 pad[0x20c];
    u32 unk20C;
} UnkStruct_02308b88_2;

typedef struct UnkStruct_02308b88 {
    u8 pad[0x54];
    u32 unk54;
    u8 pad1[0x1f4 - 0x58];
    u32 unk1F4;
} UnkStruct_02308b88;

u8 Gimmick_GetRoomIndex(UnkStruct_02308b88 *s);
UnkStruct_02308b88_2 *EnemyRoom_Get(s32 a, s32 i);
BOOL Gimmick_CheckRoomResetOnDeath(UnkStruct_02308b88_2 *p, s32 a);
void GmkWall_Action_MoveUp(UnkStruct_02308b88 *s);

void GmkWall_State_Closed(UnkStruct_02308b88 *s) {
    UnkStruct_02308b88_2 *p = EnemyRoom_Get(1, Gimmick_GetRoomIndex(s));
    if (Gimmick_CheckRoomResetOnDeath(p, 0)) {
        s->unk1F4 &= ~0x10;
        s->unk54 |= 4;
        return;
    }
    if (p == NULL) {
        s->unk54 |= 4;
        return;
    }
    if (!(p->unk20C & 0x40))
        return;
    GmkWall_Action_MoveUp(s);
}
