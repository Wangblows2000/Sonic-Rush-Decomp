#include <game/Stage/stageobj_alt.h>

typedef struct {
    u8 pad0[8];
    s32 unk8;
    u8 padC[0x10 - 0xC];
    s32 unk10;
    s32 unk14;
    u8 pad18[0x44 - 0x18];
    s16 unk44;
    s16 unk46;
    u8 pad48[0x94 - 0x48];
    UnkStruct_Spr spr0;
    UnkStruct_Spr spr1;
    u8 pad124[0x134 - 0x124];
    s16 unk134[5];
    s16 unk13E;
    u32 unk140;
    u8 pad144[4];
    u32 unk148;
} UnkStruct_0232b598;

typedef struct {
    u8 pad0[0x10];
    UnkStruct_0232b598 *work;
} UnkStruct_0232b598_Task;

typedef struct {
    u8 pad0[0x20];
    u32 unk20;
} UnkStruct_0232b598_Sys;

extern UnkStruct_0232b598_Task *gTaskCurrent;
extern UnkStruct_0232b598_Sys gGameState;

void StageObj_Draw2DEx(s32 x, s32 y, u8 rot, void *w, void *a, void *b, s32 c);
s32 Hud_GetDigitGlyphVramB(s32 a);

void ScorePopup_Draw(void) {
    s16 i;
    UnkStruct_0232b598 *w = gTaskCurrent->work;
    BOOL on = FALSE;
    s32 s0;
    s32 s1;
    u32 sh;
    s32 off;
    u32 d;

    if (w->unk13E < -0x1000 && (gGameState.unk20 & 2))
        return;
    s0 = w->spr0.unk34;
    s1 = w->spr1.unk34;
    for (i = 4, sh = 16, off = 0x2800; i >= 0; sh -= 4, off -= 0xa00, i--) {
        if (w->unk134[i] <= 8 && (on || (w->unk140 & (0xf << sh)))) {
            on = TRUE;
            w->spr0.unk34 = s0;
            w->spr1.unk34 = s1;
            if (w->unk148 >= 2000) {
                w->unk44 = w->unk8;
                w->unk46 = w->unk8;
                if (gGameState.unk20 & 2) {
                    w->spr0.unk34 = Hud_GetDigitGlyphVramB(0);
                    w->spr1.unk34 = Hud_GetDigitGlyphVramB(1);
                }
            }
            d = (w->unk140 & (0xf << sh)) >> sh;
            w->spr0.unk34 += d << 7;
            w->spr1.unk34 += d << 7;
            StageObj_Draw2DEx(w->unk10 + (w->unk134[i] << 8) - off + 0x1800,
                          w->unk13E + (w->unk14 - (w->unk134[i] << 9)) - 0x1400, 0, w, &w->spr0, &w->spr1, 0);
        }
    }
    w->spr0.unk34 = s0;
    w->spr1.unk34 = s1;
}
