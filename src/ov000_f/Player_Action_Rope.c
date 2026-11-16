#include <nitro/types.h>

typedef struct UnkStruct_02354c00 UnkStruct_02354c00;

struct UnkStruct_02354c00 {
    u8 pad0;
    u8 unk1;
    u8 pad2[0x8 - 2];
    s32 unk8;
    s32 unkC;
    u8 pad10[0x2E - 0x10];
    s16 unk2E;
    s16 unk30;
    u8 pad32[0x34 - 0x32];
    s16 unk34;
    u8 pad36[0x58 - 0x36];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x68 - 0x5E];
    void (*unk68)(UnkStruct_02354c00 *w);
    u8 pad6C[0x32C - 0x6C];
    u32 unk32C;
    u8 pad330[0x384 - 0x330];
    u8 unk384[0x3E8 - 0x384];
    void *unk3E8;
    u8 pad3EC[0x3F0 - 0x3EC];
    s32 unk3F0;
    s32 unk3F4;
    s32 unk3F8;
    s32 unk3FC;
};

extern s16 data_0206e470[];

#define IDX_02354c00(a) ((s32)((a) & 0xffff) >> 4)
#define SIN_02354c00(a) data_0206e470[(u32)(IDX_02354c00(a) << 1)]
#define COS_02354c00(a) data_0206e470[(u32)((IDX_02354c00(a) << 1) + 1)]

void Player_ResetGimmickState(UnkStruct_02354c00 *w);
void Player_SetAnimation(UnkStruct_02354c00 *w, s32 state);
void Player_State_Rope(UnkStruct_02354c00 *w);
void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);

void Player_Action_Rope(UnkStruct_02354c00 *w, void *parent, s32 tx, s32 ty) {
    s16 spd = w->unk34;
    s32 t;

    Player_ResetGimmickState(w);
    w->unk3E8 = parent;
    Player_SetAnimation(w, 0x24);
    w->unk5C |= 4;
    w->unk68 = Player_State_Rope;
    w->unk58 |= 0x10;
    w->unk58 |= 0x8000;
    w->unk58 |= 0x4000;
    w->unk58 &= ~0x80;
    w->unk58 &= ~0x40;
    w->unk32C |= 0x100000;
    if (w->unk1 == 0x40 || w->unk1 == 0xc0) {
        if (w->unk34 > 0x900) {
            w->unk34 = 0x900;
        }
        if (w->unk34 < -0x900) {
            w->unk34 = -0x900;
        }
    }
    w->unk2E = (w->unk34 * COS_02354c00((u16)(w->unk1 << 8))) >> 12;
    w->unk30 = (w->unk34 * SIN_02354c00((u16)(w->unk1 << 8))) >> 12;
    t = (w->unk34 << 4) >> 8;
    w->unk3F0 = (t * t) << 1;
    if (w->unk3F0 > 0x8000) {
        w->unk3F0 = 0x8000;
    }
    if (w->unk1 == 0x40 || w->unk1 == 0xc0) {
        w->unk3F4 = w->unk30 & 0xffff;
        w->unk2E = 0;
        w->unk34 = 0;
    } else {
        w->unk3F4 = (w->unk2E << 16) | (w->unk30 & 0xffff);
    }
    if (spd < 0) {
        w->unk1 += 0x40;
    } else {
        w->unk1 -= 0x40;
    }
    w->unk3F8 = tx;
    w->unk3FC = ty;
    w->unk8 = 0;
    w->unkC = 0;
    Sound_PlaySeqArcEx(w->unk384, 0, 0x4d);
}
