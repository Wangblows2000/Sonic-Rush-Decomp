#include <nitro/types.h>

typedef struct {
    void (*fn)(void);
} UnkWork_023354ac;

typedef struct {
    u8 pad0[0x1c];
    u8 unk1C;
    u8 unk1D;
} UnkStruct_022c4560;

typedef struct {
    u8 pad0[0xc8];
    u32 unkC8;
} UnkStruct_0236aad8;

extern UnkStruct_022c4560 gGameState;
extern UnkStruct_0236aad8 gCamera;

void StageEvent_SpawnAll(UnkWork_023354ac *w);
void StageEvent_SpawnInView(UnkWork_023354ac *w, s32 idx, u32 flags);
void StageEvent_SpawnInViewSingleScreen(UnkWork_023354ac *w, s32 idx, u32 flags);
void StageEvent_UpdateBoss(void);
void StageEvent_UpdateSingleScreen(void);
void StageEvent_UpdateDualScreen(void);

void StageEvent_InitialSpawn(UnkWork_023354ac *w) {
    if (!(gGameState.unk1C < 9 && gGameState.unk1D < 2)) {
        StageEvent_SpawnAll(w);
        w->fn = StageEvent_UpdateBoss;
        return;
    }
    if (gCamera.unkC8 & 1) {
        StageEvent_SpawnInViewSingleScreen(w, 0, 1);
        w->fn = StageEvent_UpdateSingleScreen;
        return;
    }
    StageEvent_SpawnInView(w, 0, 1);
    StageEvent_SpawnInView(w, 1, 1);
    w->fn = StageEvent_UpdateDualScreen;
}
