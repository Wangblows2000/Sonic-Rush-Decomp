#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x44 - 8];
    u16 unk44;
} UnkSpr_022c9924;

typedef struct {
    u8 pad0[0x348];
    UnkSpr_022c9924 spr;
    u8 pad[0x3b8 - 0x348 - sizeof(UnkSpr_022c9924)];
    s32 anim;
} UnkObj_022c9924;

typedef struct {
    u32 timer;
    s32 state;
} UnkSub_022c9924;

typedef struct {
    u8 pad0[0x1ec];
    UnkSub_022c9924 sub;
    u8 pad1f4[0x3ac - 0x1f4];
    UnkObj_022c9924 *obj;
} UnkStruct_022c9924;

void AnimatorSprite_SetAnimation(UnkSpr_022c9924 *s, u16 id);
void AnimatorSprite_ProcessAnimation(UnkSpr_022c9924 *s, s32 a, s32 b);
void AnimatorSprite_Draw(UnkSpr_022c9924 *s);
void OptionsMenu_DrawSprite(UnkStruct_022c9924 *p, s32 x, s32 y, s32 a, s32 b);

#pragma thumb on
void OptionsMenu_Unk_ov001_022c9924(UnkStruct_022c9924 *p, s32 x, s32 y) {
    UnkObj_022c9924 *obj;
    UnkSub_022c9924 *sub;
    BOOL blink;
    UnkSpr_022c9924 *s;

    obj = p->obj;
    sub = &p->sub;
    blink = (sub->timer % 100) < 0x3c ? TRUE : FALSE;
    switch (sub->state) {
    case 0:
    case 1:
    case 2:
    case 3:
        s = &obj->spr;
        if (obj->anim != 0) {
            AnimatorSprite_SetAnimation(s, 0);
            obj->anim = 0;
        }
        s->x = x + 0xb0;
        s->y = y + 0x63;
        s->unk44 = 1;
        AnimatorSprite_ProcessAnimation(s, 0, 0);
        AnimatorSprite_Draw(s);
        OptionsMenu_DrawSprite(p, x, y, 6, 6);
        break;
    case 4:
        if (blink) {
            OptionsMenu_DrawSprite(p, x, y, 6, 0);
        }
        break;
    case 8:
        if (blink) {
            OptionsMenu_DrawSprite(p, x, y, 6, 4);
        }
        break;
    case 9:
    case 10:
        OptionsMenu_DrawSprite(p, x, y, 6, 5);
        break;
    case 5:
    case 6:
    case 7:
        return;
    default:
        return;
    }
    OptionsMenu_DrawSprite(p, x, y, 5, 0);
}
#pragma thumb reset
