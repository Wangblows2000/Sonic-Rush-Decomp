#include <nitro/types.h>

typedef struct {
    s16 x;
    s16 y;
    u8 pad4[0x28];
} UnkAnim_022db2fc;

typedef struct {
    s32 timer;
    s32 unk4;
    u8 pad8[0xb44 - 8];
    UnkAnim_022db2fc anims[6];
} UnkWork_022db2fc;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022db2fc *work;
} UnkTask_022db2fc;

extern UnkTask_022db2fc *gTaskCurrent;

void KeyframeAnim_Update(UnkAnim_022db2fc *a);
BOOL KeyframeAnim_IsFinished(UnkAnim_022db2fc *a);
void VsMenu_Unk_ov001_022d9e50(UnkWork_022db2fc *w, s32 x, s32 y);
void VsMenu_Unk_ov001_022d9e2c(UnkWork_022db2fc *w, s32 x, s32 y);
void VsMenu_Unk_ov001_022d9d98(UnkWork_022db2fc *w, s32 i, s32 x, s32 y);
void VsMenu_Unk_ov001_022d9d64(UnkWork_022db2fc *w, s32 i, s32 x, s32 y);
void VsMenu_Unk_ov001_022da154(UnkWork_022db2fc *w, s32 a);
void VsMenu_Unk_ov001_022da244(UnkWork_022db2fc *w, s32 a);
void VsMenu_Unk_ov001_022da1cc(UnkWork_022db2fc *w, s32 a);
void VsMenu_Unk_ov001_022da288(UnkWork_022db2fc *w, s32 a);
void VsMenu_Unk_ov001_022d9fa4(UnkWork_022db2fc *w);
void VsMenu_State_Unk_ov001_022db150(void);

#pragma thumb on
void VsMenu_State_Unk_ov001_022db2fc(void) {
    UnkWork_022db2fc *w;
    BOOL done;
    UnkAnim_022db2fc *a;

    w = gTaskCurrent->work;
    KeyframeAnim_Update(&w->anims[0]);
    KeyframeAnim_Update(&w->anims[1]);
    KeyframeAnim_Update(&w->anims[2]);
    KeyframeAnim_Update(&w->anims[3]);
    KeyframeAnim_Update(&w->anims[4]);
    KeyframeAnim_Update(&w->anims[5]);
    a = &w->anims[0];
    VsMenu_Unk_ov001_022d9e50(w, a->x, a->y);
    a = &w->anims[1];
    VsMenu_Unk_ov001_022d9e2c(w, a->x, a->y);
    a = &w->anims[2];
    VsMenu_Unk_ov001_022d9d98(w, 0, a->x, a->y);
    a = &w->anims[3];
    VsMenu_Unk_ov001_022d9d98(w, 1, a->x, a->y);
    a = &w->anims[4];
    VsMenu_Unk_ov001_022d9d64(w, 0, a->x, a->y);
    a = &w->anims[5];
    VsMenu_Unk_ov001_022d9d64(w, 1, a->x, a->y);
    done = TRUE;
    if (!KeyframeAnim_IsFinished(&w->anims[0])) done = FALSE;
    if (!KeyframeAnim_IsFinished(&w->anims[1])) done = FALSE;
    if (!KeyframeAnim_IsFinished(&w->anims[2])) done = FALSE;
    if (!KeyframeAnim_IsFinished(&w->anims[3])) done = FALSE;
    if (!KeyframeAnim_IsFinished(&w->anims[4])) done = FALSE;
    if (!KeyframeAnim_IsFinished(&w->anims[5])) done = FALSE;
    w->timer++;
    if (done) {
        w->timer = 0;
        w->unk4 = 0;
        VsMenu_Unk_ov001_022da154(w, 0);
        VsMenu_Unk_ov001_022da154(w, 1);
        VsMenu_Unk_ov001_022da288(w, 1);
        VsMenu_Unk_ov001_022da288(w, 1);
        VsMenu_Unk_ov001_022d9fa4(w);
        gTaskCurrent->func = VsMenu_State_Unk_ov001_022db150;
    }
}
#pragma thumb reset
