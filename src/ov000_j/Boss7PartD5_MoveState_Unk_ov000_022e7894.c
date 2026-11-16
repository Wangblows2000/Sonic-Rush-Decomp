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
void Boss7PartD5_MoveState_Unk_ov000_022e78f0(UnkStruct_022e7894 *p);
void Boss7PartD5_MoveState_Unk_ov000_022e779c(UnkStruct_022e7894 *p);

void Boss7PartD5_MoveState_Unk_ov000_022e7894(UnkStruct_022e7894 *p) {
    Boss7PartD5_HoverBob(p);
    if (p->unk468-- != 0)
        return;
    p->unk464--;
    if (p->unk464 != 0)
        p->unk440 = Boss7PartD5_MoveState_Unk_ov000_022e78f0;
    else
        p->unk440 = Boss7PartD5_MoveState_Unk_ov000_022e779c;
}
