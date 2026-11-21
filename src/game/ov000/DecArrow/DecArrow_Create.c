#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x54];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x94 - 0x5E];
    UnkStruct_Spr spr0;
    u8 padDC[0x19C - 0xDC];
} UnkStruct_023234b8;

extern void *data_ov000_02369ba0;
extern void *data_ov000_0236a6f0[];
extern u8 data_ov000_02367450[], data_ov000_0236a790[], data_ov000_0236a7a0[], data_ov000_0236a7b0[];
extern u8 data_ov000_0236457c[];

void Decoration_Destructor(void);
void Decoration_InitFromMap(void *w, void *p, s32 a, s32 b);
void PaletteAnim_Create(u16 a, void *b, void *c, s32 d);

UnkStruct_023234b8 *DecArrow_Create(UnkStruct_ObjParam *p, s32 a, s32 b) {
    UnkStruct_Task *task;
    UnkStruct_023234b8 *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, Decoration_Destructor, 0, 0x500, 4, sizeof(UnkStruct_023234b8));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_023234b8));
    Decoration_InitFromMap(w, p, a, b);
    StageObj_Load2dBAC(w, data_ov000_02367450, 0, data_ov000_0236a6f0, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 0x3a);
    if (p->unk2 <= 0x89) {
        StageObj_SetSharedSpriteVram(w, 0xffff, data_ov000_0236a790);
        StageObj_SetAnimation(w, 0);
    } else if (p->unk2 <= 0x8b) {
        StageObj_SetSharedSpriteVram(w, 0xffff, data_ov000_0236a7a0);
        StageObj_SetAnimation(w, 1);
    } else {
        StageObj_SetSharedSpriteVram(w, 0xffff, data_ov000_0236a7b0);
        StageObj_SetAnimation(w, 2);
    }
    if (p->unk2 == 0x89 || p->unk2 == 0x8d || p->unk2 == 0x8f)
        w->unk5C ^= 1;
    if (p->unk2 == 0x8b || p->unk2 >= 0x8e)
        w->unk5C ^= 2;
    StageObj_SetOAMOrder(w, 0x16);
    PaletteAnim_Create(w->spr0.unk44, data_ov000_0236457c, data_ov000_0236a6f0[0], 0);
    w->unk54 |= 0x800;
    if (w->spr0.unk46 == 0)
        StageObj_SetOAMOrder(w, 0x16);
    w->unk58 |= 0x2100;
    w->unk54 |= 2;
    return w;
}
