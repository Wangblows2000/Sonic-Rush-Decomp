#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    UnkStruct_Blk blkA;
    UnkStruct_Blk blkB;
    u8 pad36C[0x99C - 0x36C];
    UnkStruct_Spr sprA;
    UnkStruct_Spr sprB;
} UnkStruct_02305ad8;

extern void *data_ov000_02369ba0;
extern void *data_ov000_02369e9c;
extern u8 data_ov000_02366810[], data_ov000_02369e94[], data_ov000_02366828[], data_ov000_0236683c[];
extern u8 data_ov000_02366850[], data_ov000_02369ea4[];

void CranePartED_Destructor(void);
void CranePartED_Draw(void);
void CranePartED_OnHit(void);
u16 ObjDraw_AllocSpritePalette(void *a, s32 b, s32 c);
void CranePartED_Action_Init(UnkStruct_02305ad8 *w);

UnkStruct_02305ad8 *CranePartED_Create(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_02305ad8 *w;
    vu16 zero;
    u16 v;

    task = Task_Create(StageObj_Main, CranePartED_Destructor, 0, 0xf7, 2, sizeof(UnkStruct_02305ad8));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_02305ad8));
    MapObject_Init(w, (s32)p, b, c);
    StageObj_Load2dBAC(w, data_ov000_02366810, 4, &data_ov000_02369e9c, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 0x36);
    StageObj_SetOAMOrder(w, 0x14);
    StageObj_SetAnimation(w, 2);
    w->base.spr0.unk2C |= 0x80000;
    StageObj_LoadModelSimple(w, &w->blkA, data_ov000_02366828, 0, 0xb, 0xa, data_ov000_02369e94, data_ov000_02369ba0);
    StageObj_LoadModelSimple(w, &w->blkB, data_ov000_0236683c, 0, 0xc, 9, data_ov000_02369e94, data_ov000_02369ba0);
    w->base.unk5C |= 0x200;
    Obj_SetVec(&w->base.unk128->unk18, 0x34cc, 0x34cc, 0x34cc);
    ObjAction_Load2dBAC(&w->sprA, &w->sprB, data_ov000_02366850, 0, &data_ov000_02369e9c, data_ov000_02369ba0);
    AnimatorSprite_SetAnimation(&w->sprA, 0);
    AnimatorSprite_SetAnimation(&w->sprB, 0);
    w->sprA.unk46 = 0x14;
    w->sprB.unk46 = 0x14;
    v = ObjDraw_AllocSpritePalette(data_ov000_02369e9c, 0, 0x36);
    w->sprA.unk44 = v;
    w->sprB.unk44 = v;
    w->sprA.unk2C |= 0x10;
    w->sprB.unk2C |= 0x10;
    StageObj_AcquireSpriteVramPair(&w->sprA, &w->sprB, 1, data_ov000_02369ea4);
    w->base.unk70 = CranePartED_Draw;
    if (w->base.unk1E0->unk4 & 1)
        w->base.unk5C ^= 1;
    ObjRect_SetBox2D(&w->base.coll0, -0x78, 0, -0x70, 8);
    ObjRect_SetAtkDefFlags(&w->base.coll0, 0, -16);
    w->base.coll0.unkA |= 0x800;
    w->base.coll0.unk1C = CranePartED_OnHit;
    w->base.coll0.unk18 = w;
    w->base.unk1E0->unk7 = 0x40;
    w->base.unk14 += (w->base.unk1E0->unk7 + w->base.unk1E0->unk8 + w->base.unk1E0->unk9) << 8;
    if (w->base.unk1E0->unk4 & 4)
        w->base.unk34 = 0xd00;
    w->base.unk58 |= 0x100;
    w->base.unk54 |= 0x10;
    w->base.unk1DC = Sound_AllocHandle();
    CranePartED_Action_Init(w);
    return w;
}
