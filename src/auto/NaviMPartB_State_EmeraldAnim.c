/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gTaskCurrent[];
s32 AnimatorSprite_ProcessAnimation();
s32 AnimatorSprite_Draw();
s32 NaviMPartB_Draw();
void NaviMPartB_Main();
extern u8 gNaviMNewEmeraldMask[];

void NaviMPartB_State_EmeraldAnim(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    s32 t1 = (*(u32 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0x688);
    AnimatorSprite_ProcessAnimation((*(u32 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0x688), 0, 0, *(u32 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10));
    AnimatorSprite_Draw(t1);
    NaviMPartB_Draw(t0);
    if ((*(u32 *)((u8 *)t1 + 0x2c) & 2) == 0) {
        return;
    }
    *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)NaviMPartB_Main;
    *(u16 *)gNaviMNewEmeraldMask = 0;
    *(u32 *)((u8 *)t0 + 0xc) = -1;
}
