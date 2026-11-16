#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2c];
    u32 flags;
    u8 pad30[0x80 - 0x30];
} UnkAnim_022ea458;

typedef struct {
    u8 pad0[4];
    s32 mode;
    u8 pad8[0x930 - 8];
    UnkAnim_022ea458 animA;
    u8 pad9b0[0xa98 - 0x9b0];
    UnkAnim_022ea458 animB;
    u8 padb18[0xb44 - 0xb18];
    u32 curA;
    u8 padb48[0xb58 - 0xb48];
    u32 curB;
} UnkStruct_022ea458;

void AnimatorSprite_SetAnimation(void *anim, u16 id);

#pragma thumb on
void TimeAttackMenu_Unk_ov001_022ea458(UnkStruct_022ea458 *p, s32 a, s32 b) {
    u16 id;

    if (a >= 7) {
        id = b * 2 + 1;
        if (p->curB != id) {
            AnimatorSprite_SetAnimation(&p->animB, id);
            p->curB = id;
        }
        p->animB.flags &= ~4;
        return;
    }
    switch (p->mode) {
    case 0:
    case 2:
        id = a * 4 + b * 2 + 1;
        break;
    case 1:
    case 3:
        id = a * 2 + 1;
        break;
    }
    if (p->curA != id) {
        AnimatorSprite_SetAnimation(&p->animA, id);
        p->curA = id;
    }
    p->animA.flags &= ~4;
}
#pragma thumb reset
