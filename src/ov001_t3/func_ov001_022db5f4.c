#include <nitro/types.h>

typedef struct {
    s16 x;
    s16 y;
    u8 pad4[0x28];
} UnkAnim_022db5f4;

typedef struct {
    s32 timer;
    s32 unk4;
    u8 pad8[0xb44 - 8];
    UnkAnim_022db5f4 anims[14];
} UnkWork_022db5f4;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022db5f4 *work;
} UnkTask_022db5f4;

extern UnkTask_022db5f4 *gTaskCurrent;

void KeyframeAnim_Update(UnkAnim_022db5f4 *a);
BOOL KeyframeAnim_IsFinished(UnkAnim_022db5f4 *a);
void Task_Destroy(UnkTask_022db5f4 *t);
void func_ov001_022d9e50(UnkWork_022db5f4 *w, s32 x, s32 y);
void func_ov001_022d9e2c(UnkWork_022db5f4 *w, s32 x, s32 y);
void func_ov001_022d9b3c(UnkWork_022db5f4 *w, s32 i, s32 x, s32 y);

#pragma thumb on
void func_ov001_022db5f4(void) {
    UnkWork_022db5f4 *w;
    s32 i;
    BOOL done;
    UnkAnim_022db5f4 *a;

    w = gTaskCurrent->work;
    KeyframeAnim_Update(&w->anims[0]);
    KeyframeAnim_Update(&w->anims[1]);
    for (i = 0; i < 8; i++) {
        KeyframeAnim_Update(&w->anims[6 + i]);
    }
    done = TRUE;
    if (!KeyframeAnim_IsFinished(&w->anims[0])) done = FALSE;
    if (!KeyframeAnim_IsFinished(&w->anims[1])) done = FALSE;
    for (i = 0; i < 8 && done; i++) {
        if (!KeyframeAnim_IsFinished(&w->anims[6 + i])) done = FALSE;
    }
    if (done) {
        w->timer = 0;
        w->unk4 = 0;
        Task_Destroy(gTaskCurrent);
        return;
    }
    a = &w->anims[0];
    func_ov001_022d9e50(w, a->x, a->y);
    a = &w->anims[1];
    func_ov001_022d9e2c(w, a->x, a->y);
    for (i = 0; i < 8; i++) {
        a = &w->anims[6 + i];
        func_ov001_022d9b3c(w, i, a->x, a->y);
    }
    w->timer++;
}
#pragma thumb reset
