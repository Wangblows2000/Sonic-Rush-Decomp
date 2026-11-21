#include <nitro/types.h>

typedef struct UnkStruct_023098cc_o {
    u8 pad0[0x48];
} UnkStruct_023098cc_o;

typedef struct UnkStruct_023098cc {
    u8 pad0[0x780];
    UnkStruct_023098cc_o objA;
    UnkStruct_023098cc_o objB;
    u8 pad810[0xaf0 - 0x810];
    s32 unkAF0;
} UnkStruct_023098cc;

extern UnkStruct_023098cc *gSpecialStageHud;

void AnimatorSprite_SetAnimation(UnkStruct_023098cc_o *s, u16 index);

void SpecialStageHud_ShowTrickRating(s32 n) {
    UnkStruct_023098cc *g = gSpecialStageHud;

    g->unkAF0 = 0x3c;
    switch (n) {
    case 3:
        AnimatorSprite_SetAnimation(&g->objB, 0xb);
        break;
    case 0:
        AnimatorSprite_SetAnimation(&g->objB, 4);
        break;
    case 1:
        AnimatorSprite_SetAnimation(&g->objB, 6);
        break;
    case 2:
        AnimatorSprite_SetAnimation(&g->objB, 8);
        break;
    default:
        AnimatorSprite_SetAnimation(&g->objB, 10);
        break;
    }
}
