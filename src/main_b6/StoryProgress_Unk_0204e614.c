#include <nitro/types.h>

typedef struct {
    s32 unk0;
} UnkState_0204e614;

typedef struct {
    u8 pad0[8];
    u32 unk8;
} UnkProg_0204e614;

extern UnkState_0204e614 gGameState;
extern UnkProg_0204e614 gSaveData;
extern u8 data_02074bbc[];
extern u8 data_02074bb4[];

u8 StoryProgress_Unk_0204e614(u16 t) {
    u8 r = 0;
    u8 i = 0;
    u8 *tbl;

    if (gGameState.unk0 != 1)
        tbl = data_02074bbc;
    else
        tbl = data_02074bb4;
    for (; i < 7; i++) {
        if (t < tbl[i]) {
            if (t == tbl[i] - 1)
                r += 2;
            return r;
        }
        r += 3;
    }
    if (t < tbl[7])
        return r;
    if (gSaveData.unk8 & 0x1000)
        return r + 2;
    else
        return r + 1;
}
