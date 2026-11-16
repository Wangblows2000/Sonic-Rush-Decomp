#include <ov000_b/stageobj.h>

typedef struct {
    s16 x;
    s16 y;
} UnkPt_0231b4b0;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x28c - 0x20c];
    UnkPt_0231b4b0 pt[16];
} UnkWork_0231b4b0;

void ScreenShake_Start(s32 a);
void Sound_PlaySeqArcEx(s32 a, s32 b, s32 c);
void WallBreak_State_Broken(void);
void WallBreak_DrawBroken(void);

void WallBreak_Action_Break(UnkWork_0231b4b0 *w, s32 a) {
    u8 i;

    ScreenShake_Start(3);
    w->base.unk68 = WallBreak_State_Broken;
    w->base.unk70 = WallBreak_DrawBroken;
    w->base.unkC = 0;
    w->base.unk54 |= 2;
    w->base.unk1B8 = NULL;
    for (i = 0; i < 16; i++) {
        if (((a << 15) >> 16) > 0)
            w->pt[i].x = ((a << 15) >> 16) - i * 16;
        else
            w->pt[i].x = ((a << 15) >> 16) + i * 16;
        w->pt[i].y = i * 0x80 - 0x500;
    }
    Sound_PlaySeqArcEx(0, 0, 0x4b);
}
