/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gTaskCurrent[];
s32 AnimatorSprite_ProcessAnimation();
s32 AnimatorSprite_Draw();
s32 func_02063890();
void func_02063c98();
extern u8 data_022c4dbc[];

void func_02063950(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    s32 t1 = (*(u32 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0x688);
    AnimatorSprite_ProcessAnimation((*(u32 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0x688), 0, 0, *(u32 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10));
    AnimatorSprite_Draw(t1);
    func_02063890(t0);
    if ((*(u32 *)((u8 *)t1 + 0x2c) & 2) == 0) {
        return;
    }
    *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)func_02063c98;
    *(u16 *)data_022c4dbc = 0;
    *(u32 *)((u8 *)t0 + 0xc) = -1;
}
