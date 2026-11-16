#include <ov000_c/stageobj.h>

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

extern UnkStruct_022c4560 gGameState;
extern UnkStruct_02369ac0 gBossStatus;
extern u8 data_ov000_02363e20[][4];
extern u8 data_ov000_02363e00[][4];
extern u8 data_ov000_02363e40[][4];
extern u8 data_ov000_02363e08[][4];

static inline BOOL IsMode23(void) {
    return gGameState.unk10 == 2 || gGameState.unk10 == 3;
}

static inline s32 GetLevel(void) {
    return IsMode23() ? 1 : gGameState.unk14;
}

u8 BossFPartD8_Unk_ov000_022f3f8c(void) {
    u8 ret = 0;
    u16 hp = gBossStatus.unk8;

    if (hp >= 7)
        ret = data_ov000_02363e20[GetLevel()][Rand() & 3];
    else if (hp >= 6)
        ret = data_ov000_02363e00[GetLevel()][Rand() & 3];
    else if (hp >= 4)
        ret = data_ov000_02363e40[GetLevel()][Rand() & 3];
    else if (hp >= 2)
        ret = data_ov000_02363e08[GetLevel()][Rand() & 3];
    return ret;
}
