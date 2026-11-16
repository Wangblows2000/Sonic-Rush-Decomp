/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 StageObj_HandleCollider();

void BossFPartD8_UpdateColliderFromNode(s32 p0, u8 *p1, u8 *p2) {
    s32 t0 = *(u32 *)((u8 *)p1 + 0x18);
    if (t0 == 0) {
        return;
    }
    StageObj_HandleCollider(p0, p1, p2, t0);
    if ((s32)p2 == 0) {
        return;
    }
    *(u32 *)p1 = ((s32)(*(u32 *)((u8 *)p2 + 0x24) << 4) >> 0x10);
    *(u32 *)((u8 *)p1 + 0x4) = ((s32)(-*(u32 *)((u8 *)p2 + 0x28) << 4) >> 0x10);
    *(u16 *)((u8 *)p1 + 0x8) = ((s32)*(u32 *)((u8 *)p2 + 0x2c) >> 0xc);
}
