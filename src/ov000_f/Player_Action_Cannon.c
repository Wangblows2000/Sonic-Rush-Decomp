#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 x;
    s32 y;
} UnkStruct_02351018_Obj;

typedef struct UnkStruct_02351018 UnkStruct_02351018;

struct UnkStruct_02351018 {
    u8 pad0;
    u8 unk1;
    u8 pad2[8 - 2];
    u32 unk8;
    s32 unkC;
    s32 x;
    s32 y;
    u8 pad18[0x2E - 0x18];
    s16 unk2E;
    s16 unk30;
    u8 pad32[0x34 - 0x32];
    s16 unk34;
    u8 pad36[0x58 - 0x36];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x68 - 0x5E];
    void (*unk68)(UnkStruct_02351018 *w);
    u8 pad6C[0x324 - 0x6C];
    u8 unk324;
    u8 pad325[0x32C - 0x325];
    u32 unk32C;
    u32 unk330;
    u8 pad334[0x384 - 0x334];
    u8 unk384[4];
    u8 unk388[0x3E8 - 0x388];
    UnkStruct_02351018_Obj *unk3E8;
    u8 pad3EC[0x3F0 - 0x3EC];
    s32 unk3F0;
    u8 pad3F4[0x3F8 - 0x3F4];
    s32 unk3F8;
};

void Player_State_Cannon(UnkStruct_02351018 *w);
void Player_ResetGimmickState(UnkStruct_02351018 *w);
s32 FX_Sqrt(s32 v);
s32 FX_Atan2(s32 y, s32 x);
void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);

#define ABS_02351018(x) ((x) < 0 ? -(x) : (x))

void Player_Action_Cannon(UnkStruct_02351018 *w, UnkStruct_02351018_Obj *o) {
    s32 a;
    s32 av;
    s32 v;
    s32 a2;
    s32 v2;
    s32 dx;
    s32 dy;

    if (w->unk68 == Player_State_Cannon) {
        return;
    }
    Player_ResetGimmickState(w);
    w->unk3E8 = o;
    w->unk5C |= 4;
    w->unk68 = Player_State_Cannon;
    w->unk58 |= 0x2000;
    w->unk58 |= 0x100;
    w->unk32C &= ~7;
    w->unk32C |= 0x100000;
    w->unk32C |= 0x2000;
    w->unk330 |= 0x30;
    a = w->unk2E;
    if (a < 0) {
        a = -a;
    }
    w->unk3F0 = a;
    if (w->unk3F0 < ABS_02351018(w->unk34)) {
        w->unk3F0 = ABS_02351018(w->unk34);
    }
    if (w->unk3F0 < ABS_02351018(w->unk30)) {
        w->unk3F0 = ABS_02351018(w->unk30);
    }
    w->unk2E = 0;
    w->unk30 = 0;
    w->unk34 = 0;
    w->unk1 = 0;
    dx = (w->unk3E8->x - w->x) >> 8;
    if (dx < 0) {
        dx = -dx;
    }
    dy = (w->unk3E8->y - w->y) >> 8;
    if (dy < 0) {
        dy = -dy;
    }
    dx = dx * dx << 12;
    dy = dy * dy << 12;
    w->unkC = FX_Sqrt(dx + dy) >> 4;
    w->unk3F8 = FX_Atan2(w->unk3E8->y - w->y, w->unk3E8->x - w->x);
    w->unk1 = (w->unk3F8 & 0xffff) >> 8;
    w->unk8 = 0;
    if (w->unk324 != 1) {
        Sound_PlaySeqArcEx(w->unk384, 0, 0);
    } else {
        Sound_PlaySeqArcEx(w->unk384, 0, 0x11);
    }
    Sound_PlaySeqArcEx(w->unk388, 0, 0x60);
}
