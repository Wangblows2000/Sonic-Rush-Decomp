#include <nitro/types.h>

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[6];
    s32 unk8;
    s32 unkC;
    u8 pad10[0x2E - 0x10];
    s16 unk2E;
    s16 unk30;
    u8 pad32[2];
    s16 unk34;
    u8 pad36[0x58 - 0x36];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x68 - 0x5E];
    void (*unk68)(void);
    u8 pad6C[0x32C - 0x6C];
    u32 unk32C;
    u8 pad330[0x384 - 0x330];
    u8 unk384[0x3E8 - 0x384];
    void *unk3E8;
    u8 pad3EC[4];
    s32 unk3F0;
    s32 unk3F4;
    u8 pad3F8[4];
    s32 unk3FC;
} UnkStruct_023516b8;

void Player_State_Swing(void);
void Player_ResetGimmickState(UnkStruct_023516b8 *w);
void Player_SetAnimation(UnkStruct_023516b8 *w, s32 a);
void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);

void Player_Action_Swing(UnkStruct_023516b8 *w, void *o, s32 unused, s32 r) {
    if (w->unk68 == Player_State_Swing) {
        return;
    }
    Player_ResetGimmickState(w);
    w->unk3E8 = o;
    Player_SetAnimation(w, 0x2c);
    w->unk5C |= 4;
    w->unk68 = Player_State_Swing;
    w->unk58 |= 0x10;
    w->unk58 |= 0x2000;
    w->unk58 |= 0x100;
    w->unk58 &= ~1;
    w->unk58 &= ~0x80;
    w->unk32C &= ~7;
    w->unk32C |= 0x100000;
    w->unk3F0 = w->unk34;
    w->unk2E = 0;
    w->unk30 = 0;
    w->unk8 = 0;
    w->unkC = 0;
    if (w->unk1 > 0x40 && w->unk1 < 0xc0) {
        w->unk32C |= 1;
        if (w->unk34 > 0) {
            w->unk3F0 = 0x10000;
        } else {
            w->unk3F0 = 0x20000;
        }
    } else {
        w->unk3F0 = 0x18000;
        if (w->unk34 < 0) {
            w->unk3F0 = 0x8000;
        }
        if (w->unk34 > 0x400) {
            w->unk34 = 0x400;
        }
        if (w->unk34 < -0x400) {
            w->unk34 = -0x400;
        }
        w->unkC = w->unk34;
    }
    w->unk3F4 = (w->unk1 << 8) + 0x10000;
    w->unk3FC = r << 8;
    Sound_PlaySeqArcEx(w->unk384, 0, 0x62);
}
