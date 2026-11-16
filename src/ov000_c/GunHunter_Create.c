#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
} UnkStruct_022fbe80;

typedef struct {
    u8 pad0[0x20];
    u32 unk20;
} UnkStruct_022c4560;

extern void *data_ov000_02369ba0;
extern UnkStruct_022c4560 gGameState;
extern u8 data_ov000_0236635c[], data_ov000_02369d9c[];

void MapObject_Destructor(void);
void GunHunter_State_Unk_ov000_022fbe14(void);
void func_ov000_022faf48(UnkStruct_022fbe80 *w, s32 a);

static inline u16 FrameRand(void) {
    return (u16)(((gGameState.unk20 << 16) * 0x196225 + 0x3c6ef35f) >> 16);
}

UnkStruct_022fbe80 *GunHunter_Create(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_022fbe80 *w;
    vu16 zero;
    s32 k;

    task = Task_Create(StageObj_Main, MapObject_Destructor, 0, 0x500, 2, sizeof(UnkStruct_022fbe80));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_022fbe80));
    MapObject_Init(w, (s32)p, b, c);
    StageObj_Load2dBAC(w, data_ov000_0236635c, 1, data_ov000_02369d9c, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 0x30);
    func_ov000_022faf48(w, 4);
    StageObj_SetOAMOrder(w, 9);
    if (!(w->base.unk1E0->unk4 & 1))
        w->base.unk5C &= ~1;
    w->base.unk2E = (FrameRand() & 0xff) + 0x500;
    if (w->base.unk5C & 1)
        w->base.unk2E = -w->base.unk2E;
    k = w->base.unk1E0->unk4 & 0xc;
    if (k != 0)
        w->base.unk2E = (w->base.unk2E * (((k >> 2) << 7) + 0x100)) >> 8;
    w->base.unk58 |= 0x110;
    w->base.unk5C |= 0x100;
    w->base.unk5C |= 4;
    w->base.unk68 = GunHunter_State_Unk_ov000_022fbe14;
    return w;
}
