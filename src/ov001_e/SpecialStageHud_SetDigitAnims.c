#include <nitro/types.h>

typedef struct UnkStruct_0230832c_o {
    u8 pad0[0x48];
} UnkStruct_0230832c_o;

typedef struct UnkStruct_0230832c {
    u8 pad0[0x270];
    UnkStruct_0230832c_o a;
    UnkStruct_0230832c_o b;
    UnkStruct_0230832c_o c;
} UnkStruct_0230832c;

extern UnkStruct_0230832c *gSpecialStageHud;

s32 SpecialStageHud_Unk_ov001_02314368(void);
void AnimatorSprite_SetAnimation(UnkStruct_0230832c_o *s, u16 index);

void SpecialStageHud_SetDigitAnims(s32 t) {
    UnkStruct_0230832c *g = gSpecialStageHud;
    s32 n = SpecialStageHud_Unk_ov001_02314368();
    s32 d100 = n / 100 % 10;
    s32 d10 = n / 10 % 10;
    s32 d1 = n % 10;

    switch (t) {
    case 0:
        AnimatorSprite_SetAnimation(&g->a, d100 + 0x1e);
        break;
    case 0x14:
        AnimatorSprite_SetAnimation(&g->b, d10 + 0x1e);
        break;
    case 0x28:
        AnimatorSprite_SetAnimation(&g->c, d1 + 0x1e);
        break;
    }
}
