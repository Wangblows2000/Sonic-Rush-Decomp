#include <game/Stage/stageobj_alt.h>

typedef struct {
    u8 unk0;
    u8 pad1[0x57];
    u32 unk58;
    u8 pad5C[0x3e8 - 0x5c];
    void *unk3E8;
} UnkPlayer_02312214;

typedef struct {
    u8 pad0[0x18];
    void *unk18;
} UnkColl_02312214;

void Player_Action_PipeExit(UnkPlayer_02312214 *p, s32 v);
void Player_Action_Pipe(UnkPlayer_02312214 *p, UnkStruct_StageObj *o, u8 dir);

void GmkPipe_OnHit(UnkColl_02312214 *a, UnkColl_02312214 *b) {
    UnkStruct_StageObj *o = a->unk18;
    UnkPlayer_02312214 *p = b->unk18;
    u8 dir = 0xc0;
    s16 v = 0;
    u16 f;

    if (o == NULL)
        return;
    if (p == NULL && p->unk0 != 1)
        return;
    f = o->unk1E0->unk4;
    if ((f & 4) || (p->unk3E8 != o && (p->unk58 & 0x200))) {
        if (o->unk1E0->unk6 != 0)
            v = o->unk1E0->unk6 * 0x180 + 0x780;
        Player_Action_PipeExit(p, v);
        return;
    }
    if (f & 1)
        dir += 0x40;
    if (f & 2)
        dir += 0x80;
    Player_Action_Pipe(p, o, dir);
    o->unkC = 0x60;
    o->unk54 |= 2;
}
