/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gTaskCurrent[];
extern u8 gBossStatus[];
void func_02044ba8();
s32 AnimatorSprite_Draw();

void func_02044c10(void) {
    s32 t0 = *(u16 *)((u8 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0x8);
    s32 t1 = *(u16 *)((u8 *)gBossStatus + 0x8);
    s32 t2 = *(u32 *)gTaskCurrent;
    s32 t3 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    s32 t4 = *(u32 *)((u8 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0x4);
    if (t0 != t1) {
        *(u32 *)((u8 *)t2 + 0x8) = (u32)func_02044ba8;
        *(u32 *)t3 = 2;
    }
    AnimatorSprite_Draw(((*(u16 *)((u8 *)t3 + 0x8) + 0x5) * 0x48 + (t4 + 0x10)), (*(u16 *)((u8 *)t3 + 0x8) + 0x5), (t4 + 0x10), t3);
}
