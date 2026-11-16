#include <nitro/types.h>

typedef struct {
    u8 pad0[0x5c];
    u16 unk5C;
    u8 pad5E[0x3d8 - 0x5e];
    u16 unk3D8;
    u8 pad3DA[2];
    u16 unk3DC;
    u8 pad3DE[0x440 - 0x3de];
    void *unk440;
} UnkStruct_022e9170;

void Boss7PartD5_MoveState_StartIdle(UnkStruct_022e9170 *p);
void Boss7PartD5_MoveState_StartSpindashCharge(UnkStruct_022e9170 *p);

void Boss7PartD5_MoveState_Crouch(UnkStruct_022e9170 *p) {
    if (p->unk3D8 & 0x20)
        p->unk5C |= 1;
    else if (p->unk3D8 & 0x10)
        p->unk5C &= ~1;
    if (!(p->unk3D8 & 0x80)) {
        p->unk440 = Boss7PartD5_MoveState_StartIdle;
        return;
    }
    if (p->unk3DC & 1)
        p->unk440 = Boss7PartD5_MoveState_StartSpindashCharge;
}
