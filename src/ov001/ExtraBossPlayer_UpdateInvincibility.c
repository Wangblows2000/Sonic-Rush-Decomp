#include <nitro/types.h>

typedef struct { u32 flags; u8 pad[0xc]; s32 timer; } UnkStruct_02301dd4;

void ExtraBossPlayer_UpdateInvincibility(u8 *p) {
    UnkStruct_02301dd4 *a = *(UnkStruct_02301dd4 **)(p + 0x1dc);
    if (!(a->flags & 2)) {
        return;
    }
    a->timer--;
    if (a->timer <= 0) {
        a->flags &= ~2;
        *(u32 *)(p + 0x14) &= ~0x20000;
        *(u32 *)(p + 0xb8) &= ~0x200;
        return;
    }
    if (a->timer & 4) {
        *(u32 *)(p + 0x14) &= ~0x20000;
    } else {
        *(u32 *)(p + 0x14) |= 0x20000;
    }
}
