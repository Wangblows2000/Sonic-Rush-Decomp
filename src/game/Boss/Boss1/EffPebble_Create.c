#include <game/Stage/stageobj_alt.h>

typedef struct {
    u8 pad0[0x18];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad24[0xB0 - 0x24];
} UnkStruct_02327b5c_Blk;

typedef struct {
    UnkStruct_EffObj base;
    UnkStruct_02327b5c_Blk blk;
} UnkStruct_02327b5c;

extern void *gZoneActArchive;
extern u8 data_ov000_02367c2c[];

void EffPebble_State_Lifetime(void);

UnkStruct_02327b5c *EffPebble_Create(s32 x, s32 y, s32 z, s16 a, s16 b, s16 c, s32 d) {
    UnkStruct_Task *task;
    UnkStruct_02327b5c *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_02327b5c));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_02327b5c));
    StageObj_LoadModelSimple(w, (UnkStruct_Blk *)&w->blk, data_ov000_02367c2c, 0, 0, 0, NULL, gZoneActArchive);
    w->base.unkC = d;
    w->base.unk10 = x;
    w->base.unk14 = y;
    w->base.unk18 = z;
    w->base.unk44 = 0x100;
    w->base.unk46 = 0x100;
    w->base.unk2E = a;
    w->base.unk30 = b;
    w->base.unk32 = c;
    w->base.unk40 = 0x2a;
    w->base.unk42 = 0xf00;
    w->blk.unk18 = w->blk.unk1C = w->blk.unk20 = 0x34cc;
    w->base.unk58 |= 0x180;
    w->base.unk54 |= 2;
    w->base.unk68 = EffPebble_State_Lifetime;
    return w;
}
