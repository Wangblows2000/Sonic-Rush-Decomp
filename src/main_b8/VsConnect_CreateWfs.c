#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    u32 pad14;
    u32 flags;
} UnkState_02063620;

typedef struct {
    u32 pad0;
    s32 unk4;
} UnkWork_02063620;

typedef struct {
    u8 pad0[0x10];
    UnkWork_02063620 *work;
} UnkTask_02063620;

extern UnkState_02063620 gGameState;

UnkTask_02063620 *Task_Create(void (*func)(void), void (*dtor)(void), u16 prio, u16 id, u16 unk1a, u32 workSize);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void VsConnect_Main_WaitWfs(void);
void VsConnect_Destructor(void);
void VsConnect_InitDisplay(void);
void VsConnect_LoadAssets(UnkWork_02063620 *w);
void WirelessSession_StartWfs(void);

#pragma thumb on
void VsConnect_CreateWfs(void) {
    UnkWork_02063620 *w;
    vu16 z;

    gGameState.unk10 = 1;
    gGameState.flags |= 0x20;
    gGameState.flags |= 0x10;
    w = Task_Create(VsConnect_Main_WaitWfs, VsConnect_Destructor, 0, 0, 0, 0x1c0)->work;
    z = 0;
    MIi_CpuClear16(z, w, 0x1c0);
    w->unk4 = 1;
    VsConnect_InitDisplay();
    VsConnect_LoadAssets(w);
    WirelessSession_StartWfs();
}
#pragma thumb reset
