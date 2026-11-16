#include <ov000_b/stageobj.h>

typedef struct {
    u8 unk0;
    u8 pad1[0x13];
    s32 unk14;
    u32 unk18;
} UnkPlayer_023168a4;

typedef struct {
    u8 pad0[0x18];
    void *unk18;
} UnkColl_023168a4;

typedef struct {
    u8 pad0[0x1c];
    u8 unk1C;
} UnkStruct_022c4560;

extern UnkStruct_022c4560 data_022c4560;

void Spring_Action_Activate(UnkStruct_StageObj *o);
void Player_Action_SpringLaunchAlt(UnkPlayer_023168a4 *p);
void Player_Action_SpringLaunch(UnkPlayer_023168a4 *p, s32 vx, s32 vy);
void Player_SetSourceMapObject(UnkPlayer_023168a4 *p, UnkStruct_StageObj *o);

void Spring_OnHit(UnkColl_023168a4 *a, UnkColl_023168a4 *b) {
    UnkStruct_StageObj *o = a->unk18;
    UnkPlayer_023168a4 *p = b->unk18;
    UnkStruct_ObjParam *prm;
    u16 id;
    s16 vx;
    s16 vy;

    if (o == NULL)
        return;
    if (p == NULL && p->unk0 != 1)
        return;
    if ((p->unk18 & ~0x3ff) != (o->unk18 & ~0x3ff))
        return;
    Spring_Action_Activate(o);
    prm = o->unk1E0;
    id = prm->unk2;
    vx = -0x780;
    vx -= prm->unk6 * 0x180;
    vy = vx;
    if (id <= 0x44 || id > 0x4c)
        vx = 0;
    else if (id <= 0x46)
        vy = 0;
    if (id >= 0x47 && id <= 0x4c) {
        vx = (vx * 0xb5) >> 8;
        vy = (vy * 0xb5) >> 8;
    }
    if (o->unk5C & 2)
        vy = -vy;
    if (o->unk5C & 1)
        vx = -vx;
    if ((id == 0x45 || id == 0x46) && !(prm->unk4 & 2))
        p->unk14 = o->unk14 + 0x200;
    if (data_022c4560.unk1C == 3)
        Player_Action_SpringLaunchAlt(p);
    else
        Player_Action_SpringLaunch(p, vx, vy);
    Player_SetSourceMapObject(p, o);
}
