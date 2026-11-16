#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    u32 unk8;
    s32 unkC;
    s32 unk10;
    u8 pad14[0x58 - 0x14];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x67 - 0x5E];
    s8 unk67;
    void (*unk68)(void);
    u8 pad6C[0x328 - 0x6C];
    s16 unk328;
    s16 unk32A;
    u32 unk32C;
    u8 pad330[0x3F0 - 0x330];
    s32 unk3F0;
    s32 unk3F4;
    s32 unk3F8;
    u8 pad3FC[0x410 - 0x3FC];
    u8 unk410;
} UnkStruct_0235566c;

void Player_State_CorkscrewPath(void);
void Player_ResetGimmickState(UnkStruct_0235566c *w);
void Player_SetRunAnimationForSpeed(UnkStruct_0235566c *w);

void Player_Action_CorkscrewPath(UnkStruct_0235566c *w, s32 x, s32 y, s32 flags) {
    u8 save = w->unk410;
    s32 kind;

    if (w->unk68 == Player_State_CorkscrewPath) {
        return;
    }
    w->unk410 = 0;
    Player_ResetGimmickState(w);
    kind = flags >> 12;
    w->unk410 = save;
    if (kind != 2 && w->unk328 != 0x31) {
        Player_SetRunAnimationForSpeed(w);
    }
    w->unk5C |= 4;
    w->unk58 |= 0x10;
    w->unk58 |= 0x2000;
    w->unk32C &= ~3;
    w->unk3F0 = x;
    w->unk3F4 = y;
    w->unk3F8 = kind;
    w->unk8 = flags & 0xfff;
    w->unkC = 0;
    if (w->unk8 & 1) {
        if (w->unk3F0 > w->unk10) {
            w->unkC = w->unk3F0 - w->unk10;
        }
    } else {
        if (w->unk3F0 < w->unk10) {
            w->unkC = w->unk10 - w->unk3F0;
        }
    }
    w->unk3F4 -= w->unk67 << 8;
    if (w->unk3F8 == 1) {
        if (!(w->unk8 & 2)) {
            w->unk3F4 -= 0x400;
        }
    } else {
        w->unk8 |= 2;
    }
    w->unk68 = Player_State_CorkscrewPath;
    w->unk32A = 0x10;
}
