#include <nitro/types.h>

typedef struct {
    u16 pad0;
    u16 timer;
} UnkWork_0204dbb8;

typedef struct {
    u8 pad0[0x10];
    UnkWork_0204dbb8 *work;
} UnkTask_0204dbb8;

typedef struct {
    u8 pad0[0x10];
    u32 flags;
} UnkPad_0204dbb8;

extern UnkTask_0204dbb8 *gTaskCurrent;
extern UnkPad_0204dbb8 gTouchState;

void ScreenShake_Start(s32 a);
void BrightnessFade_Create(s32 a, s32 b);
s32 BrightnessFade_IsDone(void);
void Task_DestroyAll(void);
void SceneChange_ToSegaLogo(void);
s32 ScreenShake_GetOffsetX(void);
s32 ScreenShake_GetOffsetY(void);
void ObjDraw_SetGlobalOffset(s32 a, s32 b);

void GameOverSeq_Main(void) {
    UnkWork_0204dbb8 *w = gTaskCurrent->work;

    if (w->timer < 500) {
        if (((u16)(((*(vu16 *)0x04000130 | *(vu16 *)0x027fffa8) ^ 0x2fff) & 0x2fff) & 0xc0b) || (gTouchState.flags & 0x40))
            w->timer = 500;
    }
    if (w->timer == 0x168)
        ScreenShake_Start(2);
    if (w->timer == 0x170)
        ScreenShake_Start(4);
    if (w->timer == 0x200)
        BrightnessFade_Create(5, 0x100);
    w->timer++;
    if (w->timer > 0x200 && BrightnessFade_IsDone() != 0) {
        Task_DestroyAll();
        SceneChange_ToSegaLogo();
        return;
    }
    ObjDraw_SetGlobalOffset(ScreenShake_GetOffsetX(), ScreenShake_GetOffsetY());
}
