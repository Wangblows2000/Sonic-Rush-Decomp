#include <nitro/types.h>

typedef struct {
    u8 pad0[0x14];
    s32 unk14;
    u8 pad18[0x2E - 0x18];
    s16 unk2E;
    u8 pad30[4];
    s16 unk34;
    u8 pad36[0x58 - 0x36];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x68 - 0x5E];
    void (*unk68)(void);
    u8 pad6C[0x3E8 - 0x6C];
    void *unk3E8;
    u8 pad3EC[4];
    s32 unk3F0;
    s32 unk3F4;
    s32 unk3F8;
} UnkStruct_02351950;

void Player_State_Air(void);
void Player_ResetGimmickState(UnkStruct_02351950 *w);
void Player_SetAnimation(UnkStruct_02351950 *w, s32 a);

void Player_Action_Air(UnkStruct_02351950 *w, UnkStruct_02351950 *o, s32 x0, s32 y, s32 x1) {
    if (w->unk68 == Player_State_Air) {
        if (x0 - 0x100 < w->unk3F0) {
            w->unk3F0 = x0 - 0x100;
        }
        if (x1 + 0x100 > w->unk3F8) {
            w->unk3F8 = x1 + 0x100;
        }
        if (y < w->unk3F4 || w->unk3F4 == 0) {
            w->unk3F4 = y;
        }
        return;
    }
    if (w->unk14 < y && !(o->unk5C & 2)) {
        return;
    }
    if (w->unk14 > y && (o->unk5C & 2)) {
        return;
    }
    Player_ResetGimmickState(w);
    Player_SetAnimation(w, 0x2b);
    if (w->unk58 & 1) {
        w->unk2E = w->unk34;
    }
    w->unk58 |= 0x8000;
    w->unk5C |= 4;
    w->unk58 |= 0x10;
    w->unk58 &= ~0x80;
    w->unk3E8 = o;
    w->unk3F0 = x0 - 0x100;
    w->unk3F4 = y;
    w->unk3F8 = x1 + 0x100;
    w->unk68 = Player_State_Air;
}
