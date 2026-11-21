#include <nitro/types.h>

typedef struct {
    s16 x;
    s16 y;
    u8 pad[0x28];
} UnkAnim_022dc998;

typedef struct {
    s32 step;
    s32 unk4;
    u8 pad8[0xb44 - 8];
    UnkAnim_022dc998 anim[6];
} UnkWork_022dc998;

typedef struct {
    u8 pad[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022dc998 *work;
} UnkTask_022dc998;

extern UnkTask_022dc998 *gTaskCurrent;

void KeyframeAnim_Update(UnkAnim_022dc998 *a);
BOOL KeyframeAnim_IsFinished(UnkAnim_022dc998 *a);
extern s32 gVsMenuCancelled;
void WirelessLobbyDownloadPlay_Dummy(void);
void VsMenu_State_WaitDownloadPlayGone(void);
void VsMenu_Unk_ov001_022d9e50(UnkWork_022dc998 *w, s16 x, s16 y);
void VsMenu_Unk_ov001_022d9e2c(UnkWork_022dc998 *w, s16 x, s16 y);
void VsMenu_Unk_ov001_022d9d98(UnkWork_022dc998 *w, s32 i, s16 x, s16 y);
void VsMenu_Unk_ov001_022d9d64(UnkWork_022dc998 *w, s32 i, s16 x, s16 y);

#pragma thumb on
void VsMenu_State_Unk_ov001_022dc998(void) {
    UnkWork_022dc998 *w = gTaskCurrent->work;
    BOOL done;
    UnkAnim_022dc998 *a;

    KeyframeAnim_Update(&w->anim[0]);
    KeyframeAnim_Update(&w->anim[1]);
    KeyframeAnim_Update(&w->anim[2]);
    KeyframeAnim_Update(&w->anim[3]);
    KeyframeAnim_Update(&w->anim[4]);
    KeyframeAnim_Update(&w->anim[5]);
    done = TRUE;
    if (!KeyframeAnim_IsFinished(&w->anim[0])) done = FALSE;
    if (!KeyframeAnim_IsFinished(&w->anim[1])) done = FALSE;
    if (!KeyframeAnim_IsFinished(&w->anim[2])) done = FALSE;
    if (!KeyframeAnim_IsFinished(&w->anim[3])) done = FALSE;
    if (!KeyframeAnim_IsFinished(&w->anim[4])) done = FALSE;
    if (!KeyframeAnim_IsFinished(&w->anim[5])) done = FALSE;
    a = &w->anim[0];
    VsMenu_Unk_ov001_022d9e50(w, a->x, a->y);
    a = &w->anim[1];
    VsMenu_Unk_ov001_022d9e2c(w, a->x, a->y);
    a = &w->anim[2];
    VsMenu_Unk_ov001_022d9d98(w, 0, a->x, a->y);
    a = &w->anim[3];
    VsMenu_Unk_ov001_022d9d98(w, 1, a->x, a->y);
    a = &w->anim[4];
    VsMenu_Unk_ov001_022d9d64(w, 0, a->x, a->y);
    a = &w->anim[5];
    VsMenu_Unk_ov001_022d9d64(w, 1, a->x, a->y);
    w->step++;
    if (done) {
        w->step = 0;
        w->unk4 = 0;
        gVsMenuCancelled = 0;
        WirelessLobbyDownloadPlay_Dummy();
        gTaskCurrent->func = VsMenu_State_WaitDownloadPlayGone;
    }
}
#pragma thumb reset
