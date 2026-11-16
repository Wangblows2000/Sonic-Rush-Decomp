#include <nitro/types.h>

typedef struct {
    s16 unk0;
    u8 pad2[0x30 - 2];
} UnkStruct_02368ba0;

extern UnkStruct_02368ba0 data_ov000_02368ba0[];
void Player_State_MapObj40(void);
void Player_State_MapObj80(void);
s32 Player_ResetGimmickState();
void Player_SetAnimation(u8 *w, s32 a);
void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);

void Player_Action_MapObj40(u8 *w) {
    void *f = *(void **)(w + 0x68);

    if (f == Player_State_MapObj40)
        return;
    if (f == Player_State_MapObj80)
        return;
    if (!(*(u32 *)(w + 0x58) & 1))
        return;
    Player_ResetGimmickState(w);
    Player_SetAnimation(w, 0x26);
    *(u16 *)(w + 0x5c) |= 4;
    *(u32 *)(w + 0x58) &= ~0x10;
    *(u32 *)(w + 0x58) &= ~0x40000;
    *(u32 *)(w + 0x32c) |= 0x100000;
    *(void **)(w + 0x68) = Player_State_MapObj40;
    *(s16 *)(w + 0x3c) = data_ov000_02368ba0[*(u8 *)(w + 0x324)].unk0;
    *(u8 *)(w + 4) = 0xe;
    Sound_PlaySeqArcEx(w + 0x388, 0, 0x55);
}
