#include <nitro/types.h>

typedef struct {
    u8 pad0[0x440];
    void *unk440;
    u8 pad444[0x464 - 0x444];
    u16 unk464;
    u8 pad466[2];
    u16 unk468;
} UnkStruct_022e7894;

void Boss7PartD5_HoverBob(UnkStruct_022e7894 *p);
void Boss7PartD5_MoveState_BlazeSpecialSpawnPillar(UnkStruct_022e7894 *p);
void Boss7PartD5_MoveState_BlazeSpecialEnd(UnkStruct_022e7894 *p);

void Boss7PartD5_MoveState_BlazeSpecialHover(UnkStruct_022e7894 *p) {
    Boss7PartD5_HoverBob(p);
    if (p->unk468-- != 0)
        return;
    p->unk464--;
    if (p->unk464 != 0)
        p->unk440 = Boss7PartD5_MoveState_BlazeSpecialSpawnPillar;
    else
        p->unk440 = Boss7PartD5_MoveState_BlazeSpecialEnd;
}
