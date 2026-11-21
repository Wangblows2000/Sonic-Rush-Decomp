#include <game/Stage/stageobj.h>

typedef struct {
    u32 unk0;
    s32 unk4;
    u16 unk8;
    u16 unkA;
} UnkStruct_02369ac0;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_022c4560;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x242 - 0x20C];
    u16 unk242;
    u8 pad244[0x29A - 0x244];
    u16 unk29A;
    u8 pad29C[0x2C6 - 0x29C];
    u16 unk2C6;
    u8 pad2C8[0x31E - 0x2C8];
    u16 unk31E;
    u8 pad320[0x434 - 0x320];
    s32 unk434;
    u16 unk438;
    u8 pad43A[2];
    u16 unk43C;
    u16 unk43E;
    u8 pad440[0x458 - 0x440];
    s32 unk458;
} UnkStruct_022f3874;

extern UnkStruct_022c4560 gGameState;
extern UnkStruct_02369ac0 gBossStatus;
extern u8 data_ov000_02363e48[];
extern u16 data_ov000_02363ffc[][9];
extern u16 data_ov000_02363f7c[][8];

void BossFPartD8_State_HandSlamVulnerable(UnkStruct_022f3874 *w);
void BossFPartD8_State_HandSlamLift(UnkStruct_022f3874 *w);

static inline BOOL IsMode23(void) {
    return gGameState.unk10 == 2 || gGameState.unk10 == 3;
}

static inline s32 GetLevel(void) {
    return IsMode23() ? 1 : gGameState.unk14;
}

void BossFPartD8_State_HandSlamStartRecover(UnkStruct_022f3874 *w) {
    w->unk43E = 0;
    if (w->unk438 + 1 < data_ov000_02363e48[w->unk458])
        w->unk43C = data_ov000_02363ffc[GetLevel()][w->unk458];
    else
        w->unk43C = data_ov000_02363f7c[GetLevel()][gBossStatus.unk8 - 1];
    if (w->unk43C != 0) {
        if (w->unk434 != 0) {
            w->unk242 &= ~4;
            w->unk29A |= 4;
        } else {
            w->unk2C6 &= ~4;
            w->unk31E |= 4;
        }
        w->base.unk68 = BossFPartD8_State_HandSlamVulnerable;
    } else {
        w->base.unk68 = BossFPartD8_State_HandSlamLift;
    }
    ((void (*)(UnkStruct_022f3874 *))w->base.unk68)(w);
}
