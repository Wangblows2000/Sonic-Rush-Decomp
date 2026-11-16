#include <ov000_c/stageobj.h>

extern void *data_ov000_02369ba0;
extern UnkStruct_Task *data_ov000_0236a018;
extern u8 data_ov000_02366b4c[], data_ov000_0236a01c[];

void ItemBoxPart_Destructor_ov000_0230d5b8(void);
void ItemBoxPart_State_Unk_ov000_0230d544(void);
void ObjDraw_ReleaseSpritePalette(s32 a);

void ItemBox_CreatePart_ov000_0230d5d4(s32 a, s32 b, s32 c, u32 k) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    if (data_ov000_0236a018 != NULL) {
        w = data_ov000_0236a018->work;
        if (k >= 6 && k <= 8 && (u32)w->unk8 >= 6 && (u32)w->unk8 <= 8) {
        } else if ((k >= 6 && k <= 8) || ((u32)w->unk8 >= 6 && (u32)w->unk8 <= 8)) {
            ObjDraw_ReleaseSpritePalette(w->spr0.unk44 & 0xff);
            if (k >= 6 && k <= 8)
                StageObj_AllocSpritePalette(w, 8, 0x17);
            else
                StageObj_AllocSpritePalette(w, 0, 2);
        }
    } else {
        task = Task_Create(StageObj_Main, ItemBoxPart_Destructor_ov000_0230d5b8, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
        if (task == func_02006abc(0))
            return;
        data_ov000_0236a018 = task;
        w = task->work;
        zero = 0;
        MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
        StageObj_Load2dBAC(w, data_ov000_02366b4c, 2, data_ov000_0236a01c, data_ov000_02369ba0);
        if (k >= 6 && k <= 8)
            StageObj_AllocSpritePalette(w, 8, 0x17);
        else
            StageObj_AllocSpritePalette(w, 0, 2);
        w->spr0.unk46 = 3;
        w->spr1.unk46 = 3;
    }
    StageObj_SetAnimation(w, (u16)(k + 2));
    w->spr1.unk2C |= 0x80000;
    w->unk10 = 0x8000;
    w->unk14 = 0xb800;
    w->unk30 = 0x200;
    StageObj_SetOAMPriority(w, 0);
    StageObj_SetOAMOrder(w, 3);
    w->unk5C |= 1;
    w->unk8 = k;
    w->unkC = 0x80;
    w->unk58 |= 0x100;
    w->unk54 |= 0x12;
    w->unk5C |= 0x80;
    w->unk68 = ItemBoxPart_State_Unk_ov000_0230d544;
}
