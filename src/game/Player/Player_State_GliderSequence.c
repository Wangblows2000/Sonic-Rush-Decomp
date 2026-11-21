#include <nitro/types.h>

typedef struct {
    u8 pad0[0xC];
    s32 unkC;
    u8 pad10[0x2AA - 0x10];
    u16 unk2AA;
} UnkStruct_0234ec78;

void Player_UpdateGliderSequence(UnkStruct_0234ec78 *w);

void Player_State_GliderSequence(UnkStruct_0234ec78 *w) {
    if (w->unk2AA & 0x100) {
        w->unkC++;
        if (w->unkC > 0x10) {
            w->unk2AA &= ~0x100;
        } else {
            return;
        }
    }
    Player_UpdateGliderSequence(w);
}
