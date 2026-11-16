#include <nitro/types.h>

typedef struct {
    s32 kind;
    s32 unk4;
} UnkState_02051c30;

typedef struct {
    s32 unk0;
} UnkStruct_022c4560;

extern UnkState_02051c30 *gSysSound;
extern UnkStruct_022c4560 gGameState;
extern void *gSoundHeap;
void SysSound_StopAll(void);
void NNS_SndArcLoadGroup(s32 id, void *p);

#pragma thumb on
void SysSound_LoadVoiceGroup(s32 x) {
    s32 kind;
    s32 id;
    if (x >= 23)
        return;
    if (x <= 5)
        kind = 0;
    else if (x <= 7)
        kind = 1;
    else
        return;
    if (kind == gSysSound->kind)
        return;
    SysSound_StopAll();
    gSysSound->unk4 = 7;
    switch (kind) {
    case 0:
        id = 1;
        break;
    case 1:
        id = 0x15;
        break;
    case 2:
        if (gGameState.unk0 == 0)
            id = 2;
        else
            id = 3;
        break;
    }
    NNS_SndArcLoadGroup(id, gSoundHeap);
    gSysSound->kind = kind;
}
#pragma thumb reset
