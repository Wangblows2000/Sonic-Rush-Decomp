#include <ov000_b/stageobj.h>
#include <ov000_b/res.h>

typedef struct {
    u8 pad0[0x10];
} UnkStruct_0236a790;

typedef struct {
    u8 pad0[0x54];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x94 - 0x5E];
    UnkStruct_Spr spr0;
    UnkStruct_Spr spr1;
    u8 pad124[0x19C - 0x124];
} UnkStruct_023240f8;

extern void *data_ov000_02369ba0;
extern UnkStruct_Res *data_ov000_0236a6f0[];
extern UnkStruct_0236a790 data_ov000_0236a790[];
extern u8 data_ov000_023675e0[], data_ov000_023675f8[], data_ov000_0236a6f8[], data_ov000_0236a840[];

void Decoration_Destructor(void);
s32 FX_DivS32(s32 a, s32 b);
void Decoration_InitFromMap(void *w, UnkStruct_ObjParam *p, s32 a, s32 b);

UnkStruct_023240f8 *DecGroup67_Create(UnkStruct_ObjParam *p, s32 a, s32 b) {
    UnkStruct_Task *task;
    UnkStruct_023240f8 *w;
    vu16 zero;
    s32 i;

    task = Task_Create(StageObj_Main, Decoration_Destructor, 0, 0x500, 4, sizeof(UnkStruct_023240f8));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_023240f8));
    Decoration_InitFromMap(w, p, a, b);
    switch (p->unk2) {
    default:
        StageObj_Load2dBAC(w, data_ov000_023675e0, 0, data_ov000_0236a6f0, data_ov000_02369ba0);
        StageObj_AllocSpritePalette(w, 0, 0x39);
        if (p->unk2 < 0x83) {
            i = p->unk2 - 0x67;
            i -= FX_DivS32(i, 7) * 7;
            StageObj_SetSharedSpriteVram(w, Res_GetElemVal(data_ov000_0236a6f0[0], i), &data_ov000_0236a790[i]);
            StageObj_SetAnimation(w, (u16)i);
            if (p->unk2 >= 0x6e)
                w->unk5C ^= 1;
            if (p->unk2 >= 0x75)
                w->unk5C ^= 2;
            if (p->unk2 >= 0x75 && p->unk2 <= 0x7b)
                w->unk5C ^= 1;
        } else {
            i = p->unk2 - 0x83;
            StageObj_SetSharedSpriteVram(w, Res_GetElemVal(data_ov000_0236a6f0[0], i + 7), &data_ov000_0236a790[i + 7]);
            StageObj_SetAnimation(w, (u16)(i + 7));
        }
        if (p->unk2 == 0x84 || p->unk2 == 0x86)
            StageObj_SetOAMOrder(w, 5);
        else
            StageObj_SetOAMOrder(w, 6);
        StageObj_SetOAMPriority(w, 0);
        break;
    case 0x91:
    case 0x92:
    case 0x93:
    case 0x94:
        StageObj_Load2dBAC(w, data_ov000_023675f8, 0, data_ov000_0236a6f8, data_ov000_02369ba0);
        StageObj_SetSharedSpriteVram(w, 0xffff, data_ov000_0236a840);
        if (p->unk2 < 0x93)
            StageObj_AllocSpritePalette(w, 0, 0x3e);
        else
            StageObj_AllocSpritePalette(w, 1, 0x3f);
        if (!(p->unk2 & 1))
            w->unk5C ^= 1;
        StageObj_SetOAMOrder(w, 0x16);
        break;
    }
    if (w->spr0.unk46 == 0)
        StageObj_SetOAMOrder(w, 0x16);
    w->unk58 |= 0x2100;
    w->unk54 |= 2;
    return w;
}
