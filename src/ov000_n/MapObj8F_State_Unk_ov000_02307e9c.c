#include <ov000_c/stageobj.h>

typedef struct {
    u32 unk0;
    u8 pad4[4];
    s32 unk8;
    u8 padC[0x1C - 0xC];
    u8 unk1C;
    u8 unk1D;
    u8 pad1E[0x3A - 0x1E];
    s8 unk3A;
    u8 pad3B;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
    u8 pad4C[0x64 - 0x4C];
} UnkStruct_Plr;

typedef struct {
    s32 x;
    s32 y;
} UnkStruct_Pos;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x214 - 0x20C];
    UnkStruct_Pos unk214[2];
    u32 unk224;
} UnkStruct_02307e9c;

extern UnkStruct_Plr gCamera[];
extern UnkStruct_StageObj *gPlayers[];

void *Gimmick_GetRoomIndex(void);
void *EnemyRoom_Get(s32 a, void *b);
BOOL Gimmick_Unk_ov000_02307dfc(void *a, s32 b);
s32 Stage_GetSplitLineY(s32 x);
void Camera_TrySwitchScreen(void);

void MapObj8F_State_Unk_ov000_02307e9c(UnkStruct_02307e9c *s) {
    void *o;
    UnkStruct_Plr *P;
    UnkStruct_StageObj *q;
    s32 y;
    s32 v;
    u32 f;
    u8 i;
    s32 x;

    o = EnemyRoom_Get(1, Gimmick_GetRoomIndex());
    if (o == NULL) {
        s->base.unk54 |= 4;
        return;
    }
    if (Gimmick_Unk_ov000_02307dfc(o, 0)) {
        s->base.unk54 |= 4;
        return;
    }
    s->base.unkC++;
    P = gCamera;
    if (P[2].unk0 & 1) {
        q = gPlayers[P[0].unk3A];
        y = q->unk14;
        v = Stage_GetSplitLineY(q->unk10);
        f = gCamera[2].unk0;
        if (!(f & 2) && y >= v) {
            P[0].unk8 -= 0x1000;
            Camera_TrySwitchScreen();
        } else if ((f & 2) && y <= v) {
            P[0].unk8 += 0x1000;
            Camera_TrySwitchScreen();
        } else {
            P[0].unk0 &= ~0x200;
            gCamera[2].unk0 &= ~4;
        }
    } else {
        for (i = 0; i < 2; i++) {
            if (s->unk224 & (1 << i)) {
                {UnkStruct_Plr *pp = &P[i];
                x = s->unk214[i].x;
                pp->unk3C = x + (pp->unk44 - x) * s->base.unkC / 20;
                x = s->unk214[i].y;
                pp->unk40 = x + (pp->unk48 - x) * s->base.unkC / 20;}
            }
        }
    }
    if (s->base.unkC < 20)
        return;
    f = gCamera[2].unk0;
    if (f & 1) {
        if (!(gCamera[0].unk0 & 0x800000))
            return;
        gCamera[2].unk0 = f & ~0x100;
        for (i = 0; i < 2; i++) {
            gCamera[i].unk0 &= ~0x500000;
            gCamera[i].unk0 &= ~0xa00000;
            gCamera[i].unk1C = 0xff;
            gCamera[i].unk1D = 0xff;
        }
    } else {
        if (s->unk224 & 1)
            gCamera[0].unk0 &= ~0x2000;
        if (s->unk224 & 2)
            gCamera[1].unk0 &= ~0x2000;
    }
    s->base.unk1F4 |= 0x10;
    s->base.unk54 |= 4;
}
