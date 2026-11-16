#include <nitro/types.h>

typedef struct {
    s32 state;
    u8 pad4[4];
    s32 unk8;
    s32 unkc;
    u8 pad10[8];
    s32 unk18;
    u8 pad1c[0xdb0 - 0x1c];
    s32 unkdb0;
    s32 unkdb4;
    u8 paddb8[0xdcc - 0xdb8];
} UnkWork_022ddcd4;

typedef struct UnkTask_022ddcd4 {
    u8 pad0[8];
    void (*fn)(void);
    u8 padc[4];
    UnkWork_022ddcd4 *work;
} UnkTask_022ddcd4;

extern UnkTask_022ddcd4 *gVsMenuTask;
extern s32 gVsMenuCancelled;
extern s32 gSaveDataBusy;
extern u32 gSystemFlags;

UnkTask_022ddcd4 *Task_Create(void (*fn)(void), void (*dtor)(void), s32 b, s32 c, s32 d, s32 e);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
s32 WirelessLobby_GetLinkLevel(void);
void WirelessSession_Reset(void);
void VsMenu_State_None(void);
void VsMenu_Destructor(void);
void VsMenu_State_LoadAssets(void);

#pragma thumb on
void VsMenu_Create(s32 a, s32 b, s32 c) {
    UnkTask_022ddcd4 *t = Task_Create(VsMenu_State_None, VsMenu_Destructor, 0, 0x1000, 0, sizeof(UnkWork_022ddcd4));
    UnkWork_022ddcd4 *w;

    gVsMenuTask = t;
    w = t->work;
    {
        vu32 z = 0;
        MIi_CpuClear32(z, w, sizeof(UnkWork_022ddcd4));
    }
    w->state = 0;
    w->unk8 = a;
    w->unkc = WirelessLobby_GetLinkLevel();
    w->unkdb0 = b;
    w->unkdb4 = c;
    w->unk18 = 0;
    gVsMenuCancelled = 1;
    gSaveDataBusy = 1;
    gSystemFlags |= 0x10;
    WirelessSession_Reset();
    t->fn = VsMenu_State_LoadAssets;
}
#pragma thumb reset
