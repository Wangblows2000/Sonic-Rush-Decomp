/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gTaskCurrent[];
s32 StageObj_HandleCollider();
s32 func_02067d58();

void Boss7PartD5_Collide(void) {
    if ((*(u32 *)((u8 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0x54) & 0xc) != 0) {
        return;
    }
    if (*(s16 *)((u8 *)(*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0x100) + 0xf0) > 0) {
        return;
    }
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    if (*(u32 *)((u8 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0x390) != 0) {
        StageObj_HandleCollider(t0, (t0 + 0x378));
    }
    if (*(u32 *)((u8 *)t0 + 0x3bc) != 0) {
        StageObj_HandleCollider(t0, (t0 + 0x3a4));
    }
    if (*(u32 *)((u8 *)t0 + 0x1b8) == 0) {
        return;
    }
    func_02067d58((t0 + 0x1b8));
}
