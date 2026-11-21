#include <nitro/types.h>

typedef struct {
    s16 x;
    s16 y;
    u8 pad4[0x28];
} UnkAnim_022dbfdc;

typedef struct {
    s32 timer;
    u8 pad4[0xb44 - 4];
    UnkAnim_022dbfdc anims[2];
} UnkWork_022dbfdc;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022dbfdc *work;
} UnkTask_022dbfdc;

extern UnkTask_022dbfdc *gTaskCurrent;

void KeyframeAnim_Update(UnkAnim_022dbfdc *a);
BOOL KeyframeAnim_IsFinished(UnkAnim_022dbfdc *a);
void VsMenu_Unk_ov001_022d9e50(UnkWork_022dbfdc *w, s32 x, s32 y);
void VsMenu_Unk_ov001_022d9e2c(UnkWork_022dbfdc *w, s32 x, s32 y);
void WirelessLobby_StartChild(void);
void VsMenu_PrepareMsgBox(UnkWork_022dbfdc *w);
void MsgBox_Create(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);
void VsMenu_State_Unk_ov001_022dbe3c(void);

#pragma thumb on
void VsMenu_State_ChildOpen(void) {
    UnkWork_022dbfdc *w;
    BOOL done;
    UnkAnim_022dbfdc *a;

    w = gTaskCurrent->work;
    KeyframeAnim_Update(&w->anims[0]);
    KeyframeAnim_Update(&w->anims[1]);
    a = &w->anims[0];
    VsMenu_Unk_ov001_022d9e50(w, a->x, a->y);
    a = &w->anims[1];
    VsMenu_Unk_ov001_022d9e2c(w, a->x, a->y);
    done = TRUE;
    if (!KeyframeAnim_IsFinished(&w->anims[0])) done = FALSE;
    if (!KeyframeAnim_IsFinished(&w->anims[1])) done = FALSE;
    w->timer++;
    if (done) {
        w->timer = 0;
        WirelessLobby_StartChild();
        VsMenu_PrepareMsgBox(w);
        MsgBox_Create(4, 3, 5, 1, 2, 0, 0xe, 0x200010);
        gTaskCurrent->func = VsMenu_State_Unk_ov001_022dbe3c;
    }
}
#pragma thumb reset
