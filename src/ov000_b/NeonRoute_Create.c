#include <ov000_b/stageobj.h>

typedef struct {
    void *unk0;
    u8 pad4[0x10 - 4];
    void *unk10;
    u8 pad14[4];
    void *unk18;
    s16 unk1C;
    s16 unk1E;
    u16 unk20;
    u16 unk22;
} UnkStruct_02311900_Sub;

typedef struct {
    UnkStruct_StageObj base;
    UnkStruct_02311900_Sub sub;
} UnkStruct_02311900;

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02366cf8[], data_ov000_0236a114[], data_ov000_0236a11c[], data_ov000_0236a12c[];
extern u8 data_ov000_0236a13c[], data_ov000_0236a14c[], data_ov000_023661c0[], data_ov000_023661a4[];

void MapObject_Destructor(void);
void NeonRoute_Draw(void);
void NeonRoute_Action_Unk_ov000_02311500(UnkStruct_02311900 *w);

UnkStruct_02311900 *NeonRoute_Create(s32 a, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_02311900 *w;
    UnkStruct_ObjParam *p;
    vu16 zero;

    task = Task_Create(StageObj_Main, MapObject_Destructor, 0, 0xf6, 2, sizeof(UnkStruct_02311900));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_02311900));
    MapObject_Init(w, a, b, c);
    if (w->base.unk1E0->unk9 == 0)
        w->base.unk1E0->unk9 = 2;
    if (w->base.unk1E0->unk8 == 0)
        w->base.unk1E0->unk8 = 0x10;
    w->base.unk4A += w->base.unk1E0->unk8 * 0xc;
    StageObj_Load2dBAC(w, data_ov000_02366cf8, 0, data_ov000_0236a114, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 0x49);
    if (w->base.unk1E0->unk6 == 0) {
        if (w->base.unk1E0->unk2 == 0xc0) {
            StageObj_SetSharedSpriteVram(w, 0xffff, data_ov000_0236a12c);
            StageObj_SetAnimation(w, 0);
        } else {
            StageObj_SetSharedSpriteVram(w, 0xffff, data_ov000_0236a13c);
            StageObj_SetAnimation(w, 2);
        }
    } else {
        if (w->base.unk1E0->unk2 == 0xc0) {
            StageObj_SetSharedSpriteVram(w, 0xffff, data_ov000_0236a11c);
            StageObj_SetAnimation(w, 1);
        } else {
            StageObj_SetSharedSpriteVram(w, 0xffff, data_ov000_0236a14c);
            StageObj_SetAnimation(w, 3);
        }
    }
    StageObj_SetOAMOrder(w, 0x16);
    w->base.unk70 = NeonRoute_Draw;
    w->base.unk1B8 = w;
    w->base.unk1C8 = data_ov000_023661c0;
    w->base.unk1D0 = data_ov000_023661a4;
    if (w->base.unk1E0->unk2 == 0xc0) {
        w->sub.unk0 = w;
        w->sub.unk10 = data_ov000_023661c0;
        w->sub.unk18 = data_ov000_023661a4;
        w->base.unk1D8 = (w->base.unk1E0->unk9 * 0x18) >> 3;
        w->base.unk1DA = 2;
        if (w->base.unk1E0->unk7 > 0)
            w->base.unk1D4 = 0;
        else
            w->base.unk1D4 = -(w->base.unk1D8 << 3);
        w->base.unk1D6 = -0x18;
        w->sub.unk20 = w->base.unk1D8;
        w->sub.unk22 = w->base.unk1DA;
        p = w->base.unk1E0;
        if (p->unk7 > 0)
            w->sub.unk1C = w->base.unk1D4 - p->unk8 * 0xc;
        else
            w->sub.unk1C = w->base.unk1D4 + p->unk8 * 0xc;
        w->sub.unk1E = w->base.unk1D6;
    } else {
        w->base.unk1D8 = 6;
        w->base.unk1DA = 2;
        w->base.unk1D4 = -0x18;
        if (w->base.unk1E0->unk7 <= 0)
            w->base.unk1D6 = 4;
        else
            w->base.unk1D6 = -(((w->base.unk1E0->unk9 * 0x18) >> 3) << 3);
        w->sub.unk0 = w;
        w->sub.unk10 = data_ov000_023661c0;
        w->sub.unk18 = data_ov000_023661a4;
        w->sub.unk20 = w->base.unk1D8;
        w->sub.unk22 = w->base.unk1DA;
        w->sub.unk1C = w->base.unk1D4;
        p = w->base.unk1E0;
        if (p->unk7 > 0)
            w->sub.unk1E = w->base.unk1D6 + p->unk8 * 0xc;
        else
            w->sub.unk1E = w->base.unk1D6 - p->unk8 * 0xc;
    }
    w->base.unk58 |= 0x100;
    w->base.unk54 |= 2;
    w->base.unk1DC = Sound_AllocHandle();
    NeonRoute_Action_Unk_ov000_02311500(w);
    return w;
}
