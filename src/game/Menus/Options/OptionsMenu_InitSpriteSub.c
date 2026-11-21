/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 ObjVram_Alloc();
s32 AnimatorSprite_Init();
s32 AnimatorSprite_SetAnimation();

void OptionsMenu_InitSpriteSub(s32 p0, u8 *p1, s32 p2) {
    s32 t0 = ObjVram_Alloc(1, *(u16 *)((u8 *)((s32)p1 + *(u32 *)((u8 *)p1 + 0x18)) + 0xa));
    AnimatorSprite_Init(p0, p1, p2, 0, 1, 0, t0, 0, 0x5000600);
    AnimatorSprite_SetAnimation(p0, p2);
}

#pragma thumb reset
