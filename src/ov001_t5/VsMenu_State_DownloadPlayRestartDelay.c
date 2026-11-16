#include <nitro/types.h>

typedef struct {
    s32 timer;
    s32 unk4;
    u8 pad8[0x48 - 8];
    s32 unk48;
} UnkWork_022dccc8;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022dccc8 *work;
} UnkTask_022dccc8;

extern UnkTask_022dccc8 *gTaskCurrent;

void VsMenu_DrawDownloadPlayScreen(UnkWork_022dccc8 *w);
void WirelessLobbyDownloadPlay_RequestRestart(void);
void VsMenu_Unk_ov001_022da33c(UnkWork_022dccc8 *w);
void VsMenu_Unk_ov001_022da244(UnkWork_022dccc8 *w, s32 a);
void VsMenu_Unk_ov001_022da1cc(UnkWork_022dccc8 *w, s32 a);
void VsMenu_ClearNameSlot2(UnkWork_022dccc8 *w, s32 a);
void VsMenu_State_Unk_ov001_022dd1ec(void);

static inline UnkWork_022dccc8 *Set48_022dccc8(UnkWork_022dccc8 *w, s32 v) {
    w->unk48 = v;
    return w;
}

#pragma thumb on
void VsMenu_State_DownloadPlayRestartDelay(void) {
    UnkWork_022dccc8 *w;

    w = gTaskCurrent->work;
    VsMenu_DrawDownloadPlayScreen(w);
    w->timer++;
    if (w->timer >= 0x1e) {
        w->timer = 0;
        w->unk4 = 0;
        WirelessLobbyDownloadPlay_RequestRestart();
        VsMenu_Unk_ov001_022da33c(w);
        VsMenu_Unk_ov001_022da244(w, 1);
        VsMenu_Unk_ov001_022da1cc(w, 1);
        w->unk48 = 1;
        VsMenu_ClearNameSlot2(w, 1);
        gTaskCurrent->func = VsMenu_State_Unk_ov001_022dd1ec;
    }
}
#pragma thumb reset
