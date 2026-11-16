#include <ov000_c/stageobj.h>

typedef struct {
    u8 unk0;
    u8 pad1[0x28 - 1];
    s16 unk28;
    u8 pad2A[0x54 - 0x2A];
    u32 unk54;
    u32 unk58;
    u8 pad5C[0x328 - 0x5C];
    s16 unk328;
    u8 pad32A[2];
    u32 unk32C;
    u32 unk330;
    u8 pad334[0x37C - 0x334];
    s16 unk37C;
    u8 pad37E[0x410 - 0x37E];
    u8 unk410;
    u8 unk411;
} UnkStruct_Player;

typedef struct {
    u8 pad0[0x18];
    void *unk18;
} UnkStruct_0230a04c_Coll;

typedef struct {
    u8 pad0[0x20];
    u32 unk20;
} UnkStruct_022c4560;

typedef struct {
    u8 pad0[8];
    s32 unk8;
    s32 unkC;
} UnkStruct_022c4590;

extern s16 gSwitchTimers;
extern UnkStruct_022c4560 gGameState;
extern UnkStruct_022c4590 data_022c4590;

void func_ov000_02354098(UnkStruct_Player *pl);
void func_ov000_02328d3c(UnkStruct_Player *pl, s32 a, s32 b);
void Player_Action_Die(UnkStruct_Player *pl);
void func_ov000_0234ed58(UnkStruct_Player *pl, UnkStruct_StageObj *w);
void func_ov000_0234e7b4(UnkStruct_Player *pl, UnkStruct_StageObj *w);
void func_ov000_0233e5c8(s32 a, s32 b);
void func_ov000_0233e524(s32 a, s32 b);

void MapObj40_OnHit(UnkStruct_0230a04c_Coll *a, UnkStruct_0230a04c_Coll *b) {
    UnkStruct_StageObj *w = a->unk18;
    UnkStruct_Player *pl = b->unk18;
    s32 v;

    if (w == NULL)
        return;
    if (pl == NULL)
        return;
    switch (w->unk1E0->unk2) {
    default:
        if (pl->unk0 != 1)
            return;
        if (!(pl->unk58 & 0x10))
            return;
        pl->unk330 |= 1;
        if (w->unk1E0->unk4 & 0x10)
            pl->unk330 |= 2;
        return;
    case 0x41:
        pl->unk54 &= ~1;
        if (pl->unk0 == 1)
            pl->unk411 = 0;
        return;
    case 0x42:
        pl->unk54 |= 1;
        if (pl->unk0 == 1)
            pl->unk411 = 0;
        return;
    case 0xc6:
        if (gSwitchTimers == 0)
            return;
        if (pl->unk0 == 1) {
            pl->unk54 &= ~1;
            pl->unk330 |= 0x10000;
        }
    case 0x59:
        if (pl->unk0 != 1)
            return;
        if ((pl->unk410 & ~0x80) != w->unk1E0->unk4 + 1 && (pl->unk328 < 0x1a || pl->unk328 > 0x20) &&
            !(pl->unk411 & 2)) {
            pl->unk411 = (pl->unk54 & 1) | 2;
            if (w->unk1E0->unk2 == 0xc6)
                pl->unk411 |= 1;
            pl->unk410 = (w->unk1E0->unk4 & 0xf) + 1;
            pl->unk54 &= ~1;
        }
        pl->unk330 |= 4;
        return;
    case 0x5a:
        if (pl->unk0 != 1)
            return;
        if (pl->unk328 < 0x1a)
            return;
        if (pl->unk328 <= 0x20)
            pl->unk54 &= ~1;
        return;
    case 0x5b:
        if (pl->unk0 != 1)
            return;
        if (pl->unk328 < 0x1a)
            return;
        if (pl->unk328 <= 0x20)
            pl->unk54 |= 1;
        return;
    case 0x7e:
        if (pl->unk0 != 1)
            return;
        func_ov000_02354098(pl);
        return;
    case 0xa9:
        if (pl->unk0 != 1)
            return;
        if (gGameState.unk20 & 7)
            return;
        func_ov000_02328d3c(pl, -2, -12);
        return;
    case 0x9d:
        if (pl->unk0 != 1)
            return;
        if (pl->unk32C & 0x80) {
            data_022c4590.unk8 += 10000;
        } else if (pl->unk32C & 0x100) {
            data_022c4590.unk8 += 8000;
        } else {
            v = pl->unk28;
            if (v < 0)
                v = -v;
            if (pl->unk37C >= v)
                data_022c4590.unk8 += 4000;
        }
        data_022c4590.unkC++;
        w->unk54 |= 8;
        w->coll0.unkA |= 0x1000;
        w->unk1F4 |= 0x10;
        return;
    case 0x9f:
        if (pl->unk0 != 1)
            return;
        if (pl->unk58 & 0x100)
            return;
        Player_Action_Die(pl);
        return;
    case 0xb6:
        if (pl->unk0 != 1)
            return;
        func_ov000_0234ed58(pl, w);
        w->unk54 |= 2;
        return;
    case 0xb7:
        if (pl->unk0 != 1)
            return;
        func_ov000_0234e7b4(pl, w);
        return;
    case 0xc3:
        if (pl->unk0 != 1)
            return;
        if (pl->unk330 & 0x8000)
            return;
        func_ov000_0233e5c8(3, 8);
        pl->unk330 |= 0x8000;
        return;
    case 0xc4:
        if (pl->unk0 != 1)
            return;
        if (!(pl->unk330 & 0x8000))
            return;
        if (pl->unk330 & 0x4000) {
            pl->unk330 &= ~0x8000;
            return;
        }
        func_ov000_0233e524(3, 8);
        pl->unk330 &= ~0x8000;
        pl->unk330 &= ~0x4000;
        return;
    case 0xc5:
        if (gSwitchTimers == 0)
            return;
        if (pl->unk0 != 1)
            return;
        pl->unk54 &= ~1;
        pl->unk330 |= 0x2000;
        return;
    }
}
