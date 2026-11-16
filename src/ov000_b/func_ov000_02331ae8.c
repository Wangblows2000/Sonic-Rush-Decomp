#include <ov000_b/stageobj.h>

typedef struct {
    s32 unk0;
    u8 pad4[0x18 - 4];
    u32 unk18;
    u8 unk1C;
    u8 unk1D;
} UnkStruct_022c4560;

extern void *gActComArchive;
extern u32 gObjManagerFlags;
extern UnkStruct_022c4560 gGameState;
extern u8 data_ov000_02368478[];
extern u8 data_ov000_0236a9f8[];

BOOL func_ov000_023270d8(UnkStruct_StageObj *a);
void Math_RotateVec2S16(s16 *x, s16 *y, u8 a);
void func_ov000_02334264(UnkStruct_EffObj *w);

UnkStruct_EffObj *func_ov000_02331ae8(UnkStruct_StageObj *arg, s16 x, s16 y, s32 unused) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;
    u8 n;

    if (gObjManagerFlags & 0x100)
        return NULL;
    if (func_ov000_023270d8(arg))
        return NULL;
    if (gGameState.unk1C >= 9 || gGameState.unk1D >= 2)
        return NULL;
    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    StageObj_Load2dBAC(w, data_ov000_02368478, 6, data_ov000_0236a9f8, gActComArchive);
    if (gGameState.unk18 & 0x20)
        w->spr1.unk2C |= 0x80008;
    if (gGameState.unk0 != 1) {
        StageObj_AllocSpritePalette(w, 0, 0x17);
        StageObj_SetAnimation(w, 0);
    } else {
        StageObj_AllocSpritePalette(w, 1, 0xc);
        StageObj_SetAnimation(w, 1);
    }
    StageObj_SetOAMOrder(w, 9);
    if (arg->unk5C & 1)
        w->unk5C |= 1;
    w->unk10 = arg->unk10;
    w->unk14 = arg->unk14;
    w->unk2E = 0;
    w->unk30 = 0;
    n = arg->unk1 + arg->unk5;
    if (w->unk5C & 1)
        x = -x;
    Math_RotateVec2S16(&x, &y, n);
    if (w->unk5C & 1)
        w->unk2E = -w->unk2E;
    w->unk10 += x << 8;
    w->unk14 += y << 8;
    w->unk58 |= 0x100;
    w->unk54 |= 2;
    func_ov000_02334264(w);
    return w;
}
