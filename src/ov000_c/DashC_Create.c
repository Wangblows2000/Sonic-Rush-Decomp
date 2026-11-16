#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    UnkStruct_Spr sprA;
    UnkStruct_Spr sprB;
    u8 *unk29C;
    u8 pad2A0[4];
    s32 unk2A4;
} UnkStruct_02307380;

extern void *data_ov000_02369ba0;
extern u8 data_ov000_023668e8[], data_ov000_02369ecc[], data_ov000_02369ef4[], data_ov000_02369ee4[];
extern u8 data_ov000_02369f04[], data_ov000_02369ed4[], data_ov000_02369f14[], data_ov000_02369f24[];

void DashC_Destructor(void);
void DashC_OnHit(void);
void func_ov000_02306e74(UnkStruct_02307380 *w);
s32 ObjVramRef_Acquire(void *pal, s32 a, s32 n);
void AnimatorSprite_Init(UnkStruct_Spr *s, void *a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, u32 h);

UnkStruct_02307380 *DashC_Create(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_02307380 *w;
    vu16 zero;
    u8 *pal;
    s32 n;

    task = Task_Create(StageObj_Main, DashC_Destructor, 0, 0x500, 2, sizeof(UnkStruct_02307380));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_02307380));
    MapObject_Init(w, (s32)p, b, c);
    StageObj_Load2dBAC(w, data_ov000_023668e8, 0, data_ov000_02369ecc, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 2);
    switch (p->unk2) {
    case 0x6a:
    default:
        StageObj_SetSharedSpriteVram(w, 0x10, data_ov000_02369ef4);
        break;
    case 0x6b:
        StageObj_SetSharedSpriteVram(w, 0x10, data_ov000_02369ee4);
        break;
    case 0x6c:
    case 0x6d:
        StageObj_SetSharedSpriteVram(w, 0x12, data_ov000_02369f04);
        break;
    }
    w->base.spr0.unk46 = 9;
    w->base.spr1.unk46 = 9;
    w->base.spr0.unk2C &= ~0x60000;
    w->base.spr1.unk2C &= ~0x60000;
    switch (p->unk2) {
    case 0x6a:
    default:
        pal = data_ov000_02369ed4;
        n = 8;
        w->unk29C = pal;
        break;
    case 0x6b:
        pal = data_ov000_02369f14;
        n = 8;
        w->unk29C = pal;
        break;
    case 0x6c:
    case 0x6d:
        pal = data_ov000_02369f24;
        n = 0xc;
        w->unk29C = pal;
        break;
    }
    AnimatorSprite_Init(&w->sprA, *w->base.unk8C, 0, 0x40000, 0, 0, ObjVramRef_Acquire(pal, 0, n), 0, 0x05000200);
    AnimatorSprite_Init(&w->sprB, *w->base.unk8C, 0, 0x40000, 1, 0, ObjVramRef_Acquire(pal + 8, 1, n), 0, 0x05000600);
    w->sprA.unk46 = w->sprB.unk46 = 0x14;
    w->sprA.unk44 = w->base.spr0.unk44;
    w->sprB.unk44 = w->base.spr1.unk44;
    w->sprA.unk2C |= 0x10;
    w->sprB.unk2C |= 0x10;
    ObjRect_SetAtkDefFlags(&w->base.coll0, 0, -16);
    w->base.coll0.unk1C = DashC_OnHit;
    w->base.coll0.unkA |= 0x800;
    w->base.coll1.unk28 = 0;
    w->base.unk58 |= 0x2100;
    switch (w->base.unk1E0->unk2) {
    case 0x6a:
        StageObj_SetAnimation(w, 2);
        AnimatorSprite_SetAnimation(&w->sprA, 3);
        AnimatorSprite_SetAnimation(&w->sprB, 3);
        if (!(w->base.unk1E0->unk4 & 1)) {
            w->unk2A4 = 6;
        } else {
            w->base.unk5C |= 2;
            w->sprA.unk2C |= 0x100;
            w->sprB.unk2C |= 0x100;
            w->unk2A4 = 2;
        }
        break;
    case 0x6b:
        StageObj_SetAnimation(w, 0);
        AnimatorSprite_SetAnimation(&w->sprA, 1);
        AnimatorSprite_SetAnimation(&w->sprB, 1);
        if (w->base.unk1E0->unk4 & 1) {
            w->base.unk5C &= ~1;
            w->unk2A4 = 4;
        } else {
            w->base.unk5C |= 1;
            w->sprA.unk2C |= 0x80;
            w->sprB.unk2C |= 0x80;
            w->unk2A4 = 0;
        }
        break;
    case 0x6c:
        StageObj_SetAnimation(w, 4);
        AnimatorSprite_SetAnimation(&w->sprA, 5);
        AnimatorSprite_SetAnimation(&w->sprB, 5);
        if (w->base.unk1E0->unk4 & 1) {
            w->base.unk5C &= ~1;
            w->base.unk5C |= 2;
            w->sprA.unk2C |= 0x100;
            w->sprB.unk2C |= 0x100;
            w->unk2A4 = 1;
        } else {
            w->base.unk5C |= 1;
            w->sprA.unk2C |= 0x80;
            w->sprB.unk2C |= 0x80;
            w->unk2A4 = 5;
        }
        break;
    case 0x6d:
        StageObj_SetAnimation(w, 4);
        AnimatorSprite_SetAnimation(&w->sprA, 5);
        AnimatorSprite_SetAnimation(&w->sprB, 5);
        if (w->base.unk1E0->unk4 & 1) {
            w->base.unk5C |= 3;
            w->sprA.unk2C |= 0x180;
            w->sprB.unk2C |= 0x180;
            w->unk2A4 = 3;
        } else {
            w->base.unk5C &= ~1;
            w->unk2A4 = 7;
        }
        break;
    }
    func_ov000_02306e74(w);
    return w;
}
