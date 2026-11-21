#include <game/Stage/stageobj_alt.h>

typedef struct {
    u8 pad0[0x54];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x68 - 0x5E];
    void *unk68;
    u8 pad6C[0x88 - 0x6C];
    void *unk88;
    u8 pad8C[0x94 - 0x8C];
    UnkStruct_Spr spr0;
    UnkStruct_Spr spr1;
    u8 pad124[0x134 - 0x124];
    UnkStruct_Coll coll0;
    u8 pad160[0x184 - 0x160];
    UnkStruct_ObjParam *unk184;
    u8 pad188[0x19C - 0x188];
} UnkStruct_023254dc;

extern void *data_ov000_02369ba0;
extern void *gPlayers;
extern u8 data_ov000_02367910[], data_ov000_0236a6f0[];
extern u8 data_ov000_0236792c[], data_ov000_0236a6f8[];
extern u8 data_ov000_02367948[], data_ov000_0236a7a0[], data_ov000_02367964[], data_ov000_0236a7b0[];
extern u8 data_ov000_02367980[], data_ov000_0236799c[], data_ov000_0236a7c0[];
extern u8 data_ov000_023679b8[], data_ov000_0236a700[], data_ov000_0236a7d0[];
extern u8 data_ov000_023679d0[], data_ov000_0236a7e0[];
extern u8 data_ov000_023679e8[], data_ov000_0236a708[], data_ov000_02367a04[], data_ov000_02367a20[];
extern u8 data_ov000_02367a3c[], data_ov000_0236a718[], data_ov000_0236a810[];
extern u8 data_ov000_02367a54[], data_ov000_0236a720[], data_ov000_0236a820[];
extern u8 data_ov000_02367a6c[], data_ov000_0236a728[];

void Decoration_Destructor(void);
void DecShared_State_PingParent(void);
void DecGroup41_State_FollowParentPriority(void);
void DecGroup41_OnHit(void);
void Decoration_InitFromMap(void *w, UnkStruct_ObjParam *p, s32 a, s32 b);
void *DecShared_Create(void *a, s32 b, s32 c, s32 d, s32 e);
void DecFishPart_Create(UnkStruct_023254dc *w, s32 a, s32 b);
void DecGroup41_Action_Cruise(UnkStruct_023254dc *w);
void DecGroup41_Action_Sink(UnkStruct_023254dc *w);

UnkStruct_023254dc *DecGroup41_Create(UnkStruct_ObjParam *p, s32 a, s32 b) {
    UnkStruct_Task *task;
    UnkStruct_023254dc *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, Decoration_Destructor, 0, 0x500, 4, sizeof(UnkStruct_023254dc));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_023254dc));
    Decoration_InitFromMap(w, p, a, b);
    switch (p->unk2) {
    case 0x41:
        StageObj_Load2dBAC(w, data_ov000_02367910, 0xffff, data_ov000_0236a6f0, data_ov000_02369ba0);
        StageObj_AllocSpritePalette(w, 0, 0x19);
        w->unk5C |= 4;
        StageObj_SetOAMOrder(w, 0x16);
        break;
    case 0x42:
    case 0x43:
    case 0x44:
    case 0xf1:
        StageObj_Load2dBAC(w, data_ov000_0236792c, 0, data_ov000_0236a6f8, data_ov000_02369ba0);
        switch (p->unk2) {
        case 0x42:
            w->unk88 = DecShared_Create(data_ov000_02367948, 8, (u16)(p->unk2 - 0x42), 1, 1);
            StageObj_SetSharedSpriteVram(w, 8, data_ov000_0236a7a0);
            break;
        case 0x43:
            w->unk88 = DecShared_Create(data_ov000_02367964, 0x10, (u16)(p->unk2 - 0x42), 1, 2);
            StageObj_SetSharedSpriteVram(w, 0x10, data_ov000_0236a7b0);
            break;
        case 0xf1:
            w->unk5C ^= 1;
        case 0x44:
            if (p->unk2 == 0xf1)
                w->unk88 = DecShared_Create(data_ov000_02367980, 3, 2, 1, 3);
            else
                w->unk88 = DecShared_Create(data_ov000_0236799c, 3, (u16)(p->unk2 - 0x42), 1, 3);
            StageObj_SetSharedSpriteVram(w, 3, data_ov000_0236a7c0);
            break;
        }
        StageObj_AllocSpritePalette(w, 0, 0x19);
        if (p->unk2 == 0xf1)
            StageObj_SetAnimation(w, 2);
        else
            StageObj_SetAnimation(w, (u16)(p->unk2 - 0x42));
        w->unk5C |= 4;
        StageObj_SetOAMOrder(w, 6);
        StageObj_SetOAMPriority(w, 1);
        w->spr0.unk2C |= 8;
        w->spr1.unk2C |= 8;
        w->unk54 |= 0x200;
        w->unk68 = DecShared_State_PingParent;
        break;
    case 0x4d:
        StageObj_Load2dBAC(w, data_ov000_023679b8, 0, data_ov000_0236a700, data_ov000_02369ba0);
        StageObj_SetSharedSpriteVram(w, 0x10, data_ov000_0236a7d0);
        StageObj_SetAnimation(w, 0);
        StageObj_AllocSpritePalette(w, 0, 0x26);
        StageObj_SetOAMOrder(w, 0x16);
        break;
    case 0x4e:
        StageObj_Load2dBAC(w, data_ov000_023679d0, 0, data_ov000_0236a700, data_ov000_02369ba0);
        StageObj_SetSharedSpriteVram(w, 8, data_ov000_0236a7e0);
        StageObj_SetAnimation(w, 1);
        StageObj_AllocSpritePalette(w, 0, 0x26);
        StageObj_SetOAMOrder(w, 0x16);
        w->unk88 = gPlayers;
        w->unk54 |= 0x200;
        w->unk68 = DecGroup41_State_FollowParentPriority;
        break;
    case 0x4f:
        StageObj_Load2dBAC(w, data_ov000_023679e8, 0x14, data_ov000_0236a708, data_ov000_02369ba0);
        StageObj_SetAnimation(w, 3);
        StageObj_AllocSpritePalette(w, 0, 7);
        w->unk5C |= 4;
        StageObj_SetOAMOrder(w, 5);
        StageObj_SetOAMPriority(w, 1);
        break;
    case 0x50:
        StageObj_Load2dBAC(w, data_ov000_02367a04, 0x10, data_ov000_0236a708, data_ov000_02369ba0);
        StageObj_SetAnimation(w, 4);
        StageObj_AllocSpritePalette(w, 0, 7);
        w->unk5C |= 4;
        StageObj_SetOAMPriority(w, 1);
        StageObj_SetOAMOrder(w, 5);
        break;
    case 0x90:
        StageObj_Load2dBAC(w, data_ov000_02367a20, 5, data_ov000_0236a708, data_ov000_02369ba0);
        StageObj_SetAnimation(w, 6);
        StageObj_AllocSpritePalette(w, 0, 7);
        w->unk5C |= 4;
        StageObj_SetOAMPriority(w, 1);
        StageObj_SetOAMOrder(w, 5);
        break;
    case 0x9d:
    case 0x9e:
    case 0x9f:
    case 0xa0:
        StageObj_Load2dBAC(w, data_ov000_02367a3c, 0, data_ov000_0236a718, data_ov000_02369ba0);
        StageObj_SetSharedSpriteVram(w, 0xffff, data_ov000_0236a810);
        StageObj_AllocSpritePalette(w, 0, 0x54);
        if (w->unk184->unk2 < 0x9f) {
            StageObj_SetOAMOrder(w, 0x16);
        } else {
            StageObj_SetOAMPriority(w, 1);
            StageObj_SetOAMOrder(w, 5);
        }
        if (!(w->unk184->unk2 & 1))
            w->unk5C ^= 1;
        break;
    case 0xa1:
    case 0xa2:
    case 0xa3:
    case 0xa4:
        StageObj_Load2dBAC(w, data_ov000_02367a54, 0, data_ov000_0236a720, data_ov000_02369ba0);
        StageObj_SetSharedSpriteVram(w, 0xffff, data_ov000_0236a820);
        StageObj_AllocSpritePalette(w, 0, 0x53);
        if (w->unk184->unk2 < 0xa3) {
            StageObj_SetOAMPriority(w, 3);
            StageObj_SetOAMOrder(w, 0x16);
        } else {
            StageObj_SetOAMPriority(w, 1);
            StageObj_SetOAMOrder(w, 5);
        }
        if (!(w->unk184->unk2 & 1))
            w->unk5C ^= 1;
        break;
    case 0xa5:
    case 0xa6:
    case 0xa7:
    case 0xa8:
    case 0xa9:
    case 0xaa:
    case 0xab:
    case 0xac:
    case 0xad:
    case 0xae:
        StageObj_Load2dBAC(w, data_ov000_02367a6c, 0xffff, data_ov000_0236a728, data_ov000_02369ba0);
        StageObj_AllocSpritePalette(w, 0, 0x54);
        StageObj_SetOAMPriority(w, 3);
        StageObj_SetOAMOrder(w, 0x16);
        if (w->unk184->unk2 >= 0xab) {
            StageObj_SetOAMOrder(w, 0x18);
            StageObj_SetAnimation(w, 4);
        } else if (w->unk184->unk2 >= 0xa8) {
            StageObj_SetAnimation(w, 2);
        }
        if (w->unk184->unk2 >= 0xad) {
            w->unk5C |= 0x20;
            DecFishPart_Create(w, 4, 4);
            DecFishPart_Create(w, -8, 2);
            DecFishPart_Create(w, 8, 0x10);
            DecFishPart_Create(w, 2, -8);
            DecFishPart_Create(w, -0xc, 6);
            DecFishPart_Create(w, 8, 8);
            DecFishPart_Create(w, -6, -4);
            DecFishPart_Create(w, -2, -10);
        }
        if (w->unk184->unk2 == 0xa7 || w->unk184->unk2 == 0xaa || w->unk184->unk2 == 0xac)
            DecGroup41_Action_Cruise(w);
        else
            DecGroup41_Action_Sink(w);
        w->unk54 |= 2;
        if (w->unk184->unk2 == 0xa6 || w->unk184->unk2 == 0xa9 || w->unk184->unk2 == 0xae) {
            ObjRect_SetBox2D(&w->coll0, -0x20, -0x20, 0x20, 0x20);
            ObjRect_SetAtkDefFlags(&w->coll0, 0, -0x10);
            w->coll0.unkA |= 0xc0;
            w->coll0.unk1C = DecGroup41_OnHit;
            w->coll0.unk18 = w;
            w->unk54 &= ~2;
        }
        return w;
    }
    if (w->spr0.unk46 == 0)
        StageObj_SetOAMOrder(w, 0x16);
    w->unk58 |= 0x2100;
    w->unk54 |= 2;
    return w;
}
