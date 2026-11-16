#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x1C];
    u8 unk1C;
} UnkStruct_022c4560;

extern void *data_ov000_02369ba0;
extern UnkStruct_022c4560 data_022c4560;
extern u8 data_ov000_02366d7c[], data_ov000_0236a194[], data_ov000_0236a19c[], data_ov000_023661c0[];
extern u8 data_ov000_02364434[];

void MapObject_Destructor(void);
void PaletteAnim_Create(s32 a, void *b, u32 c, s32 d);
BOOL func_ov000_02345758(s32 x, s32 y);
void func_ov000_02312c70(UnkStruct_StageObj *w);

UnkStruct_StageObj *GmkLandAA_Create(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_StageObj *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, MapObject_Destructor, 0, 0xf6, 2, sizeof(UnkStruct_StageObj));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_StageObj));
    MapObject_Init(w, (s32)p, b, c);
    StageObj_Load2dBAC(w, data_ov000_02366d7c, 0, data_ov000_0236a194, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 5);
    StageObj_SetSharedSpriteVram(w, 0x20, data_ov000_0236a19c);
    StageObj_SetOAMOrder(w, 0x15);
    w->unk1B8 = w;
    w->unk1C8 = data_ov000_023661c0;
    w->unk1D8 = 6;
    w->unk1DA = 6;
    w->unk1D4 = -0x17;
    w->unk1D6 = -0x17;
    func_02068e40(w, -0x15, -0x17, 0x15, 0x17);
    w->unk58 |= 0x4000080;
    w->unk54 |= 2;
    w->unk58 |= 0x20040;
    if (data_022c4560.unk1C == 3 && p->unk2 != 0x76) {
        PaletteAnim_Create(w->spr0.unk44, data_ov000_02364434, *(u32 *)data_ov000_0236a194, 0);
        w->unk54 |= 0x800;
    }
    func_ov000_02312c70(w);
    if (func_ov000_02345758(w->unk10, w->unk14)) {
        w->unk5C |= 2;
        w->unk5 = 0x80;
        w->unk1D6 -= 2;
    }
    w->unk4 = 0;
    w->unk3C = 0xa0;
    w->unk3E = 0xa00;
    w->unk48 = 0x1000;
    return w;
}
