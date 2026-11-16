#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_022c4560;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x440 - 0x20C];
    u16 unk440;
    u16 unk442;
    u8 pad444[0x458 - 0x444];
    s32 unk458;
} UnkStruct_022f14a8;

extern UnkStruct_022c4560 gGameState;
extern u16 data_ov000_02365e20[][0x18];
extern u32 data_ov000_02365e60[][3][8];
extern s16 data_ov000_02365da0[][3][8];
extern u16 data_ov000_02365e00[][3][8];
extern u8 data_ov000_02365c5c[][3];

u16 FX_DivS32(u32 a, s32 b);
void BossFPartD8_State_Unk_ov000_022f1374(UnkStruct_022f14a8 *w);

static inline BOOL IsMode23(void) {
    return gGameState.unk10 == 2 || gGameState.unk10 == 3;
}

static inline s32 GetLevel(void) {
    return IsMode23() ? 1 : gGameState.unk14;
}

void BossFPartD8_State_Unk_ov000_022f14a8(UnkStruct_022f14a8 *w) {
    if (w->unk458 == 2) {
        w->unk442 = data_ov000_02365e20[GetLevel()][0];
        w->unk442 += 0x3c;
    } else {
        w->unk442 = FX_DivS32(data_ov000_02365e60[GetLevel()][w->unk458][w->unk440], data_ov000_02365da0[GetLevel()][w->unk458][w->unk440]);
        w->unk442 += data_ov000_02365e00[GetLevel()][w->unk458][w->unk440];
        if (w->unk440 + 1 == data_ov000_02365c5c[GetLevel()][w->unk458])
            w->unk442 += 0x3c;
    }
    w->base.unk68 = BossFPartD8_State_Unk_ov000_022f1374;
    ((void (*)(UnkStruct_022f14a8 *))w->base.unk68)(w);
}
