#include <nitro/types.h>

typedef struct UnkStruct_02346d10 {
    u8 pad[0x32c];
    u32 unk32C;
    u8 pad1[0x3ac - 0x330];
    u16 unk3AC;
    u8 pad2[0x3cd - 0x3ae];
    u8 unk3CD;
} UnkStruct_02346d10;

void Player_StopHighSpeed(UnkStruct_02346d10 *s) {
    s->unk3CD = 0;
    s->unk3AC = 0;
    s->unk32C &= ~0x100;
}
