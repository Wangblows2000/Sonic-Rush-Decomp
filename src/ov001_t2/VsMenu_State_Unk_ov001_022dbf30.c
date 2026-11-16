#include <nitro/types.h>

typedef struct {
    s16 x;
    s16 y;
    u8 pad4[0x28];
} UnkAnim_022dbf30;

typedef struct {
    s32 timer;
    u8 pad4[0xb44 - 4];
    UnkAnim_022dbf30 anims[2];
} UnkWork_022dbf30;

typedef struct {
    u8 pad0[8];
    void (*fn)(void);
    u8 padc[4];
    UnkWork_022dbf30 *work;
} UnkTask_022dbf30;

extern UnkTask_022dbf30 *gTaskCurrent;

void KeyframeAnim_Update(UnkAnim_022dbf30 *a);
BOOL KeyframeAnim_IsFinished(UnkAnim_022dbf30 *a);
void VsMenu_Unk_ov001_022da450(UnkWork_022dbf30 *w);
void MsgBox_Create(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);
void VsMenu_State_Unk_ov001_022dbe3c(void);
void VsMenu_Unk_ov001_022d9e50(UnkWork_022dbf30 *w, s32 x, s32 y);
void VsMenu_Unk_ov001_022d9e2c(UnkWork_022dbf30 *w, s32 x, s32 y);

#pragma thumb on
void VsMenu_State_Unk_ov001_022dbf30(void) {
    UnkWork_022dbf30 *w = gTaskCurrent->work;
    BOOL done;

    KeyframeAnim_Update(&w->anims[0]);
    KeyframeAnim_Update(&w->anims[1]);
    {
        UnkAnim_022dbf30 *a = &w->anims[0];
        VsMenu_Unk_ov001_022d9e50(w, a->x, a->y);
    }
    {
        UnkAnim_022dbf30 *a = &w->anims[1];
        VsMenu_Unk_ov001_022d9e2c(w, a->x, a->y);
    }
    done = TRUE;
    if (!KeyframeAnim_IsFinished(&w->anims[0])) {
        done = FALSE;
    }
    if (!KeyframeAnim_IsFinished(&w->anims[1])) {
        done = FALSE;
    }
    w->timer++;
    if (done) {
        w->timer = 0;
        VsMenu_Unk_ov001_022da450(w);
        MsgBox_Create(4, 3, 5, 1, 2, 0, 0xe, 0x200010);
        gTaskCurrent->fn = VsMenu_State_Unk_ov001_022dbe3c;
    }
}
#pragma thumb reset
