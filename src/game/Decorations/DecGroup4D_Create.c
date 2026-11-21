#include <game/Stage/stageobj_alt.h>

typedef struct {
    u8 pad0[0xA];
    u8 unkA;
    u8 unkB;
    u8 padC[0x104 - 0xC];
} UnkStruct_02323074_Sub;

typedef struct {
    u8 pad0[0x54];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x68 - 0x5E];
    void *unk68;
    u8 pad6C[0x94 - 0x6C];
    UnkStruct_Spr spr0;
    UnkStruct_Spr spr1;
    u8 pad124[0x19C - 0x124];
    UnkStruct_02323074_Sub unk19C;
} UnkStruct_02323074;

extern void *data_ov000_02369ba0;
extern u8 data_ov000_023673b4[], data_ov000_0236a700[], data_ov000_0236a7d0[];
extern u8 data_ov000_0236a710[], data_ov000_023673cc[], data_ov000_0236a7f0[];
extern u8 data_ov000_023673e4[], data_ov000_0236a7e0[], data_ov000_023673fc[], data_ov000_0236a800[];
extern u8 data_ov000_02367414[], data_ov000_0236a708[], data_ov000_0236a860[];
extern u8 data_ov000_0236a760[], data_ov000_02367430[], data_ov000_0236a870[];

void Decoration_Destructor(void);
void DecGroup4D_State_Main(void);
void Decoration_InitFromMap(void *w, UnkStruct_ObjParam *p, s32 a, s32 b);
void StageObj_Load3dBAC(void *w, UnkStruct_02323074_Sub *s, void *a, s32 b, s32 c, void *d, void *e);
void StageObj_AcquireSharedTexture(UnkStruct_02323074_Sub *s, void *a, s32 b, s32 c);

UnkStruct_02323074 *DecGroup4D_Create(UnkStruct_ObjParam *p, s32 a, s32 b) {
    UnkStruct_Task *task;
    UnkStruct_02323074 *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, Decoration_Destructor, 0, 0x500, 4, sizeof(UnkStruct_02323074));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_02323074));
    Decoration_InitFromMap(w, p, a, b);
    switch (p->unk2) {
    case 0x4d:
        StageObj_Load2dBAC(w, data_ov000_023673b4, 0, data_ov000_0236a700, data_ov000_02369ba0);
        StageObj_SetSharedSpriteVram(w, 0x10, data_ov000_0236a7d0);
        w->spr0.unk2C |= 0x80000;
        StageObj_AllocSpritePalette(w, 0, 0x26);
        StageObj_SetOAMOrder(w, 0x16);
        StageObj_Load3dBAC(w, &w->unk19C, data_ov000_023673cc, 0, 0, data_ov000_0236a710, data_ov000_02369ba0);
        StageObj_AcquireSharedTexture(&w->unk19C, data_ov000_0236a7f0, 0x400, 0x10);
        w->unk19C.unkA = 4;
        w->unk19C.unkB = 0;
        StageObj_SetAnimation(w, 0);
        w->unk68 = DecGroup4D_State_Main;
        break;
    case 0x4e:
        StageObj_Load2dBAC(w, data_ov000_023673e4, 0, data_ov000_0236a700, data_ov000_02369ba0);
        StageObj_SetSharedSpriteVram(w, 8, data_ov000_0236a7e0);
        w->spr0.unk2C |= 0x80000;
        StageObj_AllocSpritePalette(w, 0, 0x26);
        StageObj_SetOAMOrder(w, 0x16);
        StageObj_Load3dBAC(w, &w->unk19C, data_ov000_023673fc, 0, 0, data_ov000_0236a710, data_ov000_02369ba0);
        StageObj_AcquireSharedTexture(&w->unk19C, data_ov000_0236a800, 0x200, 0x10);
        w->unk19C.unkA = 4;
        w->unk19C.unkB = 0;
        StageObj_SetAnimation(w, 1);
        w->unk68 = DecGroup4D_State_Main;
        break;
    case 0xcf:
        StageObj_Load2dBAC(w, data_ov000_02367414, 0, data_ov000_0236a708, data_ov000_02369ba0);
        StageObj_SetSharedSpriteVram(w, 0x10, data_ov000_0236a860);
        w->spr0.unk2C |= 0x80000;
        StageObj_AllocSpritePalette(w, 0, 0x50);
        StageObj_Load3dBAC(w, &w->unk19C, data_ov000_02367430, 0, 0, data_ov000_0236a760, data_ov000_02369ba0);
        StageObj_AcquireSharedTexture(&w->unk19C, data_ov000_0236a870, 0x400, 0x10);
        w->unk19C.unkA = 4;
        w->unk19C.unkB = 0;
        w->unk68 = DecGroup4D_State_Main;
        break;
    }
    if (w->spr0.unk46 == 0)
        StageObj_SetOAMOrder(w, 0x16);
    w->unk58 |= 0x2100;
    w->unk54 |= 2;
    return w;
}
