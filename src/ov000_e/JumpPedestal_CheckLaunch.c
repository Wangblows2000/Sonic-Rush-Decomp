#include <ov000_c/stageobj.h>

typedef struct {
    s16 a;
    s16 b;
    s16 c;
    s16 d;
    s16 e;
    s16 f;
} UnkStruct_Tbl;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x370 - 0x20C];
    s16 unk370;
    u8 pad372[0x37C - 0x372];
    s16 unk37C;
    u8 pad37E[0x422 - 0x37E];
    u16 unk422;
} UnkStruct_Player;

typedef struct {
    u8 pad0[4];
    u16 unk4;
} UnkStruct_Param;

extern UnkStruct_Tbl data_ov000_02364400[];

void Player_Action_PedestalLaunch(UnkStruct_Player *p, s16 a, s16 b);
void Player_SetSourceMapObject(UnkStruct_Player *p, UnkStruct_StageObj *s);
void Player_StartLaunchTrick(UnkStruct_Player *p);

void JumpPedestal_CheckLaunch(UnkStruct_StageObj *s, UnkStruct_Player *p) {
    u8 k;
    s32 px;
    s32 x;
    u32 f;
    s16 spd;
    u32 fl;
    s32 lim;

    fl = s->unk5C;
    spd = p->base.unk34;
    if (fl & 2)
        spd = -p->base.unk34;
    if (s->unk8 != 0) {
        lim = p->unk37C;
        if ((spd >= lim && !(f = fl & 1)) || (spd <= -lim && (f = fl & 1))) {
            x = s->unk10;
            px = p->base.unk10;
            k = ((UnkStruct_Param *)s->unk1E0)->unk4 & 3;
            if ((x + 0x1c00 < px && !f) || (x - 0x1c00 > px && f)) {
                Player_Action_PedestalLaunch(p, p->base.unk34 + data_ov000_02364400[k].a, data_ov000_02364400[k].b - p->unk370);
                Player_SetSourceMapObject(p, s);
            } else if ((x - 0x400 < px && !f) || (x + 0x400 > px && f)) {
                if (p->unk422 & 3) {
                    Player_Action_PedestalLaunch(p, p->base.unk34 + data_ov000_02364400[k].c, data_ov000_02364400[k].d - p->unk370);
                    Player_SetSourceMapObject(p, s);
                    Player_StartLaunchTrick(p);
                }
            }
        }
    }
    s->unk8 = p->base.unk58 & 1;
}
