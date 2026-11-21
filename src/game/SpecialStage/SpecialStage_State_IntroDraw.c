#include <nitro/types.h>

typedef struct UnkStruct_0230b594_o {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x48 - 8];
} UnkStruct_0230b594_o;

typedef struct UnkStruct_0230b594 {
    u8 pad0[0x18];
    void *unk18;
    u8 pad1C[0x2c - 0x1c];
    s32 timer;
    s32 unk30;
    s32 unk34;
    u8 pad38[0xf8 - 0x38];
    UnkStruct_0230b594_o objs[2];
    UnkStruct_0230b594_o o188;
} UnkStruct_0230b594;

typedef struct UnkStruct_0230b594_br {
    u8 pad0[4];
    u16 x;
    u16 y;
} UnkStruct_0230b594_br;

extern UnkStruct_0230b594 *gSpecialStageWork;
extern UnkStruct_0230b594_br gDisplayMain;

void SpecialStageCamera_Apply(void);
void AnimatorSprite_Draw(void *p);
void SpecialStagePlayer_Draw(void *p);
void SpecialStage_DrawFadeQuad(s32 n);

void SpecialStage_State_IntroDraw(void) {
    UnkStruct_0230b594 *g = gSpecialStageWork;
    UnkStruct_0230b594_o *o1 = &g->objs[0];
    UnkStruct_0230b594_o *o2 = &g->objs[1];
    s32 n;

    SpecialStageCamera_Apply();
    if (g->timer <= 0x14) {
        AnimatorSprite_Draw(o1);
    } else {
        AnimatorSprite_Draw(o2);
        gDisplayMain.x = g->unk30 - 10;
        gDisplayMain.y = 0xffe8;
    }
    if (g->unk34 > 0 && (g->unk34 & 0x20)) {
        g->o188.x = 0x12;
        g->o188.y = 0x4b;
        AnimatorSprite_Draw(&g->o188);
    }
    if (g->unk18 != NULL)
        SpecialStagePlayer_Draw(g->unk18);
    if (g->timer <= 0x5c)
        return;
    n = g->timer - 0x5c;
    if (n > 0x1f)
        n = 0x1f;
    SpecialStage_DrawFadeQuad(n);
}
