#include <ov000_c/stageobj.h>

typedef struct {
    u32 unk0;
    u8 pad[0x10 - 4];
    s32 unk10;
    s32 unk14;
} UnkStruct_022c4560;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x4BE - 0x20C];
    u16 unk4BE;
    s32 unk4C0;
    u16 unk4C4;
    u16 unk4C6;
} UnkStruct_022f61a0;

extern UnkStruct_022c4560 gGameState;
extern u16 data_ov000_02363db8[];

void BossFPartDE_State_Unk_ov000_022f5e74(UnkStruct_022f61a0 *w);

static inline s32 GetIdxB(void) {
    BOOL b = TRUE;
    if (gGameState.unk10 != 2 && gGameState.unk10 != 3)
        b = FALSE;
    if (b)
        return 1;
    return gGameState.unk14;
}

void func_ov000_022f61a0(UnkStruct_022f61a0 *w) {
    if (w->base.unk10 < 0x10000)
        w->unk4C0 = 0;
    if (w->base.unk10 > 0x24000)
        w->unk4C0 = 1;
    else
        w->unk4C0 = Rand() & 1;
    w->unk4C4 = data_ov000_02363db8[GetIdxB()];
    w->unk4BE = 0;
    w->unk4C6 = 0;
    w->base.unk68 = BossFPartDE_State_Unk_ov000_022f5e74;
    ((void (*)(UnkStruct_022f61a0 *))w->base.unk68)(w);
}
