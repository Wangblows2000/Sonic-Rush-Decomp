#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    u16 unk4;
    u16 unk6;
} UnkSprite_02061288;

typedef struct {
    u8 pad0[0x20];
    s32 unk20;
} UnkCfg_02061288;

typedef struct {
    u8 pad0[0x6c];
    UnkCfg_02061288 *cfg;
    u8 pad70[0x14];
    UnkSprite_02061288 spr;
} UnkWork_02061288;

void AnimatorSprite_ProcessAnimation(UnkSprite_02061288 *s, s32 a, s32 b);
void AnimatorSprite_Draw(UnkSprite_02061288 *s);

#pragma thumb on
void TalkDemoScene_DrawPrompt(UnkWork_02061288 *w) {
    UnkSprite_02061288 *s = &w->spr;

    AnimatorSprite_ProcessAnimation(s, 0, 0);
    if (w->cfg->unk20 == 1)
        s->unk4 = 0xd2;
    else
        s->unk4 = 0x7e;
    s->unk6 = 0x8a;
    AnimatorSprite_Draw(s);
}
#pragma thumb reset
