/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gTaskCurrent[];
extern u8 data_022c4560[];
s32 StageObj_HandleCollider();

void Player_Collide(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    if ((*(u32 *)((u8 *)data_022c4560 + 0x18) & 0x20) != 0) {
        if (*(u8 *)((u8 *)t0 + 0x325) != 0) {
            StageObj_HandleCollider(t0, (t0 + 0x2f8));
            return;
        }
    }
    s32 t1 = (*(u16 *)((u8 *)(t0 + 0x200) + 0xd6) | 0x20);
    *(u16 *)((u8 *)(t0 + 0x200) + 0xd6) = t1;
    StageObj_HandleCollider(t0, (t0 + 0x2cc), (t0 + 0x200), t1);
    StageObj_HandleCollider(t0, (t0 + 0x2a0));
}
