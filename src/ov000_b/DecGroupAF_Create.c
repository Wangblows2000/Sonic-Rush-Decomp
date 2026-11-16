#include <ov000_b/stageobj.h>
#include <ov000_b/res.h>

typedef struct {
    u8 pad0[0x10];
} UnkStruct_0236a790;

typedef struct {
    u8 pad0[8];
    s32 unk8;
    u8 padC[0x54 - 0xC];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x68 - 0x5E];
    void *unk68;
    u8 pad6C[0x94 - 0x6C];
    UnkStruct_Spr spr0;
    UnkStruct_Spr spr1;
    u8 pad124[0x134 - 0x124];
    UnkStruct_Coll coll0;
    u8 pad160[0x19C - 0x160];
} UnkStruct_02324400;

extern void *data_ov000_02369ba0;
extern UnkStruct_Res *data_ov000_0236a6f0[];
extern UnkStruct_0236a790 data_ov000_0236a790[];
extern u8 data_ov000_02367610[];
extern u8 data_ov000_0236762c[], data_ov000_0236a728[], data_ov000_0236a8d0[];
extern u8 data_ov000_02367648[], data_ov000_0236a718[], data_ov000_0236a8b0[];
extern u8 data_ov000_02367668[], data_ov000_0236a8c0[];
extern u8 data_ov000_02367688[], data_ov000_0236a6f8[], data_ov000_0236a840[], data_ov000_02364568[];
extern u8 data_ov000_023676a4[], data_ov000_0236a700[], data_ov000_0236a850[], data_ov000_0236454c[];
extern u8 data_ov000_023676c0[], data_ov000_0236a720[], data_ov000_0236a880[];
extern u8 data_ov000_023676d8[], data_ov000_0236a730[];
extern u8 data_ov000_023676f4[], data_ov000_0236a738[], data_ov000_0236a8f0[], data_ov000_0236a8e0[], data_ov000_02364558[];
extern u8 data_ov000_02367710[], data_ov000_0236a740[];
extern u8 data_ov000_0236772c[];

void Decoration_Destructor(void);
void DecGroupAF_State_SwitchVisible(void);
void DecGroupAF_OnHit(void);
void DecGroupAF_State_ClearTouched(void);
void DecSonicNeon_State_BlinkPair(void);
void DecSonicNeon_State_BlinkFull(void);
void Decoration_InitFromMap(void *w, UnkStruct_ObjParam *p, s32 a, s32 b);
void PaletteAnim_Create(s32 a, void *b, void *c, s32 d);
void DecGroupAF_CreateChildBack(UnkStruct_02324400 *w);
void DecGroupAF_CreateChildFront(UnkStruct_02324400 *w);

UnkStruct_02324400 *DecGroupAF_Create(UnkStruct_ObjParam *p, s32 a, s32 b) {
    UnkStruct_Task *task;
    UnkStruct_02324400 *w;
    vu16 zero;
    s32 idx = 0;

    task = Task_Create(StageObj_Main, Decoration_Destructor, 0, 0x500, 4, sizeof(UnkStruct_02324400));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_02324400));
    Decoration_InitFromMap(w, p, a, b);
    switch (p->unk2) {
    default:
        StageObj_Load2dBAC(w, data_ov000_02367610, 0, data_ov000_0236a6f0, data_ov000_02369ba0);
        if (p->unk2 < 0xb3) {
            if (p->unk2 == 0xb1)
                idx = 1;
            else if (p->unk2 == 0xb2)
                idx = 2;
            else if (!(p->unk2 & 1))
                w->unk5C ^= 1;
        } else {
            idx = ((p->unk2 - 0xb3) >> 1) + 3;
            if (!(p->unk2 & 1))
                w->unk5C ^= 1;
        }
        StageObj_SetSharedSpriteVram(w, Res_GetElemVal(data_ov000_0236a6f0[0], idx), &data_ov000_0236a790[idx]);
        StageObj_SetAnimation(w, (u16)idx);
        StageObj_AllocSpritePalette(w, 0, 0x55);
        StageObj_SetOAMOrder(w, 6);
        StageObj_SetOAMPriority(w, 0);
        w->unk68 = DecGroupAF_State_SwitchVisible;
        break;
    case 0xe4:
        w->unk5C ^= 1;
    case 0xe3:
        StageObj_Load2dBAC(w, data_ov000_0236762c, 0, data_ov000_0236a728, data_ov000_02369ba0);
        StageObj_SetSharedSpriteVram(w, 0xffff, data_ov000_0236a8d0);
        StageObj_AllocSpritePalette(w, 0, 0x55);
        StageObj_SetOAMOrder(w, 6);
        StageObj_SetOAMPriority(w, 0);
        w->unk68 = DecGroupAF_State_SwitchVisible;
        break;
    case 0xe2:
        w->unk5C ^= 2;
    case 0xd9:
        StageObj_Load2dBAC(w, data_ov000_02367648, 0, data_ov000_0236a718, data_ov000_02369ba0);
        StageObj_SetSharedSpriteVram(w, 0x12, data_ov000_0236a8b0);
        StageObj_AllocSpritePalette(w, 0, 0x5d);
        break;
    case 0xdb:
        w->unk5C ^= 1;
    case 0xda:
        StageObj_Load2dBAC(w, data_ov000_02367668, 0, data_ov000_0236a718, data_ov000_02369ba0);
        StageObj_SetSharedSpriteVram(w, 0xa, data_ov000_0236a8c0);
        StageObj_AllocSpritePalette(w, 0, 0x5d);
        StageObj_SetAnimation(w, 1);
        break;
    case 0xc3:
        StageObj_Load2dBAC(w, data_ov000_02367688, 0, data_ov000_0236a6f8, data_ov000_02369ba0);
        StageObj_SetSharedSpriteVram(w, 0xffff, data_ov000_0236a840);
        StageObj_AllocSpritePalette(w, 0, 0x57);
        StageObj_SetOAMOrder(w, 6);
        StageObj_SetOAMPriority(w, 0);
        PaletteAnim_Create(w->spr0.unk44, data_ov000_02364568, data_ov000_0236a6f0[2], 0);
        w->unk54 |= 0x800;
        break;
    case 0xc4:
        StageObj_Load2dBAC(w, data_ov000_023676a4, 0, data_ov000_0236a700, data_ov000_02369ba0);
        StageObj_SetSharedSpriteVram(w, 0xffff, data_ov000_0236a850);
        StageObj_AllocSpritePalette(w, 0, 0x58);
        StageObj_SetOAMOrder(w, 6);
        StageObj_SetOAMPriority(w, 0);
        PaletteAnim_Create(w->spr0.unk44, data_ov000_0236454c, data_ov000_0236a6f0[4], 0);
        w->unk54 |= 0x800;
        break;
    case 0xd8:
        StageObj_Load2dBAC(w, data_ov000_023676c0, 0, data_ov000_0236a720, data_ov000_02369ba0);
        StageObj_SetSharedSpriteVram(w, 0x31, data_ov000_0236a880);
        StageObj_AllocSpritePalette(w, 0, 0x5c);
        StageObj_SetAnimation(w, 0);
        DecGroupAF_CreateChildBack(w);
        DecGroupAF_CreateChildFront(w);
        StageObj_SetOAMOrder(w, 0x16);
        ObjRect_SetBox2D(&w->coll0, -0x30, -8, 0x30, 0x40);
        ObjRect_SetAtkDefFlags(&w->coll0, 0, -0x10);
        w->coll0.unkA |= 0xc0;
        w->coll0.unk1C = DecGroupAF_OnHit;
        w->coll0.unk18 = w;
        w->unk54 &= ~2;
        w->unk58 |= 0x2100;
        w->unk68 = DecGroupAF_State_ClearTouched;
        return w;
    case 0xe8:
        StageObj_Load2dBAC(w, data_ov000_023676d8, 0xffff, data_ov000_0236a730, data_ov000_02369ba0);
        StageObj_AllocSpritePalette(w, 0, 0x5f);
        StageObj_SetOAMOrder(w, 0x16);
        w->unk5C |= 4;
        break;
    case 0xe9:
    case 0xea:
        StageObj_Load2dBAC(w, data_ov000_023676f4, idx, data_ov000_0236a738, data_ov000_02369ba0);
        if (p->unk2 == 0xea) {
            StageObj_SetSharedSpriteVram(w, 5, data_ov000_0236a8f0);
            StageObj_SetAnimation(w, 3);
        } else {
            StageObj_SetSharedSpriteVram(w, 0xffff, data_ov000_0236a8e0);
        }
        StageObj_AllocSpritePalette(w, 0, 0x60);
        StageObj_SetOAMOrder(w, 0x16);
        PaletteAnim_Create(w->spr0.unk44, data_ov000_02364558, data_ov000_0236a6f0[0x12], 0);
        w->unk54 |= 0x800;
        break;
    case 0xeb:
        StageObj_Load2dBAC(w, data_ov000_02367710, 0xffff, data_ov000_0236a740, data_ov000_02369ba0);
        StageObj_AllocSpritePalette(w, 0xc, 0x61);
        StageObj_SetAnimation(w, 0xc);
        break;
    case 0xed:
        StageObj_Load2dBAC(w, data_ov000_0236772c, 0xffff, data_ov000_0236a740, data_ov000_02369ba0);
        StageObj_AllocSpritePalette(w, 0xd, 0x62);
        StageObj_SetAnimation(w, 0x19);
        break;
    case 0xec:
    case 0xee:
        w->unk68 = DecSonicNeon_State_BlinkPair;
        w->unk5C |= 0x20;
        break;
    case 0xef:
        w->unk8 = 0xffff;
        w->unk68 = DecSonicNeon_State_BlinkFull;
        w->unk5C |= 0x20;
        break;
    }
    if (w->spr0.unk46 == 0)
        StageObj_SetOAMOrder(w, 0x16);
    w->unk58 |= 0x2100;
    w->unk54 |= 2;
    return w;
}
