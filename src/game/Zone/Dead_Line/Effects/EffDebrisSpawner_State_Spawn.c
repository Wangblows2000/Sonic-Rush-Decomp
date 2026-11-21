#include <game/Stage/stageobj_alt.h>

typedef struct {
    u8 pad0[0x1c];
    u8 unk1C;
    u8 pad1D[0x24 - 0x1d];
    u32 unk24;
} UnkStruct_022c4560;

extern UnkStruct_022c4560 gGameState;

void EffHahenZako_Create(void);

void EffDebrisSpawner_State_Spawn(void) {
    if (gGameState.unk1C != 6)
        return;
    if (gGameState.unk24 & 0x7f)
        return;
    if (!(Rand() & 1))
        return;
    EffHahenZako_Create();
}
