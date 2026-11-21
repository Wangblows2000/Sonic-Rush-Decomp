#include <nitro/types.h>

typedef struct {
    s16 x;
    s16 y;
    u8 pad4[0x28];
} UnkAnim_022dbdb0;

typedef struct {
    s32 timer;
    u8 pad4[0xb44 - 4];
    UnkAnim_022dbdb0 anims[2];
} UnkWork_022dbdb0;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022dbdb0 *work;
} UnkTask_022dbdb0;

extern UnkTask_022dbdb0 *gTaskCurrent;

void KeyframeAnim_Update(UnkAnim_022dbdb0 *a);
BOOL KeyframeAnim_IsFinished(UnkAnim_022dbdb0 *a);
void Task_Destroy(UnkTask_022dbdb0 *t);
void VsMenu_Unk_ov001_022d9e50(UnkWork_022dbdb0 *w, s32 x, s32 y);
void VsMenu_Unk_ov001_022d9e2c(UnkWork_022dbdb0 *w, s32 x, s32 y);

#pragma thumb on
void VsMenu_State_Unk_ov001_022dbdb0(void) {
    UnkWork_022dbdb0 *w = gTaskCurrent->work;
    BOOL done;

    KeyframeAnim_Update(&w->anims[0]);
    KeyframeAnim_Update(&w->anims[1]);
    done = TRUE;
    if (!KeyframeAnim_IsFinished(&w->anims[0])) {
        done = FALSE;
    }
    if (!KeyframeAnim_IsFinished(&w->anims[1])) {
        done = FALSE;
    }
    if (done) {
        w->timer = 0;
        Task_Destroy(gTaskCurrent);
        return;
    }
    {
        UnkAnim_022dbdb0 *a = &w->anims[0];
        VsMenu_Unk_ov001_022d9e50(w, a->x, a->y);
    }
    {
        UnkAnim_022dbdb0 *a = &w->anims[1];
        VsMenu_Unk_ov001_022d9e2c(w, a->x, a->y);
    }
    w->timer++;
}
#pragma thumb reset
