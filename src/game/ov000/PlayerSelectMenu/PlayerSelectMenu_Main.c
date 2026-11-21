#include <ov000_d/scene.h>

typedef struct {
    u16 cont;
    u8 pad2[2];
    u16 trg;
} UnkStruct_02359624_Pad;

extern UnkStruct_02359624_Pad gPad;

void PlayerSelectMenu_Action_Intro(s32 a);

#pragma thumb on

void PlayerSelectMenu_Main(void) {
    UnkStruct_Scene *w = gTaskCurrent->work;
    UnkStruct_ScenePos *q = &w->posB68;
    s32 x;

    if (w->unkC != 1 && w->unkC != 2 && (gPad.trg & 1) && w->unk0 >= 0x15) {
        w->unk340 = 1;
    }
    if (w->unk340) {
        w->unk0 = 0x1e;
        KeyframeAnim_SetToLastFrame(q);
    }
    KeyframeAnim_Update(q);
    PlayerSelectMenu_SetBgScale(w, q->unk4);
    if (w->unk0 >= 0x26) {
        w->unk330 += 0x20;
    }
    PlayerSelectMenu_DrawSubSpriteAtX(w, w->unk330);
    x = (s32)(w->unk0 << 9) / 0x1e - 0x200;
    if (x > 0) {
        x = 0;
    }
    if (x < -0x100) {
        x = -0x100;
    }
    PlayerSelectMenu_DrawBottomSpriteScaled(w, x, 0x1000);
    w->unk0++;
    if (KeyframeAnim_IsFinished(q)) {
        PlayerSelectMenu_SetBgScale(w, 0x1000);
        PlayerSelectMenu_Action_Intro(0);
    }
}

#pragma thumb reset
