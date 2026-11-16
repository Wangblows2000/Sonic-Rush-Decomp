#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0xc];
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u8 pad18[0x2e - 0x18];
    s16 unk2E;
    s16 unk30;
    u8 pad32[0x54 - 0x32];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x108 - 0x5e];
    u32 unk108;
    u8 pad10C[0x134 - 0x10c];
} UnkWork_02333694;

typedef struct {
    u8 pad0[0x18];
    u32 unk18;
} UnkStruct_022c4560;

extern void *gActComArchive;
extern UnkStruct_022c4560 data_022c4560;
extern u8 data_ov000_02368554[], data_ov000_0236aa30[];
extern s16 data_0206e470[];

#define SIN(a) data_0206e470[(((a) & 0xffff) >> 4) * 2]
#define COS(a) data_0206e470[(((a) & 0xffff) >> 4) * 2 + 1]

void StageObj_Destructor(void);
void Math_RotateVec2S16(s16 *x, s16 *y, u8 a);
void func_ov000_02334264(UnkWork_02333694 *w);

UnkWork_02333694 *EffBrakeSmoke_Create(UnkStruct_StageObj *arg, s16 x, s16 y, s32 unused) {
    UnkStruct_Task *task;
    UnkWork_02333694 *w;
    u8 a;
    vu16 zero;

    a = 0;
    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xa00, 4, sizeof(UnkWork_02333694));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkWork_02333694));
    StageObj_Load2dBAC(w, data_ov000_02368554, 2, data_ov000_0236aa30, gActComArchive);
    StageObj_AllocSpritePalette(w, 0, 0xc);
    StageObj_SetOAMOrder(w, 9);
    StageObj_SetOAMPriority(w, 1);
    if (data_022c4560.unk18 & 0x20)
        w->unk108 |= 0x80008;
    if (arg->unk5C & 1)
        w->unk5C |= 1;
    w->unk10 = arg->unk10;
    w->unk14 = arg->unk14;
    w->unk2E = 0x10;
    w->unk30 = -6;
    if (arg->unk5 != 0) {
        a = arg->unk5;
        w->unk2E = (w->unk2E * SIN((u16)(a << 8))) >> 12;
        w->unk30 = (w->unk30 * COS((u16)(a << 8))) >> 12;
    }
    w->unkC = (w->unk2E << 16) | (w->unk30 & 0xffff);
    w->unk2E = -0x80;
    w->unk30 = 0;
    if (arg->unk5 != 0) {
        w->unk2E = (w->unk2E * SIN((u16)(a << 8))) >> 12;
        w->unk30 = (w->unk30 * COS((u16)(a << 8))) >> 12;
    }
    if (w->unk5C & 1) {
        x = -x;
        w->unk2E = -w->unk2E;
    }
    Math_RotateVec2S16(&x, &y, arg->unk1 + arg->unk5);
    w->unk10 += x << 8;
    w->unk14 += y << 8;
    w->unk58 |= 0x100;
    w->unk54 |= 2;
    func_ov000_02334264(w);
    return w;
}
