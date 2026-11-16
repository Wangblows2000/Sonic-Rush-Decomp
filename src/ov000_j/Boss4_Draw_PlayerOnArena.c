#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    u8 unk10[0x5c - 0x10];
    u16 unk5C;
    u8 pad5E[0x158 - 0x5e];
    u8 unk158[0x24];
    u8 unk17C[0x328 - 0x17c];
    s16 unk328;
} UnkStruct_022de768;

typedef struct {
    u8 pad0[4];
    s32 unk4;
} UnkStruct_02369ac0;

typedef struct {
    u8 pad0[0xd6];
    u16 unkD6;
} UnkStruct_0236a444;

extern UnkStruct_022de768 *gPlayers;
extern UnkStruct_02369ac0 gBossStatus;
extern UnkStruct_0236a444 data_ov000_0236a444;
extern s16 data_0206e470[];

void Boss4_WrapArenaX(void *a);
void Boss4_ObjArenaPosToWorld(void *a, u16 *b, void *c);
void MTX_RotY33_(void *m, s32 s, s32 c);
void Boss4_SetCameraDefaultDistance(void);

void Boss4_Draw_PlayerOnArena(void) {
    u16 ang[4];
    UnkStruct_022de768 *p;
    s32 st;
    s32 off;

    p = gPlayers;
    if (p->unk5C & 0x20)
        return;
    Boss4_WrapArenaX(p->unk10);
    Boss4_ObjArenaPosToWorld(p->unk17C, ang, p);
    if (p->unk5C & 1)
        off = 0xc000;
    else
        off = 0x4000;
    MTX_RotY33_(p->unk158, data_0206e470[(((u16)(off + ang[0]) & 0xffff) >> 4) * 2], data_0206e470[(((u16)(off + ang[0]) & 0xffff) >> 4) * 2 + 1]);
    st = gBossStatus.unk4;
    if (st == 0x1a)
        return;
    data_ov000_0236a444.unkD6 = ang[0];
    if (st == 0x18)
        return;
    if (p->unk328 == 0x30)
        return;
    Boss4_SetCameraDefaultDistance();
}
