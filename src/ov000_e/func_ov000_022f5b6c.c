#include <ov000_c/stageobj.h>

typedef struct {
    u32 unk0;
    u8 pad[0x10 - 4];
    s32 unk10;
    s32 unk14;
} UnkStruct_022c4560;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x214 - 0x20C];
    u16 unk214;
} UnkStruct_022f5b6c;

extern UnkStruct_022c4560 gGameState;
extern u8 data_ov000_02363db0[];

UnkStruct_StageObj *MapObject_Spawn(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i);
void BossFPartDE_State_Unk_ov000_022f58cc(void);

static inline s32 GetIdxB(void) {
    BOOL b = TRUE;
    if (gGameState.unk10 != 2 && gGameState.unk10 != 3)
        b = FALSE;
    if (b)
        return 1;
    return gGameState.unk14;
}

void func_ov000_022f5b6c(UnkStruct_022f5b6c *w) {
    s32 n;
    s32 i = GetIdxB();

    n = w->unk214 + 1;
    if (n < data_ov000_02363db0[i])
        MapObject_Spawn(0xde, 0, 0, (u16)((n << 4) | 1), 0, 0, 0, 0, 0);
    w->base.unk68 = BossFPartDE_State_Unk_ov000_022f58cc;
}
